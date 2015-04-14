/// <reference path="./typings/tsd.d.ts" />

'use strict';

import assert = require('assert');
import events = require('events');
import util = require('util');
import path = require('path');
import Promise = require('bluebird');
import debug = require('debug');
import _ = require('lodash');
import createError = require('custom-error-generator');

// Import the binding layer module.
import blpapijs = require('./build/Release/blpapijs');

// Extend the Session type with the methods from EventEmitter so
// its instances can listen for events.
((target: any, source: any): void => {
    for (var k in source.prototype) {
        if (source.prototype.hasOwnProperty(k)) {
            target.prototype[k] = source.prototype[k];
        }
    }
})(blpapijs.Session, events.EventEmitter);

// LOGGING
var trace = debug('blpapi:trace');
var log = debug('blpapi:debug');

// PUBLIC TYPES

export type ISessionOpts = blpapijs.ISessionOpts;

export type IIdentity = blpapijs.IIdentity;

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
    'TokenGenerationRequest': 'TokenGenerationSuccess',
    'AuthorizationRequest':   'AuthorizationSuccess'
};

// Mapping of service URIs to the event names to listen to when subscribed to these services.
var SERVICE_TO_SUBSCRIPTION_EVENTS_MAP: { [uri: string]: string[]; } = {
    '//blp/mktdata':  ['MarketDataEvents'],
    '//blp/mktvwap':  ['MarketDataEvents'],
    '//blp/mktbar':   ['MarketBarStart', 'MarketBarUpdate', 'MarketBarEnd'],
    '//blp/pagedata': ['PageUpdate']
};

