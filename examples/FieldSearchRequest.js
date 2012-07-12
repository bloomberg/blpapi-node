// Copyright (C) 2012 Bloomberg Finance L.P.

var c = require('./Console.js');
var blpapi = require('blpapi');

var hp = c.getHostPort();
var session = new blpapi.Session({ host: hp.host, port: hp.port });
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
