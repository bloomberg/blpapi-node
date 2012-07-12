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
// blpapi_request.h                                                   -*-C++-*-
#ifndef INCLUDED_BLPAPI_REQUEST
#define INCLUDED_BLPAPI_REQUEST

//@PURPOSE: Defines a request which can be sent for a service.
//
//@CLASSES:
// blpapi::Request: a request for a particular service
//
//@DESCRIPTION: This file defines a 'Request'. A generic Request object is
// created for a service using Service::createRequest() and sent over a session
// using Session::sendRequest().

#ifndef INCLUDED_BLPAPI_TYPES
#include <blpapi_types.h>
#endif

#ifndef INCLUDED_BLPAPI_CORRELATIONID
#include <blpapi_correlationid.h>
#endif

#ifndef INCLUDED_BLPAPI_ELEMENT
#include <blpapi_element.h>
#endif

#ifndef INCLUDED_BLPAPI_EXCEPTION
#include <blpapi_exception.h>
#endif

#ifndef INCLUDED_BLPAPI_DEFS
#include <blpapi_defs.h>
#endif

struct blpapi_Request;
typedef struct blpapi_Request blpapi_Request_t;

#ifdef __cplusplus
extern "C" {
#endif

BLPAPI_EXPORT
void blpapi_Request_destroy(
        blpapi_Request_t *request);

BLPAPI_EXPORT
blpapi_Element_t* blpapi_Request_elements(
        blpapi_Request_t *request);

BLPAPI_EXPORT
void blpapi_Request_setPreferredRoute(
        blpapi_Request_t *request,
        blpapi_CorrelationId_t *correlationId);

#ifdef __cplusplus
}

#ifndef INCLUDED_IOSFWD
#include <iosfwd>
#define INCLUDED_IOSFWD
#endif

namespace BloombergLP {
namespace blpapi {

// FORWARD DECLARATION
class RequestRef;
                         // =============
                         // class Request
                         // =============

class Request {
    // A single request to a single service.
    //
    // Request objects are created using Service::createRequest() or
    // Service::createAuthorizationRequest(). They are used with
    // Session::sendRequest() or Session::sendAuthorizationRequest().
    //
    // The Request object contains the parameters for a single request
    // to a single service. Once a Request has been created its fields
    // can be populated directly using the convenience functions set()
    // and append() functions or using the Element interface on the
    // Element returned by asElement().
    //
    // The schema for the Request can be queried using the Element
    // interface on the Element returned by asElement().
    //
    // Request objects cannot be duplicated. Copying a Request causes
    // the original Request to become invalid. The API provides
    // support for common idioms which require a normal copy
    // constructor. For example, you can write the following code.
    //..
    // Request request(service.createRequest("operationName"));
    //..
    // However, if you write this 'requestOne' will become invalid and
    // unusable.
    //..
    // Request requestOne(service.createRequest("operationName"));
    // Request requestTwo(requestOne);
    // // At this point requestOne is no longer valid
    //..
    // This should be written as follows.
    //..
    // Request requestOne(service.createRequest("operationName"));
    // Request requestTwo(service.createRequest("operationName"));
    //..

    blpapi_Request_t *d_handle;
    Element           d_elements;

    Request& operator=(const Request& rhs); // not implemented

  public:
    explicit Request(blpapi_Request_t *handle);

    Request(RequestRef src);
    Request(Request &src);
        // Initialize a Request from the specified non-const
        // 'src'. After this the 'src' Request is invalid.

    ~Request();
        // Destructor.

    // MANIPULATORS
    operator RequestRef();

    void set(const char* name, bool value);
        // Equivalent to asElement().set(name, value).

    void set(const char* name, char value);
        // Equivalent to asElement().set(name, value).

    void set(const char* name, Int32 value);
        // Equivalent to asElement().set(name, value).

    void set(const char* name, Int64 value);
        // Equivalent to asElement().set(name, value).

    void set(const char* name, Float32 value);
        // Equivalent to asElement().set(name, value).

    void set(const char* name, Float64 value);
        // Equivalent to asElement().set(name, value).

    void set(const char* name, const Datetime& value);
        // Equivalent to asElement().set(name, value).

    void set(const char* name, const char* value);
        // Equivalent to asElement().set(name, value).

    void set(const Name& name, bool value);
        // Equivalent to asElement().set(name, value).

    void set(const Name& name, char value);
        // Equivalent to asElement().set(name, value).

    void set(const Name& name, Int32 value);
        // Equivalent to asElement().set(name, value).

    void set(const Name& name, Int64 value);
        // Equivalent to asElement().set(name, value).

    void set(const Name& name, Float32 value);
        // Equivalent to asElement().set(name, value).

