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
// blpapi_subscriptionlist.h                                          -*-C++-*-
#ifndef INCLUDED_BLPAPI_SUBSCRIPTIONLIST
#define INCLUDED_BLPAPI_SUBSCRIPTIONLIST

//@PURPOSE: Provide a list of subscriptions.
//
//@CLASSES:
// blpapi::SubscriptionList: Represents a list of subscriptions.
//
//@DESCRIPTION: This component provides a structure to hold the data used (and
// returned) by the 'Session::subscribe', 'Session::resubscribe', and
// 'Session::unsubscribe' methods.  This structure comprises a list in which
// each list entry contains two primary fields: a 'CorrelationId' associated
// with the subscription, and a string, called a *subscription* *string*,
// describing the data to be delivered as a part of the subscription.
//
// STRUCTURE OF SUBSCRIPTION STRING
// ---------------------------------
// The simplest form of a subscription string is a *fully* *qualified*
// subscription string, which has the following structure:
//
// "//blp/mktdata/ticker/IBM US Equity?fields=BID,ASK&interval=2"
//  \-----------/\------/\-----------/\------------------------/
//        |          |         |                  |
//     Service    Prefix   Instrument           Suffix
//
// Such a fully-qualified string is composed of:
//: Service Identifier: a string matching the expression
//:   '^//[-_.a-zA-Z0-9]+/[-_.a-zA-Z0-9]+$', e.g. //blp/mktdata.  See
//:   'blpapi_abstractsession' for further details.
//:
//: Prefix: a string matching the expression '/([-_.a-zA-Z0-9]+/)?', often used
//:   as a symbology identifier.  Common examples include '/ticker/' and
//:   '/cusip/'.  Not all services make use of prefices.  Note than an "empty"
//:   topic prefix consists of the string "/", so the topic prefix always
//:   separates the service string from the instrument string.
//:
//: Instrument: a non-empty string that does not contain the character '?'
//:   (i.e. a string matching '[^?]+') e.g. "IBM US Equity", or "SPX Index".
//:   The service, prefix, and instrument together uniquely identify a source
//:   for subscription data.
//:
//: Suffix: a suffix contains a question mark followed by a list of options
//:   which can affect the content delivery.  The format of these options is
//:   service specific, but they generally follow URI query string conventions:
//:   a sequence of "key=value" pairs separated by "&" characters.  Further,
//:   many common services follow the convention that the value given for the
//:   'fields' key is formatted as a comma-separated list of field names.
//:   BLPAPI provides several convenience functions to assist in formatting
//:   subscription strings for services that make use of these conventions;
//:   see the 'SubscriptionList::add' methods for details.
//
// Subscription strings need not be fully qualified: BLPAPI allows the service
// and prefix to be omitted from subscription strings, and automatically
// qualifies these strings using information stored in a 'Session' object.
//
// QUALIFYING SUBSCRIPTION STRINGS
// -------------------------------
// The subscription strings passed to 'Session::subscribe' and
// 'Session::resubscribe' are automatically qualified if the service identifier
// is missing (i.e. if the subscription string does not start with "//"). The
// subscription parameters (i.e. the optional part after instrument identifier)
// are never modified.
// The rules for qualifying the subscription string are:
//
//: o If the subscription string begins with "//" then it is assumed to be a
//:   a fully qualified subscription string including service identifier,
//:   prefix, and instrument.  In this case the string will not be modified and
//:   session options defaults have no affect on the subscription.
//:
//: o If the subscription string begins with a '/' and the second character is
//:   not '/', then the string is assumed to begin with the topic prefix, but
//:   no service identifier. In this case the string is qualified by prepending
//:   the 'SessionOptions::defaultSubscriptionService()' to the specified
//:   string.
//:
//: o If the subscription string does not begin with a '/' it is assumed to
//:   begin with an instrument identifier.  In this case the string is
//:   qualified by prepending the
//:   'SessionOptions::defaultSubscriptionService()' followed by
//:   'SessionOptions::defaultTopicPrefix()' to the specified string.
//:   If the 'defaultTopicPrefix' is empty or null, then the prefix used is
//:   '/'.  Otherwise (in the case of a nontrivial prefix) if the separator '/'
//:   is not specified at the beginning or the end of the 'defaultTopicPrefix',
//:   then it will be added.

