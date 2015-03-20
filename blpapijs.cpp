#ifndef BUILDING_NODE_EXTENSION
#define BUILDING_NODE_EXTENSION
#endif

#include <v8.h>
#include <node.h>
#include <node_version.h>
#include <node_object_wrap.h>
#include <uv.h>

#include <blpapi_session.h>
#include <blpapi_eventdispatcher.h>

#include <blpapi_event.h>
#include <blpapi_message.h>
#include <blpapi_element.h>
#include <blpapi_name.h>
#include <blpapi_request.h>
#include <blpapi_subscriptionlist.h>
#include <blpapi_defs.h>

#include <deque>
#include <map>
#include <sstream>

#include <cmath>
#include <ctime>
#include <cstdlib>

#ifdef _WIN32
#include <time.h>
#endif

#ifndef uv_mutex_t
# ifdef _WIN32
#  define uv_mutex_t CRITICAL_SECTION
#  define uv_mutex_init(x) InitializeCriticalSection(x)
#  define uv_mutex_lock(x) EnterCriticalSection(x)
#  define uv_mutex_unlock(x) LeaveCriticalSection(x)
#  define uv_mutex_destroy(x) DeleteCriticalSection(x)
# else
#  define uv_mutex_t pthread_mutex_t
#  define uv_mutex_init(x) pthread_mutex_init(x, NULL)
#  define uv_mutex_lock(x) pthread_mutex_lock(x)
#  define uv_mutex_unlock(x) pthread_mutex_unlock(x)
#  define uv_mutex_destroy(x) pthread_mutex_destroy(x)
# endif
#endif

# define NoRetThrowException(x) args.GetIsolate()->ThrowException(x)
# define RetThrowException(x) args.GetIsolate()->ThrowException(x); return
# define NEW_STRING(x) String::NewFromUtf8(args.GetIsolate(), x)

#define BLPAPI_EXCEPTION_TRY try {

