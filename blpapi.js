var EventEmitter = require('events').EventEmitter;
var util = require('util');
var path = require('path');
var blpapi = require(path.join(__dirname, '/build/Release/blpapijs'));

// Note: When support for authorizing multiple identities was added, this
// added an optional identity parameter to functions that could be called on
// different identities. In all cases, this was added before the optional
// label. To avoid breaking existing callers, when these functions are called
// with the old number of arguments, they check the last argument and will
// accept either a label or identity.

exports.Session = function(args) {
    this.session = new blpapi.Session(args);
    var that = this;
    this.session.emit = function() {
        that.emit.apply(that, arguments);
    };
};
util.inherits(exports.Session, EventEmitter);

exports.Session.prototype.start =
    function() {
        return this.session.start();
    }
exports.Session.prototype.authorize =
    function(uri, cid) {
        return this.session.authorize(uri, cid);
    }
exports.Session.prototype.authorizeUser =
    function(request, cid) {
        return this.session.authorizeUser(request, cid);
    }
exports.Session.prototype.stop =
    function() {
        return this.session.stop();
    }
exports.Session.prototype.destroy =
    function() {
        return this.session.destroy();
    }
exports.Session.prototype.openService =
    function(uri, cid) {
        return this.session.openService(uri, cid);
    }
exports.Session.prototype.subscribe =
    function(sub, arg2, arg3) {
        var identity = arg2;
        var label = arg3;
        if ( 2 === arguments.length && typeof arg2 === 'string') {
            identity = undefined;
            label = arg2;
        }
        return this.session.subscribe(sub, identity, label);
    }
exports.Session.prototype.resubscribe =
    function(sub, label) {
        return this.session.resubscribe(sub, label);
    }
exports.Session.prototype.unsubscribe =
    function(sub, label) {
        return this.session.unsubscribe(sub, label);
    }
exports.Session.prototype.request =
    function(uri, name, request, cid, arg5, arg6) {
        var identity = arg5;
        var label = arg6;
        if (5 === arguments.length && typeof arg5 === 'string') {
            identity = undefined;
            label = arg5;
        }
        return this.session.request(uri, name, request, cid, identity, label);
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
