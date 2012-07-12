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
// blpapi_providersession.h                                           -*-C++-*-
#ifndef INCLUDED_BLPAPI_PROVIDERSESSION
#define INCLUDED_BLPAPI_PROVIDERSESSION

//@PURPOSE: Provide a session that can be used for providing services
//
//@CLASSES:
// blpapi::ProviderSession: Session with providing(publishing) services.
// blpapi::ProviderEventHandler: Event handler for ProviderSession
// blpapi::ServiceRegistrationOptions: Container holding registration options.
//
//@DESCRIPTION: ProviderSession inherits from AbstractSession. In addition to
// AbstractSession functionality, ProviderSession provides functions that are
// needed to support publishing like 'registerService', 'createTopics' and
// 'publish'.

#ifndef INCLUDED_BLPAPI_ABSTRACTSESSION
#include <blpapi_abstractsession.h>
#endif

#ifndef INCLUDED_BLPAPI_CORRELATIONID
#include <blpapi_correlationid.h>
#endif

#ifndef INCLUDED_BLPAPI_DEFS
#include <blpapi_defs.h>
#endif

#ifndef INCLUDED_BLPAPI_EVENT
#include <blpapi_event.h>
#endif

#ifndef INCLUDED_BLPAPI_EVENTDISPATCHER
#include <blpapi_eventdispatcher.h>
#endif

#ifndef INCLUDED_BLPAPI_IDENTITY
#include <blpapi_identity.h>
#endif

#ifndef INCLUDED_BLPAPI_REQUEST
#include <blpapi_request.h>
#endif

#ifndef INCLUDED_BLPAPI_RESOLUTIONLIST
#include <blpapi_resolutionlist.h>
#endif

#ifndef INCLUDED_BLPAPI_SERVICE
#include <blpapi_service.h>
#endif

#ifndef INCLUDED_BLPAPI_SESSIONOPTIONS
#include <blpapi_sessionoptions.h>
#endif

#ifndef INCLUDED_BLPAPI_SUBSCRIPTIONLIST
#include <blpapi_subscriptionlist.h>
#endif

#ifndef INCLUDED_BLPAPI_TOPIC
#include <blpapi_topic.h>
#endif

#ifndef INCLUDED_BLPAPI_TOPICLIST
#include <blpapi_topiclist.h>
#endif

#ifndef INCLUDED_BLPAPI_TYPES
#include <blpapi_types.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct blpapi_ServiceRegistrationOptions;
typedef struct blpapi_ServiceRegistrationOptions
    blpapi_ServiceRegistrationOptions_t;

typedef void(*blpapi_ProviderEventHandler_t)(blpapi_Event_t   *event,
                                     blpapi_ProviderSession_t *session,
                                     void                     *userData);

BLPAPI_EXPORT
blpapi_ProviderSession_t* blpapi_ProviderSession_create(
        blpapi_SessionOptions_t *parameters,
        blpapi_ProviderEventHandler_t handler,
        blpapi_EventDispatcher_t* dispatcher,
        void *userData);

BLPAPI_EXPORT
void blpapi_ProviderSession_destroy(
    blpapi_ProviderSession_t* session);

BLPAPI_EXPORT
int blpapi_ProviderSession_start(
    blpapi_ProviderSession_t *session);

BLPAPI_EXPORT
int blpapi_ProviderSession_startAsync(
    blpapi_ProviderSession_t *session);

BLPAPI_EXPORT
int blpapi_ProviderSession_stop(
    blpapi_ProviderSession_t* session);

BLPAPI_EXPORT
int blpapi_ProviderSession_stopAsync(
    blpapi_ProviderSession_t* session);

BLPAPI_EXPORT
int blpapi_ProviderSession_nextEvent(
    blpapi_ProviderSession_t* session,
    blpapi_Event_t **eventPointer,
    unsigned int timeoutInMilliseconds);

BLPAPI_EXPORT
int blpapi_ProviderSession_tryNextEvent(
    blpapi_ProviderSession_t* session,
    blpapi_Event_t **eventPointer);

BLPAPI_EXPORT
int blpapi_ProviderSession_registerService(
        blpapi_ProviderSession_t *session,
        const char* serviceName,
        blpapi_Identity_t const* identity,
        blpapi_ServiceRegistrationOptions_t *registrationOptions);

