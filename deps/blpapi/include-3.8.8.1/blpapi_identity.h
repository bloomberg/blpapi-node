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
// blpapi_identity.h                                                  -*-C++-*-
#ifndef INCLUDED_BLPAPI_IDENTITY
#define INCLUDED_BLPAPI_IDENTITY

//@PURPOSE: Provide access to the entitlements for a user.
//
//@CLASSES:
// blpapi::Identity: Represents an identification of a user
//
//@DESCRIPTION: This component provides an identification of a user and
// implements the access to the entitlements.

#ifndef INCLUDED_BLPAPI_DEFS
#include <blpapi_defs.h>
#endif

#ifndef INCLUDED_BLPAPI_EXCEPTION
#include <blpapi_exception.h>
#endif

#ifndef INCLUDED_BLPAPI_SERVICE
#include <blpapi_service.h>
#endif

#ifndef INCLUDED_BLPAPI_TYPES
#include <blpapi_types.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

BLPAPI_EXPORT
void blpapi_Identity_release(blpapi_Identity_t* handle);

BLPAPI_EXPORT
int blpapi_Identity_addRef(blpapi_Identity_t* handle);

BLPAPI_EXPORT
int blpapi_Identity_hasEntitlements(
        const blpapi_Identity_t* handle,
        const blpapi_Service_t *service,
        const blpapi_Element_t *eidElement,
        const int *entitlementIds,
        size_t numEntitlements,
        int *failedEntitlements,
        int *failedEntitlementsCount);

BLPAPI_EXPORT
int blpapi_Identity_isAuthorized(
        const blpapi_Identity_t* handle,
        const blpapi_Service_t *service);

BLPAPI_EXPORT
int blpapi_Identity_getSeatType(
        const blpapi_Identity_t* handle,
        int *seatType);

#ifdef __cplusplus
}

namespace BloombergLP {
namespace blpapi {

class Element;
                         // ==============
                         // class Identity
                         // ==============

class Identity {
    // Provides access to the entitlements for a specific user.
    //
    // An unauthorized Identity is created using
    // Session::createIdentity(). Once a Identity has been created
    // it can be authorized using
    // Session::sendAuthorizationRequest(). The authorized Identity
    // can then be queried or used in Session::subscribe() or
    // Session::sendRequest() calls.
    //
    // Once authorized a Identity has access to the entitlements of
    // the user which it was validated for.
    //
    // The Identity is a reference counted handle, copying it or
    // assigning it does not duplicate the underlying entitlement
    // data. Once the last Identity referring to the underlying
    // entitlement data is destroyed that entitlement data is
    // discarded and can only be re-established using
    // Session::sendAuthorizationRequest() again.

    blpapi_Identity_t *d_handle_p;

    void addRef();
    void release();

  public:
    enum SeatType {
        INVALID_SEAT = BLPAPI_SEATTYPE_INVALID_SEAT,
        BPS = BLPAPI_SEATTYPE_BPS, // Bloomberg Professional Service
        NONBPS = BLPAPI_SEATTYPE_NONBPS
    };

  public:
    Identity(blpapi_Identity_t *handle);
        // Assume ownership of the raw handle

    Identity();
        // Create an uninitialized Identity. The only valid operations
        // on an uninitialized Identity are assignment, isValid() and
        // destruction.

    Identity(const Identity& original);
        // Copy constructor

    ~Identity();
        // Destructor. Destroying the last Identity for a specific
        // user cancels any authorizations associated with it.

    // MANIPULATORS

    Identity& operator=(const Identity&);
        // Assignment operator.

    // ACCESSORS
    bool hasEntitlements(const Service& service,
                         const int *entitlementIds,
                         size_t numEntitlements) const;
        // Return true if this 'Identity' is authorized for the specified
        // 'service' and the first 'numEntitlements' elements of the specified
        // 'entitlementIds' array; otherwise return false. The behavior is
        // undefined unless 'entitlementIds' is an array containing at least
        // 'numEntitlements' elements.

    bool hasEntitlements(const Service& service,
                         const int *entitlementIds,
                         size_t numEntitlements,
                         int *failedEntitlements,
                         int *failedEntitlementsCount) const;
        // Return true if this 'Identity' is authorized for the specified
        // 'service' and the first 'numEntitlements' elements of the specified
        // 'entitlementIds' array; otherwise fill the specified
        // 'failedEntitlements' array with the subset of 'entitlementIds' this
        // 'Identity' is not authorized for, load the number of such
        // entitlements into the specified 'failedEntitlementsCount', and
        // return false. The behavior is undefined unless 'entitlementIds' and
        // 'failedEntitlements' are arrays containing at least
        // 'numEntitlements' elements, and 'failedEntitlementsCount' is
        // non-null.