#define BLPAPI_EXCEPTION_NEW(type)                                          \
    Local<Object> err =                                                     \
        Exception::Error(NEW_STRING(e.description().c_str()))->ToObject();  \
    err->Set(NEW_STRING("typeName"), NEW_STRING(#type));

#define BLPAPI_EXCEPTION_THROW(prefix, type)                                \
    BLPAPI_EXCEPTION_NEW(type)                                              \
    prefix##RetThrowException(err);

#define BLPAPI_EXCEPTION_CATCH_BLOCK(prefix, type)                          \
    } catch (const blpapi::type& e) {                                       \
        BLPAPI_EXCEPTION_THROW(prefix, type)

#define BLPAPI_EXCEPTION_IMPL(prefix)                                       \
    BLPAPI_EXCEPTION_CATCH_BLOCK(prefix, DuplicateCorrelationIdException)   \
    BLPAPI_EXCEPTION_CATCH_BLOCK(prefix, InvalidStateException)             \
    BLPAPI_EXCEPTION_CATCH_BLOCK(prefix, InvalidArgumentException)          \
    BLPAPI_EXCEPTION_CATCH_BLOCK(prefix, InvalidConversionException)        \
    BLPAPI_EXCEPTION_CATCH_BLOCK(prefix, IndexOutOfRangeException)          \
    BLPAPI_EXCEPTION_CATCH_BLOCK(prefix, FieldNotFoundException)            \
    BLPAPI_EXCEPTION_CATCH_BLOCK(prefix, NotFoundException)                 \
    BLPAPI_EXCEPTION_CATCH_BLOCK(prefix, UnknownErrorException)             \
    BLPAPI_EXCEPTION_CATCH_BLOCK(prefix, UnsupportedOperationException)     \
    }


#define BLPAPI_EXCEPTION_CATCH                                              \
    BLPAPI_EXCEPTION_IMPL(No)
#define BLPAPI_EXCEPTION_CATCH_RETURN                                       \
    BLPAPI_EXCEPTION_IMPL()

using namespace node;
using namespace v8;

namespace BloombergLP {
namespace blpapijs {

namespace {

static inline void
mkdatetime(blpapi::Datetime* dt, Local<Value> val)
{
    double ms = Date::Cast(*val)->NumberValue();
    time_t sec = static_cast<time_t>(ms / 1000.0);
    int remainder = static_cast<int>(fmod(ms, 1000.0));

    struct tm tm;
#ifdef _WIN32
    gmtime_s(&tm, &sec);
#else
    gmtime_r(&sec, &tm);
#endif

    dt->setDate(tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    dt->setTime(tm.tm_hour, tm.tm_min, tm.tm_sec, remainder);
}

template <typename T>
void loadElement(blpapi::Element *elem, const T& value, bool forArray)
{
    if (forArray) {
        elem->appendValue(value);
    } else {
        elem->setValue(value);
    }
}

int loadElement(blpapi::Element *elem,
                Local<Value>     val,
                bool             forArray,
                std::string     *error)
{
    if (val->IsString()) {
        loadElement(elem, *String::Utf8Value(val), forArray);
    } else if (val->IsBoolean()) {
        loadElement(elem, val->BooleanValue(), forArray);
    } else if (val->IsNumber()) {
        loadElement(elem, val->NumberValue(), forArray);
    } else if (val->IsInt32()) {
        loadElement(elem, val->Int32Value(), forArray);
    } else if (val->IsUint32()) {
        loadElement(elem,
                    static_cast<blpapi::Int64>(val->Uint32Value()),
                    forArray);
    } else if (val->IsDate()) {
        blpapi::Datetime dt;
        mkdatetime(&dt, val);
        loadElement(elem, dt, forArray);
    } else if (val->IsArray()) {
        blpapi::Element subElem;
        if (forArray) {
            subElem = elem->appendElement();
            elem = &subElem;
        }

        Local<Object> subArray = val->ToObject();
        const int subArrayLen = Array::Cast(*val)->Length();
        for (int i = 0; i < subArrayLen; ++i) {
            if (loadElement(elem, subArray->Get(i), true, error)) {
                return 1;
            }
        }
    } else if (val->IsObject()) {
        blpapi::Element subElem;
        if (forArray) {
            subElem = elem->appendElement();
            elem = &subElem;
        }

        Local<Object> obj = val->ToObject();
        Local<Array> props = obj->GetPropertyNames();
        for (std::size_t i = 0; i < props->Length(); ++i) {
            Local<Value> key = props->Get(i);
            String::Utf8Value keyStr(key);
            blpapi::Element elemValue = elem->getElement(*keyStr);
            if (loadElement(&elemValue, obj->Get(key), false, error)) {
                return 1;
            }
        }
    } else {
        if (forArray) {
            *error = "Array contains invalid type";
        } else {
            *error = "Object contains invalid value type.";
        }
        return 1;
    }

    return 0;
}

inline
int loadRequest(blpapi::Request *request,
                Local<Value>     val,
                std::string     *error)
{
    blpapi::Element elem = request->asElement();
    return loadElement(&elem, val->ToObject(), false, error);
}

}  // close anonymous namespace

                               // ==============
                               // class Identity
                               // ==============

// The Identity object is opaque to js code: it's part of the authentication
// response and passed back in to subsequent requests, with no other operations
// allowed.
class Identity : public ObjectWrap {
  private:
    // CLASS DATA
    static Eternal<ObjectTemplate> s_objectTemplate;
    // DATA
    blpapi::Identity d_identity;

    // PRIVATE CREATORS
    Identity(const blpapi::Identity& identity);

  public:
    // CLASS METHODS
    static void Initialize(Handle<Object> target);

    static Local<Object> New(Isolate                 *isolate,
                             const blpapi::Identity&  identity);

    // ACCESSORS
    const blpapi::Identity* getIdentity() const;
};

                               // --------------
                               // class Identity
                               // --------------

// CLASS DATA
Eternal<ObjectTemplate> Identity::s_objectTemplate;

// PRIVATE CREATORS
Identity::Identity(const blpapi::Identity& identity)
: d_identity(identity)
{
}

// CLASS METHODS
void Identity::Initialize(Handle<Object> target)
{
    Local<ObjectTemplate> objectTemplate;
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    objectTemplate = ObjectTemplate::New(isolate);
    s_objectTemplate.Set(isolate, objectTemplate);
    objectTemplate->SetInternalFieldCount(1);
}

Local<Object> Identity::New(Isolate *isolate, const blpapi::Identity& identity)
{
    Local<Object> object;
    object = s_objectTemplate.Get(isolate)->NewInstance();
    Identity *id = new Identity(identity);
    id->Wrap(object);
    return object;
}

// ACCESSORS
const blpapi::Identity* Identity::getIdentity() const
{
    return &d_identity;
}

class Session : public ObjectWrap,
                public blpapi::EventHandler {
public:
    Session(const FunctionCallbackInfo<Value>& args,
            const std::string& serverHost, int serverPort,
            const std::string& authenticationOptions);
    ~Session();

    static void Initialize(Handle<Object> target);
    static void New(const FunctionCallbackInfo<Value>& args);

    static void Start(const FunctionCallbackInfo<Value>& args);
    static void Authorize(const FunctionCallbackInfo<Value>& args);
    static void AuthorizeUser(const FunctionCallbackInfo<Value>& args);
    static void Stop(const FunctionCallbackInfo<Value>& args);
    static void Destroy(const FunctionCallbackInfo<Value>& args);
    static void OpenService(const FunctionCallbackInfo<Value>& args);
    static void Subscribe(const FunctionCallbackInfo<Value>& args);
    static void Resubscribe(const FunctionCallbackInfo<Value>& args);
    static void Unsubscribe(const FunctionCallbackInfo<Value>& args);
    static void Request(const FunctionCallbackInfo<Value>& args);

private:
    Session();
    Session(const Session&);
    Session& operator=(const Session&);

    static void subscribe(const FunctionCallbackInfo<Value>& args,
                          int action);
    static void formFields(std::string* str, Handle<Object> array);
    static void formOptions(std::string* str, Handle<Value> array);
    static Handle<Value> elementToValue(Isolate *, const blpapi::Element& e);
    static Handle<Value> elementValueToValue(Isolate *,
                                             const blpapi::Element& e,
                                             int idx = 0);

    const blpapi::Identity* getIdentity(const FunctionCallbackInfo<Value>& args,
                                        int index);

    bool processEvent(const blpapi::Event& ev, blpapi::Session* session);
    static void processEvents(uv_async_t *async);
    void processMessage(Isolate *isolate,
                        blpapi::Event::EventType et,
                        const blpapi::Message& msg);

    void emit(Isolate *isolate, int argc, Handle<Value> argv[]);

    static uv_async_t s_async;
    static Persistent<String> s_emit;
    static Persistent<String> s_event_type;
    static Persistent<String> s_message_type;
    static Persistent<String> s_topic_name;
    static Persistent<String> s_correlations;
    static Persistent<String> s_value;
    static Persistent<String> s_class_id;
    static Persistent<String> s_data;
    static Persistent<String> s_identity;

    Isolate *d_isolate;
    blpapi::SessionOptions d_options;
    blpapi::Session *d_session;
    blpapi::Identity d_identity;
    Persistent<Object> d_session_ref;
    std::deque<blpapi::Event> d_que;
    std::map<int, blpapi::Identity> d_identities;
    uv_mutex_t d_que_mutex;
    bool d_started;
    bool d_stopped;
    bool d_dispatching;
    bool d_destroy;
};

uv_async_t Session::s_async;
Persistent<String> Session::s_emit;
Persistent<String> Session::s_event_type;
Persistent<String> Session::s_message_type;
Persistent<String> Session::s_topic_name;
Persistent<String> Session::s_correlations;
Persistent<String> Session::s_value;
Persistent<String> Session::s_class_id;
Persistent<String> Session::s_data;
Persistent<String> Session::s_identity;

Session::Session(
                 const FunctionCallbackInfo<Value>& args,
                 const std::string& serverHost, int serverPort,
                 const std::string& authenticationOptions)
    : d_isolate(args.GetIsolate())
    , d_started(false)
    , d_stopped(false)
    , d_dispatching(false)
    , d_destroy(false)
{
    d_options.setServerHost(serverHost.c_str());
    d_options.setServerPort(serverPort);
    if (authenticationOptions.length())
        d_options.setAuthenticationOptions(authenticationOptions.c_str());

    BLPAPI_EXCEPTION_TRY
    d_session = new blpapi::Session(d_options, this);
    BLPAPI_EXCEPTION_CATCH

    uv_mutex_init(&d_que_mutex);

    uv_ref(reinterpret_cast<uv_handle_t *>(&s_async));
}

Session::~Session()
{
    // Ref on the event loop is released in Destroy

    uv_mutex_destroy(&d_que_mutex);

    // If the `Session` object in Javascript is collected without `stop()`
    // or `destroy()` being called, the underlying `blpapi::Session` still
    // needs to be cleaned up.
    if (d_session) {
        delete d_session;
        d_session = NULL;
    }
}

void
Session::Initialize(Handle<Object> target)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    Local<FunctionTemplate> t = FunctionTemplate::New(isolate, Session::New);

    t->InstanceTemplate()->SetInternalFieldCount(1);

    NODE_SET_PROTOTYPE_METHOD(t, "start", Start);
    NODE_SET_PROTOTYPE_METHOD(t, "authorize", Authorize);
    NODE_SET_PROTOTYPE_METHOD(t, "authorizeUser", AuthorizeUser);
    NODE_SET_PROTOTYPE_METHOD(t, "stop", Stop);
    NODE_SET_PROTOTYPE_METHOD(t, "destroy", Destroy);
    NODE_SET_PROTOTYPE_METHOD(t, "openService", OpenService);
    NODE_SET_PROTOTYPE_METHOD(t, "subscribe", Subscribe);
    NODE_SET_PROTOTYPE_METHOD(t, "resubscribe", Resubscribe);
    NODE_SET_PROTOTYPE_METHOD(t, "unsubscribe", Unsubscribe);
    NODE_SET_PROTOTYPE_METHOD(t, "request", Request);

    target->Set(String::NewFromUtf8(isolate, "Session",
                                    v8::String::kInternalizedString),
                t->GetFunction());

    uv_async_init(uv_default_loop(), &s_async, Session::processEvents);

#define NODE_PSYMBOL(x) \
    String::NewFromUtf8(isolate, x, String::kInternalizedString)
    s_emit.Reset(isolate, NODE_PSYMBOL("emit"));
    s_event_type.Reset(isolate, NODE_PSYMBOL("eventType"));
    s_message_type.Reset(isolate, NODE_PSYMBOL("messageType"));
    s_topic_name.Reset(isolate, NODE_PSYMBOL("topicName"));
    s_correlations.Reset(isolate, NODE_PSYMBOL("correlations"));
    s_value.Reset(isolate, NODE_PSYMBOL("value"));
    s_class_id.Reset(isolate, NODE_PSYMBOL("classId"));
    s_data.Reset(isolate, NODE_PSYMBOL("data"));
    s_identity.Reset(isolate, NODE_PSYMBOL("identity"));
#undef NODE_PSYMBOL
}

void
Session::New(const FunctionCallbackInfo<Value>& args)
{
    EscapableHandleScope scope(args.GetIsolate());

    std::string serverHost;
    int serverPort = 0;
    std::string authenticationOptions;

    if (args.Length() > 0 && args[0]->IsObject()) {
        Local<Object> o = args[0]->ToObject();

        // Capture the host name
        Local<Value> h = o->Get(NEW_STRING("host"));
        if (h->IsUndefined())
            h = o->Get(NEW_STRING("serverHost"));
        if (!h->IsUndefined()) {
            String::Utf8Value hv(h);
            if (hv.length())
                serverHost.assign(*hv, hv.length());
        }
        if (0 == serverHost.length()) {
            RetThrowException(Exception::Error(NEW_STRING(
                "Configuration missing 'serverHost'.")));
        }

        // Capture the port number
        Local<Value> p = o->Get(NEW_STRING("port"));
        if (p->IsUndefined())
            p = o->Get(NEW_STRING("serverPort"));
        if (p->IsInt32())
            serverPort = p->ToInt32()->Value();
        if (0 == serverPort) {
            RetThrowException(Exception::Error(NEW_STRING(
                "Configuration missing non-zero 'serverPort'.")));
        }

        // Capture optional authentication options
        Local<Value> ao = o->Get(NEW_STRING("authenticationOptions"));
        if (!ao->IsUndefined()) {
            String::Utf8Value aov(ao);
            if (aov.length())
                authenticationOptions.assign(*aov, aov.length());
        }
    } else {
        RetThrowException(Exception::Error(NEW_STRING(
            "Configuration object must be passed as parameter.")));
    }

    Session *session = new Session(args, serverHost, serverPort,
                                   authenticationOptions);
    session->Wrap(args.This());
    args.GetReturnValue().Set(scope.Escape(args.This()));
}

void
Session::Start(const FunctionCallbackInfo<Value>& args)
{
    EscapableHandleScope scope(args.GetIsolate());

    Session* session = ObjectWrap::Unwrap<Session>(args.This());

    if (!session->d_session || session->d_destroy) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Session has already been destroyed.")));
    }
    if (session->d_started) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Session has already been started.")));
    }
    if (session->d_stopped) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Stopped sessions can not be restarted.")));
    }

    BLPAPI_EXCEPTION_TRY
    session->d_session->startAsync();
    BLPAPI_EXCEPTION_CATCH_RETURN

    session->d_session_ref.Reset(args.GetIsolate(), args.This());
    session->d_started = true;

    args.GetReturnValue().Set(scope.Escape(args.This()));
}

