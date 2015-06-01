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
// blpapi_types.h                                                     -*-C++-*-
#ifndef INCLUDED_BLPAPI_TYPES
#define INCLUDED_BLPAPI_TYPES

//@PURPOSE: Provide BLPAPI types.
//
//@CLASSES:
// blpapi::DataType: Data types in the schema
//
//@DESCRIPTION: This component implements BLPAPI types.

typedef int                 blpapi_Bool_t;
typedef char                blpapi_Char_t;
typedef unsigned char       blpapi_UChar_t;
typedef short               blpapi_Int16_t;
typedef unsigned short      blpapi_UInt16_t;
typedef int                 blpapi_Int32_t;
typedef unsigned int        blpapi_UInt32_t;
typedef long long           blpapi_Int64_t;
typedef unsigned long long  blpapi_UInt64_t;
typedef float               blpapi_Float32_t;
typedef double              blpapi_Float64_t;


enum blpapi_DataType_t {
        BLPAPI_DATATYPE_BOOL           = 1     // Bool
      , BLPAPI_DATATYPE_CHAR           = 2     // Char
      , BLPAPI_DATATYPE_BYTE           = 3     // Unsigned 8 bit value
      , BLPAPI_DATATYPE_INT32          = 4     // 32 bit Integer
      , BLPAPI_DATATYPE_INT64          = 5     // 64 bit Integer
      , BLPAPI_DATATYPE_FLOAT32        = 6     // 32 bit Floating point - IEEE
      , BLPAPI_DATATYPE_FLOAT64        = 7     // 64 bit Floating point - IEEE
      , BLPAPI_DATATYPE_STRING         = 8     // ASCIIZ string
      , BLPAPI_DATATYPE_BYTEARRAY      = 9     // Opaque binary data
      , BLPAPI_DATATYPE_DATE           = 10    // Date
      , BLPAPI_DATATYPE_TIME           = 11    // Timestamp
      , BLPAPI_DATATYPE_DECIMAL        = 12    //
      , BLPAPI_DATATYPE_DATETIME       = 13    // Date and time
      , BLPAPI_DATATYPE_ENUMERATION    = 14    // An opaque enumeration
      , BLPAPI_DATATYPE_SEQUENCE       = 15    // Sequence type
      , BLPAPI_DATATYPE_CHOICE         = 16    // Choice type
      , BLPAPI_DATATYPE_CORRELATION_ID = 17    // Used for some internal
                                               // messages
};

typedef enum blpapi_Logging_Severity_t {
    blpapi_Logging_SEVERITY_OFF   = 0,
    blpapi_Logging_SEVERITY_FATAL = 1,
    blpapi_Logging_SEVERITY_ERROR = 2,
    blpapi_Logging_SEVERITY_WARN  = 3,
    blpapi_Logging_SEVERITY_INFO  = 4,
    blpapi_Logging_SEVERITY_DEBUG = 5,
    blpapi_Logging_SEVERITY_TRACE = 6
} blpapi_Logging_Severity_t;

struct blpapi_AbstractSession;
typedef struct blpapi_AbstractSession blpapi_AbstractSession_t;

struct blpapi_Constant;
typedef struct blpapi_Constant blpapi_Constant_t;

struct blpapi_ConstantList;
typedef struct blpapi_ConstantList blpapi_ConstantList_t;

struct blpapi_Element;
typedef struct blpapi_Element blpapi_Element_t;

struct blpapi_Event;
typedef struct blpapi_Event blpapi_Event_t;

struct blpapi_EventDispatcher;
typedef struct blpapi_EventDispatcher blpapi_EventDispatcher_t;

struct blpapi_EventFormatter;
typedef struct blpapi_EventFormatter blpapi_EventFormatter_t;

struct blpapi_EventQueue;
typedef struct blpapi_EventQueue blpapi_EventQueue_t;

struct blpapi_MessageIterator;
typedef struct blpapi_MessageIterator blpapi_MessageIterator_t;

struct blpapi_Name;
typedef struct blpapi_Name blpapi_Name_t;

struct blpapi_Operation;
typedef struct blpapi_Operation blpapi_Operation_t;

struct blpapi_ProviderSession;
typedef struct blpapi_ProviderSession blpapi_ProviderSession_t;

struct blpapi_Service;
typedef struct blpapi_Service blpapi_Service_t;

struct blpapi_Session;
typedef struct blpapi_Session blpapi_Session_t;

struct blpapi_SessionOptions;
typedef struct blpapi_SessionOptions blpapi_SessionOptions_t;

struct blpapi_SubscriptionItrerator;
typedef struct blpapi_SubscriptionItrerator blpapi_SubscriptionIterator_t;

struct blpapi_Identity;
typedef struct blpapi_Identity blpapi_UserHandle;
typedef struct blpapi_Identity blpapi_UserHandle_t;

struct blpapi_Identity;
typedef struct blpapi_Identity blpapi_Identity_t;

#ifdef __cplusplus

namespace BloombergLP {
namespace blpapi {
                         // ===============
                         // struct DataType
                         // ===============

struct DataType {
    // Contains the possible data types which can be represented in an
    // Element.

  public:
    enum Value {
      // The possible data types which can be represented in an Element.
      BOOL           = BLPAPI_DATATYPE_BOOL,           // Boolean
      CHAR           = BLPAPI_DATATYPE_CHAR,           // Char
      BYTE           = BLPAPI_DATATYPE_BYTE,           // Unsigned 8 bit value
      INT32          = BLPAPI_DATATYPE_INT32,          // 32 bit Integer
      INT64          = BLPAPI_DATATYPE_INT64,          // 64 bit Integer
      FLOAT32        = BLPAPI_DATATYPE_FLOAT32,        // 32 bit Floating point
      FLOAT64        = BLPAPI_DATATYPE_FLOAT64,        // 64 bit Floating point
      STRING         = BLPAPI_DATATYPE_STRING,         // ASCIIZ string
      BYTEARRAY      = BLPAPI_DATATYPE_BYTEARRAY,      // Opaque binary data
      DATE           = BLPAPI_DATATYPE_DATE,           // Date
      TIME           = BLPAPI_DATATYPE_TIME,           // Timestamp
      DECIMAL        = BLPAPI_DATATYPE_DECIMAL,        // Currently Unsupported
      DATETIME       = BLPAPI_DATATYPE_DATETIME,       // Date and time
      ENUMERATION    = BLPAPI_DATATYPE_ENUMERATION,    // An opaque enumeration
      SEQUENCE       = BLPAPI_DATATYPE_SEQUENCE,       // Sequence type
      CHOICE         = BLPAPI_DATATYPE_CHOICE,         // Choice type
      CORRELATION_ID = BLPAPI_DATATYPE_CORRELATION_ID  // Used for some
                                                       // internal messages
    };
};

typedef blpapi_Bool_t Bool;
typedef blpapi_Char_t Char;
typedef blpapi_UChar_t UChar;
typedef blpapi_Int16_t Int16;
typedef blpapi_UInt16_t UInt16;
typedef blpapi_Int32_t Int32;
typedef blpapi_UInt32_t UInt32;
typedef blpapi_Int64_t Int64;
typedef blpapi_UInt64_t UInt64;
typedef blpapi_Float32_t Float32;
typedef blpapi_Float64_t Float64;

}  // close namespace blpapi
}  // close namespace BloombergLP

#endif // ifdef __cplusplus

#endif // #ifndef INCLUDED_BLPAPI_TYPES
