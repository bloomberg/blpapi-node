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
// blpapi_exception.h                                                 -*-C++-*-
#ifndef INCLUDED_BLPAPI_EXCEPTION
#define INCLUDED_BLPAPI_EXCEPTION

//@PURPOSE: Defines Exceptions that can be thrown by the blpapi library.
//
//@CLASSES:
//                        blpapi_ErrorInfo: C struct to get more info on error
//                       blpapi::Exception: Base class from all exceptions
// blpapi::DuplicateCorrelationIdException: Duplicate CorrelationId exception
//           blpapi::InvalidStateException: Invalid state exception
//        blpapi::InvalidArgumentException: Invalid argument exception
//      blpapi::InvalidConversionException: Invalid conversion exception
//        blpapi::IndexOutOfRangeException: Index out of range exception
//          blpapi::FieldNotFoundException: Field not found exception
//           blpapi::UnknownErrorException: Unknown error exception
//   blpapi::UnsupportedOperationException: Unsupported operation exception
//                   blpapi::ExceptionUtil: Internal exception generating class
//
//@DESCRIPTION: This file defines various exceptions that blpapi can throw.
//

#ifndef INCLUDED_BLPAPI_DEFS
#include <blpapi_defs.h>
#endif

#ifndef INCLUDED_BLPAPI_ERROR
#include <blpapi_error.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct blpapi_ErrorInfo {
    int   exceptionClass;
    char  description[256];
};

typedef struct blpapi_ErrorInfo blpapi_ErrorInfo_t;

BLPAPI_EXPORT
int blpapi_getErrorInfo(blpapi_ErrorInfo_t *buffer, int errorCode);

#ifdef __cplusplus
} // extern "C"


#ifndef INCLUDED_STRING
#include <string>
#define INCLUDED_STRING
#endif

namespace BloombergLP {
namespace blpapi {

class Exception {
    std::string d_description;

  public:
    Exception(const std::string description)
        : d_description(description)
    {
    }

    const std::string& description() const
    {
        return d_description;
    }
};

class DuplicateCorrelationIdException : public Exception {
  public:
    DuplicateCorrelationIdException(const std::string &description)
        : Exception(description)
    {
    }
};

class InvalidStateException: public Exception {
  public:
    InvalidStateException(const std::string &description)
        : Exception(description)
    {
    }
};

class InvalidArgumentException: public Exception {
  public:
    InvalidArgumentException(const std::string &description)
        : Exception(description)
    {
    }
};

class InvalidConversionException: public Exception {
  public:
    InvalidConversionException(const std::string &description)
        : Exception(description)
    {
    }
};
// XXX: compatibility typedef; to be removed in version 3.5
typedef InvalidConversionException InvalidConverstionException;

class IndexOutOfRangeException: public Exception {
  public:
    IndexOutOfRangeException(const std::string& description)
        : Exception(description)
    {
    }
};

class FieldNotFoundException: public Exception {
  public:
    FieldNotFoundException(const std::string& description)
        : Exception(description)
    {
    }
};

class UnknownErrorException: public Exception {
  public:
    UnknownErrorException(const std::string& description)
        : Exception(description)
    {
    }
};

class UnsupportedOperationException: public Exception {
  public:
    UnsupportedOperationException(const std::string& description)
        : Exception(description)
    {
    }
};


class ExceptionUtil {
  private:
    static void throwException(int errorCode)
    {
        const char* description = blpapi_getLastErrorDescription(errorCode);
        if (!description) {
            description = "Unknown";
        }

        if (BLPAPI_ERROR_DUPLICATE_CORRELATIONID == errorCode) {
            throw DuplicateCorrelationIdException(description);
        }

        switch (BLPAPI_RESULTCLASS(errorCode))
          case BLPAPI_INVALIDSTATE_CLASS: {
            throw InvalidStateException(description);
          case BLPAPI_INVALIDARG_CLASS:
            throw InvalidArgumentException(description);
          case BLPAPI_CNVERROR_CLASS:
            throw InvalidConversionException(description);
          case BLPAPI_BOUNDSERROR_CLASS:
            throw IndexOutOfRangeException(description);
          case BLPAPI_FLDNOTFOUND_CLASS:
            throw FieldNotFoundException(description);
          case BLPAPI_UNSUPPORTED_CLASS:
            throw UnsupportedOperationException(description);
          default:
            throw Exception(description);
        }
    }

  public:
    static void throwOnError(int errorCode);
};


inline
void ExceptionUtil::throwOnError(int errorCode)
{
    if (errorCode) {
        throwException(errorCode);
    }
}

}  // close namespace blpapi {
}  // close namespace BloombergLP {

#endif

#endif // #ifndef INCLUDED_BLPAPI_EXCEPTION
