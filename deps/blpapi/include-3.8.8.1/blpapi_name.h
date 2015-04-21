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

//@PURPOSE: Provide a representation of strings for use as container keys.
//
//@CLASSES:
// blpapi::Name: constant string in an efficient form for use as container keys
//
//@DESCRIPTION: This component implements a representation of a string which is
// efficient for use as a key in a container (constant-time hashing and
// ordering operations using the standard "intern string" technique).

#ifndef INCLUDED_BLPAPI_TYPES
#include <blpapi_types.h>
#endif

#ifndef INCLUDED_BLPAPI_DEFS
#include <blpapi_defs.h>
#endif

#include <stddef.h>

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

#include <algorithm> // for swap
#include <iostream>
#include <cstring>   // for strcmp

namespace BloombergLP {
namespace blpapi {
                                 // ==========
                                 // class Name
                                 // ==========

class Name {
    // 'Name' represents a string in a form which is efficient for hashing and
    // comparison, thus providing efficient lookup when used as a key in either
    // ordered or hash-based containers.
    //
    // 'Name' objects are used to identify and access the classes which define
    // a schema - 'SchemaTypeDefinition', 'SchemaElementDefinition',
    // 'Constant', and 'ConstantList'.  They are also used to access the values
    // in 'Element' objects and 'Message' objects.
    //
    // The 'Name' class is an efficient substitute for a string when used as a
    // key, providing constant-time hashing and comparision.  Two 'Name'
    // objects constructed from strings for which 'strcmp()' would return 0
    // will always compare equal.
    //
    // The ordering of 'Name' objects (as defined by 'operator<(Name,Name)') is
    // consistent during a particular instance of a single application.
    // However, the ordering is not lexical and is not necessarily consistent
    // with the ordering of the same 'Name' objects in any other process.
    //
    // Where possible, 'Name' objects should be initialized once and then
    // reused.  Creating a 'Name' object from a 'const char*' involves a search
    // in a container requiring multiple string comparison operations.
    //
    // Note: Each 'Name' instance refers to an entry in a global static table.
    // 'Name' instances for identical strings will refer to the same data.
    // There is no provision for removing entries from the static table so
    // 'Name' objects should be used only when the set of input strings is
    // bounded.
    //
    // For example, creating a 'Name' for every possible field name and type in
    // a data model is reasonable (in fact, the API will do this whenever it
    // receives schema information).  Converting sequence numbers from incoming
    // messages to strings and creating a 'Name' from each one of those
    // strings, however, will cause the static table to grow in an unbounded
    // manner, and is tantamount to a memory leak.

    blpapi_Name_t *d_impl_p;

  public:
    // CLASS METHODS

    static Name findName(const char *nameString);
        // If a 'Name' already exists which matches the specified
        // 'nameString', then return a copy of that 'Name'; otherwise return a
        // 'Name' which will compare equal to a 'Name' created using the
        // default constructor. The behavior is undefined if 'nameString' does
        // not point to a null-terminated string.

    static bool hasName(const char *nameString);
        // Return 'true' if a 'Name' has been created which matches the
        // specified 'nameString'; otherwise return 'false'. The behavior is
        // undefined if 'nameString' does not point to a null-terminated
        // string.

    Name();
        // Construct an uninitialized 'Name'. An uninitialized 'Name' can be
        // assigned to, destroyed, or tested for equality. The behavior for all
        // other operations is undefined.

    Name(blpapi_Name_t *handle);

    Name(const Name& original);
        // Create a 'Name' object having the same value as the specified
        // 'original'.

    explicit Name(const char* nameString);
        // Construct a 'Name' from the specified 'nameString'. The behavior is
        // undefined unless 'nameString' is a null-terminated string. Note that
        // any null-terminated string can be specified, including an empty
        // string. Note also that constructing a 'Name' from a 'const char *'
        // is a relatively expensive operation. If a 'Name' will be used
        // repeatedly it is preferable to create it once and re-use (or copy)
        // the object.

    ~Name();
        // Destroy this object.

    // MANIPULATORS

    Name& operator=(const Name& rhs);
        // Assign to this object the value of the specified 'rhs', and return a
        // reference to this modifiable object.

    // ACCESSORS

    const char *string() const;
        // Return a pointer to the null-terminated string value of this 'Name'.
        // The pointer returned will be valid at least until main() exits.

    size_t length() const;
        // Return the length of the string value of this 'Name',
        // (excluding a terminating null). Note that 'name.length()' is
        // logically equivalent to 'strlen(name.string())', however the former
        // is potentially more efficient.

    size_t hash() const;
        // Return an integral value such that for two 'Name' objects 'a' and
        // 'b', if 'a == b' then 'a.hash() == b.hash()', and if 'a != b' then
        // it is unlikely that 'a.hash() == b.hash()'.

