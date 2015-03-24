
declare module "custom-error-generator" {
    function createError(name: string, parameters?: Object, Constructor?: any): any;
    module createError {
    }
    export = createError;
}
