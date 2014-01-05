var c = require('./Console.js');
var blpapi = require('blpapi');

var hp = c.getHostPort();
// Add 'authenticationOptions' key to session options if necessary.
var session = new blpapi.Session({ serverHost: hp.serverHost,
                                   serverPort: hp.serverPort });
var service_refdata = 1; // Unique identifier for refdata service

var seclist = ['AAPL US Equity', 'VOD LN Equity'];

session.on('SessionStarted', function(m) {
    c.log(m);
    session.openService('//blp/refdata', service_refdata);
});

session.on('ServiceOpened', function(m) {
    c.log(m);
    // Check to ensure the opened service is the refdata service
    if (m.correlations[0].value == service_refdata) {
        // Request the long-form company name for each security
        session.request('//blp/refdata', 'ReferenceDataRequest',
            { securities: seclist, fields: ['LONG_COMP_NAME'] }, 100);
        // Request intraday tick data for each security, 10:30 - 14:30
        session.request('//blp/refdata', 'HistoricalDataRequest',
            { securities: seclist,
              fields: ['PX_LAST', 'OPEN'],
              startDate: "20120101",
              endDate: "20120301",
              periodicitySelection: "DAILY" }, 101);
    }
});

var responses = 0;
function checkStop() {
    responses++;
    // Signal stop once all final responses have been received
    if (responses == 2)
        session.stop();
}

session.on('ReferenceDataResponse', function(m) {
    c.log(m);
    // At this point, m.correlations[0].value will equal:
    // 100 -> ReferenceDataResponse for long-form company names
    if (m.correlations[0].value === 100 && m.eventType === 'RESPONSE')
        checkStop();
});

session.on('HistoricalDataResponse', function(m) {
    c.log(m);
    // At this point, m.correlations[0].value will equal:
    // 101 -> HistoricalDataResponse for both securities
    //
    // m.eventType == 'PARTIAL_RESPONSE' until finally
    // m.eventType == 'RESPONSE' to indicate there is no more data
    if (m.correlations[0].value === 101 && m.eventType === 'RESPONSE')
        checkStop();
});

session.on('SessionTerminated', function(m) {
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
