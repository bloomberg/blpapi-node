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
// blpapi_element.h                                                   -*-C++-*-
#ifndef INCLUDED_BLPAPI_ELEMENT
#define INCLUDED_BLPAPI_ELEMENT

//@PURPOSE: Provide a representation of an item in a message.
//
//@CLASSES:
// blpapi::Element: Represents an item in a message
//
//@DESCRIPTION: This component implements a representation of an item in a
// 'Message'.

#ifndef INCLUDED_BLPAPI_CALL
#include <blpapi_call.h>
#endif

#ifndef INCLUDED_BLPAPI_TYPES
#include <blpapi_types.h>
#endif

#ifndef INCLUDED_BLPAPI_SCHEMA
#include <blpapi_schema.h>
#endif

#ifndef INCLUDED_BLPAPI_DATETIME
#include <blpapi_datetime.h>
#endif

#ifndef INCLUDED_BLPAPI_NAME
#include <blpapi_name.h>
#endif

#ifndef INCLUDED_BLPAPI_DEFS
#include <blpapi_defs.h>
#endif

#ifndef INCLUDED_BLPAPI_STREAMPROXY
#include <blpapi_streamproxy.h>
#endif

#ifndef INCLUDED_STDDEF
#include <stddef.h>
#define INCLUDED_STDDEF
#endif

#ifdef __cplusplus
extern "C" {
#endif

BLPAPI_EXPORT blpapi_Name_t*
blpapi_Element_name(const blpapi_Element_t *element);

BLPAPI_EXPORT const char*
blpapi_Element_nameString(const blpapi_Element_t *element);

BLPAPI_EXPORT blpapi_SchemaElementDefinition_t*
blpapi_Element_definition(const blpapi_Element_t* element);

BLPAPI_EXPORT int blpapi_Element_datatype (
        const blpapi_Element_t* element);

BLPAPI_EXPORT int blpapi_Element_isComplexType(
        const blpapi_Element_t* element);

BLPAPI_EXPORT int blpapi_Element_isArray(
        const blpapi_Element_t* element);

BLPAPI_EXPORT int blpapi_Element_isReadOnly(
        const blpapi_Element_t* element);

BLPAPI_EXPORT size_t blpapi_Element_numValues(
        const blpapi_Element_t* element);

BLPAPI_EXPORT size_t blpapi_Element_numElements(
        const blpapi_Element_t* element);

BLPAPI_EXPORT int blpapi_Element_isNullValue(
        const blpapi_Element_t* element,
        size_t position);

BLPAPI_EXPORT int blpapi_Element_isNull(
        const blpapi_Element_t* element);


BLPAPI_EXPORT int blpapi_Element_print(
        const blpapi_Element_t* element,
        blpapi_StreamWriter_t streamWriter,
        void *stream,
        int level,
        int spacesPerLevel);

BLPAPI_EXPORT
int blpapi_Element_getElementAt(
        const blpapi_Element_t* element,
        blpapi_Element_t **result,
        size_t position);

BLPAPI_EXPORT
int blpapi_Element_getElement(
        const blpapi_Element_t *element,
        blpapi_Element_t **result,
        const char* nameString,
        const blpapi_Name_t *name);

BLPAPI_EXPORT
int blpapi_Element_hasElement(
        const blpapi_Element_t *element,
        const char* nameString,
        const blpapi_Name_t *name);

BLPAPI_EXPORT
int blpapi_Element_hasElementEx(
        const blpapi_Element_t *element,
        const char* nameString,
        const blpapi_Name_t *name,
        int excludeNullElements,
        int reserved);

BLPAPI_EXPORT
int blpapi_Element_getValueAsBool(
        const blpapi_Element_t *element,
        blpapi_Bool_t *buffer,
        size_t index);

BLPAPI_EXPORT
int blpapi_Element_getValueAsChar(
        const blpapi_Element_t *element,
        blpapi_Char_t *buffer,
        size_t index);

BLPAPI_EXPORT
int blpapi_Element_getValueAsInt32(
        const blpapi_Element_t *element,
        blpapi_Int32_t *buffer,
        size_t index);

BLPAPI_EXPORT
int blpapi_Element_getValueAsInt64(
        const blpapi_Element_t *element,
        blpapi_Int64_t *buffer,
        size_t index);

BLPAPI_EXPORT
int blpapi_Element_getValueAsFloat32(
        const blpapi_Element_t *element,
        blpapi_Float32_t *buffer,
        size_t index);

BLPAPI_EXPORT
int blpapi_Element_getValueAsFloat64(
        const blpapi_Element_t *element,
        blpapi_Float64_t *buffer,
        size_t index);

BLPAPI_EXPORT
int blpapi_Element_getValueAsString(
        const blpapi_Element_t *element,
        const char **buffer,
        size_t index);

BLPAPI_EXPORT
int blpapi_Element_getValueAsDatetime(
        const blpapi_Element_t *element,
        blpapi_Datetime_t *buffer,
        size_t index);

BLPAPI_EXPORT
int blpapi_Element_getValueAsHighPrecisionDatetime(
        const blpapi_Element_t *element,
        blpapi_HighPrecisionDatetime_t *buffer,
        size_t index);

BLPAPI_EXPORT
int blpapi_Element_getValueAsElement(
        const blpapi_Element_t *element,
        blpapi_Element_t **buffer,
        size_t index);

BLPAPI_EXPORT
int blpapi_Element_getValueAsName(
        const blpapi_Element_t *element,
        blpapi_Name_t **buffer,
        size_t index);

BLPAPI_EXPORT
int blpapi_Element_getChoice(
        const blpapi_Element_t *element,
        blpapi_Element_t **result);

BLPAPI_EXPORT
int blpapi_Element_setValueBool(
        blpapi_Element_t *element,
        blpapi_Bool_t value,
        size_t index);

BLPAPI_EXPORT
int blpapi_Element_setValueChar(
        blpapi_Element_t *element,
        blpapi_Char_t value,
        size_t index);

BLPAPI_EXPORT
int blpapi_Element_setValueInt32(
        blpapi_Element_t *element,
        blpapi_Int32_t value,
        size_t index);

BLPAPI_EXPORT
int blpapi_Element_setValueInt64(
        blpapi_Element_t *element,
        blpapi_Int64_t value,
        size_t index);

BLPAPI_EXPORT
int blpapi_Element_setValueFloat32(
        blpapi_Element_t *element,
        blpapi_Float32_t value,
        size_t index);

BLPAPI_EXPORT
int blpapi_Element_setValueFloat64(
        blpapi_Element_t *element,
        blpapi_Float64_t value,
        size_t index);

BLPAPI_EXPORT
int blpapi_Element_setValueString(
        blpapi_Element_t *element,
        const char *value,
        size_t index);

BLPAPI_EXPORT
int blpapi_Element_setValueDatetime(
        blpapi_Element_t *element,
        const blpapi_Datetime_t *value,
        size_t index);

BLPAPI_EXPORT
int blpapi_Element_setValueHighPrecisionDatetime(
        blpapi_Element_t *element,
        const blpapi_HighPrecisionDatetime_t *value,
        size_t index);

BLPAPI_EXPORT
int blpapi_Element_setValueFromElement(
        blpapi_Element_t *element,
        blpapi_Element_t *value,
        size_t index);

BLPAPI_EXPORT
int blpapi_Element_setValueFromName (
        blpapi_Element_t *element,
        const blpapi_Name_t *value,
        size_t index);

BLPAPI_EXPORT
int blpapi_Element_setElementBool(
        blpapi_Element_t *element,
        const char* nameString,
        const blpapi_Name_t* name,
        blpapi_Bool_t value);

BLPAPI_EXPORT
int blpapi_Element_setElementChar(
        blpapi_Element_t *element,
        const char* nameString,
        const blpapi_Name_t* name,
        blpapi_Char_t value);

BLPAPI_EXPORT
int blpapi_Element_setElementInt32(
        blpapi_Element_t *element,
        const char* nameString,
        const blpapi_Name_t* name,
        blpapi_Int32_t value);

BLPAPI_EXPORT
int blpapi_Element_setElementInt64(
        blpapi_Element_t *element,
        const char* nameString,
        const blpapi_Name_t* name,
        blpapi_Int64_t value);

BLPAPI_EXPORT
int blpapi_Element_setElementFloat32(
        blpapi_Element_t *element,
        const char* nameString,
        const blpapi_Name_t* name,
        blpapi_Float32_t value);

BLPAPI_EXPORT
int blpapi_Element_setElementFloat64(
        blpapi_Element_t *element,
        const char* nameString,
        const blpapi_Name_t* name,
        blpapi_Float64_t value);

BLPAPI_EXPORT
int blpapi_Element_setElementString(
        blpapi_Element_t *element,
        const char *nameString,
        const blpapi_Name_t* name,
        const char *value);

BLPAPI_EXPORT
int blpapi_Element_setElementDatetime(
        blpapi_Element_t *element,
        const char* nameString,
        const blpapi_Name_t* name,
        const blpapi_Datetime_t *value);

BLPAPI_EXPORT
int blpapi_Element_setElementHighPrecisionDatetime(
        blpapi_Element_t *element,
        const char *nameString,
        const blpapi_Name_t *name,
        const blpapi_HighPrecisionDatetime_t *value);

BLPAPI_EXPORT
int blpapi_Element_setElementFromField(
        blpapi_Element_t *element,
        const char* nameString,
        const blpapi_Name_t* name,
        blpapi_Element_t *sourcebuffer);

BLPAPI_EXPORT
int blpapi_Element_setElementFromName (
        blpapi_Element_t *element,
        const char* elementName,
        const blpapi_Name_t* name,
        const blpapi_Name_t *buffer);

BLPAPI_EXPORT
int blpapi_Element_appendElement (
        blpapi_Element_t *element,
        blpapi_Element_t **appendedElement);

BLPAPI_EXPORT
int blpapi_Element_setChoice (
        blpapi_Element_t *element,
        blpapi_Element_t **resultElement,
        const char* nameCstr,
        const blpapi_Name_t* name,
        size_t index);

#ifdef __cplusplus
}

