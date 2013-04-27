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
//@DESCRIPTION: This component provides a list of subscriptions for subscribing
// and unsubscribing.

#ifndef INCLUDED_BLPAPI_TYPES
#include <blpapi_types.h>
#endif

#ifndef INCLUDED_BLPAPI_CORRELATIONID
#include <blpapi_correlationid.h>
#endif

#ifndef INCLUDED_BLPAPI_DEFS
#include <blpapi_defs.h>
#endif

#ifndef INCLUDED_STDDEF
#include <stddef.h>
#define INCLUDED_STDDEF
#endif

struct blpapi_SubscriptionList;
typedef struct blpapi_SubscriptionList blpapi_SubscriptionList_t;

#ifdef __cplusplus
extern "C" {
#endif

BLPAPI_EXPORT
blpapi_SubscriptionList_t* blpapi_SubscriptionList_create();

BLPAPI_EXPORT
void blpapi_SubscriptionList_destroy(
        blpapi_SubscriptionList_t *list);

BLPAPI_EXPORT
int blpapi_SubscriptionList_add(
        blpapi_SubscriptionList_t* list,
        const char* topic,
        const blpapi_CorrelationId_t *correlationId,
        const char** fields,
        const char** options,
        size_t numfields,
        size_t numOptions);

BLPAPI_EXPORT
int blpapi_SubscriptionList_clear(
        blpapi_SubscriptionList_t* list);

BLPAPI_EXPORT
int blpapi_SubscriptionList_append(
        blpapi_SubscriptionList_t* dest,
        const blpapi_SubscriptionList_t* src);

BLPAPI_EXPORT
int blpapi_SubscriptionList_size(
        const blpapi_SubscriptionList_t *list);

BLPAPI_EXPORT
int blpapi_SubscriptionList_correlationIdAt(
        const blpapi_SubscriptionList_t* list,
        blpapi_CorrelationId_t* result,
        size_t index);

BLPAPI_EXPORT
int blpapi_SubscriptionList_topicStringAt(
        blpapi_SubscriptionList_t* list,
        const char** result,
        size_t index);


#ifdef __cplusplus
}

#ifndef INCLUDED_BLPAPI_EXCEPTION
#include <blpapi_exception.h>
#endif

#ifndef INCLUDED_CSTRING
#include <cstring>
#define INCLUDED_CSTRING
#endif

#ifndef INCLUDED_VECTOR
#include <vector>
#define INCLUDED_VECTOR
#endif

#ifndef INCLUDED_STRING
#include <string>
#define INCLUDED_STRING
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
    // A SubscriptionList is used when calling Session::subscribe(),
    // Session::resubscribe() and Session::unsubscribe(). The entries
    // can be constructed in a variety of ways.

    blpapi_SubscriptionList_t *d_handle_p;

  public:
    SubscriptionList();
        // Create an empty SubscriptionList.

    SubscriptionList(const SubscriptionList& original);
        // Copy constructor.

    ~SubscriptionList();
        // Destroy this SubscriptionList.

    // MANIPULATORS

    int add(const char* topic);
        // Add the specified 'topic' to this SubscriptionList,
        // associating an internally generated CorrelationId with
        // it. The topic string may include options.

    int add(const char* topic,
            const CorrelationId& correlationID);
        // Add the specified 'topic' to this SubscriptionList,
        // associating the specified 'correlationID' with it. The
        // topic string may include options.

    int add(const char* topic,
            const char* fields,
            const char* options,
            const CorrelationId& correlationId);
        // Add the specified 'topic', with the specified 'fields' and
        // the specified 'options' to this SubscriptionList,
        // associating the specified 'correlationId' with it. The
        // 'fields' must be specified as a comma separated list. The
        // 'options' must be specified as a ampersand separated list.

    int add(const char* topic,
            const std::vector<std::string>& fields,
            const std::vector<std::string>& options,
            const CorrelationId& correlationId);
        // Add the specified 'topic', with the specified 'fields' and
        // the specified 'options' to this SubscriptionList,
        // associating the specified 'correlationId' with it.

    int add(const CorrelationId& correlationID);
        // Add the specified 'correlationID' to this
        // SubscriptionList. A SubscriptionList entry which only has a
        // CorrelationId can only be used in a Session::unsubscribe()
        // call.

    int append(const SubscriptionList& other);
        // Append a copy of the specified 'other' SubscriptionList to
        // this SubscriptionList.

    void clear();
        // Remove all entries from this SubscriptionList.

    SubscriptionList& operator=(const SubscriptionList& rhs);
        // Replace the contents of this SubscriptionList with contents
        // of the specified 'rhs' SubscriptionList and return a
        // modifiable reference to this SubscriptionList.

    // ACCESSORS

    size_t size() const;
        // Return the number of subscriptions in this SubscriptionList.

    CorrelationId correlationIdAt(size_t index) const;
        // Return the CorrelationId of the specified 'index'th entry
        // in this SubscriptionList. An exception is thrown if
        // 'index'>=size().

    const char* topicStringAt(size_t index) const;
        // Return a pointer to a null terminated string which contains
        // the full topic string (including any field and option
        // portions) of the 'index'th entry in this
        // SubscriptionList. The pointer remains valid until this
        // SubscriptionList is destroyed or clear() is called. An
        // exception is thrown if 'index'>=size().

    const blpapi_SubscriptionList_t* impl() const;
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
    if (!d_handle_p) {
        // TODO
    }
}

