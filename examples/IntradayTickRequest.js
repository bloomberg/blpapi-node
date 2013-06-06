// Copyright (C) 2012 Bloomberg Finance L.P.

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
        // Request intraday tick data for each security, 10:30 - 11:00
        session.request('//blp/refdata', 'IntradayTickRequest',
            { security: seclist[0],
              eventTypes: ['TRADE'],
              startDateTime: new Date(2012, 2, 1, 10, 30, 0, 0),
              endDateTime: new Date(2012, 2, 1, 11, 00, 0, 0) }, 101);
        session.request('//blp/refdata', 'IntradayTickRequest',
            { security: seclist[1],
              eventTypes: ['TRADE'],
              startDateTime: new Date(2012, 2, 1, 10, 30, 0, 0),
              endDateTime: new Date(2012, 2, 1, 11, 00, 0, 0) }, 102);
        // NOTE: Intraday tick data is only available for a limited
        //       historical window. Adjust the YYYY-MM-DD values if
        //       running this example in the far future. No data will
        //       be returned if the date is > 140 days in the past.
    }
});

var responses = 0;
function checkStop() {
    responses++;
    // Signal stop once all final responses have been received
    if (responses == 3)
        session.stop();
}

session.on('ReferenceDataResponse', function(m) {
    c.log(m);
    // At this point, m.correlations[0].value will equal:
    // 100 -> ReferenceDataResponse for long-form company names
    if (m.correlations[0].value === 100 && m.eventType === 'RESPONSE')
        checkStop();
});

session.on('IntradayTickResponse', function(m) {
    c.log(m);
    // At this point, m.correlations[0].value will equal:
    // 101 -> IntradayTickResponse for AAPL US Equity
    // 102 -> IntradayTickResponse for VOD LN Equity
    //
    // m.eventType == 'PARTIAL_RESPONSE' until finally
    // m.eventType == 'RESPONSE' to indicate there is no more data
    if ((m.correlations[0].value === 101 ||
         m.correlations[0].value === 102) &&
        m.eventType === 'RESPONSE')
        checkStop();
});

session.on('SessionTerminated', function(m)  {
    session.destroy();
});

session.start();
