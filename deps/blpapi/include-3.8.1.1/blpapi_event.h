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
// blpapi_event.h                                                     -*-C++-*-
#ifndef INCLUDED_BLPAPI_EVENT
#define INCLUDED_BLPAPI_EVENT

//@PURPOSE: A component which defines events related operations
//
//@CLASSES:
//            blpapi::Event: an event resulting from a subscription or request.
//       blpapi::EventQueue: A construct to handle replies synchronously.
//  blpapi::MessageIterator: An iterator over the Messages within an Event.
//
//@DESCRIPTION: This file defines an 'Event'. One or more 'Event's are
// generated as a result of a subscription or a request. Events contain Message
// objects which can be accessed using a 'MessageIterator'. This file also
// defines a 'EventQueue' for handling replies synchronously.
//
///Usage
///-----
// The following code snippet shows a how a 'EventQueue' is used with a
// 'generateToken' request. For any established session 'session' pass an
// 'EventQueue' object 'tokenEventQueue' when calling 'generateToken'.
// All Events in responses to 'generateToken' request will be returned in
// 'tokenEventQueue'.
//..
// //...
// EventQueue tokenEventQueue;
// session->generateToken(CorrelationId(), &tokenEventQueue);
//..
// Synchronously read the response 'event' and parse over messages using 'iter'
//..
// Event event = tokenEventQueue.nextEvent();
// if (event.eventType() == Event::TOKEN_STATUS) {
//     MessageIterator iter(event);
//     while (iter.next()) {
//         Message msg = iter.message();
//         //...
//..

#ifndef INCLUDED_BLPAPI_MESSAGE
#include <blpapi_message.h>
#endif

#ifndef INCLUDED_BLPAPI_DEFS
#include <blpapi_defs.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

BLPAPI_EXPORT
int blpapi_Event_eventType(
        const blpapi_Event_t *event);

BLPAPI_EXPORT
int blpapi_Event_addRef(
        const blpapi_Event_t *event);

BLPAPI_EXPORT
int blpapi_Event_release(
        const blpapi_Event_t *event);

BLPAPI_EXPORT
blpapi_EventQueue_t* blpapi_EventQueue_create();

BLPAPI_EXPORT
int blpapi_EventQueue_destroy(blpapi_EventQueue_t* eventQueue);

BLPAPI_EXPORT
blpapi_Event_t* blpapi_EventQueue_nextEvent(
        blpapi_EventQueue_t *eventQueue,
        int timeout);

BLPAPI_EXPORT
int blpapi_EventQueue_purge(blpapi_EventQueue_t *eventQueue);

BLPAPI_EXPORT
int blpapi_EventQueue_tryNextEvent(
        blpapi_EventQueue_t *eventQueue,
        blpapi_Event_t **eventPointer);

BLPAPI_EXPORT
blpapi_MessageIterator_t* blpapi_MessageIterator_create(
        const blpapi_Event_t *event);

BLPAPI_EXPORT
void blpapi_MessageIterator_destroy(
        blpapi_MessageIterator_t* iterator);

BLPAPI_EXPORT
int blpapi_MessageIterator_next(
        blpapi_MessageIterator_t* iterator,
        blpapi_Message_t** result);

#ifdef __cplusplus
}

namespace BloombergLP {
namespace blpapi {

                         // ===========
                         // class Event
                         // ===========
class Event {
    // A single event resulting from a subscription or a request.
    //
    // Event objects are created by the API and passed to the
    // application either through a registered EventHandler or
    // EventQueue or returned from the Session::nextEvent()
    // method. Event objects contain Message objects which can be
    // accessed using a MessageIterator.
    //
    // The Event object is a handle to an event. The event is the
    // basic unit of work provided to applications. Each Event object
    // consists of an EventType attribute and zero or more Message
    // objects. The underlying event data including the messages is
    // reference counted - as long as at least one Event object still
    // exists then the underlying data will not be freed.

    blpapi_Event_t *d_impl_p;

