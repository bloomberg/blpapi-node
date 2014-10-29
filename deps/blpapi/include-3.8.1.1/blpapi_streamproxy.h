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
// blpapi_streamproxy.h                                               -*-C++-*-
#ifndef INCLUDED_BLPAPI_STREAMPROXY
#define INCLUDED_BLPAPI_STREAMPROXY

//@PURPOSE: A signature for callback on print and default C++ implementation
//
//@DESCRIPTION:
// This file defines 'blpapi_StreamWriter_t' a function pointer type.
// The user of the C API need to specify a callback of above type which will
// be called on xxx_print(...) with the formatted data. For C++ API, a default
// callback is specified which writes data to the stream specified in
// xxx::print

typedef int(*blpapi_StreamWriter_t)(const char* data,
                                    int length,
                                    void *stream);

#ifdef __cplusplus

#include <ostream>

namespace BloombergLP {
namespace blpapi {

static int OstreamWriter(const char* data, int length, void *stream)
{
    reinterpret_cast<std::ostream*>(stream)->write(data, length);
    return 0;
}

}  // close namespace blpapi
}  // close namespace BloombergLP

#endif // #ifdef __cplusplus
#endif // #ifndef INCLUDED_BLPAPI_STREAMPROXY
