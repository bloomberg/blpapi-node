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
//                blpapi_Datetime_tag: C struct for date and/or msec time
//   blpapi_HighPrecisionDatetime_tag: C struct for date and/or psec time
//                   blpapi::Datetime: C++ class for date and/or psec time
//              blpapi::DatetimeParts: Identifiers for fields within date/time
//               blpapi::DatetimeUtil: 'Datetime' utility functions 
//
//@DESCRIPTION: This file provides a C and C++ data type to represent a
// date and/or time value.

#ifndef INCLUDED_BLPAPI_CALL
#include <blpapi_call.h>
#endif

#ifndef INCLUDED_BLPAPI_DEFS
#include <blpapi_defs.h>
#endif

#ifndef INCLUDED_BLPAPI_EXCEPTION
#include <blpapi_exception.h>
#endif

#ifndef INCLUDED_BLPAPI_STREAMPROXY
#include <blpapi_streamproxy.h>
#endif

#ifndef INCLUDED_BLPAPI_TIMEPOINT
#include <blpapi_timepoint.h>
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
    blpapi_Int16_t  offset;       // (signed) minutes ahead of UTC
};

typedef struct blpapi_Datetime_tag blpapi_Datetime_t;

struct blpapi_HighPrecisionDatetime_tag {
    blpapi_Datetime_t datetime;
    blpapi_UInt32_t picoseconds; // picosecond offset into current
                                 // *millisecond* i.e. the picosecond offset
                                 // into the current full second is
                                 // '1000000000LL * milliSeconds + picoSeconds'
};

typedef struct blpapi_HighPrecisionDatetime_tag blpapi_HighPrecisionDatetime_t;

BLPAPI_EXPORT
int blpapi_Datetime_compare(blpapi_Datetime_t lhs, blpapi_Datetime_t rhs);


BLPAPI_EXPORT
int blpapi_Datetime_print(const blpapi_Datetime_t *datetime,
                          blpapi_StreamWriter_t streamWriter,
                          void *stream,
                          int level,
                          int spacesPerLevel);

BLPAPI_EXPORT
int blpapi_HighPrecisionDatetime_compare(
                                    const blpapi_HighPrecisionDatetime_t *lhs,
                                    const blpapi_HighPrecisionDatetime_t *rhs);


BLPAPI_EXPORT
int blpapi_HighPrecisionDatetime_print(
                         const blpapi_HighPrecisionDatetime_t *datetime,
                         blpapi_StreamWriter_t                 streamWriter,
                         void                                 *stream,
                         int                                   level,
                         int                                   spacesPerLevel);

BLPAPI_EXPORT
int blpapi_HighPrecisionDatetime_fromTimePoint(
                                     blpapi_HighPrecisionDatetime_t *datetime,
                                     const blpapi_TimePoint_t       *timePoint,
                                     short                           offset);

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
        // Enumeration used to indicate which parts of the 'Datetime' object
        // have had their values set.  The actual enumeration constants are
        // thus *not* consecutive -- they are bit fields that can be combined
        // using bitwise operators.  Note that the constants themselves are
        // defined in 'blpapi_defs.h'.

          YEAR            = BLPAPI_DATETIME_YEAR_PART            // year is set
        , MONTH           = BLPAPI_DATETIME_MONTH_PART           // month is set
        , DAY             = BLPAPI_DATETIME_DAY_PART             // day is set
        , OFFSET          = BLPAPI_DATETIME_OFFSET_PART          // offset is set
        , HOURS           = BLPAPI_DATETIME_HOURS_PART           // hours is set
        , MINUTES         = BLPAPI_DATETIME_MINUTES_PART         // minutes is set
        , SECONDS         = BLPAPI_DATETIME_SECONDS_PART         // seconds is set
        , FRACSECONDS     = BLPAPI_DATETIME_FRACSECONDS_PART     // fraction-of-second (both millisecond and picosecond) is set
        , MILLISECONDS    = BLPAPI_DATETIME_MILLISECONDS_PART    // 'MILLISECONDS' is a (legacy) synonym for 'FRACSECONDS'
        , DATE            = BLPAPI_DATETIME_DATE_PART            // year, month, and day are set
        , TIME            = BLPAPI_DATETIME_TIME_PART            // hours, minutes, and seconds are set
        , TIMEFRACSECONDS = BLPAPI_DATETIME_TIMEFRACSECONDS_PART // hours, minutes, seconds, and fraction-of-second are set
        , TIMEMILLI       = BLPAPI_DATETIME_TIMEMILLI_PART       // 'TIMEMILLI' is a (legacy) synonym for 'TIMEFRACSECONDS'
    };
};

                               // ==============
                               // class Datetime
                               // ==============

class Datetime {
    // Represents a date and/or time.
    //
    // Datetime can represent a date and/or a time or any combination of the
    // components of a date and time.  The value is represented as eight parts
    // which can be set or queried independently.
    //
    // These parts are: year; month (from January as 1 to December as 12); day
    // (of month, from 1 to 31); hour (from 0 to 23); minute (0 to 59); second
    // (0 to 59); fraction-of-second (logically representing arbitrary
    // precision, with the current interface providing picosecond resolution);
    // and offset (time zone as minutes ahead of UTC).
    //
    // Methods are provided to set and query the parts individually and in
    // groups, e.g. 'setDate()' and 'setTime()'.  It is also possible to
    // determine which parts of the 'Datetime' have been set (via the 'parts()'
    // method).

    blpapi_HighPrecisionDatetime_t d_value;

    static bool isLeapYear(int year);

    struct TimeTag {};
    Datetime(unsigned hours,
             unsigned minutes,
             unsigned seconds,
             TimeTag);
        // Create a 'Datetime' object having the value representing the
        // specified 'hours', 'minutes', and 'seconds'.  The
        // behavior is undefined unless 'hours', 'minutes', and 'seconds'
        // represent a valid time as specified by the 'isValidTime' function.
        // The resulting 'Datetime' object has the parts specified by 'TIME'
        // set, and all other parts unset.  In particular, the 'FRACSECONDS'
        // part is unset.
        // Note that the final 'TimeTag' parameter is used purely to
        // disambiguate this constructor from that for year, month, and day.
        // A constuctor for 4 PM would be written as follows:
        //..
        //  Datetime dt = Datetime(16, 0, 0, Datetime::TimeTag());
        //..
        // Note that this constructor is intended for internal use only; client
        // code should use the 'createTime' interface.

  public:
    struct Milliseconds {
        int d_msec;
        explicit Milliseconds(int milliseconds);
            // The behavior is undefined unless '0 <= milliseconds < 1000'.
    };
    struct Microseconds {
        int d_usec;
        explicit Microseconds(int microseconds);
            // The behavior is undefined unless
            // '0 <= microseconds < 1,000,000'.
    };
    struct Nanoseconds {
        int d_nsec;
        explicit Nanoseconds(int nanoseconds);
            // The behavior is undefined unless
            // '0 <= nanoseconds < 1,000,000,000'.
    };
    struct Picoseconds {
        long long d_psec;
        explicit Picoseconds(long long picoseconds);
            // The behavior is undefined unless
            // '0 <= picoseconds < 1,000,000,000,000'.
    };
    struct Offset {
        short d_minutesAheadOfUTC;
        explicit Offset(short minutesAheadOfUTC);
            // The behavior is undefined unless
            // '-840 <= minutesAheadOfUTC <= 840'.
    };

    static bool isValidDate(int year, int month, int day);
        // Return 'true' if the specified 'year', 'month', and 'day' represent
        // a valid calendar date, and 'false' otherwise.  Note that many
        // functions within 'Datetime' provide defined behavior only when valid
        // dates are provided as arguments.

    static bool isValidTime(int          hours,
                            int          minutes,
                            int          seconds);
    static bool isValidTime(int          hours,
                            int          minutes,
                            int          seconds,
                            int          milliSeconds);
    static bool isValidTime(int          hours,
                            int          minutes,
                            int          seconds,
                            Milliseconds fractionOfSecond);
    static bool isValidTime(int          hours,
                            int          minutes,
                            int          seconds,
                            Microseconds fractionOfSecond);
    static bool isValidTime(int          hours,
                            int          minutes,
                            int          seconds,
                            Nanoseconds  fractionOfSecond);
    static bool isValidTime(int          hours,
                            int          minutes,
                            int          seconds,
                            Picoseconds  fractionOfSecond);
        // Return 'true' if the specified 'hours', 'minutes', 'seconds', and
        // (optionally specified) 'milliseconds' or 'fractionOfSecond'
        // represent a valid time of day, and 'false' otherwise.  Note that
        // many functions within 'Datetime' provide defined behavior only when
        // valid times are provided as arguments.