    void set(const Name& name, Float64 value);
        // Equivalent to asElement().set(name, value).

    void set(const Name& name, const Datetime& value);
        // Equivalent to asElement().set(name, value).

    void set(const Name& name, const char* value);
        // Equivalent to asElement().set(name, value).

    void append(const char* name, bool value);
        // Equivalent to asElement().append(name, value).

    void append(const char* name, char value);
        // Equivalent to asElement().append(name, value).

    void append(const char* name, Int32 value);
        // Equivalent to asElement().append(name, value).

    void append(const char* name, Int64 value);
        // Equivalent to asElement().append(name, value).

    void append(const char* name, Float32 value);
        // Equivalent to asElement().append(name, value).

    void append(const char* name, Float64 value);
        // Equivalent to asElement().append(name, value).

    void append(const char* name, const Datetime& value);
        // Equivalent to asElement().append(name, value).

    void append(const char* name, const char* value);
        // Equivalent to asElement().append(name, value).

    void append(const Name& name, bool value);
        // Equivalent to asElement().append(name, value).

    void append(const Name& name, char value);
        // Equivalent to asElement().append(name, value).

    void append(const Name& name, Int32 value);
        // Equivalent to asElement().append(name, value).

    void append(const Name& name, Int64 value);
        // Equivalent to asElement().append(name, value).

    void append(const Name& name, Float32 value);
        // Equivalent to asElement().append(name, value).

    void append(const Name& name, Float64 value);
        // Equivalent to asElement().append(name, value).

    void append(const Name& name, const Datetime& value);
        // Equivalent to asElement().append(name, value).

    void append(const Name& name, const char* value);
        // Equivalent to asElement().append(name, value).

    Element asElement();
        // Returns the contents of this request as a modifiable
        // Element.

    Element getElement(const char* name);
        // Equivalent to asElement().getElement(name).

    Element getElement(const Name& name);
        // Equivalent to asElement().getElement(name).


    // ACCESSORS

    const Element getElement(const char* name) const;
        // Equivalent to asElement().getElement(name).

    const Element getElement(const Name& name) const;
        // Equivalent to asElement().getElement(name).

    const Element asElement() const;
        // Returns the contents of this request as a read-only
        // Element.

    blpapi_Request_t* handle() const;

    std::ostream& print(std::ostream& stream,
                        int level=0,
                        int spacesPerLevel=4) const;
       // Format this Element to the specified output 'stream' at the
       // (absolute value of) the optionally specified indentation
       // 'level' and return a reference to 'stream'. If 'level' is
       // specified, optionally specify 'spacesPerLevel', the number
       // of spaces per indentation level for this and all of its
       // nested objects. If 'level' is negative, suppress indentation
       // of the first line. If 'spacesPerLevel' is negative, format
       // the entire output on one line, suppressing all but the
       // initial indentation (as governed by 'level').
};

// FREE OPERATORS
std::ostream& operator<<(std::ostream& stream, const Request &request);
    // Write the value of the specified 'request' object to the specified
    // output 'stream' in a single-line format, and return a reference to
    // 'stream'.  If 'stream' is not valid on entry, this operation has no
    // effect.  Note that this human-readable format is not fully specified,
    // can change without notice, and is logically equivalent to:
    //..
    //  print(stream, 0, -1);
    //..
                         // ================
                         // class RequestRef
                         // ================

class RequestRef {
    Request *d_request_p;

public:
    RequestRef(Request *request_p);
    Request* ptr() const;
    Request* operator->() const;
};

//=============================================================================
//                           INLINE FUNCTION DEFINITIONS
//=============================================================================

