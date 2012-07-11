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

//@PURPOSE: Provide a representation of schema
//
//@CLASSES:
// blpapi::SchemaStatus: The status for the schema
// blpapi::SchemaTypeDefinition: Definitions for schema types
// blpapi::SchemaElementDefinition: Definitions for element types
//
//@DESCRIPTION: This component provides a representation of schema.

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
    // The possible values for an item in the schema.

    // Possible values for an item in the schema.

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
    };
};

class SchemaTypeDefinition;

                       // =============================
                       // class SchemaElementDefinition
                       // =============================

class SchemaElementDefinition {
    // The definition of an item in the data schema.
    //
    // SchemaElementDefinition are returned by Service objects and
    // Operation objects to define the content of requests, replies
    // and events. The SchemaTypeDefinition returned by
    // SchemaElementDefinition::typeDefinition() may itself return
    // SchemaElementDefinition objects when the schema contains
    // complex items. SchemaElementDefinition objects are read-only
    // and always created by the API, never by the application.
    //
    // The SchemaElementDefinition contains the symbolic name, any
    // constraints on this item and the SchemaTypeDefinition for
    // instances of this item.
    //
    // An item which is optional in the schema has minValues()==0.
    //
    // An item which is mandatory in the schema has minValues()>=1.
    //
    // An item which is a single value has maxValues()==1.
    //
    // An item which is an array has maxValues()>1.
    //
    // An item which is an unbounded array has maxValues()==UNBOUNDED.
    //
    // As well as the symbolic name, array constraints and other
    // constraints this class also provides access to the description
    // and status of the item.

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
        // Destroy this SchemaElementDefinition.

    // MANIPULATORS

    void setUserData(void *userData);
        // Set the user data associated with this
        // SchemaElementDefinition to the specified 'userData'.

    // ACCESSORS

    Name name() const;
        // Returns the name of this item.

    const char *description() const;
        // Returns a pointer to a null-terminated string containing a
        // human readable description of this item. This pointer is
        // valid until this SchemaElementDefinition is destroyed.

    int status() const;
        // Returns the status (SchemaStatus::Value) of this
        // SchemaElementDefinition.

    const SchemaTypeDefinition typeDefinition() const;
        // Returns the SchemaTypeDefinition of this item.

    size_t minValues() const;
        // Returns the minimum number of occurrences of this item. This
        // value is always >= 0.

    size_t maxValues() const ;
        // Returns the maximum number of occurrences of this item. This
        // value is always >= 1.

    size_t numAlternateNames() const;
        // Returns the number of alternate names for this item.

    Name getAlternateName(size_t index) const;
        // Returns the specified 'index'th alternate name for this
        // item. If 'index'>=numAlternateNames() an exception is
        // thrown.

    void *userData() const;
        // Returns the user data associated with this
        // SchemaElementDefinition. If setUserData() has never been
        // called for this SchemaElementDefinition this returns 0.

    std::ostream& print(std::ostream& stream,
                        int level=0,
                        int spacesPerLevel=4) const;
        // Format this SchemaElementDefinition to the specified output
        // 'stream' at the (absolute value of) the optionally specified
        // indentation 'level' and return a reference to 'stream'. If
        // 'level' is specified, optionally specify 'spacesPerLevel',
        // the number of spaces per indentation level for this and all
        // of its nested objects. If 'level' is negative, suppress
        // indentation of the first line. If 'spacesPerLevel' is
        // negative, format the entire output on one line, suppressing
        // all but the initial indentation (as governed by 'level').


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
    // The definition of a type in the data schema.
    //
    // Each SchemaElementDefinition object has a SchemaTypeDefinition
    // (which it may share with other SchemaElementDefinition
    // objects). SchemaTypeDefinition objects are read-only and always
    // created by the API, never by the application.
    //
    // A SchemaTypeDefinition can define either a items which are
    // simple (that is, they are a single value) or sequence or
    // choice Element (that is, they may contain an item or items
    // which are accessed by name)
    //
    // For sequence or choice items the SchemaTypeDefinition provides
    // access to the individual SchemaElementDefinition objects that
    // define each member of the sequence.
    //
    // In addition, the SchemaTypeDefinition provides access to the
    // symbolic name of the TypeDefinition, its description and its
    // status.

    blpapi_SchemaTypeDefinition_t *d_impl_p;

  public:
    SchemaTypeDefinition(blpapi_SchemaTypeDefinition_t *handle);

    ~SchemaTypeDefinition();
        // Destroy this SchemaTypeDefinition.

    // MANIPULATORS