#ifndef INCLUDED_BLPAPI_CALL
#include <blpapi_call.h>
#endif

#ifndef INCLUDED_BLPAPI_CORRELATIONID
#include <blpapi_correlationid.h>
#endif

#ifndef INCLUDED_BLPAPI_DEFS
#include <blpapi_defs.h>
#endif

#ifndef INCLUDED_BLPAPI_EXCEPTION
#include <blpapi_exception.h>
#endif

#ifndef INCLUDED_BLPAPI_TYPES
#include <blpapi_types.h>
#endif

#include <stddef.h>

struct blpapi_SubscriptionList;
typedef struct blpapi_SubscriptionList blpapi_SubscriptionList_t;

#ifdef __cplusplus
extern "C" {
#endif

BLPAPI_EXPORT
blpapi_SubscriptionList_t *blpapi_SubscriptionList_create();

BLPAPI_EXPORT
void blpapi_SubscriptionList_destroy(blpapi_SubscriptionList_t *list);

BLPAPI_EXPORT
int blpapi_SubscriptionList_add(
                             blpapi_SubscriptionList_t     *list,
                             const char                    *subscriptionString,
                             const blpapi_CorrelationId_t  *correlationId,
                             const char                   **fields,
                             const char                   **options,
                             size_t                         numfields,
                             size_t                         numOptions);

BLPAPI_EXPORT
int blpapi_SubscriptionList_addResolved(
                              blpapi_SubscriptionList_t    *list,
                              const char                   *subscriptionString,
                              const blpapi_CorrelationId_t *correlationId);

BLPAPI_EXPORT
int blpapi_SubscriptionList_clear(blpapi_SubscriptionList_t *list);

BLPAPI_EXPORT
int blpapi_SubscriptionList_append(blpapi_SubscriptionList_t       *dest,
                                   const blpapi_SubscriptionList_t *src);

BLPAPI_EXPORT
int blpapi_SubscriptionList_size(const blpapi_SubscriptionList_t *list);

BLPAPI_EXPORT
int blpapi_SubscriptionList_correlationIdAt(
                                       const blpapi_SubscriptionList_t *list,
                                       blpapi_CorrelationId_t          *result,
                                       size_t                           index);

BLPAPI_EXPORT
int blpapi_SubscriptionList_topicStringAt(blpapi_SubscriptionList_t  *list,
                                          const char                **result,
                                          size_t                      index);

BLPAPI_EXPORT
int blpapi_SubscriptionList_isResolvedAt(blpapi_SubscriptionList_t *list,
                                         int                       *result,
                                         size_t                     index);


#ifdef __cplusplus
}

#ifndef INCLUDED_CSTRING
#include <cstring>
#define INCLUDED_CSTRING
#endif

#ifndef INCLUDED_STRING
#include <string>
#define INCLUDED_STRING
#endif

#ifndef INCLUDED_VECTOR
#include <vector>
#define INCLUDED_VECTOR
#endif

