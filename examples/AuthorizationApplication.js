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
var request_token = 50;    // Unique identifier for generate token request
var request_apiauth = 100; // Unique identifier for authorization request

session.on('SessionStarted', function(m) {
    session.openService('//blp/apiauth', service_apiauth);
});

session.on('ServiceOpened', function(m) {
    // Check to ensure the opened service is the apiauth service
    if (m.correlations[0].value == service_apiauth) {
        session.generateToken(request_token);
    }
});

session.on('TokenGenerationSuccess', function(m) {
    // Match correlation identifier used when generating token
    if (m.correlations[0].value == request_token) {
        var token = m.data.token;
        var identity = session.createIdentity();
        session.sendAuthorizationRequest(token, identity, request_apiauth);
    }
});

session.on('TokenGenerationFailure', function(err) {
    // Match correlation identifier used when generating token
    if (m.correlations[0].value == request_token) {
        console.log('Token generation error');
        session.stop();
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
