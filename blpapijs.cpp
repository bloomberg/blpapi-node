#ifndef BUILDING_NODE_EXTENSION
#define BUILDING_NODE_EXTENSION
#endif

#include <v8.h>
#include <node.h>
#include <node_version.h>
#if NODE_VERSION_AT_LEAST(0, 11, 0)
#include <node_object_wrap.h>
#endif
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

#if NODE_VERSION_AT_LEAST(0, 11, 0)
# define NoRetThrowException(x) args.GetIsolate()->ThrowException(x)
# define RetThrowException(x) args.GetIsolate()->ThrowException(x); return
# define NEW_STRING(x) String::NewFromUtf8(args.GetIsolate(), x)
#else
# define NoRetThrowException(x) ThrowException(x)
# define RetThrowException(x) return ThrowException(x)
# define NEW_STRING(x) String::New(x)
#endif

#define BLPAPI_EXCEPTION_TRY try {
#define BLPAPI_EXCEPTION_CATCH                                              \
    } catch (blpapi::Exception& e) {                                        \
        NoRetThrowException(Exception::Error(                               \
            NEW_STRING(e.description().c_str())));                          \
    }
#define BLPAPI_EXCEPTION_CATCH_RETURN                                       \
    } catch (blpapi::Exception& e) {                                        \
        RetThrowException(Exception::Error(                                 \
            NEW_STRING(e.description().c_str())));                          \
    }

using namespace node;
using namespace v8;