// Set the default identity to use when a request/subscription does not
// specify the identity to use.
void
Session::Authorize(const FunctionCallbackInfo<Value>& args)
{
    EscapableHandleScope scope(args.GetIsolate());

    if (args.Length() < 1 || !args[0]->IsString()) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Service URI string must be provided as first parameter.")));
    }
    if (args.Length() < 2 || !args[1]->IsInt32()) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Integer correlation identifier must be provided as second "
            "parameter.")));
    }
    if (args.Length() > 2) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Function expects at most two arguments.")));
    }

    Local<String> s = args[0]->ToString();
    String::Utf8Value uriv(s);

    int cidi = args[1]->Int32Value();

    Session* session = ObjectWrap::Unwrap<Session>(args.This());

    if (!session->d_session || session->d_destroy) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Session has already been destroyed.")));
    }

    BLPAPI_EXCEPTION_TRY

    blpapi::EventQueue tokenEventQueue;
    blpapi::CorrelationId tokenCid(static_cast<void*>(&tokenEventQueue));
    session->d_session->generateToken(tokenCid, &tokenEventQueue);

    std::string token;
    blpapi::Event ev = tokenEventQueue.nextEvent();
    if (blpapi::Event::TOKEN_STATUS == ev.eventType() ||
        blpapi::Event::REQUEST_STATUS == ev.eventType()) {
        blpapi::MessageIterator msgIter(ev);
        while (msgIter.next()) {
            blpapi::Message msg = msgIter.message();
            if ("TokenGenerationSuccess" == msg.messageType()) {
                token = msg.getElementAsString("token");
            } else {
                std::stringstream ss;
                ss << "Failed to generate token: " << msg.getElement("reason");
                std::string s = ss.str();
                RetThrowException(Exception::Error(NEW_STRING(s.c_str())));
            }
        }
    }
    if (0 == token.length()) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Failed to get token.")));
    }

    blpapi::Service authService = session->d_session->getService(*uriv);
    blpapi::Request authRequest = authService.createAuthorizationRequest(
                                                       "AuthorizationRequest");
    authRequest.set("token", token.c_str());

    session->d_identity = session->d_session->createIdentity();

    blpapi::CorrelationId cid(cidi);
    session->d_session->sendAuthorizationRequest(authRequest,
                                                 &session->d_identity,
                                                 cid);

    BLPAPI_EXCEPTION_CATCH_RETURN

    args.GetReturnValue().Set(
        scope.Escape(Integer::New(args.GetIsolate(), cidi)));
}

