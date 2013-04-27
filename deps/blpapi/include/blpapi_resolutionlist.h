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
// blpapi_resolutionlist.h                                            -*-C++-*-
#ifndef INCLUDED_BLPAPI_RESOLUTIONLIST
#define INCLUDED_BLPAPI_RESOLUTIONLIST

//@PURPOSE: Provide a representation of a list of topics.
//
//@CLASSES:
// blpapi::ResolutionList: Represents a list of topics
//
//@SEE_ALSO: blpapi_event
//
//@DESCRIPTION: This component implements a list of topics that require
// resolution.

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

#ifndef INCLUDED_STDDEF
#include <stddef.h>
#define INCLUDED_STDDEF
#endif

struct blpapi_ResolutionList;
typedef struct blpapi_ResolutionList blpapi_ResolutionList_t;

#ifdef __cplusplus
extern "C" {
#endif

BLPAPI_EXPORT
blpapi_Element_t* blpapi_ResolutionList_extractAttributeFromResolutionSuccess(
    const blpapi_Message_t* message,
    const blpapi_Name_t* attribute);

BLPAPI_EXPORT
blpapi_ResolutionList_t* blpapi_ResolutionList_create(
    blpapi_ResolutionList_t* from);

BLPAPI_EXPORT
void blpapi_ResolutionList_destroy(
    blpapi_ResolutionList_t *list);

BLPAPI_EXPORT
int blpapi_ResolutionList_add(
    blpapi_ResolutionList_t* list,
    const char* topic,
    const blpapi_CorrelationId_t *correlationId);

BLPAPI_EXPORT
int blpapi_ResolutionList_addFromMessage(
    blpapi_ResolutionList_t* list,
    const blpapi_Message_t* topic,
    const blpapi_CorrelationId_t *correlationId);

BLPAPI_EXPORT
int blpapi_ResolutionList_addAttribute(
    blpapi_ResolutionList_t* list,
    const blpapi_Name_t* name);

BLPAPI_EXPORT
int blpapi_ResolutionList_correlationIdAt(
    const blpapi_ResolutionList_t* list,
    blpapi_CorrelationId_t* result,
    size_t index);

BLPAPI_EXPORT
int blpapi_ResolutionList_topicString(
    const blpapi_ResolutionList_t* list,
    const char** topic,
    const blpapi_CorrelationId_t* id);

BLPAPI_EXPORT
int blpapi_ResolutionList_topicStringAt(
    const blpapi_ResolutionList_t* list,
    const char** topic,
    size_t index);

BLPAPI_EXPORT
int blpapi_ResolutionList_status(
    const blpapi_ResolutionList_t* list,
    int* status,
    const blpapi_CorrelationId_t* id);

BLPAPI_EXPORT
int blpapi_ResolutionList_statusAt(
    const blpapi_ResolutionList_t* list,
    int* status,
    size_t index);

BLPAPI_EXPORT
int blpapi_ResolutionList_attribute(
    const blpapi_ResolutionList_t* list,
    blpapi_Element_t** element,
    const blpapi_Name_t* attribute,
    const blpapi_CorrelationId_t* id);

BLPAPI_EXPORT
int blpapi_ResolutionList_attributeAt(
    const blpapi_ResolutionList_t* list,
    blpapi_Element_t** element,
    const blpapi_Name_t* attribute,
    size_t index);

BLPAPI_EXPORT
int blpapi_ResolutionList_message(
    const blpapi_ResolutionList_t* list,
    blpapi_Message_t** element,
    const blpapi_CorrelationId_t* id);

BLPAPI_EXPORT
int blpapi_ResolutionList_messageAt(
    const blpapi_ResolutionList_t* list,
    blpapi_Message_t** element,
    size_t index);

BLPAPI_EXPORT
int blpapi_ResolutionList_size(
    const blpapi_ResolutionList_t* list);

#ifdef __cplusplus
}

#ifndef INCLUDED_BLPAPI_EXCEPTION
#include <blpapi_exception.h>
#endif

namespace BloombergLP {
namespace blpapi {
                         // ====================
                         // class ResolutionList
                         // ====================

class ResolutionList {
    // Contains a list of topics that require resolution.
    //
    // Created from topic strings or from SUBSCRIPTION_STARTED
    // messages. This is passed to a resolve() call or resolveAsync()
    // call on a ProviderSession. It is updated and returned by the
    // resolve() call.

    blpapi_ResolutionList_t *d_handle_p;

  public:
    enum Status {
        UNRESOLVED,
        RESOLVED,
        RESOLUTION_FAILURE_BAD_SERVICE,
        RESOLUTION_FAILURE_SERVICE_AUTHORIZATION_FAILED,
        RESOLUTION_FAILURE_BAD_TOPIC,
        RESOLUTION_FAILURE_TOPIC_AUTHORIZATION_FAILED
    };