  public:
    enum EventType {
      // The possible types of event
      ADMIN                 = BLPAPI_EVENTTYPE_ADMIN,
        // Admin event
      SESSION_STATUS        = BLPAPI_EVENTTYPE_SESSION_STATUS,
        // Status updates for a session.
      SUBSCRIPTION_STATUS   = BLPAPI_EVENTTYPE_SUBSCRIPTION_STATUS,
        // Status updates for a subscription.
      REQUEST_STATUS        = BLPAPI_EVENTTYPE_REQUEST_STATUS,
        // Status updates for a request.
      RESPONSE              = BLPAPI_EVENTTYPE_RESPONSE,
        // The final (possibly only) response to a request.
      PARTIAL_RESPONSE      = BLPAPI_EVENTTYPE_PARTIAL_RESPONSE,
        // A partial response to a request.
      SUBSCRIPTION_DATA     = BLPAPI_EVENTTYPE_SUBSCRIPTION_DATA,
        // Data updates resulting from a subscription.
      SERVICE_STATUS        = BLPAPI_EVENTTYPE_SERVICE_STATUS,
        // Status updates for a service.
      TIMEOUT               = BLPAPI_EVENTTYPE_TIMEOUT,
        // An Event returned from nextEvent() if it timed out.
      AUTHORIZATION_STATUS  = BLPAPI_EVENTTYPE_AUTHORIZATION_STATUS,
        // Status updates for user authorization.
      RESOLUTION_STATUS     = BLPAPI_EVENTTYPE_RESOLUTION_STATUS,
        // Status updates for a resolution operation.
      TOPIC_STATUS          = BLPAPI_EVENTTYPE_TOPIC_STATUS,
        // Status updates about topics for service providers.
      TOKEN_STATUS          = BLPAPI_EVENTTYPE_TOKEN_STATUS,
        // Status updates for a generate token request.
      REQUEST               = BLPAPI_EVENTTYPE_REQUEST,
        // Request event
      UNKNOWN               = -1
    };

    Event();
        // Construct an uninitialized Event. The only valid operations
        // on an uninitialized Event are assignment, isValid() and
        // destruction.

    Event(blpapi_Event_t *handle);

    Event(const Event& original);
        // Copy constructor. This performs a shallow copy, increasing
        // the reference count on the actual data underlying this
        // handle.

    ~Event();
        // Destructor. If this is the last reference to this Event
        // then the underlying data (including all Messages associated
        // with the Event) are invalidated.

    // MANIPULATORS

    Event& operator=(const Event& rhs);
        // Assignment operator. This performs a shallow assignment,
        // increasing the reference count on the actual data
        // underlying this handle.

    // ACCESSORS

    EventType eventType() const;
        // Returns the type of messages contained by this Event.

    bool isValid() const;
        // Returns true if this Event is a valid event.

    blpapi_Event_t* impl() const;
};

                         // ================
                         // class EventQueue
                         // ================

class EventQueue {
    // A construct used to handle replies to request synchronously.
    //
    // An EventQueue can be supplied when using Session::sendRequest()
    // and Session::sendAuthorizationRequest() methods.
    //
    // When a request is submitted an application can either handle
    // the responses asynchronously as they arrive or use an
    // EventQueue to handle all responses for a given request or
    // requests synchronously. The EventQueue will only deliver
    // responses to the request(s) it is associated with.

    blpapi_EventQueue_t *d_handle_p;

  public:
    EventQueue();
        // Construct an empty event queue.

    ~EventQueue();
        // Destroy this event queue and cancel any pending request
        // that are linked to this queue.

    // MANIPULATORS

    Event nextEvent(int timeout=0);
        // Returns the next Event available from the EventQueue. If
        // the specified 'timeout' is zero this will wait forever for
        // the next event. If the specified 'timeout' is non zero then
        // if no Event is available within the specified 'timeout' an
        // Event with a type() of TIMEOUT will be returned.

    int tryNextEvent(Event *event);
        // If the EventQueue is non-empty, load the next Event available
        // into event and return 0 indicating success. If the EventQueue is
        // empty, return a non-zero value with no effect on event or the
        // the state of EventQueue. This method never blocks.

    void purge();
        // Purges any Event objects in this EventQueue which have not
        // been processed and cancel any pending requests linked to
        // this EventQueue. The EventQueue can subsequently be
        // re-used for a subsequent request.

    blpapi_EventQueue_t* handle() const;
};

                         // =====================
                         // class MessageIterator
                         // =====================

class MessageIterator {
    // An iterator over the Message objects within an Event.
    //
    // MessageIterator objects are used to process the individual
    // Message objects in an Event received in an EventHandler, from
    // EventQueue::nextEvent() or from Session::nextEvent().
    //
    // This class is used to iterate over each message in an
    // Event. The user must ensure that the Event this iterator is
    // created for is not destroyed before the iterator.

    blpapi_MessageIterator_t *d_impl_p;
    blpapi_Message_t         *d_current_p;

