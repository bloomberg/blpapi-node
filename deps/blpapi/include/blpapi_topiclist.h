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
// blpapi_topiclist.h                                                 -*-C++-*-
#ifndef INCLUDED_BLPAPI_TOPICLIST
#define INCLUDED_BLPAPI_TOPICLIST

//@PURPOSE: Provide a representation of a list of topics.
//
//@CLASSES:
// blpapi::TopicList: Represents a list of topics
//
//@DESCRIPTION: This component implements a list of topics which require
// topic creation.

#ifndef INCLUDED_BLPAPI_TYPES
#include <blpapi_types.h>
#endif

#ifndef INCLUDED_BLPAPI_CORRELATIONID
#include <blpapi_correlationid.h>
#endif

#ifndef INCLUDED_BLPAPI_DEFS
#include <blpapi_defs.h>
#endif

#ifndef INCLUDED_BLPAPI_ELEMENT
#include <blpapi_element.h>
#endif

#ifndef INCLUDED_BLPAPI_NAME
#include <blpapi_name.h>
#endif

#ifndef INCLUDED_BLPAPI_MESSAGE
#include <blpapi_message.h>
#endif

#ifndef INCLUDED_BLPAPI_RESOLUTIONLIST
#include <blpapi_resolutionlist.h>
#endif

struct blpapi_TopicList;
typedef struct blpapi_TopicList blpapi_TopicList_t;

#ifdef __cplusplus
extern "C" {
#endif

BLPAPI_EXPORT
blpapi_TopicList_t* blpapi_TopicList_create(
    blpapi_TopicList_t* from);

BLPAPI_EXPORT
void blpapi_TopicList_destroy(
    blpapi_TopicList_t *list);

BLPAPI_EXPORT
int blpapi_TopicList_add(
    blpapi_TopicList_t* list,
    const char* topic,
    const blpapi_CorrelationId_t *correlationId);

BLPAPI_EXPORT
int blpapi_TopicList_addFromMessage(
    blpapi_TopicList_t* list,
    const blpapi_Message_t* topic,
    const blpapi_CorrelationId_t *correlationId);

BLPAPI_EXPORT
int blpapi_TopicList_correlationIdAt(
    const blpapi_TopicList_t* list,
    blpapi_CorrelationId_t* result,
    size_t index);

BLPAPI_EXPORT
int blpapi_TopicList_topicString(
    const blpapi_TopicList_t* list,
    char const** topic,
    const blpapi_CorrelationId_t* id);

BLPAPI_EXPORT
int blpapi_TopicList_topicStringAt(
    const blpapi_TopicList_t* list,
    char const** topic,
    size_t index);

BLPAPI_EXPORT
int blpapi_TopicList_status(
    const blpapi_TopicList_t* list,
    int* status,
    const blpapi_CorrelationId_t* id);

BLPAPI_EXPORT
int blpapi_TopicList_statusAt(
    const blpapi_TopicList_t* list,
    int* status,
    size_t index);

BLPAPI_EXPORT
int blpapi_TopicList_message(
    const blpapi_TopicList_t* list,
    blpapi_Message_t** element,
    const blpapi_CorrelationId_t* id);

BLPAPI_EXPORT
int blpapi_TopicList_messageAt(
    const blpapi_TopicList_t* list,
    blpapi_Message_t** element,
    size_t index);

BLPAPI_EXPORT
int blpapi_TopicList_size(
    const blpapi_TopicList_t* list);

#ifdef __cplusplus
}

#ifndef INCLUDED_BLPAPI_EXCEPTION
#include <blpapi_exception.h>
#endif

namespace BloombergLP {
namespace blpapi {

class ResolutionList;
                         // ===============
                         // class TopicList
                         // ===============

class TopicList {
    // Contains a list of topics which require creation.
    //
    // Created from topic strings or from TOPIC_SUBSCRIBED or
    // RESOLUTION_SUCCESS messages.
    // This is passed to a createTopics() call or createTopicsAsync()
    // call on a ProviderSession. It is updated and returned by the
    // createTopics() call.

    blpapi_TopicList_t *d_handle_p;

  public:
    enum Status {
        NOT_CREATED,
        CREATED,
        FAILURE
    };

    // CLASS METHODS
    TopicList();
        // Create an empty TopicList.

    TopicList(const ResolutionList& original);
        // Create a topic list from ResolutionList.
        // User has previously resolved topics in ResolutionList
        // and wants to now createTopics.

    TopicList(const TopicList& original);
        // Copy constructor.

    ~TopicList();
        // Destroy this TopicList.

    // MANIPULATORS
    int add(char const* topic,
            const CorrelationId& correlationId=CorrelationId());
        // Add the specified 'topic' to this list, optionally
        // specifying a 'correlationId'. Returns 0 on success or
        // negative number on failure. After a successful call to
        // add() the status for this entry is NOT_CREATED.

    int add(Message const& message,
            const CorrelationId& correlationId=CorrelationId());
        // Add the topic contained in the specified
        // 'topicSubscribedMessage' or 'resolutionSuccessMessage'
        // to this list, optionally specifying a 'correlationId'.
        // Returns 0 on success or a negative number on failure.
        // After a successful call to add()
        // the status for this entry is NOT_CREATED.

