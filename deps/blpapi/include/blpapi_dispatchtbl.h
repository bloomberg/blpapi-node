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
// blpapi_dispatchtbl.h                                               -*-C++-*-
#ifndef INCLUDED_BLPAPI_DISPATCHTBL
#define INCLUDED_BLPAPI_DISPATCHTBL

//@PURPOSE: Work with dispatch table
//
//@DESCRIPTION: This provides dispatch table for extended exported functions.
// These functions are called through dispatch table based on preprocessors. It
// avoids direct call of these functions by applications. So when blpapi DLL
// is rolled back to the previous versions, blpapi DLL still can be loaded.

// We only support dispatchtbl on windows currently
#if defined(_WIN32) || defined(__WIN32__)

#include <blpapi_defs.h>
#include <blpapi_correlationid.h>
#include <blpapi_versioninfo.h>
#include <stddef.h>

#ifdef __cplusplus

#ifndef BLPAPI_MIN_VERSION
#define BLPAPI_MIN_VERSION BLPAPI_SDK_VERSION
#endif

#ifndef BLPAPI_TARGET_VERSION
#define BLPAPI_TARGET_VERSION BLPAPI_SDK_VERSION
#endif

#if BLPAPI_MIN_VERSION > BLPAPI_TARGET_VERSION
#error "Min required version cannot be greater than target version"
#endif

extern "C" {

// Forward declarations
struct blpapi_EventFormatter;
typedef struct blpapi_EventFormatter blpapi_EventFormat_t;

struct blpapi_Topic;
typedef struct blpapi_Topic blpapi_Topic_t;

struct blpapi_Message;
typedef struct blpapi_Message blpapi_Message_t;

struct blpapi_Request;
typedef struct blpapi_Request blpapi_Request_t;

// End Forward declarations

// Function dispatch table declaration
typedef struct blpapi_FunctionEntries {
    int (*blpapi_EventFormatter_appendMessageSeq)(
        blpapi_EventFormatter_t* formatter,
        char const* typeString,
        blpapi_Name_t* typeName,
        const blpapi_Topic_t* topic,
        unsigned int sequenceNumber,
        unsigned int);
    int (*blpapi_EventFormatter_appendRecapMessageSeq)(
        blpapi_EventFormatter_t* formatter,
        const blpapi_Topic_t *topic,
        const blpapi_CorrelationId_t *cid,
        unsigned int sequenceNumber,
        unsigned int);
    int (*blpapi_Message_addRef)(
        const blpapi_Message_t* message);
    int (*blpapi_Message_release)(
        const blpapi_Message_t* message);
    void (*blpapi_SessionOptions_setMaxEventQueueSize)(
        blpapi_SessionOptions_t *parameters,
        size_t maxEventQueueSize);
    int (*blpapi_SessionOptions_setSlowConsumerWarningHiWaterMark)(
        blpapi_SessionOptions_t *parameters,
        float hiWaterMark);
    int (*blpapi_SessionOptions_setSlowConsumerWarningLoWaterMark)(
        blpapi_SessionOptions_t *parameters,
        float loWaterMark);
    void (*blpapi_Request_setPreferredRoute)(
        blpapi_Request_t *request,
        blpapi_CorrelationId_t *correlationId);
    int (*blpapi_Message_fragmentType)(
        const blpapi_Message_t *message);
    size_t (*blpapi_SessionOptions_maxEventQueueSize)(
        blpapi_SessionOptions_t *parameters);
    float (*blpapi_SessionOptions_slowConsumerWarningHiWaterMark)(
        blpapi_SessionOptions_t *parameters);
    float (*blpapi_SessionOptions_slowConsumerWarningLoWaterMark)(
        blpapi_SessionOptions_t *parameters);
    int (*blpapi_SessionOptions_setDefaultKeepAliveInactivityTime)(
        blpapi_SessionOptions_t *parameters,
        int inactivityTime);
    int (*blpapi_SessionOptions_setDefaultKeepAliveResponseTimeout)(
        blpapi_SessionOptions_t *parameters,
        int responseTimeout);
    int (*blpapi_SessionOptions_defaultKeepAliveInactivityTime)(
        blpapi_SessionOptions_t *parameters);
    int (*blpapi_SessionOptions_defaultKeepAliveResponseTimeout)(
        blpapi_SessionOptions_t *parameters);

} blpapi_FunctionEntries_t;

BLPAPI_EXPORT extern size_t                   g_blpapiFunctionTableSize;
BLPAPI_EXPORT extern blpapi_FunctionEntries_t g_blpapiFunctionEntries;

} // extern "C"

#endif // __cplusplus

#endif // if defined(_WIN32) || defined(__WIN32__)

#endif // INCLUDED_BLPAPI_DISPATCHTBL
