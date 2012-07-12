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
// blpapi_datetime.h                                                  -*-C++-*-
#ifndef INCLUDED_BLPAPI_DATETIME
#define INCLUDED_BLPAPI_DATETIME
//@PURPOSE: Represents a date and/or time.
//
//@CLASSES:
//      blpapi::Datetime: C++ class to represents a date and/or time value.
//   blpapi_Datetime_tag: A C style struct represents a date and/or time value.
// blpapi::DatetimeParts: Bit flags and masks to access parts in date/time.
//
//@DESCRIPTION: This file provides a C and C++ data type to represent a
// date and/or time value.

#ifndef INCLUDED_BLPAPI_DEFS
#include <blpapi_defs.h>
#endif

#ifndef INCLUDED_BLPAPI_EXCEPTION
#include <blpapi_exception.h>
#endif

#ifndef INCLUDED_BLPAPI_STREAMPROXY
#include <blpapi_streamproxy.h>
#endif

#ifndef INCLUDED_BLPAPI_TYPES
#include <blpapi_types.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct blpapi_Datetime_tag {
    blpapi_UChar_t  parts;        // bitmask of date/time parts that are set
    blpapi_UChar_t  hours;
    blpapi_UChar_t  minutes;
    blpapi_UChar_t  seconds;
    blpapi_UInt16_t milliSeconds;
    blpapi_UChar_t  month;
    blpapi_UChar_t  day;
    blpapi_UInt16_t year;
    blpapi_Int16_t  offset;
};


typedef struct blpapi_Datetime_tag blpapi_Datetime_t;

BLPAPI_EXPORT
int blpapi_Datetime_compare(blpapi_Datetime_t lhs, blpapi_Datetime_t rhs);


BLPAPI_EXPORT
int blpapi_Datetime_print(const blpapi_Datetime_t *datetime,
                          blpapi_StreamWriter_t streamWriter,
                          void *stream,
                          int level,
                          int spacesPerLevel);

#ifdef __cplusplus
} // extern "C"

#ifndef INCLUDED_CASSERT
#include <cassert>
#define INCLUDED_CASSERT
#endif

#ifndef INCLUDED_CSTRING
#include <cstring> // for std::memset
#define INCLUDED_CSTRING
#endif

#ifndef INCLUDED_IOSFWD
#include <iosfwd>
#define INCLUDED_IOSFWD
#endif

namespace BloombergLP {
namespace blpapi {

                         // ====================
                         // struct DatetimeParts
                         // ====================

struct DatetimeParts {
    // Bit flags and masks used to determine which parts of a Datetime are
    // valid.

    enum Value {
          YEAR         = BLPAPI_DATETIME_YEAR_PART
        , MONTH        = BLPAPI_DATETIME_MONTH_PART
        , DAY          = BLPAPI_DATETIME_DAY_PART
        , OFFSET       = BLPAPI_DATETIME_OFFSET_PART
        , HOURS        = BLPAPI_DATETIME_HOURS_PART
        , MINUTES      = BLPAPI_DATETIME_MINUTES_PART
        , SECONDS      = BLPAPI_DATETIME_SECONDS_PART
        , MILLISECONDS = BLPAPI_DATETIME_MILLISECONDS_PART
        , DATE         = BLPAPI_DATETIME_DATE_PART
        , TIME         = BLPAPI_DATETIME_TIME_PART
        , TIMEMILLI    = BLPAPI_DATETIME_TIMEMILLI_PART
    };
};
                         // ==============
                         // class Datetime
                         // ==============

class Datetime {
    // Represents a date and/or time.
    //
    // Datetime can represent a date and/or a time or any combination
    // of the components of a date and time. The value is represented
    // as eight parts which can be set or queried independently.
    //
    // These parts are: year; month; day (of month); hour; minute;
    // second; milliseconds and offset (of time zone from GMT in
    // minutes).
    //
    // Methods are provided to set and query the parts individually
    // and in groups. For example, setDate() and setTime(). It is also
    // possible to determine which parts of the Datetime have been
    // set.

    blpapi_Datetime_t d_value;