namespace BloombergLP {
namespace blpapijs {

class Session : public ObjectWrap,
                public blpapi::EventHandler {
public:
#if NODE_VERSION_AT_LEAST(0, 11, 0)
    Session(const FunctionCallbackInfo<Value>& args,
            const std::string& serverHost, int serverPort,
            const std::string& authenticationOptions);
#else
    Session(const std::string& serverHost, int serverPort,
            const std::string& authenticationOptions);
#endif
    ~Session();

    static void Initialize(Handle<Object> target);
#if NODE_VERSION_AT_LEAST(0, 11, 0)
    static void New(const FunctionCallbackInfo<Value>& args);

    static void Start(const FunctionCallbackInfo<Value>& args);
    static void Authorize(const FunctionCallbackInfo<Value>& args);
    static void Stop(const FunctionCallbackInfo<Value>& args);
    static void Destroy(const FunctionCallbackInfo<Value>& args);
    static void OpenService(const FunctionCallbackInfo<Value>& args);
    static void Subscribe(const FunctionCallbackInfo<Value>& args);
    static void Resubscribe(const FunctionCallbackInfo<Value>& args);
    static void Unsubscribe(const FunctionCallbackInfo<Value>& args);
    static void Request(const FunctionCallbackInfo<Value>& args);
#else
    static Handle<Value> New(const Arguments& args);

    static Handle<Value> Start(const Arguments& args);
    static Handle<Value> Authorize(const Arguments& args);
    static Handle<Value> Stop(const Arguments& args);
    static Handle<Value> Destroy(const Arguments& args);
    static Handle<Value> OpenService(const Arguments& args);
    static Handle<Value> Subscribe(const Arguments& args);
    static Handle<Value> Resubscribe(const Arguments& args);
    static Handle<Value> Unsubscribe(const Arguments& args);
    static Handle<Value> Request(const Arguments& args);
#endif

private:
    Session();
    Session(const Session&);
    Session& operator=(const Session&);

#if NODE_VERSION_AT_LEAST(0, 11, 0)
    static void subscribe(const FunctionCallbackInfo<Value>& args,
                          int action);
#else
    static Handle<Value> subscribe(const Arguments& args, int action);
#endif
    static void formFields(std::string* str, Handle<Object> array);
    static void formOptions(std::string* str, Handle<Value> array);
    static Handle<Value> elementToValue(Isolate *, const blpapi::Element& e);
    static Handle<Value> elementValueToValue(Isolate *,
                                             const blpapi::Element& e,
                                             int idx = 0);

    bool processEvent(const blpapi::Event& ev, blpapi::Session* session);
#if NODE_VERSION_AT_LEAST(0, 11, 0)
    static void processEvents(uv_async_t *async);
#else
    static void processEvents(uv_async_t *async, int status);
#endif
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
    static Persistent<String> s_overrides;

    Isolate *d_isolate;
    blpapi::SessionOptions d_options;
    blpapi::Session *d_session;
    blpapi::Identity d_identity;
    Persistent<Object> d_session_ref;
    std::deque<blpapi::Event> d_que;
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
Persistent<String> Session::s_overrides;

Session::Session(
#if NODE_VERSION_AT_LEAST(0, 11, 0)
                 const FunctionCallbackInfo<Value>& args,
#endif
                 const std::string& serverHost, int serverPort,
                 const std::string& authenticationOptions)
#if NODE_VERSION_AT_LEAST(0, 11, 0)
    : d_isolate(args.GetIsolate())
#else
    : d_isolate(Isolate::GetCurrent())
#endif
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

#if NODE_VERSION_AT_LEAST(0, 7, 9)
    uv_ref(reinterpret_cast<uv_handle_t *>(&s_async));
#else
    uv_ref(uv_default_loop());
#endif
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
#if NODE_VERSION_AT_LEAST(0, 11, 0)
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    Local<FunctionTemplate> t = FunctionTemplate::New(isolate, Session::New);
#else
    HandleScope scope;
    Local<FunctionTemplate> t = FunctionTemplate::New(Session::New);
#endif

    t->InstanceTemplate()->SetInternalFieldCount(1);

    NODE_SET_PROTOTYPE_METHOD(t, "start", Start);
    NODE_SET_PROTOTYPE_METHOD(t, "authorize", Authorize);
    NODE_SET_PROTOTYPE_METHOD(t, "stop", Stop);
    NODE_SET_PROTOTYPE_METHOD(t, "destroy", Destroy);
    NODE_SET_PROTOTYPE_METHOD(t, "openService", OpenService);
    NODE_SET_PROTOTYPE_METHOD(t, "subscribe", Subscribe);
    NODE_SET_PROTOTYPE_METHOD(t, "resubscribe", Resubscribe);
    NODE_SET_PROTOTYPE_METHOD(t, "unsubscribe", Unsubscribe);
    NODE_SET_PROTOTYPE_METHOD(t, "request", Request);

#if NODE_VERSION_AT_LEAST(0, 11, 0)
    target->Set(String::NewFromUtf8(isolate, "Session",
                                    v8::String::kInternalizedString),
                t->GetFunction());
#else
    target->Set(String::NewSymbol("Session"), t->GetFunction());
#endif

    uv_async_init(uv_default_loop(), &s_async, Session::processEvents);
#if !NODE_VERSION_AT_LEAST(0, 7, 9)
    uv_unref(uv_default_loop());
#endif

#if NODE_VERSION_AT_LEAST(0, 11, 0)
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
    s_overrides.Reset(isolate, NODE_PSYMBOL("overrides"));
#undef NODE_PSYMBOL
#else
    s_emit = NODE_PSYMBOL("emit");
    s_event_type = NODE_PSYMBOL("eventType");
    s_message_type = NODE_PSYMBOL("messageType");
    s_topic_name = NODE_PSYMBOL("topicName");
    s_correlations = NODE_PSYMBOL("correlations");
    s_value = NODE_PSYMBOL("value");
    s_class_id = NODE_PSYMBOL("classId");
    s_data = NODE_PSYMBOL("data");
    s_overrides = NODE_PSYMBOL("overrides");
#endif
}

#if NODE_VERSION_AT_LEAST(0, 11, 0)
void
Session::New(const FunctionCallbackInfo<Value>& args)
#else
Handle<Value>
Session::New(const Arguments& args)
#endif
{
#if NODE_VERSION_AT_LEAST(0, 11, 0)
    EscapableHandleScope scope(args.GetIsolate());
#else
    HandleScope scope;
#endif

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

#if NODE_VERSION_AT_LEAST(0, 11, 0)
    Session *session = new Session(args, serverHost, serverPort,
                                   authenticationOptions);
    session->Wrap(args.This());
    args.GetReturnValue().Set(scope.Escape(args.This()));
#else
    Session *session = new Session(serverHost, serverPort,
                                   authenticationOptions);
    session->Wrap(args.This());
    return scope.Close(args.This());
#endif
}

#if NODE_VERSION_AT_LEAST(0, 11, 0)
void
Session::Start(const FunctionCallbackInfo<Value>& args)
#else
Handle<Value>
Session::Start(const Arguments& args)
#endif
{
#if NODE_VERSION_AT_LEAST(0, 11, 0)
    EscapableHandleScope scope(args.GetIsolate());
#else
    HandleScope scope;
#endif

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

#if NODE_VERSION_AT_LEAST(0, 11, 0)
    session->d_session_ref.Reset(args.GetIsolate(), args.This());
#else
    session->d_session_ref = Persistent<Object>::New(args.This());
#endif
    session->d_started = true;

#if NODE_VERSION_AT_LEAST(0, 11, 0)
    args.GetReturnValue().Set(scope.Escape(args.This()));
#else
    return scope.Close(args.This());
#endif
}

#if NODE_VERSION_AT_LEAST(0, 11, 0)
void
Session::Authorize(const FunctionCallbackInfo<Value>& args)
#else
Handle<Value>
Session::Authorize(const Arguments& args)
#endif
{
#if NODE_VERSION_AT_LEAST(0, 11, 0)
    EscapableHandleScope scope(args.GetIsolate());
#else
    HandleScope scope;
#endif

    if (args.Length() < 1 || !args[0]->IsString()) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Service URI string must be provided as first parameter.")));
    }
    if (args.Length() < 2 || !args[1]->IsInt32()) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Integer correlation identifier must be provided "
                "as second parameter.")));
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
    blpapi::Request authRequest = authService.createAuthorizationRequest();
    authRequest.set("token", token.c_str());

    session->d_identity = session->d_session->createIdentity();

    blpapi::CorrelationId cid(cidi);
    session->d_session->sendAuthorizationRequest(authRequest,
                                                 &session->d_identity, cid);

    BLPAPI_EXCEPTION_CATCH_RETURN

