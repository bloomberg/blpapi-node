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
// blpapi_service.h                                                   -*-C++-*-
#ifndef INCLUDED_BLPAPI_SERVICE
#define INCLUDED_BLPAPI_SERVICE

//@PURPOSE: A service which provides access to API data (provide or consume).
//
//@CLASSES:
//   blpapi::Service: The service which provides access to API data.
// blpapi::Operation: An operation which can be performed by the Service.
//
//@DESCRIPTION: This file defines a 'Service'.
// All API data is associated with a 'Service'. A service object is obtained
// from a Session and contains zero or more 'Operations'. A service can be a
// provider service (can generate API data) or a consumer service.

#ifndef INCLUDED_BLPAPI_DEFS
#include <blpapi_defs.h>
#endif

#ifndef INCLUDED_BLPAPI_TYPES
#include <blpapi_types.h>
#endif

#ifndef INCLUDED_BLPAPI_SCHEMA
#include <blpapi_schema.h>
#endif

#ifndef INCLUDED_BLPAPI_REQUEST
#include <blpapi_request.h>
#endif

#ifndef INCLUDED_BLPAPI_CORRELATIONID
#include <blpapi_correlationid.h>
#endif

#ifndef INCLUDED_BLPAPI_STREAMPROXY
#include <blpapi_streamproxy.h>
#endif

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

BLPAPI_EXPORT
const char* blpapi_Operation_name(
        blpapi_Operation_t *service);

BLPAPI_EXPORT
const char* blpapi_Operation_description(
        blpapi_Operation_t *service);

BLPAPI_EXPORT
int blpapi_Operation_requestDefinition(
        blpapi_Operation_t *service,
        blpapi_SchemaElementDefinition_t **requestDefinition);

BLPAPI_EXPORT
int blpapi_Operation_numResponseDefinitions(
        blpapi_Operation_t *service);

BLPAPI_EXPORT
int blpapi_Operation_responseDefinition(
        blpapi_Operation_t *service,
        blpapi_SchemaElementDefinition_t **responseDefinition,
        size_t index);

BLPAPI_EXPORT
const char* blpapi_Service_name(blpapi_Service_t *service);

BLPAPI_EXPORT
const char* blpapi_Service_description(blpapi_Service_t *service);

BLPAPI_EXPORT
int blpapi_Service_numOperations(blpapi_Service_t *service);

BLPAPI_EXPORT
int blpapi_Service_numEventDefinitions(blpapi_Service_t *service);

BLPAPI_EXPORT
int blpapi_Service_addRef(blpapi_Service_t *service);

BLPAPI_EXPORT
void blpapi_Service_release(blpapi_Service_t *service);

BLPAPI_EXPORT
const char* blpapi_Service_authorizationServiceName(
        blpapi_Service_t *service);

BLPAPI_EXPORT
int blpapi_Service_getOperation(
        blpapi_Service_t *service,
        blpapi_Operation_t **operation,
        const char* nameString,
        const blpapi_Name_t *name);

BLPAPI_EXPORT
int blpapi_Service_getOperationAt(
        blpapi_Service_t *service,
        blpapi_Operation_t **operation,
        size_t index);

BLPAPI_EXPORT
int blpapi_Service_getEventDefinition(
        blpapi_Service_t *service,
        blpapi_SchemaElementDefinition_t **result,
        const char* nameString,
        const blpapi_Name_t *name);

BLPAPI_EXPORT
int blpapi_Service_getEventDefinitionAt(
        blpapi_Service_t *service,
        blpapi_SchemaElementDefinition_t **result,
        size_t index);

BLPAPI_EXPORT
int blpapi_Service_createRequest(
        blpapi_Service_t* service,
        blpapi_Request_t** request,
        const char *operation);

BLPAPI_EXPORT
int blpapi_Service_createAuthorizationRequest(
        blpapi_Service_t* service,
        blpapi_Request_t** request,
        const char *operation);

BLPAPI_EXPORT
int blpapi_Service_createPublishEvent(
    blpapi_Service_t* service,
    blpapi_Event_t** event);

