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

//@PURPOSE: Provide Constant representation in the schema.
//
//@CLASSES:
//     blpapi::Constant: Represents a constant value in the schema.
// blpapi::ConstantList: Represents a list of constant values in the schema.
//
//@DESCRIPTION: This component provides representation of constant values.

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

#ifndef INCLUDED_STDDEF
#include <stddef.h>
#define INCLUDED_STDDEF
#endif

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
    // Represents a constant value in the schema.
    //
    // Constants can be any of the following DataTypes: BOOL, CHAR,
    // BYTE, INT32, INT64, FLOAT32, FLOAT64, STRING, DATE, TIME,
    // DATETIME. As well as the constants value this class also
    // provides access to the symbolic name, description and status of
    // the constant.
    //
    // Constants are read-only and always created by the API, never by
    // the application.

    blpapi_Constant_t *d_impl_p;

  public:
    // CREATORS
    Constant(blpapi_Constant_t *handle);

    Constant(const Constant& original);
        // Copy constructor

    // MANIPULATORS

    void setUserData(void * userData);
        // Set the user data associated with this Constant to the
        // specified 'userData'.

    // ACCESSORS

    Name name() const;
        // Returns the symbolic name of this Constant.

    const char* description() const;
        // Returns a pointer to a null-terminated string containing a
        // human readable description of this Constant. This pointer
        // remains valid until this Constant is destroyed.

    int status() const;
        // Returns the status (SchemaStatus::Value) of this Constant.

    int datatype() const;
        // Return the DataType used to represent the value of this
        // constant.

    int getValueAs(char* buffer) const;
        // If this Constant's value can be converted to a char return
        // the value at the specified 'buffer' location and return
        // 0. Otherwise return non-zero.

    int getValueAs(Int32* buffer) const;
        // If this Constant's value can be converted to an Int32
        // return the value at the specified 'buffer' location and
        // return 0. Otherwise return non-zero.

    int getValueAs(Int64* buffer) const;
        // If this Constant's value can be converted to an Int64
        // return the value at the specified 'buffer' location and
        // return 0. Otherwise return non-zero.

    int getValueAs(Float32* buffer) const;
        // If this Constant's value can be converted to a Float32
        // return the value at the specified 'buffer' location and
        // return 0. Otherwise return non-zero.

    int getValueAs(Float64* buffer) const;
        // If this Constant's value can be converted to a Float64
        // return the value at the specified 'buffer' location and
        // return 0. Otherwise return non-zero.

    int getValueAs(Datetime* buffer) const;
        // If this Constant's value can be converted to a Datetime
        // return the value at the specified 'buffer' location and
        // return 0. Otherwise return non-zero.

    int getValueAs(std::string* buffer) const;
        // If this Constant's value can be converted to a string
        // return the value at the specified 'buffer' location and
        // return 0. Otherwise return non-zero.

    char getValueAsChar() const;
        // Returns this Constant's value as a char. If the value
        // cannot be converted to a char an exception is thrown.

    Int32 getValueAsInt32() const;
        // Returns this Constant's value as an Int32. If the value
        // cannot be converted to an Int32 an exception is thrown.

    Int64 getValueAsInt64() const;
        // Returns this Constant's value as an Int64. If the value
        // cannot be converted to an Int64 an exception is thrown.

    Float32 getValueAsFloat32() const;
        // Returns this Constant's value as a Float32. If the value
        // cannot be converted to a Float32 an exception is thrown.

    Float64 getValueAsFloat64() const;
        // Returns this Constant's value as a Float64. If the value
        // cannot be converted to a Float64 an exception is thrown.

    Datetime getValueAsDatetime() const;
        // Returns this Constant's value as a Datetime. If the value
        // cannot be converted to a Datetime an exception is thrown.

    std::string getValueAsString() const;
        // Returns this Constant's value as a string. If the value
        // cannot be converted to a string an exception is thrown.

    void * userData() const;
        // Returns the user data associated with this Constant. If
        // setUserData() has never been called for this
        // Constant this returns 0.

    const blpapi_Constant_t* impl() const;
};

                             // ==================
                             // class ConstantList 
                             // ==================
