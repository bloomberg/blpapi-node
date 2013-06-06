// Copyright (C) 2013 Bloomberg Finance L.P.

var c = require('./Console.js');
var blpapi = require('blpapi');

var hp = c.getHostPort();

if (process.argv.length != 4) {
    c.usage('<application_name>');
}
var appname = process.argv[3];

var ao = 'AuthenticationMode=APPLICATION_ONLY;' +
         'ApplicationAuthenticationType=APPNAME_AND_KEY;' +
         'ApplicationName=' + appname;
var session = new blpapi.Session({ serverHost: hp.serverHost,
                                   serverPort: hp.serverPort,
                                   authenticationOptions: ao });
var service_apiauth = 1;   // Unique identifier for apiauth service
var request_apiauth = 100; // Unique identifier for apiauth request

session.on('SessionStarted', function(m) {
    session.openService('//blp/apiauth', service_apiauth);
});

session.on('ServiceOpened', function(m) {
    // Check to ensure the opened service is the apiauth service
    if (m.correlations[0].value == service_apiauth) {
        try {
            session.authorize('//blp/apiauth', request_apiauth);
        } catch(e) {
            console.log('Authorization request failure:', e);
            session.stop();
        }
    }
});

session.on('AuthorizationSuccess', function(m) {
    // Match correlation identifier used when authorizing
    if (m.correlations[0].value == request_apiauth) {
        console.log('Authorization successful.');
        session.stop();
    }
});

session.on('AuthorizationFailure', function(m) {
    // Match correlation identifier used when authorizing
    if (m.correlations[0].value == request_apiauth) {
        console.log('Authorization response failure.');
        session.stop();
    }
});

session.on('SessionTerminated', function(m) {
    // Once the session is stopped, release the event loop
    session.destroy();
});

session.start();