namespace BloombergLP {
namespace blpapi {
                         // ======================
                         // class SubscriptionList
                         // ======================

class SubscriptionList {
    // Contains a list of subscriptions used when subscribing and
    // unsubscribing.
    //
    // A 'SubscriptionList' is used when calling 'Session::subscribe()',
    // 'Session::resubscribe()' and 'Session::unsubscribe()'.  The entries can
    // be constructed in a variety of ways.
    // The two important elements when creating a subscription are
    //: Subscription String: A subscription string represents a topic whose
    //:  updates user is interested in.  A subscription string follows a
    //:  structure as specified below.
    //: CorrelationId: the unique identifier to tag all data associated with
    //:  this subscription.
    //
    // The following table describes how various operations use the above
    // elements:
    // --------------|--------------------------------------------------------|
    //  OPERATION    |  SUBSCRIPTION STRING  |       CORRELATION ID           |
    // --------------|-----------------------+--------------------------------|
    //  'subscribe'  |Used to specify the    |Identifier for the subscription.|
    //               |topic to subscribe to. |If uninitialized correlationid  |
    //               |                       |was specified an internally     |
    //               |                       |generated correlationId will be |
    //               |                       |set for the subscription.       |
    // --------------+-----------------------+--------------------------------|
    // 'resubscribe' |Used to specify the new|Identifier of the subscription  |
    //               |topic to which the     |which needs to be modified.     |
    //               |subscription should be |                                |
    //               |modified to.           |                                |
    // --------------+-----------------------+--------------------------------|
    // 'unsubscribe' |        NOT USED       |Identifier of the subscription  |
    //               |                       |which needs to be canceled.     |
    // -----------------------------------------------------------------------|

    blpapi_SubscriptionList_t *d_handle_p;

  public:
    SubscriptionList();
        // Create an empty 'SubscriptionList'.
    SubscriptionList(const SubscriptionList& original);
        // Create a 'SubscriptionList' object initialized to the list of
        // subscriptions from the specified 'original'.

    ~SubscriptionList();
        // Destroy this object.

    // MANIPULATORS

    int add(const char *subscriptionString);
        // Append the specified 'subscriptionString' to this 'SubscriptionList'
        // object, associating an internally generated 'CorrelationId' with it.
        // The subscription string may include options.  Note that the
        // subscription string provided to this function may be resolved by
        // functions operating on this 'SubscriptionList' object; use
        // 'SubscriptionList::addResolved', below, to request that such
        // functions bypass resolution.

    int add(const char           *subscriptionString,
            const CorrelationId&  correlationId);
        // Append the specified 'subscriptionString' to this 'SubscriptionList'
        // object, associating the specified 'correlationId' with it.  The
        // subscription string may include options.  Note that the subscription
        // string provided to this function  may be resolved by functions
        // operating on this 'SubscriptionList' object; use
        // 'SubscriptionList::addResolved', below, to request that such
        // functions bypass resolution.

    int add(const char                      *subscriptionWithoutOptions,
            const std::vector<std::string>&  fields,
            const std::vector<std::string>&  options,
            const CorrelationId&             correlationId);
        // Append the specified 'subscriptionString', with the specified
        // 'fields' and the specified 'options', to this 'SubscriptionList'
        // object, associating the specified 'correlationId' with it.

    int add(const char           *subscriptionWithoutOptions,
            const char           *fields,
            const char           *options,
            const CorrelationId&  correlationId);
        // Append the specified 'subscriptionString' with the specified
        // 'fields', which must be formatted as a comma-separated list, and the
        // specified 'options', which must be formatted as an
        // ampersand-separated list, to this 'SubscriptionList' object, and
        // associate the specified 'correlationId' with it.

    int add(const CorrelationId& correlationId);
        // Append the specified 'correlationId' to this 'SubscriptionList'
        // object.  A 'SubscriptionList' entry containing only a
        // 'CorrelationId' can be used only in a 'Session::unsubscribe' call.

    int addResolved(const char *subscriptionString);
        // Add the specified 'subscriptionString' to this 'SubscriptionList',
        // associating an internally generated 'CorrelationId' with it.  The
        // subscription string may include options.  The behavior of this
        // function, and of functions operating on this 'SubscriptionList'
        // object, is undefined unless 'subscriptionString' is a
        // fully-resolved subscription string; clients that cannot provide
        // fully-resolved subscription strings should use
        // 'SubscriptionList::add' instead.  Note that it is at the discretion
        // of each function operating on a 'SubscriptionList' whether to
        // perform resolution on this subscription.