// Convert generic BLPAPI errors caused by thrown C++ exceptions to error objects with
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
    private session: blpapijs.Session;
    private eventListeners: {[index: string]: {[index: number]: Function}} = {};
    private requests: {[index: string]: IRequestCallback} = {};
    private subscriptions: {[index: string]: Subscription} = {};
    private services: {[index: string]: Promise<void>} = {};
    private correlatorId: number = 0;
    private stopped: Promise<void> = null;

    // PRIVATE MANIPULATORS
    private invoke(func: any, ...args: any[]): any
    {
        // Wrapper for BLPAPI function calls.
        try {
            return func.apply(this.session, args);
        } catch (err) {
            throw getError(err);
        }
    }

    private nextCorrelatorId(): number {
        return this.correlatorId++;
    }

    private listen(eventName: string, expectedId: number, handler: Function): void {
        if (!(eventName in this.eventListeners)) {
            trace('Listener added: ' + eventName);
            this.session.on(eventName, ((eventName: string, m: any): void => {
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
            this.session.removeAllListeners(eventName);
            delete this.eventListeners[eventName];
        }
    }

    private openService(uri: string): Promise<void> {
        var thenable = this.services[uri] = this.services[uri] ||
                                            new Promise<void>((resolve: Function,
                                                               reject: Function): void => {
            trace('Opening service: ' + uri);
            var openServiceId = this.nextCorrelatorId();
            this.invoke(this.session.openService, uri, openServiceId);

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
        var isFinal = (EVENT_TYPE.PARTIAL_RESPONSE !== eventType);

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
            this.session.removeAllListeners(eventName);
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
        this.invoke(this.session.destroy);
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

        this.session.once('SessionTerminated', this.sessionTerminatedHandler.bind(this));
        log('Session created');
        trace(opts);
        this.setMaxListeners(0);  // Remove max listener limit
    }

    // MANIPULATORS
    start(cb?: (err: any, value: any) => void): Promise<void> {
        this.validateSession();

        return new Promise<void>((resolve: Function, reject: Function): void => {
            trace('Starting session');
            this.invoke(this.session.start);

            var listener = (listenerName: string, handler: Function, ev: any): void => {
                this.session.removeAllListeners(listenerName);
                handler(ev.data);
            };

            this.session.once('SessionStarted',
                              listener.bind(this,
                                            'SessionStartupFailure', (data: any): void => {
                                  log('Session started');
                                  trace(data);
                                  resolve();
                              }));

            this.session.once('SessionStartupFailure',
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
            this.invoke(this.session.stop);
            this.session.once('SessionTerminated', (ev: any): void => {
                resolve();
            });
        }).nodeify(cb);
    }


    // 'request' function overloads.

    request(uri: string,
            requestName: string,
            request: any,
            callback: IRequestCallback): void;

    request(uri: string,
            requestName: string,
            request: any,
            identity: IIdentity,
            callback: IRequestCallback): void;

    request(uri: string,
            requestName: string,
            request: any,
            label: string,
            callback: IRequestCallback): void;

    request(uri: string,
            requestName: string,
            request: any,
            identity: IIdentity,
            label: string,
            callback: IRequestCallback): void;

    // 'request' function definition
    request(uri: string,
            requestName: string,
            request: any,
            arg3: Object | string | IRequestCallback,
            arg4?: string | IRequestCallback,
            arg5?: IRequestCallback): void
    {
        var args = Array.prototype.slice.call(arguments);
        // Get callback and remove it from array.
        var callback = args.splice(-1)[0];
        // Get optional identity and label arguments (idx 3 and 4 if present).
        var optionalArgs = args.slice(3);

        var executeRequest = (correlatorId: number): void => {
            var args = [this.session.request,
                        uri,
                        requestName,
                        request,
                        correlatorId].concat(optionalArgs);
            this.invoke.apply(this, args);
        };
        this.doRequest(uri, requestName, request, executeRequest, callback);
    }

    authenticate(cb?: (err: any, value: any) => void): Promise<string>
    {
        var correlatorId = this.nextCorrelatorId();

        return new Promise<string>((resolve: (token: string) => void,
                                    reject: (error: Error) => void): void =>
        {
            function callback(err: Error, data?: any): void
            {
                if (err) {
                    reject(err);
                } else {
                    if (data.hasOwnProperty('token')) {
                        resolve(data.token);
                    } else {
                        reject(new BlpApiError(data));
                    }
                }
            }

            var executeRequest = (correlatorId: number): void =>
            {
                this.invoke(this.session.generateToken, correlatorId);

                this.listen('TokenGenerationFailure',
                            correlatorId,
                            (m: any): void =>
                            {
                                this.unlisten('TokenGenerationFailure',
                                              correlatorId);
                                reject(new BlpApiError(m.data));
                            });
            };

            this.doRequest('//blp/apiauth',
                           'TokenGenerationRequest',
                           null /* request */,
                           executeRequest,
                           callback);
        }).nodeify(cb);
    }

    authorize(token: string,
              cb?: (err: any, value: any) => void): Promise<IIdentity>
    {
        var correlatorId = this.nextCorrelatorId();

        return new Promise<IIdentity>((resolve: (identity: IIdentity) => void,
                                       reject: (error: Error) => void): void =>
        {
            var identity = this.session.createIdentity();

            function callback(err: Error): void
            {
                if (err) {
                    reject(err);
                } else {
                    resolve(identity);
                }
            }

            var executeRequest = (correlatorId: number): void =>
            {
                this.invoke(this.session.sendAuthorizationRequest,
                            token,
                            identity,
                            correlatorId);

                this.listen('AuthorizationFailure',
                            correlatorId,
                            (m: any): void =>
                            {
                                this.unlisten('AuthorizationFailure',
                                              correlatorId);
                                reject(new BlpApiError(m.data));
                            });
            };

            this.doRequest('//blp/apiauth',
                           'AuthorizationRequest',
                           null /* request */,
                           executeRequest,
                           callback);
        }).nodeify(cb);
    }

    // 'subscribe' function overloads.

    subscribe(subscriptions: Subscription[],
              cb?: (err: any) => void): Promise<void>;

    subscribe(subscriptions: Subscription[],
              identity: IIdentity,
              cb?: (err: any) => void): Promise<void>;

    subscribe(subscriptions: Subscription[],
              label: string,
              cb?: (err: any) => void): Promise<void>;

    subscribe(subscriptions: Subscription[],
              identity: IIdentity,
              label: string,
              cb?: (err: any) => void): Promise<void>;

    // 'subscribe' function implementation.

    subscribe(subscriptions: Subscription[],
              arg1?: IIdentity | string | ((err: any) => void),
              arg2?: string | ((err: any) => void),
              arg3?: (err: any) => void): Promise<void>
    {
        this.validateSession();

        var args = Array.prototype.slice.call(arguments);
        var callback: (err: any) => void = null;
        // If callback is present it is the last argument.
        if (typeof args[args.length - 1] === 'function') {
            // Get callback argument and remove it from the array of arguments.
            callback = args.splice(-1)[0];
        }
        // Get identity and label optional arguments, if specified they will be at index
        // 1 and 2.
        var optionalArgs = args.slice(1);

        _.forEach(subscriptions, (s: Subscription, i: number): void => {
            // XXX: O(N) - not critical but note to use ES6 Map in the future
            var cid = _.findKey(this.subscriptions, (other: Subscription): boolean => {
                return s === other;
            });

            if (undefined !== cid) {
                throw new Error('Subscription already exists for index ' + i);
            }
        });

        var subs = _.map(subscriptions, (s: Subscription): blpapijs.ISubscription => {
            var cid = this.nextCorrelatorId();

            // XXX: yes, this is a side-effect of map, but it is needed for performance
            //      reasons until ES6 Map is available
            this.subscriptions[cid] = s;

            var result: blpapijs.ISubscription = {
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

            var fwdArgs: any[] = [this.session.subscribe, subs].concat(optionalArgs);
            this.invoke.apply(this, fwdArgs);

            _.forEach(subs, (s: blpapijs.ISubscription): void => {
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
            _.forEach(subs, (s: blpapijs.ISubscription): void => {
                var cid = s.correlation;
                delete this.subscriptions[cid];
            });
            throw ex;
        }).nodeify(callback);
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

        var subs = _.map(cids, (cid: number): blpapijs.ISubscription => {
            return <blpapijs.ISubscription>{
                security: ' ',
                correlation: cid,
                fields: []
            };
        });

        // Build argument list appending optional arguments passed by caller.
        var args: any[] = [this.session.unsubscribe, subs].
                          concat(Array.prototype.slice.call(arguments, 1));
        this.invoke.apply(this, args);

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
// Copyright (C) 2015 Bloomberg L.P.
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
