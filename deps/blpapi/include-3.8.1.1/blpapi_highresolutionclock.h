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
// blpapi_clock.h                                                     -*-C++-*-
#ifndef INCLUDED_BLPAPI_HIGHRESOLUTIONCLOCK
#define INCLUDED_BLPAPI_HIGHRESOLUTIONCLOCK
//@PURPOSE: Provide a high resolution clock.
//
//@CLASSES:
// blpapi::HighResolutionClock: a high resolution clock
//
//@DESCRIPTION: This component provides a way to access the current time as a
// 'blpapi::TimePoint' value.

#ifndef INCLUDED_BLPAPI_CALL
#include <blpapi_call.h>
#endif

#ifndef INCLUDED_BLPAPI_DEFS
#include <blpapi_defs.h>
#endif

#ifndef INCLUDED_BLPAPI_TIMEPOINT
#include <blpapi_timepoint.h>
#endif

#ifndef INCLUDED_BLPAPI_TYPES
#include <blpapi_types.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

BLPAPI_EXPORT
int blpapi_HighResolutionClock_now(blpapi_TimePoint_t *timePoint);
    // Load the current time into the specified 'timePoint' and return zero, or
    // leave timePoint unchanged and return a non-zero value.
#ifdef __cplusplus
}

namespace BloombergLP {
namespace blpapi {
                          // ==========================
                          // struct HighResolutionClock
                          // ==========================

struct HighResolutionClock {
    // This utility struct provides a source for the current moment in time as
    // a 'blpapi::TimePoint' object. This is currently intended for use
    // primarily in conjunction with the 'blpapi::Message::timeReceived'
    // interfaces, to allow measurement of the amount of time a message spends
    // in the client event queue.

    static TimePoint now();
        // Return the current moment in time as a 'TimePoint' value.
};

// ============================================================================
//                      INLINE AND TEMPLATE FUNCTION IMPLEMENTATIONS
// ============================================================================

                          // --------------------------
                          // struct HighResolutionClock
                          // --------------------------
inline
TimePoint HighResolutionClock::now()
{
    TimePoint tp;
    BLPAPI_CALL_HIGHRESOLUTIONCLOCK_NOW(&tp);
    return tp;
}

}  // close namespace blpapi
}  // close namespace BloombergLP

#endif // #ifdef __cplusplus
#endif // #ifndef INCLUDED_BLPAPI_HIGHRESOLUTIONCLOCK