inline
SubscriptionList::SubscriptionList(
        const SubscriptionList& original)
: d_handle_p(blpapi_SubscriptionList_create())
{
    if (!d_handle_p) {

    }
    blpapi_SubscriptionList_append(d_handle_p, original.d_handle_p);
}


inline
SubscriptionList::~SubscriptionList()
{
    blpapi_SubscriptionList_destroy(d_handle_p);
}


inline
int SubscriptionList::add(const char* topic)
{
    blpapi_CorrelationId_t correlationId;
    std::memset(&correlationId, 0, sizeof(correlationId));
    return blpapi_SubscriptionList_add(d_handle_p,
                                       topic,
                                       &correlationId,
                                       0,0,0,0);
}

inline
int SubscriptionList::add(const char* topic,
                                 const CorrelationId& correlationId)
{
    return blpapi_SubscriptionList_add(d_handle_p,
                                       topic,
                                       &correlationId.impl(),
                                       0,0,0,0
                                       );
}

inline
int SubscriptionList::add(
        const char* topic,
        const char* fields,
        const char* options,
        const CorrelationId& correlationId)
{
    return blpapi_SubscriptionList_add(d_handle_p,
                                       topic,
                                       &correlationId.impl(),
                                       &fields,
                                       &options,
                                       fields ? 1 : 0,
                                       options ? 1 : 0 );

}

inline
int SubscriptionList::add(
        const char* topic,
        const std::vector<std::string>& fields,
        const std::vector<std::string>& options,
        const CorrelationId& correlationId)
{
    std::vector<const char*> tmpVector;
    const char* arena[256];
    const char** tmpArray=arena;
    size_t sizeNeeded = fields.size() + options.size();

    if (sizeNeeded > sizeof(arena)/sizeof(arena[0])) {
        tmpVector.resize(sizeNeeded);
        tmpArray = &tmpVector[0];
    }

    const char** p = tmpArray;
    for (std::vector<std::string>::const_iterator itr = fields.begin(),
            end = fields.end(); itr != end; ++itr, ++p) {
        *p = itr->c_str();
    }

    for (std::vector<std::string>::const_iterator itr = options.begin(),
            end = options.end(); itr != end; ++itr, ++p) {
        *p = itr->c_str();
    }

    return blpapi_SubscriptionList_add(d_handle_p,
                                       topic,
                                       &correlationId.impl(),
                                       tmpArray,
                                       tmpArray + fields.size(),
                                       fields.size(),
                                       options.size());


}

inline
int SubscriptionList::add(const CorrelationId& correlationId)
{
    return blpapi_SubscriptionList_add(d_handle_p,
                                       "",
                                       &correlationId.impl(),
                                       0, 0, 0, 0);
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
SubscriptionList& SubscriptionList::operator=(
        const SubscriptionList& rhs)
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
            blpapi_SubscriptionList_correlationIdAt(d_handle_p, &correlationId,
            index));

    return CorrelationId(correlationId);
}

inline
const char* SubscriptionList::topicStringAt(size_t index) const
{
    const char* result;

    ExceptionUtil::throwOnError(
            blpapi_SubscriptionList_topicStringAt(
                d_handle_p,
                &result,
                index));

    return result;
}

inline
const blpapi_SubscriptionList_t* SubscriptionList::impl() const
{
    return d_handle_p;
}

}  // close namespace blpapi
}  // close namespace BloombergLP

#endif // ifdef __cplusplus


#endif // #ifndef INCLUDED_BLPAPI_SUBSCRIPTIONLIST