BLPAPI_EXPORT
int blpapi_Service_createAdminEvent(
    blpapi_Service_t* service,
    blpapi_Event_t** event);

BLPAPI_EXPORT
int blpapi_Service_createResponseEvent(
    blpapi_Service_t* service,
    const blpapi_CorrelationId_t* correlationId,
    blpapi_Event_t** event);

BLPAPI_EXPORT int blpapi_Service_print(
        const blpapi_Service_t* service,
        blpapi_StreamWriter_t streamWriter,
        void *stream,
        int level,
        int spacesPerLevel);

#ifdef __cplusplus
}

#ifndef INCLUDED_BLPAPI_EXCEPTION
#include <blpapi_exception.h>
#endif

namespace BloombergLP {
namespace blpapi {
                         // ===============
                         // class Operation
                         // ===============

class Operation {
    // Defines an operation which can be performed by a Service.
    //
    // Operation objects are obtained from a Service object. They
    // provide read-only access to the schema of the Operations
    // Request and the schema of the possible response.

    blpapi_Operation_t *d_handle;

  public:
    Operation(blpapi_Operation_t *handle);

    ~Operation();
        // Destroy this Operation object.

    // ACCESSORS

    const char* name() const;
        // Returns a pointer to a null-terminated, read-only string
        // which contains the name of this operation. The pointer
        // remains valid until this Operation is destroyed.

    const char* description() const;
        // Returns a pointer to a null-terminated, read-only string
        // which contains a human readable description of this
        // Operation. The pointer returned remains valid until this
        // Operation is destroyed.

    SchemaElementDefinition requestDefinition() const;
        // Returns a read-only SchemaElementDefinition which defines
        // the schema for this Operation.

    int numResponseDefinitions() const;
        // Returns the number of the response types that can be
        // returned by this Operation.

    SchemaElementDefinition responseDefinition(size_t index) const;
        // Returns a read-only SchemaElementDefinition which defines
        // the schema for the response that this Operation delivers.

    blpapi_Operation_t* handle() const;

    bool isValid() const;
        // TBD - no default ctor so no need for this?
};
                         // =============
                         // class Service
                         // =============

class Service {
    // Defines a service which provides access to API data.
    //
    // A Service object is obtained from a Session and contains the
    // Operations (each of which contains its own schema) and the
    // schema for Events which this Service may produce. A Service
    // object is also used to create Request objects used with a
    // Session to issue requests.
    //
    // All API data is associated with a service. Before accessing API
    // data using either request-reply or subscription, the appropriate
    // Service must be opened and, if necessary, authorized.
    //
    // Provider services are created to generate API data and must be
    // registered before use.
    //
    // The Service object is a handle to the underlying data which is
    // owned by the Session. Once a Service has been successfully
    // opened in a Session it remains accessible until the Session is
    // terminated.

    blpapi_Service_t *d_handle;

    void addRef();
    void release();

  public:
    Service();
        // Create an uninitialized Service. The only valid operations on
        // an uninitialized Service are assignment, isValid() and
        // destruction.

    Service(blpapi_Service_t *handle);
        // Assume ownership of the raw handle

    Service(const Service& original);
        // Copy constructor.

    ~Service();
        // Destructor. Destroying a Service object obtained from a
        // Session does not close the underlying service.

    // MANIPULATORS

    Service& operator=(const Service& rhs);
        // Assignment operator.

    // ACCESSORS

    Request createRequest(const char* operation) const;
        // Returns a empty Request object for the specified
        // 'operation'. If 'operation' does not identify a valid
        // operation in the Service then an exception is thrown.
        //
        // An application must populate the Request before issuing it
        // using Session::sendRequest().

    Request createAuthorizationRequest(
                                   const char* authorizationOperation=0) const;
        // Returns an empty Request object for the specified
        // 'authorizationOperation'. If the 'authorizationOperation'
        // does not identify a valid operation for this Service then
        // an exception is thrown.
        //
        // An application must populate the Request before issuing it
        // using Session::sendAuthorizationRequest().