#if NODE_VERSION_AT_LEAST(0, 11, 0)
    args.GetReturnValue().Set(
        scope.Escape(Integer::New(args.GetIsolate(), cidi)));
#else
    return scope.Close(Integer::New(cidi));
#endif
}

#if NODE_VERSION_AT_LEAST(0, 11, 0)
void
Session::Stop(const FunctionCallbackInfo<Value>& args)
#else
Handle<Value>
Session::Stop(const Arguments& args)
#endif
{
#if NODE_VERSION_AT_LEAST(0, 11, 0)
    EscapableHandleScope scope(args.GetIsolate());
#else
    HandleScope scope;
#endif

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

#if NODE_VERSION_AT_LEAST(0, 11, 0)
    args.GetReturnValue().Set(scope.Escape(args.This()));
#else
    return scope.Close(args.This());
#endif
}

#if NODE_VERSION_AT_LEAST(0, 11, 0)
void
Session::Destroy(const FunctionCallbackInfo<Value>& args)
#else
Handle<Value>
Session::Destroy(const Arguments& args)
#endif
{
#if NODE_VERSION_AT_LEAST(0, 11, 0)
    EscapableHandleScope scope(args.GetIsolate());
#else
    HandleScope scope;
#endif

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

#if NODE_VERSION_AT_LEAST(0, 11, 0)
    session->d_session_ref.Reset();
#else
    session->d_session_ref.Dispose();
    session->d_session_ref.Clear();
#endif

#if NODE_VERSION_AT_LEAST(0, 7, 9)
    uv_unref(reinterpret_cast<uv_handle_t *>(&s_async));
#else
    uv_unref(uv_default_loop());
#endif

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

#if NODE_VERSION_AT_LEAST(0, 11, 0)
    args.GetReturnValue().Set(scope.Escape(args.This()));
#else
    return scope.Close(args.This());
#endif
}

