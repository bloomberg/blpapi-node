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
// blpapi_schema.h                                                    -*-C++-*-
#ifndef INCLUDED_BLPAPI_SCHEMA
#define INCLUDED_BLPAPI_SCHEMA

//@PURPOSE: Provide a representation of a schema describing structured messages
//
//@CLASSES:
// blpapi::SchemaStatus: the version status of a schema
// blpapi::SchemaTypeDefinition: definitions of schema types
// blpapi::SchemaElementDefinition: definitions of message elements
//
//@DESCRIPTION: This component provides types for representing schemata which
// describe structured messages. Such schemata consist of two distinct kinds of
// definitions: "type" definitions (represented by 'SchemaTypeDefinition'
// objects) declare types than can be used within other definitions (of both
// kinds); an "element" definition defines a specific field by associating a
// field identifier with a particular type, as well as the number of values of
// that type that are permitted to be associated with that identifier.

#ifndef INCLUDED_BLPAPI_CONSTANT
#include <blpapi_constant.h>
#endif

#ifndef INCLUDED_BLPAPI_DEFS
#include <blpapi_defs.h>
#endif

#ifndef INCLUDED_BLPAPI_EXCEPTION
#include <blpapi_exception.h>
#endif

#ifndef INCLUDED_BLPAPI_NAME
#include <blpapi_name.h>
#endif

#ifndef INCLUDED_BLPAPI_STREAMPROXY
#include <blpapi_streamproxy.h>
#endif

#ifndef INCLUDED_BLPAPI_TYPES
#include <blpapi_types.h>
#endif

typedef void *blpapi_SchemaElementDefinition_t;
typedef void *blpapi_SchemaTypeDefinition_t;

#ifdef __cplusplus
extern "C" {
#endif

BLPAPI_EXPORT
blpapi_Name_t *blpapi_SchemaElementDefinition_name(
        const blpapi_SchemaElementDefinition_t *field);

BLPAPI_EXPORT
const char *blpapi_SchemaElementDefinition_description(
        const blpapi_SchemaElementDefinition_t *field);

BLPAPI_EXPORT
int blpapi_SchemaElementDefinition_status(
        const blpapi_SchemaElementDefinition_t *field);

BLPAPI_EXPORT
blpapi_SchemaTypeDefinition_t *blpapi_SchemaElementDefinition_type(
        const blpapi_SchemaElementDefinition_t *field);

BLPAPI_EXPORT
size_t blpapi_SchemaElementDefinition_numAlternateNames(
        const blpapi_SchemaElementDefinition_t *field);

BLPAPI_EXPORT
blpapi_Name_t *blpapi_SchemaElementDefinition_getAlternateName(
        const blpapi_SchemaElementDefinition_t *field,
        size_t index);


//   -- NOT USED or IMPLEMENTED: removing
// BLPAPI_EXPORT
// size_t blpapi_SchemaElementDefinition_numNames(
//         const blpapi_SchemaElementDefinition_t *field);


BLPAPI_EXPORT
size_t blpapi_SchemaElementDefinition_minValues(
        const blpapi_SchemaElementDefinition_t *field);

BLPAPI_EXPORT
size_t blpapi_SchemaElementDefinition_maxValues(
        const blpapi_SchemaElementDefinition_t *field);

BLPAPI_EXPORT
int blpapi_SchemaElementDefinition_print(
        const blpapi_SchemaElementDefinition_t *element,
        blpapi_StreamWriter_t streamWriter,
        void *userStream,
        int level,
        int spacesPerLevel);

BLPAPI_EXPORT
void blpapi_SchemaElementDefinition_setUserData(
        blpapi_SchemaElementDefinition_t *field,
        void *userdata);

BLPAPI_EXPORT
void *blpapi_SchemaElementDefinition_userData(
        const blpapi_SchemaElementDefinition_t *field);

BLPAPI_EXPORT
blpapi_Name_t *blpapi_SchemaTypeDefinition_name(
        const blpapi_SchemaTypeDefinition_t *type);

BLPAPI_EXPORT
const char *blpapi_SchemaTypeDefinition_description(
        const blpapi_SchemaTypeDefinition_t *type);

BLPAPI_EXPORT
int blpapi_SchemaTypeDefinition_status(
        const blpapi_SchemaTypeDefinition_t *type);

BLPAPI_EXPORT
int blpapi_SchemaTypeDefinition_datatype(
        const blpapi_SchemaTypeDefinition_t *type);

BLPAPI_EXPORT
int blpapi_SchemaTypeDefinition_isComplexType(
        const blpapi_SchemaTypeDefinition_t *type);

BLPAPI_EXPORT
int blpapi_SchemaTypeDefinition_isSimpleType(
        const blpapi_SchemaTypeDefinition_t *type);

BLPAPI_EXPORT
int blpapi_SchemaTypeDefinition_isEnumerationType(
        const blpapi_SchemaTypeDefinition_t *type);

BLPAPI_EXPORT
int blpapi_SchemaTypeDefinition_isComplex(
        const blpapi_SchemaTypeDefinition_t *type);

BLPAPI_EXPORT
int blpapi_SchemaTypeDefinition_isSimple(
        const blpapi_SchemaTypeDefinition_t *type);

BLPAPI_EXPORT
int blpapi_SchemaTypeDefinition_isEnumeration(
        const blpapi_SchemaTypeDefinition_t *type);


BLPAPI_EXPORT
size_t blpapi_SchemaTypeDefinition_numElementDefinitions(
        const blpapi_SchemaTypeDefinition_t *type);

BLPAPI_EXPORT
blpapi_SchemaElementDefinition_t*
blpapi_SchemaTypeDefinition_getElementDefinition(
        const blpapi_SchemaTypeDefinition_t *type,
        const char *nameString,
        const blpapi_Name_t *name);

BLPAPI_EXPORT
blpapi_SchemaElementDefinition_t*
blpapi_SchemaTypeDefinition_getElementDefinitionAt(
        const blpapi_SchemaTypeDefinition_t *type,
        size_t index);

BLPAPI_EXPORT
int blpapi_SchemaTypeDefinition_print(
        const blpapi_SchemaTypeDefinition_t *element,
        blpapi_StreamWriter_t streamWriter,
        void *userStream,
        int level,
        int spacesPerLevel);

BLPAPI_EXPORT
void blpapi_SchemaTypeDefinition_setUserData(
        blpapi_SchemaTypeDefinition_t *element,
        void *userdata);

BLPAPI_EXPORT
void *blpapi_SchemaTypeDefinition_userData(
        const blpapi_SchemaTypeDefinition_t *element);

BLPAPI_EXPORT
blpapi_ConstantList_t*
blpapi_SchemaTypeDefinition_enumeration(
        const blpapi_SchemaTypeDefinition_t *element);


#ifdef __cplusplus
}

