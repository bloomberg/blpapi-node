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
// blpapi_name.h                                                      -*-C++-*-
#ifndef INCLUDED_BLPAPI_NAME
#define INCLUDED_BLPAPI_NAME

//@PURPOSE: Provide a representation of a string for efficient comparison.
//
//@CLASSES:
// blpapi::Name: Represents a string in a form for efficient string comparison
//
//@DESCRIPTION: This component implements a representation of a string which is
// efficient for string comparison.

#ifndef INCLUDED_BLPAPI_TYPES
#include <blpapi_types.h>
#endif

#ifndef INCLUDED_BLPAPI_DEFS
#include <blpapi_defs.h>
#endif

#ifndef INCLUDED_STDDEF
#include <stddef.h>
#define INCLUDED_STDDEF
#endif

#ifdef __cplusplus
extern "C" {
#endif

BLPAPI_EXPORT
blpapi_Name_t* blpapi_Name_create(
        const char* nameString);

BLPAPI_EXPORT
void blpapi_Name_destroy(
        blpapi_Name_t *name);

BLPAPI_EXPORT
blpapi_Name_t* blpapi_Name_duplicate(
        const blpapi_Name_t *src);

BLPAPI_EXPORT
int blpapi_Name_equalsStr(
        const blpapi_Name_t *name,
        const char *string);

BLPAPI_EXPORT
const char* blpapi_Name_string(
        const blpapi_Name_t *name);

BLPAPI_EXPORT
size_t blpapi_Name_length(
        const blpapi_Name_t *name);

BLPAPI_EXPORT
blpapi_Name_t* blpapi_Name_findName(
        const char* nameString);

#ifdef __cplusplus
}

#ifndef INCLUDED_ALGORITHM
#include <algorithm> // for swap
#define INCLUDED_ALGORITHM
#endif

#ifndef INCLUDED_IOSTREAM
#include <iostream>
#define INCLUDED_IOSTREAM
#endif

#ifndef INCLUDED_CSTRING
#include <cstring>   // for strcmp
#define INCLUDED_CSTRING
#endif

namespace BloombergLP {
namespace blpapi {
                         // ==========
                         // class Name
                         // ==========

class Name {
    // Name represents a string in a form which is efficient for
    // comparison.
    //
    // Name objects are used to identify and access the classes which
    // define the schema - SchemaTypeDefinition,
    // SchemaElementDefinition, SchemaConstant,
    // SchemaConstantList. They are also used to access the values in
    // Element objects and Message objects.
    //
    // The Name class is an efficient substitute for a string when
    // used as a key, providing constant time comparison and ordering
    // operations. Two Name objects constructed from strings for which
    // strcmp() would return 0 will always compare equally.
    //
    // The ordering of Name objects (as defined by the operator<
    // defined for Name) is consistent during a particular instance of
    // a single application. However, the ordering is not lexical and
    // is not consistent with the ordering of the same Name objects in
    // any other process.
    //
    // Where possible Name objects should be initialized once and then
    // reused. Creating a Name object from a char const* involves a
    // search in a container requiring multiple string comparison
    // operations.
    //
    // Note: Each Name instance refers to an entry in a global static
    // table. Name instances for identical strings will refer to the
    // same data. There is no provision for removing entries from the
    // static table so Name objects should only be used when the set
    // of input strings is bounded.
    //
    // For example, creating a Name for every possible field name and
    // type in a data model is reasonable (in fact, the API will do
    // this whenever it receives schema information). However
    // converting sequence numbers on incoming messages to strings and
    // creating a Name from each one of those strings will cause the
    // static table to grow in an unbounded manner.

    blpapi_Name_t *d_impl_p;

  public:
    // CLASS METHODS

    static Name findName(const char* nameString);
        // If a Name already exists which matches the specified
        // 'nameString' it is returned. Otherwise a Name which will
        // compare equal to a Name created using the default
        // constructor is returned. The behavior is undefined if
        // 'nameString' does not point to a NUL-terminated string.

    static bool hasName(const char* nameString);
        // Returns true if a Name has been created which matches the
        // specified 'nameString' otherwise returns false. The
        // behavior is undefined if 'nameString' does not point to a
        // NUL-terminated string.

    Name();
        // Construct an uninitialized Name. An uninitialized Name can
        // be assigned to, destroyed or tested for equality. The
        // behavior for all other operations is undefined.

