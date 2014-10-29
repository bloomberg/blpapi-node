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
// blpapi_error.h                                                     -*-C++-*-
#ifndef INCLUDED_BLPAPI_ERROR
#define INCLUDED_BLPAPI_ERROR

//@PURPOSE: Provide a collection of errors that library can return
//
//@DESCRIPTION: This component provides a collection of preprocessor macros
// that define the errors that library can generate.

#ifndef INCLUDED_BLPAPI_DEFS
#include <blpapi_defs.h>
#endif

#define BLPAPI_RESULTCODE(res) ((res) & 0xffff)
#define BLPAPI_RESULTCLASS(res) ((res) & 0xff0000)

#define BLPAPI_UNKNOWN_CLASS                0x00000
#define BLPAPI_INVALIDSTATE_CLASS           0x10000
#define BLPAPI_INVALIDARG_CLASS             0x20000
#define BLPAPI_IOERROR_CLASS                0x30000
#define BLPAPI_CNVERROR_CLASS               0x40000
#define BLPAPI_BOUNDSERROR_CLASS            0x50000
#define BLPAPI_NOTFOUND_CLASS               0x60000
#define BLPAPI_FLDNOTFOUND_CLASS            0x70000
#define BLPAPI_UNSUPPORTED_CLASS            0x80000

#define BLPAPI_ERROR_UNKNOWN                 (BLPAPI_UNKNOWN_CLASS | 1)
#define BLPAPI_ERROR_ILLEGAL_ARG             (BLPAPI_INVALIDARG_CLASS | 2)
#define BLPAPI_ERROR_ILLEGAL_ACCESS          (BLPAPI_UNKNOWN_CLASS | 3)
#define BLPAPI_ERROR_INVALID_SESSION         (BLPAPI_INVALIDARG_CLASS | 4)
#define BLPAPI_ERROR_DUPLICATE_CORRELATIONID (BLPAPI_INVALIDARG_CLASS | 5)
#define BLPAPI_ERROR_INTERNAL_ERROR          (BLPAPI_UNKNOWN_CLASS | 6)
#define BLPAPI_ERROR_RESOLVE_FAILED          (BLPAPI_IOERROR_CLASS | 7)
#define BLPAPI_ERROR_CONNECT_FAILED          (BLPAPI_IOERROR_CLASS | 8)
#define BLPAPI_ERROR_ILLEGAL_STATE           (BLPAPI_INVALIDSTATE_CLASS| 9)
#define BLPAPI_ERROR_CODEC_FAILURE           (BLPAPI_UNKNOWN_CLASS | 10)
#define BLPAPI_ERROR_INDEX_OUT_OF_RANGE      (BLPAPI_BOUNDSERROR_CLASS | 11)
#define BLPAPI_ERROR_INVALID_CONVERSION      (BLPAPI_CNVERROR_CLASS | 12)
#define BLPAPI_ERROR_ITEM_NOT_FOUND          (BLPAPI_NOTFOUND_CLASS | 13)
#define BLPAPI_ERROR_IO_ERROR                (BLPAPI_IOERROR_CLASS | 14)
#define BLPAPI_ERROR_CORRELATION_NOT_FOUND   (BLPAPI_NOTFOUND_CLASS | 15)
#define BLPAPI_ERROR_SERVICE_NOT_FOUND       (BLPAPI_NOTFOUND_CLASS | 16)
#define BLPAPI_ERROR_LOGON_LOOKUP_FAILED     (BLPAPI_UNKNOWN_CLASS | 17)
#define BLPAPI_ERROR_DS_LOOKUP_FAILED        (BLPAPI_UNKNOWN_CLASS | 18)
#define BLPAPI_ERROR_UNSUPPORTED_OPERATION   (BLPAPI_UNSUPPORTED_CLASS | 19)
#define BLPAPI_ERROR_DS_PROPERTY_NOT_FOUND   (BLPAPI_NOTFOUND_CLASS | 20)

#ifdef __cplusplus
extern "C" {
#endif

BLPAPI_EXPORT
const char* blpapi_getLastErrorDescription(int resultCode);

#ifdef __cplusplus
}

namespace BloombergLP {
namespace blpapi {

}  // close namespace blpapi
}  // close namespace BloombergLP

#endif


#endif // #ifndef INCLUDED_BLPAPI_ERROR
