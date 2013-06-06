// Copyright (C) 2012 Bloomberg Finance L.P.

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

session.on('ServiceOpened', function(m) {
    c.log(m);
    // Check to ensure the opened service is the mktdata service
    if (m.correlations[0].value == service_mktdata) {
        // Subscribe to market bars for each security
        session.subscribe([
            { security: seclist[0], correlation: 100,
              fields: ['LAST_PRICE', 'BID', 'ASK'] },
            { security: seclist[1], correlation: 101,
              fields: ['LAST_PRICE', 'BID', 'ASK'] }
        ]);
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
