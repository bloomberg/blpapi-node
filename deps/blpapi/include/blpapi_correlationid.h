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
// blpapi_correlationid.h                                             -*-C++-*-
#ifndef INCLUDED_BLPAPI_CORRELATIONID
#define INCLUDED_BLPAPI_CORRELATIONID

//@PURPOSE: Provide a key to identify individual subscriptions or requests
//
//@CLASSES:
// blpapi::CorrelationId: a key to track requests and subscriptions
//
//@DESCRIPTION: This component provides an identifier that is attached on to
// individual subscriptions and requests. CorrelationId are used to distinguish
// between various subscriptions and are a way to find the response for an
// asynchronous request.

#ifndef INCLUDED_BLPAPI_TYPES
#include <blpapi_types.h>
#endif

#ifndef INCLUDED_BLPAPI_DEFS
#include <blpapi_defs.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct blpapi_ManagedPtr_t_;
typedef struct blpapi_ManagedPtr_t_ blpapi_ManagedPtr_t;

typedef int (*blpapi_ManagedPtr_ManagerFunction_t)(
    blpapi_ManagedPtr_t *managedPtr,
    const blpapi_ManagedPtr_t *srcPtr,
    int operation);

typedef union {
    int   intValue;
    void *ptr;
} blpapi_ManagedPtr_t_data_;

struct blpapi_ManagedPtr_t_ {
    void *pointer;
    blpapi_ManagedPtr_t_data_ userData[4];
    blpapi_ManagedPtr_ManagerFunction_t  manager;
};

typedef struct blpapi_CorrelationId_t_ {
    unsigned int  size:8;       // fill in the size of this struct
    unsigned int  valueType:4;  // type of value held by this correlation id
    unsigned int  classId:16;   // user defined classification id
    unsigned int  reserved:4;   // for internal use must be 0

    union {
        blpapi_UInt64_t      intValue;
        blpapi_ManagedPtr_t  ptrValue;
    } value;
} blpapi_CorrelationId_t;


#ifdef __cplusplus
}

#ifndef INCLUDED_CSTRING
#include <cstring>
#define INCLUDED_CSTRING
#endif

#ifndef INCLUDED_IOSTREAM
#include <iostream>
#define INCLUDED_IOSTREAM
#endif

#ifndef INCLUDED_ALGORITHM
#include <algorithm> // for swap
#define INCLUDED_ALGORITHM
#endif

namespace BloombergLP {
namespace blpapi {

                         // ===================
                         // class CorrelationId
                         // ===================

class CorrelationId {
    // A key used to identify individual subscriptions or requests.
    //
    // CorrelationId objects are passed to many of the Session object
    // methods which initiate an asynchronous operations and are
    // obtained from Message objects which are delivered as a result
    // of those asynchronous operations.
    //
    // When subscribing or requesting information an application has
    // the choice of providing a CorrelationId they construct
    // themselves or allowing the session to construct one for
    // them. If the application supplies a CorrelationId it must not
    // re-use the value contained in it in another CorrelationId
    // whilst the original request or subscription is still active.
    //
    // It is possible that an application supplied CorrelationId and a
    // CorrelationId constructed by the API could return the same
    // result for asInteger(). However, they will not compare equal
    // using the defined operator== for CorrelationId and there is a
    // consistent order defined using the defined operator< for
    // CorrelationId.
    //
    // A CorrelationId constructed by an application can contain either
    //
    // - a 64 bit integer,
    //
    // - a simple pointer or
    //
    // - a "smart" pointer object (for example, tr1::shared_ptr)
    //
    // For 64 bit integers and simple pointers the values are copied
    // when CorrelationIds are copied and compared when CorrelationIds
    // are compared.
    //
    // For "smart" pointers the API can accommodate smart pointer classes
    // that meet the following restrictions.
    //
    // - It is no more than sizeof(void*)*4 bytes in size.
    //
    // - It is thread safe.
    //
    // - It performs all its necessary management as a direct side
    // effect of their copy constructor and destructor.
    //
    // - Its contents are location independent (that is given two
    // instances of a smart pointer s1 and s2 one can call std::swap(s1,
    // s2) ).
    //
    // The API will embed a smart pointer in the CorrelationId without
    // allocating memory separately for it. The specified 'smartPtr'
    // will have its copy constructor invoked when the CorrelationId
    // is copied and its destructor invoked when the CorrelationId is
    // destroyed so its resource management will continue to work as
    // normal.
    //
    // CorrelationId's based on a simple pointer and CorrelationId's
    // based on a smart pointer have the same ValueType
    // (POINTER_VALUE) which allows them to be compared to each other.
    //
    // A CorrelationId based on a simple pointer and a CorrelationId
    // based on a smart pointer will compare equally with operator==
    // as long as the pointer is the same.
    //
    // Likewise, when comparing two CorrelationId's based on a smart
    // pointer only the pointer value itself is used for the
    // comparison, the contents of the smart pointer object are
    // ignored.