    blpapi_Event_t* createPublishEvent() const;
        // Create an Event suitable for publishing to this Service.
        // Use an EventFormatter to add Messages to the Event and set fields.

    blpapi_Event_t* createAdminEvent() const;
        // Create an Admin Event suitable for publishing to this Service.
        // Use an EventFormatter to add Messages to the Event and set fields.

    blpapi_Event_t* createResponseEvent(
                                     const CorrelationId& correlationId) const;
        // Create a response Event to answer the request.
        // Use an EventFormatter to add a Message to the Event and set fields.

    const char* name() const;
        // Returns a pointer to a null-terminated, read-only string
        // which contains the name of this Service. The pointer
        // remains valid until this Service object is destroyed.

    const char* description() const;
        // Returns a pointer to a null-terminated, read-only string
        // which contains a human-readable description of this
        // Service. The pointer remains valid until this Service
        // object is destroyed.

    size_t numOperations() const;
        // Returns the number of Operations defined by this Service.

    bool hasOperation(const char* name) const;
        // Returns true if the specified 'name' identifies a valid
        // Operation in this Service. Otherwise returns false.

    bool hasOperation(const Name& name) const;
        // Returns true if the specified 'name' identifies a valid
        // Operation in this Service. Otherwise returns false.

    Operation getOperation(size_t index) const;
        // Returns the specified 'index'th Operation in this
        // Service. If 'index'>=numOperations() then an exception is
        // thrown.

    Operation getOperation(const char* name) const;
        // Return the definition of the Operation identified by the
        // specified 'name'. If this Service does not define an
        // operation 'name' an exception is thrown.

    Operation getOperation(const Name& name) const;
        // Return the definition of the Operation having the specified
        // 'name'. Throw exception if no such Operation exists in this
        // service.

    int numEventDefinitions() const;
        // Returns the number of unsolicited events defined by this
        // Service.

    bool hasEventDefinition(const char* name) const;
        // Returns true if the specified 'name' identifies a valid
        // event in this Service.

    bool hasEventDefinition(const Name& name) const;
        // Returns true if the specified 'name' identifies a valid
        // event in this Service.

    SchemaElementDefinition getEventDefinition(size_t index) const;
        // Returns the SchemaElementDefinition of the specified
        // 'index'th unsolicited event defined by this service. If
        // 'index'>=numEventDefinitions() an exception is thrown.

    SchemaElementDefinition getEventDefinition(const char* name) const;
        // Return the SchemaElementDefinition of the unsolicited event
        // defined by this Service identified by the specified
        // 'name'. If this Service does not define an unsolicited
        // event 'name' an exception is thrown.

    SchemaElementDefinition getEventDefinition(const Name& name) const;
        // Return the definition of the unsolicited message having the
        // specified 'name' defined by this service.  Throw exception of the no
        // unsolicited message having the specified 'name' is defined by this
        // service.

    const char* authorizationServiceName() const;
        // Returns the name of the Service which must be used in order
        // to authorize access to restricted operations on this
        // Service. If no authorization is required to access
        // operations on this service an empty string is
        // returned. Authorization services never require
        // authorization to use.

    bool isValid() const;
        // Returns true if this Service is valid. That is, it was
        // returned from a Session.

    blpapi_Service_t* handle() const;

    std::ostream& print(std::ostream& stream,
                        int level=0,
                        int spacesPerLevel=4) const;
        // Format this Service schema to the specified output 'stream' at
        // (absolute value specified for) the optionally specified indentation
        // 'level' and return a reference to 'stream'. If 'level' is
        // specified, optionally specify 'spacesPerLevel', the number
        // of spaces per indentation level for this and all of its
        // nested objects. If 'level' is negative, suppress indentation
        // of the first line. If 'spacesPerLevel' is negative, format
        // the entire output on one line, suppressing all but the
        // initial indentation (as governed by 'level').
};

// FREE OPERATORS
std::ostream& operator<<(std::ostream& stream, const Service& service);
    // Write the value of the specified 'service' object to the specified
    // output 'stream' in a single-line format, and return a reference to
    // 'stream'.  If 'stream' is not valid on entry, this operation has no
    // effect.  Note that this human-readable format is not fully specified,
    // can change without notice, and is logically equivalent to:
    //..
    //  print(stream, 0, -1);
    //..

// ============================================================================
//                      INLINE FUNCTION DEFINITIONS
// ============================================================================

