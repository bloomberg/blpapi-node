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
