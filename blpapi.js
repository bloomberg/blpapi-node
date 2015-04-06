var EventEmitter = require('events').EventEmitter;
var util = require('util');
var path = require('path');
var blpapi = require(path.join(__dirname, '/build/Release/blpapijs'));
var createCustomError = require('custom-error-generator');

// Note: When support for authorizing multiple identities was added, this
// added an optional identity parameter to functions that could be called on
// different identities. In all cases, this was added before the optional
// label. To avoid breaking existing callers, when these functions are called
// with the old number of arguments, they check the last argument and will
// accept either a label or identity.

var getError = function () {
    var errorTypeNames = [
        'DuplicateCorrelationIdException',
        'InvalidStateException',
        'InvalidArgumentException',
        'InvalidConversionException',
        'IndexOutOfRangeException',
        'FieldNotFoundException',
        'NotFoundException',
        'UnknownErrorException',
        'UnsupportedOperationException'
    ];

    errorTypeNames.forEach(function(typeName) {
        exports[typeName] = createCustomError(typeName, Error);
    });
    return function(error) {
        var typeName = error.typeName;
        if (typeName in exports) {
            return new exports[typeName](error.message);
        } else {
            return error;
        }
    }
}();

var invoke = function(func) {
    try {
        return func.apply(this, arguments[1]);
    } catch(err) {
        throw getError(err);
    }
};

exports.Session = function(args) {
    this.session = new blpapi.Session(args);
    var that = this;
    this.session.emit = function() {
        that.emit.apply(that, arguments);
    };
};
util.inherits(exports.Session, EventEmitter);

// Usage: no args
// Returns: this(Session Object)
exports.Session.prototype.start =
    function() {
        return invoke.call(this.session, this.session.start, arguments);
    }

// Usage: no args
// Returns: this(Session Object)
exports.Session.prototype.stop =
    function() {
        return invoke.call(this.session, this.session.stop, arguments);
    }

// Usage: no args
// Returns: this(Session Object)
exports.Session.prototype.destroy =
    function() {
        return invoke.call(this.session, this.session.destroy, arguments);
    }

// Usage: <uri>, <cid>
// Returns: cid
exports.Session.prototype.openService =
    function() {
        return invoke.call(this.session, this.session.openService, arguments);
    }

// Usage: <subscriptions>, [identity], [label]
// Returns: this(Session Object)
exports.Session.prototype.subscribe =
    function() {
        return invoke.call(this.session, this.session.subscribe, arguments);
    }

// Usage: <subscriptions>, [identity], [label]
// Returns: this(Session Object)
exports.Session.prototype.resubscribe =
    function() {
        return invoke.call(this.session, this.session.resubscribe, arguments);
    }

// Usage: <subscriptions>, [identity], [label]
// Returns: this(Session Object)
exports.Session.prototype.unsubscribe =
    function() {
        return invoke.call(this.session, this.session.unsubscribe, arguments);
    }

// Usage: <uri>, <request_name>, <request_body>, <cid>, [identity], [label]
// Returns: cid
exports.Session.prototype.request =
    function() {
        return invoke.call(this.session, this.session.request, arguments);
    }

// Usage: no args
// Returns: Identity
exports.Session.prototype.createIdentity =
    function() {
        return invoke.call(this.session, this.session.createIdentity, arguments);
    }        

// Usage: <cid>
// Returns: cid
exports.Session.prototype.generateToken =
    function() {
        return invoke.call(this.session, this.session.generateToken, arguments);
    }

// Usage: <token>, <identity>, <cid>
// Returns: cid
exports.Session.prototype.sendAuthorizationRequest =
    function() {
        return invoke.call(this.session, this.session.sendAuthorizationRequest, arguments);
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