#ifndef INCLUDED_BLPAPI_EXCEPTION
#include <blpapi_exception.h>
#endif

#ifndef INCLUDED_STRING
#include <string>
#define INCLUDED_STRING
#endif

namespace BloombergLP {
namespace blpapi {
                         // =============
                         // class Element
                         // =============

class Element {
    // Element represents an item in a message.
    //
    // An Element can represent: a single value of any data type
    // supported by the Bloomberg API; an array of values; a sequence
    // or a choice.
    //
    // The value(s) in an Element can be queried in a number of
    // ways. For an Element which represents a single value or an
    // array of values use the getValueAs() functions or
    // getValueAsBool() etc. For an Element which represents a
    // sequence or choice use getElementAsBool() etc. In addition, for
    // choices and sequences, hasElement() and getElement() are
    // useful.
    //
    // This example shows how to access the value of a
    // scalar element 's' as a floating point number:
    //..
    //      float f = s.getValueAsFloat32();
    //..
    // Similarly, this example shows how to retrieve the third value
    // in an array element 'a', as a floating pointer number:
    //..
    //      float f = a.getValueAsFloat32(2);
    //..
    // Use numValues() to determine the number of values
    // available. For single values, it will return either 0 or 1. For
    // arrays it will return the actual number of values in the array.
    //
    // To retrieve values from a complex element types (sequences and
    // choices) use the getElementAs...() family of methods. This
    // example shows how to get the value of the element 'city' in
    // the sequence element 'address':
    //..
    //      const char* city = address.getElementAsString("city");
    //..
    // The value(s) of an Element can be set in a number of ways. For
    // an Element which represents a single value or an array of
    // values use the setValue() or appendValue() functions. For an
    // element which represents a sequence or a choice use the
    // setElement() functions.
    //
    // This example shows how to set the value of an Element 's'
    // from an integer:
    //..
    //      Int32 value=5;
    //      s.setValue(value);
    //..
    // This example shows how to append an integer a value to an array
    // element 'a':
    //..
    //      Int32 value=5;
    //      s.appendValue(value);
    //..
    // To set values in a complex element (a sequence or a choice)
    // use the setElement() family of functions. This example shows
    // how to set the value of the element 'city' in the sequence
    // element 'address' to a string.
    //..
    //      Element address;
    //      ...
    //      address.setElement("city", "New York");
    //..
    // Methods which specify a Element name come in two forms. One
    // which takes a Name as a parameter and one which takes a const
    // char*.  The form which takes Name is more efficient. However, it
    // requires the Name to have been created in the global name
    // table.
    //
    // The form which takes a const char* is less efficient but will
    // not cause a new Name to be created in the global Name
    // table. Because all valid Element names will have already been
    // placed in the global name table by the API if the supplied
    // string cannot be found in the global name table the appropriate
    // error or exception can be returned.
    //
    // The API will convert data types as long as there is no loss of
    // precision involved. So
    //
    // - A BOOL can be returned as bool, char, Int32, Int64, Float32,
    // Float64, std::string (the string will be "true" or "false")
    //
    // - A BOOL can be set from bool, std::string (if it has a value
    // of "y", "n", "yes", "no", "true" or "false")
    //
    // - A CHAR can be returned as char, Int32, Int64, Float32, Float64,
    // std::string
    //
    // - A CHAR can be set from char
    //
    // - An INT32 can be returned as Int32, Int64, Float64, std::string
    //
    // - An INT32 can be set from char, Int32
    //
    // - An INT64 can be returned as Int64, std::string
    //
    // - An INT64 can be set from char, Int32, Int64
    //
    // - A FLOAT32 can be returned as Float32, Float64, std::string
    //
    // - A FLOAT32 can be set from char
    //
    // - A FLOAT64 can be returned as Float64, std::string
    //
    // - A FLOAT64 can be set from char, Int32, Float32
    //
    // - A STRING can be returned as std::string, Name
    //
    // - A STRING can be set from bool (the string will be set to
    // "true" or "false"), char, Int32, Int64, Float32, Float64, Name
    //
    // - A DATETIME can be returned as Datetime
    //
    // - A DATETIME can be set from Datetime

    blpapi_Element_t *d_handle_p;

  public:

    Element();
        // Create an uninitialized Element. The only valid operations
        // to perform on an unitialized Element are assignment,
        // isValid() and destruction.

    Element(blpapi_Element_t *element);

    // MANIPULATORS

    void rebind(blpapi_Element_t *element);

    void setElement(const char* name, bool value);
        // Set the specified element 'name' within this sequence or
        // choice Element to the specified 'value'. An exception is
        // thrown if the specified 'name' is not valid for this
        // Element, or if the Element identified by the specified
        // 'name' cannot be initialized from the type of the specified
        // 'value'.
        // 'name' must be a valid pointer.

    void setElement(const char* name, char value);
        // Set the specified element 'name' within this sequence or
        // choice Element to the specified 'value'. An exception is
        // thrown if the specified 'name' is not valid for this
        // Element, or if the Element identified by the specified
        // 'name' cannot be initialized from the type of the specified
        // 'value'.
        // 'name' must be a valid pointer.

    void setElement(const char* name, Int32 value);
        // Set the specified element 'name' within this sequence or
        // choice Element to the specified 'value'. An exception is
        // thrown if the specified 'name' is not valid for this
        // Element, or if the Element identified by the specified
        // 'name' cannot be initialized from the type of the specified
        // 'value'.
        // 'name' must be a valid pointer.

    void setElement(const char* name, Int64 value);
        // Set the specified element 'name' within this sequence or
        // choice Element to the specified 'value'. An exception is
        // thrown if the specified 'name' is not valid for this
        // Element, or if the Element identified by the specified
        // 'name' cannot be initialized from the type of the specified
        // 'value'.
        // 'name' must be a valid pointer.