#ifndef INCLUDED_IOSFWD
#include <iosfwd>
#define INCLUDED_IOSFWD
#endif

namespace BloombergLP {
namespace blpapi {
                         // ===================
                         // struct SchemaStatus
                         // ===================

struct SchemaStatus {
    // This 'struct' provides a namespace for enumerating the possible
    // deprecation statuses of a schema element or type.

    enum Value {
        ACTIVE              = BLPAPI_STATUS_ACTIVE,     // This item is current
                                                        // and may appear in
                                                        // Messages
        DEPRECATED          = BLPAPI_STATUS_DEPRECATED, // This item is current
                                                        // and may appear in
                                                        // Messages but will be
                                                        // removed in due
                                                        // course
        INACTIVE            = BLPAPI_STATUS_INACTIVE,   // This item is not
                                                        // current and will not
                                                        // appear in Messages
        PENDING_DEPRECATION = BLPAPI_STATUS_PENDING_DEPRECATION
                                                        // This item is
                                                        // expected to be
                                                        // deprecated in the
                                                        // future; clients are
                                                        // advised to migrate
                                                        // away from use of
                                                        // this item.
    };
};

class SchemaTypeDefinition;

                       // =============================
                       // class SchemaElementDefinition
                       // =============================

class SchemaElementDefinition {
    // This class implements the definition of an individual field within a
    // schema type. An element is defined by an identifer/name, a type, and the
    // number of values of that type that may be associated with the
    // identifier/name. In addition, this class offers access to metadata
    // providing a description and deprecation status for the field. Finally,
    // 'SchemaElementDefinition' provides an interface for associating
    // arbitrary user-defined data (specified as a 'void*') with an element
    // definition.
    //
    // 'SchemaElementDefinition' objects are returned by 'Service' and
    // 'Operation' objects to define the content of requests, replies and
    // events. The 'SchemaTypeDefinition' returned by
    // 'SchemaElementDefinition::typeDefinition()' may itself provide access to
    // 'SchemaElementDefinition' objects when the schema contains nested
    // elements. (See the 'SchemaTypeDefinition' documentation for more
    // information on complex types.)
    //
    // An optional element has 'minValues() == 0'.
    //
    // A mandatory element has 'minValues() >= 1'.
    //
    // An element that must constain a single value has
    // 'minValues() == maxValues() == 1'.
    //
    // An element containing an array has 'maxValues() > 1'.
    //
    // An element with no upper limit on the number of values has
    // 'maxValues() == UNBOUNDED'.
    //
    // 'SchemaElementDefinition' objects are read-only, with the exception of a
    // single 'void*' attribute for storing user data.
    // 'SchemaElementDefinition' objects have *reference* *semantics* with
    // respect to this user data field: calling 'c.setUserData(void*)' modifies
    // the user data associated with 'c', as well as that associated with all
    // copies of 'c'. As a result, functions which set or read this attribute
    // are *NOT* per-object thread-safe. Clients must syncrhonize such
    // operations across *all* *copies* of an object.
    //
    // Application clients need never create fresh 'SchemaElementDefinition'
    // objects directly; applications will typically work with copies of
    // objects returned by other 'blpapi' components.

