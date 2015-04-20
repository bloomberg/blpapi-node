var c = require('./Console.js');
var blpapi = require('blpapi');

var hp = c.getHostPort();
// Add 'authenticationOptions' key to session options if necessary.
var session = new blpapi.Session({ serverHost: hp.serverHost,
                                   serverPort: hp.serverPort });

if (process.argv.length != 4) {
    c.usage('<text search>');
}
var search = process.argv[3];

session.start().then(sendRequest).catch(function (error) {
    console.log('Session start failure:', error);
    process.exit();
});

function sendRequest() {
    session.request('//blp/apiflds',
                    'FieldSearchRequest',
                    { searchSpec: search },
                    onResponse);
}

var results = 0;
var limit = 20;

function onResponse(error, data, isFinal) {
    if (error) {
        console.log('Received error:', error);
    } else if (results < limit) {
        for (var i in data.fieldData) {
            var fd = data.fieldData[i];
            console.log(fd.fieldInfo.mnemonic, "=>",
                        fd.fieldInfo.description);
            results++;
            if (results === limit)
                break;
        }
    }
    // Stop once the final response is received
    if (isFinal || error) {
        session.stop();
    }
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
