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
// blpapi_call.h                                                      -*-C++-*-

#ifndef INCLUDED_BLPAPI_CALL
#define INCLUDED_BLPAPI_CALL

//@PURPOSE: Provide functions for dispatchtbl
//
//@DESCRIPTION: This component provides an interface to initialize the
// function table. [NOT TO BE INCLUDED BY APPLICATION CODE DIRECTLY]
#ifdef __cplusplus

// We only support dispatchtbl on windows currently
#if defined(_WIN32) || defined(__WIN32__)
#include <blpapi_dispatchtbl.h>
#include <blpapi_exception.h>

// NOTE: The following macros are used to avoid direct dependencies
// on functions that may not be available in an old DLL.

#define BLPAPI_TABLE_CHECK(FUNCNAME) \
    ((g_blpapiFunctionTableSize &&                                            \
      offsetof(blpapi_FunctionEntries_t, FUNCNAME) <                          \
      g_blpapiFunctionTableSize - 1) ? 1 : 0)

#define BLPAPI_TABLE_CALL(FUNCNAME)                                           \
    (BLPAPI_TABLE_CHECK(FUNCNAME)                                             \
        ? (g_blpapiFunctionEntries.FUNCNAME)                                  \
        : BLPAPI_UNSUPPORTED_CALL(FUNCNAME))