                            // -------------
                            // class Request
                            // -------------

inline
Request::Request(blpapi_Request_t *handle)
{
    d_handle = handle;
    if (handle) {
        d_elements.rebind(blpapi_Request_elements(handle));
    }
}

inline
Request::Request(RequestRef ref)
{
    Request *src = ref.ptr();
    d_handle = src->d_handle;
    d_elements = src->d_elements;
    src->d_handle = 0;
    src->d_elements.rebind(0);
}

inline
Request::Request(Request &src)
{
    d_handle = src.d_handle;
    d_elements = src.d_elements;
    src.d_handle = 0;
    src.d_elements.rebind(0);
}

inline
Request::~Request()
{
    if (d_handle) {
        blpapi_Request_destroy(d_handle);
    }
}

inline
Request::operator RequestRef()
{
    return this;
}

inline
void Request::set(const char* element, bool value)
{
    d_elements.setElement(element, value);
}

inline
void Request::set(const char* element, char value)
{
    d_elements.setElement(element, value);
}

inline
void Request::set(const char* element, Int32 value)
{
    d_elements.setElement(element, value);
}

inline
void Request::set(const char* element, Int64 value)
{
    d_elements.setElement(element, value);
}

inline
void Request::set(const char* element, Float32 value)
{
    d_elements.setElement(element, value);
}

inline
void Request::set(const char* element, Float64 value)
{
    d_elements.setElement(element, value);
}

inline
void Request::set(const char* element, const Datetime& value)
{
    d_elements.setElement(element, value);
}

inline
void Request::set(const char* element, const char* value)
{
    d_elements.setElement(element, value);
}

inline
void Request::append(const char* element, bool value)
{
    d_elements.getElement(element).appendValue(value);
}

inline
void Request::append(const char* element, char value)
{
    d_elements.getElement(element).appendValue(value);
}

inline
void Request::append(const char* element, Int32 value)
{
    d_elements.getElement(element).appendValue(value);
}

inline
void Request::append(const char* element, Int64 value)
{
    d_elements.getElement(element).appendValue(value);
}

inline
void Request::append(const char* element, Float32 value)
{
    d_elements.getElement(element).appendValue(value);
}

inline
void Request::append(const char* element, Float64 value)
{
    d_elements.getElement(element).appendValue(value);
}

inline
void Request::append(const char* element, const Datetime& value)
{
    d_elements.getElement(element).appendValue(value);
}

inline
void Request::append(const char* element, const char* value)
{
    d_elements.getElement(element).appendValue(value);
}

inline
void Request::set(const Name& element, bool value)
{
    d_elements.setElement(element, value);
}

inline
void Request::set(const Name& element, char value)
{
    d_elements.setElement(element, value);
}

inline
void Request::set(const Name& element, Int32 value)
{
    d_elements.setElement(element, value);
}

inline
void Request::set(const Name& element, Int64 value)
{
    d_elements.setElement(element, value);
}

inline
void Request::set(const Name& element, Float32 value)
{
    d_elements.setElement(element, value);
}

inline
void Request::set(const Name& element, Float64 value)
{
    d_elements.setElement(element, value);
}

inline
void Request::set(const Name& element, const Datetime& value)
{
    d_elements.setElement(element, value);
}

inline
void Request::set(const Name& element, const char* value)
{
    d_elements.setElement(element, value);
}

inline
void Request::append(const Name& element, bool value)
{
    d_elements.getElement(element).appendValue(value);
}

inline
void Request::append(const Name& element, char value)
{
    d_elements.getElement(element).appendValue(value);
}

inline
void Request::append(const Name& element, Int32 value)
{
    d_elements.getElement(element).appendValue(value);
}

inline
void Request::append(const Name& element, Int64 value)
{
    d_elements.getElement(element).appendValue(value);
}

inline
void Request::append(const Name& element, Float32 value)
{
    d_elements.getElement(element).appendValue(value);
}

inline
void Request::append(const Name& element, Float64 value)
{
    d_elements.getElement(element).appendValue(value);
}

inline
void Request::append(const Name& element, const Datetime& value)
{
    d_elements.getElement(element).appendValue(value);
}

inline
void Request::append(const Name& element, const char* value)
{
    d_elements.getElement(element).appendValue(value);
}


inline
Element Request::getElement(const char* name)
{
    return d_elements.getElement(name);
}

inline
Element Request::getElement(const Name& name)
{
    return d_elements.getElement(name);
}

inline
Element Request::asElement()
{
    return d_elements;
}


inline
const Element Request::getElement(const char* name) const
{
    return d_elements.getElement(name);
}

inline
const Element Request::getElement(const Name& name) const
{
    return d_elements.getElement(name);
}

inline
const Element Request::asElement() const
{
    return d_elements;
}

inline
blpapi_Request_t* Request::handle() const
{
    return d_handle;
}

inline
std::ostream& Request::print(
        std::ostream& stream,
        int level,
        int spacesPerLevel) const
{
    return d_elements.print(stream, level, spacesPerLevel);
}

inline
std::ostream& operator<<(std::ostream& stream, const Request &request)
{
    return request.print(stream, 0,-1);
}
                            // ----------------
                            // class RequestRef
                            // ----------------

inline
RequestRef::RequestRef(Request *request)
: d_request_p(request)
{
}

inline
Request* RequestRef::ptr() const
{
    return this->d_request_p;
}

inline
Request* RequestRef::operator->() const
{
    return this->d_request_p;
}

}  // close namespace blpapi
}  // close namespace BloombergLP

#endif // #ifdef __cplusplus

#endif // #ifndef INCLUDED_BLPAPI_REQUEST
