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
// blpapi_constant.h                                                  -*-C++-*-

#ifndef INCLUDED_BLPAPI_CONSTANT
#define INCLUDED_BLPAPI_CONSTANT

//@PURPOSE: Provide a representation for schema-level enumeration constants.
//
//@CLASSES:
//     blpapi::Constant: schema enumeration constant
// blpapi::ConstantList: list of schema enumeration constants
//
//@SEE_ALSO: blpapi_schema
//
//@DESCRIPTION: This component provides a representation of a schema
// enumeration constant, and a representation for lists of such constants.

#ifndef INCLUDED_BLPAPI_TYPES
#include <blpapi_types.h>
#endif

#ifndef INCLUDED_BLPAPI_NAME
#include <blpapi_name.h>
#endif

#ifndef INCLUDED_BLPAPI_DEFS
#include <blpapi_defs.h>
#endif

#ifndef INCLUDED_BLPAPI_DATETIME
#include <blpapi_datetime.h>
#endif

#ifndef INCLUDED_BLPAPI_EXCEPTION
#include <blpapi_exception.h>
#endif

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

BLPAPI_EXPORT
void blpapi_Constant_setUserData(
    blpapi_Constant_t *constant,
    void * userdata);

BLPAPI_EXPORT
blpapi_Name_t* blpapi_Constant_name(
        const blpapi_Constant_t *constant);

BLPAPI_EXPORT
const char* blpapi_Constant_description(
        const blpapi_Constant_t *constant);

BLPAPI_EXPORT
int blpapi_Constant_status(
        const blpapi_Constant_t *constant);

BLPAPI_EXPORT
int blpapi_Constant_datatype(
        const blpapi_Constant_t *constant);

BLPAPI_EXPORT
int blpapi_Constant_getValueAsChar(
    const blpapi_Constant_t *constant,
    blpapi_Char_t *buffer);

BLPAPI_EXPORT
int blpapi_Constant_getValueAsInt32(
    const blpapi_Constant_t *constant,
    blpapi_Int32_t *buffer);

BLPAPI_EXPORT
int blpapi_Constant_getValueAsInt64(
    const blpapi_Constant_t *constant,
    blpapi_Int64_t *buffer);

BLPAPI_EXPORT
int blpapi_Constant_getValueAsFloat32(
    const blpapi_Constant_t *constant,
    blpapi_Float32_t *buffer);

BLPAPI_EXPORT
int blpapi_Constant_getValueAsFloat64(
    const blpapi_Constant_t *constant,
    blpapi_Float64_t *buffer);

BLPAPI_EXPORT
int blpapi_Constant_getValueAsDatetime(
    const blpapi_Constant_t *constant,
    blpapi_Datetime_t *buffer);

BLPAPI_EXPORT
int blpapi_Constant_getValueAsString(
    const blpapi_Constant_t *constant,
    const char **buffer);

BLPAPI_EXPORT
void * blpapi_Constant_userData(
    const blpapi_Constant_t *constant);


BLPAPI_EXPORT
void blpapi_ConstantList_setUserData(
    blpapi_ConstantList_t *constant,
    void * userdata);

BLPAPI_EXPORT
blpapi_Name_t* blpapi_ConstantList_name(
        const blpapi_ConstantList_t *list);

BLPAPI_EXPORT
const char* blpapi_ConstantList_description(
        const blpapi_ConstantList_t *list);

BLPAPI_EXPORT
int blpapi_ConstantList_numConstants(
        const blpapi_ConstantList_t *list);

BLPAPI_EXPORT
int blpapi_ConstantList_datatype(
        const blpapi_ConstantList_t *constant);

BLPAPI_EXPORT
int blpapi_ConstantList_status(
        const blpapi_ConstantList_t *list);

BLPAPI_EXPORT
blpapi_Constant_t* blpapi_ConstantList_getConstant(
        const blpapi_ConstantList_t *constant,
        const char *nameString,
        const blpapi_Name_t *name);

BLPAPI_EXPORT
blpapi_Constant_t* blpapi_ConstantList_getConstantAt(
        const blpapi_ConstantList_t *constant,
        size_t index);

BLPAPI_EXPORT
void * blpapi_ConstantList_userData(
    const blpapi_ConstantList_t *constant);

#ifdef __cplusplus
}


namespace BloombergLP {
namespace blpapi {