    void setElement(const char* name, Float32 value);
        // Set the specified element 'name' within this sequence or
        // choice Element to the specified 'value'. An exception is
        // thrown if the specified 'name' is not valid for this
        // Element, or if the Element identified by the specified
        // 'name' cannot be initialized from the type of the specified
        // 'value'.
        // 'name' must be a valid pointer.

    void setElement(const char* name, Float64 value);
        // Set the specified element 'name' within this sequence or
        // choice Element to the specified 'value'. An exception is
        // thrown if the specified 'name' is not valid for this
        // Element, or if the Element identified by the specified
        // 'name' cannot be initialized from the type of the specified
        // 'value'.
        // 'name' must be a valid pointer.

    void setElement(const char* name, const Datetime& value);
        // Set the specified element 'name' within this sequence or
        // choice Element to the specified 'value'. An exception is
        // thrown if the specified 'name' is not valid for this
        // Element, or if the Element identified by the specified
        // 'name' cannot be initialized from the type of the specified
        // 'value'.
        // 'name' must be a valid pointer.

    void setElement(const char* name, const char* value);
        // Set the specified element 'name' within this sequence or
        // choice Element to the specified 'value'. An exception is
        // thrown if the specified 'name' is not valid for this
        // Element, or if the Element identified by the specified
        // 'name' cannot be initialized from the type of the specified
        // 'value'.
        // 'name' must be a valid pointer.

    void setElement(const char* name, const Name& value);
        // Set the specified element 'name' within this sequence or
        // choice Element to the specified 'value'. An exception is
        // thrown if the specified 'name' is not valid for this
        // Element, or if the Element identified by the specified
        // 'name' cannot be initialized from the type of the specified
        // 'value'.
        // 'name' must be a valid pointer.

    void setElement(const Name& name, bool value);
        // Set the specified element 'name' within this sequence or
        // choice Element to the specified 'value'. An exception is
        // thrown if the specified 'name' is not valid for this
        // Element, or if the Element identified by the specified
        // 'name' cannot be initialized from the type of the specified
        // 'value'.
        // 'name' cannot be an uninitialized Name.

    void setElement(const Name& name, char value);
        // Set the specified element 'name' within this sequence or
        // choice Element to the specified 'value'. An exception is
        // thrown if the specified 'name' is not valid for this
        // Element, or if the Element identified by the specified
        // 'name' cannot be initialized from the type of the specified
        // 'value'.
        // 'name' cannot be an uninitialized Name.

    void setElement(const Name& name, Int32 value);
        // Set the specified element 'name' within this sequence or
        // choice Element to the specified 'value'. An exception is
        // thrown if the specified 'name' is not valid for this
        // Element, or if the Element identified by the specified
        // 'name' cannot be initialized from the type of the specified
        // 'value'.
        // 'name' cannot be an uninitialized Name.

    void setElement(const Name& name, Int64 value);
        // Set the specified element 'name' within this sequence or
        // choice Element to the specified 'value'. An exception is
        // thrown if the specified 'name' is not valid for this
        // Element, or if the Element identified by the specified
        // 'name' cannot be initialized from the type of the specified
        // 'value'.
        // 'name' cannot be an uninitialized Name.

    void setElement(const Name& name, Float32 value);
        // Set the specified element 'name' within this sequence or
        // choice Element to the specified 'value'. An exception is
        // thrown if the specified 'name' is not valid for this
        // Element, or if the Element identified by the specified
        // 'name' cannot be initialized from the type of the specified
        // 'value'.
        // 'name' cannot be an uninitialized Name.

    void setElement(const Name& name, Float64 value);
        // Set the specified element 'name' within this sequence or
        // choice Element to the specified 'value'. An exception is
        // thrown if the specified 'name' is not valid for this
        // Element, or if the Element identified by the specified
        // 'name' cannot be initialized from the type of the specified
        // 'value'.
        // 'name' cannot be an uninitialized Name.

    void setElement(const Name& name, const Datetime& value);
        // Set the specified element 'name' within this sequence or
        // choice Element to the specified 'value'. An exception is
        // thrown if the specified 'name' is not valid for this
        // Element, or if the Element identified by the specified
        // 'name' cannot be initialized from the type of the specified
        // 'value'.
        // 'name' cannot be an uninitialized Name.

    void setElement(const Name& name, const char* value);
        // Set the specified element 'name' within this sequence or
        // choice Element to the specified 'value'. An exception is
        // thrown if the specified 'name' is not valid for this
        // Element, or if the Element identified by the specified
        // 'name' cannot be initialized from the type of the specified
        // 'value'.
        // 'name' cannot be an uninitialized Name.

    void setElement(const Name& name, const Name& value);
        // Set the specified element 'name' within this sequence or
        // choice Element to the specified 'value'. An exception is
        // thrown if the specified 'name' is not valid for this
        // Element, or if the Element identified by the specified
        // 'name' cannot be initialized from the type of the specified
        // 'value'.
        // 'name' cannot be an uninitialized Name.

    void setValue(bool value, size_t index=0);
        // Set the value of the specified 'index'th entry in this
        // Element to the specified 'value'. An exception is thrown if
        // this Element's DataType means it cannot be initialized from
        // an instance of the supplied 'value'. An exception is thrown
        // if 'index'>=numValues().

    void setValue(char value, size_t index=0);
        // Set the value of the specified 'index'th entry in this
        // Element to the specified 'value'. An exception is thrown if
        // this Element's DataType means it cannot be initialized from
        // an instance of the supplied 'value'. An exception is thrown
        // if 'index'>=numValues().

    void setValue(Int32 value, size_t index=0);
        // Set the value of the specified 'index'th entry in this
        // Element to the specified 'value'. An exception is thrown if
        // this Element's DataType means it cannot be initialized from
        // an instance of the supplied 'value'. An exception is thrown
        // if 'index'>=numValues().

    void setValue(Int64 value, size_t index=0);
        // Set the value of the specified 'index'th entry in this
        // Element to the specified 'value'. An exception is thrown if
        // this Element's DataType means it cannot be initialized from
        // an instance of the supplied 'value'. An exception is thrown
        // if 'index'>=numValues().

    void setValue(Float32 value, size_t index=0);
        // Set the value of the specified 'index'th entry in this
        // Element to the specified 'value'. An exception is thrown if
        // this Element's DataType means it cannot be initialized from
        // an instance of the supplied 'value'. An exception is thrown
        // if 'index'>=numValues().

    void setValue(Float64 value, size_t index=0);
        // Set the value of the specified 'index'th entry in this
        // Element to the specified 'value'. An exception is thrown if
        // this Element's DataType means it cannot be initialized from
        // an instance of the supplied 'value'. An exception is thrown
        // if 'index'>=numValues().

    void setValue(const Datetime& value, size_t index=0);
        // Set the value of the specified 'index'th entry in this
        // Element to the specified 'value'. An exception is thrown if
        // this Element's DataType means it cannot be initialized from
        // an instance of the supplied 'value'. An exception is thrown
        // if 'index'>=numValues().

    void setValue(const char* value, size_t index=0);
        // Set the value of the specified 'index'th entry in this
        // Element to the specified 'value'. An exception is thrown if
        // this Element's DataType means it cannot be initialized from
        // an instance of the supplied 'value'. An exception is thrown
        // if 'index'>=numValues().

    void setValue(const Name& valueName, size_t index=0);
        // Set the value of the specified 'index'th entry in this
        // Element to the specified 'value'. An exception is thrown if
        // this Element's DataType means it cannot be initialized from
        // an instance of the supplied 'value'. An exception is thrown
        // if 'index'>=numValues().