BLPAPI_EXPORT
int blpapi_ProviderSession_registerServiceAsync(
        blpapi_ProviderSession_t *session,
        const char* serviceName,
        blpapi_Identity_t const* identity,
        blpapi_CorrelationId_t *correlationId,
        blpapi_ServiceRegistrationOptions_t *registrationOptions);

BLPAPI_EXPORT
int blpapi_ProviderSession_resolve(
    blpapi_ProviderSession_t* session,
    blpapi_ResolutionList_t* resolutionList,
    int resolveMode,
    blpapi_Identity_t const* identity);

BLPAPI_EXPORT
int blpapi_ProviderSession_resolveAsync(
    blpapi_ProviderSession_t* session,
    const blpapi_ResolutionList_t* resolutionList,
    int resolveMode,
    blpapi_Identity_t const* identity);

BLPAPI_EXPORT
int blpapi_ProviderSession_createTopics(
    blpapi_ProviderSession_t* session,
    blpapi_TopicList_t* topicList,
    int resolveMode,
    blpapi_Identity_t const* identity);

BLPAPI_EXPORT
int blpapi_ProviderSession_createTopicsAsync(
    blpapi_ProviderSession_t* session,
    const blpapi_TopicList_t* topicList,
    int resolveMode,
    blpapi_Identity_t const* identity);

BLPAPI_EXPORT
int blpapi_ProviderSession_getTopic(
    blpapi_ProviderSession_t* session,
    const blpapi_Message_t* message,
    blpapi_Topic_t** topic);

BLPAPI_EXPORT
int blpapi_ProviderSession_createTopic(
    blpapi_ProviderSession_t* session,
    const blpapi_Message_t* message,
    blpapi_Topic_t** topic);

BLPAPI_EXPORT
int blpapi_ProviderSession_createServiceStatusTopic(
    blpapi_ProviderSession_t* session,
    const blpapi_Service_t *service,
    blpapi_Topic_t** topic);

BLPAPI_EXPORT
int blpapi_ProviderSession_publish(
    blpapi_ProviderSession_t* session,
    blpapi_Event_t* event);

BLPAPI_EXPORT
int blpapi_ProviderSession_sendResponse(
    blpapi_ProviderSession_t* session,
    blpapi_Event_t* event,
    int isPartialResponse);

BLPAPI_EXPORT
blpapi_AbstractSession_t* blpapi_ProviderSession_getAbstractSession(
    blpapi_ProviderSession_t* session);

// ServiceRegistrationOptions

BLPAPI_EXPORT
blpapi_ServiceRegistrationOptions_t*
blpapi_ServiceRegistrationOptions_create();

BLPAPI_EXPORT
blpapi_ServiceRegistrationOptions_t*
blpapi_ServiceRegistrationOptions_duplicate(
                        const blpapi_ServiceRegistrationOptions_t* parameters);

BLPAPI_EXPORT
void blpapi_ServiceRegistrationOptions_destroy(
    blpapi_ServiceRegistrationOptions_t *parameters);

BLPAPI_EXPORT
void blpapi_ServiceRegistrationOptions_copy(
                               blpapi_ServiceRegistrationOptions_t* lhs,
                               const blpapi_ServiceRegistrationOptions_t* rhs);

BLPAPI_EXPORT
void blpapi_ServiceRegistrationOptions_setGroupId(
    blpapi_ServiceRegistrationOptions_t *parameters,
    const char* groupId, unsigned int groupIdLength);

BLPAPI_EXPORT
int blpapi_ServiceRegistrationOptions_setServicePriority(
    blpapi_ServiceRegistrationOptions_t *parameters,
    int priority);

BLPAPI_EXPORT
int blpapi_ServiceRegistrationOptions_getGroupId(
    blpapi_ServiceRegistrationOptions_t *parameters,
    const char* groupdIdBuffer,
    int *groupIdLength);

BLPAPI_EXPORT
int blpapi_ServiceRegistrationOptions_getServicePriority(
    blpapi_ServiceRegistrationOptions_t *parameters);

#ifdef __cplusplus
}

namespace BloombergLP {
namespace blpapi {

static void providerEventHandlerProxy(blpapi_Event_t           *event,
                                      blpapi_ProviderSession_t *session,
                                      void                     *userData);

class ProviderSession;
                         // ==========================
                         // class ProviderEventHandler
                         // ==========================

class ProviderEventHandler {
  public:
    virtual ~ProviderEventHandler() {}