class ConstantList {
    // Represents a list of Constant objects in the schema.
    //
    // As well as the list of Constant objects this class also
    // provides access to the symbolic name, description and status of
    // the ConstantsList. All Constant objects in a ConstantsList are
    // of the same DataType.
    //
    // ConstantLists are read-only and always created by the API,
    // never by the application.

    blpapi_ConstantList_t *d_impl_p;

  public:
    ConstantList(blpapi_ConstantList_t *handle);

    ConstantList(const ConstantList& original);
        // Copy constructor

    // MANIPULATORS

    void setUserData(void * userData);
        // Set the user data associated with this Constant to the
        // specified 'userData'.

    // ACCESSORS

    Name name() const;
        // Returns the symbolic name of this ConstantsList.

    char const* description() const;
        // Returns a pointer to a null-terminated string containing a
        // human readable description of this ConstantsList. This
        // pointer is valid until this ConstantsList is destroyed.

    int status() const;
        // Returns the status (SchemaStatus::Value) of this
        // ConstantsList.

    int numConstants() const;
        // Return the number of Constant objects contained in this
        // ConstantsList.

    int datatype() const;
        // Return the DataType of the Constant objects contained in
        // this ConstantsList.

    Constant getConstant(const Name& name) const;
        // Return a Constant from this ConstantsList identified by the
        // specified 'name'. If this ConstantsList does not contain a
        // Constant with the specified 'name' an exception is thrown.

    Constant getConstant(const char* nameString) const;
        // Return a Constant from this ConstantsList identified by the
        // specified 'name'. If this ConstantsList does not contain a
        // Constant with the specified 'name' an exception is thrown.

    Constant getConstantAt(size_t index) const;
        // Return a read only Constant from at the specified 'index'
        // in this ConstantList. If 'index' is not in the range 0 to
        // numConstants()-1 an exception is thrown.

    void * userData() const;
        // Returns the user data associated with this ConstantList. If
        // setUserData() has never been called for this Constant this
        // returns 0.

    const blpapi_ConstantList_t* impl() const;
        // Returns the internal implementation.
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
const char* Constant::description() const
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
const blpapi_Constant_t* Constant::impl() const
{
    return d_impl_p;
}

inline
int Constant::getValueAs(char* buffer) const
{
    return blpapi_Constant_getValueAsChar(d_impl_p, buffer);
}

inline
int Constant::getValueAs(Int32* buffer) const
{
    return blpapi_Constant_getValueAsInt32(d_impl_p, buffer);
}

inline
int Constant::getValueAs(Int64* buffer) const
{
    return blpapi_Constant_getValueAsInt64(d_impl_p, buffer);
}

inline
int Constant::getValueAs(Float32* buffer) const
{
    return blpapi_Constant_getValueAsFloat32(d_impl_p, buffer);
}

inline
int Constant::getValueAs(Float64* buffer) const
{
    return blpapi_Constant_getValueAsFloat64(d_impl_p, buffer);
}

inline
int Constant::getValueAs(Datetime* buffer) const
{
    return blpapi_Constant_getValueAsDatetime(d_impl_p, &buffer->rawValue());
}

inline
int Constant::getValueAs(std::string* result) const
{
    const char* buffer;
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
void Constant::setUserData(void * userData)
{
    blpapi_Constant_setUserData(d_impl_p, userData);
}

inline
void * Constant::userData() const
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
Constant ConstantList::getConstant(const Name& name) const
{
    return blpapi_ConstantList_getConstant(d_impl_p, 0, name.impl());
}

inline
Constant ConstantList::getConstant(const char* nameString) const
{
    return blpapi_ConstantList_getConstant(d_impl_p, nameString, 0 );
}

inline
Constant ConstantList::getConstantAt(size_t index) const
{
    return blpapi_ConstantList_getConstantAt(d_impl_p, index);
}

inline
const blpapi_ConstantList_t* ConstantList::impl() const
{
    return d_impl_p;
}

inline
void ConstantList::setUserData(void * userData)
{
    blpapi_ConstantList_setUserData(d_impl_p, userData);
}

inline
void * ConstantList::userData() const
{
    return blpapi_ConstantList_userData(d_impl_p);
}

}  // close namespace blpapi
}  // close namespace BloombergLP

#endif // #ifndef __cplusplus

#endif // #ifndef INCLUDED_BLPAPI_CONSTANT