    void appendValue(bool value);
        // Appends the specified 'value' to this Element as the last
        // element. An exception is thrown if this Element's DataType
        // means it cannot be initialized from an instance of the
        // supplied 'value', or if the current size of this Element
        // (numValues()) is equal to the maximum defined by
        // elementDefinition().maxValues().

    void appendValue(char value);
        // Appends the specified 'value' to this Element as the last
        // element. An exception is thrown if this Element's DataType
        // means it cannot be initialized from an instance of the
        // supplied 'value', or if the current size of this Element
        // (numValues()) is equal to the maximum defined by
        // elementDefinition().maxValues().

    void appendValue(Int32 value);
        // Appends the specified 'value' to this Element as the last
        // element. An exception is thrown if this Element's DataType
        // means it cannot be initialized from an instance of the
        // supplied 'value', or if the current size of this Element
        // (numValues()) is equal to the maximum defined by
        // elementDefinition().maxValues().

    void appendValue(Int64 value);
        // Appends the specified 'value' to this Element as the last
        // element. An exception is thrown if this Element's DataType
        // means it cannot be initialized from an instance of the
        // supplied 'value', or if the current size of this Element
        // (numValues()) is equal to the maximum defined by
        // elementDefinition().maxValues().

    void appendValue(Float32 value);
        // Appends the specified 'value' to this Element as the last
        // element. An exception is thrown if this Element's DataType
        // means it cannot be initialized from an instance of the
        // supplied 'value', or if the current size of this Element
        // (numValues()) is equal to the maximum defined by
        // elementDefinition().maxValues().

    void appendValue(Float64 value);
        // Appends the specified 'value' to this Element as the last
        // element. An exception is thrown if this Element's DataType
        // means it cannot be initialized from an instance of the
        // supplied 'value', or if the current size of this Element
        // (numValues()) is equal to the maximum defined by
        // elementDefinition().maxValues().

    void appendValue(const Datetime& value);
        // Appends the specified 'value' to this Element as the last
        // element. An exception is thrown if this Element's DataType
        // means it cannot be initialized from an instance of the
        // supplied 'value', or if the current size of this Element
        // (numValues()) is equal to the maximum defined by
        // elementDefinition().maxValues().

    void appendValue(const char* value);
        // Appends the specified 'value' to this Element as the last
        // element. An exception is thrown if this Element's DataType
        // means it cannot be initialized from an instance of the
        // supplied 'value', or if the current size of this Element
        // (numValues()) is equal to the maximum defined by
        // elementDefinition().maxValues().

    void appendValue(const Name& value);
        // Appends the specified 'value' to this Element as the last
        // element. An exception is thrown if this Element's DataType
        // means it cannot be initialized from an instance of the
        // supplied 'value', or if the current size of this Element
        // (numValues()) is equal to the maximum defined by
        // elementDefinition().maxValues().

    Element appendElement();
        // If this Element is an array of sequence or choice Elements
        // append an element to this element and return the appended
        // element. Otherwise an exception is thrown.

    Element setChoice(const char* selectionName);
        // If this Element has datatype()==DataType::CHOICE then set
        // the active Element to the one specified by 'selectionName'
        // if 'selectionName' valid for this Element and return a
        // reference to it. Otherwise an exception is thrown.

    Element setChoice(const Name& selectionName);
        // If this Element has datatype()==DataType::CHOICE then set
        // the active Element to the one specified by 'selectionName'
        // if 'selectionName' valid for this Element and return a
        // reference to it. Otherwise an exception is thrown.

    blpapi_Element_t* handle();

    // ACCESSORS

    Name name() const;
        // If this Element is part of a sequence or choice Element
        // then this returns the Name of this Element within the
        // sequence or choice Element that owns it. If this Element
        // is not part of a sequence Element (that is it is an entire
        // Request or Message) then the Name of the Request or Message
        // is returned.

    int datatype() const;
        // Return the basic data type used to represent a value in
        // this element. The possible return values are enumerated in
        // DataType.

    bool isComplexType() const;
        // Returns true if datatype()==DataType::SEQUENCE or
        // datatype()==DataType::CHOICE.

    bool isArray() const;
        // Returns true if elementDefinition().maxValues()>1 or if
        // elementDefinition().maxValues()==UNBOUNDED.

    bool isNull() const;
        // Returns true if this element is in a NULL state. Otherwise
        // returns false.

    bool isReadOnly() const;
        // Returns true if this element cannot be modified.

    SchemaElementDefinition elementDefinition() const;
        // Return a reference to the read-only element definition
        // object that defines the properties of this elements value.

    size_t numValues() const;
        // Return the number of values contained by this element.  For
        // scalar element types. The value returned will always be in
        // the range defined by elementDefinition().minValues() and
        // elementDefinition().maxValues().

    size_t numElements() const;
        // Return the number of elements contained by this element if
        // isComplexType()==true. If the DataType is CHOICE this will
        // always return 1. If the DataType is SEQUENCE this may
        // return any number (including 0). If isComplex()==false this
        // returns 0.

    bool isValid() const;
        // Returns true if this Element is valid. An Element created
        // using the default constructor is not valid until it has had
        // a value assigned to it.

    bool isNullValue(size_t position = 0) const;
        // Returns true if the value of the sub-element at the specified 
        // 'position' in a sequence or choice element is a "NULL" value.
        // An exception is thrown if 'position'>=numElements().

    bool hasElement(const char* name, bool excludeNullElements = false) const;
    bool hasElement(const Name& name, bool excludeNullElements = false) const;
        // Returns true if this element is a choice or sequence
        // (isComplexType()==true) and it contains an Element with the
        // specified 'name'.

    int getValueAs(bool* result, size_t index=0) const;
        // Set the specified 'result' to the value of the specified
        // 'index'th entry in this Element and return 0. An error
        // (non-zero) is returned if the DataType of this Element
        // cannot be converted to the type of 'result' or if 'index'
        // is greater than the current size of this Element
        // (numValues()).

    int getValueAs(char* result, size_t index=0) const;
        // Set the specified 'result' to the value of the specified
        // 'index'th entry in this Element and return 0. An error
        // (non-zero) is returned if the DataType of this Element
        // cannot be converted to the type of 'result' or if 'index'
        // is greater than the current size of this Element
        // (numValues()).

    int getValueAs(Int32* result, size_t index=0) const;
        // Set the specified 'result' to the value of the specified
        // 'index'th entry in this Element and return 0. An error
        // (non-zero) is returned if the DataType of this Element
        // cannot be converted to the type of 'result' or if 'index'
        // is greater than the current size of this Element
        // (numValues()).

    int getValueAs(Int64* result, size_t index=0) const;
        // Set the specified 'result' to the value of the specified
        // 'index'th entry in this Element and return 0. An error
        // (non-zero) is returned if the DataType of this Element
        // cannot be converted to the type of 'result' or if 'index'
        // is greater than the current size of this Element
        // (numValues()).

    int getValueAs(Float32* result, size_t index=0) const;
        // Set the specified 'result' to the value of the specified
        // 'index'th entry in this Element and return 0. An error
        // (non-zero) is returned if the DataType of this Element
        // cannot be converted to the type of 'result' or if 'index'
        // is greater than the current size of this Element
        // (numValues()).

    int getValueAs(Float64* result, size_t index=0) const;
        // Set the specified 'result' to the value of the specified
        // 'index'th entry in this Element and return 0. An error
        // (non-zero) is returned if the DataType of this Element
        // cannot be converted to the type of 'result' or if 'index'
        // is greater than the current size of this Element
        // (numValues()).

    int getValueAs(Datetime* result, size_t index=0) const;
        // Set the specified 'result' to the value of the specified
        // 'index'th entry in this Element and return 0. An error
        // (non-zero) is returned if the DataType of this Element
        // cannot be converted to the type of 'result' or if 'index'
        // is greater than the current size of this Element
        // (numValues()).