// Create a new Identity object and send an authorization request for it.
// If the authorization request succeeds, the wrapped Identity object is
// in the response as data.identity.
void
Session::AuthorizeUser(const FunctionCallbackInfo<Value>& args)
{
    EscapableHandleScope scope(args.GetIsolate());
    if (args.Length() < 1 || !args[0]->IsObject()) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Object containing auth request parameters must be provided as "
            "first parameter.")));
    }
    if (args.Length() < 2 || !args[1]->IsInt32()) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Integer correlation identifier must be provided as second "
            "parameter.")));
    }
    if (args.Length() > 2) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Function expects at most two arguments.")));
    }

    int cidi = args[1]->Int32Value();

    Session *session = ObjectWrap::Unwrap<Session>(args.This());

    if (!session->d_session || session->d_destroy) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Session has already been destroyed.")));
    }

    BLPAPI_EXCEPTION_TRY

    blpapi::Service service = session->d_session->getService("//blp/apiauth");
    blpapi::Request request(service.createAuthorizationRequest(
                                                      "AuthorizationRequest"));
    std::string error;
    if (loadRequest(&request, args[0], &error)) {
        RetThrowException(Exception::Error(NEW_STRING(error.c_str())));
    }
    blpapi::CorrelationId cid(cidi);
    // We need to insert the completed Identity object into the response,
    // so we store it here.
    blpapi::Identity& identity = session->d_identities[cidi]
        = session->d_session->createIdentity();
    session->d_session->sendAuthorizationRequest(request, &identity, cid);

    BLPAPI_EXCEPTION_CATCH_RETURN

    args.GetReturnValue().Set(scope.Escape(Integer::New(args.GetIsolate(),
                                                        cidi)));
}

void
Session::Stop(const FunctionCallbackInfo<Value>& args)
{
    EscapableHandleScope scope(args.GetIsolate());

    Session* session = ObjectWrap::Unwrap<Session>(args.This());

    if (!session->d_session || session->d_destroy) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Session has already been destroyed.")));
    }
    if (!session->d_started) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Session has not been started.")));
    }
    if (session->d_stopped) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Session has already been stopped.")));
    }

    session->d_stopped = true;

    BLPAPI_EXCEPTION_TRY
    session->d_session->stopAsync();
    BLPAPI_EXCEPTION_CATCH_RETURN

    args.GetReturnValue().Set(scope.Escape(args.This()));
}

void
Session::Destroy(const FunctionCallbackInfo<Value>& args)
{
    EscapableHandleScope scope(args.GetIsolate());

    Session* session = ObjectWrap::Unwrap<Session>(args.This());

    if (!session->d_session || session->d_destroy) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Session has already been destroyed.")));
    }
    if (!session->d_started) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Session has not been started.")));
    }
    if (!session->d_stopped) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Session has not been stopped.")));
    }

    session->d_session_ref.Reset();

    uv_unref(reinterpret_cast<uv_handle_t *>(&s_async));

    // The `blpapi::Session` can not be deleted from within a dispatch
    // loop while a `MessageIterator` still exists.  Instead, indicate
    // it should be destroyed after the dispatching function exits the
    // loop.
    if (session->d_dispatching) {
        session->d_destroy = true;
    } else {
        delete session->d_session;
        session->d_session = NULL;
    }

    args.GetReturnValue().Set(scope.Escape(args.This()));
}