    virtual bool processEvent(const Event& event,
                              ProviderSession *session) = 0;
};
                      // ================================
                      // class ServiceRegistrationOptions
                      // ================================

class ServiceRegistrationOptions {
    // Contains the options which the user can specify when registering
    // a service
    //
    // To use non-default options to registerService, create a
    // ServiceRegistrationOptions instance and set the required options and
    // then supply it when using the registerService interface.

    blpapi_ServiceRegistrationOptions_t *d_handle_p;
  public:

    enum ServiceRegistrationPriority {
        PRIORITY_LOW    = BLPAPI_SERVICEREGISTRATIONOPTIONS_PRIORITY_LOW,
        PRIORITY_MEDIUM = BLPAPI_SERVICEREGISTRATIONOPTIONS_PRIORITY_MEDIUM,
        PRIORITY_HIGH   = BLPAPI_SERVICEREGISTRATIONOPTIONS_PRIORITY_HIGH
    };

    ServiceRegistrationOptions();
        // Create ServiceRegistrationOptions with default options.

    ServiceRegistrationOptions(const ServiceRegistrationOptions& original);
        // Copy Constructor

    ~ServiceRegistrationOptions();
        // Destroy this ServiceRegistrationOptions.

    // MANIUPLATORS

    ServiceRegistrationOptions& operator=(
                                        const ServiceRegistrationOptions& rhs);
        // Assign to this object the value of the specified 'rhs' object.

    void setGroupId(const char* groupId, unsigned int groupIdLength);
        // Set the Group ID for the service to be registered to the specified
        // char array beginning at groupId with size groupIdLength.
        // If groupIdLength > MAX_GROUP_ID_SIZE (=64) only the first
        // MAX_GROUP_ID_SIZE chars are considered as Group Id.

    int setServicePriority(int priority);
        // Set the priority with which a service will be registered to
        // the non-negative value specified in priority.
        // This call returns with a non-zero value indicating error when
        // a negative priority is specified.
        // Any non-negative priority value, other than the one
        // pre-defined in ServiceRegistrationPriority can be used.
        // Default value is PRIORITY_HIGH

    // ACCESSORS

    blpapi_ServiceRegistrationOptions_t* handle() const;

    int getGroupId(const char* groupId, int *groupIdLength) const;
        // Copy the previously specified groupId at the memory location
        // specified by groupId and the set the size of the groupId returned at
        // groupIdLength. A Non-zero value indicates an error.
        // The behavior is undefined if there is not enough space to copy the
        // group Id to the specified buffer.
        // Note that the groupId is not null terminated and buffer for groupId
        // needs to be at most MAX_GROUP_ID_SIZE.

    int getServicePriority() const;
        // Return the value of the service priority in this
        // ServiceRegistrationOptions instance.

};
                         // =====================
                         // class ProviderSession
                         // =====================

class ProviderSession : public AbstractSession {
    // This class provides a session that can be used for providing services.
    //
    // It inherits from AbstractSession. In addition to the AbstractSession
    // functionality a ProviderSession provides the following
    // functions to applications.
    //
    // Applications can register to provide Services using either
    // registerService() or registerServiceAsync(). Before registering
    // to provide a Service an application must have established its
    // identity.
    //
    // Applications use the ProviderSession to resolve topics using
    // resolve() or resolveAsync(), create the resulting Topic objects
    // using createTopic() and then publish(). Another way to start
    // publishing is the call createTopics or createTopicsAsync
    // with a list of topics. This call internally resolves and
    // creates topics for user.
    // Events containing Messages addressed to specific topics.

    blpapi_ProviderSession_t    *d_handle_p;
    ProviderEventHandler        *d_eventHandler_p;

  private:
    friend void providerEventHandlerProxy(blpapi_Event_t           *event,
                                          blpapi_ProviderSession_t *session,
                                          void                     *userData);

    void dispatchEvent(const Event& event);

  private:
    // NOT IMPLEMENTED
    ProviderSession(const ProviderSession&);
    ProviderSession& operator=(const ProviderSession&);

  public:
    enum ResolveMode {
      AUTO_REGISTER_SERVICES = BLPAPI_RESOLVEMODE_AUTO_REGISTER_SERVICES,
              // Try to register services found in ResolutionList if necessary.
      DONT_REGISTER_SERVICES = BLPAPI_RESOLVEMODE_DONT_REGISTER_SERVICES
              // Fail to resolve a topic if the service has not already been
              // registered.
    };