                             // ==============
                             // class Constant
                             // ==============
class Constant {
    // Represents the value of a schema enumeration constant.
    //
    // Constants can be any of the following DataTypes: BOOL, CHAR, BYTE,
    // INT32, INT64, FLOAT32, FLOAT64, STRING, DATE, TIME, DATETIME. This class
    // provides access to not only the constant value, but also the symbolic
    // name, the description, and the status of the constant. It also provides
    // an interface for associating arbitrary user-defined data (specified as
    // a 'void*') with a 'Constant'.
    //
    // 'Constant' objects are read-only, with the exception of a single
    // 'void*' attribute for storing user data. 'Constant' objects have
    // *reference* *semantics* with respect to this user data field: calling
    // 'c.setUserData(void*)' modifies the user data associated with 'c', as
    // well as that associated with all copies of 'c'. As a result, functions
    // which set or read this field are *NOT* per-object thread-safe. Clients
    // must syncrhonize such operations across *all* *copies* of an object.
    //
    // Application clients need never create fresh 'Constant' objects directly;
    // applications will typically work with copies of objects returned by
    // other 'blpapi' components.

    blpapi_Constant_t *d_impl_p;

  public:
    // CREATORS
    Constant(blpapi_Constant_t *handle);

    Constant(const Constant& original);
        // Create a 'Constant' object having the same value as the specified
        // 'original'. Note that this function does *not* require cross-object
        // thread synchronization, as it does not directly read or modify the
        // 'userData' field.

    // MANIPULATORS

    void setUserData(void *userData);
        // Set the user data associated with this 'Constant' -- and all copies
        // of this 'Constant' -- to the specified 'userData'. Clients are
        // responsible for synchronizing calls to this function, and to
        // 'userData()', across all copies of this 'Constant' object.

    // ACCESSORS

    Name name() const;
        // Return the symbolic name of this 'Constant'.

    const char *description() const;
        // Return a null-terminated string containing a human-readable
        // description of this 'Constant'. The returned pointer remains valid
        // until this 'Constant' is destroyed.

    int status() const;
        // Return the status, as a 'SchemaStatus::Value', of this 'Constant'.

    int datatype() const;
        // Return the data type used to represent the value of this constant as
        // an integer specified by the 'blpapi_DataType_t' enumeration defined
        // in 'blpapi_types'.

    int getValueAs(char *buffer) const;
        // Load the 'char' value of this 'Constant' into the specified 'buffer'
        // and return 0; if this 'Constant' cannot be converted into a 'char'
        // then return a nonzero value and leave 'buffer' unchanged.

    int getValueAs(Int32 *buffer) const;
        // Load the 'Int32' value of this 'Constant' into the specified
        // 'buffer' and return 0; if this 'Constant' cannot be converted into
        // an 'Int32' then return a nonzero value and leave 'buffer' unchanged.

    int getValueAs(Int64 *buffer) const;
        // Load the 'Int64' value of this 'Constant' into the specified
        // 'buffer' and return 0; if this 'Constant' cannot be converted into
        // an 'Int64' then return a nonzero value and leave 'buffer' unchanged.

    int getValueAs(Float32 *buffer) const;
        // Load the 'Float32' value of this 'Constant' into the specified
        // 'buffer' and return 0; if this 'Constant' cannot be converted into a
        // 'Float32' then return a nonzero value and leave 'buffer' unchanged.

    int getValueAs(Float64 *buffer) const;
        // Load the 'Float64' value of this 'Constant' into the specified
        // 'buffer' and return 0; if this 'Constant' cannot be converted into a
        // 'Float64' then return a nonzero value and leave 'buffer' unchanged.

    int getValueAs(Datetime *buffer) const;
        // Load the 'Datetime' value of this 'Constant' into the specified
        // 'buffer' and return 0; if this 'Constant' cannot be converted into a
        // 'Datetime' then return a nonzero value and leave 'buffer' unchanged.

    int getValueAs(std::string *buffer) const;
        // Load the 'std::string' value of this 'Constant' into the specified
        // 'buffer' and return 0; if this 'Constant' cannot be converted into a
        // 'std::string' then return a nonzero value and leave 'buffer'
        // unchanged.

    char getValueAsChar() const;
        // Return the value of this object as a 'char'. If the value cannot be
        // converted to a 'char' an exception is thrown.

    Int32 getValueAsInt32() const;
        // Return the value of this object as an 'Int32'. If the value cannot
        // be converted to an 'Int32' an exception is thrown.

    Int64 getValueAsInt64() const;
        // Return the value of this object as an 'Int64'. If the value cannot
        // be converted to an 'Int64' an exception is thrown.

    Float32 getValueAsFloat32() const;
        // Return the value of this object as a 'Float32'. If the value cannot
        // be converted to a 'Float32' an exception is thrown.

    Float64 getValueAsFloat64() const;
        // Return the value of this object as a 'Float64'. If the value cannot
        // be converted to a 'Float64' an exception is thrown.

    Datetime getValueAsDatetime() const;
        // Return the value of this object as a 'Datetime'. If the value cannot
        // be converted to a 'Datetime' an exception is thrown.

