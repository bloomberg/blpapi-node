import events = require('events');

declare module blpapijs {

    interface ISessionOpts {
        serverHost?: string;
        serverPort?: number;
        authenticationOptions?: string;
    }

    interface ISubscription {
        security: string;
        fields: string[];
        options?: any;
        correlation: number;
    }

    interface IIdentity {
    }

    class Session extends events.EventEmitter
    {
        constructor(opts: ISessionOpts);

        start(): Session;

        stop(): Session;

        destroy(): Session;

        openService(uri: string, cid: number): number;

        subscribe(sub: ISubscription[], identity?: IIdentity, label?: string): Session;

        resubscribe(sub: ISubscription[], label?: string): Session;

        unsubscribe(sub: ISubscription[], label?: string): Session;

        request(uri: string,
                name: string,
                request: any,
                cid: number,
                identity?: IIdentity,
                label?: string): number;

        createIdentity(): IIdentity;

        generateToken(cid: number): number;

        sendAuthorizationRequest(token: string,
                                 identity: IIdentity,
                                 cid: number): number;
    }
}

export = blpapijs;
