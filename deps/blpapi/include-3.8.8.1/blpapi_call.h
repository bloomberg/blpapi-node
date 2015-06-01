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

#define BLPAPI_CALL_AVAILABLE(FUNCNAME) BLPAPI_TABLE_CHECK(FUNCNAME)
#define BLPAPI_CALL(FUNCNAME) BLPAPI_TABLE_CALL(FUNCNAME)

#define BLPAPI_UNSUPPORTED_CALL(FUNCNAME)                                     \
    (throw UnsupportedOperationException(#FUNCNAME " not supported"), 0)

#define BLPAPI_CALL_EVENTFORMATTER_APPENDMESSAGESEQ(a1, a2, a3, a4, a5, a6)   \
    (BLPAPI_TABLE_CALL(                                                       \
        blpapi_EventFormatter_appendMessageSeq)(a1, a2, a3, a4, a5, a6))

#define BLPAPI_CALL_EVENTFORMATTER_APPENDRECAPMESSAGESEQ(a1, a2, a3, a4, a5)  \
    (BLPAPI_TABLE_CALL(                                                       \
        blpapi_EventFormatter_appendRecapMessageSeq)(a1, a2, a3, a4, a5))

#define BLPAPI_CALL_MESSAGE_ADDREF(a1)                                        \
    (BLPAPI_TABLE_CHECK(blpapi_Message_addRef)                                \
    ? g_blpapiFunctionEntries.blpapi_Message_addRef(a1)                       \
    : 0)

#define BLPAPI_CALL_MESSAGE_RELEASE(a1)                                       \
    (BLPAPI_TABLE_CHECK(blpapi_Message_release)                               \
    ? g_blpapiFunctionEntries.blpapi_Message_release(a1)                      \
    : 0)

#define BLPAPI_CALL_SESSIONOPTIONS_SETMAXEVENTQUEUESIZE(a1, a2)               \
    (BLPAPI_TABLE_CHECK(blpapi_SessionOptions_setMaxEventQueueSize)           \
    ? g_blpapiFunctionEntries.                                                \
                      blpapi_SessionOptions_setMaxEventQueueSize(a1, a2)      \
	: 0)

#define BLPAPI_CALL_SESSIONOPTIONS_SETSLOWCONSUMERHIWATERMARK(a1, a2)         \
    (BLPAPI_TABLE_CHECK(                                                      \
        blpapi_SessionOptions_setSlowConsumerWarningHiWaterMark)              \
    ? g_blpapiFunctionEntries.                                                \
          blpapi_SessionOptions_setSlowConsumerWarningHiWaterMark(a1, a2)     \
    : 0)

#define BLPAPI_CALL_SESSIONOPTIONS_SETSLOWCONSUMERLOWATERMARK(a1, a2)         \
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

#define BLPAPI_CALL_MESSAGE_FRAGMENTTYPE(a1)                                  \
    (BLPAPI_TABLE_CHECK(blpapi_Message_fragmentType)                          \
    ? g_blpapiFunctionEntries.blpapi_Message_fragmentType(a1)                 \
    : 0)

#define BLPAPI_CALL_SESSIONOPTIONS_MAXEVENTQUEUESIZE(a1)                      \
    (BLPAPI_TABLE_CHECK(blpapi_SessionOptions_maxEventQueueSize)              \
    ? g_blpapiFunctionEntries.blpapi_SessionOptions_maxEventQueueSize(a1)     \
    : ULONG_MAX)

#define BLPAPI_CALL_SESSIONOPTIONS_SLOWCONSUMERHIWATERMARK(a1)                \
    (BLPAPI_TABLE_CHECK(blpapi_SessionOptions_slowConsumerWarningHiWaterMark) \
    ? g_blpapiFunctionEntries                                                 \
        .blpapi_SessionOptions_slowConsumerWarningHiWaterMark(a1)             \
    : 1.0f)

#define BLPAPI_CALL_SESSIONOPTIONS_SLOWCONSUMERLOWATERMARK(a1)                \
    (BLPAPI_TABLE_CHECK(blpapi_SessionOptions_slowConsumerWarningLoWaterMark) \
    ? g_blpapiFunctionEntries                                                 \
        .blpapi_SessionOptions_slowConsumerWarningLoWaterMark(a1)             \
    : 0.0f)

#define BLPAPI_CALL_SESSIONOPTIONS_DEFAULTKEEPALIVEINACTIVITYTIME(h)          \
    (BLPAPI_TABLE_CHECK(                                                      \
        blpapi_SessionOptions_defaultKeepAliveInactivityTime)                 \
    ? g_blpapiFunctionEntries.                                                \
                blpapi_SessionOptions_defaultKeepAliveInactivityTime(h)       \
    : INT_MAX)

#define BLPAPI_CALL_SESSIONOPTIONS_DEFAULTKEEPALIVERESPONSETIMEOUT(h)         \
    (BLPAPI_TABLE_CHECK(                                                      \
        blpapi_SessionOptions_defaultKeepAliveResponseTimeout)                \
    ? g_blpapiFunctionEntries.                                                \
                blpapi_SessionOptions_defaultKeepAliveResponseTimeout(h)      \
    : INT_MAX)

#define BLPAPI_CALL_SESSIONOPTIONS_KEEPALIVEENABLED(h)                        \
    (BLPAPI_TABLE_CHECK(blpapi_SessionOptions_keepAliveEnabled)               \
    ? g_blpapiFunctionEntries.                                                \
                blpapi_SessionOptions_keepAliveEnabled(h)                     \
    : 1)

#define BLPAPI_CALL_SESSIONOPTIONS_SETDEFAULTKEEPALIVEINACTIVITYTIME(h, t)    \
    (BLPAPI_TABLE_CHECK(                                                      \
        blpapi_SessionOptions_setDefaultKeepAliveInactivityTime)              \
    ? g_blpapiFunctionEntries.                                                \
                blpapi_SessionOptions_setDefaultKeepAliveInactivityTime(h,t)  \
    : 0)

#define BLPAPI_CALL_SESSIONOPTIONS_SETDEFAULTKEEPALIVERESPONSETIMEOUT(h, t)   \
    (BLPAPI_TABLE_CHECK(                                                      \
        blpapi_SessionOptions_setDefaultKeepAliveResponseTimeout)             \
    ? g_blpapiFunctionEntries.                                                \
                blpapi_SessionOptions_setDefaultKeepAliveResponseTimeout(h,t) \
    : 0)

#define BLPAPI_CALL_SESSIONOPTIONS_SETKEEPALIVEENABLED(h, t)                  \
    (BLPAPI_TABLE_CHECK(blpapi_SessionOptions_setKeepAliveEnabled)            \
    ? g_blpapiFunctionEntries.blpapi_SessionOptions_setKeepAliveEnabled(h, t) \
    : 0)

#define BLPAPI_CALL_HIGHPRECISIONDATETIME_COMPARE(a1, a2)                     \
    (BLPAPI_TABLE_CHECK(blpapi_HighPrecisionDatetime_compare)                 \
    ? g_blpapiFunctionEntries.blpapi_HighPrecisionDatetime_compare(a1, a2)    \
    : blpapi_Datetime_compare((a1)->datetime, (a2)->datetime))

#define BLPAPI_CALL_HIGHPRECISIONDATETIME_PRINT(a1, a2, a3, a4, a5)           \
    (BLPAPI_TABLE_CHECK(blpapi_HighPrecisionDatetime_print)                   \
    ? g_blpapiFunctionEntries.blpapi_HighPrecisionDatetime_print(a1,          \
                                                                 a2,          \
                                                                 a3,          \
                                                                 a4,          \
                                                                 a5)          \
    : blpapi_Datetime_print(&((a1)->datetime), a2, a3, a4, a5))

#define BLPAPI_CALL_ELEMENT_SETVALUEHIGHPRECISIONDATETIME(a1, a2, a3)         \
    (BLPAPI_TABLE_CHECK(blpapi_Element_setValueHighPrecisionDatetime)         \
    ? g_blpapiFunctionEntries.blpapi_Element_setValueHighPrecisionDatetime    \
        (a1, a2, a3)                                                          \
    : blpapi_Element_setValueDatetime(a1, &((a2)->datetime), a3))

#define BLPAPI_CALL_ELEMENT_SETELEMENTHIGHPRECISIONDATETIME(a1, a2, a3, a4)   \
    (BLPAPI_TABLE_CHECK(blpapi_Element_setElementHighPrecisionDatetime)       \
    ? g_blpapiFunctionEntries.blpapi_Element_setElementHighPrecisionDatetime  \
        (a1, a2, a3, a4)                                                      \
    : blpapi_Element_setElementDatetime(a1, a2, a3, &((a4)->datetime)))

#define BLPAPI_CALL_ELEMENT_GETVALUEASHIGHPRECISIONDATETIME(a1, a2, a3)       \
    if (BLPAPI_TABLE_CHECK(blpapi_Element_getValueAsHighPrecisionDatetime)) { \
        return g_blpapiFunctionEntries                                        \
                .blpapi_Element_getValueAsHighPrecisionDatetime(              \
                                               a1,                            \
                                               &(a2)->rawHighPrecisionValue(),\
                                               a3);                           \
    } else {                                                                  \
        buffer->rawHighPrecisionValue().picoseconds = 0;                      \
        return blpapi_Element_getValueAsDatetime(a1,                          \
                                                 &(a2)->rawValue(),           \
                                                 a3);                         \
    }

#define BLPAPI_CALL_SESSION_RESUBSCRIBEWITHID(a1, a2, a3, a4, a5)             \
    (BLPAPI_TABLE_CHECK(blpapi_Session_resubscribeWithId)                     \
    ? g_blpapiFunctionEntries.blpapi_Session_resubscribeWithId(a1,            \
                                                               a2,            \
                                                               a3,            \
                                                               a4,            \
                                                               a5)            \
    : blpapi_Session_resubscribe(a1, a2, a4, a5))

#define BLPAPI_CALL_EVENTFORMATTER_SETVALUENULL(a1, a2, a3)                   \
    (BLPAPI_TABLE_CALL(blpapi_EventFormatter_setValueNull)(a1, a2, a3))

#define BLPAPI_CALL_DIAGNOSTICSUTIL_MEMORYINFO(a1, a2)                        \
    if (BLPAPI_TABLE_CHECK(blpapi_DiagnosticsUtil_memoryInfo)) {              \
        return g_blpapiFunctionEntries.blpapi_DiagnosticsUtil_memoryInfo(a1,  \
                                                                         a2); \
    } else {                                                                  \
        return -1;                                                            \
    }

#define BLPAPI_CALL_SUBSCRIPTIONLIST_ADDRESOLVED(a1, a2, a3)                  \
(BLPAPI_TABLE_CHECK(blpapi_SubscriptionList_addResolved)                  \
    ? g_blpapiFunctionEntries.blpapi_SubscriptionList_addResolved(        \
                                         a1,                              \
                                         a2,                              \
                                         a3)                              \
    : blpapi_SubscriptionList_add(a1, a2, a3, 0, 0, 0, 0))

#define BLPAPI_CALL_SUBSCRIPTIONLIST_ISRESOLVEDAT(a1, a2, a3)                 \
(BLPAPI_TABLE_CHECK(blpapi_SubscriptionList_isResolvedAt)                 \
    ? g_blpapiFunctionEntries                                             \
            .blpapi_SubscriptionList_isResolvedAt(a1, a2, a3)             \
    : (*a2 = 0, 0))

#define BLPAPI_CALL_PROVIDERSESSION_DEREGISTERSERVICE(a1, a2)                 \
    (BLPAPI_TABLE_CALL(blpapi_ProviderSession_deregisterService)(a1, a2))

#define BLPAPI_CALL_SERVICEREGISTRATIONOPTIONS_SETPARTSTOREGISTER(a1, a2)     \
    (BLPAPI_TABLE_CALL(                                                       \
        blpapi_ServiceRegistrationOptions_setPartsToRegister)(a1, a2))

#define BLPAPI_CALL_SERVICEREGISTRATIONOPTIONS_GETPARTSTOREGISTER(a1)         \
    (BLPAPI_TABLE_CALL(                                                       \
        blpapi_ServiceRegistrationOptions_getPartsToRegister)(a1))

#define BLPAPI_CALL_PROVIDERSESSION_DELETETOPICS(a1, a2, a3)                 \
    (BLPAPI_TABLE_CALL(blpapi_ProviderSession_deleteTopics)(a1, a2, a3))

#define BLPAPI_CALL_PROVIDERSESSION_ACTIVATESUBSERVICECODERANGE(              \
                                                          a1, a2, a3, a4, a5) \
    (BLPAPI_TABLE_CALL(blpapi_ProviderSession_activateSubServiceCodeRange)(   \
                           a1, a2, a3, a4, a5))

#define BLPAPI_CALL_PROVIDERSESSION_DEACTIVATESUBSERVICECODERANGE(            \
                                                              a1, a2, a3, a4) \
    (BLPAPI_TABLE_CALL(blpapi_ProviderSession_deactivateSubServiceCodeRange)( \
                           a1, a2, a3, a4))