    std::string getValueAsString() const;
        // Return the value of this object as a 'std::string'. If the value
        // cannot be converted to a 'std::string' an exception is thrown.

    void *userData() const;
        // Return the user data associated with this 'Constant'. If no user
        // data has been associated with this 'Constant' then return 0. Clients
        // are responsible for synchronizing calls to this function with calls
        // to 'setUserData(void*)' made on not only this 'Constant', but also
        // all copies of this 'Constant'. Note that 'Constant' objects have
        // reference semantics: this function will reflect the last value set
        // on *any* copy of this 'Constant'.

    const blpapi_Constant_t *impl() const;
};

                             // ==================
                             // class ConstantList
                             // ==================
class ConstantList {
    // Represents a list schema enumeration constants.
    //
    // As well as the list of 'Constant' objects, this class also provides
    // access to the symbolic name, description, and status of the list as a
    // whole, and provides a facility for associating arbitrary user data (in
    // the form of a 'void*') with the list. All 'Constant' objects in a
    // 'ConstantList' are of the same DataType.
    //
    // 'ConstantList' objects are read-only, with the exception of a single
    // 'void*' attribute for storing user data. 'ConstantList' objects have
    // *reference* *semantics* with respect to this user data field: calling
    // 'c.setUserData(void*)' modifies the user data associated with 'c', as
    // well as that associated with all copies of 'c'. As a result, functions
    // which set or read this field are *NOT* per-object thread-safe. Clients
    // must syncrhonize such operations across *all* *copies* of an object.
    //
    // Application clients need never create fresh 'ConstantList' objects
    // directly; applications will typically work with copies of objects
    // returned by other 'blpapi' components.

    blpapi_ConstantList_t *d_impl_p;

  public:
    ConstantList(blpapi_ConstantList_t *handle);

    ConstantList(const ConstantList& original);
        // Create a 'ConstantList' object having the same value as the
        // specified 'original'. Note that this function does *not* require
        // cross-object thread synchronization, as it does not directly read or
        // modify the 'userData' field.

    // MANIPULATORS

    void setUserData(void *userData);
        // Set the user data associated with this 'ConstantList' -- and all
        // copies of this 'ConstantList' -- to the specified 'userData'.
        // Clients are responsible for syncrhonizing calls to this function,
        // and to 'userData()', across all copies of this 'ConstantList'
        // object.

    // ACCESSORS

    Name name() const;
        // Return the symbolic name of this 'ConstantList'.

    const char *description() const;
        // Return a null-terminated string containing a human-readable
        // description of this 'ConstantList'. The returned pointer remains
        // valid until this 'ConstantList' is destroyed.

    int status() const;
        // Return the status, as a'SchemaStatus::Value', of this
        // 'ConstantList'.

    int numConstants() const;
        // Return the number of 'Constant' objects contained in this
        // 'ConstantList'.

    int datatype() const;
        // Return the data type used to represent the value of this constant as
        // an integer specified by the 'blpapi_DataType_t' enumeration defined
        // in 'blpapi_types'.

    Constant getConstant(const Name& name) const;
        // Return the 'Constant' in this 'ConstantList' identified by the
        // specified 'name'. If this 'ConstantList' does not contain a
        // 'Constant' with the specified 'name' then an exception is thrown.

    Constant getConstant(const char *name) const;
        // Return the 'Constant' in this 'ConstantList' identified by the
        // specified 'name'. If this 'ConstantList' does not contain a
        // 'Constant' with the specified 'name' then an exception is thrown.

    Constant getConstantAt(size_t index) const;
        // Return the 'Constant' at the specified 'index' in this
        // 'ConstantList'. If 'index' is not in the range from 0 to
        // 'numConstants() - 1' then an exception is thrown.

    void *userData() const;
        // Return the user data associated with this 'ConstantList'. If no user
        // data has been associated with this 'ConstantList' then return 0.
        // Clients are responsible for synchronizing calls to this function
        // with calls to 'setUserData(void*)' made on not only this
        // 'ConstantList', but also all copies of this 'ConstantList'. Note
        // that 'ConstantList' objects have reference semantics: this function
        // will reflect the last value set on *any* copy of this
        // 'ConstantList'.