    blpapi_SchemaElementDefinition_t *d_impl_p;

  public:
    // Constants used in the SchemaElementDefinition interface.

    enum {
        UNBOUNDED = BLPAPI_ELEMENTDEFINITION_UNBOUNDED  // Indicates an array
                                                        // has an unbounded
                                                        // number of values.
    };

    SchemaElementDefinition(blpapi_SchemaElementDefinition_t *handle);

    ~SchemaElementDefinition();
        // Destroy this object.

    // MANIPULATORS

    void setUserData(void *userData);
        // Set the user data associated with this 'SchemaElementDefinition' --
        // and all copies of this 'SchemaElementDefinition' -- to the specified
        // 'userData'. Clients are responsible for synchronizing calls to this
        // function, and to 'userData()', across all copies of this
        // 'SchemaElementDefinition' object.

    // ACCESSORS

    Name name() const;
        // Return the name identifying this element within its containing
        // structure/type.

    const char *description() const;
        // Return a null-terminated string containing a human-readable
        // description of this element. This pointer is valid until this
        // 'SchemaElementDefinition' is destroyed.

    int status() const;
        // Return the deprecation status, as a 'SchemaStatus::Value', of this
        // element.

    const SchemaTypeDefinition typeDefinition() const;
        // Return the type of values contained in this element.

    size_t minValues() const;
        // Return the minimum number of occurrences of this element. This value
        // is always greater than or equal to zero.

    size_t maxValues() const ;
        // Return the maximum number of occurrences of this element. This value
        // is always greater than or equal to one.

    size_t numAlternateNames() const;
        // Return the number of alternate names for this element.

    Name getAlternateName(size_t index) const;
        // Return the specified 'index'th alternate name for this element. If
        // 'index >=numAlternateNames()' an exception is thrown.

    void *userData() const;
        // Return the user data associated with this 'SchemaElementDefinition'.
        // If no user data has been associated with this
        // 'SchemaElementDefinition' then return 0. Clients are responsible for
        // synchronizing calls to this function with calls to
        // 'setUserData(void*)' made on not only this
        // 'SchemaElementDefinition', but also all copies of this
        // 'SchemaElementDefinition'. Note that 'SchemaElementDefinition'
        // objects have reference semantics: this function will reflect the
        // last value set on *any* copy of this 'SchemaElementDefinition'.

    std::ostream& print(std::ostream& stream,
                        int level=0,
                        int spacesPerLevel=4) const;
        // Format this SchemaElementDefinition to the specified output 'stream'
        // at the (absolute value of) the optionally specified indentation
        // 'level' and return a reference to 'stream'. If 'level' is specified,
        // optionally specify 'spacesPerLevel', the number of spaces per
        // indentation level for this and all of its nested objects. If 'level'
        // is negative, suppress indentation of the first line. If
        // 'spacesPerLevel' is negative, format the entire output on one line,
        // suppressing all but the initial indentation (as governed by
        // 'level').

    blpapi_SchemaElementDefinition_t *impl() const;
};

// FREE OPERATORS
std::ostream& operator<<(std::ostream& stream,
                         const SchemaElementDefinition& element);
    // Write the value of the specified 'element' object to the specified
    // output 'stream' in a single-line format, and return a reference to
    // 'stream'.  If 'stream' is not valid on entry, this operation has no
    // effect.  Note that this human-readable format is not fully specified,
    // can change without notice, and is logically equivalent to:
    //..
    //  print(stream, 0, -1);
    //..

