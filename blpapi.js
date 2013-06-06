// Copyright (C) 2012 Bloomberg Finance L.P.

var EventEmitter = require('events').EventEmitter;
var util = require('util');
var path = require('path');
var blpapi = require(path.join(__dirname, '/build/Release/blpapijs'));

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
    function(sub, label) {
        return this.session.subscribe(sub, label);
    }
exports.Session.prototype.resubscribe =
    function(sub, label) {
        return this.session.resubscribe(sub, label);
    }
exports.Session.prototype.request =
    function(uri, name, request, cid, label) {
        return this.session.request(uri, name, request, cid, label);
    }
