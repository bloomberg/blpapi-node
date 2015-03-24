/// <reference path="./typings/tsd.d.ts" />

'use strict';

import assert = require('assert');
import events = require('events');
import util = require('util');

import Promise = require('bluebird');
import debug = require('debug');
import _ = require('lodash');

import path = require('path');
/* tslint:disable:no-var-requires */
// TODO REVIEW: Not sure how this module can be imported in a
// 'TypeScript' fashion.
var blpapijs = require(path.join(__dirname, '/build/Release/blpapijs'));
/* tslint:enable:no-var-requires */

import createError = require('custom-error-generator');

// LOGGING
var trace = debug('blpapi:trace');
var log = debug('blpapi:debug');

interface IBlpApiSubscription {
    security: string;
    fields: string[];
    options?: any;
    correlation: number;
}

// PUBLIC TYPES

export interface ISessionOpts {
    serverHost?: string;
    serverPort?: number;
}

export interface IIdentity {
}


export interface IRequestCallback {
    (err: Error, data?: any, isFinal?: boolean): void;
}

export class Subscription extends events.EventEmitter {
    security: string;
    fields: string[];
    options: any = null;

    constructor(security: string, fields: string[], options?: any) {
        super();

        this.security = security;
        this.fields = fields;
        if (options) {
            this.options = options;
        }
    }

    private toJSON(): Object {
        var result: { security: string; fields: string[]; options?: any; } = {
            security: this.security,
            fields: this.fields
        };

        if (this.options) {
            result.options = this.options;
        }

        return result;
    }
}

export class BlpApiError implements Error {
    // STATIC DATA
    static NAME: string = 'BlpApiError';

    // DATA
    data: any;
    name: string;
    message: string;
    constructor(data: any) {
        this.data = data;
        this.name = BlpApiError.NAME;
        // Subscription errors have a description, other errors have a message.
        this.message = data.reason.message || data.reason.description;
    }
}

// CONSTANTS
var EVENT_TYPE = {
    RESPONSE:         'RESPONSE',
    PARTIAL_RESPONSE: 'PARTIAL_RESPONSE'
};

// Mapping of request types to response names to listen for.
// The strings are taken from section A of the BLPAPI Developer's Guide, and are organized by
// service.
var REQUEST_TO_RESPONSE_MAP: { [index: string]: string; } = {
    // //blp/refdata
    'HistoricalDataRequest': 'HistoricalDataResponse',
    'IntradayTickRequest':   'IntradayTickResponse',
    'IntradayBarRequest':    'IntradayBarResponse',
    'ReferenceDataRequest':  'ReferenceDataResponse',
    'PortfolioDataRequest':  'PortfolioDataResponse',
    'BeqsRequest':           'BeqsResponse',

    // //blp/apiflds
    'FieldInfoRequest':              'fieldResponse',
    'FieldSearchRequest':            'fieldResponse',
    'CategorizedFieldSearchRequest': 'categorizedFieldResponse',

    // //blp/instruments
    'instrumentListRequest': 'InstrumentListResponse',
    'curveListRequest':      'CurveListResponse',
    'govtListRequest':       'GovtListResponse',

    // //blp/tasvc
    'studyRequest':          'studyResponse',

    // //blp/apiauth
    'AuthorizationRequest':      'AuthorizationResponse',
    'AuthorizationTokenRequest': 'AuthorizationTokenResponse'
};

// Mapping of service URIs to the event names to listen to when subscribed to these services.
var SERVICE_TO_SUBSCRIPTION_EVENTS_MAP: { [uri: string]: string[]; } = {
    '//blp/mktdata':  ['MarketDataEvents'],
    '//blp/mktvwap':  ['MarketDataEvents'],
    '//blp/mktbar':   ['MarketBarStart', 'MarketBarUpdate', 'MarketBarEnd'],
    '//blp/pagedata': ['PageUpdate']
};

// Convert generic BLAPI errors caused by thrown C++ exceptions to error objects with
// a type that has the same name than the exception type.
var getError = (() => {
    var errorTypeNames = [
        'DuplicateCorrelationIdException',
        'InvalidStateException',
        'InvalidAggrgumentException',
        'InvalidConversionException',
        'IndexOutOfRangeException',
        'FieldNotFoundException',
        'NotFoundException',
        'UnknownErrorException',
        'UnsupportedOperationException'
    ];

    errorTypeNames.forEach((typeName: string): void => {
        exports[typeName] = createError(typeName, Error);
    });
    return (error: any): Error => {
        var typeName = error.typeName;
        if (typeName in exports) {
            return new exports[typeName](error.message);
        } else {
            return error;
        }
    };
})();

