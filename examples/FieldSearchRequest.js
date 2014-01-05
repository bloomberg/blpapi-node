var c = require('./Console.js');
var blpapi = require('blpapi');

var hp = c.getHostPort();
// Add 'authenticationOptions' key to session options if necessary.
var session = new blpapi.Session({ serverHost: hp.serverHost,
                                   serverPort: hp.serverPort });
var service_apiflds = 1; // Unique identifier for apiflds service

if (process.argv.length != 4) {
    c.usage('<text search>');
}
var search = process.argv[3];

session.on('SessionStarted', function(m) {
    session.openService('//blp/apiflds', service_apiflds);
});

session.on('ServiceOpened', function(m) {
    // Check to ensure the opened service is the apiflds service
    if (m.correlations[0].value == service_apiflds) {
        session.request('//blp/apiflds', 'FieldSearchRequest',
            { searchSpec: search }, 100);
    }
});

var results = 0;
var limit = 20;

session.on('fieldResponse', function(m) {
    //c.log(m);
    // At this point, m.correlations[0].value will equal:
    // 100 -> FieldResponse for the free-form FieldSearchRequest
    if (m.correlations[0].value == 100) {
        if (results < limit) {
            for (var i in m.data.fieldData) {
                var fd = m.data.fieldData[i];
                console.log(fd.fieldInfo.mnemonic, "=>",
                            fd.fieldInfo.description);
                results++;
                if (results === limit)
                    break;
            }
        }
        // Stop once the final response is received
        if (m.eventType === 'RESPONSE') {
            session.stop();
        }
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