#define BLPAPI_UNSUPPORTED_CALL(FUNCNAME)                                     \
    (throw UnsupportedOperationException(#FUNCNAME " not supported"), 0)

#define BLPAPI_CALL_EVENTFORMATTER_APPENDMESSAGESEQ(a1, a2, a3, a4, a5, a6)   \
    (BLPAPI_TABLE_CALL(                                                       \
        blpapi_EventFormatter_appendMessageSeq)(a1, a2, a3, a4, a5, a6))

#define BLPAPI_CALL_EVENTFORMATTER_APPENDRECAPMESSAGESEQ(a1, a2, a3, a4, a5)  \
    (BLPAPI_TABLE_CALL(                                                       \
        blpapi_EventFormatter_appendRecapMessageSeq)(a1, a2, a3, a4, a5))

#define BLPAPI_CALL_MESSAGE_ADDREF(a1)                                        \
    do {                                                                      \
        if (BLPAPI_TABLE_CHECK(blpapi_Message_addRef))                        \
            g_blpapiFunctionEntries.blpapi_Message_addRef(a1);                \
    } while(0)

#define BLPAPI_CALL_MESSAGE_RELEASE(a1)                                       \
    do {                                                                      \
        if (BLPAPI_TABLE_CHECK(blpapi_Message_release))                       \
            g_blpapiFunctionEntries.blpapi_Message_release(a1);               \
    } while(0)

#define BLPAPI_CALL_SESSIONOPTION_SETMAXEVENTQUEUESIZE(a1, a2)                \
    do {                                                                      \
        if (BLPAPI_TABLE_CHECK(blpapi_SessionOptions_setMaxEventQueueSize))   \
            g_blpapiFunctionEntries.                                          \
                      blpapi_SessionOptions_setMaxEventQueueSize(a1, a2);     \
    } while(0)

#define BLPAPI_CALL_SESSIONOPTION_SETSLOWCONSUMERHIWATERMARK(a1, a2)          \
    (BLPAPI_TABLE_CHECK(                                                      \
        blpapi_SessionOptions_setSlowConsumerWarningHiWaterMark)              \
    ? g_blpapiFunctionEntries.                                                \
          blpapi_SessionOptions_setSlowConsumerWarningHiWaterMark(a1, a2)     \
    : 0)

#define BLPAPI_CALL_SESSIONOPTION_SETSLOWCONSUMERLOWATERMARK(a1, a2)          \
    (BLPAPI_TABLE_CHECK(                                                      \
        blpapi_SessionOptions_setSlowConsumerWarningLoWaterMark)              \
    ? g_blpapiFunctionEntries.                                                \
          blpapi_SessionOptions_setSlowConsumerWarningLoWaterMark(a1, a2)     \
    : 0)

#define BLPAPI_CALL_REQUEST_SETPREFERREDROUTE(a1, a2)                         \
    do {                                                                      \
        if (BLPAPI_TABLE_CHECK(blpapi_Request_setPreferredRoute))             \
            g_blpapiFunctionEntries.blpapi_Request_setPreferredRoute(a1, a2); \
    } while(0)

#define BLPAPI_CALL_MESSAGE_FRAGMENTTYPE(a1)                              \
    (BLPAPI_TABLE_CHECK(blpapi_Message_fragmentType)                      \
    ? g_blpapiFunctionEntries.blpapi_Message_fragmentType(a1)             \
    : 0)

#define BLPAPI_CALL_SESSIONOPTION_MAXEVENTQUEUESIZE(a1)                    \
    (BLPAPI_TABLE_CHECK(blpapi_SessionOptions_maxEventQueueSize)           \
    ? g_blpapiFunctionEntries.blpapi_SessionOptions_maxEventQueueSize(a1)  \
    : ULONG_MAX)

#define BLPAPI_CALL_SESSIONOPTION_SLOWCONSUMERHIWATERMARK(a1)                          \
    (BLPAPI_TABLE_CHECK(blpapi_SessionOptions_slowConsumerWarningHiWaterMark)          \
    ? g_blpapiFunctionEntries.blpapi_SessionOptions_slowConsumerWarningHiWaterMark(a1) \
    : 1.0f)

#define BLPAPI_CALL_SESSIONOPTION_SLOWCONSUMERLOWATERMARK(a1)                          \
    (BLPAPI_TABLE_CHECK(blpapi_SessionOptions_slowConsumerWarningLoWaterMark)          \
    ? g_blpapiFunctionEntries.blpapi_SessionOptions_slowConsumerWarningLoWaterMark(a1) \
    : 0.0f)

#define BLPAPI_CALL_SESSIONOPTION_DEFAULTKEEPALIVEINACTIVITYTIME(h)           \
    (BLPAPI_TABLE_CHECK(                                                      \
        blpapi_SessionOptions_defaultKeepAliveInactivityTime)                 \
    ? g_blpapiFunctionEntries.                                                \
                blpapi_SessionOptions_defaultKeepAliveInactivityTime(h)       \
    : INT_MAX)

#define BLPAPI_CALL_SESSIONOPTION_DEFAULTKEEPALIVERESPONSETIMEOUT(h)          \
    (BLPAPI_TABLE_CHECK(                                                      \
        blpapi_SessionOptions_defaultKeepAliveResponseTimeout)                \
    ? g_blpapiFunctionEntries.                                                \
                blpapi_SessionOptions_defaultKeepAliveResponseTimeout(h)      \
    : INT_MAX)

#define BLPAPI_CALL_SESSIONOPTION_SETDEFAULTKEEPALIVEINACTIVITYTIME(h, t)     \
    (BLPAPI_TABLE_CHECK(                                                      \
        blpapi_SessionOptions_setDefaultKeepAliveInactivityTime)              \
    ? g_blpapiFunctionEntries.                                                \
                blpapi_SessionOptions_setDefaultKeepAliveInactivityTime(h,t)  \
    : 0)

#define BLPAPI_CALL_SESSIONOPTION_SETDEFAULTKEEPALIVERESPONSETIMEOUT(h, t)    \
    (BLPAPI_TABLE_CHECK(                                                      \
        blpapi_SessionOptions_setDefaultKeepAliveResponseTimeout)             \
    ? g_blpapiFunctionEntries.                                                \
                blpapi_SessionOptions_setDefaultKeepAliveResponseTimeout(h,t) \
    : 0)

#else  // if defined(_WIN32) || defined(__WIN32__)
#define BLPAPI_CALL_EVENTFORMATTER_APPENDMESSAGESEQ(a1, a2, a3, a4, a5, a6)   \
    (blpapi_EventFormatter_appendMessageSeq)(a1, a2, a3, a4, a5, a6)
#define BLPAPI_CALL_EVENTFORMATTER_APPENDRECAPMESSAGESEQ(a1, a2, a3, a4, a5)  \
    (blpapi_EventFormatter_appendRecapMessageSeq)(a1, a2, a3, a4, a5)
#define BLPAPI_CALL_MESSAGE_ADDREF(a1)                                        \
    (blpapi_Message_addRef)(a1)
#define BLPAPI_CALL_MESSAGE_RELEASE(a1)                                       \
    (blpapi_Message_release)(a1)
#define BLPAPI_CALL_SESSIONOPTION_SETMAXEVENTQUEUESIZE(a1, a2)                \
    (blpapi_SessionOptions_setMaxEventQueueSize)(a1, a2)
#define BLPAPI_CALL_SESSIONOPTION_SETSLOWCONSUMERHIWATERMARK(a1, a2)          \
    (blpapi_SessionOptions_setSlowConsumerWarningHiWaterMark)(a1, a2)
#define BLPAPI_CALL_SESSIONOPTION_SETSLOWCONSUMERLOWATERMARK(a1, a2)          \
    (blpapi_SessionOptions_setSlowConsumerWarningLoWaterMark)(a1, a2)
#define BLPAPI_CALL_REQUEST_SETPREFERREDROUTE(a1, a2)                         \
    (blpapi_Request_setPreferredRoute)(a1, a2)
#define BLPAPI_CALL_MESSAGE_FRAGMENTTYPE(a1)                                  \
    (blpapi_Message_fragmentType)(a1)
#define BLPAPI_CALL_SESSIONOPTION_MAXEVENTQUEUESIZE(a1)                       \
    (blpapi_SessionOptions_maxEventQueueSize)(a1)
#define BLPAPI_CALL_SESSIONOPTION_SLOWCONSUMERHIWATERMARK(a1)                 \
    (blpapi_SessionOptions_slowConsumerWarningHiWaterMark)(a1)
#define BLPAPI_CALL_SESSIONOPTION_SLOWCONSUMERLOWATERMARK(a1)                 \
    (blpapi_SessionOptions_slowConsumerWarningLoWaterMark)(a1)

#define BLPAPI_CALL_SESSIONOPTION_DEFAULTKEEPALIVEINACTIVITYTIME(a1)          \
    (blpapi_SessionOptions_defaultKeepAliveInactivityTime)(a1)
#define BLPAPI_CALL_SESSIONOPTION_DEFAULTKEEPALIVERESPONSETIMEOUT(a1)         \
    (blpapi_SessionOptions_defaultKeepAliveResponseTimeout)(a1)
#define BLPAPI_CALL_SESSIONOPTION_SETDEFAULTKEEPALIVEINACTIVITYTIME(a1, a2)   \
    (blpapi_SessionOptions_setDefaultKeepAliveInactivityTime)(a1, a2)
#define BLPAPI_CALL_SESSIONOPTION_SETDEFAULTKEEPALIVERESPONSETIMEOUT(a1, a2)  \
    (blpapi_SessionOptions_setDefaultKeepAliveResponseTimeout)(a1, a2)
#endif // if defined(_WIN32) || defined(__WIN32__)

#endif // __cplusplus

#endif // INCLUDED_BLPAPI_CALL