// Wrapper for BLPAPI function calls.
var invoke = function (func: any): any {
    try {
        return func.apply(this,
                          Array.prototype.slice.call(arguments, 1));
    } catch (err) {
        throw getError(err);
    }
};

function isObjectEmpty(obj: Object): boolean {
    return (0 === Object.getOwnPropertyNames(obj).length);
}

function securityToService(security: string): string {
    var serviceRegex = /^\/\/blp\/[a-z]+/;
    var match = serviceRegex.exec(security);
    // XXX: note that we shoud probably capture what the default service is to use when
    //      reading in the session options.  However, when not specified, it is
    //      '//blp/mktdata'.
    return match ? match[0] : '//blp/mktdata';
}

function subscriptionsToServices(subscriptions: Subscription[]): string[] {
    return _.chain(subscriptions).map((s: Subscription): string => {
        return securityToService(s.security);
    }).uniq().value();
}

export class Session extends events.EventEmitter {
    //ATTRIBUTES
    // TypeScript compiler needs this to allow "this['property-string']" type of access
    [index: string]: any;

    // DATA
    private session: any;
    private eventListeners: {[index: string]: {[index: number]: Function}} = {};
    private requests: {[index: string]: IRequestCallback} = {};
    private subscriptions: {[index: string]: Subscription} = {};
    private services: {[index: string]: Promise<void>} = {};
    private correlatorId: number = 0;
    private stopped: Promise<void> = null;

    // PRIVATE MANIPULATORS
    private nextCorrelatorId(): number {
        return this.correlatorId++;
    }

    private listen(eventName: string, expectedId: number, handler: Function): void {
        if (!(eventName in this.eventListeners)) {
            trace('Listener added: ' + eventName);
            this.on(eventName, ((eventName: string, m: any): void => {
                var correlatorId = m.correlations[0].value;
                assert(correlatorId in this.eventListeners[eventName],
                       'correlation id does not exist: ' + correlatorId);
                this.eventListeners[eventName][correlatorId](m);
            }).bind(this, eventName));

            this.eventListeners[eventName] = {};
        }
        this.eventListeners[eventName][expectedId] = handler;
    }

    private unlisten(eventName: string, correlatorId: number): void {
        delete this.eventListeners[eventName][correlatorId];
        if (isObjectEmpty(this.eventListeners[eventName])) {
            trace('Listener removed: ' + eventName);
            this.removeAllListeners(eventName);
            delete this.eventListeners[eventName];
        }
    }

    private openService(uri: string): Promise<void> {
        var thenable = this.services[uri] = this.services[uri] ||
                                            new Promise<void>((resolve: Function,
                                                               reject: Function): void => {
            trace('Opening service: ' + uri);
            var openServiceId = this.nextCorrelatorId();
            invoke.call(this.session, this.session.openService, uri, openServiceId);

            this.listen('ServiceOpened', openServiceId, (ev: any): void => {
                log('Service opened: ' + uri);
                trace(ev);
                this.unlisten('ServiceOpened', openServiceId);
                this.unlisten('ServiceOpenFailure', openServiceId);
                resolve();
            });

            this.listen('ServiceOpenFailure', openServiceId, (ev: any): void => {
                log('Service open failure' + uri);
                trace(ev);
                this.unlisten('ServiceOpened', openServiceId);
                this.unlisten('ServiceOpenFailure', openServiceId);
                delete this.services[uri];
                reject(new BlpApiError(ev.data));
            });
        }).bind(this); // end 'new Promise'

        return thenable;
    }

    private requestHandler(cb: IRequestCallback, m: any): void {
        var eventType = m.eventType;
        var isFinal = (EVENT_TYPE.RESPONSE === eventType);

        log(util.format('Response: %s|%d|%s',
                        m.messageType,
                        m.correlations[0].value,
                        eventType));
        trace(m);

        cb(null, m.data, isFinal);

        if (isFinal) {
            var correlatorId = m.correlations[0].value;
            var messageType = m.messageType;
            delete this.requests[correlatorId];
            this.unlisten(messageType, correlatorId);
        }
    }

    private sessionTerminatedHandler(ev: any): void {
        log('Session terminating');
        trace(ev);

        _([{prop: 'eventListeners', cleanupFn: (eventName: string): void => {
            this.removeAllListeners(eventName);
         }},
         {prop: 'requests', cleanupFn: (k: string): void => {
            this.requests[k](new Error('session terminated'));
         }},
         {prop: 'subscriptions', cleanupFn: (k: string): void => {
            this.subscriptions[k].emit('error', new Error('session terminated'));
         }}
       ]).forEach((table: {prop: string; cleanupFn: (s: string) => void}): void => {
            Object.getOwnPropertyNames(this[table.prop]).forEach((key: string): void => {
                table.cleanupFn(key);
            });
            this[table.prop] = null;
        });

        if (!this.stopped) {
            this.stopped = Promise.resolve();
        }

        // tear down the session
        invoke.call(this.session, this.session.destroy);
        this.session = null;

        // emit event to any listeners
        this.emit('SessionTerminated', ev.data);
        log('Session terminated');
    }