                       // ==========================
                       // class SchemaTypeDefinition
                       // ==========================

class SchemaTypeDefinition {
    // This class implements a representation of a "type" that can be used
    // within a schema, including both simple atomic types (integers, dates,
    // strings, etc.) as well as "complex" types defined a sequences of or
    // choice among a collection (named) elements, each of which is in turn
    // described by another type. In addition to accessors for the type's
    // structure, this class also offers access to metadata providing a
    // description and deprecation status for the type. Finally,
    // 'SchemaTypeDefinition' provides an interface for associating arbitrary
    // user-defined data (specified as a 'void*') with a type definition.
    //
    // Each 'SchemaElementDefinition' object is associated with a single
    // 'SchemaTypeDefinition'; one 'SchemaTypeDefinition' may be used by zero,
    // one, or many 'SchemaElementDefinition' objects.
    //
    // 'SchemaTypeDefinition' objects are read-only, with the exception of a
    // single 'void*' attribute for storing user data. 'SchemaTypeDefinition'
    // objects have *reference* *semantics* with respect to this user data
    // field: calling 'c.setUserData(void*)' modifies the user data associated
    // with 'c', as well as that associated with all copies of 'c'. As a
    // result, functions which set or read this attribute are *NOT* per-object
    // thread-safe. Clients must syncrhonize such operations across *all*
    // *copies* of an object.
    //
    // Application clients need never create fresh 'SchemaTypeDefinition'
    // objects directly; applications will typically work with copies of
    // objects returned by other 'blpapi' components.

    blpapi_SchemaTypeDefinition_t *d_impl_p;

  public:
    SchemaTypeDefinition(blpapi_SchemaTypeDefinition_t *handle);

    ~SchemaTypeDefinition();
        // Destroy this object.

    // MANIPULATORS

    void setUserData(void *userData);
        // Set the user data associated with this 'SchemaTypeDefinition' -- and
        // all copies of this 'SchemaTypeDefinition' -- to the specified
        // 'userData'. Clients are responsible for synchronizing calls to this
        // function, and to 'userData()', across all copies of this
        // 'SchemaTypeDefinition' object.

    // ACCESSORS

    int datatype() const;
        // Return the 'DataType' of this 'SchemaTypeDefinition'.

    Name name() const;
        // Return the name of this 'SchemaTypeDefinition'.

    const char *description() const;
        // Return a null-terminated string which contains a human readable
        // description of this 'SchemaTypeDefinition'. The returned pointer
        // remains valid until this 'SchemaTypeDefinition' is destroyed.

    int status() const;
        // Return the deprecation status, as a 'SchemaStatus::Value', of this
        // 'SchemaTypeDefinition'.

    size_t numElementDefinitions() const;
        // Return the number of 'SchemaElementDefinition' objects contained by
        // this 'SchemaTypeDefinition'. If this 'SchemaTypeDefinition' is
        // neither a choice nor a sequence this will return 0.

    bool isComplexType() const;
        // Return 'true' if this 'SchemaTypeDefinition' represents a sequence
        // or choice type.

    bool isSimpleType() const;
        // Return 'true' if this 'SchemaTypeDefinition' represents neither a
        // sequence nor a choice type.

    bool isEnumerationType() const;
        // Return 'true' if this 'SchemaTypeDefinition' represents an enumeration
        // type.

    bool hasElementDefinition(const Name& name) const;
        // Return 'true' if this 'SchemaTypeDefinition' contains an element
        // with the specified 'name'; otherwise returns 'false'.

    bool hasElementDefinition(const char *name) const;
        // Return 'true' if this 'SchemaTypeDefinition' contains an element
        // with the specified 'name'; otherwise returns 'false'.

    SchemaElementDefinition getElementDefinition(const Name& name) const;
        // Return the definition of the element identified by the specified
        // 'name'. If 'hasElementDefinition(name) != true' then an exception is
        // thrown.

    SchemaElementDefinition getElementDefinition(const char *nameString) const;
        // Return the definition of the element identified by the specified
        // 'nameString'. If 'hasElementDefinition(nameString) != true' then an
        // exception is thrown.

    SchemaElementDefinition getElementDefinition(size_t index) const;
        // Return the definition of the element a the specified 'index' in the
        // sequence of elements. If 'index >= numElementDefinitions()' an
        // exception is thrown.