#define BLPAPI_CALL_SERVICEREGISTRATIONOPTIONS_ADDACTIVATESUBSERVICECODERANGE(\
                                                              a1, a2, a3, a4) \
    (BLPAPI_TABLE_CALL(                                                       \
         blpapi_ServiceRegistrationOptions_addActiveSubServiceCodeRange)(     \
             a1, a2, a3, a4))

#define                                                                       \
  BLPAPI_CALL_SERVICEREGISTRATIONOPTIONS_REMOVEALLACTIVESUBSERVICECODERANGES( \
                                                                          a1) \
    (BLPAPI_TABLE_CALL(                                                       \
       blpapi_ServiceRegistrationOptions_removeAllActiveSubServiceCodeRanges)(\
                                                                           a1))

#define BLPAPI_CALL_MESSAGE_TIMERECEIVED(a1, a2)                              \
    (BLPAPI_TABLE_CALL(blpapi_Message_timeReceived)(a1, a2))

#define BLPAPI_CALL_SESSIONOPTION_SETRECORDSUBSCRIPTIONDATARECEIVETIMES(a1,   \
                                                                        a2)   \
    (BLPAPI_TABLE_CHECK(                                                      \
              blpapi_SessionOptions_setRecordSubscriptionDataReceiveTimes)    \
	? g_blpapiFunctionEntries                                                 \
        .blpapi_SessionOptions_setRecordSubscriptionDataReceiveTimes(a1, a2)  \
	: 0)

