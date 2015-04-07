
declare module "blpapijs" {
    
    import events = require('events');

    export interface ISessionOpts {
        serverHost?: string;
        serverPort?: number;
        authenticationOptions?: string;
    }

    export interface Subscription {
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

        subscribe(sub: Subscription[], identity?: IIdentity, label?: string): Session;

        resubscribe(sub: Subscription[], label?: string): Session;

        unsubscribe(sub: Subscription[], label?: string): Session;

        request(uri: string, name: string, request: any, cid: number,
                identity?: IIdentity, label?: string): number;

        createIdentity(): IIdentity;

        generateToken(cid: number): number;

        sendAuthorizationRequest(token: string, identity: IIdentity,
                                 cid: number): number;
    }
}