    ProviderSession(const SessionOptions& options = SessionOptions(),
                    ProviderEventHandler* eventHandler=0,
                    EventDispatcher* eventDispatcher=0);
        // Construct a Session using the optionally specified
        // 'options', the optionally specified 'eventHandler' and the
        // optionally specified 'eventDispatcher'.
        //
        // See the SessionOptions documentation for details on what
        // can be specified in the 'options'.
        //
        // If 'eventHandler' is not 0 then this Session will operation
        // in asynchronous mode, otherwise the Session will operate in
        // synchronous mode.
        //
        // If 'eventDispatcher' is 0 then the Session will create a
        // default EventDispatcher for this Session which will use a
        // single thread for dispatching events. For more control over
        // event dispatching a specific instance of EventDispatcher
        // can be supplied. This can be used to share a single
        // EventDispatcher amongst multiple Session objects.
        //
        // If an 'eventDispatcher' is supplied which uses more than
        // one thread the Session will ensure that events which should
        // be ordered are passed to callbacks in a correct order. For
        // example, partial response to a request or updates to a
        // single subscription.
        //
        // If 'eventHandler' is 0 and the 'eventDispatcher' is not
        // 0 an exception is thrown.
        //
        // Each EventDispatcher uses its own thread or pool of
        // threads so if you want to ensure that a session which
        // receives very large messages and takes a long time to
        // process them does not delay a session that receives small
        // messages and processes each one very quickly then give each
        // one a separate EventDispatcher.

    explicit ProviderSession(blpapi_ProviderSession_t *handle);

    virtual ~ProviderSession();
        // Destructor.

    // MANIPULATORS
    virtual bool start();
        // See AbstractSession::start().

    virtual bool startAsync();
        // See AbstractSession::startAsync().

    virtual void stop();
        // See AbstractSession::stop().

    virtual void stopAsync();
        // See AbstractSession::stopAsync().

    virtual Event nextEvent(int timeout=0);
        // See AbstractSession::nextEvent().

    virtual int tryNextEvent(Event *event);
        // See AbstractSession::tryNextEvent()

    bool registerService(
                        const char                        *uri,
                        const Identity&                    providerIdentity
                            = Identity(),
                        const ServiceRegistrationOptions&  registrationOptions
                            = ServiceRegistrationOptions());
        // Attempt to register the service identified by the specified
        // 'uri' and block until the service is either registered
        // successfully or has failed to be registered. The optionally
        // specified 'providerIdentity' is used to verify permissions
        // to provide the service being registered. The optionally
        // specified 'registrationOptions' is used to specify the
        // group ID and service priority of the service being registered.
        // Returns 'true' if the service is registered successfully and
        // 'false' if the service cannot be registered successfully.
        //
        // The 'uri' must begin with a full qualified service
        // name. That is it must begin with
        // "//<namespace>/<service-name>[/]". Any portion of the 'uri'
        // after the service name is ignored.
        //
        // Before registerService() returns a SERVICE_STATUS Event is
        // generated. If this is an asynchronous ProviderSession then
        // this Event may be processed by the registered Event before
        // registerService() has returned.

    CorrelationId registerServiceAsync(
                        const char                        *uri,
                        const Identity&                    providerIdentity
                            = Identity(),
                        const CorrelationId&               correlationId
                            = CorrelationId(),
                        const ServiceRegistrationOptions&  registrationOptions
                            = ServiceRegistrationOptions());
        // Begin the process of registering the service identified by
        // the specified 'uri' and return immediately. The optionally
        // specified 'providerIdentity' is used to verify permissions
        // to provide the service being registered. The optionally
        // specified 'correlationId' is used to track Events generated
        // as a result of this call. The actual correlationId that will
        // identify Events generated as a result of this call is
        // returned. The optionally specified 'registrationOptions' is
        // used to specify the group ID and service priority of the
        // service being registered.
        //
        // The 'uri' must begin with a full qualified service
        // name. That is it must begin with
        // "//<namespace>/<service-name>[/]". Any portion of the 'uri'
        // after the service name is ignored.
        //
        // The application must monitor events for a SERVICE_STATUS
        // Event which will be generated once the service has been
        // successfully registered or registration has failed.

