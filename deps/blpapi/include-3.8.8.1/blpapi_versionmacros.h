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
// blpapi_versionmacros.h                                             -*-C++-*-
#ifndef INCLUDED_BLPAPI_VERSIONMACROS
#define INCLUDED_BLPAPI_VERSIONMACROS

//@PURPOSE: Provide preprocessor macros for BLPAPI library version information.
//
//@DESCRIPTION: This file is not meant to be included directly; see
// 'blpapi_versioninfo.h' for library version interfaces.

#define BLPAPI_VERSION_MAJOR 3
#define BLPAPI_VERSION_MINOR 8
#define BLPAPI_VERSION_PATCH 8
#define BLPAPI_VERSION_BUILD 1

#define BLPAPI_MAKE_VERSION(MAJOR, MINOR, PATCH) ((MAJOR) * 65536 + \
                                                  (MINOR) * 256 + \
                                                  (PATCH))
    // Combine the specified 'MAJOR', 'MINOR', and 'PATCH' values to form
    // a single integer that can be used for comparisons at compile time.

#define BLPAPI_SDK_VERSION BLPAPI_MAKE_VERSION(BLPAPI_VERSION_MAJOR, \
                                               BLPAPI_VERSION_MINOR, \
                                               BLPAPI_VERSION_PATCH)
    // Form a single integer representing the version of the BLPAPI headers
    // that can be compared with values formed by 'BLPAPI_MAKE_VERSION' at
    // compile time.

#endif // INCLUDED_BLPAPI_VERSIONMACROS
