// Copyright (C) 2012 Bloomberg Finance L.P.

var c = require('./Console.js');
var blpapi = require('blpapi');

var hp = c.getHostPort();
// Add 'authenticationOptions' key to session options if necessary.
var session = new blpapi.Session({ serverHost: hp.serverHost,
                                   serverPort: hp.serverPort });
var service_refdata = 1; // Unique identifier for refdata service

var seclist = ['AAPL US Equity'];

session.on('SessionStarted', function(m) {
    //c.log(m);
    session.openService('//blp/refdata', service_refdata);
});

session.on('ServiceOpened', function(m) {
    //c.log(m);
    // Check to ensure the opened service is the refdata service
    if (m.correlations[0].value == service_refdata) {
        // Request the long-form company name and option chain
        session.request('//blp/refdata', 'ReferenceDataRequest',
            { securities: seclist,
              fields: ['LONG_COMP_NAME', 'CHAIN_FULL']
            }, 100);
    }
});

var options = [];

session.on('ReferenceDataResponse', function(m) {
    //c.log(m);
    // At this point, m.correlations[0].value will equal:
    // 100 -> ReferenceDataResponse for option chain
    // 101 -> ReferenceDataResponse for LAST_PRICE of individual options
    if (m.correlations[0].value === 100) {
        var chain = m.data.securityData[0].fieldData.CHAIN_FULL;
        for (i in chain) {
            var strike = chain[i];
            var call = strike['Call Ticker'] + " Equity";
            var put = strike['Put Ticker'] + " Equity";
            options.push(call, put);
        }
        if (m.eventType === 'RESPONSE') {
            session.request('//blp/refdata', 'ReferenceDataRequest',
                { securities: options, fields: ['LAST_PRICE'] }, 101);
        }
    } else if (m.correlations[0].value === 101) {
        for (i in m.data.securityData) {
            var data = m.data.securityData[i];
            console.log(data.security, '=>', data.fieldData.LAST_PRICE);
        }
        if (m.eventType === 'RESPONSE') {
            session.stop();
        }
    }
});

session.on('SessionTerminated', function(m) {
    session.destroy();
});

session.start();