    // CREATORS
    static Datetime createDatetime(unsigned year,
                                   unsigned month,
                                   unsigned day,
                                   unsigned hours,
                                   unsigned minutes,
                                   unsigned seconds);
        // Create a 'Datetime' object having the value representing the
        // specified 'year', 'month', 'day', 'hours', 'minutes', and 'seconds'
        // parts.  The behavior is undefined unless 'year', 'month', and 'day'
        // represent a valid date as specified by the 'isValidDate' function,
        // and 'hours', 'minutes', and 'seconds' represent a valid time as
        // specified by the 'isValidTime' function.  The resulting 'Datetime'
        // object has the parts specified by 'DATE' and 'TIME' set, and the
        // 'OFFSET' and 'FRACSECONDS' parts unset.

    static Datetime createDatetime(unsigned year,
                                   unsigned month,
                                   unsigned day,
                                   unsigned hours,
                                   unsigned minutes,
                                   unsigned seconds,
                                   Offset   offset);
        // Create a 'Datetime' object having the value representing the
        // specified 'year', 'month', 'day', 'hours', 'minutes', 'seconds', and
        // 'offset' parts.  The behavior is undefined unless 'year', 'month',
        // and 'day' represent a valid date as specified by the 'isValidDate'
        // function, and 'hours', 'minutes', and 'seconds' represent a valid
        // time as specified by the 'isValidTime' function.  The resulting
        // 'Datetime' object has the parts specified by 'DATE', 'TIME', and
        // 'OFFSET' set, and the 'FRACSECONDS' part unset.

    static Datetime createDatetime(unsigned     year,
                                   unsigned     month,
                                   unsigned     day,
                                   unsigned     hours,
                                   unsigned     minutes,
                                   unsigned     seconds,
                                   Milliseconds fractionOfSecond);
    static Datetime createDatetime(unsigned     year,
                                   unsigned     month,
                                   unsigned     day,
                                   unsigned     hours,
                                   unsigned     minutes,
                                   unsigned     seconds,
                                   Microseconds fractionOfSecond);
    static Datetime createDatetime(unsigned    year,
                                   unsigned    month,
                                   unsigned    day,
                                   unsigned    hours,
                                   unsigned    minutes,
                                   unsigned    seconds,
                                   Nanoseconds fractionOfSecond);
    static Datetime createDatetime(unsigned    year,
                                   unsigned    month,
                                   unsigned    day,
                                   unsigned    hours,
                                   unsigned    minutes,
                                   unsigned    seconds,
                                   Picoseconds fractionOfSecond);
        // Create a 'Datetime' object having the value representing the
        // specified 'year', 'month', 'day', 'hours', 'minutes', 'seconds', and
        // 'fractionOfSecond'.  The behavior is undefined unless 'year',
        // 'month', and 'day' represent a valid date as specified by the
        // 'isValidDate' function, and 'hours', 'minutes', 'seconds', and
        // 'fractionOfSecond' represent a valid time as specified by the
        // 'isValidTime' function.  The resulting 'Datetime' object has the
        // parts specified by 'DATE' and 'TIMEFRACSECONDS' set, and the
        // 'OFFSET' part unset.

    static Datetime createDatetime(unsigned     year,
                                   unsigned     month,
                                   unsigned     day,
                                   unsigned     hours,
                                   unsigned     minutes,
                                   unsigned     seconds,
                                   Milliseconds fractionOfSecond,
                                   Offset       offset);
    static Datetime createDatetime(unsigned     year,
                                   unsigned     month,
                                   unsigned     day,
                                   unsigned     hours,
                                   unsigned     minutes,
                                   unsigned     seconds,
                                   Microseconds fractionOfSecond,
                                   Offset       offset);
    static Datetime createDatetime(unsigned     year,
                                   unsigned     month,
                                   unsigned     day,
                                   unsigned     hours,
                                   unsigned     minutes,
                                   unsigned     seconds,
                                   Nanoseconds  fractionOfSecond,
                                   Offset       offset);
    static Datetime createDatetime(unsigned     year,
                                   unsigned     month,
                                   unsigned     day,
                                   unsigned     hours,
                                   unsigned     minutes,
                                   unsigned     seconds,
                                   Picoseconds  fractionOfSecond,
                                   Offset       offset);
        // Create a 'Datetime' object having the value representing the
        // specified 'year', 'month', 'day', 'hours', 'minutes', 'seconds',
        // 'fractionOfSecond', and 'offset'.  The behavior is undefined unless
        // 'year', 'month', and 'day' represent a valid date as specified by
        // the 'isValidDate' function, and 'hours', 'minutes', 'seconds', and
        // 'fractionOfSecond' represent a valid time as specified by the
        // 'isValidTime' function.  The resulting 'Datetime' object has all
        // parts set.

    static Datetime createDate(unsigned year,
                               unsigned month,
                               unsigned day);
        // Create a 'Datetime' object having the value representing the
        // specified 'year', 'month', and 'day'.  The behavior is undefined
        // unless 'year', 'month', and 'day' represent a valid date as
        // specified by the 'isValidDate' function.  The resulting 'Datetime'
        // object has the parts specified by 'DATE' set, and all other parts
        // unset.

    static Datetime createTime(unsigned hours,
                               unsigned minutes,
                               unsigned seconds);
        // Create a 'Datetime' object having the value representing the
        // specified 'hours', 'minutes', and 'seconds'.  The
        // behavior is undefined unless 'hours', 'minutes', and 'seconds'
        // represent a valid time as specified by the 'isValidTime' function.
        // The resulting 'Datetime' object has the parts specified by 'TIME'
        // set, and all other parts unset.  Note that the 'FRACSECONDS' part is
        // unset.

    static Datetime createTime(unsigned hours,
                               unsigned minutes,
                               unsigned seconds,
                               Offset   offset);
        // Create a 'Datetime' object having the value representing the
        // specified 'hours', 'minutes', 'seconds', and 'offset'.  The
        // behavior is undefined unless 'hours', 'minutes', and 'seconds'
        // represent a valid time as specified by the 'isValidTime' function.
        // The resulting 'Datetime' object has the parts specified by 'TIME'
        // and 'OFFSET' set, and all other parts unset.  Note that the
        // 'FRACSECONDS' part is unset.

    static Datetime createTime(unsigned hours,
                               unsigned minutes,
                               unsigned seconds,
                               unsigned milliseconds);
        // Create a 'Datetime' object having the value representing the
        // specified 'hours', 'minutes', 'seconds', and 'milliseconds'.  The
        // behavior is undefined unless 'hours', 'minutes', 'seconds', and
        // 'milliseconds' represent a valid time as specified by the
        // 'isValidTime' function.  The resulting 'Datetime' object has the
        // parts specified by 'TIMEFRACSECONDS' set, and all other parts unset.

    static Datetime createTime(unsigned hours,
                               unsigned minutes,
                               unsigned seconds,
                               unsigned milliseconds,
                               Offset   offset);
        // Create a 'Datetime' object having the value representing the
        // specified 'hours', 'minutes', 'seconds', 'milliseconds', and
        // 'offset'.  The behavior is undefined unless 'hours', 'minutes',
        // 'seconds', and 'milliseconds' represent a valid time as specified by
        // the 'isValidTime' function.  The resulting 'Datetime' object has the
        // parts specified by 'TIMEFRACSECONDS' and 'OFFSET' set, and all other
        // parts unset.

    static Datetime createTime(unsigned     hours,
                               unsigned     minutes,
                               unsigned     seconds,
                               Milliseconds fractionOfSecond);
    static Datetime createTime(unsigned     hours,
                               unsigned     minutes,
                               unsigned     seconds,
                               Microseconds fractionOfSecond);
    static Datetime createTime(unsigned     hours,
                               unsigned     minutes,
                               unsigned     seconds,
                               Nanoseconds  fractionOfSecond);
    static Datetime createTime(unsigned     hours,
                               unsigned     minutes,
                               unsigned     seconds,
                               Picoseconds  fractionOfSecond);
        // Create a 'Datetime' object having the value representing the
        // specified 'hours', 'minutes', 'seconds', and 'fractionOfSecond'.
        // The behavior is undefined unless 'hours', 'minutes', 'seconds', and
        // 'fractionOfSecond' represent a valid time as specified by the
        // 'isValidTime' function.  The resulting 'Datetime' object has the
        // parts specified by 'TIMEFRACSECONDS' set, and all other
        // parts unset.

