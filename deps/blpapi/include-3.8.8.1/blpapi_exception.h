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
//               blpapi::NotFoundException: Not present exception
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


#ifndef INCLUDED_EXCEPTION
#include <exception>
#define INCLUDED_EXCEPTION
#endif

#ifndef INCLUDED_STRING
#include <string>
#define INCLUDED_STRING
#endif

namespace BloombergLP {
namespace blpapi {
                            // ===============
                            // class Exception
                            // ===============

class Exception : public std::exception {
    // This class defines a base exception for blpapi operations.  Objects of
    // this class contain the error description for the exception.

    // DATA
    const std::string d_description;

  private:
    // NOT IMPLEMENTED
    Exception& operator=(const Exception&);  // = delete

  public:
    // CREATORS
    explicit Exception(const std::string& description);
        // Create an exception object initialized with the specified
        // 'description'.

    // ACCESSORS
    const std::string& description() const throw();
        // Return the error description supplied at construction.

    virtual const char* what() const throw();
        // Return the error description supplied at construction as a
        // null-terminated character sequence.

    virtual ~Exception() throw();
        // Destroy this object.
};
                   // =====================================
                   // class DuplicateCorrelationIdException
                   // =====================================

class DuplicateCorrelationIdException : public Exception {
    // The class defines an exception for non unqiue 'blpapi::CorrelationId'.
  public:
    // CREATORS
    explicit DuplicateCorrelationIdException(const std::string &description);
        // Create an exception object initialized with the specified
        // 'description'.
};

                        // ===========================
                        // class InvalidStateException
                        // ===========================

class InvalidStateException: public Exception {
    // This class defines an exception for calling methods on an object that is
    // not in a valid state.
  public:
    // CREATORS
    explicit InvalidStateException(const std::string &description);
        // Create an exception object initialized with the specified
        // 'description'.
};

                       // ==============================
                       // class InvalidArgumentException
                       // ==============================

class InvalidArgumentException: public Exception {
    // This class defines an exception for invalid arguments on method
    // invocations.
  public:
    // CREATORS
    explicit InvalidArgumentException(const std::string& description);
        // Create an exception object initialized with the specified
        // 'description'.
};

                      // ================================
                      // class InvalidConversionException
                      // ================================

class InvalidConversionException: public Exception {
    // This class defines an exception for invalid conversion of data.
  public:
    // CREATORS
    explicit InvalidConversionException(const std::string& description);
        // Create an exception object initialized with the specified
        // 'description'.
};

                       // ==============================
                       // class IndexOutOfRangeException
                       // ==============================

class IndexOutOfRangeException: public Exception {
    // This class defines an exception to capture the error when an invalid
    // index is used for an operation that needs index.
  public:
    // CREATORS
    explicit IndexOutOfRangeException(const std::string& description);
        // Create an exception object initialized with the specified
        // 'description'.
};

                        // ============================
                        // class FieldNotFoundException
                        // ============================

class FieldNotFoundException: public Exception {
    // This class defines an exception to capture the error when an invalid
    // field is used for operation.
    // DEPRECATED
  public:
    // CREATORS
    explicit FieldNotFoundException(const std::string& description);
        // Create an exception object initialized with the specified
        // 'description'.
};

                        // ===========================
                        // class UnknownErrorException
                        // ===========================

class UnknownErrorException: public Exception {
    // This class defines an exception for errors that do not fall in any
    // predefined category.
  public:
    // CREATORS
    explicit UnknownErrorException(const std::string& description);
        // Create an exception object initialized with the specified
        // 'description'.
};

                    // ===================================
                    // class UnsupportedOperationException
                    // ===================================

class UnsupportedOperationException: public Exception {
    // This class defines an exception for unsupported operations.
  public:
    // CREATORS
    explicit UnsupportedOperationException(const std::string& description);
        // Create an exception object initialized with the specified
        // 'description'.
};

                          // =======================
                          // class NotFoundException
                          // =======================

class NotFoundException: public Exception {
    // This class defines an exception to capture the error when an item is
    // not found for an operation.
  public:
    // CREATORS
    explicit NotFoundException(const std::string& description);
        // Create an exception object initialized with the specified
        // 'description'.
};

                            // ===================
                            // class ExceptionUtil
                            // ===================

class ExceptionUtil {
    // This class provides a namespace for utility functions that convert
    // C-style error codes to 'blpapi::Exception' objects.

  private:
    static void throwException(int errorCode);
        // Throw the appropriate exception for the specified 'errorCode'.

  public:
    static void throwOnError(int errorCode);
        // Throw the appropriate exception for the specified 'errorCode' if the
        // errorCode is not 0.
};

// ============================================================================
//                        INLINE FUNCTION DEFINITIONS
// ============================================================================

                              // ---------------
                              // class Exception
                              // ---------------

inline
Exception::Exception(const std::string& newDescription)
: d_description(newDescription)
{
}

inline
Exception::~Exception() throw()
{
}

inline
const std::string& Exception::description() const throw()
{
    return d_description;
}

inline
const char* Exception::what() const throw()
{
    return description().c_str();
}

                   // -------------------------------------
                   // class DuplicateCorrelationIdException
                   // -------------------------------------

inline
DuplicateCorrelationIdException::DuplicateCorrelationIdException(
                                             const std::string& newDescription)
: Exception(newDescription)
{
}

                        // ---------------------------
                        // class InvalidStateException
                        // ---------------------------

inline
InvalidStateException::InvalidStateException(const std::string& newDescription)
: Exception(newDescription)
{
}

                       // ------------------------------
                       // class InvalidArgumentException
                       // ------------------------------

inline
InvalidArgumentException::InvalidArgumentException(
                                             const std::string& newDescription)
: Exception(newDescription)
{
}

                      // --------------------------------
                      // class InvalidConversionException
                      // --------------------------------

inline
InvalidConversionException::InvalidConversionException(
                                             const std::string& newDescription)
: Exception(newDescription)
{
}

                       // ------------------------------
                       // class IndexOutOfRangeException
                       // ------------------------------

inline
IndexOutOfRangeException::IndexOutOfRangeException(
                                             const std::string& newDescription)
: Exception(newDescription)
{
}

                        // ----------------------------
                        // class FieldNotFoundException
                        // ----------------------------

inline
FieldNotFoundException::FieldNotFoundException(
                                             const std::string& newDescription)
: Exception(newDescription)
{
}

                        // ---------------------------
                        // class UnknownErrorException
                        // ---------------------------

inline
UnknownErrorException::UnknownErrorException(const std::string& newDescription)
: Exception(newDescription)
{
}

                    // -----------------------------------
                    // class UnsupportedOperationException
                    // -----------------------------------

inline
UnsupportedOperationException::UnsupportedOperationException(
                                             const std::string& newDescription)
: Exception(newDescription)
{
}

                          // -----------------------
                          // class NotFoundException
                          // -----------------------

inline
NotFoundException::NotFoundException(const std::string& newDescription)
: Exception (newDescription)
{
}

                            // -------------------
                            // class ExceptionUtil
                            // -------------------

inline
void ExceptionUtil::throwException(int errorCode)
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
      case BLPAPI_NOTFOUND_CLASS:
        throw NotFoundException(description);
      default:
        throw Exception(description);
    }
}

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