    // CLASS METHODS
    static Element extractAttributeFromResolutionSuccess(
                                                        Message const& message,
                                                        Name const& attribute);
        // Return the value of the value in the specified 'message'
        // which represents the specified 'attribute'. The 'message'
        // must be a message of type "RESOLUTION_SUCCESS". The
        // 'attribute' should be an attribute that was requested using
        // addAttribute() on the ResolutionList passed to the
        // resolve() or resolveAsync() that caused this
        // RESOLUTION_SUCCESS message. If the 'attribute' is not
        // present an empty Element is returned.

    ResolutionList();
        // Create an empty ResolutionList.

    ResolutionList(const ResolutionList& original);
        // Copy constructor.

    ~ResolutionList();
        // Destroy this ResolutionList.

    // MANIPULATORS

    int add(const char* topic,
            const CorrelationId& correlationId=CorrelationId());
        // Add the specified 'topic' to this list, optionally
        // specifying a 'correlationId'. Returns 0 on success or
        // negative number on failure. After a successful call to
        // add() the status for this entry is UNRESOLVED_TOPIC.

    int add(Message const& subscriptionStartedMessage,
            const CorrelationId& correlationId=CorrelationId());
        // Add the topic contained in the specified
        // 'subscriptionStartedMessage' to this list, optionally
        // specifying a 'correlationId'.  Returns 0 on success or a
        // negative number on failure. After a successful call to add()
        // the status for this entry is UNRESOLVED_TOPIC.

    int addAttribute(const Name& attribute);
        // Add the specified 'attribute' to the list of attributes
        // requested during resolution for each topic in this
        // ResolutionList. Returns 0 on success or a negative number
        // on failure.

    // ACCESSORS

    CorrelationId correlationIdAt(size_t index) const;
        // Returns the CorrelationId of the specified 'index'th entry in
        // this ResolutionList. If 'index' >= size() an exception is
        // thrown.

    const char* topicString(const CorrelationId& correlationId) const;
        // Returns a pointer to the topic of the entry identified by
        // the specified 'correlationId'. If the 'correlationId' does
        // not identify an entry in this ResolutionList then an
        // exception is thrown.

    const char* topicStringAt(size_t index) const;
        // Returns a pointer to the topic of the specified 'index'th
        // entry. If 'index' >= size() an exception is thrown.

    int status(const CorrelationId& correlationId) const;
        // Returns the status of the entry in this ResolutionList
        // identified by the specified 'correlationId'. This may be
        // UNRESOLVED, RESOLVED, RESOLUTION_FAILURE_BAD_SERVICE,
        // RESOLUTION_FAILURE_SERVICE_AUTHORIZATION_FAILED
        // RESOLUTION_FAILURE_BAD_TOPIC,
        // RESOLUTION_FAILURE_TOPIC_AUTHORIZATION_FAILED.  If the
        // 'correlationId' does not identify an entry in this
        // ResolutionList then an exception is thrown.

    int statusAt(size_t index) const;
        // Returns the status of the specified 'index'th entry in this
        // ResolutionList. This may be UNRESOLVED,
        // RESOLVED, RESOLUTION_FAILURE_BAD_SERVICE,
        // RESOLUTION_FAILURE_SERVICE_AUTHORIZATION_FAILED
        // RESOLUTION_FAILURE_BAD_TOPIC,
        // RESOLUTION_FAILURE_TOPIC_AUTHORIZATION_FAILED.  If 'index'
        // > size() an exception is thrown.

    Element const attribute(const Name& attribute,
                            const CorrelationId& correlationId) const;
        // Returns the value for the specified 'attribute' of the
        // entry in this ResolutionList identified by the specified
        // 'correlationId'. The Element returned may be empty if the
        // resolution service cannot provide the attribute. If
        // 'correlationId' does not identify an entry in this
        // ResolutionList or if the status of the entry identified by
        // 'correlationId' is not RESOLVED an exception is thrown.

    Element const attributeAt(const Name& attribute, size_t index) const;
        // Returns the value for the specified 'attribute' of the
        // specified 'index'th entry in this ResolutionList. The
        // Element returned may be empty if the resolution service
        // cannot provide the attribute. If 'index' >= size() or if
        // the status of the 'index'th entry is not RESOLVED an
        // exception is thrown.

    Message const message(const CorrelationId& correlationId) const;
        // Returns the value of the message received during resolution
        // of the topic identified by the specified
        // 'correlationId'. If 'correlationId' does not identify an
        // entry in this ResolutionList or if the status of the entry
        // identify by 'correlationId' is not RESOLVED an exception is
        // thrown.
        //
        // The message returned can be used when creating an instance
        // of Topic.

    Message const messageAt(size_t index) const;
        // Returns the value of the message received during resolution
        // of the specified 'index'th entry in this ResolutionList. If
        // 'index' >= size() or if the status of the 'index'th entry
        // is not RESOLVED an exception is thrown.
        //
        // The message returned can be used when creating an instance
        // of Topic.

    size_t size() const;
        // Returns the number of entries in this list.