    Name(blpapi_Name_t *handle);

    Name(const Name& original);
        // Copy constructor.

    explicit Name(const char* nameString);
        // Construct a Name from the specified 'nameString' as long as
        // 'nameString' points to a valid NUL-terminated string. Any
        // null-terminated string can be specified (including an empty
        // string).
        //
        // Constructing a Name from a char const* is a relatively
        // expensive operation. If a Name will be used repeatedly it
        // is preferable to create it once and re-use the object.

    ~Name();
        // Destructor.

    // MANIPULATORS

    Name& operator=(const Name& rhs);
        // Assignment operator.

    // ACCESSORS

    const char *string() const;
        // Returns a pointer to the Name as a C style, null-terminated
        // string. The pointer returned will be valid at least until
        // main() exits.

    size_t length() const;
        // Returns the length of the Names string representation
        // (excluding a terminating null). Using name.length() is more
        // efficient than using strlen(name.string()) but the result is
        // the same.

    blpapi_Name_t* impl() const;
};

// FREE OPERATORS
inline
bool operator==(const Name& lhs, const Name& rhs);
    // Return true if the specified 'lhs' and 'rhs' name objects have
    // the same value, and false otherwise.  Two 'Name' objects have
    // the same value if and only if the strings supplied when they
    // were created would return 0 when compared using strcmp().

inline
bool operator!=(const Name& lhs, const Name& rhs);
    // Equivalent to !(lhs==rhs).

inline
bool operator==(const Name& lhs, const char* rhs);
    // Return true if the specified 'lhs' and 'rhs' name objects have the
    // same value, and false otherwise.  Two 'Name' objects have the same value
    // if and only if the strings supplied when they were created would return
    // 0 when compared with strcmp().

inline
bool operator<(const Name& lhs, const Name& rhs);
    // This operator defines a sort ordering amongst Name objects. The
    // sort ordering is stable within the lifetime of a single process
    // but is not lexical and is also not guaranteed to be consistent
    // across different processes (including repeated runs of the same
    // process).

//=============================================================================
//                           INLINE FUNCTION DEFINITIONS
//=============================================================================

                            // ----------
                            // class Name
                            // ----------

inline
Name::Name(blpapi_Name_t *handle)
: d_impl_p(handle)
{
}

inline
Name::Name()
: d_impl_p(0)
{
}

inline
Name::Name(const Name& original)
: d_impl_p(blpapi_Name_duplicate(original.d_impl_p))
{
}

inline
Name::Name(const char* nameString)
{
    d_impl_p = blpapi_Name_create(nameString);
}

inline
Name::~Name()
{
    if (d_impl_p) {
        blpapi_Name_destroy(d_impl_p);
    }
}

inline
Name& Name::operator=(const Name& rhs)
{
    if (&rhs != this) {
        Name tmp(rhs);
        std::swap(tmp.d_impl_p, d_impl_p);
    }
    return *this;
}

inline
const char* Name::string() const
{
    return blpapi_Name_string(d_impl_p);
}

inline
size_t Name::length() const
{
    return blpapi_Name_length(d_impl_p);
}

inline
blpapi_Name_t* Name::impl() const
{
    return d_impl_p;
}

inline
Name Name::findName(const char* nameString)
{
    return Name(blpapi_Name_findName(nameString));
}

inline
bool Name::hasName(const char* nameString)
{
    return blpapi_Name_findName(nameString) ? true : false;
}

inline
bool operator==(const Name& lhs, const Name& rhs)
{
    return (lhs.impl() == rhs.impl());
}

inline
bool operator==(const Name& lhs, const char* rhs)
{
    return blpapi_Name_equalsStr(lhs.impl(), rhs) != 0;
}

inline
bool operator!=(const Name& lhs, const Name& rhs)
{
    return !(lhs == rhs);
}

inline
bool operator<(const Name& lhs, const Name& rhs)
{
    return std::strcmp(lhs.string(), rhs.string()) < 0;
}

inline
std::ostream& operator<<(std::ostream& stream,
                         const Name& name)
{
    stream << name.string();
    return stream;
}


}  // close namespace blpapi
}  // close namespace BloombergLP

#endif // __cplusplus

#endif // #ifndef INCLUDED_BLPAPI_NAME
