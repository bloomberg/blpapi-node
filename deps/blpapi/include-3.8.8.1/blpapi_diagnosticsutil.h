/* Copyright 2013. Bloomberg Finance L.P.
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
// blpapi_diagnosticsutil.h                                           -*-C++-*-
#ifndef INCLUDED_BLPAPI_DIAGNOSTICSUTIL
#define INCLUDED_BLPAPI_DIAGNOSTICSUTIL

//@PURPOSE: Provide api to access diagnostics information on the blpapi library
//
//@CLASSES:
// blpapi::DiagnosticsUtil: collection of diagnostics information functions
//
//@DESCRIPTION: This component provide a collection of functions which give
// access to various sets of diagnostics information on the 'blpapi' library.


#ifndef INCLUDED_BLPAPI_CALL
#include <blpapi_call.h>
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
int blpapi_DiagnosticsUtil_memoryInfo(char   *buffer,
                                      size_t  bufferLength);

#ifdef __cplusplus
}

namespace BloombergLP {
namespace blpapi {
                         // =====================
                         // class DiagnosticsUtil
                         // =====================

class DiagnosticsUtil {
  public:
    static int memoryInfo(char   *buffer,
                          size_t  bufferLength);
        // Load into the specified 'buffer' up to the specified 'bufferLength'
        // characters of a null-terminated string describing the 'blpapi'
        // library's memory usage; the format of the string is
        // platform-specific.  On success, return the number of characters
        // required to output the entire string, not including the null
        // terminator; otherwise return a negative value.  Note that if the
        // value returned from this function is greater than or equal to
        // 'bufferLength' then the output is truncated and is *not*
        // null-terminated.
};


                        // ---------------------
                        // class DiagnosticsUtil
                        // ---------------------

inline
int DiagnosticsUtil::memoryInfo(char   *buffer,
                                size_t  bufferLength)
{
    BLPAPI_CALL_DIAGNOSTICSUTIL_MEMORYINFO(buffer, bufferLength);
}

}
}

#endif

#endif
