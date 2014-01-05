var c = require('./Console.js');
var blpapi = require('blpapi');

var hp = c.getHostPort();
// Add 'authenticationOptions' key to session options if necessary.
var session = new blpapi.Session({ serverHost: hp.serverHost,
                                   serverPort: hp.serverPort });
var service_mktdata = 1; // Unique identifier for mktdata service

var seclist = ['AAPL US Equity', 'VOD LN Equity'];

session.on('SessionStarted', function(m) {
    c.log(m);
    session.openService('//blp/mktdata', service_mktdata);
});

var sublist = [
	{ security: seclist[0], correlation: 100, fields: ['LAST_PRICE', 'BID', 'ASK'] },
	{ security: seclist[1], correlation: 101, fields: ['LAST_PRICE', 'BID', 'ASK'] }
];

session.on('ServiceOpened', function(m) {
    c.log(m);
    // Check to ensure the opened service is the mktdata service
    if (m.correlations[0].value == service_mktdata) {
        // Subscribe to market bars for each security
        session.subscribe(sublist);

		setTimeout(function() {
			session.unsubscribe(sublist);
		}, 10000);
    }
});

session.on('MarketDataEvents', function(m) {
    c.log(m);
    // At this point, m.correlations[0].value will equal:
    // 100 -> MarketBarStart for AAPL US Equity
    // 101 -> MarketBarStart for VOD LN Equity
});

// Helper to put the console in raw mode and shutdown session on close
c.createConsole(session);

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