    blpapi_CorrelationId_t d_impl;

    void copy(const blpapi_CorrelationId_t& src);

    template <typename TYPE>
    static int managerFunc(blpapi_ManagedPtr_t *managedPtr,
        const blpapi_ManagedPtr_t *srcPtr, int operation);

    template <typename TYPE>
    static void assertSmartPtrFits();

  public:
    // Possible return values for valueType() method.

    enum ValueType {
      UNSET_VALUE   = BLPAPI_CORRELATION_TYPE_UNSET,
          // The CorrelationId is unset. That is, it was created by the default
          // CorrelationId constructor.
      INT_VALUE     = BLPAPI_CORRELATION_TYPE_INT,
          // The CorrelationId was created from an integer supplied by the
          // user.
      POINTER_VALUE = BLPAPI_CORRELATION_TYPE_POINTER,
          // The CorrelationId was created from a pointer supplied by the user.
      AUTOGEN_VALUE = BLPAPI_CORRELATION_TYPE_AUTOGEN
          // The CorrelationId was created internally by API.
    };

    // The maximum value allowed for classId

    enum {
        MAX_CLASS_ID = BLPAPI_CORRELATION_MAX_CLASS_ID // The maximum value
                                                       // allowed for classId
    };

    CorrelationId();
        // The default constructor creates an unitialized
        // CorrelationId. This will compare equal to another
        // CorrelationId object constructed using the default
        // constructor. The only valid operations on an unitialized
        // CorrelationId are assignment, comparison for equality and
        // destruction.

    CorrelationId(const blpapi_CorrelationId_t &correlation);

    CorrelationId(const CorrelationId& original);
        // Copy constructor. If the specified 'original' contains a
        // smart pointer it will be copy constructed into this
        // CorrelationId.

    explicit CorrelationId(long long value, int classId = 0);
        // Construct a CorrelationId object and initialize it with the
        // specified integer 'value'.

    explicit CorrelationId(void *value, int classId = 0);
        // Construct a CorrelationId object and initialize it with the
        // specified pointer 'value'.

    template<typename TYPE>
    CorrelationId(const TYPE& smartPtr, void *pointerValue, int classId = 0);
        // Construct a CorrelationId object and initialize it with the
        // specified 'smartPtr' (whose copy constructor will be called
        // in the process) and with the specified 'pointerValue' which
        // should be the result of operator-> on the specified
        // 'smartPtr'.

    ~CorrelationId();
        // Destroy this CorrelationId. If this CorrelationId contains
        // a smart pointer its destructor will be called.

    // MANIPULATORS

    blpapi_CorrelationId_t& impl();

    void swap(CorrelationId &other);
        // Swap the value of this CorrelationId object and the
        // specified 'other' CorrelationId object.

    CorrelationId& operator=(const CorrelationId &rhs);
        // Assign to this CorrelationId object the value of the
        // specified 'rhs' CorrelationId object.  Return a modifiable
        // reference to this object.

    // ACCESSORS

    ValueType valueType() const;
        // Return the type of this CorrelationId object.

    unsigned short classId() const;
        // Return the user defined classification of this correlation
        // correlation id object.

    void* asPointer() const;
        // Return the value of this CorrelationId as a pointer
        // value. The result is undefined if this CorrelationId does
        // not have valueType()==POINTER_VALUE.

    template<typename TYPE>
    TYPE asSmartPointer() const;
        // Return the CorrelationId as a smart pointer.