#define BLPAPI_CALL_SESSIONOPTION_RECORDSUBSCRIPTIONDATARECEIVETIMES(a1)      \
    (BLPAPI_TABLE_CHECK(                                                      \
            blpapi_SessionOptions_recordSubscriptionDataReceiveTimes)         \
        ? g_blpapiFunctionEntries                                             \
            .blpapi_SessionOptions_recordSubscriptionDataReceiveTimes(a1)     \
        : false)

#define BLPAPI_CALL_TIMEPOINTUTIL_NANOSECONDSBETWEEN(a1, a2)                  \
    (BLPAPI_TABLE_CALL(blpapi_TimePointUtil_nanosecondsBetween)(a1, a2))

#define BLPAPI_CALL_HIGHRESOLUTIONCLOCK_NOW(a1)                               \
    (BLPAPI_TABLE_CALL(blpapi_HighResolutionClock_now)(a1))

#define BLPAPI_CALL_HIGHPRECISIONDATETIME_FROMTIMEPOINT(a1, a2, a3)           \
    (BLPAPI_TABLE_CALL(blpapi_HighPrecisionDatetime_fromTimePoint)(a1, a2, a3))

#else  // if defined(_WIN32) || defined(__WIN32__)

#define BLPAPI_CALL_AVAILABLE(FUNCNAME) true
#define BLPAPI_CALL(FUNCNAME) FUNCNAME