void
Session::OpenService(const FunctionCallbackInfo<Value>& args)
{
    EscapableHandleScope scope(args.GetIsolate());

    if (args.Length() < 1 || !args[0]->IsString()) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Service URI string must be provided as first parameter.")));
    }
    if (args.Length() < 2 || !args[1]->IsInt32()) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Integer correlation identifier must be provided as second "
            "parameter.")));
    }
    if (args.Length() > 2) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Function expects at most two arguments.")));
    }

    Local<String> s = args[0]->ToString();
    String::Utf8Value uriv(s);

    int cidi = args[1]->Int32Value();
    blpapi::CorrelationId cid(cidi);

    Session* session = ObjectWrap::Unwrap<Session>(args.This());

    if (!session->d_session || session->d_destroy) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Session has already been destroyed.")));
    }

    BLPAPI_EXCEPTION_TRY
    session->d_session->openServiceAsync(*uriv, cid);
    BLPAPI_EXCEPTION_CATCH_RETURN

    args.GetReturnValue().Set(
        scope.Escape(Integer::New(args.GetIsolate(), cidi)));
}

void
Session::formFields(std::string* str, Handle<Object> object)
{
    // Use the HandleScope of the calling function for speed.

    assert(object->IsArray());

    std::stringstream ss;

    // Format each array value into the options string "V[&V]"
    for (std::size_t i = 0; i < Array::Cast(*object)->Length(); ++i) {
        Local<String> s = object->Get(i)->ToString();
        String::Utf8Value v(s);
        if (v.length()) {
            if (i > 0)
                ss << ",";
            ss << *v;
        }
    }

    *str = ss.str();
}

void
Session::formOptions(std::string* str, Handle<Value> value)
{
    // Use the HandleScope of the calling function for speed.

    if (value->IsUndefined() || value->IsNull())
        return;

    assert(value->IsObject());

    std::stringstream ss;

    if (value->IsArray()) {
        // Format each array value into the options string "V[&V]"
        Local<Object> object = value->ToObject();
        for (std::size_t i = 0; i < Array::Cast(*object)->Length(); ++i) {
            Local<String> key = object->Get(i)->ToString();
            String::Utf8Value valv(key);
            if (valv.length()) {
                if (i > 0)
                    ss << "&";
                ss << *valv;
            }
        }
    } else {
        // Format each KV pair into the options string "K=V[&K=V]"
        Local<Object> object = value->ToObject();
        Local<Array> keys = object->GetPropertyNames();
        for (std::size_t i = 0; i < keys->Length(); ++i) {
            Local<String> key = keys->Get(i)->ToString();
            String::Utf8Value keyv(key);
            if (keyv.length()) {
                if (i > 0)
                    ss << "&";
                ss << *keyv << "=";
            }

            Local<String> val = object->Get(key)->ToString();
            String::Utf8Value valv(val);
            if (valv.length())
                ss << *valv;
        }
    }

    *str = ss.str();
}

void
Session::subscribe(const FunctionCallbackInfo<Value>& args, int action)
{
    EscapableHandleScope scope(args.GetIsolate());

    if (args.Length() < 1 || !args[0]->IsArray()) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Array of subscription information must be provided.")));
    }
    if (args.Length() >= 2 && !args[1]->IsUndefined() &&
        !args[1]->IsNull() && !args[1]->IsObject()) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Optional identity must be an object.")));
    }
    if (args.Length() >= 3 && !args[2]->IsUndefined() &&
        !args[2]->IsNull() && !args[2]->IsString()) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Optional subscription label must be a string.")));
    }
    if (args.Length() > 3) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Function expects at most three arguments.")));
    }

    blpapi::SubscriptionList sl;

    Local<Object> o = args[0]->ToObject();
    for (std::size_t i = 0; i < Array::Cast(*(args[0]))->Length(); ++i) {
        Local<Value> v = o->Get(i);
        if (!v->IsObject()) {
            RetThrowException(Exception::Error(NEW_STRING(
                "Array elements must be objects containing subscription "
                "information.")));
        }
        Local<Object> io = v->ToObject();

        // Process 'security' string
        Local<Value> iv = io->Get(NEW_STRING("security"));
        if (!iv->IsString()) {
            RetThrowException(Exception::Error(NEW_STRING(
                "Property 'security' must be a string.")));
        }
        String::Utf8Value secv(iv);
        if (0 == secv.length()) {
            RetThrowException(Exception::Error(NEW_STRING(
                "Property 'security' must be a string.")));
        }

        // Process 'fields' array
        iv = io->Get(NEW_STRING("fields"));
        if (!iv->IsArray()) {
            RetThrowException(Exception::Error(NEW_STRING(
                "Property 'fields' must be an array of strings.")));
        }
        std::string fields;
        formFields(&fields, iv->ToObject());

        // Process 'options' array
        iv = io->Get(NEW_STRING("options"));
        if (!iv->IsUndefined() && !iv->IsNull() && !iv->IsObject()) {
            RetThrowException(Exception::Error(NEW_STRING(
                "Property 'options' must be an object containing "
                "whose keys and key values will be configured as "
                "options.")));
        }
        std::string options;
        formOptions(&options, iv);

        // Process 'correlation' int or string
        iv = io->Get(NEW_STRING("correlation"));
        if (!iv->IsInt32()) {
            RetThrowException(Exception::Error(NEW_STRING(
                "Property 'correlation' must be an integer.")));
        }
        int correlation = iv->Int32Value();

        sl.add(*secv, fields.c_str(), options.c_str(),
               blpapi::CorrelationId(correlation));
    }

    Session* session = ObjectWrap::Unwrap<Session>(args.This());

    if (!session->d_session || session->d_destroy) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Session has already been destroyed.")));
    }

    BLPAPI_EXCEPTION_TRY

    const blpapi::Identity *identity = session->getIdentity(args, 1);
    if (args.Length() == 3) {
        Local<String> s = args[2]->ToString();
        String::Utf8Value labelv(s);
        if (action == 1)
            session->d_session->resubscribe(sl, *labelv, labelv.length());
        else if (action == 2)
            session->d_session->unsubscribe(sl);
        else
            session->d_session->subscribe(sl, *identity, *labelv, labelv.length());
    } else {
        if (action == 1)
            session->d_session->resubscribe(sl);
        else if (action == 2)
            session->d_session->unsubscribe(sl);
        else
            session->d_session->subscribe(sl, *identity);
    }
    BLPAPI_EXCEPTION_CATCH_RETURN

    args.GetReturnValue().Set(scope.Escape(args.This()));
}