    int addResolved(const char           *subscriptionString,
                    const CorrelationId&  correlationId);
        // Add the specified 'subscriptionString' to this 'SubscriptionList'
        // object, associating the specified 'correlationId' with it.  The
        // subscription string may include options.  The behavior of this
        // function, and of functions operating on this 'SubscriptionList'
        // object, is undefined unless 'subscriptionString' is a
        // fully-resolved subscription string; clients that cannot provide
        // fully-resolved subscription strings should use
        // 'SubscriptionList::add' instead.  Note that it is at the discretion
        // of each function operating on a 'SubscriptionList' whether to
        // perform resolution on this subscription.

    int append(const SubscriptionList& other);
        // Extend this object by appending a copy of each entry in the
        // specified 'other'.  Note that this function adds 'other.size()' new
        // entries to this object.  Note also that this function is alias-safe;
        // i.e. 'x.append(x)' has well-defined behavior.

    void clear();
        // Remove all entries from this object.

    SubscriptionList& operator=(const SubscriptionList& rhs);
        // Replace the contents of this 'SubscriptionList' object with the
        // contents of the specified 'rhs' and return a modifiable reference to
        // this 'SubscriptionList' object.

    // ACCESSORS

    size_t size() const;
        // Return the number of entries in this object.

    CorrelationId correlationIdAt(size_t index) const;
        // Return the 'CorrelationId' of the specified 'index'th entry in this
        // 'SubscriptionList' object.  An exception is thrown if
        // 'index >= size()'.

    const char *topicStringAt(size_t index) const;
        // Return a pointer to a null-terminated string which contains the full
        // topic string (including any field and option portions) of the
        // 'index'th entry in this 'SubscriptionList' object.  The pointer
        // remains valid until this 'SubscriptionList' object is destroyed or
        // 'clear()' is called.  An exception is thrown if 'index >= size()'.

    bool isResolvedTopicAt(size_t index) const;
        // Return 'true' if the 'index'th entry in this 'SubscriptionList'
        // object was created using 'SubscriptionList::addResolved' and 'false'
        // if it was created using 'SubscriptionList::add'.  An exception is
        // thrown if 'index >= size()'.

    const blpapi_SubscriptionList_t *impl() const;
};

// ============================================================================
//                      INLINE FUNCTION DEFINITIONS
// ============================================================================