    private doRequest(uri: string,
                      requestName: string,
                      request: any,
                      execute: (correlatorId: number) => void,
                      callback: IRequestCallback): void
    {
        // TODO: Review all calls to 'validateSession' which throws bypassing
        // the expected error reporting mechanism.
        try {
            this.validateSession();
        } catch (ex) {
            callback(ex);
            return;
        }

        var correlatorId = this.nextCorrelatorId();
        this.requests[correlatorId] = callback;

        this.openService(uri).then((): void => {
            log(util.format('Request: %s|%d', requestName, correlatorId));
            trace(request);
            execute(correlatorId);
            assert(requestName in REQUEST_TO_RESPONSE_MAP,
                   util.format('Request, %s, not handled', requestName));
            this.listen(REQUEST_TO_RESPONSE_MAP[requestName],
                        correlatorId,
                        this.requestHandler.bind(this, callback));
        }).catch((ex: Error): void => {
            delete this.requests[correlatorId];
            callback(ex);
        });
    }

    // PRIVATE ACCESSORS
    private validateSession(): void {
        if (this.stopped) {
            throw new Error('session terminated');
        }
    }

    // CREATORS
    constructor(opts: ISessionOpts) {
        super();

        this.session = new blpapijs.Session(opts);
        this.once('SessionTerminated', this.sessionTerminatedHandler.bind(this));
        trace(opts);
        var that = this;
        this.session.emit = () => {
            that.emit.apply(that, arguments);
        };
    }

    // MANIPULATORS
    start(cb?: (err: any, value: any) => void): Promise<void> {
        this.validateSession();

        return new Promise<void>((resolve: Function, reject: Function): void => {
            trace('Starting session');
            invoke.call(this.session, this.session.start);

            var listener = (listenerName: string, handler: Function, ev: any): void => {
                this.removeAllListeners(listenerName);
                handler(ev.data);
            };

            this.once('SessionStarted',
                      listener.bind(this, 'SessionStartupFailure', (data: any): void => {
                          log('Session started');
                          trace(data);
                          resolve();
                      }));

            this.once('SessionStartupFailure',
                      listener.bind(this, 'SessionStarted', (data: any): void => {
                          log('Session start failure');
                          trace(data);
                          reject(new BlpApiError(data));
                      }));
        }).nodeify(cb);
    }

    stop(cb?: (err: any, value: any) => void): Promise<void> {
        return this.stopped = this.stopped ||
                              new Promise<void>((resolve: Function, reject: Function): void => {
            log('Stopping session');
            invoke.call(this.session, this.session.stop);
            this.once('SessionTerminated', (ev: any): void => {
                resolve();
            });
        }).nodeify(cb);
    }

    request(uri: string,
            requestName: string,
            request: any,
            callback: IRequestCallback,
            identity?: IIdentity,
            label?: string): void
    {
        var executeRequest = (correlatorId: number): void => {
            invoke.call(this.session,
                        this.session.request,
                        uri,
                        requestName,
                        request,
                        correlatorId,
                        identity,
                        label);
        };
        this.doRequest(uri, requestName, request, executeRequest, callback);
    }

    authorize(cb?: (err: any, value: any) => void): Promise<void> {
        return new Promise<void>((resolve: () => void, reject: (err: Error) => void): void => {
            var callback = (err: Error, data?: any, isFinal?: boolean): void => {
                if (err) {
                    reject(err);
                } else if (isFinal) {
                    resolve();
                }
            };
            var uri = '//blp/apiauth';
            var executeRequest = (correlatorId: number): void => {
                invoke.call(this.session, this.session.authorize, uri, correlatorId);
            };
            this.doRequest(uri,
                           'AuthorizationRequest', // requestName
                           null, // request
                           executeRequest,
                           callback);
        }).nodeify(cb);
    }

    authorizeUser(request: any, cb?: (err: any, value: any) => void): Promise<IIdentity> {
        return new Promise<IIdentity>((resolve: (identity: IIdentity) => void,
                                       reject: (err: Error) => void): void => {
            function callback(err: Error, data?: any, isFinal?: boolean): void {
                if (err) {
                    reject(err);
                } else {
                    var identity: IIdentity = null;
                    if (data.hasOwnProperty('identity')) {
                        identity = data.identity;
                    }
                    if (isFinal) {
                        if (identity) {
                            resolve(identity);
                        } else {
                            reject(new BlpApiError(data));
                        }
                    }
                }
            }
            var uri = '//blp/apiauth';
            var executeRequest = (correlatorId: number): void => {
                invoke.call(this.session, this.session.authorizeUser, request, correlatorId);
            };
            this.doRequest('//blp/apiauth', // uri
                           'AuthorizationRequest', // requestName
                           request,
                           executeRequest,
                           callback);
        }).nodeify(cb);
    }