    const ConstantList enumeration() const;
        // Return a 'ConstantList' containing all possible values of the
        // enumeration defined by this type. The behavior of this function is
        // undefined unless 'isEnumerationType() == true'.

    void *userData() const;
        // Return the user data associated with this 'SchemaTypeDefinition'. If
        // no user data has been associated with this 'SchemaTypeDefinition'
        // then return 0. Clients are responsible for synchronizing calls to
        // this function with calls to 'setUserData(void*)' made on not only
        // this 'SchemaTypeDefinition', but also all copies of this
        // 'SchemaTypeDefinition'. Note that 'SchemaTypeDefinition' objects
        // have reference semantics: this function will reflect the last value
        // set on *any* copy of this 'SchemaTypeDefinition'.

    std::ostream& print(std::ostream& stream,
                        int level=0,
                        int spacesPerLevel=4) const;
        // Format this SchemaTypeDefinition to the specified output 'stream' at
        // the (absolute value of) the optionally specified indentation 'level'
        // and return a reference to 'stream'. If 'level' is specified,
        // optionally specify 'spacesPerLevel', the number of spaces per
        // indentation level for this and all of its nested objects. If 'level'
        // is negative, suppress indentation of the first line. If
        // 'spacesPerLevel' is negative, format the entire output on one line,
        // suppressing all but the initial indentation (as governed by
        // 'level').
};

// FREE OPERATORS
std::ostream& operator<<(std::ostream& stream,
                         const SchemaTypeDefinition& typeDef);
    // Write the value of the specified 'typeDef' object to the specified
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

