/// <reference path='../../typings/tsd.d.ts' />

import assert = require('assert');
import events = require('events');
import util = require('util');
import blpapijs = require('../../build/Release/blpapijs');

export interface IInstructions {
    onStart?: (session: blpapijs.Session) => void;
    onStop?: (session: blpapijs.Session) => void;
    onOpenService?: (session: blpapijs.Session, uri: string, cid: number) => void;
    onRequest?: (session: blpapijs.Session, responseName: string, cid: number) => void;
    onGenerateToken?: (session: blpapijs.Session, cid: number) => void;
    onSendAuthorizationRequest?: (session: blpapijs.Session, token: string, cid: number) => void;
    onSubscribe?: (session: blpapijs.Session, subs: { event: string; cid: number }[]) => void;
}

export var INSTRUCTIONS: IInstructions;

// Mapping of request types to response names to listen for.
// The strings are taken from section A of the BLPAPI Developer's Guide, and are organized by
// service.
// TODO: Move the const mapping into a separate file that could be shared with blpapi-wrapper
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
// TODO: Move the const mapping into a separate file that could be shared with blpapi-wrapper
var SERVICE_TO_SUBSCRIPTION_EVENTS_MAP: { [uri: string]: string[]; } = {
    '//blp/mktdata':  ['MarketDataEvents'],
    '//blp/mktvwap':  ['MarketDataEvents'],
    '//blp/mktbar':   ['MarketBarStart', 'MarketBarUpdate', 'MarketBarEnd'],
    '//blp/pagedata': ['PageUpdate']
};

// TODO: Move this function into a separate file that could be shared with blpapi-wrapper
function getServiceForSecurity(security: string): string
{
    var serviceRegex = /^\/\/blp\/[a-z]+/;
    var match = serviceRegex.exec(security);
    // XXX: note that we shoud probably capture what the default service is to use when
    //      reading in the session options.  However, when not specified, it is
    //      '//blp/mktdata'.
    return match ? match[0] : '//blp/mktdata';
}

// Mock Session object
export class Session extends events.EventEmitter implements blpapijs.Session {
    // PRIVATE MANIPULATORS
    private invoke(func: Function, ...args: any[]): void
    {
        if (func) {
            args.unshift(this);
            process.nextTick((): void => {
                func.apply(undefined, args);
            });
        }
    }

    // CREATORS
    constructor (args: blpapijs.ISessionOpts) {
        super();
    }

    // MANIPULATORS
    start(): Session {
        this.invoke(INSTRUCTIONS.onStart);
        return this;
    }

    stop(): Session {
        this.invoke(INSTRUCTIONS.onStop);
        return this;
    }

    destroy(): Session {
        return this;
    }

    openService(uri: string, cid: number): number {
        this.invoke(INSTRUCTIONS.onOpenService, uri, cid);
        return cid;
    }

    subscribe(subs: blpapijs.ISubscription[],
              identity?: blpapijs.IIdentity,
              label?: string): blpapijs.Session
    {
        var subscriptions: { event: string; cid: number; }[] = [];
        subs.forEach((sub: blpapijs.ISubscription): void => {
            var serviceUri = getServiceForSecurity(sub.security);
            assert(SERVICE_TO_SUBSCRIPTION_EVENTS_MAP[serviceUri],
                   'Invalid service name.');
            var events: string[] = SERVICE_TO_SUBSCRIPTION_EVENTS_MAP[serviceUri];
            events.forEach((e: string): void => {
                subscriptions.push({ event: e, cid: sub.correlation });
            });
        });
        this.invoke(INSTRUCTIONS.onSubscribe, subscriptions);
        return this;
    }

    resubscribe(subs: blpapijs.ISubscription[], label?: string): blpapijs.Session {
        throw new Error('Not yet implemented');
    }

    unsubscribe(subs: blpapijs.ISubscription[], label?: string): blpapijs.Session {
        return this;  //no-ops
    }

    request(uri: string,
            name: string,
            request: any,
            cid: number,
            identity?: blpapijs.IIdentity,
            label?: string): number
    {
        assert(REQUEST_TO_RESPONSE_MAP[name], 'Invalid request name.');
        var responseName = REQUEST_TO_RESPONSE_MAP[name];
        this.invoke(INSTRUCTIONS.onRequest, responseName, cid);
        return cid;
    }

    createIdentity(): blpapijs.IIdentity {
        return {};
    }

    generateToken(cid: number): number {
        this.invoke(INSTRUCTIONS.onGenerateToken, cid);
        return cid;
    }

    sendAuthorizationRequest(token: string,
                             identity: blpapijs.IIdentity,
                             cid: number): number
    {
        this.invoke(INSTRUCTIONS.onSendAuthorizationRequest, token, cid);
        return cid;
    }

    // The following methods are called from test cases
    terminateSession(): void {
        this.emit('SessionTerminated', { data: 'Session Terminated.' });
    }

    sendSessionStarted(): void {
        this.emit('SessionStarted', { data: 'Session Started.' });
    }

    sendSessionStartupFailure(): void {
        this.emit('SessionStartupFailure', {
            data: {
                reason: { description:  'Session Fail to Start.' }
            }
        });
    }

    sendSessionStopped(): void {
        this.emit('SessionTerminated', { data: 'Session Terminated.' });
    }

    sendServiceOpened(cid: number): void {
        this.emit('ServiceOpened', {
            correlations: [ { value: cid } ]
        });
    }

    sendServiceOpenFailure(uri: string, cid: number): void {
        this.emit('ServiceOpenFailure', {
            data: {
                reason: { description:  uri + ' Service Fail to Open.' }
            },
            correlations: [ { value: cid } ]
        });
    }

    sendPartialRequestData(responseName: string, cid: number): void {
        this.emit(responseName, {
            eventType: 'PARTIAL_RESPONSE',
            messageType: responseName,
            correlations: [ { value: cid } ],
            data: 'TestData'
        });
    }

    sendFinalRequestData(responseName: string, cid: number): void {
        this.emit(responseName, {
            eventType: 'RESPONSE',
            messageType: responseName,
            correlations: [ { value: cid } ],
            data: 'FinalTestData'
        });
    }

    sendTokenGenerationSuccess(cid: number): void {
        this.emit('TokenGenerationSuccess', {
            eventType: 'TOKEN_STATUS',
            messageType: 'TokenGenerationSuccess',
            correlations: [ { value: cid } ],
            data: { token: 'abcdefghijklmn' }
        });
    }

    sendTokenGenerationFailure(cid: number): void {
        this.emit('TokenGenerationFailure', {
            data: {
                reason: { description: 'Fail to generate token.' }
            },
            correlations: [ { value: cid } ]
        });
    }

    sendAuthorizationSuccess(cid: number): void {
        this.emit('AuthorizationSuccess', {
            eventType: 'RESPONSE',
            messageType: 'AuthorizationSuccess',
            correlations: [ { value: cid } ]
        });
    }

    sendAuthorizationFailure(cid: number): void {
        this.emit('AuthorizationFailure', {
            data: {
                reason: { description: 'Fail to authorize.' }
            },
            correlations: [ { value: cid } ]
        });
    }

    sendSubscriptionData(eventName: string, cid: number): void {
        this.emit(eventName, {
            correlations: [ { value: cid } ],
            data: 'TestData'
        });
    }
}