    void resolve(ResolutionList* resolutionList,
                 ResolveMode resolveMode=DONT_REGISTER_SERVICES,
                 Identity providerIdentity=Identity());
        // Resolves the topics in the specified 'resolutionList' and
        // updates the 'resolutionList' with the results of the
        // resolution process. If the specified 'resolveMode' is
        // DONT_REGISTER_SERVICES (the default) then all the services
        // referenced in the topics in the 'resolutionList' must
        // already have been registered using registerService(). If
        // 'resolveMode' is AUTO_REGISTER_SERVICES then the specified
        // 'providerIdentity' should be supplied and ProviderSession
        // will automatically attempt to register any services
        // reference in the topics in the 'resolutionList' that have
        // not already been registered. Once resolveSync() returns
        // each entry in the 'resolutionList' will have been updated
        // with a new status.
        //
        // Before resolveSync() returns one or more RESOLUTION_STATUS
        // events and, if 'resolveMode' is AUTO_REGISTER_SERVICES,
        // zero or more SERVICE_STATUS events are generated. If this
        // is an asynchronous ProviderSession then these Events may be
        // processed by the registered EventHandler before resolve()
        // has returned.

    void resolveAsync(const ResolutionList& resolutionList,
                      ResolveMode resolveMode=DONT_REGISTER_SERVICES,
                      const Identity& providerIdentity=Identity());
        // Begin the resolution of the topics in the specified
        // 'resolutionList'. If the specified 'resolveMode' is
        // DONT_REGISTER_SERVICES (the default) then all the services
        // referenced in the topics in the 'resolutionList' must
        // already have been registered using registerService(). If
        // 'resolveMode' is AUTO_REGISTER_SERVICES then the specified
        // 'providerIdentity' should be supplied and ProviderSession
        // will automatically attempt to register any services
        // reference in the topics in the 'resolutionList' that have
        // not already been registered.
        //
        // One or more RESOLUTION_STATUS events will be delivered with
        // the results of the resolution. These events may be
        // generated before or after resolve() returns. If
        // AUTO_REGISTER_SERVICES is specified SERVICE_STATUS events
        // may also be generated before or after resolve() returns.

    Topic createTopic(const Message& message);
        // DEPRECATED method, see createTopics()/createTopicsAsync(), and
        // getTopic().

    Topic getTopic(const Message& message);
        // Finds a previously created Topic object based on the specified
        // 'message'. The 'message' must be one of the following
        // types: TopicCreated, TopicActivated, TopicDeactivated,
        // TopicSubscribed, TopicUnsubscribed, TopicRecap.
        // If the 'message' is not valid then invoking isValid() on the
        // returned Topic will return false.

    Topic createServiceStatusTopic(const Service& service);
        // Creates a Service Status Topic which is to be used to provide
        // service status. On success invoking isValid() on the returned Topic
        // will return false.

    void publish(const Event& event);
        // Publish the specified 'event'.

    void sendResponse(const Event& event, bool isPartialResponse = false);
        // Send the response event for previously received request

    void createTopics(TopicList* topicList,
                      ResolveMode resolveMode=DONT_REGISTER_SERVICES,
                      Identity providerIdentity=Identity());
        // Creates the topics in the specified 'topicList' and
        // updates the 'topicList' with the results of the
        // creation process. If service needs to be registered,
        // 'providerIdentity' should be supplied.
        // Once a call to this function returns,
        // each entry in the 'topicList' will have been updated
        // with a new topic creation status.
        //
        // Before createTopics() returns one or more RESOLUTION_STATUS
        // events, zero or more SERVICE_STATUS events and one or more
        // TOPIC_STATUS events are generated.
        // If this is an asynchronous ProviderSession then these
        // Events may be processed by the registered EventHandler
        // before createTopics() has returned.

    void createTopicsAsync(const TopicList& topicList,
                           ResolveMode resolveMode=DONT_REGISTER_SERVICES,
                           const Identity& providerIdentity=Identity());
        // Creates the topics in the specified 'topicList' and
        // updates the 'topicList' with the results of the
        // creation process. If service needs to be registered,
        // 'providerIdentity' should be supplied.
        //
        // One or more RESOLUTION_STATUS events, zero or more
        // SERVICE_STATUS events and one or more TOPIC_STATUS
        // events are generated.
        // If this is an asynchronous ProviderSession then these
        // Events may be processed by the registered EventHandler
        // before createTopics() has returned.

