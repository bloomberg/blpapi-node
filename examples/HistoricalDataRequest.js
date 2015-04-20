var c = require('./Console.js');
var blpapi = require('blpapi');

var hp = c.getHostPort();
// Add 'authenticationOptions' key to session options if necessary.
var session = new blpapi.Session({ serverHost: hp.serverHost,
                                   serverPort: hp.serverPort });

var seclist = ['AAPL US Equity', 'VOD LN Equity'];

session.start().then(sendRequest).catch(function (error) {
        c.log(error);
});

function sendRequest() {
    // Request the long-form company name for each security
    session.request('//blp/refdata',
                    'ReferenceDataRequest',
                    { securities: seclist, fields: ['LONG_COMP_NAME'] },
                    onResponse);
    
    // Request intraday tick data for each security, 10:30 - 14:30
    session.request('//blp/refdata',
                    'HistoricalDataRequest',
                    { securities: seclist,
                      fields: ['PX_LAST', 'OPEN'],
                      startDate: "20120101",
                      endDate: "20120301",
                      periodicitySelection: "DAILY" },
                    onResponse);
}

var responses = 0;

function onResponse(error, data, isFinal) {
    if (error) {
        c.log(error);
        session.stop();
    } else {
        c.log(data);
        if (isFinal) {        
            // Signal stop once all final responses have been received
            if (++responses == 2) {
                session.stop();
            }
        }
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