    const blpapi_ResolutionList_t* impl() const;

    blpapi_ResolutionList_t* impl();
};

// ============================================================================
//                      INLINE FUNCTION DEFINITIONS
// ============================================================================

                            // --------------------
                            // class ResolutionList
                            // --------------------

inline
Element ResolutionList::extractAttributeFromResolutionSuccess(
     Message const& message,
     Name const& attribute)
{
    Element result(blpapi_ResolutionList_extractAttributeFromResolutionSuccess(
                                                            message.impl(),
                                                            attribute.impl()));
    return result;
}

inline
ResolutionList::ResolutionList()
: d_handle_p(blpapi_ResolutionList_create(0))
{
}

inline
ResolutionList::ResolutionList(const ResolutionList& original)
: d_handle_p(blpapi_ResolutionList_create(original.d_handle_p))
{

}

inline
ResolutionList::~ResolutionList()
{
    blpapi_ResolutionList_destroy(d_handle_p);
}

inline
int ResolutionList::add(const char* topic,
                               const CorrelationId& correlationId)
{
    return blpapi_ResolutionList_add(d_handle_p, topic, &correlationId.impl());
}

inline
int ResolutionList::add(Message const& subscriptionStartedMessage,
                               const CorrelationId& correlationId)
{
    return blpapi_ResolutionList_addFromMessage(
        d_handle_p, subscriptionStartedMessage.impl(),
        &correlationId.impl());
}

inline
int ResolutionList::addAttribute(const Name& attribute)
{
    return blpapi_ResolutionList_addAttribute(d_handle_p, attribute.impl());
}

inline
CorrelationId ResolutionList::correlationIdAt(size_t index) const
{
    blpapi_CorrelationId_t correlationId;
    ExceptionUtil::throwOnError(
        blpapi_ResolutionList_correlationIdAt(d_handle_p,
                                              &correlationId,
                                              index));

    return CorrelationId(correlationId);
}

inline
const char* ResolutionList::topicString(
                                      const CorrelationId& correlationId) const
{
    const char* topic;
    ExceptionUtil::throwOnError(
        blpapi_ResolutionList_topicString(d_handle_p,
                                          &topic,
                                          &correlationId.impl()));

    return topic;
}

inline
const char* ResolutionList::topicStringAt(size_t index) const
{
    const char* topic;
    ExceptionUtil::throwOnError(
        blpapi_ResolutionList_topicStringAt(d_handle_p, &topic, index));

    return topic;
}

inline
int ResolutionList::status(const CorrelationId& correlationId) const
{
    int result;
    ExceptionUtil::throwOnError(
        blpapi_ResolutionList_status(d_handle_p,
                                     &result,
                                     &correlationId.impl()));

    return result;
}

inline
int ResolutionList::statusAt(size_t index) const
{
    int result;
    ExceptionUtil::throwOnError(
        blpapi_ResolutionList_statusAt(d_handle_p, &result, index));

    return result;
}

inline
Element const ResolutionList::attribute(
                                     const Name& attribute,
                                     const CorrelationId& correlationId) const
{
    blpapi_Element_t* element;
    ExceptionUtil::throwOnError(
        blpapi_ResolutionList_attribute(d_handle_p, &element, attribute.impl(),
                                        &correlationId.impl()));
    return Element(element);
}

inline
Element const ResolutionList::attributeAt(const Name& attribute,
                                          size_t index) const
{
    blpapi_Element_t* element;
    ExceptionUtil::throwOnError(
        blpapi_ResolutionList_attributeAt(d_handle_p,
                                          &element,
                                          attribute.impl(),
                                          index));
    return Element(element);
}

inline
Message const ResolutionList::message(const CorrelationId& correlationId) const
{
    blpapi_Message_t* message;
    ExceptionUtil::throwOnError(
        blpapi_ResolutionList_message(d_handle_p,
                                      &message,
                                      &correlationId.impl()));

    bool makeMessageCopyable = true;
    BLPAPI_CALL_MESSAGE_ADDREF(message);
    return Message(message, makeMessageCopyable);
}

inline
Message const ResolutionList::messageAt(size_t index) const
{
    blpapi_Message_t* message;
    ExceptionUtil::throwOnError(
        blpapi_ResolutionList_messageAt(d_handle_p, &message, index));

    bool makeMessageCopyable = true;
    BLPAPI_CALL_MESSAGE_ADDREF(message);
    return Message(message, makeMessageCopyable);
}

inline
size_t ResolutionList::size() const
{
    return blpapi_ResolutionList_size(d_handle_p);
}

inline
const blpapi_ResolutionList_t* ResolutionList::impl() const
{
    return d_handle_p;
}

inline
blpapi_ResolutionList_t* ResolutionList::impl()
{
    return d_handle_p;
}

}  // close namespace blpapi
}  // close namespace BloombergLP

#endif // #ifdef __cplusplus
#endif // #ifndef INCLUDED_BLPAPI_RESOLUTIONLIST
