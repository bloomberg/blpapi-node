// Copyright (C) 2012 Bloomberg Finance L.P.

var c = require('./Console.js');
var blpapi = require('blpapi');

var hp = c.getHostPort();
var session = new blpapi.Session({ host: hp.host, port: hp.port });
var service_mktbar = 1; // Unique identifier for mktbar service

var seclist = ['//blp/mktbar/ticker/AAPL US Equity',
               '//blp/mktbar/ticker/VOD LN Equity'];

session.on('SessionStarted', function(m) {
    c.log(m);
    session.openService('//blp/mktbar', service_mktbar);
});

session.on('ServiceOpened', function(m) {
    c.log(m);
    // Check to ensure the opened service is the mktbar service
    if (m.correlations[0].value == service_mktbar) {
        // Subscribe to market bars for each security
        session.subscribe([
            { security: seclist[0], correlation: 100,
              fields: ['LAST_PRICE'], options: { interval: 5 } },
            { security: seclist[1], correlation: 101,
              fields: ['LAST_PRICE'], options: { interval: 5 } }
        ]);
    }
});

session.on('MarketBarStart', function(m) {
    c.log(m);
    // At this point, m.correlations[0].value will equal:
    // 100 -> MarketBarStart for AAPL US Equity
    // 101 -> MarketBarStart for VOD LN Equity
});

session.on('MarketBarUpdate', function(m) {
    c.log(m);
    // At this point, m.correlations[0].value will equal:
    // 100 -> MarketBarUpdate for AAPL US Equity
    // 101 -> MarketBarUpdate for VOD LN Equity
});

session.on('MarketBarEnd', function(m) {
    c.log(m);
    // At this point, m.correlations[0].value will equal:
    // 100 -> MarketBarEnd for AAPL US Equity
    // 101 -> MarketBarEnd for VOD LN Equity
});

// Helper to put the console in raw mode and shutdown session on close
c.createConsole(session);

session.start();