    // ACCESSORS

    blpapi_ProviderSession_t* handle() const;
        // Return the handle to this provider session
};

//=============================================================================
//                           INLINE FUNCTION DEFINITIONS
//=============================================================================

                            // ---------------------
                            // class ProviderSession
                            // ---------------------

inline
ProviderSession::ProviderSession(const SessionOptions&  parameters,
                                 ProviderEventHandler  *handler,
                                 EventDispatcher       *dispatcher)
    : d_eventHandler_p(handler)
{
    d_handle_p = blpapi_ProviderSession_create(parameters.handle(),
        handler ? (blpapi_ProviderEventHandler_t)providerEventHandlerProxy : 0,
        dispatcher ? dispatcher->impl() : 0,
        this);
    initAbstractSessionHandle(
        blpapi_ProviderSession_getAbstractSession(d_handle_p));
}

inline
ProviderSession::ProviderSession(blpapi_ProviderSession_t *handle)
    : d_handle_p(handle)
{
    initAbstractSessionHandle(
        blpapi_ProviderSession_getAbstractSession(d_handle_p));
}

inline
ProviderSession::~ProviderSession()
{
    blpapi_ProviderSession_destroy(d_handle_p);
}

inline
bool ProviderSession::start()
{
    return blpapi_ProviderSession_start(d_handle_p) ? false : true;
}

inline
bool ProviderSession::startAsync()
{
    return blpapi_ProviderSession_startAsync(d_handle_p) ? false : true;
}

inline
void  ProviderSession::stop()
{
    blpapi_ProviderSession_stop(d_handle_p);
}

inline
 void  ProviderSession::stopAsync()
{
    blpapi_ProviderSession_stopAsync(d_handle_p);
}

inline
Event ProviderSession::nextEvent(int timeout)
{
    blpapi_Event_t *event;
    ExceptionUtil::throwOnError(
        blpapi_ProviderSession_nextEvent(d_handle_p, &event, timeout));
    return Event(event);
}

inline
int ProviderSession::tryNextEvent(Event *event)
{
    blpapi_Event_t *impl;
    int ret = blpapi_ProviderSession_tryNextEvent(
                            d_handle_p, &impl);
    if(0 == ret) {
        *event = Event(impl);
    }
    return ret;
}

inline
bool ProviderSession::registerService(
                        const char                        *uri,
                        const Identity&                    identity,
                        const ServiceRegistrationOptions&  registrationOptions)
{
    return blpapi_ProviderSession_registerService(
        d_handle_p, uri, identity.handle(), registrationOptions.handle()) ?
            false : true;
}

inline
CorrelationId ProviderSession::registerServiceAsync(
                        const char                        *uri,
                        const Identity&                    identity,
                        const CorrelationId&               correlationId,
                        const ServiceRegistrationOptions&  registrationOptions)
{
    blpapi_CorrelationId_t retv = correlationId.impl();
    ExceptionUtil::throwOnError(
        blpapi_ProviderSession_registerServiceAsync(
            d_handle_p, uri, identity.handle(),
            &retv, registrationOptions.handle())
        );

    return retv;
}

inline
void ProviderSession::resolve(ResolutionList* resolutionList,
                              ResolveMode resolveMode,
                              Identity identity)
{
    ExceptionUtil::throwOnError(blpapi_ProviderSession_resolve(
                                    d_handle_p,
                                    resolutionList->impl(),
                                    resolveMode,
                                    identity.handle()));
    return;
}

inline
void ProviderSession::resolveAsync(const ResolutionList& resolutionList,
                                   ResolveMode resolveMode,
                                   const Identity& identity)
{
    ExceptionUtil::throwOnError(blpapi_ProviderSession_resolveAsync(
                                    d_handle_p,
                                    resolutionList.impl(),
                                    resolveMode,
                                    identity.handle()));
    return;
}

inline
Topic ProviderSession::createTopic(const Message& message)
{
    blpapi_Topic_t* topic;
    ExceptionUtil::throwOnError(
        blpapi_ProviderSession_createTopic(
        d_handle_p, message.impl(), &topic));

    return Topic(topic);
}

inline
Topic ProviderSession::getTopic(const Message& message)
{
    blpapi_Topic_t* topic;
    ExceptionUtil::throwOnError(
        blpapi_ProviderSession_getTopic(
        d_handle_p, message.impl(), &topic));

    return Topic(topic);
}

inline
Topic ProviderSession::createServiceStatusTopic(const Service& service)
{
    blpapi_Topic_t* topic;
    ExceptionUtil::throwOnError(
        blpapi_ProviderSession_createServiceStatusTopic(
        d_handle_p, service.handle(), &topic));

    return Topic(topic);
}

inline
void ProviderSession::createTopics(TopicList* topicList,
                                   ResolveMode resolveMode,
                                   Identity identity)
{
    ExceptionUtil::throwOnError(blpapi_ProviderSession_createTopics(
                                    d_handle_p,
                                    topicList->impl(),
                                    resolveMode,
                                    identity.handle()));
    return;
}

inline
void ProviderSession::createTopicsAsync(const TopicList& topicList,
                                        ResolveMode resolveMode,
                                        const Identity& identity)
{
    ExceptionUtil::throwOnError(blpapi_ProviderSession_createTopicsAsync(
                                    d_handle_p,
                                    topicList.impl(),
                                    resolveMode,
                                    identity.handle()));
    return;
}

inline
void ProviderSession::publish(const Event& event)
{
    ExceptionUtil::throwOnError(
        blpapi_ProviderSession_publish(d_handle_p, event.impl()));
    return;
}

inline
void ProviderSession::sendResponse(
    const Event& event,
    bool isPartialResponse)
{
    ExceptionUtil::throwOnError(
        blpapi_ProviderSession_sendResponse(
            d_handle_p, event.impl(), isPartialResponse));
    return;
}

inline
blpapi_ProviderSession_t* ProviderSession::handle() const
{
    return d_handle_p;
}

inline
void ProviderSession::dispatchEvent(const Event& event)
{
    d_eventHandler_p->processEvent(event, this);
}
                      // --------------------------------
                      // class ServiceRegistrationOptions
                      // --------------------------------

inline
ServiceRegistrationOptions::ServiceRegistrationOptions()
{
    d_handle_p = blpapi_ServiceRegistrationOptions_create();
}

inline
ServiceRegistrationOptions::ServiceRegistrationOptions(
                                    const ServiceRegistrationOptions& original)
{
    d_handle_p = blpapi_ServiceRegistrationOptions_duplicate(
        original.handle());
}

inline
ServiceRegistrationOptions::~ServiceRegistrationOptions()
{
    blpapi_ServiceRegistrationOptions_destroy(d_handle_p);
}

inline
ServiceRegistrationOptions&  ServiceRegistrationOptions::operator=(
                                         const ServiceRegistrationOptions& rhs)
{
    blpapi_ServiceRegistrationOptions_copy(this->handle(), rhs.handle());
    return *this;
}

inline
void ServiceRegistrationOptions::setGroupId(const char* groupId,
                                            unsigned int groupIdLength)
{
    blpapi_ServiceRegistrationOptions_setGroupId(d_handle_p, groupId,
                                                 groupIdLength);
}
inline
int ServiceRegistrationOptions::setServicePriority(int priority)
{
    return blpapi_ServiceRegistrationOptions_setServicePriority(
            d_handle_p, priority);
}

inline
int ServiceRegistrationOptions::getGroupId(
    const char* groupIdBuffer, int *groupIdLength) const
{
    return blpapi_ServiceRegistrationOptions_getGroupId(
                d_handle_p, groupIdBuffer, groupIdLength);
}

inline
int ServiceRegistrationOptions::getServicePriority() const
{
    return blpapi_ServiceRegistrationOptions_getServicePriority(d_handle_p);
}

inline
blpapi_ServiceRegistrationOptions_t* ServiceRegistrationOptions::handle() const
{
    return d_handle_p;
}
                            // --------------------------
                            // class ProviderEventHandler
                            // --------------------------

static void providerEventHandlerProxy(blpapi_Event_t           *event,
                                      blpapi_ProviderSession_t *session,
                                      void                     *userData)
{
    reinterpret_cast<ProviderSession*>(userData)->dispatchEvent(Event(event));
}

}  // close namespace blpapi
}  // close namespace BloombergLP


#endif // #ifdef __cplusplus
#endif // #ifndef INCLUDED_BLPAPI_PROVIDERSESSION