    bool hasEntitlements(const Service& service,
                         const Element& entitlementIds,
                         int *failedEntitlements,
                         int *failedEntitlementsCount) const;
        // Return true if this 'Identity' is authorized for the specified
        // 'service' and for each of the entitlement IDs contained in the
        // specified 'entitlementIds', which must be an 'Element' which is an
        // array of integers; otherwise, fill the specified
        // 'failedEntitlements' array with the subset of entitlement IDs this
        // 'Identity' is not authorized for, load the number of such
        // entitlements into the specified 'failedEntitlementsCount', and
        // return false. The behavior is undefined unless 'failedEntitlements'
        // is an array containing at least 'entitlementIds.numValues()'
        // elements and 'failedEntitlementsCount' is non-null.

    bool isValid() const;
        // Return true if this 'Identity' is valid; otherwise return false.
        // Note that a valid 'Identity' has not necessarily been authorized.
        // This function is deprecated.

    bool isAuthorized(const Service& service) const;
        // Return true if this 'Identity' is authorized for the specified
        // 'service'; otherwise return false.

    SeatType getSeatType() const;
        // Return the seat type of this 'Identity'.

    blpapi_Identity_t* handle() const;
};

//=============================================================================
//                           INLINE FUNCTION DEFINITIONS
//=============================================================================

                         // --------------
                         // class Identity
                         // --------------

inline
Identity::Identity()
    : d_handle_p(0)
{
}

inline
Identity::Identity(blpapi_Identity_t *newHandle)
    : d_handle_p(newHandle)
{
}

inline
Identity::Identity(const Identity& original)
    : d_handle_p(original.d_handle_p)
{
    addRef();
}

inline
Identity::~Identity()
{
    release();
}

inline
Identity& Identity::operator=(const Identity& rhs)
{
    if (&rhs != this) {
        release();
        d_handle_p = rhs.d_handle_p;
        addRef();
    }
    return *this;
}

inline
void Identity::addRef()
{
    if (d_handle_p) {
        blpapi_Identity_addRef(d_handle_p);
    }
}

inline
void Identity::release()
{
    if (d_handle_p) {
        blpapi_Identity_release(d_handle_p);
    }
}

inline
bool Identity::hasEntitlements(
        const Service& service,
        const int *entitlementIds,
        size_t numEntitlements) const
{
    return blpapi_Identity_hasEntitlements(
            d_handle_p,
            service.handle(),
            0,
            entitlementIds,
            numEntitlements,
            0,
            0) ? true : false;
}

inline
bool Identity::hasEntitlements(
        const Service& service,
        const int *entitlementIds,
        size_t numEntitlements,
        int *failedEntitlements,
        int *failedEntitlementsCount) const
{
    return blpapi_Identity_hasEntitlements(
            d_handle_p,
            service.handle(),
            0,
            entitlementIds,
            numEntitlements,
            failedEntitlements,
            failedEntitlementsCount) ? true : false;
}

inline
bool Identity::hasEntitlements(
        const Service& service,
        const Element& entitlementIds,
        int *failedEntitlements,
        int *failedEntitlementsCount) const
{
    return blpapi_Identity_hasEntitlements(
            d_handle_p,
            service.handle(),
            entitlementIds.handle(),
            0,
            0,
            failedEntitlements,
            failedEntitlementsCount) ? true : false;
}

inline
bool Identity::isValid() const
{
    return (d_handle_p != 0);
}

inline
bool Identity::isAuthorized(const Service& service) const
{
    return blpapi_Identity_isAuthorized(d_handle_p,
                                        service.handle()) ? true : false;
}

inline
Identity::SeatType Identity::getSeatType() const
{
    int seatType = BLPAPI_SEATTYPE_INVALID_SEAT;
    ExceptionUtil::throwOnError(
            blpapi_Identity_getSeatType(d_handle_p, &seatType));
    return static_cast<SeatType>(seatType);
}

inline
blpapi_Identity_t* Identity::handle() const
{
    return d_handle_p;
}

}  // close namespace blpapi
}  // close namespace BloombergLP

#endif // #ifdef __cplusplus
#endif // #ifndef INCLUDED_BLPAPI_IDENTITY