                       // -----------------------------
                       // class SchemaElementDefinition
                       // -----------------------------

inline
SchemaElementDefinition::SchemaElementDefinition(
    blpapi_SchemaElementDefinition_t *handle)
: d_impl_p(handle)
{
}


inline
SchemaElementDefinition::~SchemaElementDefinition()
{
}

inline
Name SchemaElementDefinition::name() const
{
    return Name(blpapi_SchemaElementDefinition_name(d_impl_p));
}

inline
const char *SchemaElementDefinition::description() const
{
    return blpapi_SchemaElementDefinition_description(d_impl_p);
}

inline
int SchemaElementDefinition::status() const
{
    return blpapi_SchemaElementDefinition_status(d_impl_p);
}

inline
const SchemaTypeDefinition SchemaElementDefinition::typeDefinition() const
{
    return blpapi_SchemaElementDefinition_type(d_impl_p);
}

inline
size_t SchemaElementDefinition::minValues() const
{
    return blpapi_SchemaElementDefinition_minValues(d_impl_p);
}

inline
size_t SchemaElementDefinition::maxValues() const
{
    return blpapi_SchemaElementDefinition_maxValues(d_impl_p);
}

inline
size_t SchemaElementDefinition::numAlternateNames() const
{
    return blpapi_SchemaElementDefinition_numAlternateNames(d_impl_p);
}

inline
Name SchemaElementDefinition::getAlternateName(size_t index) const
{
    blpapi_Name_t *alternateName =
        blpapi_SchemaElementDefinition_getAlternateName(d_impl_p, index);
    if (alternateName == 0) {
        ExceptionUtil::throwOnError(BLPAPI_ERROR_INDEX_OUT_OF_RANGE);
    }
    return alternateName;
}

inline
std::ostream& SchemaElementDefinition::print(
        std::ostream& stream,
        int level,
        int spacesPerLevel) const
{
    blpapi_SchemaElementDefinition_print(d_impl_p,
                                         StreamProxyOstream::writeToStream,
                                         &stream,
                                         level,
                                         spacesPerLevel);
    return stream;
}

inline
std::ostream& operator<<(
        std::ostream& stream,
        const SchemaElementDefinition& element)
{
    element.print(stream, 0, -1);
    return stream;
}

inline
void SchemaElementDefinition::setUserData(void *newUserData)
{
    blpapi_SchemaElementDefinition_setUserData(d_impl_p, newUserData);
}

inline
void *SchemaElementDefinition::userData() const
{
    return blpapi_SchemaElementDefinition_userData(d_impl_p);
}

inline
blpapi_SchemaElementDefinition_t *SchemaElementDefinition::impl() const
{
    return d_impl_p;
}
                       // --------------------------
                       // class SchemaTypeDefinition
                       // --------------------------

inline
SchemaTypeDefinition::SchemaTypeDefinition(
                                         blpapi_SchemaTypeDefinition_t *handle)
: d_impl_p(handle)
{
}

inline
SchemaTypeDefinition::~SchemaTypeDefinition()
{
}

inline
int SchemaTypeDefinition::datatype() const
{
    return blpapi_SchemaTypeDefinition_datatype(d_impl_p);
}

inline
Name SchemaTypeDefinition::name() const
{
    return blpapi_SchemaTypeDefinition_name(d_impl_p);
}

inline
const char *SchemaTypeDefinition::description() const
{
    return blpapi_SchemaTypeDefinition_description(d_impl_p);
}

inline
int SchemaTypeDefinition::status() const
{
    return blpapi_SchemaTypeDefinition_status(d_impl_p);
}

inline
size_t SchemaTypeDefinition::numElementDefinitions() const
{
    return blpapi_SchemaTypeDefinition_numElementDefinitions(d_impl_p);
}

inline
bool SchemaTypeDefinition::hasElementDefinition(const Name& elementName) const
{
    return blpapi_SchemaTypeDefinition_getElementDefinition(
            d_impl_p, 0, elementName.impl()) ? true : false;
}

inline
bool
SchemaTypeDefinition::hasElementDefinition(const char *nameString) const
{
    return blpapi_SchemaTypeDefinition_getElementDefinition(
            d_impl_p, nameString, 0) ? true : false;
}

inline
SchemaElementDefinition
SchemaTypeDefinition::getElementDefinition(const Name& elementName) const
{
    blpapi_SchemaElementDefinition_t *def =
        blpapi_SchemaTypeDefinition_getElementDefinition(d_impl_p,
                                                         0,
                                                         elementName.impl());
    if (def == 0) {
        ExceptionUtil::throwOnError(BLPAPI_ERROR_ITEM_NOT_FOUND);
    }
    return def;
}

inline
SchemaElementDefinition
SchemaTypeDefinition::getElementDefinition(const char *nameString) const
{
    blpapi_SchemaElementDefinition_t *def =
        blpapi_SchemaTypeDefinition_getElementDefinition(d_impl_p,
                                                         nameString,
                                                         0);
    if (def == 0) {
        ExceptionUtil::throwOnError(BLPAPI_ERROR_ITEM_NOT_FOUND);
    }
    return def;
}

inline
SchemaElementDefinition
SchemaTypeDefinition::getElementDefinition(size_t index) const
{
    blpapi_SchemaElementDefinition_t *def =
        blpapi_SchemaTypeDefinition_getElementDefinitionAt(d_impl_p, index);
    if (def == 0) {
        ExceptionUtil::throwOnError(BLPAPI_ERROR_INDEX_OUT_OF_RANGE);
    }
    return def;
}

inline
bool SchemaTypeDefinition::isComplexType() const
{
    return blpapi_SchemaTypeDefinition_isComplexType(d_impl_p) ? true : false;
}

inline
bool SchemaTypeDefinition::isSimpleType() const
{
    return blpapi_SchemaTypeDefinition_isSimpleType(d_impl_p) ? true : false;
}

inline
bool SchemaTypeDefinition::isEnumerationType() const
{
    return blpapi_SchemaTypeDefinition_isEnumerationType(d_impl_p)
        ? true
        : false;
}

inline
std::ostream& SchemaTypeDefinition::print(
        std::ostream& stream,
        int level,
        int spacesPerLevel) const
{
    blpapi_SchemaTypeDefinition_print(d_impl_p,
                                      StreamProxyOstream::writeToStream,
                                      &stream,
                                      level,
                                      spacesPerLevel);
    return stream;
}

inline
std::ostream& operator<<(
        std::ostream& stream,
        const SchemaTypeDefinition& typeDef)
{
    typeDef.print(stream, 0, -1);
    return stream;
}

inline
void SchemaTypeDefinition::setUserData(void *newUserData)
{
    blpapi_SchemaTypeDefinition_setUserData(d_impl_p, newUserData);
}

inline
void *SchemaTypeDefinition::userData() const
{
    return blpapi_SchemaTypeDefinition_userData(d_impl_p);
}

inline
const ConstantList SchemaTypeDefinition::enumeration() const
{
    return blpapi_SchemaTypeDefinition_enumeration(d_impl_p);
}

}  // close namespace blpapi
}  // close namespace BloombergLP

#endif // #ifdef __cplusplus

#endif // #ifndef INCLUDED_BLPAPI_SCHEMA