    static Datetime createTime(unsigned     hours,
                               unsigned     minutes,
                               unsigned     seconds,
                               Milliseconds fractionOfSecond,
                               Offset       offset);
    static Datetime createTime(unsigned     hours,
                               unsigned     minutes,
                               unsigned     seconds,
                               Microseconds fractionOfSecond,
                               Offset       offset);
    static Datetime createTime(unsigned     hours,
                               unsigned     minutes,
                               unsigned     seconds,
                               Nanoseconds  fractionOfSecond,
                               Offset       offset);
    static Datetime createTime(unsigned     hours,
                               unsigned     minutes,
                               unsigned     seconds,
                               Picoseconds  fractionOfSecond,
                               Offset       offset);
        // Create a 'Datetime' object having the value representing the
        // specified 'hours', 'minutes', 'seconds', 'fractionOfSecond', and
        // 'offset'.  The behavior is undefined unless 'hours', 'minutes',
        // 'seconds', and 'fractionOfSecond' represent a valid time as
        // specified by the 'isValidTime' function.  The resulting 'Datetime'
        // object has the parts specified by 'TIMEFRACSECONDS' and 'OFFSET'
        // set, and all other parts unset.

    Datetime();
        // Construct a 'Datetime' object with all parts unset.

    Datetime(const Datetime& original);
        // Copy constructor.

    Datetime(const blpapi_Datetime_t& rawValue);

    explicit Datetime(const blpapi_HighPrecisionDatetime_t& rawValue);

    Datetime(unsigned year,
             unsigned month,
             unsigned day,
             unsigned hours,
             unsigned minutes,
             unsigned seconds);
        // Create a 'Datetime' object having the value representing the
        // specified 'year', 'month', 'day', 'hours', 'minutes', and 'seconds'
        // parts.  The behavior is undefined unless 'year', 'month', and 'day'
        // represent a valid date as specified by the 'isValidDate' function,
        // and 'hours', 'minutes', and 'seconds' represent a valid time as
        // specified by the 'isValidTime' function.  The resulting 'Datetime'
        // object has the parts specified by 'DATE' and 'TIME' set, and the
        // 'OFFSET' and 'FRACSECONDS' parts unset.
        // Use of this function is discouraged; use 'createDatetime' instead.

    Datetime(unsigned year,
             unsigned month,
             unsigned day,
             unsigned hours,
             unsigned minutes,
             unsigned seconds,
             unsigned milliseconds);
        // Create a 'Datetime' object having the value representing the
        // specified 'year', 'month', 'day', 'hours', 'minutes', 'seconds', and
        // 'milliseconds'.  The behavior is undefined unless 'year', 'month',
        // and 'day' represent a valid date as specified by the 'isValidDate'
        // function, and 'hours', 'minutes', 'seconds', and 'milliseconds'
        // represent a valid time as specified by the 'isValidTime' function.
        // The resulting 'Datetime' object has the parts specified by 'DATE'
        // and 'TIMEFRACSECONDS' set, and the 'OFFSET' part unset.
        // Use of this function is discouraged; use 'createDatetime' instead.

    Datetime(unsigned     year,
             unsigned     month,
             unsigned     day,
             unsigned     hours,
             unsigned     minutes,
             unsigned     seconds,
             Milliseconds fractionOfSecond);
    Datetime(unsigned     year,
             unsigned     month,
             unsigned     day,
             unsigned     hours,
             unsigned     minutes,
             unsigned     seconds,
             Microseconds fractionOfSecond);
    Datetime(unsigned    year,
             unsigned    month,
             unsigned    day,
             unsigned    hours,
             unsigned    minutes,
             unsigned    seconds,
             Nanoseconds fractionOfSecond);
    Datetime(unsigned    year,
             unsigned    month,
             unsigned    day,
             unsigned    hours,
             unsigned    minutes,
             unsigned    seconds,
             Picoseconds fractionOfSecond);
        // Create a 'Datetime' object having the value representing the
        // specified 'year', 'month', 'day', 'hours', 'minutes', 'seconds', and
        // 'fractionOfSecond'.  The behavior is undefined unless 'year',
        // 'month', and 'day' represent a valid date as specified by the
        // 'isValidDate' function, and 'hours', 'minutes', 'seconds', and
        // 'fractionOfSecond' represent a valid time as specified by the
        // 'isValidTime' function.  The resulting 'Datetime' object has the
        // parts specified by 'DATE' and 'TIMEFRACSECONDS' set, and the
        // 'OFFSET' part unset.
        // Use of these functions is discouraged; use 'createDatetime' instead.

    Datetime(unsigned year,
             unsigned month,
             unsigned day);
        // Create a 'Datetime' object having the value representing the
        // specified 'year', 'month', and 'day'.  The behavior is undefined
        // unless 'year', 'month', and 'day' represent a valid date as
        // specified by the 'isValidDate' function.  The resulting 'Datetime'
        // object has the parts specified by 'DATE' set, and all other parts
        // unset.
        // Note that constructing a 'Datetime' from three integers produces a
        // date; to create a time from hour, minute, and second (without the
        // fraction-of-second part unset) use the constructor taking a
        // 'TimeTag'.
        // Use of this function is discouraged; use 'createDate' instead.


    Datetime(unsigned hours,
             unsigned minutes,
             unsigned seconds,
             unsigned milliseconds);
        // Create a 'Datetime' object having the value representing the
        // specified 'hours', 'minutes', 'seconds', and 'milliseconds'.  The
        // behavior is undefined unless 'hours', 'minutes', 'seconds', and
        // 'milliseconds' represent a valid time as specified by the
        // 'isValidTime' function.  The resulting 'Datetime' object has the
        // parts specified by 'TIMEFRACSECONDS' set, and all other parts unset.
        // Note that removing the final argument from a call to this function
        // results in a constructor creating a date, not a time.

    Datetime(unsigned     hours,
             unsigned     minutes,
             unsigned     seconds,
             Milliseconds fractionOfSecond);
    Datetime(unsigned     hours,
             unsigned     minutes,
             unsigned     seconds,
             Microseconds fractionOfSecond);
    Datetime(unsigned     hours,
             unsigned     minutes,
             unsigned     seconds,
             Nanoseconds  fractionOfSecond);
    Datetime(unsigned     hours,
             unsigned     minutes,
             unsigned     seconds,
             Picoseconds  fractionOfSecond);
        // Create a 'Datetime' object having the value representing the
        // specified 'hours', 'minutes', 'seconds', and 'fractionOfSecond'.
        // The behavior is undefined unless 'hours', 'minutes', 'seconds', and
        // 'fractionOfSecond' represent a valid time as specified by the
        // 'isValidTime' function.  The resulting 'Datetime' object has the
        // parts specified by 'TIMEFRACSECONDS' set, and all other parts unset.
        // Note that removing the final argument from a call to this function
        // results in a constructor creating a date, not a time.

    // MANIPULATORS

    Datetime& operator=(const Datetime& rhs);
        // Assignment operator.

    void setDate(unsigned year,
                 unsigned month,
                 unsigned day);
        // Set the 'DATE' parts of this 'Datetime' object to the specified
        // 'year', 'month', and 'day'.  The behavior is undefined unless
        // 'isValidDate(year, month, day)' would return 'true'.

    void setTime(unsigned hours,
                 unsigned minutes,
                 unsigned seconds);
        // Set the 'TIME' parts of this 'Datetime' object to the specified
        // 'hours', 'minutes', and 'seconds', and mark the 'FRACSECONDS' part
        // of this 'Datetime' as unset.  The behavior is undefined unless
        // 'isValidTime(hours, minutes, seconds)' would return 'true'.

    void setTime(unsigned hours,
                 unsigned minutes,
                 unsigned seconds,
                 unsigned milliseconds);
        // Set the 'TIMEFRACSECONDS' parts of this 'Datetime' object to the
        // specified 'hours', 'minutes', 'seconds', and 'milliseconds'.  The
        // behavior is undefined unless
        // 'isValidTime(hours, minutes, seconds, milliseconds)' would return
        // 'true'.

    void setTime(unsigned     hours,
                 unsigned     minutes,
                 unsigned     seconds,
                 Milliseconds fractionOfSecond);
    void setTime(unsigned     hours,
                 unsigned     minutes,
                 unsigned     seconds,
                 Microseconds fractionOfSecond);
    void setTime(unsigned    hours,
                 unsigned    minutes,
                 unsigned    seconds,
                 Nanoseconds fractionOfSecond);
    void setTime(unsigned    hours,
                 unsigned    minutes,
                 unsigned    seconds,
                 Picoseconds fractionOfSecond);
        // Set the 'TIMEFRACSECONDS' parts of this 'Datetime' object to the
        // specified 'hours', 'minutes', 'seconds', and 'fractionOfSecond'.
        // The behavior is undefined unless
        // 'isValidTime(hours, minutes, seconds, fractionOfSecond)' would
        // return 'true'.