    int getValueAs(std::string* result, size_t index=0) const;
        // Set the specified 'result' to the value of the specified
        // 'index'th entry in this Element and return 0. An error
        // (non-zero) is returned if the DataType of this Element
        // cannot be converted to the type of 'result' or if 'index'
        // is greater than the current size of this Element
        // (numValues()).

    int getValueAs(Element *result, size_t index=0) const;
        // Set the specified 'result' to the value of the specified
        // 'index'th entry in this Element and return 0. An error
        // (non-zero) is returned if the DataType of this Element
        // cannot be converted to the type of 'result' or if 'index'
        // is greater than the current size of this Element
        // (numValues()).

    int getValueAs(Name *result, size_t index=0) const;
        // Set the specified 'result' to the value of the specified
        // 'index'th entry in this Element and return 0. An error
        // (non-zero) is returned if the DataType of this Element
        // cannot be converted to the type of 'result' or if 'index'
        // is greater than the current size of this Element
        // (numValues()).

    bool getValueAsBool(size_t index=0) const;
        // Returns the specified 'index'th entry in the Element as a
        // bool. An exception is thrown if the DataType of this
        // Element cannot be converted to bool or if 'index' is
        // greater than the current size of this Element
        // (numValues()).

    char getValueAsChar(size_t index=0) const;
        // Returns the specified 'index'th entry in the Element as a
        // char. An exception is thrown if the DataType of this
        // Element cannot be converted to char or if 'index' is
        // greater than the current size of this Element
        // (numValues()).

    Int32 getValueAsInt32(size_t index=0) const;
        // Returns the specified 'index'th entry in the Element as a
        // Int32. An exception is thrown if the DataType of this
        // Element cannot be converted to Int32 or if 'index' is
        // greater than the current size of this Element
        // (numValues()).

    Int64 getValueAsInt64(size_t index=0) const;
        // Returns the specified 'index'th entry in the Element as a
        // Int64. An exception is thrown if the DataType of this
        // Element cannot be converted to Int64 or if 'index' is
        // greater than the current size of this Element
        // (numValues()).

    Float32 getValueAsFloat32(size_t index=0) const;
        // Returns the specified 'index'th entry in the Element as a
        // Float32. An exception is thrown if the DataType of this
        // Element cannot be converted to Float32 or if 'index' is
        // greater than the current size of this Element
        // (numValues()).

    Float64 getValueAsFloat64(size_t index=0) const;
        // Returns the specified 'index'th entry in the Element as a
        // Float64. An exception is thrown if the DataType of this
        // Element cannot be converted to Float64 or if 'index' is
        // greater than the current size of this Element
        // (numValues()).

    Datetime getValueAsDatetime(size_t index=0) const;
        // Returns the specified 'index'th entry in the Element as a
        // Datetime. An exception is thrown if the DataType of this
        // Element cannot be converted to Datetime or if 'index' is
        // greater than the current size of this Element
        // (numValues()).

    const char* getValueAsString(size_t index=0) const;
        // Returns the specified 'index'th entry in the Element as a
        // pointer to a null-terminated string. An exception is thrown
        // if the DataType of this Element cannot be converted to a
        // null-terminated string or if 'index' is
        // greater than the current size of this Element
        // (numValues()). The pointer returned remains valid until
        // this Element is destroyed.

    Element getValueAsElement(size_t index=0) const;
        // Returns the specified 'index'th entry in the Element as an
        // Element. An exception is thrown if the DataType of this
        // Element cannot be converted to an Element or if 'index' is
        // greater than the current size of this Element
        // (numValues()).

    Name getValueAsName(size_t index=0) const;
        // Returns the specified 'index'th entry in the Element as a
        // Name. An exception is thrown if the DataType of this
        // Element cannot be converted to Name or if 'index' is
        // greater than the current size of this Element
        // (numValues()).

    int getElement(Element* result, const char *name) const;
        // If this Element is either a sequence or a choice and
        // contains a valid Element identified by the specified 'name'
        // return that Element at the location specified by 'result'
        // and return 0. Otherwise, return an error (non-zero).
        // 'name' must be a valid pointer.

    int getElement(Element* result, const Name& name) const;
        // If this Element is either a sequence or a choice and
        // contains a valid Element identified by the specified 'name'
        // return that Element at the location specified by 'result'
        // and return 0. Otherwise, return an error (non-zero).
        // 'name' cannot be an uninitialized Name.

    int getElement(Element *result, size_t position) const;
        // If this Element is either a sequence or a choice and
        // numElements() is greater than the specified 'position'
        // return the 'position'th element at the location specified
        // by 'result' and return 0. Otherwise, return an error
        // (non-zero).

    Element getElement(size_t position) const;
        // If this Element is either a sequence or a choice and
        // numElements() is greater than the specified 'position'
        // return the 'position'th element. Otherwise, an exception is
        // thrown.

    Element getElement(const Name& name) const;
        // If this Element is either a sequence or a choice and
        // contains a valid Element identified by the specified 'name'
        // return that Element. Otherwise, an exception is thrown.
        // 'name' cannot be an uninitialized Name.

    Element getElement(const char* name) const;
        // If this Element is either a sequence or a choice and
        // contains a valid Element identified by the specified 'name'
        // return that Element. Otherwise, an exception is thrown.
        // 'name' must be a valid pointer.

    bool getElementAsBool(const char* name) const;
        // If this Element is either a sequence or a choice and
        // contains a valid Element identified by the specified 'name'
        // which can be returned as a bool then its value is
        // returned. Otherwise an exception is thrown.
        // 'name' must be a valid pointer.

    bool getElementAsBool(const Name& name) const;
        // If this Element is either a sequence or a choice and
        // contains a valid Element identified by the specified 'name'
        // which can be returned as a bool then its value is
        // returned. Otherwise an exception is thrown.
        // 'name' cannot be an uninitialized Name.

    char getElementAsChar(const char* name) const;
        // If this Element is either a sequence or a choice and
        // contains a valid Element identified by the specified 'name'
        // which can be returned as a char then its value is
        // returned. Otherwise an exception is thrown.
        // 'name' must be a valid pointer.

    char getElementAsChar(const Name& name) const;
        // If this Element is either a sequence or a choice and
        // contains a valid Element identified by the specified 'name'
        // which can be returned as a char then its value is
        // returned. Otherwise an exception is thrown.
        // 'name' cannot be an uninitialized Name.

    Int32 getElementAsInt32(const char* name) const;
        // If this Element is either a sequence or a choice and
        // contains a valid Element identified by the specified 'name'
        // which can be returned as an Int32 then its value is
        // returned. Otherwise an exception is thrown.
        // 'name' must be a valid pointer.

    Int32 getElementAsInt32(const Name& name) const;
        // If this Element is either a sequence or a choice and
        // contains a valid Element identified by the specified 'name'
        // which can be returned as an Int32 then its value is
        // returned. Otherwise an exception is thrown.
        // 'name' cannot be an uninitialized Name.

    Int64 getElementAsInt64(const char* name) const;
        // If this Element is either a sequence or a choice and
        // contains a valid Element identified by the specified 'name'
        // which can be returned as an Int64 then its value is
        // returned. Otherwise an exception is thrown.
        // 'name' must be a valid pointer.

    Int64 getElementAsInt64(const Name& name) const;
        // If this Element is either a sequence or a choice and
        // contains a valid Element identified by the specified 'name'
        // which can be returned as an Int64 then its value is
        // returned. Otherwise an exception is thrown.
        // 'name' cannot be an uninitialized Name.

    Float32 getElementAsFloat32(const char* name) const;
        // If this Element is either a sequence or a choice and
        // contains a valid Element identified by the specified 'name'
        // which can be returned as a Float32 then its value is
        // returned. Otherwise an exception is thrown.
        // 'name' must be a valid pointer.

    Float32 getElementAsFloat32(const Name& name) const;
        // If this Element is either a sequence or a choice and
        // contains a valid Element identified by the specified 'name'
        // which can be returned as a Float32 then its value is
        // returned. Otherwise an exception is thrown.
        // 'name' cannot be an uninitialized Name.