    long long asInteger() const;
        // Return the value of this CorrelationId as an integer
        // value. The result is undefined if this CorrelationId does
        // not have valueType()==INT_VALUE or
        // valueType()==AUTOGEN_VALUE.

    const blpapi_CorrelationId_t& impl() const;
};

// FREE OPERATORS
inline
bool operator==(const CorrelationId& lhs, const CorrelationId& rhs);
    // Return true if the specified 'lhs' and 'rhs' CorrelationId
    // objects contain the same value. Return false otherwise. Two
    // CorrelationId objects contain the same value if the result of
    // valueType() is the same and the result of asPointer() or
    // asInteger() as appropriate is also the same.

inline
bool operator!=(const CorrelationId& lhs, const CorrelationId& rhs);
    // Equivalent to !(lhs==rhs).

inline
bool operator<(const CorrelationId& lhs, const CorrelationId& rhs);
    // Return true if the value of the specified 'lhs' CorrelationId
    // object is less than the value of the specified 'rhs'
    // CorrelationId object. Return false otherwise. The operator
    // takes account of the value of the CorrelationId as well as its
    // valueType() to ensure a consistent ordering amongst
    // CorrelationIds regardless of whether they are generated by the
    // API or the user. The operator is provided solely for
    // convenience for operations such as insertion into ordered
    // containers.

//=============================================================================
//                           INLINE FUNCTION DEFINITIONS
//=============================================================================

