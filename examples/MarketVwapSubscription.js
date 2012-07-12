// Copyright (C) 2012 Bloomberg Finance L.P.

var c = require('./Console.js');
var blpapi = require('blpapi');

var hp = c.getHostPort();
var session = new blpapi.Session({ host: hp.host, port: hp.port });
var service_mktvwap = 1; // Unique identifier for mktvwap service

var seclist = ['//blp/mktvwap/ticker/AAPL US Equity',
               '//blp/mktvwap/ticker/VOD LN Equity'];

session.on('SessionStarted', function(m) {
    c.log(m);
    session.openService('//blp/mktvwap', service_mktvwap);
});

session.on('ServiceOpened', function(m) {
    c.log(m);
    // Check to ensure the opened service is the mktvwap service
    if (m.correlations[0].value == service_mktvwap) {
        // Subscribe to market bars for each security
        session.subscribe([
            { security: seclist[0], correlation: 100,
              fields: ['LAST_PRICE', 'BID', 'ASK'],
              options: {
                  VWAP_START_TIME: '10:00',
                  VWAP_END_TIME: '16:00'
              } },
            { security: seclist[1], correlation: 101,
              fields: ['LAST_PRICE', 'BID', 'ASK'],
              options: {
                  VWAP_START_TIME: '10:00',
                  VWAP_END_TIME: '16:00'
              } }
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
