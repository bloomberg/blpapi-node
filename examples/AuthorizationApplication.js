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

session.
    start().
    then(function () { authenticate(session); }).
    catch(function (error) {
        console.log('Session start failure:', error);
        process.exit();
    });

function authenticate(session) {
    session.authenticate().then(function (token) {
        console.log('Authentication successful');
        authorize(session, token);
    }).catch(function (err) {
        console.log('Authentication failure:', err);
        session.stop();
    });
}

function authorize(session, token) {
    session.authorize(token).then(function (identity) {
        console.log('Authorization successful.');
        session.stop();
    }).catch(function (err) {
        console.log('Authorization request failure:', err);
        session.stop();
    });
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