# define DEFINE_WRAPPER(name, func, i)                                      \
    void                                                                    \
    Session::name(const FunctionCallbackInfo<Value>& args)                  \
    {                                                                       \
        Session::func(args, i);                                             \
    }

DEFINE_WRAPPER(Subscribe, subscribe, 0)
DEFINE_WRAPPER(Resubscribe, subscribe, 1)
DEFINE_WRAPPER(Unsubscribe, subscribe, 2)

void
Session::Request(const FunctionCallbackInfo<Value>& args)
{
    EscapableHandleScope scope(args.GetIsolate());

    if (args.Length() < 1 || !args[0]->IsString()) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Service URI string must be provided as first parameter.")));
    }
    if (args.Length() < 2 || !args[1]->IsString()) {
        RetThrowException(Exception::Error(NEW_STRING(
            "String request name must be provided as second parameter.")));
    }
    if (args.Length() < 3 || !args[2]->IsObject()) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Object containing request parameters must be provided "
            "as third parameter.")));
    }
    if (args.Length() < 4 || !args[3]->IsInt32()) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Integer correlation identifier must be provided "
            "as fourth parameter.")));
    }
    if (args.Length() >= 5 && !args[4]->IsUndefined() &&
        !args[4]->IsNull() && !args[4]->IsObject()) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Optional identity must be an object.")));
    }
    if (args.Length() >= 6 && !args[5]->IsUndefined() &&
        !args[5]->IsNull() && !args[5]->IsString()) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Optional request label must be a string.")));
    }
    if (args.Length() > 6) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Function expects at most six arguments.")));
    }

    int cidi = args[3]->Int32Value();

    Session* session = ObjectWrap::Unwrap<Session>(args.This());

    if (!session->d_session || session->d_destroy) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Session has already been destroyed.")));
    }

    BLPAPI_EXCEPTION_TRY

    Local<String> uri = args[0]->ToString();
    String::Utf8Value uriv(uri);

    blpapi::Service service = session->d_session->getService(*uriv);

    Local<String> name = args[1]->ToString();
    String::Utf8Value namev(name);

    blpapi::Request request(service.createRequest(*namev));
    std::string error;
    if (loadRequest(&request, args[2], &error)) {
        RetThrowException(Exception::Error(NEW_STRING(error.c_str())));
    }

    blpapi::CorrelationId cid(cidi);

    const blpapi::Identity *identity = session->getIdentity(args, 4);

    if (args.Length() == 6) {
        String::Utf8Value labelv(args[5]->ToString());
        session->d_session->sendRequest(request, *identity,
                                        cid, 0, *labelv, labelv.length());
    } else {
        session->d_session->sendRequest(request, *identity, cid);
    }

    BLPAPI_EXCEPTION_CATCH_RETURN

    args.GetReturnValue().Set(
        scope.Escape(Integer::New(args.GetIsolate(), cidi)));
}

Handle<Value>
Session::elementToValue(Isolate *isolate, const blpapi::Element& e)
{
    if (e.isComplexType()) {
        int numElements = e.numElements();
        Local<Object> o = Object::New(isolate);
        for (int i = 0; i < numElements; ++i) {
            blpapi::Element se = e.getElement(i);
            Handle<Value> sev;
            if (se.isComplexType() || se.isArray()) {
                sev = elementToValue(isolate, se);
            } else {
                sev = elementValueToValue(isolate, se);
            }
            o->ForceSet(String::NewFromUtf8(isolate,
                                            se.name().string(),
                                            String::kNormalString,
                                            se.name().length()),
                   sev, (PropertyAttribute)(ReadOnly | DontDelete));
        }
        return o;
    } else if (e.isArray()) {
        int numValues = e.numValues();
        Local<Object> o = Array::New(isolate, numValues);
        for (int i = 0; i < numValues; ++i) {
            o->Set(i, elementValueToValue(isolate, e, i));
        }
        return o;
    } else {
        return elementValueToValue(isolate, e);
    }
}

static inline struct tm*
mknow(struct tm* tm)
{
    time_t sec;
    time(&sec);
#ifdef _WIN32
    gmtime_s(tm, &sec);
    return tm;
#else
    return gmtime_r(&sec, tm);
#endif
}

static inline time_t
mkutctime(struct tm* tm)
{
    time_t ret;

#ifdef _WIN32
    char tz[128];
    std::size_t len = 0;
    getenv_s(&len, tz, sizeof(tz), "TZ");
    _putenv_s("TZ", "UTC");
    _tzset();
#else
    char* tz = getenv("TZ");
    setenv("TZ", "UTC", 1);
    tzset();
#endif

    ret = mktime(tm);

#ifdef _WIN32
    _putenv_s("TZ", len ? tz : "");
    _tzset();
#else
    if (tz)
        setenv("TZ", tz, 1);
    else
        unsetenv("TZ");
    tzset();
#endif

    return ret;
}