                            // -------------------
                            // class CorrelationId
                            // -------------------

inline
CorrelationId::CorrelationId()
{
    std::memset(&d_impl, 0, sizeof(d_impl));
}

inline
CorrelationId::CorrelationId(const blpapi_CorrelationId_t &correlationId)
{
    copy(correlationId);
}

inline
CorrelationId::CorrelationId(const CorrelationId& original)
{
    copy(original.d_impl);
}

inline
CorrelationId::CorrelationId(long long intValue, int classId)
{
    std::memset(&d_impl, 0, sizeof(d_impl));

    d_impl.size           = sizeof(d_impl);
    d_impl.valueType      = INT_VALUE;
    d_impl.value.intValue = intValue;
    d_impl.classId        = classId;
}

inline
CorrelationId::CorrelationId(void *ptrValue, int classId)
{
    std::memset(&d_impl, 0, sizeof(d_impl));

    d_impl.size                   = sizeof(d_impl);
    d_impl.valueType              = POINTER_VALUE;
    d_impl.value.ptrValue.pointer = ptrValue;
    d_impl.classId                = classId;
}

template <typename TYPE>
inline
CorrelationId::CorrelationId(const TYPE& smartPtr, void *ptrValue,
    int classId)
{
    // If you get a compiler error here, the specified smart pointer does not
    // fit in the CorrelationId and cannot be used at this time.

    assertSmartPtrFits<TYPE>();

    std::memset(&d_impl, 0, sizeof(d_impl));

    d_impl.size           = sizeof(d_impl);
    d_impl.valueType      = POINTER_VALUE;
    d_impl.classId        = classId;

    d_impl.value.ptrValue.pointer = ptrValue;
    d_impl.value.ptrValue.manager = &CorrelationId::managerFunc<TYPE>;

    void *arena = (void *)d_impl.value.ptrValue.userData;
    new (arena) TYPE(smartPtr);
}

inline
CorrelationId::~CorrelationId()
{
    if (POINTER_VALUE == valueType()) {
        blpapi_ManagedPtr_ManagerFunction_t &manager =
            d_impl.value.ptrValue.manager;
        if (manager) {
            manager(&d_impl.value.ptrValue, 0, BLPAPI_MANAGEDPTR_DESTROY);
        }
    }
}

inline
void CorrelationId::swap(CorrelationId &other)
{
    std::swap(other.d_impl, d_impl);
}

inline
CorrelationId& CorrelationId::operator=(const CorrelationId &rhs)
{
    if (&rhs == this) {
        return *this;
    }
    CorrelationId tmp(rhs);
    tmp.swap(*this);
    return *this;
}

inline
blpapi_CorrelationId_t& CorrelationId::impl()
{
    return d_impl;
}

inline
CorrelationId::ValueType CorrelationId::valueType() const
{
    return (ValueType)d_impl.valueType;
}

inline
unsigned short CorrelationId::classId() const
{
    return d_impl.classId;
}

inline
void* CorrelationId::asPointer() const
{
    return d_impl.value.ptrValue.pointer;
}

template<typename TYPE>
inline
TYPE CorrelationId::asSmartPointer() const
{
    return *(TYPE *)d_impl.value.ptrValue.userData;
}

inline
long long CorrelationId::asInteger() const
{
    return d_impl.value.intValue;
}

inline
const blpapi_CorrelationId_t& CorrelationId::impl() const
{
    return d_impl;
}

inline
void CorrelationId::copy(const blpapi_CorrelationId_t& src)
{
    d_impl = src;

    if (POINTER_VALUE == valueType()) {
        blpapi_ManagedPtr_ManagerFunction_t& manager =
            d_impl.value.ptrValue.manager;
        if (manager) {
            manager(&d_impl.value.ptrValue, &src.value.ptrValue,
                BLPAPI_MANAGEDPTR_COPY);
        }
    }
}

template <typename TYPE>
inline
int CorrelationId::managerFunc(blpapi_ManagedPtr_t *managedPtr,
    const blpapi_ManagedPtr_t *srcPtr, int operation)
{
    if (operation == BLPAPI_MANAGEDPTR_COPY) {
        managedPtr->pointer = srcPtr->pointer;
        managedPtr->manager = srcPtr->manager;

        void *arena = managedPtr->userData;
        new (arena) TYPE(*((TYPE*)&srcPtr->userData[0]));
    }
    else if (operation == BLPAPI_MANAGEDPTR_DESTROY) {
        TYPE *managedPtr_p = (TYPE*)&managedPtr->userData[0];
        managedPtr_p->~TYPE();
    }
    return 0;
}

template <typename TYPE>
inline
void CorrelationId::assertSmartPtrFits()
{
    if (false) {
        // If you get a compiler error here, the specified smart pointer does
        // not fit in the CorrelationId and cannot be used at this time.

        char errorIfSmartPtrDoesNotFit[
            sizeof(TYPE) <= (sizeof(void*)*4) ? 1 : 0] = {' '};
    }
}

inline
bool operator==(const CorrelationId& lhs, const CorrelationId& rhs)
{
    if (lhs.valueType() != rhs.valueType()) {
        return false;
    }
    if (lhs.classId() != rhs.classId()) {
        return false;
    }

    if (lhs.valueType() == CorrelationId::POINTER_VALUE) {
        if (lhs.asPointer() != rhs.asPointer()) {
            return false;
        }
    }
    else if (lhs.asInteger() != rhs.asInteger()) {
        return false;
    }

    return true;
}

inline
bool operator!=(const CorrelationId& lhs, const CorrelationId& rhs)
{
    return !(lhs == rhs);
}

inline
bool operator<(const CorrelationId& lhs, const CorrelationId& rhs)
{
    return std::memcmp(&lhs.impl(), &rhs.impl(), sizeof(rhs.impl())) < 0;
}

inline
std::ostream& operator<<(std::ostream& os,
    const BloombergLP::blpapi::CorrelationId& correlator)
{
    const char *valueType = 0;
    switch (correlator.valueType()) {
      case CorrelationId::UNSET_VALUE:   valueType = "UNSET";   break;
      case CorrelationId::INT_VALUE:     valueType = "INT";     break;
      case CorrelationId::POINTER_VALUE: valueType = "POINTER"; break;
      case CorrelationId::AUTOGEN_VALUE: valueType = "AUTOGEN"; break;
      default: valueType = "UNKNOWN";
    }

    os << "[ valueType=" << valueType << " classId=" << correlator.classId()
       << " value=";

    if (correlator.valueType() == CorrelationId::POINTER_VALUE) {
        os << correlator.asPointer();
    }
    else os << correlator.asInteger();
    os << " ]";

    return os;
}

}  // close namespace blpapi
}  // close namespace BloombergLP

#endif // #ifdef __cplusplus
#endif // #ifndef INCLUDED_BLPAPI_CORRELATIONID