    static bool isLeapYear(int year);

    static bool isValidDate(int year, int month, int day);

    static bool isValidTime(int hours,
                            int minutes,
                            int seconds,
                            int milliSeconds);

  public:
    Datetime();
        // Construct a 'Datetime' object with each part initialized to
        // an unset state.

    Datetime(const Datetime& original);
        // Copy constructor.

    Datetime(const blpapi_Datetime_t& rawValue);

    // MANIPULATORS

    Datetime& operator=(const Datetime& rhs);
        // Assignment operator.

    void setDate(unsigned year,
                 unsigned month,
                 unsigned day);
        // Set the date portions of the 'Datetime' object to the
        // specified 'day', 'month', and 'year' respectively.
        // The behavior is undefined unless the combined representation
        // comprises a date for which 'isValidDate' returns true.

    void setTime(unsigned hours,
                 unsigned minutes,
                 unsigned seconds);
        // Set the time of the 'Datetime' object to the specified
        // 'hours', 'minutes', and 'seconds' respectively. The
        // millisecond value will be set to 0.
        // The behavior is undefined unless the combined representation
        // comprises a time for which 'isValidTime' with 'millisecond' set as 0
        // returns true.


    void setTime(unsigned hours,
                 unsigned minutes,
                 unsigned seconds,
                 unsigned milliSeconds);
        // Set the time of the 'Datetime' object to the specified 'hours',
        // 'minutes', 'seconds' and 'milliSeconds' respectively.
        // The behavior is undefined unless the combined representation
        // comprises a time for which 'isValidTime' returns true.

    void setOffset(short value);
        // Set the offset value of this 'Datetime' object to the specified
        // 'value'.

    void setMonth(unsigned value);
        // Set the month value of this 'Datetime' object to the specified
        // 'value'.
        // The behavior is undefined unless '1 <= value <= 12'.


    void setDay(unsigned value);
        // Set the day value of this 'Datetime' object to the specified
        // 'value'.
        // The behavior is undefined unless '1 <= value <= 31'.

    void setYear(unsigned value);
        // Set the year value of this 'Datetime' object to the
        // specified 'value'.
        // The behavior is undefined unless '1 <= value <= 9999'.

    void setHours(unsigned value);
        // Set the hours value of this 'Datetime' object to the
        // specified 'value'.
        // The behavior is undefined unless '0 <= value <= 24'.


    void setMinutes(unsigned value);
        // Set the minutes value of this 'Datetime' object to the
        // specified 'value'.
        // The behavior is undefined unless '0 <= value <= 59'.

    void setSeconds(unsigned value);
        // Set the seconds value of this 'Datetime' object to the
        // specified 'value'.
        // The behavior is undefined unless '0 <= value <= 59'.

    void setMilliseconds(unsigned value);
        // Set the milliseconds value of this 'Datetime' object to the
        // specified 'value'.
        // The behavior is undefined unless '0 <= value <= 999'.

    blpapi_Datetime_t& rawValue();

    // ACCESSORS
    short offset() const;
        // Return the offset value of this 'Datetime' object. The
        // result is undefined unless object has a offset value set.

    unsigned month() const;
        // Return the month value of this 'Datetime' object. The
        // result is undefined unless object has a month value set.

    unsigned day() const;
        // Return the day value of this 'Datetime' object. The result
        // is undefined unless object has a day value set.

    unsigned year() const;
        // Return the year value of this 'Datetime' object. The result
        // is undefined unless object has a year value set.

    unsigned hours() const;
        // Return the hours value of this 'Datetime' object. The
        // result is undefined unless object has a hours value set.

    unsigned minutes() const;
        // Return the minutes value of this 'Datetime' object. The
        // result is undefined unless object has a minutes value set.

    unsigned seconds() const;
        // Return the seconds value of this 'Datetime' object. The
        // result is undefined unless object has a seconds value set.

    unsigned milliSeconds() const;
        // Return the milliseconds value of this 'Datetime'
        // object. The result is undefined unless object has a
        // milliseconds value set.

    const blpapi_Datetime_t& rawValue() const;