#if NODE_VERSION_AT_LEAST(0, 11, 0)
void
Session::OpenService(const FunctionCallbackInfo<Value>& args)
#else
Handle<Value>
Session::OpenService(const Arguments& args)
#endif
{
#if NODE_VERSION_AT_LEAST(0, 11, 0)
    EscapableHandleScope scope(args.GetIsolate());
#else
    HandleScope scope;
#endif

    if (args.Length() < 1 || !args[0]->IsString()) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Service URI string must be provided as first parameter.")));
    }
    if (args.Length() < 2 || !args[1]->IsInt32()) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Integer correlation identifier must be provided "
                "as second parameter.")));
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

#if NODE_VERSION_AT_LEAST(0, 11, 0)
    args.GetReturnValue().Set(
        scope.Escape(Integer::New(args.GetIsolate(), cidi)));
#else
    return scope.Close(Integer::New(cidi));
#endif
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

#if NODE_VERSION_AT_LEAST(0, 11, 0)
void
Session::subscribe(const FunctionCallbackInfo<Value>& args, int action)
#else
Handle<Value>
Session::subscribe(const Arguments& args, int action)
#endif
{
#if NODE_VERSION_AT_LEAST(0, 11, 0)
    EscapableHandleScope scope(args.GetIsolate());
#else
    HandleScope scope;
#endif

    if (args.Length() < 1 || !args[0]->IsArray()) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Array of subscription information must be provided.")));
    }
    if (args.Length() >= 2 && !args[1]->IsUndefined() && !args[1]->IsString()) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Optional subscription label must be a string.")));
    }
    if (args.Length() > 2) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Function expects at most two arguments.")));
    }

    blpapi::SubscriptionList sl;

    Local<Object> o = args[0]->ToObject();
    for (std::size_t i = 0; i < Array::Cast(*(args[0]))->Length(); ++i) {
        Local<Value> v = o->Get(i);
        if (!v->IsObject()) {
            RetThrowException(Exception::Error(NEW_STRING(
                "Array elements must be objects "
                        "containing subscription information.")));
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
    if (args.Length() == 2) {
        Local<String> s = args[1]->ToString();
        String::Utf8Value labelv(s);
        if (action == 1)
            session->d_session->resubscribe(sl, *labelv, labelv.length());
        else if (action == 2)
            session->d_session->unsubscribe(sl);
        else
            session->d_session->subscribe(sl, session->d_identity, *labelv, labelv.length());
    } else {
        if (action == 1)
            session->d_session->resubscribe(sl);
        else if (action == 2)
            session->d_session->unsubscribe(sl);
        else
            session->d_session->subscribe(sl);
    }
    BLPAPI_EXCEPTION_CATCH_RETURN

#if NODE_VERSION_AT_LEAST(0, 11, 0)
    args.GetReturnValue().Set(scope.Escape(args.This()));
#else
    return scope.Close(args.This());
#endif
}

#if NODE_VERSION_AT_LEAST(0, 11, 0)
# define DEFINE_WRAPPER(name, func, i)                                      \
    void                                                                    \
    Session::name(const FunctionCallbackInfo<Value>& args)                  \
    {                                                                       \
        Session::func(args, i);                                             \
    }
#else
# define DEFINE_WRAPPER(name, func, i)                                      \
    Handle<Value>                                                           \
    Session::name(const Arguments& args)                                    \
    {                                                                       \
        return Session::func(args, i);                                      \
    }
#endif

DEFINE_WRAPPER(Subscribe, subscribe, 0)
DEFINE_WRAPPER(Resubscribe, subscribe, 1)
DEFINE_WRAPPER(Unsubscribe, subscribe, 2)

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

#if NODE_VERSION_AT_LEAST(0, 11, 0)
void
Session::Request(const FunctionCallbackInfo<Value>& args)
#else
Handle<Value>
Session::Request(const Arguments& args)
#endif
{
#if NODE_VERSION_AT_LEAST(0, 11, 0)
    EscapableHandleScope scope(args.GetIsolate());
#else
    HandleScope scope;
#endif

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
    if (args.Length() >= 5 && !args[4]->IsUndefined() && !args[4]->IsString()) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Optional request label must be a string.")));
    }
    if (args.Length() > 5) {
        RetThrowException(Exception::Error(NEW_STRING(
            "Function expects at most five arguments.")));
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

    blpapi::Request request = service.createRequest(*namev);

    // Loop over object properties, appending/setting into the request.
    Local<Object> obj = args[2]->ToObject();
    Local<Array> props = obj->GetPropertyNames();

    for (std::size_t i = 0; i < props->Length(); ++i) {
        // Process the key.
        Local<Value> keyval = props->Get(i);
        Local<String> key = keyval->ToString();
        String::Utf8Value keyv(key);

        // Process the value.
        //
        // The values present on the outer object are marshalled into the
        // blpapi::Request by setting values using 'set'.  Arrays indicate
        // values which should be marshalled using 'append'.
        Local<Value> val = obj->Get(keyval);
        if (val->IsString()) {
            Local<String> s = val->ToString();
            String::Utf8Value valv(s);
            request.set(*keyv, *valv);
        } else if (val->IsBoolean()) {
            request.set(*keyv, val->BooleanValue());
        } else if (val->IsNumber()) {
            request.set(*keyv, val->NumberValue());
        } else if (val->IsInt32()) {
            request.set(*keyv, val->Int32Value());
        } else if (val->IsUint32()) {
            request.set(*keyv,
                static_cast<blpapi::Int64>(val->Uint32Value()));
        } else if (val->IsDate()) {
            blpapi::Datetime dt;
            mkdatetime(&dt, val);
            request.set(*keyv, dt);
        } else if (val->IsArray()) {
            // Arrays are marshalled into the blpapi::Request by appending
            // value types using the key of the array in the outer object.
            Local<Object> subarray = val->ToObject();
            int jmax = Array::Cast(*val)->Length();
            for (int j = 0; j < jmax; ++j) {
                Local<Value> subval = subarray->Get(j);
                // Only strings, booleans, and numbers are marshalled.
                if (subval->IsString()) {
                    Local<String> s = subval->ToString();
                    String::Utf8Value subvalv(s);
                    request.append(*keyv, *subvalv);
                } else if (subval->IsBoolean()) {
                    request.append(*keyv, subval->BooleanValue());
                } else if (subval->IsNumber()) {
                    request.append(*keyv, subval->NumberValue());
                } else if (subval->IsInt32()) {
                    request.append(*keyv, subval->Int32Value());
                } else if (subval->IsUint32()) {
                    request.append(*keyv,
                        static_cast<blpapi::Int64>(subval->Uint32Value()));
                } else if (subval->IsDate()) {
                    blpapi::Datetime dt;
                    mkdatetime(&dt, subval);
                    request.append(*keyv, dt);
                } else {
                    RetThrowException(Exception::Error(NEW_STRING(
                        "Array contains invalid value type.")));
                }
            }
#if NODE_VERSION_AT_LEAST(0, 11, 0)
        } else if (val->IsObject() &&
                   key->Equals(
                        Local<String>::New(args.GetIsolate(), s_overrides))) {
#else
        } else if (val->IsObject() && key->Equals(s_overrides)) {
#endif
            blpapi::Element overrides = request.getElement("overrides");
            Local<Array> subkeys = val->ToObject()->GetPropertyNames();
            for (std::size_t j = 0; j < subkeys->Length(); ++j) {
                blpapi::Element override = overrides.appendElement();
                Local<String> subkey = subkeys->Get(j)->ToString();
                String::Utf8Value subkeyv(subkey);
                Local<Value> subval = val->ToObject()->Get(subkey);
                if (subval->IsString()) {
                    Local<String> s = subval->ToString();
                    String::Utf8Value subvalv(s);
                    override.setElement(*subkeyv, *subvalv);
                } else if (subval->IsBoolean()) {
                    override.setElement(*subkeyv, subval->BooleanValue());
                } else if (subval->IsNumber()) {
                    override.setElement(*subkeyv, subval->NumberValue());
                } else if (subval->IsInt32()) {
                    override.setElement(*subkeyv, subval->Int32Value());
                } else if (subval->IsUint32()) {
                    override.setElement(*subkeyv,
                        static_cast<blpapi::Int64>(subval->Uint32Value()));
                } else if (subval->IsDate()) {
                    blpapi::Datetime dt;
                    mkdatetime(&dt, subval);
                    request.append(*subkeyv, dt);
                } else {
                    RetThrowException(Exception::Error(NEW_STRING(
                        "Object 'overrides' contains invalid type.")));
                }
            }
        } else {
            RetThrowException(Exception::Error(NEW_STRING(
                "Object contains invalid value type.")));
        }
    }

    blpapi::CorrelationId cid(cidi);

    if (args.Length() == 5) {
        String::Utf8Value labelv(args[4]->ToString());
        session->d_session->sendRequest(request, session->d_identity,
                                        cid, 0, *labelv, labelv.length());
    } else {
        session->d_session->sendRequest(request, session->d_identity, cid);
    }

    BLPAPI_EXCEPTION_CATCH_RETURN

#if NODE_VERSION_AT_LEAST(0, 11, 0)
    args.GetReturnValue().Set(
        scope.Escape(Integer::New(args.GetIsolate(), cidi)));
#else
    return scope.Close(Integer::New(cidi));
#endif
}

Handle<Value>
Session::elementToValue(Isolate *isolate, const blpapi::Element& e)
{
    if (e.isComplexType()) {
        int numElements = e.numElements();
#if NODE_VERSION_AT_LEAST(0, 11, 0)
        Local<Object> o = Object::New(isolate);
#else
        Local<Object> o = Object::New();
#endif
        for (int i = 0; i < numElements; ++i) {
            blpapi::Element se = e.getElement(i);
            Handle<Value> sev;
            if (se.isComplexType() || se.isArray()) {
                sev = elementToValue(isolate, se);
            } else {
                sev = elementValueToValue(isolate, se);
            }
#if NODE_VERSION_AT_LEAST(0, 11, 15)
            o->ForceSet(String::NewFromUtf8(isolate,
                                            se.name().string(),
                                            String::kNormalString,
                                            se.name().length()),
                   sev, (PropertyAttribute)(ReadOnly | DontDelete));
#else
            o->Set(String::New(se.name().string(), se.name().length()),
                   sev, (PropertyAttribute)(ReadOnly | DontDelete));
#endif
        }
        return o;
    } else if (e.isArray()) {
        int numValues = e.numValues();
#if NODE_VERSION_AT_LEAST(0, 11, 0)
        Local<Object> o = Array::New(isolate, numValues);
#else
        Local<Object> o = Array::New(numValues);
#endif
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
#if NODE_VERSION_AT_LEAST(0, 11, 0)
        return Null(isolate);
#else
        return Null();
#endif

    switch (e.datatype()) {
        case blpapi::DataType::BOOL:
#if NODE_VERSION_AT_LEAST(0, 11, 0)
            return Boolean::New(isolate, e.getValueAsBool(idx));
#else
            return Boolean::New(e.getValueAsBool(idx));
#endif
        case blpapi::DataType::CHAR: {
            char c = e.getValueAsChar(idx);
#if NODE_VERSION_AT_LEAST(0, 11, 0)
            return String::NewFromUtf8(isolate, &c, String::kNormalString, 1);
#else
            return String::New(&c, 1);
#endif
        }
        case blpapi::DataType::BYTE:
        case blpapi::DataType::INT32:
#if NODE_VERSION_AT_LEAST(0, 11, 0)
            return Integer::New(isolate, e.getValueAsInt32(idx));
#else
            return Integer::New(e.getValueAsInt32(idx));
#endif
        case blpapi::DataType::FLOAT32:
#if NODE_VERSION_AT_LEAST(0, 11, 0)
            return Number::New(isolate, e.getValueAsFloat32(idx));
#else
            return Number::New(e.getValueAsFloat32(idx));
#endif
        case blpapi::DataType::FLOAT64:
#if NODE_VERSION_AT_LEAST(0, 11, 0)
            return Number::New(isolate, e.getValueAsFloat64(idx));
#else
            return Number::New(e.getValueAsFloat64(idx));
#endif
        case blpapi::DataType::ENUMERATION: {
            blpapi::Name n = e.getValueAsName(idx);
#if NODE_VERSION_AT_LEAST(0, 11, 0)
            return String::NewFromUtf8(isolate,
                                       n.string(),
                                       String::kNormalString,
                                       n.length());
#else
            return String::New(n.string(), n.length());
#endif
        }
        case blpapi::DataType::INT64: {
            // IEEE754 double can represent the range [-2^53, 2^53].
            static const blpapi::Int64 MAX_DOUBLE_INT = 9007199254740992LL;
            blpapi::Int64 i = e.getValueAsInt64(idx);
            if ((i >= -MAX_DOUBLE_INT) && (i <= MAX_DOUBLE_INT))
#if NODE_VERSION_AT_LEAST(0, 11, 0)
                return Number::New(isolate, static_cast<double>(i));
#else
                return Number::New(static_cast<double>(i));
#endif
            break;
        }
        case blpapi::DataType::STRING:
#if NODE_VERSION_AT_LEAST(0, 11, 0)
            return String::NewFromUtf8(isolate, e.getValueAsString(idx));
#else
            return String::New(e.getValueAsString(idx));
#endif
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
#if NODE_VERSION_AT_LEAST(0, 11, 0)
                return Date::New(isolate, ms);
#else
                return Date::New(ms);
#endif
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
#if NODE_VERSION_AT_LEAST(0, 11, 0)
                return Date::New(isolate, ms);
#else
                return Date::New(ms);
#endif
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
#if NODE_VERSION_AT_LEAST(0, 11, 0)
            return Date::New(isolate, ms);
#else
            return Date::New(ms);
#endif
        }
        case blpapi::DataType::SEQUENCE:
            return elementToValue(isolate, e.getValueAsElement(idx));
        default:
            break;
    }

#if NODE_VERSION_AT_LEAST(0, 11, 0)
    return Null(isolate);
#else
    return Null();
#endif
}

#if NODE_VERSION_AT_LEAST(0, 11, 0)
#define EVENT_TO_STRING(e) \
    case blpapi::Event::e : return String::NewFromUtf8(isolate, #e)
#else
#define EVENT_TO_STRING(e) \
    case blpapi::Event::e : return String::New(#e)
#endif

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
#if NODE_VERSION_AT_LEAST(0, 11, 0)
    return isolate->ThrowException(Exception::Error(
                String::NewFromUtf8(isolate, "Invalid event type.")));
#else
    return ThrowException(Exception::Error(
                String::New("Invalid event type.")));
#endif
}

void
Session::processMessage(Isolate *isolate,
                        blpapi::Event::EventType et,
                        const blpapi::Message& msg)
{
#if NODE_VERSION_AT_LEAST(0, 11, 0)
    HandleScope scope(isolate);
#else
    HandleScope scope;
#endif

    Handle<Value> argv[2];

    const blpapi::Name& messageType = msg.messageType();
    if ("SessionTerminated" == messageType) {
        d_stopped = true;
    }
#if NODE_VERSION_AT_LEAST(0, 11, 0)
    argv[0] = String::NewFromUtf8(isolate,
                                  messageType.string(),
                                  String::kNormalString,
                                  messageType.length());
#else
    argv[0] = String::New(messageType.string(), messageType.length());
#endif

#if NODE_VERSION_AT_LEAST(0, 11, 15)
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
#else
    Local<Object> o = Object::New();
    o->Set(s_event_type, eventTypeToString(isolate, et),
           (PropertyAttribute)(ReadOnly | DontDelete));
    o->Set(s_message_type, argv[0],
           (PropertyAttribute)(ReadOnly | DontDelete));
    o->Set(s_topic_name, NEW_STRING(msg.topicName()),
           (PropertyAttribute)(ReadOnly | DontDelete));
#endif

#if NODE_VERSION_AT_LEAST(0, 11, 0)
    Local<Array> correlations = Array::New(isolate, msg.numCorrelationIds());
#else
    Local<Array> correlations = Array::New(msg.numCorrelationIds());
#endif
    for (int i = 0, j = 0; i < msg.numCorrelationIds(); ++i) {
        blpapi::CorrelationId cid = msg.correlationId(i);
        // Only pack user-specified integers and auto-generated
        // values into the correlations array returned to the user.
        if (cid.valueType() == blpapi::CorrelationId::INT_VALUE ||
            cid.valueType() == blpapi::CorrelationId::AUTOGEN_VALUE) {
#if NODE_VERSION_AT_LEAST(0, 11, 0)
            Local<Object> cido = Object::New(isolate);
            cido->Set(Local<String>::New(isolate, s_value),
                      Integer::New(isolate, (int)cid.asInteger()));
            cido->Set(Local<String>::New(isolate, s_class_id),
                      Integer::New(isolate, cid.classId()));
#else
            Local<Object> cido = Object::New();
            cido->Set(s_value, Integer::New((int)cid.asInteger()));
            cido->Set(s_class_id, Integer::New(cid.classId()));
#endif
            correlations->Set(j++, cido);
        } else {
#if NODE_VERSION_AT_LEAST(0, 11, 0)
            correlations->Set(j++, Object::New(isolate));
#else
            correlations->Set(j++, Object::New());
#endif
        }
    }
#if NODE_VERSION_AT_LEAST(0, 11, 15)
    o->ForceSet(Local<String>::New(isolate, s_correlations),
                correlations, (PropertyAttribute)(ReadOnly | DontDelete));

    o->ForceSet(Local<String>::New(isolate, s_data),
                elementToValue(isolate, msg.asElement()));
#else
    o->Set(s_correlations, correlations,
           (PropertyAttribute)(ReadOnly | DontDelete));

    o->Set(s_data, elementToValue(isolate, msg.asElement()));
#endif

    argv[1] = o;

    this->emit(isolate, sizeof(argv) / sizeof(argv[0]), argv);
}

void
#if NODE_VERSION_AT_LEAST(0, 11, 0)
Session::processEvents(uv_async_t *async)
#else
Session::processEvents(uv_async_t *async, int status)
#endif
{
    Isolate *isolate = Isolate::GetCurrent();
#if NODE_VERSION_AT_LEAST(0, 11, 0)
    HandleScope scope(isolate);
#else
    HandleScope scope;
#endif
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
#if NODE_VERSION_AT_LEAST(0, 11, 0)
    HandleScope scope(isolate);
    Local<Function> emit =
        Local<Function>::Cast(
                handle(isolate)->Get(Local<String>::New(isolate, s_emit)));
    node::MakeCallback(isolate,
                       isolate->GetCurrentContext()->Global(),
                       emit, argc, argv);
#else
    HandleScope scope;
    Local<Function> emit = Local<Function>::Cast(handle_->Get(s_emit));
    emit->Call(handle_, argc, argv);
#endif
}

}   // close namespace blpapijs
}   // close namespace BloombergLP

void init(Handle<Object> target) {
    BloombergLP::blpapijs::Session::Initialize(target);
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
