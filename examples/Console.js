// Copyright (C) 2012 Bloomberg Finance L.P.

var path = require('path');
var util = require('util');

// Utility for console apps monitoring market data.
//
// When the user hits <esc> or <ctrl-c>, stop the session and
// destroy it once the 'SessionTerminated' message is received.
// This is intended for console apps which continually monitor
// data and are not request/response based.
module.exports.createConsole = function(session) {
    session.on('SessionTerminated', function(m) {
        session.destroy();
        process.exit();
    });
};

// Utility to print command line usage.
module.exports.usage = function(extra) {
    console.log('Usage:',
                process.argv[0],
                path.basename(process.argv[1]), 
                '<host>[:<port>]', extra);
    process.exit(-1);
};

// Utility to extract host:port information from the command line.
module.exports.getHostPort = function() {
    if (process.argv.length < 3) {
        module.exports.usage('...');
    }
    var hp = /\b((?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?))\b(?::([\d]+))?/.exec(process.argv[2]);
    if (hp === null)
        module.exports.usage('...');
    var port = (hp[2] === undefined) ? 8194 : hp[2];
    return { serverHost: hp[1], serverPort: parseInt(port) };
};

// Utility for dumping full message contents to the console.
module.exports.log = function(m) {
    if (m)
        console.log(m.messageType, '=>',
                    (m ? util.inspect(m, true, null) : 'null'));
};