    subscribe(subscriptions: Subscription[],
              identity?: IIdentity,
              label?: string,
              cb?: (err: any) => void): Promise<void>
    {
        this.validateSession();

        _.forEach(subscriptions, (s: Subscription, i: number): void => {
            // XXX: O(N) - not critical but note to use ES6 Map in the future
            var cid = _.findKey(this.subscriptions, (other: Subscription): boolean => {
                return s === other;
            });

            if (undefined !== cid) {
                throw new Error('Subscription already exists for index ' + i);
            }
        });

        var subs = _.map(subscriptions, (s: Subscription): IBlpApiSubscription => {
            var cid = this.nextCorrelatorId();

            // XXX: yes, this is a side-effect of map, but it is needed for performance
            //      reasons until ES6 Map is available
            this.subscriptions[cid] = s;

            var result: IBlpApiSubscription = {
                security: s.security,
                correlation: cid,
                fields: s.fields
            };

            if ('options' in s) {
                result.options = s.options;
            }

            return result;
        });

        return Promise.all(_.map(subscriptionsToServices(subscriptions),
                                 (uri: string): Promise<void> =>
        {
            return this.openService(uri);
        })).then((): void => {
            log('Subscribing to: ' + JSON.stringify(subscriptions));

            invoke.call(this.session,
                        this.session.subscribe,
                        subs,
                        identity,
                        label);

            _.forEach(subs, (s: IBlpApiSubscription): void => {
                var uri = securityToService(s.security);
                var cid = s.correlation;
                var userSubscription = this.subscriptions[cid];

                assert(uri in SERVICE_TO_SUBSCRIPTION_EVENTS_MAP,
                       util.format('Service, %s, not handled', uri));
                var events = SERVICE_TO_SUBSCRIPTION_EVENTS_MAP[uri];
                events.forEach((event: string): void => {
                    log('listening on event: ' + event + ' for cid: ' + cid);
                    this.listen(event, cid, (m: any): void => {
                        userSubscription.emit('data', m.data, s);
                    });
                });
            });
        }).catch((ex: Error): void => {
            _.forEach(subs, (s: IBlpApiSubscription): void => {
                var cid = s.correlation;
                delete this.subscriptions[cid];
            });
            throw ex;
        }).nodeify(cb);
    }

    unsubscribe(subscriptions: Subscription[], label?: string): void {
        this.validateSession();

        log('Unsubscribing: ' + JSON.stringify(subscriptions));

        _.forEach(subscriptions, (s: Subscription, i: number): void => {
            // XXX: O(N) - not critical but note to use ES6 Map in the future
            var cid = _.findKey(this.subscriptions, (other: Subscription): boolean => {
                return s === other;
            });

            if (undefined === cid) {
                throw new Error('Subscription not found at index ' + i);
            }
        });

        var cids = _.map(subscriptions, (s: Subscription): number => {
            // XXX: O(N) - not critical but note to use ES6 Map in the future
            var cid = _.findKey(this.subscriptions, (other: Subscription): boolean => {
                return s === other;
            });
            return _.parseInt(cid);
        });

        var subs = _.map(cids, (cid: number): IBlpApiSubscription => {
            return <IBlpApiSubscription>{
                security: ' ',
                correlation: cid,
                fields: []
            };
        });

        invoke.call(this.session, this.session.unsubscribe, subs, label);

        _.forEach(cids, (cid: number): void => {
            process.nextTick((): void => {
                this.subscriptions[cid].emit('end');
                delete this.subscriptions[cid];
            });
            // unlisten for events
            var service = securityToService(this.subscriptions[cid].security);
            assert(service in SERVICE_TO_SUBSCRIPTION_EVENTS_MAP,
                   util.format('Service, %s, not handled', service));
            var events = SERVICE_TO_SUBSCRIPTION_EVENTS_MAP[service];
            _.forEach(events, (e: string): void => {
                this.unlisten(e, cid);
            });
        });
    }
}

// Local variables:
// c-basic-offset: 4
// tab-width: 4
// indent-tabs-mode: nil
// End:
//
// vi: set shiftwidth=4 tabstop=4 expandtab:
// :indentSize=4:tabSize=4:noTabs=true:

// ----------------------------------------------------------------------------
// Copyright (C) 2014 Bloomberg L.P.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ----------------------------- END-OF-FILE ----------------------------------
