/// <reference path="./typings/tsd.d.ts" />

import path = require('path');
import events = require('events');

// Somewhat hacky but needed to import the 'blpapijs' module.
process.env.NODE_PATH = path.join(__dirname, 'build/Release');
/* tslint:disable:no-var-requires */
require('module').Module._initPaths();
/* tslint:enable:no-var-requires */

import blpapijs = require('blpapijs');

// Expose this module as if it were the C++ binding layer module.
export = blpapijs;

// Extend the Session type with the methods from EventEmitter so
// its instances can listen for events.
((target: any, source: any): void => {
    for (var k in source.prototype) {
        if (source.prototype.hasOwnProperty(k)) {
            target.prototype[k] = source.prototype[k];
        }
    }
})(blpapijs.Session, events.EventEmitter);
