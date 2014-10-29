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
// blpapi_logging.h                                                   -*-C++-*-
#ifndef INCLUDED_BLPAPI_LOGGING
#define INCLUDED_BLPAPI_LOGGING

//@PURPOSE: Provide a C call to register a call back for logging
//
//@DESCRIPTION: This component provides a C function that is used to
// register a callback for logging
//
///Usage
///-----
// To use the call back a function needs to be created with the same definition
// of blpapi_Logging_Func_t.
// The callback will be called for all the log messages that have severity
// greater than or equal to the specified 'thresholdSeverity'.
// A callback can be registered multiple number of times but only the last
// registered callback will be used.
// Registering with a 'NULL' callback will de-register the callback.
//..
// extern "C" {
// void myLoggerCallback(blpapi_UInt64_t    threadId,
//                       int                severity,
//                       blpapi_Datetime_t  timestamp,
//                       const char        *category,
//                       const char        *message)
// {
//     if (severity == blpapi_Logging_SEVERITY_FATAL) {
//         // Do some logic for abort here
//     }
//     std::cout << severity << "-->" << message << std::endl;
// }
//..
// This callback needs to be registered with the library as
//..
// int main()  {
//     // ....
//     blpapi_Logging_registerCallback(myLoggerCallback,
//                                     blpapi_Logging_SEVERITY_TRACE);
//     // ....
// }
//..


#ifndef INCLUDED_BLPAPI_DATETIME
#include <blpapi_datetime.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum blpapi_Logging_Severity_t {
    blpapi_Logging_SEVERITY_OFF   = 0,
    blpapi_Logging_SEVERITY_FATAL = 1,
    blpapi_Logging_SEVERITY_ERROR = 2,
    blpapi_Logging_SEVERITY_WARN  = 3,
    blpapi_Logging_SEVERITY_INFO  = 4,
    blpapi_Logging_SEVERITY_DEBUG = 5,
    blpapi_Logging_SEVERITY_TRACE = 6
} blpapi_Logging_Severity_t;

typedef void (*blpapi_Logging_Func_t)(blpapi_UInt64_t    threadId,
                                      int                severity,
                                      blpapi_Datetime_t  timestamp,
                                      const char        *category,
                                      const char        *message);

BLPAPI_EXPORT
int blpapi_Logging_registerCallback(
                                  blpapi_Logging_Func_t     callback,
                                  blpapi_Logging_Severity_t thresholdSeverity);
    // Register the specified 'callback' at the specified 'thresholdSeverity'.
    // The 'callback' will be called for all the log messages that have
    // severity higher or equal to the 'thresholdSeverity'.
    // '0' is returned if callback is registered and a non-zero otherwise.
    // The callback needs to be registered before the start of all sessions.

#ifdef __cplusplus
}
#endif

#endif