    void setOffset(short minutesAheadOfUTC);
        // Set the 'OFFSET' (i.e. timezone) part of this 'Datetime' object to
        // the specified 'minutesAheadOfUTC'.
        // The behavior is undefined unless '-840 <= minutesAheadOfUTC <= 840'.

    void setYear(unsigned value);
        // Set the 'YEAR' part of this 'Datetime' object to the specified
        // 'value'.
        // The behavior is undefined unless '1 <= value <= 9999', and either
        // the 'MONTH' part is not set, the 'DAY' part is not set, or
        // 'isValidDate(value. this->month(), this->day()) == true'.

    void setMonth(unsigned value);
        // Set the 'MONTH' part of this 'Datetime' object to the specified
        // 'value'.
        // The behavior is undefined unless '1 <= value <= 12', and either the
        // 'DAY' part is not set, the 'YEAR' part is not set, or
        // 'isValidDate(this->year(). value, this->day()) == true'.

    void setDay(unsigned value);
        // Set the 'DAY' part of this 'Datetime' object to the specified
        // 'value'.
        // The behavior is undefined unless '1 <= value <= 31', and either the
        // 'MONTH' part is not set, the 'YEAR' part is not set, or
        // 'isValidDate(this->year(). this->month(), value) == true'.

    void setHours(unsigned value);
        // Set the 'HOURS' part of this 'Datetime' object to the specified
        // 'value'.
        // The behavior is undefined unless '0 <= value <= 23'.

    void setMinutes(unsigned value);
        // Set the 'MINUTES' part of this 'Datetime' object to the specified
        // 'value'.
        // The behavior is undefined unless '0 <= value <= 59'.

    void setSeconds(unsigned value);
        // Set the 'SECONDS' part of this 'Datetime' object to the specified
        // 'value'.
        // The behavior is undefined unless '0 <= value <= 59'.

    void setMilliseconds(unsigned milliseconds);
        // Set the 'FRACSECONDS' part of this 'Datetime' object to the
        // specified 'milliseconds'.
        // The behavior is undefined unless '0 <= value <= 999'.

    void setFractionOfSecond(Milliseconds value);
        // Set the 'FRACSECONDS' part of this 'Datetime' object to the
        // Set the fraction of a second of the value of this object to the
        // specified 'value'.  Note that the behavior is undefined unless
        // '0 <= value <= 999 ms'.

    void setFractionOfSecond(Microseconds value);
        // Set the 'FRACSECONDS' part of this 'Datetime' object to the
        // specified 'value'. Note that the behavior is undefined unless
        // '0 <= value <= 999,999 us'.

    void setFractionOfSecond(Nanoseconds value);
        // Set the 'FRACSECONDS' part of this 'Datetime' object to the
        // specified 'value'.  Note that the behavior is undefined unless
        // '0 <= value <= 999,999,999 ns'.

    void setFractionOfSecond(Picoseconds value);
        // Set the 'FRACSECONDS' part of this 'Datetime' object to the
        // specified 'value'.  Note that the behavior is undefined unless
        // '0 <= value <= 999,999,999,999 ps'.

    blpapi_Datetime_t& rawValue();
        // Return a (modifiable) reference to the millisecond-resolution C
        // struct underlying this object.  Behavior of the object is undefined
        // if the returned struct is modified concurrently with other non-const
        // methods of this object, or if the fields of the 'blpapi_Datetime_t'
        // are modified such that the 'Datetime::isValid' methods of this class
        // would return 'false' when passed those fields of the struct whose
        // bits are set in the struct's 'parts' field.  Further, direct setting
        // of the 'FRACSECONDS' bit in the returned struct's 'parts' field will
        // cause this 'Datetime' object to compute its fraction-of-second part
        // not just from the struct's 'milliSeconds' field, but also from the
        // 'picoseconds' field of the the struct returned from
        // 'rawHighPrecisionValue()'; if neither that field nor this 'Datetime'
        // objects' fraction-of-second part have been initialized, then
        // the behavior of setting the 'FRACSECONDS' bit directly is undefined.

    blpapi_HighPrecisionDatetime_t& rawHighPrecisionValue();
        // Return a (modifiable) reference to the high-resolution C struct
        // underlying this object.  Behavior of the object is undefined if the
        // returned struct is modified concurrently with other non-const
        // methods of this object, or if the fields of the
        // 'blpapi_HighPrecisionDatetime_t' are modified such that the
        // 'Datetime::isValid*' methods of this class would return 'false' when
        // passed those fields of the struct whose bits are set in the struct's
        // 'parts' field.

    // ACCESSORS
    bool hasParts(unsigned parts) const;
        // Return true if this 'Datetime' object has all of the specified
        // 'parts' set.  The 'parts' parameter must be constructed by or'ing
        // together values from the 'DatetimeParts' enum.

    unsigned parts() const;
        // Return a bitmask of all parts that are set in this 'Datetime'
        // object.  This can be compared to the values in the 'DatetimeParts'
        // enum using bitwise operations.

    unsigned year() const;
        // Return the year value of this 'Datetime' object.  The result is
        // undefined unless the 'YEAR' part of this object is set.

    unsigned month() const;
        // Return the month value of this 'Datetime' object.  The result is
        // undefined unless the 'MONTH' part of this object is set.

    unsigned day() const;
        // Return the day value of this 'Datetime' object.  The result is
        // undefined unless the 'DAY' part of this object is set.

    unsigned hours() const;
        // Return the hours value of this 'Datetime' object.  The result is
        // undefined unless the 'HOURS' part of this object is set.

    unsigned minutes() const;
        // Return the minutes value of this 'Datetime' object.  The result is
        // undefined unless the 'MINUTES' part of this object is set.

    unsigned seconds() const;
        // Return the seconds value of this 'Datetime' object.  The result is
        // undefined unless the 'SECONDS' part of this object is set.

    unsigned milliSeconds() const;
        // Return the milliseconds value of this 'Datetime' object.  The result
        // is undefined unless the 'FRACSECONDS' part of this object is set.
        // This function is deprecated; use 'milliseconds()' instead.

    unsigned milliseconds() const;
        // Return the number of (whole) milliseconds in the
        // fraction-of-a-second part of the value of this object.  The result
        // is undefined unless the 'FRACSECONDS' part of this object is set.

    unsigned microseconds() const;
        // Return the number of (whole) microseconds in the
        // fraction-of-a-second part of the value of this object.  The result
        // is undefined unless the 'FRACSECONDS' part of this object is set.

    unsigned nanoseconds() const;
        // Return the number of (whole) nanoseconds in the fraction-of-a-second
        // part of the value of this object.  The result is undefined unless
        // the 'FRACSECONDS' part of this object is set.

    unsigned long long picoseconds() const;
        // Return the number of (whole) picoseconds in the fraction-of-a-second
        // part of the value of this object.  The result is undefined unless
        // the 'FRACSECONDS' part of this object is set.

    short offset() const;
        // Return the number of minutes this 'Datetime' object is ahead of UTC.
        // The result is undefined unless the 'OFFSET' part of this object is
        // set.

    const blpapi_Datetime_t& rawValue() const;
        // Return a (read-only) reference to the millisecond-resolution C
        // struct underlying this object.

    const blpapi_HighPrecisionDatetime_t& rawHighPrecisionValue() const;
        // Return a (read-only) reference to the high-precision C struct
        // underlying this object.

