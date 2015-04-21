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

#ifndef INCLUDED_BLPAPI_CALL
#include <blpapi_call.h>
#endif

#ifndef INCLUDED_BLPAPI_DATETIME
#include <blpapi_datetime.h>
#endif

#ifndef INCLUDED_BLPAPI_DEFS
#include <blpapi_defs.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*blpapi_Logging_Func_t)(blpapi_UInt64_t    threadId,
                                      int                severity,
                                      blpapi_Datetime_t  timestamp,
                                      const char        *category,
                                      const char        *message);
    // Provide a type definition for the callback that will be called when a
    // message is logged.  The specified 'threadId' is the id of the calling
    // thread.  The specified 'severity' is the severity of the log message
    // whose value is defined by the type 'blpapi_Logging_Severity_t'.  The
    // specified 'timestamp' is the date and time the message is logged.  The
    // specified 'category' identifies the source of the logged message.  The
    // specified 'message' is the actual log message text.

BLPAPI_EXPORT
int blpapi_Logging_registerCallback(
                                  blpapi_Logging_Func_t     callback,
                                  blpapi_Logging_Severity_t thresholdSeverity);
    // Register the specified 'callback' that will be called for all log
    // messages with severity greater than or equal to the specified
    // 'thresholdSeverity'.  The callback needs to be registered before the
    // start of all sessions.  If this function is called multiple times, only
    // the last registered callback will take effect.
    // '0' is returned if callback is registered and a non-zero otherwise.

BLPAPI_EXPORT
void blpapi_Logging_logTestMessage(blpapi_Logging_Severity_t severity);
    // Log a test message at the specified 'severity'.  Note that this
    // function is intended for testing of the logging configuration only.


#ifdef __cplusplus
}

namespace BloombergLP {
namespace blpapi {

struct Logging {
    // This utility 'struct' provides a namespace for functions to test the
    // logging configuration.

    struct Severity {
        enum Type {
            // Enumeration used to specify different logging severity levels.

            e_off   = blpapi_Logging_SEVERITY_OFF,
            e_fatal = blpapi_Logging_SEVERITY_FATAL,
            e_error = blpapi_Logging_SEVERITY_ERROR,
            e_warn  = blpapi_Logging_SEVERITY_WARN,
            e_info  = blpapi_Logging_SEVERITY_INFO,
            e_debug = blpapi_Logging_SEVERITY_DEBUG,
            e_trace = blpapi_Logging_SEVERITY_TRACE
        };
    };

    // CLASS METHODS
    static void logTestMessage(Severity::Type severity);
        // Log a test message with the specified 'severity'.  Note that this
        // function is intended for testing of the logging configuration only.
};


// ============================================================================
//                      INLINE FUNCTION DEFINITIONS
// ============================================================================

                             // --------------
                             // struct Logging 
                             // --------------

// CLASS METHODS
inline
void Logging::logTestMessage(Severity::Type severity)
{
    BLPAPI_CALL(blpapi_Logging_logTestMessage)(
                             static_cast<blpapi_Logging_Severity_t>(severity));
}

}  // close namespace blpapi
}  // close namespace BloombergLP

#endif

#endif
