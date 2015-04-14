/// <reference path='../typings/tsd.d.ts' />

import util = require('util');
import mockery = require('mockery');
import should = require('should');
import MOCKBLPAPIJS = require('./lib/mock-blpapijs');
import BLPAPI = require('../blpapi');
should(true).ok;    // Added so ts won't get rid of should module

describe('blpapi', (): void => {
    var instructions: MOCKBLPAPIJS.IInstructions;
    var session: BLPAPI.Session;
    var blpapi: typeof BLPAPI;

    // Setup mock for blpapijs
    before((): void => {
        mockery.registerMock('./build/Release/blpapijs', MOCKBLPAPIJS);
        mockery.enable({
            useCleanCache: true,
            warnOnReplace: true,
            warnOnUnregistered: false
        });
        // import testing target blpapi.ts
        blpapi = require('../blpapi');
    });

    beforeEach((): void => {
        session = new blpapi.Session({});
        instructions = {};
        MOCKBLPAPIJS.INSTRUCTIONS = instructions;
    });

    describe('#start()', (): void => {
        it('should start correctly', (done: Function): void => {
            instructions.onStart = (mockSession: MOCKBLPAPIJS.Session): void => {
                mockSession.sendSessionStarted();
            };
            session.start().then((): void => {
                done();
            });
        });
        it('should not start', (done: Function): void => {
            instructions.onStart = (mockSession: MOCKBLPAPIJS.Session): void => {
                mockSession.sendSessionStartupFailure();
            };
            session.start().catch((err: Error): void => {
                done();
            });
        });
        it('should start correctly with callback', (done: Function): void => {
            instructions.onStart = (mockSession: MOCKBLPAPIJS.Session): void => {
                mockSession.sendSessionStarted();
            };
            session.start((): void => {
                done();
            });
        });
    });

    describe('#stop()', (): void => {
        it('should stop correctly', (done: Function): void => {
            instructions.onStop = (mockSession: MOCKBLPAPIJS.Session): void => {
                mockSession.sendSessionStopped();
            };
            session.stop().then((): void => {
                done();
            });
        });
        it('should stop correctly with callback', (done: Function): void => {
            instructions.onStop = (mockSession: MOCKBLPAPIJS.Session): void => {
                mockSession.sendSessionStopped();
            };
            session.stop((): void => {
                done();
            });
        });
        // Use function instead of lambda to avoid ts capturing "this" in a variable
        it('should timeout after 2000ms if session did not stop', function(done: Function): void {
            instructions.onStop = null;
            var EXPECTED_TIMEOUT: number = 2000;
            this.timeout(EXPECTED_TIMEOUT + 100);
            var timeout = setTimeout((): void => {
                                        done();
                                     },
                                     EXPECTED_TIMEOUT);
            session.stop()
                .then((): void => {
                    clearTimeout(timeout);
                    done(new Error('Timeout did not happen!'));
                })
                .catch((err: Error): void => {
                    done(err);
                });
        });
    });

    describe('#request()', (): void => {
        it('should respond correctly if data comes in 1 chunk', (done: Function): void => {
            instructions.onOpenService = (mockSession: MOCKBLPAPIJS.Session,
                                          uri: string,
                                          cid: number): void => {
                mockSession.sendServiceOpened(cid);
            };
            instructions.onRequest = (mockSession: MOCKBLPAPIJS.Session,
                                      responseName: string,
                                      cid: number): void => {
                mockSession.sendFinalRequestData(responseName, cid);
            };
            session.request('//blp/test',
                            'HistoricalDataRequest',
                            null,
                            (err: Error, data?: any, isFinal?: boolean): void => {
                                should.ifError(err);
                                data.should.eql('FinalTestData');
                                should(isFinal).be.true;
                                done();
                            });
        });
        it('should respond correctly if data comes in 3 chunks', (done: Function): void => {
            instructions.onOpenService = (mockSession: MOCKBLPAPIJS.Session,
                                          uri: string,
                                          cid: number): void => {
                mockSession.sendServiceOpened(cid);
            };
            instructions.onRequest = (mockSession: MOCKBLPAPIJS.Session,
                                      responseName: string,
                                      cid: number): void => {
                mockSession.sendPartialRequestData(responseName, cid);
                mockSession.sendPartialRequestData(responseName, cid);
                mockSession.sendFinalRequestData(responseName, cid);
            };
            var counter: number = 0;
            session.request('//blp/test',
                            'HistoricalDataRequest',
                            null,
                            (err: Error, data?: any, isFinal?: boolean): void => {
                                should.ifError(err);
                                if (++counter < 3) {
                                    data.should.eql('TestData');
                                    should(isFinal).be.false;
                                } else {
                                    data.should.eql('FinalTestData');
                                    should(isFinal).be.true;
                                    done();
                                }
                            });
        });
        it('should respond correctly if multiple requests come in', (done: Function): void => {
            instructions.onOpenService = (mockSession: MOCKBLPAPIJS.Session,
                                          uri: string,
                                          cid: number): void => {
                mockSession.sendServiceOpened(cid);
            };
            instructions.onRequest = (mockSession: MOCKBLPAPIJS.Session,
                                      responseName: string,
                                      cid: number): void => {
                mockSession.sendFinalRequestData(responseName, cid);
            };
            var counter: number = 0;
            session.request('//blp/test',
                            'HistoricalDataRequest',
                            null,
                            (err: Error, data?: any, isFinal?: boolean): void => {
                                should.ifError(err);
                                data.should.eql('FinalTestData');
                                should(isFinal).be.true;
                                if (++counter === 2) {
                                    done();
                                }
                            });
            session.request('//blp/test',
                            'IntradayTickRequest',
                            null,
                            (err: Error, data?: any, isFinal?: boolean): void => {
                                should.ifError(err);
                                data.should.eql('FinalTestData');
                                should(isFinal).be.true;
                                if (++counter === 2) {
                                    done();
                                }
                            });
        });
        it('should fail if service cannot be opened', (done: Function): void => {
            instructions.onOpenService = (mockSession: MOCKBLPAPIJS.Session,
                                          uri: string,
                                          cid: number): void => {
                mockSession.sendServiceOpenFailure(uri, cid);
            };
            session.request('//blp/test',
                            'HistoricalDataRequest',
                            null,
                            (err: Error, data?: any, isFinal?: boolean): void => {
                                should(err).not.be.empty;
                                err.name.should.eql('BlpApiError');
                                err.message.should.eql('//blp/test Service Fail to Open.');
                                done();
                            });
        });
        it('should fail if request name cannot be handled', (done: Function): void => {
            instructions.onOpenService = (mockSession: MOCKBLPAPIJS.Session,
                                          uri: string,
                                          cid: number): void => {
                mockSession.sendServiceOpened(cid);
            };
            session.request('//blp/test',
                            'HistoricDataRequest',
                            null,
                            (err: Error, data?: any, isFinal?: boolean): void => {
                                should(err).not.be.empty;
                                err.name.should.eql('AssertionError');
                                err.message.should.eql('Invalid request name.');
                                done();
                            });
        });
    });

    describe('#authenticate()', (): void => {
        it('should give back a token', (done: Function): void => {
            instructions.onOpenService = (mockSession: MOCKBLPAPIJS.Session,
                                          uri: string,
                                          cid: number): void => {
                if (uri === '//blp/apiauth') {
                    mockSession.sendServiceOpened(cid);
                }
            };
            instructions.onGenerateToken = (mockSession: MOCKBLPAPIJS.Session,
                                            cid: number): void => {
                mockSession.sendTokenGenerationSuccess(cid);
            };
            session.authenticate().then((token: string): void => {
                token.should.eql('abcdefghijklmn');
                done();
            });
        });
        it('should give back a token with optional callback', (done: Function): void => {
            instructions.onOpenService = (mockSession: MOCKBLPAPIJS.Session,
                                          uri: string,
                                          cid: number): void => {
                if (uri === '//blp/apiauth') {
                    mockSession.sendServiceOpened(cid);
                }
            };
            instructions.onGenerateToken = (mockSession: MOCKBLPAPIJS.Session,
                                            cid: number): void => {
                mockSession.sendTokenGenerationSuccess(cid);
            };
            session.authenticate((err: Error, token: any): void => {
                should.ifError(err);
                token.should.eql('abcdefghijklmn');
                done();
            });
        });
        it('should fail if token cannot be generated', (done: Function): void => {
            instructions.onOpenService = (mockSession: MOCKBLPAPIJS.Session,
                                          uri: string,
                                          cid: number): void => {
                if (uri === '//blp/apiauth') {
                    mockSession.sendServiceOpened(cid);
                }
            };
            instructions.onGenerateToken = (mockSession: MOCKBLPAPIJS.Session,
                                            cid: number): void => {
                mockSession.sendTokenGenerationFailure(cid);
            };
            session.authenticate().catch((err: Error): void => {
                should(err).not.be.empty;
                err.name.should.eql('BlpApiError');
                err.message.should.eql('Fail to generate token.');
                done();
            });
        });
        it('should fail if auth service cannot be opened', (done: Function): void => {
            instructions.onOpenService = (mockSession: MOCKBLPAPIJS.Session,
                                          uri: string,
                                          cid: number): void => {
                mockSession.sendServiceOpenFailure(uri, cid);
            };
            session.authenticate().catch((err: Error): void => {
                should(err).not.be.empty;
                err.name.should.eql('BlpApiError');
                err.message.should.eql('//blp/apiauth Service Fail to Open.');
                done();
            });
        });
    });

    describe('#authorize()', (): void => {
        it('should authorize given token', (done: Function): void => {
            instructions.onOpenService = (mockSession: MOCKBLPAPIJS.Session,
                                          uri: string,
                                          cid: number): void => {
                if (uri === '//blp/apiauth') {
                    mockSession.sendServiceOpened(cid);
                }
            };
            instructions.onSendAuthorizationRequest = (mockSession: MOCKBLPAPIJS.Session,
                                                       token: string,
                                                       cid: number): void => {
                if (token === 'abcdefghijklmn') {
                    mockSession.sendAuthorizationSuccess(cid);
                }
            };
            session.authorize('abcdefghijklmn').then((identity: BLPAPI.IIdentity): void => {
                should(identity).not.be.null;
                done();
            });
        });
        it('should authorize given token with optional callback', (done: Function): void => {
            instructions.onOpenService = (mockSession: MOCKBLPAPIJS.Session,
                                          uri: string,
                                          cid: number): void => {
                if (uri === '//blp/apiauth') {
                    mockSession.sendServiceOpened(cid);
                }
            };
            instructions.onSendAuthorizationRequest = (mockSession: MOCKBLPAPIJS.Session,
                                                       token: string,
                                                       cid: number): void => {
                if (token === 'abcdefghijklmn') {
                    mockSession.sendAuthorizationSuccess(cid);
                }
            };
            session.authorize('abcdefghijklmn', (err: Error, identity: any): void => {
                should.ifError(err);
                should(identity).not.be.null;
                done();
            });
        });
        it('should fail to authorize', (done: Function): void => {
            instructions.onOpenService = (mockSession: MOCKBLPAPIJS.Session,
                                          uri: string,
                                          cid: number): void => {
                if (uri === '//blp/apiauth') {
                    mockSession.sendServiceOpened(cid);
                }
            };
            instructions.onSendAuthorizationRequest = (mockSession: MOCKBLPAPIJS.Session,
                                                       token: string,
                                                       cid: number): void => {
                mockSession.sendAuthorizationFailure(cid);
            };
            session.authorize('abcdefghijklmn').catch((err: Error): void => {
                should(err).not.be.empty;
                err.name.should.eql('BlpApiError');
                err.message.should.eql('Fail to authorize.');
                done();
            });
        });
        it('should fail if auth service cannot be opened', (done: Function): void => {
            instructions.onOpenService = (mockSession: MOCKBLPAPIJS.Session,
                                          uri: string,
                                          cid: number): void => {
                mockSession.sendServiceOpenFailure(uri, cid);
            };
            session.authorize('abcdefghijklmn').catch((err: Error): void => {
                should(err).not.be.empty;
                err.name.should.eql('BlpApiError');
                err.message.should.eql('//blp/apiauth Service Fail to Open.');
                done();
            });
        });
    });

    describe('#subscribe()', (): void => {
        it('should subscribe successfully if 1 data event occurs', (done: Function): void => {
            instructions.onOpenService = (mockSession: MOCKBLPAPIJS.Session,
                                          uri: string,
                                          cid: number): void => {
                mockSession.sendServiceOpened(cid);
            };
            instructions.onSubscribe = (mockSession: MOCKBLPAPIJS.Session,
                                        subs: { event: string; cid: number }[]): void => {
                subs.forEach((sub: { event: string; cid: number }): void => {
                    mockSession.sendSubscriptionData(sub.event, sub.cid);
                });
            };
            var subscription = new BLPAPI.Subscription('ABC', ['P']);
            session.subscribe([subscription]);
            subscription.on('data', (data: any): void => {
                data.should.eql('TestData');
                done();
            });
        });
        it('should subscribe successfully if 3 data events occurs', (done: Function): void => {
            instructions.onOpenService = (mockSession: MOCKBLPAPIJS.Session,
                                          uri: string,
                                          cid: number): void => {
                mockSession.sendServiceOpened(cid);
            };
            instructions.onSubscribe = (mockSession: MOCKBLPAPIJS.Session,
                                        subs: { event: string; cid: number }[]): void => {
                subs.forEach((sub: { event: string; cid: number }): void => {
                    mockSession.sendSubscriptionData(sub.event, sub.cid);
                    mockSession.sendSubscriptionData(sub.event, sub.cid);
                    mockSession.sendSubscriptionData(sub.event, sub.cid);
                });
            };
            var counter: number = 0;
            var subscription = new BLPAPI.Subscription('ABC', ['P']);
            session.subscribe([subscription]);
            subscription.on('data', (data: any): void => {
                data.should.eql('TestData');
                if (++counter === 3) {
                    done();
                }
            });
        });
        it('should subscribe successfully for 2 subscriptions', (done: Function): void => {
            instructions.onOpenService = (mockSession: MOCKBLPAPIJS.Session,
                                          uri: string,
                                          cid: number): void => {
                mockSession.sendServiceOpened(cid);
            };
            instructions.onSubscribe = (mockSession: MOCKBLPAPIJS.Session,
                                        subs: { event: string; cid: number }[]): void => {
                subs.forEach((sub: { event: string; cid: number }): void => {
                    mockSession.sendSubscriptionData(sub.event, sub.cid);
                });
            };
            var counter: number = 0;
            var subscription1 = new BLPAPI.Subscription('ABC', ['P']);
            var subscription2 = new BLPAPI.Subscription('DEF', ['P']);
            session.subscribe([ subscription1, subscription2 ]);
            subscription1.on('data', (data: any): void => {
                data.should.eql('TestData');
                if (++counter === 2) {
                    done();
                }
            });
            subscription2.on('data', (data: any): void => {
                data.should.eql('TestData');
                if (++counter === 2) {
                    done();
                }
            });
        });
        it('should subscribe successfully for 2 different securities', (done: Function): void => {
            instructions.onOpenService = (mockSession: MOCKBLPAPIJS.Session,
                                          uri: string,
                                          cid: number): void => {
                mockSession.sendServiceOpened(cid);
            };
            instructions.onSubscribe = (mockSession: MOCKBLPAPIJS.Session,
                                        subs: { event: string; cid: number }[]): void => {
                subs.forEach((sub: { event: string; cid: number }): void => {
                    mockSession.sendSubscriptionData(sub.event, sub.cid);
                });
            };
            var counter: number = 0;
            var subscription1 = new BLPAPI.Subscription('//blp/mktdata', ['P']);
            var subscription2 = new BLPAPI.Subscription('//blp/mktvwap', ['P']);
            session.subscribe([ subscription1, subscription2 ]);
            subscription1.on('data', (data: any): void => {
                data.should.eql('TestData');
                if (++counter === 2) {
                    done();
                }
            });
            subscription2.on('data', (data: any): void => {
                data.should.eql('TestData');
                if (++counter === 2) {
                    done();
                }
            });
        });
        it('should subscribe successfully for multiple events', (done: Function): void => {
            instructions.onOpenService = (mockSession: MOCKBLPAPIJS.Session,
                                          uri: string,
                                          cid: number): void => {
                mockSession.sendServiceOpened(cid);
            };
            instructions.onSubscribe = (mockSession: MOCKBLPAPIJS.Session,
                                        subs: { event: string; cid: number }[]): void => {
                subs.forEach((sub: { event: string; cid: number }): void => {
                    mockSession.sendSubscriptionData(sub.event, sub.cid);
                });
            };
            var counter: number = 0;
            var subscription = new BLPAPI.Subscription('//blp/mktbar', ['P']);
            session.subscribe([ subscription ]);
            subscription.on('data', (data: any): void => {
                data.should.eql('TestData');
                if (++counter === 3) {
                    done();
                }
            });
        });
        it('should fail to subscribe if service cannot be opened', (done: Function): void => {
            instructions.onOpenService = (mockSession: MOCKBLPAPIJS.Session,
                                          uri: string,
                                          cid: number): void => {
                mockSession.sendServiceOpenFailure(uri, cid);
            };
            var subscription = new BLPAPI.Subscription('//blp/mktbar', ['P']);
            session.subscribe([ subscription ]).catch((err: Error): void => {
                should(err).not.be.empty;
                err.name.should.eql('BlpApiError');
                err.message.should.eql('//blp/mktbar Service Fail to Open.');
                done();
            });
        });
    });

    describe('#unsubscribe()', (): void => {
        it('should unsubscribe all successfully', (done: Function): void => {
            instructions.onOpenService = (mockSession: MOCKBLPAPIJS.Session,
                                          uri: string,
                                          cid: number): void => {
                mockSession.sendServiceOpened(cid);
            };
            var mSession: MOCKBLPAPIJS.Session;
            var sub: { event: string; cid: number };
            instructions.onSubscribe = (mockSession: MOCKBLPAPIJS.Session,
                                        subs: { event: string; cid: number }[]): void => {
                mSession = mockSession;
                sub = subs[0];
                mockSession.sendSubscriptionData(sub.event, sub.cid);
            };
            var subscription = new BLPAPI.Subscription('ABC', ['P']);
            session.subscribe([subscription]);
            subscription.once('data', (data: any): void => {
                data.should.eql('TestData');
                session.unsubscribe([subscription]);
                mSession.sendSubscriptionData(sub.event, sub.cid);
                subscription.once('data', (data: any): void => {
                    done(new Error('unsubscribe fails'));
                });
                setTimeout(done, 100);
            });
        });
        it('should unsubscribe part successfully', (done: Function): void => {
            instructions.onOpenService = (mockSession: MOCKBLPAPIJS.Session,
                                          uri: string,
                                          cid: number): void => {
                mockSession.sendServiceOpened(cid);
            };
            var mSession: MOCKBLPAPIJS.Session;
            var sub1: { event: string; cid: number };
            var sub2: { event: string; cid: number };
            instructions.onSubscribe = (mockSession: MOCKBLPAPIJS.Session,
                                        subs: { event: string; cid: number }[]): void => {
                mSession = mockSession;
                sub1 = subs[0];
                sub2 = subs[1];
                mockSession.sendSubscriptionData(sub1.event, sub1.cid);
            };
            var subscription1 = new BLPAPI.Subscription('ABC', ['P']);
            var subscription2 = new BLPAPI.Subscription('DEF', ['P']);
            session.subscribe([ subscription1, subscription2 ]);
            subscription1.on('data', (data: any): void => {
                data.should.eql('TestData');
                session.unsubscribe([subscription2]);
                mSession.sendSubscriptionData.bind(mSession, sub2.event, sub2.cid).should.throw();
                done();
            });
            subscription2.on('data', (data: any): void => {
                done(new Error('Unsubscribe fails'));
            });
        });
        it('should throw exception if unknown subscription passed in', (done: Function): void => {
            try {
                var subscription = new BLPAPI.Subscription('ABC', ['P']);
                session.unsubscribe([subscription]);
            } catch (err) {
                done();
            }
        });
    });

    describe('#sessionterminated', (): void => {
        it('should error pending request', (done: Function): void => {
            instructions.onOpenService = (mockSession: MOCKBLPAPIJS.Session,
                                          uri: string,
                                          cid: number): void => {
                mockSession.sendServiceOpened(cid);
            };
            instructions.onRequest = (mockSession: MOCKBLPAPIJS.Session,
                                      responseName: string,
                                      cid: number): void => {
                mockSession.terminateSession();
            };
            session.request('//blp/test',
                            'HistoricalDataRequest',
                            null,
                            (err: Error, data?: any, isFinal?: boolean): void => {
                                should(err).be.an.Error;
                            });
            session.on('SessionTerminated', (): void => {
                done();
            });
        });
        it('should error pending subscription', (done: Function): void => {
            instructions.onOpenService = (mockSession: MOCKBLPAPIJS.Session,
                                          uri: string,
                                          cid: number): void => {
                mockSession.sendServiceOpened(cid);
            };
            instructions.onSubscribe = (mockSession: MOCKBLPAPIJS.Session,
                                        subs: { event: string; cid: number }[]): void => {
                subs.forEach((): void => {
                    mockSession.terminateSession();
                });
            };
            var subscription = new BLPAPI.Subscription('ABC', ['P']);
            session.subscribe([subscription]);
            subscription.on('error', (err: Error): void => {
                should(err).be.an.Error;
            });
            session.on('SessionTerminated', (): void => {
                done();
            });
        });
    });
});