    const blpapi_ConstantList_t *impl() const;
        // Return the internal implementation.
};

//=============================================================================
//                           INLINE FUNCTION DEFINITIONS
//=============================================================================

inline
Constant::Constant(blpapi_Constant_t *handle)
: d_impl_p(handle)
{
}

inline
Constant::Constant(
    const Constant& original)
: d_impl_p(original.d_impl_p)
{
}

inline
Name Constant::name() const
{
    return blpapi_Constant_name(d_impl_p);
}

inline
const char *Constant::description() const
{
    return blpapi_Constant_description(d_impl_p);
}

inline
int Constant::status() const
{
    return blpapi_Constant_status(d_impl_p);
}

inline
int Constant::datatype() const
{
    return blpapi_Constant_datatype(d_impl_p);
}

inline
const blpapi_Constant_t *Constant::impl() const
{
    return d_impl_p;
}

inline
int Constant::getValueAs(char *buffer) const
{
    return blpapi_Constant_getValueAsChar(d_impl_p, buffer);
}

inline
int Constant::getValueAs(Int32 *buffer) const
{
    return blpapi_Constant_getValueAsInt32(d_impl_p, buffer);
}

inline
int Constant::getValueAs(Int64 *buffer) const
{
    return blpapi_Constant_getValueAsInt64(d_impl_p, buffer);
}

inline
int Constant::getValueAs(Float32 *buffer) const
{
    return blpapi_Constant_getValueAsFloat32(d_impl_p, buffer);
}

inline
int Constant::getValueAs(Float64 *buffer) const
{
    return blpapi_Constant_getValueAsFloat64(d_impl_p, buffer);
}

inline
int Constant::getValueAs(Datetime *buffer) const
{
    return blpapi_Constant_getValueAsDatetime(d_impl_p, &buffer->rawValue());
}

inline
int Constant::getValueAs(std::string *result) const
{
    const char *buffer;
    int rc = blpapi_Constant_getValueAsString(d_impl_p, &buffer);
    if (!rc) {
        *result = buffer;
    }
    return rc;
}


inline
char Constant::getValueAsChar() const
{
    char value;
    ExceptionUtil::throwOnError(getValueAs(&value));
    return value;
}

inline
Int32 Constant::getValueAsInt32() const
{
    int value;
    ExceptionUtil::throwOnError(getValueAs(&value));
    return value;
}

inline
Int64 Constant::getValueAsInt64() const
{
    Int64 value;
    ExceptionUtil::throwOnError(getValueAs(&value));
    return value;
}

inline
float Constant::getValueAsFloat32() const
{
    Float32 value;
    ExceptionUtil::throwOnError(getValueAs(&value));
    return value;
}

inline
double Constant::getValueAsFloat64() const
{
    Float64 value;
    ExceptionUtil::throwOnError(getValueAs(&value));
    return value;
}

inline
Datetime Constant::getValueAsDatetime() const
{
    Datetime value;
    ExceptionUtil::throwOnError(getValueAs(&value));
    return value;
}

inline
std::string Constant::getValueAsString() const
{
    std::string value;
    ExceptionUtil::throwOnError(getValueAs(&value));
    return value;
}

inline
void Constant::setUserData(void *newUserData)
{
    blpapi_Constant_setUserData(d_impl_p, newUserData);
}

inline
void *Constant::userData() const
{
    return blpapi_Constant_userData(d_impl_p);
}



inline
ConstantList::ConstantList(
        blpapi_ConstantList_t *handle)
: d_impl_p(handle)
{
}

inline
ConstantList::ConstantList(
    const ConstantList& original)
: d_impl_p(original.d_impl_p)
{
}

inline
Name ConstantList::name() const
{
    return blpapi_ConstantList_name(d_impl_p);
}

inline
const char* ConstantList::description() const
{
    return blpapi_ConstantList_description(d_impl_p);
}

inline
int ConstantList::status() const
{
    return blpapi_ConstantList_status(d_impl_p);
}

inline
int ConstantList::datatype() const
{
    return blpapi_ConstantList_datatype(d_impl_p);
}

inline
int ConstantList::numConstants() const
{
    return blpapi_ConstantList_numConstants(d_impl_p);
}

inline
Constant ConstantList::getConstant(const Name& constantName) const
{
    return blpapi_ConstantList_getConstant(d_impl_p, 0, constantName.impl());
}

inline
Constant ConstantList::getConstant(const char *nameString) const
{
    return blpapi_ConstantList_getConstant(d_impl_p, nameString, 0 );
}

inline
Constant ConstantList::getConstantAt(size_t index) const
{
    return blpapi_ConstantList_getConstantAt(d_impl_p, index);
}

inline
const blpapi_ConstantList_t *ConstantList::impl() const
{
    return d_impl_p;
}

inline
void ConstantList::setUserData(void *newUserData)
{
    blpapi_ConstantList_setUserData(d_impl_p, newUserData);
}

inline
void *ConstantList::userData() const
{
    return blpapi_ConstantList_userData(d_impl_p);
}

}  // close namespace blpapi
}  // close namespace BloombergLP

#endif // #ifndef __cplusplus

#endif // #ifndef INCLUDED_BLPAPI_CONSTANT
