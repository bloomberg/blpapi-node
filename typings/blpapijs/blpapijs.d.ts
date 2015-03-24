
declare module "blpapijs" {
    
    import events = require('events');
    
    export class Session extends events.EventEmitter
    {
        constructor(...args: any[]);

        start(): void;
        stop(): void;
        destroy(): void;
        authorize(uri: string, cid: number): void;
        authorizeUser(request: any, cid: number): void;
        openService(uri: string, cid: number): void;
        subscribe(sub: any[], arg2?: Object, arg3?: string): void;
        resubscribe(sub: any[], label?: string): void;
        unsubscribe(sub: any[], label?: string): void;
        request(uri: string, name: string, request: any, cid: number, identity?: Object, label?: string): void;
    }
}