    Float64 getElementAsFloat64(const char* name) const;
        // If this Element is either a sequence or a choice and
        // contains a valid Element identified by the specified 'name'
        // which can be returned as a Float64 then its value is
        // returned. Otherwise an exception is thrown.
        // 'name' must be a valid pointer.

    Float64 getElementAsFloat64(const Name& name) const;
        // If this Element is either a sequence or a choice and
        // contains a valid Element identified by the specified 'name'
        // which can be returned as a Float64 then its value is
        // returned. Otherwise an exception is thrown.
        // 'name' cannot be an uninitialized Name.

    Datetime getElementAsDatetime(const char* name) const;
        // If this Element is either a sequence or a choice and
        // contains a valid Element identified by the specified 'name'
        // which can be returned as a Datetime then its value is
        // returned. Otherwise an exception is thrown.
        // 'name' must be a valid pointer.

    Datetime getElementAsDatetime(const Name& name) const;
        // If this Element is either a sequence or a choice and
        // contains a valid Element identified by the specified 'name'
        // which can be returned as Datetime then its value is
        // returned. Otherwise an exception is thrown.
        // 'name' cannot be an uninitialized Name.

    const char* getElementAsString(const char* name) const;
        // If this Element is either a sequence or a choice and
        // contains a valid Element identified by the specified 'name'
        // which can be returned as a null-terminated string then a
        // pointer to a string is returned. The pointer remains valid
        // until this Element is destroyed. Otherwise an exception is
        // thrown.
        // 'name' must be a valid pointer.

    const char* getElementAsString(const Name& name) const;
        // If this Element is either a sequence or a choice and
        // contains a valid Element identified by the specified 'name'
        // which can be returned as a null-terminated string then a
        // pointer to a string is returned. The pointer remains valid
        // until this Element is destroyed. Otherwise an exception is
        // thrown.
        // 'name' cannot be an uninitialized Name.

    Name getElementAsName(const char* name) const;
        // If this Element is either a sequence or a choice and
        // contains a valid Element identified by the specified 'name'
        // which can be returned as a Name then its value is
        // returned. Otherwise an exception is thrown.
        // 'name' must be a valid pointer.

    Name getElementAsName(const Name& name) const;
        // If this Element is either a sequence or a choice and
        // contains a valid Element identified by the specified 'name'
        // which can be returned as a Name then its value is
        // returned. Otherwise an exception is thrown.
        // 'name' cannot be an uninitialized Name.

    Element getChoice() const;
        // Return the selection name of this element if this element is a
        // "choice" element.  Throw exception otherwise.

    const blpapi_Element_t* handle() const;

    std::ostream& print(std::ostream& stream,
                        int level=0,
                        int spacesPerLevel=4) const;
        // Format this Element to the specified output 'stream' at the
        // (absolute value of) the optionally specified indentation
        // 'level' and return a reference to 'stream'. If 'level' is
        // specified, optionally specify 'spacesPerLevel', the number
        // of spaces per indentation level for this and all of its
        // nested objects. If 'level' is negative, suppress indentation
        // of the first line. If 'spacesPerLevel' is negative, format
        // the entire output on one line, suppressing all but the
        // initial indentation (as governed by 'level').
};

// FREE OPERATORS
std::ostream& operator<<(std::ostream& stream, const Element &element);
    // Write the value of the specified 'element' object to the specified
    // output 'stream' in a single-line format, and return a reference to
    // 'stream'.  If 'stream' is not valid on entry, this operation has no
    // effect.  Note that this human-readable format is not fully specified,
    // can change without notice, and is logically equivalent to:
    //..
    //  print(stream, 0, -1);
    //..

// ============================================================================
//                      INLINE AND TEMPLATE FUNCTION IMPLEMENTATIONS
// ============================================================================