#define BLPAPI_CALL_EVENTFORMATTER_APPENDMESSAGESEQ(a1, a2, a3, a4, a5, a6)   \
    (blpapi_EventFormatter_appendMessageSeq)(a1, a2, a3, a4, a5, a6)
#define BLPAPI_CALL_EVENTFORMATTER_APPENDRECAPMESSAGESEQ(a1, a2, a3, a4, a5)  \
    (blpapi_EventFormatter_appendRecapMessageSeq)(a1, a2, a3, a4, a5)
#define BLPAPI_CALL_MESSAGE_ADDREF(a1)                                        \
    (blpapi_Message_addRef)(a1)
#define BLPAPI_CALL_MESSAGE_RELEASE(a1)                                       \
    (blpapi_Message_release)(a1)
#define BLPAPI_CALL_SESSIONOPTIONS_SETMAXEVENTQUEUESIZE(a1, a2)               \
    (blpapi_SessionOptions_setMaxEventQueueSize)(a1, a2)
#define BLPAPI_CALL_SESSIONOPTIONS_SETSLOWCONSUMERHIWATERMARK(a1, a2)         \
    (blpapi_SessionOptions_setSlowConsumerWarningHiWaterMark)(a1, a2)
#define BLPAPI_CALL_SESSIONOPTIONS_SETSLOWCONSUMERLOWATERMARK(a1, a2)         \
    (blpapi_SessionOptions_setSlowConsumerWarningLoWaterMark)(a1, a2)