Handle<Value>
Session::elementValueToValue(Isolate *isolate,
                             const blpapi::Element& e, int idx)
{
    if (e.isNull())
        return Null(isolate);

    switch (e.datatype()) {
        case blpapi::DataType::BOOL:
            return Boolean::New(isolate, e.getValueAsBool(idx));
        case blpapi::DataType::CHAR: {
            char c = e.getValueAsChar(idx);
            return String::NewFromUtf8(isolate, &c, String::kNormalString, 1);
        }
        case blpapi::DataType::BYTE:
        case blpapi::DataType::INT32:
            return Integer::New(isolate, e.getValueAsInt32(idx));
        case blpapi::DataType::FLOAT32:
            return Number::New(isolate, e.getValueAsFloat32(idx));
        case blpapi::DataType::FLOAT64:
            return Number::New(isolate, e.getValueAsFloat64(idx));
        case blpapi::DataType::ENUMERATION: {
            blpapi::Name n = e.getValueAsName(idx);
            return String::NewFromUtf8(isolate,
                                       n.string(),
                                       String::kNormalString,
                                       n.length());
        }
        case blpapi::DataType::INT64: {
            // IEEE754 double can represent the range [-2^53, 2^53].
            static const blpapi::Int64 MAX_DOUBLE_INT = 9007199254740992LL;
            blpapi::Int64 i = e.getValueAsInt64(idx);
            if ((i >= -MAX_DOUBLE_INT) && (i <= MAX_DOUBLE_INT))
                return Number::New(isolate, static_cast<double>(i));
            break;
        }
        case blpapi::DataType::STRING:
            return String::NewFromUtf8(isolate, e.getValueAsString(idx));
        case blpapi::DataType::DATE: {
            blpapi::Datetime dt = e.getValueAsDatetime(idx);
            if (dt.hasParts(blpapi::DatetimeParts::DATE)) {
                struct tm date;
                date.tm_sec = 0;
                date.tm_min = 0;
                date.tm_hour = 0;
                date.tm_mday = dt.day();
                date.tm_mon = dt.month() - 1;
                date.tm_year = dt.year() - 1900;
                date.tm_wday = 0;
                date.tm_yday = 0;
                date.tm_isdst = 0;
                time_t sec = mkutctime(&date);
                double ms = sec * 1000.0L;
                return Date::New(isolate, ms);
            }
            break;
        }
        case blpapi::DataType::TIME: {
            blpapi::Datetime dt = e.getValueAsDatetime(idx);
            if (dt.hasParts(blpapi::DatetimeParts::TIME)) {
                struct tm date;
                mknow(&date);
                date.tm_sec = dt.seconds();
                date.tm_min = dt.minutes();
                date.tm_hour = dt.hours();
                date.tm_isdst = 0;
                time_t sec = mkutctime(&date);
                if (dt.hasParts(blpapi::DatetimeParts::OFFSET)) {
                    sec -= dt.offset() * 60;  // UTC offset (in minutes)
                }
                double ms = sec * 1000.0L;
                if (dt.hasParts(blpapi::DatetimeParts::FRACSECONDS))
                    ms += dt.milliSeconds();
                return Date::New(isolate, ms);
            }
            break;
        }
        case blpapi::DataType::DATETIME: {
            blpapi::Datetime dt = e.getValueAsDatetime(idx);
            struct tm date;
            // Use date if present, otherwise default to "now".
            if (dt.hasParts(blpapi::DatetimeParts::DATE)) {
                date.tm_mday = dt.day();
                date.tm_mon = dt.month() - 1;
                date.tm_year = dt.year() - 1900;
                date.tm_wday = 0;
                date.tm_yday = 0;
            } else {
                mknow(&date);
            }
            // Use time if present, otherwise default to midnight.
            if (dt.hasParts(blpapi::DatetimeParts::TIME)) {
                date.tm_sec = dt.seconds();
                date.tm_min = dt.minutes();
                date.tm_hour = dt.hours();
                date.tm_isdst = 0;
            } else {
                date.tm_sec = 0;
                date.tm_min = 0;
                date.tm_hour = 0;
                date.tm_isdst = 0;
            }
            time_t sec = mkutctime(&date);
            if (dt.hasParts(blpapi::DatetimeParts::OFFSET)) {
                sec -= dt.offset() * 60;  // UTC offset (in minutes)
            }
            double ms = sec * 1000.0L;
            if (dt.hasParts(blpapi::DatetimeParts::FRACSECONDS))
                ms += dt.milliSeconds();
            return Date::New(isolate, ms);
        }
        case blpapi::DataType::SEQUENCE:
            return elementToValue(isolate, e.getValueAsElement(idx));
        default:
            break;
    }

    return Null(isolate);
}


const blpapi::Identity*
Session::getIdentity(const FunctionCallbackInfo<Value>& args, int index)
{
    const blpapi::Identity* identity = &(this->d_identity);
    if (args.Length() > index && args[index]->IsObject()) {
        // If the identity was supplied, use it.
        Identity* id = ObjectWrap::Unwrap<Identity>(args[index]->ToObject());
        if (id != NULL) {
            identity = id->getIdentity();
        }
    }
    return identity;
}

