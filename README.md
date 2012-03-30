node-blpapi
===========

[Bloomberg Open API] binding for [Node.js].

Find source code in the [Github repository].

[Bloomberg Open API]: http://open.bloomberg.com
[Node.js]: http://nodejs.org
[Github repository]: https://github.com/bloomberg/node-blpapi

Dependencies
------------

This module requires:

+ Node.js version 0.6.x
+ 32-bit or 64-bit Linux
+ [Bloomberg BLPAPI C++ SDK] v3.x
+ Bloomberg Desktop API (DAPI), Server API (SAPI), or [B-PIPE] subscription

[Bloomberg BLPAPI C++ SDK]: http://open.bloomberg.com
[B-PIPE]: http://www.bloomberg.com/enterprise/enterprise_products/data_optimization/data_feeds

Building / Installation
-----------------------

To build the module, the Bloomberg BLPAPI C++ SDK must first be downloaded
and unpacked.  An option is passed to `node-waf` to instruct it where to
find the BLPAPI C++ SDK root directory:

    #> node-waf configure --blpapi /path/to/blpapi_cpp_dir
    #> node-waf build

Alternatively, the `BLPAPI_ROOT` environment variable can be set ahead of
time to point to the BLPAPI C++ SDK root directory:

    #> export BLPAPI_ROOT=/path/to/blpapi_cpp_dir
    #> node-waf configure build

The two files required to use these bindings are node-blpapi.js and
node-blpapi.node.

Usage
-----

The module design closely follows the BLPAPI SDK design, with slight
modifications in syntax for easier consumption in Javascript.  The SDK
developer's guide should serve as the main guide for the module's
functionality.

Full examples contained in the `examples` directory demonstrate how to
use most SDK functionality.  Full descriptions of all availabe requests,
responses, and options are contained within the BLPAPI API
[Developer Guide](http://open.bloomberg.com/pdf/blpapi-developers-guide.pdf).


### Opening A Session ###

    var blpapi = require('node-blpapi');
    var session = new blpapi.Session({ host: '127.0.0.1', port: 8194 });

    session.on('SessionStarted', function(m) {
        // ready for work
    });

### Opening A Subscription Service ###

    var service_id = 1;

    session.on('SessionStarted', function(m) {
        session.openService('//blp/mktdata', service_id);
    });

    session.on('ServiceOpened', function(m) {
        // m.correlations[0].value == service_id
        // ready for subscriptions
    });

### Subscribing To Market Data ###

    var securities = [
        { security: 'AAPL US Equity', correlation: 0, fields: ['LAST_TRADE'] },
        { security: 'GOOG US Equity', correlation: 1, fields: ['LAST_TRADE'] }
    ];

    session.on('ServiceOpened', function(m) {
        if (m.correlations[0].value == service_id) {
            session.subscribe(securities);
        }
    });

    session.on('MarketDataEvents', function(m) {
        if (m.data.hasOwnProperty('LAST_TRADE')) {
            console.log(securities[m.correlations[0].value].security,
                        'LAST_TRADE', m.data.LAST_TRADE);
            // outputs:
            // AAPL US Equity LAST_TRADE 600.00
            // AAPL US Equity LAST_TRADE 601.00
            // GOOG US Equity LAST_TRADE 650.00
            // ...
        }
    });

License
-------

MIT license. See license text in [LICENSE](https://github.com/bloomberg/node-blpapi/blob/master/LICENSE).