                            // ----------------------
                            // class SubscriptionList
                            // ----------------------
inline
SubscriptionList::SubscriptionList()
: d_handle_p(blpapi_SubscriptionList_create())
{
}

inline
SubscriptionList::SubscriptionList(const SubscriptionList& original)
: d_handle_p(blpapi_SubscriptionList_create())
{
    blpapi_SubscriptionList_append(d_handle_p, original.d_handle_p);
}

inline
SubscriptionList::~SubscriptionList()
{
    blpapi_SubscriptionList_destroy(d_handle_p);
}

inline
int SubscriptionList::add(const char *subscriptionString)
{
    blpapi_CorrelationId_t correlationId;
    std::memset(&correlationId, 0, sizeof(correlationId));
    return blpapi_SubscriptionList_add(d_handle_p,
                                       subscriptionString,
                                       &correlationId,
                                       0,
                                       0,
                                       0,
                                       0);
}

inline
int SubscriptionList::add(const char           *subscriptionString,
                          const CorrelationId&  correlationId)
{
    return blpapi_SubscriptionList_add(d_handle_p,
                                       subscriptionString,
                                       &correlationId.impl(),
                                       0,
                                       0,
                                       0,
                                       0);
}

inline
int SubscriptionList::add(const char                      *subscriptionString,
                          const std::vector<std::string>&  fields,
                          const std::vector<std::string>&  options,
                          const CorrelationId&             correlationId)
{
    std::vector<const char *> tmpVector;
    const char *arena[256];
    const char **tmpArray = arena;
    size_t sizeNeeded = fields.size() + options.size();

    if (sizeNeeded > sizeof(arena)/sizeof(arena[0])) {
        tmpVector.resize(sizeNeeded);
        tmpArray = &tmpVector[0];
    }

    const char **p = tmpArray;
    for (std::vector<std::string>::const_iterator itr = fields.begin(),
            end = fields.end(); itr != end; ++itr, ++p) {
        *p = itr->c_str();
    }

    for (std::vector<std::string>::const_iterator itr = options.begin(),
            end = options.end(); itr != end; ++itr, ++p) {
        *p = itr->c_str();
    }

    return blpapi_SubscriptionList_add(d_handle_p,
                                       subscriptionString,
                                       &correlationId.impl(),
                                       tmpArray,
                                       tmpArray + fields.size(),
                                       fields.size(),
                                       options.size());
}

inline
int SubscriptionList::add(const char           *subscriptionString,
                          const char           *fields,
                          const char           *options,
                          const CorrelationId&  correlationId)
{
    return blpapi_SubscriptionList_add(d_handle_p,
                                       subscriptionString,
                                       &correlationId.impl(),
                                       &fields,
                                       &options,
                                       fields ? 1 : 0,
                                       options ? 1 : 0 );

}

inline
int SubscriptionList::add(const CorrelationId& correlationId)
{
    return blpapi_SubscriptionList_add(d_handle_p,
                                       "",
                                       &correlationId.impl(),
                                       0,
                                       0,
                                       0,
                                       0);
}

inline
int SubscriptionList::addResolved(const char *subscriptionString)
{
    blpapi_CorrelationId_t correlationId;
    std::memset(&correlationId, 0, sizeof(correlationId));
    return BLPAPI_CALL_SUBSCRIPTIONLIST_ADDRESOLVED(d_handle_p,
                                                    subscriptionString,
                                                    &correlationId);
}

inline
int SubscriptionList::addResolved(const char *subscriptionString,
                                  const CorrelationId& correlationId)
{
    return BLPAPI_CALL_SUBSCRIPTIONLIST_ADDRESOLVED(d_handle_p,
                                                    subscriptionString,
                                                    &correlationId.impl());
}

inline
int SubscriptionList::append(const SubscriptionList& other)
{
    return blpapi_SubscriptionList_append(d_handle_p, other.d_handle_p);
}

inline
void SubscriptionList::clear()
{
    blpapi_SubscriptionList_clear(d_handle_p);
}

inline
SubscriptionList& SubscriptionList::operator=(const SubscriptionList& rhs)
{
    blpapi_SubscriptionList_clear(d_handle_p);
    blpapi_SubscriptionList_append(d_handle_p, rhs.d_handle_p);
    return *this;
}

inline
size_t SubscriptionList::size() const
{
    return blpapi_SubscriptionList_size(d_handle_p);
}

inline
CorrelationId SubscriptionList::correlationIdAt(size_t index) const
{
    blpapi_CorrelationId_t correlationId;

    ExceptionUtil::throwOnError(
            blpapi_SubscriptionList_correlationIdAt(d_handle_p,
                                                    &correlationId,
                                                    index));

    return CorrelationId(correlationId);
}

inline
const char *SubscriptionList::topicStringAt(size_t index) const
{
    const char *result;

    ExceptionUtil::throwOnError(
            blpapi_SubscriptionList_topicStringAt(d_handle_p,
                                                  &result,
                                                  index));

    return result;
}

inline
bool SubscriptionList::isResolvedTopicAt(size_t index) const
{
    int result;

    ExceptionUtil::throwOnError(
            BLPAPI_CALL_SUBSCRIPTIONLIST_ISRESOLVEDAT(d_handle_p,
                                                      &result,
                                                      index));

    return result ? true : false;
}

inline
const blpapi_SubscriptionList_t *SubscriptionList::impl() const
{
    return d_handle_p;
}

}  // close package namespace
}  // close enterprise namespace

#endif // ifdef __cplusplus


#endif // #ifndef INCLUDED_BLPAPI_SUBSCRIPTIONLIST