    bool isValid() const;
        // Check whether the value of this 'Datetime' is valid.  The behaviour
        // is undefined unless this object represents a date (has YEAR, MONTH
        // and DAY part set) or time (has HOURS, MINUTES, SECONDS and
        // MILLISECONDS part set).  Note that in almost all cases where this
        // function returns 'false', prior member function calls have had
        // undefined behavior.
        // This function is deprecated; use 'isValidDate' and/or 'isValidTime'
        // directly instead.

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
    // Return 'true' if the specified 'lhs' and 'rhs' have the same value, and
    // 'false' otherwise.  Two 'Datetime' objects have the same value if they
    // have the same parts set, and the same values for each of those parts.

bool operator!=(const Datetime& lhs, const Datetime& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' do not have the same
    // value, and 'false' otherwise.  Two 'Datetime' objects have the same
    // value if they have the same parts set, and the same values for each of
    // those parts.

bool operator<(const Datetime& lhs, const Datetime& rhs);
bool operator<=(const Datetime& lhs, const Datetime& rhs);
bool operator>(const Datetime& lhs, const Datetime& rhs);
bool operator>=(const Datetime& lhs, const Datetime& rhs);
    // Compare the specified 'lhs' and 'rhs'.  The ordering used is temporal,
    // with earlier moments comparing less than later moments, in the case that
    // 'lhs.parts() == rhs.parts()' and 'parts()' is one of 'DATE', 'TIME',
    // 'TIMEFRACSECONDS', 'DATE | TIME', and 'DATE | TIMEFRACSECONDS'; the
    // ordering in all other cases is unspecified, but guaranteed stable within
    // process.

std::ostream& operator<<(std::ostream& stream, const Datetime& datetime);
    // Write the value of the specified 'datetime' object to the specified
    // output 'stream' in a single-line format, and return a reference to
    // 'stream'.  If 'stream' is not valid on entry, this operation has no
    // effect.  Note that this human-readable format is not fully specified,
    // can change without notice, and is logically equivalent to:
    //..
    //  print(stream, 0, -1);
    //..

                            // ===================
                            // struct DatetimeUtil
                            // ===================

struct DatetimeUtil {
    // This provides a namespace for 'Datetime' utility functions.

    static Datetime fromTimePoint(
                                const TimePoint& timePoint,
                                Datetime::Offset offset = Datetime::Offset(0));
        // Create and return a 'Datetime' object having the value of the
        // specified 'timePoint' and the optionally specified timezone
        // 'offset', 0 by default. The resulting 'Datetime' object has the
        // parts specified by 'DATE', 'TIMEFRACSECONDS', and 'OFFSET' set.
};

// ============================================================================
//                      INLINE FUNCTION DEFINITIONS
// ============================================================================

                            // --------------
                            // class Datetime
                            // --------------

inline
bool Datetime::isLeapYear(int y)
{
    return 0 == y % 4 && (y <= 1752 || 0 != y % 100 || 0 == y % 400);
}

inline
Datetime::Datetime(unsigned newHours,
                   unsigned newMinutes,
                   unsigned newSeconds,
                   TimeTag)
{
    std::memset(&d_value, 0, sizeof(d_value));
    setTime(newHours, newMinutes, newSeconds);
}

inline
Datetime::Milliseconds::Milliseconds(int milliseconds)
: d_msec(milliseconds)
{
}

inline
Datetime::Microseconds::Microseconds(int microseconds)
: d_usec(microseconds)
{
}

inline
Datetime::Nanoseconds::Nanoseconds(int nanoseconds)
: d_nsec(nanoseconds)
{
}

inline
Datetime::Picoseconds::Picoseconds(long long picoseconds)
: d_psec(picoseconds)
{
}

inline
Datetime::Offset::Offset(short minutesAheadOfUTC)
: d_minutesAheadOfUTC(minutesAheadOfUTC)
{
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
                           int seconds) {
    return (hours >= 0) && (hours < 24)
        && (minutes >= 0) && (minutes < 60)
        && (seconds >= 0) && (seconds < 60);
}

inline
bool Datetime::isValidTime(int hours,
                           int minutes,
                           int seconds,
                           int milliSeconds)
{
    return (hours >= 0) && (hours < 24)
        && (minutes >= 0) && (minutes < 60)
        && (seconds >= 0) && (seconds < 60)
        && (milliSeconds >= 0) && (milliSeconds < 1000);
}

inline
bool Datetime::isValidTime(int          hours,
                           int          minutes,
                           int          seconds,
                           Milliseconds fractionOfSecond)
{
    return isValidTime(hours, minutes, seconds, fractionOfSecond.d_msec);
}

inline
bool Datetime::isValidTime(int          hours,
                           int          minutes,
                           int          seconds,
                           Microseconds fractionOfSecond)
{
    return (hours >= 0) && (hours < 24)
        && (minutes >= 0) && (minutes < 60)
        && (seconds >= 0) && (seconds < 60)
        && (fractionOfSecond.d_usec >= 0)
        && (fractionOfSecond.d_usec < 1000 * 1000);
}

inline
bool Datetime::isValidTime(int         hours,
                           int         minutes,
                           int         seconds,
                           Nanoseconds fractionOfSecond)
{
    return (hours >= 0) && (hours < 24)
        && (minutes >= 0) && (minutes < 60)
        && (seconds >= 0) && (seconds < 60)
        && (fractionOfSecond.d_nsec >= 0)
        && (fractionOfSecond.d_nsec < 1000 * 1000 * 1000);
}

inline
bool Datetime::isValidTime(int         hours,
                           int         minutes,
                           int         seconds,
                           Picoseconds fractionOfSecond)
{
    return (hours >= 0) && (hours < 24)
        && (minutes >= 0) && (minutes < 60)
        && (seconds >= 0) && (seconds < 60)
        && (fractionOfSecond.d_psec >= 0)
        && (fractionOfSecond.d_psec < 1000LL * 1000 * 1000 * 1000);
}

inline
Datetime Datetime::createDatetime(unsigned year,
                                  unsigned month,
                                  unsigned day,
                                  unsigned hours,
                                  unsigned minutes,
                                  unsigned seconds)
{
    return Datetime(year, month, day, hours, minutes, seconds);
}

inline
Datetime Datetime::createDatetime(unsigned year,
                                  unsigned month,
                                  unsigned day,
                                  unsigned hours,
                                  unsigned minutes,
                                  unsigned seconds,
                                  Offset   offset)
{
    Datetime dt(year, month, day, hours, minutes, seconds);
    dt.setOffset(offset.d_minutesAheadOfUTC);
    return dt;
}

inline
Datetime Datetime::createDatetime(unsigned     year,
                                  unsigned     month,
                                  unsigned     day,
                                  unsigned     hours,
                                  unsigned     minutes,
                                  unsigned     seconds,
                                  Milliseconds fractionOfSecond)
{
    return Datetime(year,
                    month,
                    day,
                    hours,
                    minutes,
                    seconds,
                    fractionOfSecond);
}

inline
Datetime Datetime::createDatetime(unsigned     year,
                                  unsigned     month,
                                  unsigned     day,
                                  unsigned     hours,
                                  unsigned     minutes,
                                  unsigned     seconds,
                                  Microseconds fractionOfSecond)
{
    return Datetime(year,
                    month,
                    day,
                    hours,
                    minutes,
                    seconds,
                    fractionOfSecond);
}

inline
Datetime Datetime::createDatetime(unsigned    year,
                                  unsigned    month,
                                  unsigned    day,
                                  unsigned    hours,
                                  unsigned    minutes,
                                  unsigned    seconds,
                                  Nanoseconds fractionOfSecond)
{
    return Datetime(year,
                    month,
                    day,
                    hours,
                    minutes,
                    seconds,
                    fractionOfSecond);
}

inline
Datetime Datetime::createDatetime(unsigned    year,
                                  unsigned    month,
                                  unsigned    day,
                                  unsigned    hours,
                                  unsigned    minutes,
                                  unsigned    seconds,
                                  Picoseconds fractionOfSecond)
{
    return Datetime(year,
                    month,
                    day,
                    hours,
                    minutes,
                    seconds,
                    fractionOfSecond);
}

inline
Datetime Datetime::createDatetime(unsigned     year,
                                  unsigned     month,
                                  unsigned     day,
                                  unsigned     hours,
                                  unsigned     minutes,
                                  unsigned     seconds,
                                  Milliseconds fractionOfSecond,
                                  Offset       offset)
{
    Datetime dt(year, month, day, hours, minutes, seconds, fractionOfSecond);
    dt.setOffset(offset.d_minutesAheadOfUTC);
    return dt;
}

inline
Datetime Datetime::createDatetime(unsigned     year,
                                  unsigned     month,
                                  unsigned     day,
                                  unsigned     hours,
                                  unsigned     minutes,
                                  unsigned     seconds,
                                  Microseconds fractionOfSecond,
                                  Offset       offset)
{
    Datetime dt(year, month, day, hours, minutes, seconds, fractionOfSecond);
    dt.setOffset(offset.d_minutesAheadOfUTC);
    return dt;
}

inline
Datetime Datetime::createDatetime(unsigned     year,
                                  unsigned     month,
                                  unsigned     day,
                                  unsigned     hours,
                                  unsigned     minutes,
                                  unsigned     seconds,
                                  Nanoseconds  fractionOfSecond,
                                  Offset       offset)
{
    Datetime dt(year, month, day, hours, minutes, seconds, fractionOfSecond);
    dt.setOffset(offset.d_minutesAheadOfUTC);
    return dt;
}

inline
Datetime Datetime::createDatetime(unsigned     year,
                                  unsigned     month,
                                  unsigned     day,
                                  unsigned     hours,
                                  unsigned     minutes,
                                  unsigned     seconds,
                                  Picoseconds  fractionOfSecond,
                                  Offset       offset)
{
    Datetime dt(year, month, day, hours, minutes, seconds, fractionOfSecond);
    dt.setOffset(offset.d_minutesAheadOfUTC);
    return dt;
}

inline
Datetime Datetime::createDate(unsigned year,
                              unsigned month,
                              unsigned day)
{
    return Datetime(year, month, day);
}

inline
Datetime Datetime::createTime(unsigned hours,
                              unsigned minutes,
                              unsigned seconds)
{
    return Datetime(hours, minutes, seconds, TimeTag());
}

inline
Datetime Datetime::createTime(unsigned hours,
                              unsigned minutes,
                              unsigned seconds,
                              Offset   offset)
{
    Datetime dt(hours, minutes, seconds, TimeTag());
    dt.setOffset(offset.d_minutesAheadOfUTC);
    return dt;
}

inline
Datetime Datetime::createTime(unsigned hours,
                              unsigned minutes,
                              unsigned seconds,
                              unsigned milliseconds)
{
    return Datetime(hours, minutes, seconds, milliseconds);
}

inline

Datetime Datetime::createTime(unsigned hours,
                              unsigned minutes,
                              unsigned seconds,
                              unsigned milliseconds,
                              Offset   offset)
{
    Datetime dt(hours, minutes, seconds, milliseconds);
    dt.setOffset(offset.d_minutesAheadOfUTC);
    return dt;
}

inline
Datetime Datetime::createTime(unsigned     hours,
                              unsigned     minutes,
                              unsigned     seconds,
                              Milliseconds fractionOfSecond)
{
    return Datetime(hours, minutes, seconds, fractionOfSecond);
}

inline
Datetime Datetime::createTime(unsigned     hours,
                              unsigned     minutes,
                              unsigned     seconds,
                              Microseconds fractionOfSecond)
{
    return Datetime(hours, minutes, seconds, fractionOfSecond);
}

inline
Datetime Datetime::createTime(unsigned     hours,
                              unsigned     minutes,
                              unsigned     seconds,
                              Nanoseconds  fractionOfSecond)
{
    return Datetime(hours, minutes, seconds, fractionOfSecond);
}

inline
Datetime Datetime::createTime(unsigned     hours,
                              unsigned     minutes,
                              unsigned     seconds,
                              Picoseconds  fractionOfSecond)
{
    return Datetime(hours, minutes, seconds, fractionOfSecond);
}

inline
Datetime Datetime::createTime(unsigned     hours,
                              unsigned     minutes,
                              unsigned     seconds,
                              Milliseconds fractionOfSecond,
                              Offset       offset)
{
    Datetime dt(hours, minutes, seconds, fractionOfSecond);
    dt.setOffset(offset.d_minutesAheadOfUTC);
    return dt;
}

inline
Datetime Datetime::createTime(unsigned     hours,
                              unsigned     minutes,
                              unsigned     seconds,
                              Microseconds fractionOfSecond,
                              Offset       offset)
{
    Datetime dt(hours, minutes, seconds, fractionOfSecond);
    dt.setOffset(offset.d_minutesAheadOfUTC);
    return dt;
}

inline
Datetime Datetime::createTime(unsigned     hours,
                              unsigned     minutes,
                              unsigned     seconds,
                              Nanoseconds  fractionOfSecond,
                              Offset       offset)
{
    Datetime dt(hours, minutes, seconds, fractionOfSecond);
    dt.setOffset(offset.d_minutesAheadOfUTC);
    return dt;
}

inline
Datetime Datetime::createTime(unsigned     hours,
                              unsigned     minutes,
                              unsigned     seconds,
                              Picoseconds  fractionOfSecond,
                              Offset       offset)
{
    Datetime dt(hours, minutes, seconds, fractionOfSecond);
    dt.setOffset(offset.d_minutesAheadOfUTC);
    return dt;
}


inline
Datetime::Datetime()
{
    std::memset(&d_value, 0, sizeof(d_value));
    d_value.datetime.year = 1;
    d_value.datetime.month = 1;
    d_value.datetime.day = 1;
}

inline
Datetime::Datetime(const Datetime& original)
: d_value(original.d_value)
{
}

inline
Datetime::Datetime(const blpapi_Datetime_t& newRawValue)
{
    d_value.datetime = newRawValue;
    d_value.picoseconds = 0;
}

inline
Datetime::Datetime(const blpapi_HighPrecisionDatetime_t& newRawValue)
: d_value(newRawValue)
{
}

inline
Datetime::Datetime(unsigned newYear,
                   unsigned newMonth,
                   unsigned newDay,
                   unsigned newHours,
                   unsigned newMinutes,
                   unsigned newSeconds)
{
    d_value.datetime.offset = 0;
    d_value.datetime.year = static_cast<blpapi_UInt16_t>(newYear);
    d_value.datetime.month = static_cast<blpapi_UChar_t>(newMonth);
    d_value.datetime.day = static_cast<blpapi_UChar_t>(newDay);
    d_value.datetime.hours = static_cast<blpapi_UChar_t>(newHours);
    d_value.datetime.minutes = static_cast<blpapi_UChar_t>(newMinutes);
    d_value.datetime.seconds = static_cast<blpapi_UChar_t>(newSeconds);
    d_value.datetime.milliSeconds = 0;
    d_value.picoseconds = 0;
    d_value.datetime.parts = DatetimeParts::DATE | DatetimeParts::TIME;
}

inline
Datetime::Datetime(unsigned newYear,
                   unsigned newMonth,
                   unsigned newDay,
                   unsigned newHours,
                   unsigned newMinutes,
                   unsigned newSeconds,
                   unsigned newMilliSeconds)
{
    d_value.datetime.offset = 0;
    d_value.datetime.year = static_cast<blpapi_UInt16_t>(newYear);
    d_value.datetime.month = static_cast<blpapi_UChar_t>(newMonth);
    d_value.datetime.day = static_cast<blpapi_UChar_t>(newDay);
    d_value.datetime.hours = static_cast<blpapi_UChar_t>(newHours);
    d_value.datetime.minutes = static_cast<blpapi_UChar_t>(newMinutes);
    d_value.datetime.seconds = static_cast<blpapi_UChar_t>(newSeconds);
    d_value.datetime.milliSeconds
        = static_cast<blpapi_UInt16_t>(newMilliSeconds);
    d_value.picoseconds = 0;
    d_value.datetime.parts
        = DatetimeParts::DATE | DatetimeParts::TIMEFRACSECONDS;
}

inline
Datetime::Datetime(unsigned     newYear,
                   unsigned     newMonth,
                   unsigned     newDay,
                   unsigned     newHours,
                   unsigned     newMinutes,
                   unsigned     newSeconds,
                   Milliseconds fractionOfSecond)
{
    d_value.datetime.offset = 0;
    d_value.datetime.year = static_cast<blpapi_UInt16_t>(newYear);
    d_value.datetime.month = static_cast<blpapi_UChar_t>(newMonth);
    d_value.datetime.day = static_cast<blpapi_UChar_t>(newDay);
    d_value.datetime.hours = static_cast<blpapi_UChar_t>(newHours);
    d_value.datetime.minutes = static_cast<blpapi_UChar_t>(newMinutes);
    d_value.datetime.seconds = static_cast<blpapi_UChar_t>(newSeconds);
    d_value.datetime.milliSeconds
        = static_cast<blpapi_UInt16_t>(fractionOfSecond.d_msec);
    d_value.picoseconds = 0;
    d_value.datetime.parts
        = DatetimeParts::DATE | DatetimeParts::TIMEFRACSECONDS;
}

inline
Datetime::Datetime(unsigned     newYear,
                   unsigned     newMonth,
                   unsigned     newDay,
                   unsigned     newHours,
                   unsigned     newMinutes,
                   unsigned     newSeconds,
                   Microseconds fractionOfSecond)
{
    d_value.datetime.offset = 0;
    d_value.datetime.year = static_cast<blpapi_UInt16_t>(newYear);
    d_value.datetime.month = static_cast<blpapi_UChar_t>(newMonth);
    d_value.datetime.day = static_cast<blpapi_UChar_t>(newDay);
    d_value.datetime.hours = static_cast<blpapi_UChar_t>(newHours);
    d_value.datetime.minutes = static_cast<blpapi_UChar_t>(newMinutes);
    d_value.datetime.seconds = static_cast<blpapi_UChar_t>(newSeconds);
    d_value.datetime.milliSeconds
        = static_cast<blpapi_UInt16_t>(fractionOfSecond.d_usec / 1000);
    d_value.picoseconds = (fractionOfSecond.d_usec % 1000) * 1000 * 1000;
    d_value.datetime.parts
        = DatetimeParts::DATE | DatetimeParts::TIMEFRACSECONDS;
}

inline
Datetime::Datetime(unsigned    newYear,
                   unsigned    newMonth,
                   unsigned    newDay,
                   unsigned    newHours,
                   unsigned    newMinutes,
                   unsigned    newSeconds,
                   Nanoseconds fractionOfSecond)
{
    d_value.datetime.offset = 0;
    d_value.datetime.year = static_cast<blpapi_UInt16_t>(newYear);
    d_value.datetime.month = static_cast<blpapi_UChar_t>(newMonth);
    d_value.datetime.day = static_cast<blpapi_UChar_t>(newDay);
    d_value.datetime.hours = static_cast<blpapi_UChar_t>(newHours);
    d_value.datetime.minutes = static_cast<blpapi_UChar_t>(newMinutes);
    d_value.datetime.seconds = static_cast<blpapi_UChar_t>(newSeconds);
    d_value.datetime.milliSeconds
        = static_cast<blpapi_UInt16_t>(fractionOfSecond.d_nsec / 1000 / 1000);
    d_value.picoseconds = (fractionOfSecond.d_nsec % (1000 * 1000)) * 1000;
    d_value.datetime.parts
        = DatetimeParts::DATE | DatetimeParts::TIMEFRACSECONDS;
}

inline
Datetime::Datetime(unsigned    newYear,
                   unsigned    newMonth,
                   unsigned    newDay,
                   unsigned    newHours,
                   unsigned    newMinutes,
                   unsigned    newSeconds,
                   Picoseconds fractionOfSecond)
{
    d_value.datetime.offset = 0;
    d_value.datetime.year = static_cast<blpapi_UInt16_t>(newYear);
    d_value.datetime.month = static_cast<blpapi_UChar_t>(newMonth);
    d_value.datetime.day = static_cast<blpapi_UChar_t>(newDay);
    d_value.datetime.hours = static_cast<blpapi_UChar_t>(newHours);
    d_value.datetime.minutes = static_cast<blpapi_UChar_t>(newMinutes);
    d_value.datetime.seconds = static_cast<blpapi_UChar_t>(newSeconds);
    d_value.datetime.milliSeconds = static_cast<blpapi_UInt16_t>(
                                 fractionOfSecond.d_psec / 1000 / 1000 / 1000);
    d_value.picoseconds
        = static_cast<blpapi_UInt32_t>(
              fractionOfSecond.d_psec % (1000 * 1000 * 1000));
    d_value.datetime.parts
        = DatetimeParts::DATE | DatetimeParts::TIMEFRACSECONDS;
}

inline
Datetime::Datetime(unsigned newYear,
                   unsigned newMonth,
                   unsigned newDay)
{
    std::memset(&d_value, 0, sizeof(d_value));
    setDate(newYear, newMonth, newDay);
}

inline
Datetime::Datetime(unsigned newHours,
                   unsigned newMinutes,
                   unsigned newSeconds,
                   unsigned newMilliSeconds)
{
    std::memset(&d_value, 0, sizeof(d_value));
    setTime(newHours, newMinutes, newSeconds, newMilliSeconds);
}

inline
Datetime::Datetime(unsigned     newHours,
                   unsigned     newMinutes,
                   unsigned     newSeconds,
                   Milliseconds fractionOfSecond)
{
    std::memset(&d_value, 0, sizeof(d_value));
    setTime(newHours, newMinutes, newSeconds, fractionOfSecond);
}

inline
Datetime::Datetime(unsigned     newHours,
                   unsigned     newMinutes,
                   unsigned     newSeconds,
                   Microseconds fractionOfSecond)
{
    std::memset(&d_value, 0, sizeof(d_value));
    setTime(newHours, newMinutes, newSeconds, fractionOfSecond);
}

inline
Datetime::Datetime(unsigned    newHours,
                   unsigned    newMinutes,
                   unsigned    newSeconds,
                   Nanoseconds fractionOfSecond)
{
    std::memset(&d_value, 0, sizeof(d_value));
    setTime(newHours, newMinutes, newSeconds, fractionOfSecond);
}

inline
Datetime::Datetime(unsigned    newHours,
                   unsigned    newMinutes,
                   unsigned    newSeconds,
                   Picoseconds fractionOfSecond)
{
    std::memset(&d_value, 0, sizeof(d_value));
    setTime(newHours, newMinutes, newSeconds, fractionOfSecond);
}

inline
Datetime& Datetime::operator=(const Datetime& rhs)
{
    d_value = rhs.d_value;
    return *this;
}

inline
void Datetime::setDate(unsigned newYear,
                       unsigned newMonth,
                       unsigned newDay)
{
    d_value.datetime.day   = static_cast<blpapi_UChar_t>(newDay);
    d_value.datetime.month = static_cast<blpapi_UChar_t>(newMonth);
    d_value.datetime.year  = static_cast<blpapi_UInt16_t>(newYear);
    d_value.datetime.parts |= DatetimeParts::DATE;
}

inline
void Datetime::setTime(unsigned newHours,
                       unsigned newMinutes,
                       unsigned newSeconds)
{
    d_value.datetime.hours        = static_cast<blpapi_UChar_t>(newHours);
    d_value.datetime.minutes      = static_cast<blpapi_UChar_t>(newMinutes);
    d_value.datetime.seconds      = static_cast<blpapi_UChar_t>(newSeconds);
    d_value.datetime.milliSeconds = 0;
    d_value.picoseconds           = 0;
    d_value.datetime.parts        =  (d_value.datetime.parts
                                    & ~DatetimeParts::FRACSECONDS)
                                    | DatetimeParts::TIME;
}


inline
void Datetime::setTime(unsigned newHours,
                       unsigned newMinutes,
                       unsigned newSeconds,
                       unsigned newMilliSeconds)
{
    d_value.datetime.hours        = static_cast<blpapi_UChar_t>(newHours);
    d_value.datetime.minutes      = static_cast<blpapi_UChar_t>(newMinutes);
    d_value.datetime.seconds      = static_cast<blpapi_UChar_t>(newSeconds);
    d_value.datetime.milliSeconds
        = static_cast<blpapi_UInt16_t>(newMilliSeconds);
    d_value.picoseconds           = 0;
    d_value.datetime.parts       |= DatetimeParts::TIMEFRACSECONDS;
}


inline
void Datetime::setTime(unsigned     newHours,
                       unsigned     newMinutes,
                       unsigned     newSeconds,
                       Milliseconds fractionOfSecond)
{
    d_value.datetime.hours        = static_cast<blpapi_UChar_t>(newHours);
    d_value.datetime.minutes      = static_cast<blpapi_UChar_t>(newMinutes);
    d_value.datetime.seconds      = static_cast<blpapi_UChar_t>(newSeconds);
    d_value.datetime.milliSeconds
        = static_cast<blpapi_UInt16_t>(fractionOfSecond.d_msec);
    d_value.picoseconds           = 0;
    d_value.datetime.parts       |= DatetimeParts::TIMEFRACSECONDS;
}


inline
void Datetime::setTime(unsigned     newHours,
                       unsigned     newMinutes,
                       unsigned     newSeconds,
                       Microseconds fractionOfSecond)
{
    d_value.datetime.hours        = static_cast<blpapi_UChar_t>(newHours);
    d_value.datetime.minutes      = static_cast<blpapi_UChar_t>(newMinutes);
    d_value.datetime.seconds      = static_cast<blpapi_UChar_t>(newSeconds);
    d_value.datetime.milliSeconds
        = static_cast<blpapi_UInt16_t>(fractionOfSecond.d_usec / 1000);
    d_value.picoseconds           = fractionOfSecond.d_usec % 1000
                                                            * 1000
                                                            * 1000;
    d_value.datetime.parts       |= DatetimeParts::TIMEFRACSECONDS;
}


inline
void Datetime::setTime(unsigned    newHours,
                       unsigned    newMinutes,
                       unsigned    newSeconds,
                       Nanoseconds fractionOfSecond)
{
    d_value.datetime.hours        = static_cast<blpapi_UChar_t>(newHours);
    d_value.datetime.minutes      = static_cast<blpapi_UChar_t>(newMinutes);
    d_value.datetime.seconds      = static_cast<blpapi_UChar_t>(newSeconds);
    d_value.datetime.milliSeconds
        = static_cast<blpapi_UInt16_t>(fractionOfSecond.d_nsec / 1000 / 1000);
    d_value.picoseconds           = fractionOfSecond.d_nsec % (1000 * 1000)
                                                            * 1000;
    d_value.datetime.parts       |= DatetimeParts::TIMEFRACSECONDS;
}


inline
void Datetime::setTime(unsigned    newHours,
                       unsigned    newMinutes,
                       unsigned    newSeconds,
                       Picoseconds fractionOfSecond)
{
    d_value.datetime.hours        = static_cast<blpapi_UChar_t>(newHours);
    d_value.datetime.minutes      = static_cast<blpapi_UChar_t>(newMinutes);
    d_value.datetime.seconds      = static_cast<blpapi_UChar_t>(newSeconds);
    d_value.datetime.milliSeconds
        = static_cast<blpapi_UInt16_t>(fractionOfSecond.d_psec / 1000
                                                               / 1000
                                                               / 1000);
    d_value.picoseconds
        = static_cast<blpapi_UInt32_t>(
              fractionOfSecond.d_psec % (1000 * 1000 * 1000));
    d_value.datetime.parts       |= DatetimeParts::TIMEFRACSECONDS;
}


inline
void Datetime::setOffset(short value)
{
    d_value.datetime.offset = value;
    d_value.datetime.parts |= DatetimeParts::OFFSET;
}


inline
void Datetime::setYear(unsigned value)
{
    d_value.datetime.year   = static_cast<blpapi_UInt16_t>(value);
    d_value.datetime.parts |= DatetimeParts::YEAR;
}


inline
void Datetime::setMonth(unsigned value)
{
    d_value.datetime.month  = static_cast<blpapi_UChar_t>(value);
    d_value.datetime.parts |= DatetimeParts::MONTH;
}


inline
void Datetime::setDay(unsigned value)
{
    d_value.datetime.day    = static_cast<blpapi_UChar_t>(value);
    d_value.datetime.parts |= DatetimeParts::DAY;
}


inline
void Datetime::setHours(unsigned value)
{
    d_value.datetime.hours  = static_cast<blpapi_UChar_t>(value);
    d_value.datetime.parts |= DatetimeParts::HOURS;
}


inline
void Datetime::setMinutes(unsigned value)
{
    d_value.datetime.minutes  = static_cast<blpapi_UChar_t>(value);
    d_value.datetime.parts   |= DatetimeParts::MINUTES;
}


inline
void Datetime::setSeconds(unsigned value)
{
    d_value.datetime.seconds  = static_cast<blpapi_UChar_t>(value);
    d_value.datetime.parts   |= DatetimeParts::SECONDS;
}


inline
void Datetime::setMilliseconds(unsigned value)
{
    d_value.datetime.milliSeconds = static_cast<blpapi_UInt16_t>(value);
    d_value.picoseconds           = 0;
    d_value.datetime.parts       |= DatetimeParts::FRACSECONDS;
}

inline
void Datetime::setFractionOfSecond(Milliseconds value)
{
    d_value.datetime.milliSeconds = static_cast<blpapi_UInt16_t>(value.d_msec);
    d_value.picoseconds           = 0;
    d_value.datetime.parts       |= DatetimeParts::FRACSECONDS;
}

inline
void Datetime::setFractionOfSecond(Microseconds value)
{
    d_value.datetime.milliSeconds
        = static_cast<blpapi_UInt16_t>(value.d_usec / 1000);
    d_value.picoseconds           = value.d_usec % 1000 * 1000 * 1000;
    d_value.datetime.parts       |= DatetimeParts::FRACSECONDS;
}

inline
void Datetime::setFractionOfSecond(Nanoseconds value)
{
    d_value.datetime.milliSeconds
        = static_cast<blpapi_UInt16_t>(value.d_nsec / 1000 / 1000);
    d_value.picoseconds           = value.d_nsec % (1000 * 1000) * 1000;
    d_value.datetime.parts       |= DatetimeParts::FRACSECONDS;
}

inline
void Datetime::setFractionOfSecond(Picoseconds value)
{
    d_value.datetime.milliSeconds
        = static_cast<blpapi_UInt16_t>(value.d_psec / 1000 / 1000 / 1000);
    d_value.picoseconds
        = static_cast<blpapi_UInt32_t>(value.d_psec % (1000 * 1000 * 1000));
    d_value.datetime.parts       |= DatetimeParts::FRACSECONDS;
}

inline
blpapi_Datetime_t& Datetime::rawValue()
{
    return d_value.datetime;
}

inline
blpapi_HighPrecisionDatetime_t& Datetime::rawHighPrecisionValue()
{
    return d_value;
}

inline
bool Datetime::hasParts(unsigned newParts) const
{
    return newParts == (d_value.datetime.parts & newParts);
}

inline
unsigned Datetime::parts() const
{
    return d_value.datetime.parts;
}

inline
unsigned Datetime::year() const
{
    return d_value.datetime.year;
}

inline
unsigned Datetime::month() const
{
    return d_value.datetime.month;
}

inline
unsigned Datetime::day() const
{
    return d_value.datetime.day;
}

inline
unsigned Datetime::hours() const
{
    return d_value.datetime.hours;
}

inline
unsigned Datetime::minutes() const
{
    return d_value.datetime.minutes;
}

inline
unsigned Datetime::seconds() const
{
    return d_value.datetime.seconds;
}

inline
unsigned Datetime::milliSeconds() const
{
    return d_value.datetime.milliSeconds;
}

inline
unsigned Datetime::milliseconds() const
{
    return d_value.datetime.milliSeconds;
}

inline
unsigned Datetime::microseconds() const
{
    return   d_value.datetime.milliSeconds * 1000
           + d_value.picoseconds / 1000 / 1000;
}

inline
unsigned Datetime::nanoseconds() const
{
    return   d_value.datetime.milliSeconds * 1000 * 1000
           + d_value.picoseconds / 1000;
}

inline
unsigned long long Datetime::picoseconds() const
{
    return   d_value.datetime.milliSeconds * 1000LL * 1000 * 1000
           + d_value.picoseconds;
}

inline
short Datetime::offset() const
{
    return d_value.datetime.offset;
}

inline
const blpapi_Datetime_t& Datetime::rawValue() const
{
    return d_value.datetime;
}

inline
const blpapi_HighPrecisionDatetime_t& Datetime::rawHighPrecisionValue() const
{
    return d_value;
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
    if (   hasParts(DatetimeParts::FRACSECONDS)
        && (picoseconds() >= 1000LL * 1000 * 1000 * 1000)) {
        return false;
    }
    return true;
}

inline
std::ostream& Datetime::print(std::ostream& stream,
                              int           level,
                              int           spacesPerLevel) const
{
    BLPAPI_CALL_HIGHPRECISIONDATETIME_PRINT(&d_value,
                                            StreamProxyOstream::writeToStream,
                                            &stream,
                                            level,
                                            spacesPerLevel);
    return stream;
}

inline
bool operator==(const Datetime& lhs, const Datetime& rhs)
{
    if (lhs.parts() == rhs.parts()) {
        return (BLPAPI_CALL_HIGHPRECISIONDATETIME_COMPARE(
                                                  &lhs.rawHighPrecisionValue(),
                                                  &rhs.rawHighPrecisionValue())
                == 0);
    }
    return false;
}


inline
bool operator!=(const Datetime& lhs, const Datetime& rhs)
{
    return !(lhs == rhs);
}

inline
bool operator<(const Datetime& lhs, const Datetime& rhs)
{
    return (BLPAPI_CALL_HIGHPRECISIONDATETIME_COMPARE(
                                              &lhs.rawHighPrecisionValue(),
                                              &rhs.rawHighPrecisionValue())
            < 0);
}

inline
bool operator<=(const Datetime& lhs, const Datetime& rhs)
{
    return !(rhs < lhs);
}

inline
bool operator>(const Datetime& lhs, const Datetime& rhs)
{
    return rhs < lhs;
}

inline
bool operator>=(const Datetime& lhs, const Datetime& rhs)
{
    return !(lhs < rhs);
}

inline
std::ostream& operator<<(std::ostream& stream, const Datetime& datetime)
{
    return datetime.print(stream, 0, -1);
}

                            // ------------------
                            // class DatetimeUtil
                            // ------------------

inline
Datetime DatetimeUtil::fromTimePoint(const TimePoint& timePoint,
                                     Datetime::Offset offset)
{
    blpapi_HighPrecisionDatetime_t highPrecisionDatetime;
    BLPAPI_CALL_HIGHPRECISIONDATETIME_FROMTIMEPOINT(
                      &highPrecisionDatetime,
                      &timePoint,
                      offset.d_minutesAheadOfUTC);
    return Datetime(highPrecisionDatetime);
}

}  // close namespace blpapi
}  // close namespace BloombergLP

#endif // #ifdef __cplusplus

#endif // #ifndef INCLUDED_BLPAPI_DATETIME