                            // ---------------
                            // class Operation
                            // ---------------
inline
Operation::Operation(blpapi_Operation_t *newHandle)
: d_handle(newHandle)
{
}

inline
Operation::~Operation()
{
}

inline
const char* Operation::name() const
{
    return blpapi_Operation_name(d_handle);
}

inline
const char* Operation::description() const
{
    return blpapi_Operation_description(d_handle);
}

inline
SchemaElementDefinition Operation::requestDefinition() const
{
    blpapi_SchemaElementDefinition_t *definition;

    ExceptionUtil::throwOnError(
            blpapi_Operation_requestDefinition(d_handle, &definition));
    return SchemaElementDefinition(definition);
}

inline
int Operation::numResponseDefinitions() const
{
    return blpapi_Operation_numResponseDefinitions(d_handle);
}

inline
SchemaElementDefinition Operation::responseDefinition(size_t index) const
{
    blpapi_SchemaElementDefinition_t *definition;

    ExceptionUtil::throwOnError(
            blpapi_Operation_responseDefinition(d_handle, &definition, index));
    return SchemaElementDefinition(definition);
}

inline
blpapi_Operation_t* Operation::handle() const
{
    return d_handle;
}

inline
bool Operation::isValid() const
{
    return d_handle != 0;
}
                            // -------------
                            // class Service
                            // -------------

inline
Service::Service()
: d_handle(0)
{
}

inline
Service::Service(const Service& original)
    : d_handle(original.d_handle)
{
    addRef();
}

inline
Service::Service(blpapi_Service_t *newHandle)
    : d_handle(newHandle)
{
    addRef();
}

inline
Service::~Service()
{
    release();
}

inline
Service& Service::operator=(const Service& rhs)
{
    if (&rhs != this) {
        release();
        d_handle = rhs.d_handle;
        addRef();
    }
    return *this;
}

inline
void Service::addRef()
{
    if (d_handle) {
        blpapi_Service_addRef(d_handle);
    }
}

inline
void Service::release()
{
    if (d_handle) {
        blpapi_Service_release(d_handle);
    }
}

inline
Request Service::createRequest(const char* operation) const
{
    blpapi_Request_t *request;
    ExceptionUtil::throwOnError(
            blpapi_Service_createRequest(
                d_handle,
                &request,
                operation)
            );
    return Request(request);
}

inline
Request Service::createAuthorizationRequest(
        const char* authorizationOperation) const
{
    blpapi_Request_t *request;
    ExceptionUtil::throwOnError(
            blpapi_Service_createAuthorizationRequest(
                d_handle,
                &request,
                authorizationOperation)
            );
    return Request(request);
}

inline
blpapi_Event_t* Service::createPublishEvent() const
{
    blpapi_Event_t* event;
    ExceptionUtil::throwOnError(
        blpapi_Service_createPublishEvent(
            d_handle,
            &event));
    return event;
}

inline
blpapi_Event_t* Service::createAdminEvent() const
{
    blpapi_Event_t* event;
    ExceptionUtil::throwOnError(
        blpapi_Service_createAdminEvent(
            d_handle,
            &event));
    return event;
}

inline
blpapi_Event_t* Service::createResponseEvent(
    const CorrelationId& correlationId) const
{
    blpapi_Event_t* event;
    ExceptionUtil::throwOnError(
        blpapi_Service_createResponseEvent(
            d_handle,
            &correlationId.impl(),
            &event));
    return event;
}

inline
blpapi_Service_t* Service::handle() const
{
    return d_handle;
}

inline
bool Service::isValid() const
{
    return (d_handle != 0);
}

inline
const char* Service::name() const
{
    return blpapi_Service_name(d_handle);
}


inline
const char* Service::description() const
{
    return blpapi_Service_description(d_handle);
}


inline
size_t Service::numOperations() const
{
    return blpapi_Service_numOperations(d_handle);
}

inline
bool Service::hasOperation(const char* operationName) const
{
    blpapi_Operation_t *operation;
    return
      blpapi_Service_getOperation(d_handle, &operation, operationName, 0) == 0;
}

inline
bool Service::hasOperation(const Name& operationName) const
{
    blpapi_Operation_t *operation;
    return blpapi_Service_getOperation(d_handle,
                                       &operation,
                                       0,
                                       operationName.impl()) == 0;
}

inline
Operation Service::getOperation(size_t index) const
{
    blpapi_Operation_t *operation;
    ExceptionUtil::throwOnError(
            blpapi_Service_getOperationAt(d_handle, &operation, index));
    return operation;
}

inline
Operation Service::getOperation(const char* operationName) const
{
    blpapi_Operation_t *operation;
    ExceptionUtil::throwOnError(
          blpapi_Service_getOperation(d_handle, &operation, operationName, 0));
    return operation;
}

inline
Operation Service::getOperation(const Name& operationName) const
{
    blpapi_Operation_t *operation;
    ExceptionUtil::throwOnError(
        blpapi_Service_getOperation(d_handle,
                                    &operation,
                                    0,
                                    operationName.impl()));
    return operation;
}

inline
int Service::numEventDefinitions() const
{
    return blpapi_Service_numEventDefinitions(d_handle);
}


inline
bool Service::hasEventDefinition(const char* definitionName) const
{
    blpapi_SchemaElementDefinition_t *eventDefinition;

    return blpapi_Service_getEventDefinition(
            d_handle, &eventDefinition, definitionName, 0) == 0 ? true : false;
}

inline
bool Service::hasEventDefinition(const Name& definitionName) const
{
    blpapi_SchemaElementDefinition_t *eventDefinition;

    return blpapi_Service_getEventDefinition(d_handle,
                                             &eventDefinition,
                                             0,
                                             definitionName.impl()) == 0
           ? true : false;
}


inline
SchemaElementDefinition Service::getEventDefinition(size_t index) const
{
    blpapi_SchemaElementDefinition_t *eventDefinition;

    ExceptionUtil::throwOnError(
            blpapi_Service_getEventDefinitionAt(
                d_handle, &eventDefinition, index));
    return SchemaElementDefinition(eventDefinition);
}


inline
SchemaElementDefinition Service::getEventDefinition(
                                              const char* definitionName) const
{
    blpapi_SchemaElementDefinition_t *eventDefinition;
    ExceptionUtil::throwOnError(
        blpapi_Service_getEventDefinition(
            d_handle,
            &eventDefinition,
            definitionName,
            0));
    return SchemaElementDefinition(eventDefinition);
}

inline
SchemaElementDefinition Service::getEventDefinition(
                                              const Name& definitionName) const
{
    blpapi_SchemaElementDefinition_t *eventDefinition;
    ExceptionUtil::throwOnError(
        blpapi_Service_getEventDefinition(
            d_handle,
            &eventDefinition,
            0,
            definitionName.impl()));
    return SchemaElementDefinition(eventDefinition);
}

inline
const char* Service::authorizationServiceName() const
{
    return blpapi_Service_authorizationServiceName(d_handle);
}

inline
std::ostream& Service::print(
        std::ostream& stream,
        int level,
        int spacesPerLevel) const
{
    blpapi_Service_print(d_handle,
                         StreamProxyOstream::writeToStream,
                         &stream,
                         level,
                         spacesPerLevel);
    return stream;
}

inline
std::ostream& operator<<(std::ostream& stream, const Service& service)
{
    return service.print(stream, 0, -1);
}

}  // close namespace blpapi
}  // close namespace BloombergLP

#endif // #ifdef __cplusplus
#endif // #ifndef INCLUDED_BLPAPI_SERVICE