    // ACCESSORS
    CorrelationId correlationIdAt(size_t index) const;
        // Returns the CorrelationId of the specified 'index'th entry in
        // this TopicList. If 'index' >= size() an exception is
        // thrown.

    char const* topicString(const CorrelationId& correlationId) const;
        // Returns a pointer to the topic of the entry identified by
        // the specified 'correlationId'. If the 'correlationId' does
        // not identify an entry in this TopicList then an
        // exception is thrown.

    char const* topicStringAt(size_t index) const;
        // Returns a pointer to the topic of the specified 'index'th
        // entry. If 'index' >= size() an exception is thrown.

    int status(const CorrelationId& correlationId) const;
        // Returns the status of the entry in this TopicList
        // identified by the specified 'correlationId'. This may be
        // NOT_CREATED, CREATED and FAILURE.
        // If the 'correlationId' does not identify an entry in this
        // TopicList then an exception is thrown.

    int statusAt(size_t index) const;
        // Returns the status of the specified 'index'th entry in this
        // TopicList. This may be NOT_CREATED, CREATED and FAILURE.
        // If 'index' > size() an exception is thrown.

    Message const message(const CorrelationId& correlationId) const;
        // Returns the value of the message received during
        // creation of the topic identified by the specified
        // 'correlationId'. If 'correlationId' does not identify an
        // entry in this TopicList or if the status of the entry
        // identify by 'correlationId' is not CREATED an exception is
        // thrown.
        //
        // The message returned can be used when creating an instance
        // of Topic.

    Message const messageAt(size_t index) const;
        // Returns the value of the message received during creation
        // of the specified 'index'th entry in this TopicList. If
        // 'index' >= size() or if the status of the 'index'th entry
        // is not CREATED an exception is thrown.
        //
        // The message returned can be used when creating an instance
        // of Topic.

    size_t size() const;
        // Returns the number of entries in this list.

    const blpapi_TopicList_t* impl() const;

    blpapi_TopicList_t* impl();
};

// ============================================================================
//                      INLINE FUNCTION DEFINITIONS
// ============================================================================

                            // ---------------
                            // class TopicList
                            // ---------------

inline
TopicList::TopicList()
: d_handle_p(blpapi_TopicList_create(0))
{
}

inline
TopicList::TopicList(
        const TopicList& original)
: d_handle_p(blpapi_TopicList_create(original.d_handle_p))
{
}

inline
TopicList::TopicList(
        const ResolutionList& original)
: d_handle_p(
      blpapi_TopicList_create((blpapi_TopicList_t *)original.impl()))
{
}

inline
TopicList::~TopicList()
{
    blpapi_TopicList_destroy(d_handle_p);
}

inline
int TopicList::add(char const* topic,
                               const CorrelationId& correlationId)
{
    return blpapi_TopicList_add(d_handle_p, topic, &correlationId.impl());
}

inline
int TopicList::add(Message const& message,
                          const CorrelationId& correlationId)
{
    return blpapi_TopicList_addFromMessage(
        d_handle_p, message.impl(), &correlationId.impl());
}

inline
CorrelationId TopicList::correlationIdAt(size_t index) const
{
    blpapi_CorrelationId_t correlationId;
    ExceptionUtil::throwOnError(
        blpapi_TopicList_correlationIdAt(d_handle_p, &correlationId, index));

    return CorrelationId(correlationId);
}

inline
char const* TopicList::topicString(const CorrelationId& correlationId) const
{
    char const* topic;
    ExceptionUtil::throwOnError(
        blpapi_TopicList_topicString(d_handle_p,
                                     &topic,
                                     &correlationId.impl()));

    return topic;
}

inline
char const* TopicList::topicStringAt(size_t index) const
{
    char const* topic;
    ExceptionUtil::throwOnError(
        blpapi_TopicList_topicStringAt(d_handle_p, &topic, index));

    return topic;
}

inline
int TopicList::status(const CorrelationId& correlationId) const
{
    int result;
    ExceptionUtil::throwOnError(
        blpapi_TopicList_status(d_handle_p, &result, &correlationId.impl()));

    return result;
}

inline
int TopicList::statusAt(size_t index) const
{
    int result;
    ExceptionUtil::throwOnError(
        blpapi_TopicList_statusAt(d_handle_p, &result, index));

    return result;
}

inline
Message const TopicList::message(const CorrelationId& correlationId) const
{
    blpapi_Message_t* message;
    ExceptionUtil::throwOnError(
        blpapi_TopicList_message(d_handle_p, &message, &correlationId.impl()));

    BLPAPI_CALL_MESSAGE_ADDREF(message);
    return Message(message, true);
}

inline
Message const TopicList::messageAt(size_t index) const
{
    blpapi_Message_t* message;
    ExceptionUtil::throwOnError(
        blpapi_TopicList_messageAt(d_handle_p, &message, index));

    BLPAPI_CALL_MESSAGE_ADDREF(message);
    return Message(message, true);
}

inline
size_t TopicList::size() const
{
    return blpapi_TopicList_size(d_handle_p);
}

inline
const blpapi_TopicList_t* TopicList::impl() const
{
    return d_handle_p;
}

inline
blpapi_TopicList_t* TopicList::impl()
{
    return d_handle_p;
}

}  // close namespace blpapi
}  // close namespace BloombergLP

#endif // ifdef __cplusplus


#endif // #ifndef INCLUDED_BLPAPI_TOPICLIST