    blpapi_Name_t* impl() const;
};

// FREE OPERATORS
bool operator==(const Name& lhs, const Name& rhs);
    // Return true if the specified 'lhs' and 'rhs' name objects have
    // the same value, and false otherwise. Two 'Name' objects 'a' and 'b' have
    // the same value if and only if 'strcmp(a.string(), b.string()) == 0'.

bool operator!=(const Name& lhs, const Name& rhs);
    // Return false if the specified 'lhs' and 'rhs' name objects have the same
    // value, and true otherwise. Two 'Name' objects 'a' and 'b' have the same
    // value if and only if 'strcmp(a.string(), b.string()) == 0'.  Note that
    // 'lhs != rhs' is equivalent to '!(lhs==rhs)'.

bool operator==(const Name& lhs, const char *rhs);
    // Return true if the specified 'lhs' and 'rhs' have the same value, and
    // false otherwise. A 'Name' object 'a' and a null-terminated string 'b'
    // have the same value if and only if 'strcmp(a.string(), b) == 0'. The
    // behavior is undefined unless 'rhs' is a null-terminated string.

bool operator!=(const Name& lhs, const char *rhs);
    // Return false if the specified 'lhs' and 'rhs' have the same value, and
    // true otherwise. A 'Name' object 'a' and a null-terminated string 'b'
    // have the same value if and only if 'strcmp(a.string(), b) == 0'. The
    // behavior is undefined unless 'rhs' is a null-terminated string.

bool operator==(const char *lhs, const Name& rhs);
    // Return true if the specified 'lhs' and 'rhs' have the same value, and
    // false otherwise. A 'Name' object 'a' and a null-terminated string 'b'
    // have the same value if and only if 'strcmp(a.string(), b) == 0'. The
    // behavior is undefined unless 'lhs' is a null-terminated string.

bool operator!=(const char *lhs, const Name& rhs);
    // Return false if the specified 'lhs' and 'rhs' have the same value, and
    // true otherwise. A 'Name' object 'a' and a null-terminated string 'b'
    // have the same value if and only if 'strcmp(a.string(), b) == 0'. The
    // behavior is undefined unless 'lhs' is a null-terminated string.

bool operator<(const Name& lhs, const Name& rhs);
    // Return 'true' if the specified 'lhs' is ordered before the specified
    // 'rhs', and 'false' otherwise. The ordering used is stable within the
    // lifetime of a single process and is compatible with
    // 'operator==(const Name&, const Name&)', however this order is neither
    // guaranteed to be consistent across different processes (including
    // repeated runs of the same process), nor guaranteed to be lexical (i.e.
    // compatible with 'strcmp').

bool operator<=(const Name& lhs, const Name& rhs);
    // Return 'false' if the specified 'rhs' is ordered before the specified
    // 'lhs', and 'true' otherwise. The ordering used is stable within the
    // lifetime of a single process and is compatible with
    // 'operator==(const Name&, const Name&)', however this order is neither
    // guaranteed to be consistent across different processes (including
    // repeated runs of the same process), nor guaranteed to be lexical (i.e.
    // compatible with 'strcmp').

bool operator>(const Name& lhs, const Name& rhs);
    // Return 'true' if the specified 'rhs' is ordered before the specified
    // 'lhs', and 'false' otherwise. The ordering used is stable within the
    // lifetime of a single process and is compatible with
    // 'operator==(const Name&, const Name&)', however this order is neither
    // guaranteed to be consistent across different processes (including
    // repeated runs of the same process), nor guaranteed to be lexical (i.e.
    // compatible with 'strcmp').

bool operator>=(const Name& lhs, const Name& rhs);
    // Return 'false' if the specified 'lhs' is ordered before the specified
    // 'rhs', and 'true' otherwise. The ordering used is stable within the
    // lifetime of a single process and is compatible with
    // 'operator==(const Name&, const Name&)', however this order is neither
    // guaranteed to be consistent across different processes (including
    // repeated runs of the same process), nor guaranteed to be lexical (i.e.
    // compatible with 'strcmp').

std::ostream& operator<<(std::ostream& stream, const Name& name);
    // Write the value of the specified 'name' object to the specified output
    // 'stream', and return a reference to 'stream'.  Note that this
    // human-readable format is not fully specified and can change without
    // notice.

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
Name::Name(const char *nameString)
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
const char *Name::string() const
{
    return blpapi_Name_string(d_impl_p);
}

inline
size_t Name::length() const
{
    return blpapi_Name_length(d_impl_p);
}

inline
blpapi_Name_t *Name::impl() const
{
    return d_impl_p;
}

inline
Name Name::findName(const char *nameString)
{
    return Name(blpapi_Name_findName(nameString));
}

inline
bool Name::hasName(const char *nameString)
{
    return blpapi_Name_findName(nameString) ? true : false;
}

inline
size_t Name::hash() const
{
    return reinterpret_cast<size_t>(impl());
}

}  // close namespace blpapi

inline
bool blpapi::operator==(const Name& lhs, const Name& rhs)
{
    return (lhs.impl() == rhs.impl());
}

inline
bool blpapi::operator!=(const Name& lhs, const Name& rhs)
{
    return !(lhs == rhs);
}

inline
bool blpapi::operator==(const Name& lhs, const char *rhs)
{
    return blpapi_Name_equalsStr(lhs.impl(), rhs) != 0;
}

inline
bool blpapi::operator!=(const Name& lhs, const char *rhs)
{
    return !(lhs == rhs);
}

inline
bool blpapi::operator==(const char *lhs, const Name& rhs)
{
    return rhs == lhs;
}

inline
bool blpapi::operator!=(const char *lhs, const Name& rhs)
{
    return !(rhs == lhs);
}

inline
bool blpapi::operator<(const Name& lhs, const Name& rhs)
{
    return lhs.impl() < rhs.impl();
}

inline
bool blpapi::operator<=(const Name& lhs, const Name& rhs)
{
    return !(rhs < lhs);
}

inline
bool blpapi::operator>(const Name& lhs, const Name& rhs)
{
    return rhs < lhs;
}

inline
bool blpapi::operator>=(const Name& lhs, const Name& rhs)
{
    return !(lhs < rhs);
}

inline
std::ostream& blpapi::operator<<(std::ostream& stream, const Name& name)
{
    return stream << name.string();
}

}  // close namespace BloombergLP

#endif // __cplusplus

#endif // #ifndef INCLUDED_BLPAPI_NAME
