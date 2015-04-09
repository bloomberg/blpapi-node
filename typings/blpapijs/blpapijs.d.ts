
declare module "blpapijs" {
    
    import events = require('events');

    export interface ISessionOpts {
        serverHost?: string;
        serverPort?: number;
        authenticationOptions?: string;
    }

    export interface ISubscription {
        security: string;
        fields: string[];
        options?: any;
        correlation: number;
    }

    export interface IIdentity {
    }

    export class Session extends events.EventEmitter
    {
        constructor(opts: ISessionOpts);

        start(): Session;

        stop(): Session;

        destroy(): Session;

        openService(uri: string, cid: number): number;

        subscribe(sub: ISubscription[], identity?: IIdentity, label?: string): Session;

        resubscribe(sub: ISubscription[], label?: string): Session;

        unsubscribe(sub: ISubscription[], label?: string): Session;

        request(uri: string, name: string, request: any, cid: number,
                identity?: IIdentity, label?: string): number;

        createIdentity(): IIdentity;

        generateToken(cid: number): number;

        sendAuthorizationRequest(token: string, identity: IIdentity,
                                 cid: number): number;
    }
}