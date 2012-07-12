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
// blpapi_topic.h                                                     -*-C++-*-
#ifndef INCLUDED_BLPAPI_TOPIC
#define INCLUDED_BLPAPI_TOPIC

//@PURPOSE: Provide representation of a Topic
//
//@CLASSES:
// blpapi::Topic: Represents a Topic
//
//@DESCRIPTION: This component provides a topic that is used for publishing
// data on.

#ifndef INCLUDED_BLPAPI_DEFS
#include <blpapi_defs.h>
#endif

#ifndef INCLUDED_BLPAPI_SERVICE
#include <blpapi_service.h>
#endif

struct blpapi_Topic;
typedef struct blpapi_Topic blpapi_Topic_t;

#ifdef __cplusplus
extern "C" {
#endif

BLPAPI_EXPORT
blpapi_Topic_t* blpapi_Topic_create(blpapi_Topic_t* from);

BLPAPI_EXPORT
void blpapi_Topic_destroy(blpapi_Topic_t* victim);

BLPAPI_EXPORT
int blpapi_Topic_compare(const blpapi_Topic_t* lhs, const blpapi_Topic_t* rhs);

BLPAPI_EXPORT
blpapi_Service_t* blpapi_Topic_service(const blpapi_Topic_t *topic);

BLPAPI_EXPORT
int blpapi_Topic_isActive(const blpapi_Topic_t *topic);

#ifdef __cplusplus
}

namespace BloombergLP {
namespace blpapi {
                         // ===========
                         // class Topic
                         // ===========

class Topic {
    // Used to identify the stream on which a message is published.
    //
    // Topic objects are obtained from createTopic() on
    // ProviderSession. They are used when adding a message to an
    // Event for publishing using appendMessage() on EventFormatter.

    blpapi_Topic_t *d_handle;

  public:

    Topic();
        // Create a Topic object. A Topic created from the default
        // constructor is not a valid topic and must be assigned to
        // from a valid topic before it can be used.

    Topic(blpapi_Topic_t* handle);
        // Create a Topic object from a handle (main constructor)

    Topic(const Topic& original);
        // Create a copy of the specified 'original' Topic.

    ~Topic();
        // Destroy this Topic object.

    // MANIPULATORS

    Topic& operator=(const Topic& rhs);

    // ACCESSORS

    bool isValid() const;
        // Returns true if this Topic is valid and can be used to
        // publish a message on.

    bool isActive() const;
        // Returns true if this topic was elected by the platform to become
        // the primary publisher.

    Service service() const;
        // Returns the service for which this topic was created.

    const blpapi_Topic_t* impl() const;

    blpapi_Topic_t* impl();
};

bool operator==(Topic const& lhs, Topic const& rhs);

bool operator!=(Topic const& lhs, Topic const& rhs);

bool operator<(Topic const& lhs, Topic const& rhs);

// ============================================================================
//                      INLINE FUNCTION DEFINITIONS
// ============================================================================

                            // -----------
                            // class Topic
                            // -----------

inline
Topic::Topic() :
    d_handle(0)
{}

inline
Topic::Topic(blpapi_Topic_t* handle) :
    d_handle(blpapi_Topic_create(handle))
{}

inline
Topic::Topic(Topic const& original) :
    d_handle(blpapi_Topic_create(original.d_handle))
{
}

inline
Topic::~Topic()
{
    blpapi_Topic_destroy(d_handle);
}

inline
Topic& Topic::operator=(Topic const& rhs)
{
    if (this != &rhs) {
        blpapi_Topic_destroy(d_handle);
        d_handle = blpapi_Topic_create(rhs.d_handle);
    }
    return *this;
}

inline
bool Topic::isValid() const
{
    return 0 != d_handle;
}

inline
bool Topic::isActive() const
{
    return blpapi_Topic_isActive(d_handle) != 0;
}

inline
Service Topic::service() const
{
    return blpapi_Topic_service(d_handle);
}

inline
const blpapi_Topic_t* Topic::impl() const
{
    return d_handle;
}

inline
blpapi_Topic_t* Topic::impl()
{
    return d_handle;
}

inline
bool operator==(Topic const& lhs, Topic const& rhs)
{
    return blpapi_Topic_compare(lhs.impl(), rhs.impl())==0;
}

inline
bool operator!=(Topic const& lhs, Topic const& rhs)
{
    return blpapi_Topic_compare(lhs.impl(), rhs.impl())!=0;
}

inline
bool operator<(Topic const& lhs, Topic const& rhs)
{
    return blpapi_Topic_compare(lhs.impl(), rhs.impl())<0;
}


}  // close namespace blpapi
}  // close namespace BloombergLP

#endif // #ifdef __cplusplus
#endif // #ifndef INCLUDED_BLPAPI_TOPIC
