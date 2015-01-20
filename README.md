[![build status](https://secure.travis-ci.org/bloomberg/blpapi-node.png)](http://travis-ci.org/bloomberg/blpapi-node)
blpapi-node
===========

[Bloomberg Open API] binding for [Node.js].

Find source code in the [Github repository].

[Bloomberg Open API]: http://www.bloomberglabs.com/api/about/
[Node.js]: http://nodejs.org
[Github repository]: https://github.com/bloomberg/blpapi-node

**Note:** This repository was renamed from `node-blpapi` to `blpapi-node`.

Dependencies
------------

This module requires:

+ Node.js version >= **0.6.x** (io.js >= **1.0.x** is supported)
+ Linux, Windows, or Mac OS X (32 or 64-bit)
+ GCC (Linux), MSVC++ (Windows), or Xcode (Mac OS X)
+ Bloomberg Desktop API (DAPI), Server API (SAPI), or [B-PIPE] subscription

This module includes:

+ [Bloomberg BLPAPI C++ SDK] v3.7.9.1 (Linux/Windows)
+ [Bloomberg BLPAPI C++ SDK] v3.8.1.1 (Mac OS X)

**Note:** Mac OS X users can only connect to SAPI or [B-PIPE] products.

[Bloomberg BLPAPI C++ SDK]: http://www.bloomberglabs.com/api/libraries/
[B-PIPE]: http://www.bloomberg.com/enterprise/enterprise_products/data_optimization/data_feeds

Installation
------------

From your project directory, run:

```
$ npm install blpapi
```

To install directly from github source, run:

```
$ npm install git://github.com/bloomberg/blpapi-node.git
```

This will download and build `blpapi` in `node_modules/`.

**Note:** Windows users using the Express version of Visual Studio may not
have the 64-bit compiler platform installed. If errors are seen related
to the `x64` platform not being found, please force a 32-bit arch before
invoking `npm` by running from the command shell:

```
> set npm_config_arch="ia32"
```

Usage
-----

The module design closely follows the BLPAPI SDK design, with slight
modifications in syntax for easier consumption in Javascript.  The SDK
developer's guide should serve as the main guide for the module's
functionality.

Full examples contained in the `examples` directory demonstrate how to
use most SDK functionality.  Full descriptions of all available requests,
responses, and options are contained within the BLPAPI API
[Developer Guide](http://www.bloomberglabs.com/api/documentation/).


### Opening A Session ###

    var blpapi = require('blpapi');
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

### Creating An Authorized Identity ###

Some session configurations, for example when connecting to a B-PIPE, may
require calls to `request` and `subscribe` to specify an authorized Identity.
The `authorizeUser` function performs an `AuthorizationRequest` on the
`//blp/apiauth`. This function differs slightly from the BLPAPI SDK design in
two ways. First, rather than having separate response events for success and
failure, it emits the `AuthorizationResponse` event for both. Second, the
`Identity` object is returned via the response as `data.identity`. This is only
set for successful authorization, so its presence or absence can be used to
determine whether the `AuthorizationResponse` indicates success or failure.
`data.identity` is an opaque object representing the authorized user. Its only
use is to be passed to `request` and `subscribe`.

    var auth_service_id = 2;
    var token_correlation_id = 3;
    var identity_correlation_id = 4;

    session.on('SessionStarted', function(m) {
        session.openService('//blp/apiauth', auth_service_id);
    });

    session.on('ServiceOpened', function(m) {
        if (m.correlations[0].value == auth_service_id) {
            // Request a token to be sent to you via MSG.
            session.request('//blp/apiauth', 'AuthorizationTokenRequest',
                { uuid: 12345678, label: 'testApp' }, token_correlation_id);
        }
    });

    session.on('AuthorizationTokenResponse', function(m) {
        if (m.correlations[0].value == token_correlation_id) {
            // Request the identity
            session.authorizeUser({ uuid: 12345678, token: 'token from MSG' },
                identity_correlation_id);
        }
    });

    session.on('AuthorizationResponse', function(m) {
        if (m.correlations[0].value == identity_correlation_id) {
            if (m.data.hasOwnProperty('identity') {
                // Authorization successful;
                // Save m.data.identity for use with later requests.
            }
        }
    });

### Using An Authorized Identity To Make A Request ###

    var identity;  // Assumed to be set by a previous AuthorizationResponse

    var refdata_service_id = 5;
    var refdata_correlation_id = 6;

    session.on('SessionStarted', function(m) {
        session.openService('//blp/refdata', refdata_service_id);
    });

    session.on('ServiceOpened', function(m) {
        if (m.correlations[0].value == refdata_service_id) {
            session.request('//blp/refdata', 'ReferenceDataRequest',
                { securities: ['IBM US Equity'], fields: [PX_LAST'] },
                refdata_correlation_id, identity);
        }
    });

    session.on('ReferenceDataResponse', function(m) {
        if (m.correlations[0].value == refdata_correlation_id) {
             console.log(m.data);
        }
    });

License
-------

MIT license. See license text in [LICENSE](https://github.com/bloomberg/blpapi-node/blob/master/LICENSE).