  private:
    // NOT IMPLEMENTED
    MessageIterator(const MessageIterator&);
    MessageIterator& operator=(const MessageIterator&);

  public:
    MessageIterator(const Event& event);
        // Construct a forward iterator to iterate over the message in
        // the specified 'event' object. The MessageIterator is
        // created in a state where next() must be called to advance
        // it to the first item.

    ~MessageIterator();
        // Destructor.

    // MANIPULATORS

    bool next();
        // Attempts to advance this MessageIterator to the next
        // Message in this Event. Returns 0 on success and non-zero if
        // there are no more messages. After next() returns 0
        // isValid() returns true, even if called repeatedly until the
        // next call to next(). After next() returns non-zero then
        // isValid() always returns false.

    // ACCESSORS

    bool isValid() const;
        // Returns true if this iterator is currently positioned on a
        // valid Message.  Returns false otherwise.

    Message message(bool createClonable=false) const;
        // Returns the Message at the current position of this iterator. If the
        // specified 'createClonable' flag is set, the internal handle of the
        // message returned is added a reference and the message can outlive
        // the call to next(). If the 'createClonable' flag is set to false,
        // the use of message outside the scope of the iterator or after the
        // next() call is undefined.
        // The behavior is undefined if isValid() returns false.
};

//=============================================================================
//                           INLINE FUNCTION DEFINITIONS
//=============================================================================

                            // -----------
                            // class Event
                            // -----------

inline
Event::Event()
: d_impl_p(0)
{
}

inline
Event::Event(blpapi_Event_t *handle)
: d_impl_p(handle)
{
}

inline
Event::Event(const Event& original)
: d_impl_p(original.d_impl_p)
{
    if (d_impl_p) {
        blpapi_Event_addRef(d_impl_p);
    }
}

inline
Event::~Event()
{
    if (d_impl_p) {
        blpapi_Event_release(d_impl_p);
    }
}

inline
Event& Event::operator=(const Event& rhs)
{
    if (this == &rhs) {
        return *this;
    }
    if (d_impl_p) {
        blpapi_Event_release(d_impl_p);
    }
    d_impl_p = rhs.d_impl_p;
    if (d_impl_p) {
        blpapi_Event_addRef(d_impl_p);
    }
    return *this;
}

inline
Event::EventType Event::eventType() const
{
    return (EventType) blpapi_Event_eventType(d_impl_p);
}

inline
bool Event::isValid() const
{
    return d_impl_p ? true : false;
}

inline
blpapi_Event_t* Event::impl() const
{
    return d_impl_p;
}

                            // ----------------
                            // class EventQueue
                            // ----------------

inline
EventQueue::EventQueue()
{
    d_handle_p = blpapi_EventQueue_create();
}

inline
EventQueue::~EventQueue()
{
    blpapi_EventQueue_destroy(d_handle_p);
}

inline
Event EventQueue::nextEvent(int timeout)
{
    return blpapi_EventQueue_nextEvent(d_handle_p, timeout);
}

inline
int EventQueue::tryNextEvent(Event *event)
{
    blpapi_Event_t *impl;
    int ret = blpapi_EventQueue_tryNextEvent(d_handle_p, &impl);
    if(0 == ret) {
        *event = Event(impl);
    }
    return ret;
}

inline
void EventQueue::purge()
{
    blpapi_EventQueue_purge(d_handle_p);
}

inline
blpapi_EventQueue_t* EventQueue::handle() const
{
    return d_handle_p;
}

                            // ---------------------
                            // class MessageIterator
                            // ---------------------

inline
MessageIterator::MessageIterator(const Event& event)
: d_impl_p(0)
, d_current_p(0)
{
    d_impl_p = blpapi_MessageIterator_create(event.impl());
}

inline
MessageIterator::~MessageIterator()
{
    blpapi_MessageIterator_destroy(d_impl_p);
}

inline
bool MessageIterator::next()
{
    return !blpapi_MessageIterator_next(d_impl_p, &d_current_p);
}

inline
bool MessageIterator::isValid() const
{
    return d_current_p ? true : false;
}

inline
Message MessageIterator::message(bool createClonable) const
{
    if (createClonable) {
        BLPAPI_CALL_MESSAGE_ADDREF(d_current_p);
    }
    return Message(d_current_p, createClonable);
}


}  // close namespace blpapi
}  // close namespace BloombergLP

#endif // #ifdef __cplusplus
#endif // #ifndef INCLUDED_BLPAPI_EVENT