    void setUserData(void *userData);
        // Set the user data associated with this
        // SchemaTypeDefinition to the specified 'userData'.

    // ACCESSORS

    int datatype() const;
        // Returns the DataType of this SchemaTypeDefinition.

    Name name() const;
        // Returns the name of this SchemaTypeDefinition.

    const char *description() const;
        // Returns a pointer to a null-terminated string which
        // contains a human readable description of this
        // SchemaTypeDefinition. The pointer remains valid until this
        // SchemaTypeDefinition is destroyed.

    int status() const;
        // Returns the status (SchemaStatus::Value) of this
        // SchemaTypeDefinition.

    size_t numElementDefinitions() const;
        // Returns the number of SchemaElementDefinition objects
        // contained by this SchemaTypeDefinition. If this
        // SchemaTypeDefinition is neither a choice nor a sequence this
        // will return 0.

    bool isComplexType() const;
        // Returns 'true' if this SchemaTypeDefinition represents a
        // sequence or choice type.

    bool isSimpleType() const;
        // Returns 'true' if this SchemaTypeDefinition represents
        // neither a sequence nor a choice type.

    bool isEnumerationType() const;
        // Returns 'true' if this SchemaTypeDefinition represents an
        // enumeration type.

    bool hasElementDefinition(const Name& name) const;
        // Returns 'true' if this SchemaTypeDefinition contains an item
        // with the specified 'name', otherwise returns 'false'.

    bool hasElementDefinition(const char *name) const;
        // Returns 'true' if this SchemaTypeDefinition contains an
        // item with the specified 'name', otherwise returns 'false'.

    SchemaElementDefinition getElementDefinition(const Name& name) const;
        // Returns the definition of the item identified by the
        // specified 'name'. If there is no item 'name' an exception
        // is thrown.

    SchemaElementDefinition getElementDefinition(const char *nameString) const;
        // Returns the definition of the item identified by the
        // specified 'nameString'. If there is no item 'nameString' an
        // exception is thrown.

    SchemaElementDefinition getElementDefinition(size_t index) const;
        // Returns the definition of the specified 'index'th item. If
        // 'index'>=numElementDefinitions() an exception is thrown.

    const ConstantList enumeration() const;
        // If this SchemaTypeDefinition returns 'true' to
        // isEnumerationType() this returns a ConstantList which
        // contains the possible values.

    void *userData() const;
        // Returns the user data associated with this
        // SchemaTypeDefinition object. If setUserData() has never
        // been called for this SchemaTypeDefinition this returns 0.

    std::ostream& print(std::ostream& stream,
                        int level=0,
                        int spacesPerLevel=4) const;
        // Format this SchemaTypeDefinition to the specified output
        // 'stream' at the (absolute value of) the optionally specified
        // indentation 'level' and return a reference to 'stream'. If
        // 'level' is specified, optionally specify 'spacesPerLevel',
        // the number of spaces per indentation level for this and all
        // of its nested objects. If 'level' is negative, suppress
        // indentation of the first line. If 'spacesPerLevel' is
        // negative, format the entire output on one line, suppressing
        // all but the initial indentation (as governed by 'level').
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
    blpapi_Name_t *name =
        blpapi_SchemaElementDefinition_getAlternateName(d_impl_p, index);
    if (name == 0) {
        ExceptionUtil::throwOnError(BLPAPI_ERROR_INDEX_OUT_OF_RANGE);
    }
    return name;
}

inline
std::ostream& SchemaElementDefinition::print(
        std::ostream& stream,
        int level,
        int spacesPerLevel) const
{
    blpapi_SchemaElementDefinition_print(
            d_impl_p, OstreamWriter,  &stream, level, spacesPerLevel);
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
void SchemaElementDefinition::setUserData(void *userData)
{
    blpapi_SchemaElementDefinition_setUserData(d_impl_p, userData);
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
bool SchemaTypeDefinition::hasElementDefinition(const Name& name) const
{
    return blpapi_SchemaTypeDefinition_getElementDefinition(
            d_impl_p, 0, name.impl()) ? true : false;
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
SchemaTypeDefinition::getElementDefinition(const Name& name) const
{
    blpapi_SchemaElementDefinition_t *def =
        blpapi_SchemaTypeDefinition_getElementDefinition(d_impl_p,
                                                         0,
                                                         name.impl());
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
    blpapi_SchemaTypeDefinition_print(
            d_impl_p, OstreamWriter,  &stream, level, spacesPerLevel);
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
void SchemaTypeDefinition::setUserData(void *userData)
{
    blpapi_SchemaTypeDefinition_setUserData(d_impl_p, userData);
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