#define BLPAPI_CALL_REQUEST_SETPREFERREDROUTE(a1, a2)                         \
    (blpapi_Request_setPreferredRoute)(a1, a2)
#define BLPAPI_CALL_MESSAGE_FRAGMENTTYPE(a1)                                  \
    (blpapi_Message_fragmentType)(a1)
#define BLPAPI_CALL_SESSIONOPTIONS_MAXEVENTQUEUESIZE(a1)                      \
    (blpapi_SessionOptions_maxEventQueueSize)(a1)
#define BLPAPI_CALL_SESSIONOPTIONS_SLOWCONSUMERHIWATERMARK(a1)                \
    (blpapi_SessionOptions_slowConsumerWarningHiWaterMark)(a1)
#define BLPAPI_CALL_SESSIONOPTIONS_SLOWCONSUMERLOWATERMARK(a1)                \
    (blpapi_SessionOptions_slowConsumerWarningLoWaterMark)(a1)

#define BLPAPI_CALL_SESSIONOPTIONS_DEFAULTKEEPALIVEINACTIVITYTIME(a1)         \
    (blpapi_SessionOptions_defaultKeepAliveInactivityTime)(a1)
#define BLPAPI_CALL_SESSIONOPTIONS_DEFAULTKEEPALIVERESPONSETIMEOUT(a1)        \
    (blpapi_SessionOptions_defaultKeepAliveResponseTimeout)(a1)
#define BLPAPI_CALL_SESSIONOPTIONS_KEEPALIVEENABLED(a1)                       \
    (blpapi_SessionOptions_keepAliveEnabled)(a1)
#define BLPAPI_CALL_SESSIONOPTIONS_SETDEFAULTKEEPALIVEINACTIVITYTIME(a1, a2)  \
    (blpapi_SessionOptions_setDefaultKeepAliveInactivityTime)(a1, a2)
#define BLPAPI_CALL_SESSIONOPTIONS_SETDEFAULTKEEPALIVERESPONSETIMEOUT(a1, a2) \
    (blpapi_SessionOptions_setDefaultKeepAliveResponseTimeout)(a1, a2)
#define BLPAPI_CALL_SESSIONOPTIONS_SETKEEPALIVEENABLED(a1, a2)                \
    (blpapi_SessionOptions_setKeepAliveEnabled)(a1, a2)

#define BLPAPI_CALL_HIGHPRECISIONDATETIME_COMPARE(a1, a2)                     \
    (blpapi_HighPrecisionDatetime_compare)(a1, a2)
#define BLPAPI_CALL_HIGHPRECISIONDATETIME_PRINT(a1, a2, a3, a4, a5)           \
    (blpapi_HighPrecisionDatetime_print)(a1, a2, a3, a4, a5)
#define BLPAPI_CALL_ELEMENT_SETVALUEHIGHPRECISIONDATETIME(a1, a2, a3)         \
    (blpapi_Element_setValueHighPrecisionDatetime(a1, a2, a3))
#define BLPAPI_CALL_ELEMENT_SETELEMENTHIGHPRECISIONDATETIME(a1, a2, a3, a4)   \
    (blpapi_Element_setElementHighPrecisionDatetime(a1, a2, a3, a4))
#define BLPAPI_CALL_ELEMENT_GETVALUEASHIGHPRECISIONDATETIME(a1, a2, a3)       \
    return blpapi_Element_getValueAsHighPrecisionDatetime(                    \
                                                 a1,                          \
                                                 &a2->rawHighPrecisionValue(),\
                                                 a3);
#define BLPAPI_CALL_SESSION_RESUBSCRIBEWITHID(a1, a2, a3, a4, a5)             \
    (blpapi_Session_resubscribeWithId)(a1, a2, a3, a4, a5)