    bool hasParts(unsigned parts) const;
        // Return true if this 'Datetime' object has all of the
        // specified 'parts' set. The 'parts' parameter must be
        // constructed by or'ing together values from the
        // DateTimeParts enum.

    unsigned parts() const;
        // Return a bitmask of all parts that are set in this
        // 'Datetime' object. This can be compared to the values in
        // the DatetimeParts enum using bitwise operations.

    bool isValid() const;
        // Check whether the value of this 'Datetime' is valid. The behaviour
        // is undefined unless this object represents a date (has YEAR, MONTH
        // and DAY part set) or time (has HOURS, MINUTES, SECONDS and
        // MILLISECONDS part set).

    std::ostream& print(std::ostream& stream,
                        int           level = 0,
                        int           spacesPerLevel = 4) const;
        // Write the value of this object to the specified output 'stream' in
        // a human-readable format, and return a reference to 'stream'.
        // Optionally specify an initial indentation 'level', whose absolute
        // value is incremented recursively for nested objects.  If 'level' is
        // specified, optionally specify 'spacesPerLevel', whose absolute
        // value indicates the number of spaces per indentation level for this
        // and all of its nested objects.  If 'level' is negative, suppress
        // indentation of the first line.  If 'spacesPerLevel' is negative,
        // format the entire output on one line, suppressing all but the
        // initial indentation (as governed by 'level').  If 'stream' is not
        // valid on entry, this operation has no effect.  Note that this
        // human-readable format is not fully specified, and can change
        // without notice.

};

// FREE OPERATORS
bool operator==(const Datetime& lhs, const Datetime& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' Datetimes have the same
    // value, and 'false' otherwise.  Two Datetimes have the same value if they
    // have the same respective 'parts()' and the same value for the fields
    // for which the parts have been set.

bool operator!=(const Datetime& lhs, const Datetime& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' Datetimes do not have the
    // same value, and 'false' otherwise.  Two Datetimes do not have the same
    // value if they do not have the same 'parts()' and they do not have the
    // same values for any of fields for which the parts have been set.

bool operator<(const Datetime& lhs, const Datetime& rhs);
    // Return 'true' if the specified 'lhs' Datetime value is less than the
    // specified 'rhs' Datetime value, and 'false' otherwise. If the parts()
    // for 'lhs' and 'rhs' are not the same, an InvalidArgumentException
    // exception is thrown.

bool operator<=(const Datetime& lhs, const Datetime& rhs);
    // Return 'true' if the specified 'lhs' Datetime value is less than or
    // equal to the specified 'rhs' Datetime value, and 'false' otherwise. If
    // the parts() for 'lhs' and 'rhs' are not the same, an
    // InvalidArgumentException exception is thrown.

bool operator>(const Datetime& lhs, const Datetime& rhs);
    // Return 'true' if the specified 'lhs' Datetime value is greater than the
    // specified 'rhs' Datetime value, and 'false' otherwise. If the parts()
    // for 'lhs' and 'rhs' are not the same, an InvalidArgumentException
    // exception is thrown.

bool operator>=(const Datetime& lhs, const Datetime& rhs);
    // Return 'true' if the specified 'lhs' Datetime value is greater than or
    // equal to the specified 'rhs' Datetime value, and 'false' otherwise. If
    // the parts() for 'lhs' and 'rhs' are not the same, an
    // InvalidArgumentException exception is thrown.

std::ostream& operator<<(std::ostream& stream, const Datetime& datetime);
    // Write the value of the specified 'datetime' object to the specified
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

                            // --------------
                            // class Datetime
                            // --------------

inline
Datetime::Datetime(const Datetime& original)
: d_value(original.d_value)
{
}

inline
Datetime::Datetime()
{
    std::memset(&d_value, 0, sizeof(d_value));
    d_value.year = 1;
    d_value.month = 1;
    d_value.day = 1;
}

inline
Datetime::Datetime(const blpapi_Datetime_t& rawValue)
: d_value(rawValue)
{
}

inline
Datetime& Datetime::operator=(const Datetime& rhs)
{
    if (&rhs != this) {
        d_value = rhs.d_value;
    }
    return *this;
}

inline
void Datetime::setDate(unsigned year,
                       unsigned month,
                       unsigned day)
{
    d_value.day   = day;
    d_value.month = month;
    d_value.year  = year;
    d_value.parts |= DatetimeParts::DATE;
}


inline
void Datetime::setTime(unsigned hours,
                       unsigned minutes,
                       unsigned seconds)
{
    d_value.hours        = hours;
    d_value.minutes      = minutes;
    d_value.seconds      = seconds;
    d_value.milliSeconds = 0;
    d_value.parts        = (d_value.parts & ~DatetimeParts::TIMEMILLI) |
                           DatetimeParts::TIME;
}


inline
void Datetime::setTime(unsigned hours,
                       unsigned minutes,
                       unsigned seconds,
                       unsigned milliSeconds)
{
    d_value.hours        = hours;
    d_value.minutes      = minutes;
    d_value.seconds      = seconds;
    d_value.milliSeconds = milliSeconds;
    d_value.parts       |= DatetimeParts::TIMEMILLI;
}


inline
void Datetime::setOffset(short value)
{
    d_value.offset = value;
    d_value.parts |= DatetimeParts::OFFSET;
}


inline
void Datetime::setMonth(unsigned value)
{
    d_value.month  = value;
    d_value.parts |= DatetimeParts::MONTH;
}


inline
void Datetime::setDay(unsigned value)
{
    d_value.day    = value;
    d_value.parts |= DatetimeParts::DAY;
}

inline
void Datetime::setYear(unsigned value)
{
    d_value.year   = value;
    d_value.parts |= DatetimeParts::YEAR;
}


inline
void Datetime::setHours(unsigned value)
{
    d_value.hours  = value;
    d_value.parts |= DatetimeParts::HOURS;
}


inline
void Datetime::setMinutes(unsigned value)
{
    d_value.minutes  = value;
    d_value.parts   |= DatetimeParts::MINUTES;
}


inline
void Datetime::setSeconds(unsigned value)
{
    d_value.seconds  = value;
    d_value.parts   |= DatetimeParts::SECONDS;
}


inline
void Datetime::setMilliseconds(unsigned value)
{
    d_value.milliSeconds = value;
    d_value.parts       |= DatetimeParts::MILLISECONDS;
}

inline
blpapi_Datetime_t& Datetime::rawValue()
{
    return d_value;
}

inline
short Datetime::offset() const
{
    return d_value.offset;
}

inline
unsigned Datetime::month() const
{
    return d_value.month;
}

inline
unsigned Datetime::day() const
{
    return d_value.day;
}

inline
unsigned Datetime::year() const
{
    return d_value.year;
}

inline
unsigned Datetime::hours() const
{
    return d_value.hours;
}

inline
unsigned Datetime::minutes() const
{
    return d_value.minutes;
}

inline
unsigned Datetime::seconds() const
{
    return d_value.seconds;
}

inline
unsigned Datetime::milliSeconds() const
{
    return d_value.milliSeconds;
}

inline
const blpapi_Datetime_t& Datetime::rawValue() const
{
    return d_value;
}

inline
unsigned Datetime::parts() const
{
    return d_value.parts;
}

inline
bool Datetime::hasParts(unsigned parts) const
{
    return parts == (d_value.parts & parts);
}

inline
bool Datetime::isValid() const
{
    if ( (hasParts(DatetimeParts::YEAR)
          || hasParts(DatetimeParts::MONTH)
          || hasParts(DatetimeParts::DAY) )
         && !isValidDate(year(), month(), day()) ) {
        return false;
    }
    if ( (hasParts(DatetimeParts::HOURS)
          || hasParts(DatetimeParts::MINUTES)
          || hasParts(DatetimeParts::SECONDS)
          || hasParts(DatetimeParts::MILLISECONDS))
         && !isValidTime(hours(), minutes(), seconds(), milliSeconds()) ) {
        return false;
    }
    return true;
}

inline
bool Datetime::isLeapYear(int y)
{
    return 0 == y % 4 && (y <= 1752 || 0 != y % 100 || 0 == y % 400);
}

inline
bool Datetime::isValidDate(int year,
                           int month,
                           int day)
{
    if ((year <= 0) || (year > 9999) ||
        (month <= 0) || (month > 12) ||
        (day <= 0) || (day > 31) ) {
        return false;
    }
    if (day < 29) {
        return true;
    }
    if (year == 1752) {
        if (month == 9 && day > 2 && day < 14) {
            return false;
        }
    }
    switch (month) {
      case 1:
      case 3:
      case 5:
      case 7:
      case 8:
      case 10:
      case 12:
        return true;

      case 4:
      case 6:
      case 9:
      case 11: {
        if (day > 30) {
            return false;
        }
        else {
            return true;
        }
      }
      case 2: {
        if (isLeapYear(year)) {
            if (day > 29) {
                return false;
            }
            else {
                return true;
            }
        }
        else if (day > 28) {
            return false;
        }
        else {
            return true;
        }
      }
      default: {
        return true;
      }
    }
}

inline
bool Datetime::isValidTime(int hours,
                           int minutes,
                           int seconds,
                           int milliSeconds)
{
    if (hours == 24) {
        if ((minutes != 0) || (seconds !=0) || (milliSeconds != 0)) {
            return false;
        }
        else {
            return true;
        }
    }
    else {
        return (hours >= 0) && (hours < 24) &&
            (minutes >= 0) && (minutes < 60) &&
            (seconds >= 0) && (seconds < 60) &&
            (milliSeconds >= 0) && (milliSeconds < 1000);
    }
}

inline
std::ostream& Datetime::print( std::ostream& stream,
                               int level,
                               int spacesPerLevel) const
{
    blpapi_Datetime_print(
            &d_value, OstreamWriter, &stream, level, spacesPerLevel);
    return stream;
}

inline
bool operator==(const Datetime& lhs, const Datetime& rhs)
{
    if (lhs.parts() == rhs.parts()) {
        return (blpapi_Datetime_compare(lhs.rawValue(), rhs.rawValue()) == 0);
    }
    return false;
}


inline
bool operator!=(const Datetime& lhs, const Datetime& rhs)
{
    if (lhs.parts() == rhs.parts()) {
        return (blpapi_Datetime_compare(lhs.rawValue(), rhs.rawValue()) != 0);
    }
    return true;
}

inline
bool operator<(const Datetime& lhs, const Datetime& rhs)
{
    if (lhs.parts() == rhs.parts()) {
        return blpapi_Datetime_compare(lhs.rawValue(), rhs.rawValue()) < 0;
    }
    assert(0);
    throw InvalidArgumentException("parts() not same for the arguments");
}

inline
bool operator<=(const Datetime& lhs, const Datetime& rhs)
{
    if (lhs.parts() == rhs.parts()) {
        return blpapi_Datetime_compare(lhs.rawValue(), rhs.rawValue()) <= 0;
    }
    assert(0);
    throw InvalidArgumentException("parts() not same for the arguments");
}

inline
bool operator>(const Datetime& lhs, const Datetime& rhs)
{
    if (lhs.parts() == rhs.parts()) {
        return blpapi_Datetime_compare(lhs.rawValue(), rhs.rawValue()) > 0;
    }
    assert(0);
    throw InvalidArgumentException("parts() not same for the arguments");
}

inline
bool operator>=(const Datetime& lhs, const Datetime& rhs)
{
    if (lhs.parts() == rhs.parts()) {
        return blpapi_Datetime_compare(lhs.rawValue(), rhs.rawValue()) >= 0;
    }
    assert(0);
    throw InvalidArgumentException("parts() not same for the arguments");
}

inline
std::ostream& operator<<(std::ostream& stream, const Datetime& datetime)
{
    return datetime.print(stream, 0, -1);
}

}  // close namespace blpapi
}  // close namespace BloombergLP

#endif // #ifdef __cplusplus

#endif // #ifndef INCLUDED_BLPAPI_DATETIME
