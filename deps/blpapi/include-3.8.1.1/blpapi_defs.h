/* Copyright 2012. Bloomberg Finance L.P.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:  The above
 * copyright notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */
// blpapi_defs.h                                                      -*-C++-*-
#ifndef INCLUDED_BLPAPI_DEFS
#define INCLUDED_BLPAPI_DEFS

//@PURPOSE: Common definitions used by the library.
//
//@DESCRIPTION: This component provides a list of preprocessors that are used
// by the library.

#include <limits.h>

#define BLPAPI_CORRELATION_TYPE_UNSET   0
#define BLPAPI_CORRELATION_TYPE_INT     1
#define BLPAPI_CORRELATION_TYPE_POINTER 2
#define BLPAPI_CORRELATION_TYPE_AUTOGEN 3
#define BLPAPI_CORRELATION_MAX_CLASS_ID ((1 << 16)-1)

#define BLPAPI_MANAGEDPTR_COPY    1
#define BLPAPI_MANAGEDPTR_DESTROY (-1)


#define BLPAPI_DATETIME_YEAR_PART         0x1
#define BLPAPI_DATETIME_MONTH_PART        0x2
#define BLPAPI_DATETIME_DAY_PART          0x4
#define BLPAPI_DATETIME_OFFSET_PART       0x8
#define BLPAPI_DATETIME_HOURS_PART        0x10
#define BLPAPI_DATETIME_MINUTES_PART      0x20
#define BLPAPI_DATETIME_SECONDS_PART      0x40
#define BLPAPI_DATETIME_MILLISECONDS_PART 0x80
#define BLPAPI_DATETIME_FRACSECONDS_PART  0x80
#define BLPAPI_DATETIME_DATE_PART         (BLPAPI_DATETIME_YEAR_PART|   \
                                          BLPAPI_DATETIME_MONTH_PART|  \
                                          BLPAPI_DATETIME_DAY_PART)
#define BLPAPI_DATETIME_TIME_PART         (BLPAPI_DATETIME_HOURS_PART|   \
                                          BLPAPI_DATETIME_MINUTES_PART| \
                                          BLPAPI_DATETIME_SECONDS_PART)
#define BLPAPI_DATETIME_TIMEMILLI_PART    (BLPAPI_DATETIME_TIME_PART|   \
                                          BLPAPI_DATETIME_MILLISECONDS_PART)
#define BLPAPI_DATETIME_TIMEFRACSECONDS_PART (BLPAPI_DATETIME_TIME_PART|   \
                                          BLPAPI_DATETIME_FRACSECONDS_PART)


#define BLPAPI_EVENTTYPE_ADMIN                 1
#define BLPAPI_EVENTTYPE_SESSION_STATUS        2
#define BLPAPI_EVENTTYPE_SUBSCRIPTION_STATUS   3
#define BLPAPI_EVENTTYPE_REQUEST_STATUS        4
#define BLPAPI_EVENTTYPE_RESPONSE              5
#define BLPAPI_EVENTTYPE_PARTIAL_RESPONSE      6
#define BLPAPI_EVENTTYPE_SUBSCRIPTION_DATA     8
#define BLPAPI_EVENTTYPE_SERVICE_STATUS        9
#define BLPAPI_EVENTTYPE_TIMEOUT               10
#define BLPAPI_EVENTTYPE_AUTHORIZATION_STATUS  11
#define BLPAPI_EVENTTYPE_RESOLUTION_STATUS     12
#define BLPAPI_EVENTTYPE_TOPIC_STATUS          13
#define BLPAPI_EVENTTYPE_TOKEN_STATUS          14
#define BLPAPI_EVENTTYPE_REQUEST               15


#define BLPAPI_ELEMENT_INDEX_END 0xffffffff

#define BLPAPI_STATUS_ACTIVE                0
#define BLPAPI_STATUS_DEPRECATED            1
#define BLPAPI_STATUS_INACTIVE              2
#define BLPAPI_STATUS_PENDING_DEPRECATION   3

#define BLPAPI_SUBSCRIPTIONSTATUS_UNSUBSCRIBED          0
#define BLPAPI_SUBSCRIPTIONSTATUS_SUBSCRIBING           1
#define BLPAPI_SUBSCRIPTIONSTATUS_SUBSCRIBED            2
#define BLPAPI_SUBSCRIPTIONSTATUS_CANCELLED             3
#define BLPAPI_SUBSCRIPTIONSTATUS_PENDING_CANCELLATION  4

#define BLPAPI_CLIENTMODE_AUTO          0
#define BLPAPI_CLIENTMODE_DAPI          1
#define BLPAPI_CLIENTMODE_SAPI          2
#define BLPAPI_CLIENTMODE_COMPAT_33X    16

#define BLPAPI_ELEMENTDEFINITION_UNBOUNDED  -1

#define BLPAPI_RESOLVEMODE_DONT_REGISTER_SERVICES 0
#define BLPAPI_RESOLVEMODE_AUTO_REGISTER_SERVICES 1

#define BLPAPI_SEATTYPE_INVALID_SEAT -1
#define BLPAPI_SEATTYPE_BPS 0
#define BLPAPI_SEATTYPE_NONBPS 1

#define BLPAPI_SERVICEREGISTRATIONOPTIONS_PRIORITY_LOW 0
#define BLPAPI_SERVICEREGISTRATIONOPTIONS_PRIORITY_MEDIUM INT_MAX/2
#define BLPAPI_SERVICEREGISTRATIONOPTIONS_PRIORITY_HIGH INT_MAX

#define BLPAPI_REGISTRATIONPARTS_DEFAULT 0x1
#define BLPAPI_REGISTRATIONPARTS_PUBLISHING 0x2
#define BLPAPI_REGISTRATIONPARTS_OPERATIONS 0x4
#define BLPAPI_REGISTRATIONPARTS_SUBSCRIBER_RESOLUTION 0x8
#define BLPAPI_REGISTRATIONPARTS_PUBLISHER_RESOLUTION 0x10

#if defined(_WIN32) || defined(__WIN32__) && \
      ! (defined(cygwin) || defined(__cygwin))
    #ifdef BLPAPI_BUILD
        #define BLPAPI_EXPORT __declspec(dllexport)
    #else
        #define BLPAPI_EXPORT __declspec(dllimport)
    #endif
#elif __APPLE__
    #define BLPAPI_EXPORT  __attribute__((visibility("default")))
#else
    #define BLPAPI_EXPORT
#endif

#endif // #ifndef INCLUDED_BLPAPI_DEFS