#define EVENT_TO_STRING(e) \
    case blpapi::Event::e : return String::NewFromUtf8(isolate, #e)

static inline Handle<Value>
eventTypeToString(Isolate *isolate, blpapi::Event::EventType et)
{
    switch (et) {
        EVENT_TO_STRING(ADMIN);
        EVENT_TO_STRING(SESSION_STATUS);
        EVENT_TO_STRING(SUBSCRIPTION_STATUS);
        EVENT_TO_STRING(REQUEST_STATUS);
        EVENT_TO_STRING(RESPONSE);
        EVENT_TO_STRING(PARTIAL_RESPONSE);
        EVENT_TO_STRING(SUBSCRIPTION_DATA);
        EVENT_TO_STRING(SERVICE_STATUS);
        EVENT_TO_STRING(TIMEOUT);
        EVENT_TO_STRING(AUTHORIZATION_STATUS);
        EVENT_TO_STRING(RESOLUTION_STATUS);
        EVENT_TO_STRING(TOPIC_STATUS);
        EVENT_TO_STRING(TOKEN_STATUS);
        EVENT_TO_STRING(REQUEST);
        EVENT_TO_STRING(UNKNOWN);
    }
    return isolate->ThrowException(Exception::Error(
                String::NewFromUtf8(isolate, "Invalid event type.")));
}

void
Session::processMessage(Isolate *isolate,
                        blpapi::Event::EventType et,
                        const blpapi::Message& msg)
{
    HandleScope scope(isolate);

    Handle<Value> argv[2];

    blpapi::Name messageType = msg.messageType();
    bool isAuthSuccess = false;
    bool isAuthFailure = false;
    if ("SessionTerminated" == messageType) {
        d_stopped = true;
    }
    else if ("AuthorizationSuccess" == messageType) {
        isAuthSuccess = true;
    }
    else if ("AuthorizationFailure" == messageType) {
        isAuthFailure = true;
    }

    Local<Object> identityObj;

    if (isAuthSuccess || isAuthFailure) {
        // We convert both authorization successes and failures into a more
        // generic AuthorizationResponse to simplify handling in the js. Client
        // code can distinguish between them by checking for the identity
        // object, which is only present for successes.
        messageType = blpapi::Name("AuthorizationResponse");
        if (et == blpapi::Event::RESPONSE) {
            std::map<int, blpapi::Identity>::iterator f =
                d_identities.find(msg.correlationId(0).asInteger());
            if (f != d_identities.end()) {
                if (isAuthSuccess) {
                    identityObj = Identity::New(isolate, f->second);
                }
                d_identities.erase(f);
            }
        }
    }

    argv[0] = String::NewFromUtf8(isolate,
                                  messageType.string(),
                                  String::kNormalString,
                                  messageType.length());

    Local<Object> o = Object::New(isolate);
    o->ForceSet(Local<String>::New(isolate, s_event_type),
                eventTypeToString(isolate, et),
                (PropertyAttribute)(ReadOnly | DontDelete));
    o->ForceSet(Local<String>::New(isolate, s_message_type),
                argv[0],
                (PropertyAttribute)(ReadOnly | DontDelete));
    o->ForceSet(Local<String>::New(isolate, s_topic_name),
                String::NewFromUtf8(isolate, msg.topicName()),
                (PropertyAttribute)(ReadOnly | DontDelete));

    Local<Array> correlations = Array::New(isolate, msg.numCorrelationIds());
    for (int i = 0, j = 0; i < msg.numCorrelationIds(); ++i) {
        blpapi::CorrelationId cid = msg.correlationId(i);
        // Only pack user-specified integers and auto-generated
        // values into the correlations array returned to the user.
        if (cid.valueType() == blpapi::CorrelationId::INT_VALUE ||
            cid.valueType() == blpapi::CorrelationId::AUTOGEN_VALUE) {
            Local<Object> cido = Object::New(isolate);
            cido->Set(Local<String>::New(isolate, s_value),
                      Integer::New(isolate, (int)cid.asInteger()));
            cido->Set(Local<String>::New(isolate, s_class_id),
                      Integer::New(isolate, cid.classId()));
            correlations->Set(j++, cido);
        } else {
            correlations->Set(j++, Object::New(isolate));
        }
    }

    Local<Object> data = elementToValue(isolate, msg.asElement())->ToObject();
    o->ForceSet(Local<String>::New(isolate, s_correlations),
                correlations, (PropertyAttribute)(ReadOnly | DontDelete));

    o->ForceSet(Local<String>::New(isolate, s_data), data);

    if (!identityObj.IsEmpty()) {
        data->ForceSet(Local<String>::New(isolate, s_identity), identityObj);
    }

    argv[1] = o;

    this->emit(isolate, sizeof(argv) / sizeof(argv[0]), argv);
}

void
Session::processEvents(uv_async_t *async)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    Session *session = reinterpret_cast<Session *>(async->data);

    bool empty;
    do {
        {
            // Create a block scope to ensure the `MessageIterator` is
            // destroyed before potentially destroying the `Session`.

            // Determine if the queue is empty
            uv_mutex_lock(&session->d_que_mutex);
            empty = session->d_que.empty();
            if (empty) {
                uv_mutex_unlock(&session->d_que_mutex);
                break;
            }

            // Keep the lock and release once the head is retrieved
            const blpapi::Event& ev = session->d_que.front();
            uv_mutex_unlock(&session->d_que_mutex);

            // Iterate over contained messages without holding lock
            blpapi::MessageIterator msgIter(ev);
            while (msgIter.next()) {
                const blpapi::Message& msg = msgIter.message();
                // Indicate a message is being dispatched and break if
                // the callback winds up destroying the session.  The
                // session can not be directly destroyed in `::Destroy`
                // because the `MessageIterator` requires the session
                // to still exist.
                session->d_dispatching = true;
                session->processMessage(isolate, ev.eventType(), msg);
                session->d_dispatching = false;
                if (session->d_destroy)
                    break;
            }
        }

        // Reacquire and pop, updating empty flag to having to
        // loop and acquire the mutex for a second time.
        uv_mutex_lock(&session->d_que_mutex);
        if (session->d_destroy) {
            // Drain the queue, as `Event` release requires `Session`
            session->d_que.clear();
            empty = true;
            // Destroy the `blpapi::Session`
            delete session->d_session;
            session->d_session = NULL;
            session->d_destroy = false;
        } else {
            session->d_que.pop_front();
            empty = session->d_que.empty();
        }
        uv_mutex_unlock(&session->d_que_mutex);
    } while (!empty);
}

bool
Session::processEvent(const blpapi::Event& ev, blpapi::Session* session)
{
    uv_mutex_lock(&d_que_mutex);

    d_que.push_back(ev);

    uv_mutex_unlock(&d_que_mutex);

    s_async.data = this;
    uv_async_send(&s_async);

    return true;
}

void
Session::emit(Isolate *isolate, int argc, Handle<Value> argv[])
{
    HandleScope scope(isolate);
    Local<Function> emit =
        Local<Function>::Cast(
                handle(isolate)->Get(Local<String>::New(isolate, s_emit)));
    node::MakeCallback(isolate,
                       isolate->GetCurrentContext()->Global(),
                       emit, argc, argv);
}

}   // close namespace blpapijs
}   // close namespace BloombergLP

void init(Handle<Object> target) {
    BloombergLP::blpapijs::Session::Initialize(target);
    BloombergLP::blpapijs::Identity::Initialize(target);
}

NODE_MODULE(blpapijs, init)

// Local variables:
// c-basic-offset: 4
// tab-width: 4
// indent-tabs-mode: nil
// End:
//
// vi: set shiftwidth=4 tabstop=4 expandtab:
// :indentSize=4:tabSize=4:noTabs=true:

// ----------------------------------------------------------------------------
// Copyright (C) 2014 Bloomberg L.P.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ----------------------------- END-OF-FILE ----------------------------------