                            // -------------
                            // class Element
                            // -------------

inline
Element::Element()
: d_handle_p(0)
{
}

inline
Element::Element(blpapi_Element_t *handle)
: d_handle_p(handle)
{
}

inline
void Element::rebind(blpapi_Element_t *element)
{
    d_handle_p = element;
}


inline
void Element::setElement(const char* name, bool value)
{
    ExceptionUtil::throwOnError(
            blpapi_Element_setElementBool(d_handle_p, name, 0 , value ? 1 :
                0));
}

inline
void Element::setElement(const char* name, char value)
{
    ExceptionUtil::throwOnError(
            blpapi_Element_setElementChar(d_handle_p, name, 0 , value));
}

inline
void Element::setElement(const char* name, Int32 value)
{
    ExceptionUtil::throwOnError(
            blpapi_Element_setElementInt32(d_handle_p, name, 0 , value));
}

inline
void Element::setElement(const char* name, Int64 value)
{
    ExceptionUtil::throwOnError(
            blpapi_Element_setElementInt64(d_handle_p, name, 0 , value));
}

inline
void Element::setElement(const char* name, Float32 value)
{
    ExceptionUtil::throwOnError(
        blpapi_Element_setElementFloat32(d_handle_p, name, 0 , value));
}

inline
void Element::setElement(const char* name, Float64 value)
{
    ExceptionUtil::throwOnError(
        blpapi_Element_setElementFloat64(d_handle_p, name, 0 , value));
}

inline
void Element::setElement(const char* name, const Datetime& value)
{
    ExceptionUtil::throwOnError(
        BLPAPI_CALL_ELEMENT_SETELEMENTHIGHPRECISIONDATETIME(
                                                d_handle_p,
                                                name,
                                                0,
                                                &value.rawHighPrecisionValue())
    );
}

inline
void Element::setElement(const char* name, const char* value)
{
    ExceptionUtil::throwOnError(
            blpapi_Element_setElementString(d_handle_p, name, 0 , value));
}

inline
void Element::setElement(const char* name, const Name& value)
{
    ExceptionUtil::throwOnError(
            blpapi_Element_setElementFromName(d_handle_p,
                                              name,
                                              0,
                                              value.impl()));
}

inline
void Element::setElement(const Name& name, bool value)
{
    ExceptionUtil::throwOnError(
            blpapi_Element_setElementBool(d_handle_p,
                                          0,
                                          name.impl(),
                                          value ? 1 : 0));
}

inline
void Element::setElement(const Name& name, char value)
{
    ExceptionUtil::throwOnError(
            blpapi_Element_setElementChar(d_handle_p, 0, name.impl(), value));
}

inline
void Element::setElement(const Name& name, Int32 value)
{
    ExceptionUtil::throwOnError(
            blpapi_Element_setElementInt32(d_handle_p, 0, name.impl(), value));
}

inline
void Element::setElement(const Name& name, Int64 value)
{
    ExceptionUtil::throwOnError(
            blpapi_Element_setElementInt64(d_handle_p, 0, name.impl(), value));
}

inline
void Element::setElement(const Name& name, Float32 value)
{
    ExceptionUtil::throwOnError(
        blpapi_Element_setElementFloat32(d_handle_p, 0, name.impl(), value));
}

inline
void Element::setElement(const Name& name, Float64 value)
{
    ExceptionUtil::throwOnError(
        blpapi_Element_setElementFloat64(d_handle_p, 0, name.impl(), value));
}

inline
void Element::setElement(const Name& name, const Datetime& value)
{
    ExceptionUtil::throwOnError(
        BLPAPI_CALL_ELEMENT_SETELEMENTHIGHPRECISIONDATETIME(
                                                d_handle_p,
                                                0,
                                                name.impl(),
                                                &value.rawHighPrecisionValue())
    );
}

inline
void Element::setElement(const Name& name, const char* value)
{
    ExceptionUtil::throwOnError(
            blpapi_Element_setElementString(d_handle_p,
                                            0,
                                            name.impl(),
                                            value));
}

inline
void Element::setElement(const Name& name, const Name& value)
{
    ExceptionUtil::throwOnError(
            blpapi_Element_setElementFromName(d_handle_p,
                                              0,
                                              name.impl(),
                                              value.impl()));
}

inline
void Element::setValue(bool value, size_t index)
{
    ExceptionUtil::throwOnError(
            blpapi_Element_setValueBool(d_handle_p, value, index));
}

inline
void Element::setValue(char value, size_t index)
{
    ExceptionUtil::throwOnError(
            blpapi_Element_setValueChar(d_handle_p, value, index));
}

inline
void Element::setValue(Int32 value, size_t index)
{
    ExceptionUtil::throwOnError(
            blpapi_Element_setValueInt32(d_handle_p, value, index));
}

inline
void Element::setValue(Int64 value, size_t index)
{
    ExceptionUtil::throwOnError(
            blpapi_Element_setValueInt64(d_handle_p, value, index));
}

inline
void Element::setValue(Float32 value, size_t index)
{
    ExceptionUtil::throwOnError(
        blpapi_Element_setValueFloat32(d_handle_p, value, index));
}

inline
void Element::setValue(Float64 value, size_t index)
{
    ExceptionUtil::throwOnError(
        blpapi_Element_setValueFloat64(d_handle_p, value, index));
}

inline
void Element::setValue(const Datetime& value, size_t index)
{
    ExceptionUtil::throwOnError(
        BLPAPI_CALL_ELEMENT_SETVALUEHIGHPRECISIONDATETIME(
                                                d_handle_p,
                                                &value.rawHighPrecisionValue(),
                                                index)
    );
}

inline
void Element::setValue(const char* value, size_t index)
{
    ExceptionUtil::throwOnError(
        blpapi_Element_setValueString(d_handle_p, value, index));
}

inline
void Element::setValue(const Name& valueName, size_t index)
{
    ExceptionUtil::throwOnError(
        blpapi_Element_setValueFromName(d_handle_p, valueName.impl(), index));
}

inline
void Element::appendValue(bool value)
{
    ExceptionUtil::throwOnError(
            blpapi_Element_setValueBool(d_handle_p,
                                        value,
                                        BLPAPI_ELEMENT_INDEX_END));
}

inline
void Element::appendValue(char value)
{
    ExceptionUtil::throwOnError(
            blpapi_Element_setValueChar(d_handle_p,
                                        value,
                                        BLPAPI_ELEMENT_INDEX_END));
}

inline
void Element::appendValue(Int32 value)
{
    ExceptionUtil::throwOnError(
            blpapi_Element_setValueInt32(d_handle_p,
                                         value,
                                         BLPAPI_ELEMENT_INDEX_END));
}

inline
void Element::appendValue(Int64 value)
{
    ExceptionUtil::throwOnError(
            blpapi_Element_setValueInt64(d_handle_p,
                                         value,
                                         BLPAPI_ELEMENT_INDEX_END));
}

inline
void Element::appendValue(Float32 value)
{
    ExceptionUtil::throwOnError(
        blpapi_Element_setValueFloat32(d_handle_p,
                                       value,
                                       BLPAPI_ELEMENT_INDEX_END));
}

inline
void Element::appendValue(Float64 value)
{
    ExceptionUtil::throwOnError(
        blpapi_Element_setValueFloat64(d_handle_p,
                                       value,
                                       BLPAPI_ELEMENT_INDEX_END));
}

inline
void Element::appendValue(const Datetime& value)
{
    ExceptionUtil::throwOnError(
        BLPAPI_CALL_ELEMENT_SETVALUEHIGHPRECISIONDATETIME(
                                                d_handle_p,
                                                &value.rawHighPrecisionValue(),
                                                BLPAPI_ELEMENT_INDEX_END)
    );
}

inline
void Element::appendValue(const char* value)
{
    ExceptionUtil::throwOnError(
        blpapi_Element_setValueString(d_handle_p,
                                      value,
                                      BLPAPI_ELEMENT_INDEX_END));
}

inline
void Element::appendValue(const Name& valueName)
{
    ExceptionUtil::throwOnError(
        blpapi_Element_setValueFromName(d_handle_p,
                                        valueName.impl(),
                                        BLPAPI_ELEMENT_INDEX_END));
}

inline
Element Element::appendElement()
{
    blpapi_Element_t *appendedElement;
    ExceptionUtil::throwOnError(
        blpapi_Element_appendElement(d_handle_p, &appendedElement));
    return Element(appendedElement);
}

inline
Element Element::setChoice(const char* selectionName)
{
    blpapi_Element_t *resultElement;
    ExceptionUtil::throwOnError(
        blpapi_Element_setChoice(d_handle_p,
                                 &resultElement,
                                 selectionName,
                                 0,
                                 0));
    return Element(resultElement);
}

inline
Element Element::setChoice(const Name& selectionName)
{
    blpapi_Element_t *resultElement;
    ExceptionUtil::throwOnError(
        blpapi_Element_setChoice(d_handle_p,
                                 &resultElement,
                                 0,
                                 selectionName.impl(),
                                 0));
    return Element(resultElement);
}

inline
blpapi_Element_t* Element::handle()
{
    return d_handle_p;
}

inline
Name Element::name() const
{
    return blpapi_Element_name(d_handle_p);
}

inline
int Element::getElement(Element* element, const char *nameString) const
{
    blpapi_Element_t *fldt;
    int rc = blpapi_Element_getElement(d_handle_p, &fldt, nameString, 0);
    if (!rc) {
        element->rebind(fldt);
    }

    return rc;
}

inline
int Element::getElement(Element* element, const Name& name) const
{
    blpapi_Element_t *fldt;
    int rc = blpapi_Element_getElement(d_handle_p, &fldt, 0, name.impl());
    if (!rc) {
        element->rebind(fldt);
    }

    return rc;
}

inline
int Element::getElement(Element *element, size_t position) const
{
    blpapi_Element_t *fldt;
    int rc = blpapi_Element_getElementAt(d_handle_p, &fldt, position);
    if (!rc) {
        element->rebind(fldt);
    }

    return rc;
}

inline
int Element::datatype() const
{
    return blpapi_Element_datatype(d_handle_p);
}

inline
bool Element::isComplexType() const
{
    return blpapi_Element_isComplexType(d_handle_p) ? true : false;
}

inline
bool Element::isArray() const
{
    return blpapi_Element_isArray(d_handle_p) ? true : false;
}

inline
bool Element::isNull() const
{
    return blpapi_Element_isNull(d_handle_p) ? true : false;
}

inline
bool Element::isReadOnly() const
{
    return blpapi_Element_isReadOnly(d_handle_p) ? true : false;
}

inline
SchemaElementDefinition Element::elementDefinition() const
{
    return blpapi_Element_definition(d_handle_p);
}

inline
size_t Element::numValues() const
{
    return blpapi_Element_numValues(d_handle_p);
}

inline
size_t Element::numElements() const
{
    return blpapi_Element_numElements(d_handle_p);
}

inline
bool Element::isValid() const
{
    return d_handle_p  ? true : false;
}

inline
bool Element::isNullValue(size_t position) const
{
    int rc = blpapi_Element_isNullValue(d_handle_p, position);
    if (rc != 0 && rc != 1) {
        ExceptionUtil::throwOnError(rc);
    }
    return rc ? true : false;
}

inline
bool Element::hasElement(const char* nameString,
                                bool excludeNullElements) const
{
    if (excludeNullElements) {
        return (blpapi_Element_hasElementEx(d_handle_p,
              nameString, 0, excludeNullElements, 0) ? true : false);
    }
    return blpapi_Element_hasElement(d_handle_p, nameString, 0) ? true : false;
}

inline
bool Element::hasElement(const Name& name,
                                bool excludeNullElements) const
{
    if (excludeNullElements) {
        return (blpapi_Element_hasElementEx(d_handle_p, 0,
                name.impl(), excludeNullElements, 0) ? true : false);
    }
    return blpapi_Element_hasElement(d_handle_p, 0, name.impl())
                                     ? true
                                     : false;
}

inline
Element Element::getElement(const Name& name) const
{
    blpapi_Element_t *fldt;
    ExceptionUtil::throwOnError(
                blpapi_Element_getElement(
                    d_handle_p,
                    &fldt,
                    0,
                    name.impl()));
    return Element(fldt);
}

inline
Element Element::getElement(const char* name) const
{
    blpapi_Element_t *fldt;
    ExceptionUtil::throwOnError(
            blpapi_Element_getElement(
                d_handle_p,
                &fldt,
                name,
                0));
    return Element(fldt);
}

inline
Element Element::getElement(size_t position) const
{
    blpapi_Element_t *element;
    ExceptionUtil::throwOnError(
            blpapi_Element_getElementAt(
                d_handle_p,
                &element,
                position));
    return Element(element);
}

inline
int Element::getValueAs(bool* buffer, size_t index) const
{
    blpapi_Bool_t tmp=false;

    int res = blpapi_Element_getValueAsBool(d_handle_p, &tmp, index);
    *buffer = tmp ? true : false;
    return res;
}

inline
int Element::getValueAs(char* buffer, size_t index) const
{
    return blpapi_Element_getValueAsChar(d_handle_p, buffer, index);
}

inline
int Element::getValueAs(Int32* buffer, size_t index) const
{
    return blpapi_Element_getValueAsInt32(d_handle_p, buffer, index);
}

inline
int Element::getValueAs(Int64* buffer, size_t index) const
{
    return blpapi_Element_getValueAsInt64(d_handle_p,buffer, index);
}

inline
int Element::getValueAs(Float32* buffer, size_t index) const
{
    return blpapi_Element_getValueAsFloat32(d_handle_p,buffer, index);
}

inline
int Element::getValueAs(Float64* buffer, size_t index) const
{
    return blpapi_Element_getValueAsFloat64(d_handle_p,buffer, index);
}

inline
int Element::getValueAs(Datetime* buffer, size_t index) const
{
    BLPAPI_CALL_ELEMENT_GETVALUEASHIGHPRECISIONDATETIME(d_handle_p,
                                                        buffer,
                                                        index);
}

inline
int Element::getValueAs(std::string* result, size_t index) const
{
    const char* buffer;
    int rc = blpapi_Element_getValueAsString(d_handle_p,&buffer, index);
    if (!rc) {
        *result = buffer;
    }
    return rc;
}

inline
int Element::getValueAs(Element *buffer, size_t index) const
{
    return blpapi_Element_getValueAsElement(
            d_handle_p,
            &buffer->d_handle_p,
            index);
}

inline
int Element::getValueAs(Name *buffer, size_t index) const
{
    blpapi_Name_t* tmpName;
    int res = blpapi_Element_getValueAsName(d_handle_p,
                                            &tmpName,
                                            index);
    if (!res) {
        *buffer = Name(tmpName);
    }
    return res;
}

inline
bool Element::getValueAsBool(size_t index) const
{
    bool value;
    ExceptionUtil::throwOnError(getValueAs(&value, index));
    return value;
}

inline
char Element::getValueAsChar(size_t index) const
{
    char value;
    ExceptionUtil::throwOnError(getValueAs(&value, index));
    return value;
}

inline
Int32 Element::getValueAsInt32(size_t index) const
{
    Int32 value;
    ExceptionUtil::throwOnError(getValueAs(&value, index));
    return value;
}

inline
Int64 Element::getValueAsInt64(size_t index) const
{
    Int64 value;
    ExceptionUtil::throwOnError(getValueAs(&value, index));
    return value;
}

inline
Float32 Element::getValueAsFloat32(size_t index) const
{
    Float32 value;
    ExceptionUtil::throwOnError(getValueAs(&value, index));
    return value;
}

inline
Float64 Element::getValueAsFloat64(size_t index) const
{
    Float64 value;
    ExceptionUtil::throwOnError(getValueAs(&value, index));
    return value;
}

inline
Datetime Element::getValueAsDatetime(size_t index) const
{
    Datetime value;
    ExceptionUtil::throwOnError(getValueAs(&value, index));
    return value;
}

inline
const char* Element::getValueAsString(size_t index) const
{
    const char* tmpStringBuffer;
    ExceptionUtil::throwOnError(blpapi_Element_getValueAsString(
                                d_handle_p,
                                &tmpStringBuffer,
                                index));
    return tmpStringBuffer;
}

inline
Element Element::getValueAsElement(size_t index) const
{
    blpapi_Element_t *element;
    ExceptionUtil::throwOnError(blpapi_Element_getValueAsElement(d_handle_p,
                                                                 &element,
                                                                 index));
    return Element(element);
}

inline
Name Element::getValueAsName(size_t index) const
{
    blpapi_Name_t *name;
    ExceptionUtil::throwOnError(blpapi_Element_getValueAsName(d_handle_p,
                                                              &name,
                                                              index));
    return name;
}

inline
Element Element::getChoice() const
{
    blpapi_Element_t *element;
    ExceptionUtil::throwOnError(blpapi_Element_getChoice(d_handle_p,
                                                         &element));
    return Element(element);
}

inline
bool Element::getElementAsBool(const char* name) const
{
    return getElement(name).getValueAsBool();
}

inline
bool Element::getElementAsBool(const Name& name) const
{
    return getElement(name).getValueAsBool();
}

inline
char Element::getElementAsChar(const char* name) const
{
    return getElement(name).getValueAsChar();
}

inline
char Element::getElementAsChar(const Name& name) const
{
    return getElement(name).getValueAsChar();
}

inline
Int32 Element::getElementAsInt32(const char* name) const
{
    return getElement(name).getValueAsInt32();
}

inline
Int32 Element::getElementAsInt32(const Name& name) const
{
    return getElement(name).getValueAsInt32();
}

inline
Int64 Element::getElementAsInt64(const char* name) const
{
    return getElement(name).getValueAsInt64();
}

inline
Int64 Element::getElementAsInt64(const Name& name) const
{
    return getElement(name).getValueAsInt64();
}

inline
Float32 Element::getElementAsFloat32(const char* name) const
{
    return getElement(name).getValueAsFloat32();
}

inline
Float32 Element::getElementAsFloat32(const Name& name) const
{
    return getElement(name).getValueAsFloat32();
}

inline
Float64 Element::getElementAsFloat64(const char* name) const
{
    return getElement(name).getValueAsFloat64();
}

inline
Float64 Element::getElementAsFloat64(const Name& name) const
{
    return getElement(name).getValueAsFloat64();
}

inline
Datetime Element::getElementAsDatetime(const char* name) const
{
    return getElement(name).getValueAsDatetime();
}

inline
Datetime Element::getElementAsDatetime(const Name& name) const
{
    return getElement(name).getValueAsDatetime();
}

inline
const char* Element::getElementAsString(const char* name) const
{
    return getElement(name).getValueAsString();
}

inline
const char* Element::getElementAsString(const Name& name) const
{
    return getElement(name).getValueAsString();
}

inline
Name Element::getElementAsName(const char* name) const
{
    return getElement(name).getValueAsName();
}

inline
Name Element::getElementAsName(const Name& name) const
{
    return getElement(name).getValueAsName();
}

inline
const blpapi_Element_t* Element::handle() const
{
    return d_handle_p;
}

inline
std::ostream& Element::print(
        std::ostream& stream,
        int level,
        int spacesPerLevel) const
{
    blpapi_Element_print(
            d_handle_p, OstreamWriter,  &stream, level, spacesPerLevel);
    return stream;
}

inline
std::ostream& operator<<(std::ostream& stream, const Element& element)
{
    element.print(stream, 0, -1);
    return stream;
}

}  // close namespace blpapi
}  // close namespace BloombergLP

#endif // #ifdef __cplusplus
#endif // #ifndef INCLUDED_BLPAPI_ELEMENT