#define BLPAPI_CALL_EVENTFORMATTER_SETVALUENULL(a1, a2, a3)                   \
    (blpapi_EventFormatter_setValueNull)(a1, a2, a3)

#define BLPAPI_CALL_DIAGNOSTICSUTIL_MEMORYINFO(a1, a2)                        \
    return blpapi_DiagnosticsUtil_memoryInfo(a1, a2);

#define BLPAPI_CALL_SUBSCRIPTIONLIST_ADDRESOLVED(a1, a2, a3)                  \
    (blpapi_SubscriptionList_addResolved)(a1, a2, a3)
#define BLPAPI_CALL_SUBSCRIPTIONLIST_ISRESOLVEDAT(a1, a2, a3)                 \
    (blpapi_SubscriptionList_isResolvedAt)(a1, a2, a3)

#define BLPAPI_CALL_PROVIDERSESSION_DEREGISTERSERVICE(a1, a2)                 \
    (blpapi_ProviderSession_deregisterService(a1, a2))

#define BLPAPI_CALL_SERVICEREGISTRATIONOPTIONS_SETPARTSTOREGISTER(a1, a2)     \
    (blpapi_ServiceRegistrationOptions_setPartsToRegister(a1, a2))

#define BLPAPI_CALL_SERVICEREGISTRATIONOPTIONS_GETPARTSTOREGISTER(a1)         \
    (blpapi_ServiceRegistrationOptions_getPartsToRegister(a1))

#define BLPAPI_CALL_PROVIDERSESSION_DELETETOPICS(a1, a2, a3)                 \
    (blpapi_ProviderSession_deleteTopics(a1, a2, a3))

#define BLPAPI_CALL_PROVIDERSESSION_ACTIVATESUBSERVICECODERANGE(              \
                                                          a1, a2, a3, a4, a5) \
    (blpapi_ProviderSession_activateSubServiceCodeRange)(a1, a2, a3, a4, a5)

#define BLPAPI_CALL_PROVIDERSESSION_DEACTIVATESUBSERVICECODERANGE(            \
                                                              a1, a2, a3, a4) \
    (blpapi_ProviderSession_deactivateSubServiceCodeRange)(a1, a2, a3, a4)

#define BLPAPI_CALL_SERVICEREGISTRATIONOPTIONS_ADDACTIVATESUBSERVICECODERANGE(\
                                                              a1, a2, a3, a4) \
    (blpapi_ServiceRegistrationOptions_addActiveSubServiceCodeRange)(         \
                                                                a1, a2, a3, a4)

#define                                                                       \
  BLPAPI_CALL_SERVICEREGISTRATIONOPTIONS_REMOVEALLACTIVESUBSERVICECODERANGES( \
                                                                          a1) \
    (blpapi_ServiceRegistrationOptions_removeAllActiveSubServiceCodeRanges)(a1)

#define BLPAPI_CALL_MESSAGE_TIMERECEIVED(a1, a2)                              \
    (blpapi_Message_timeReceived)(a1, a2)
#define BLPAPI_CALL_SESSIONOPTION_SETRECORDSUBSCRIPTIONDATARECEIVETIMES(a1,   \
                                                                        a2)   \
    (blpapi_SessionOptions_setRecordSubscriptionDataReceiveTimes)(a1, a2)
#define BLPAPI_CALL_SESSIONOPTION_RECORDSUBSCRIPTIONDATARECEIVETIMES(a1)      \
    (blpapi_SessionOptions_recordSubscriptionDataReceiveTimes)(a1)

#define BLPAPI_CALL_TIMEPOINTUTIL_NANOSECONDSBETWEEN(a1,a2)                   \
    (blpapi_TimePointUtil_nanosecondsBetween)(a1, a2)
#define BLPAPI_CALL_HIGHRESOLUTIONCLOCK_NOW(a1)                               \
    (blpapi_HighResolutionClock_now)(a1)
#define BLPAPI_CALL_HIGHPRECISIONDATETIME_FROMTIMEPOINT(a1, a2, a3)           \
    (blpapi_HighPrecisionDatetime_fromTimePoint)(a1, a2, a3)

#endif // if defined(_WIN32) || defined(__WIN32__)

#endif // __cplusplus

#endif // INCLUDED_BLPAPI_CALL
