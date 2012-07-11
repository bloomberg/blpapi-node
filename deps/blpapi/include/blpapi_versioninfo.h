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
// blpapi_versioninfo.h                                               -*-C++-*-
#ifndef INCLUDED_BLPAPI_VERSIONINFO
#define INCLUDED_BLPAPI_VERSIONINFO

//@PURPOSE: Provide BLPAPI SDK library version information.
//
//@CLASSES:
// blpapi::VersionInfo: basic BLPAPI SDK library version attribute class
//
//@DESCRIPTION: This component provides access the BLPAPI SDK library version
// information.
//
///Usage
///-----
///C++ usage
///- - - - -
// This example logs the BLPAPI version in order to make the logs analysis more
// productive:
//..
//  blpapi::VersionInfo blpapiVersion;
//  // Client log the library version for reference and future debugging
//  log("BLPAPI version: %d.%d.%d.%d",
//      blpapiVersion.majorVersion(),
//      blpapiVersion.minorVersion(),
//      blpapiVersion.patchVersion(),
//      blpapiVersion.buildVersion());
//..
//
///C usage
///- - - -
// This example logs the BLPAPI version in order to make the logs analysis more
// productive:
//..
//  int majorVersion, minorVersion, patchVersion, buildVersion;
//  blpapi_getVersionInfo(&majorVersion, &minorVersion, &patchVersion,
//      &buildVersion);
//  // Client log the library version for reference and future debugging
//  log("BLPAPI version: %d.%d.%d.%d", majorVersion, minorVersion,
//      patchVersion, buildVersion);
//..
//
//
// This example provide shows how to use the backward API signature
// compatibility macros BLPAPI_SDK_VERSION and BLPAPI_MAKE_VERSION (see
// blpapi_dispatchtbl.h):
//..
//  #if BLPAPI_SDK_VERSION >= BLPAPI_MAKE_VERSION(3, 3, 7)
//      // Do version 3.3.7 specific stuff here (e.g. adding functions with old
//      // signature to an array so a client code that was compiled with
//      // version 3.3.7 will still be able to run using the new version.
//      ...
//  #elif BLPAPI_SDK_VERSION ...
//..

#ifndef INCLUDED_BLPAPI_DEFS
#include <blpapi_defs.h>
#endif

#define BLPAPI_VERSION_MAJOR 3
#define BLPAPI_VERSION_MINOR 4
#define BLPAPI_VERSION_PATCH 8
#define BLPAPI_VERSION_BUILD 1

#define BLPAPI_MAKE_VERSION(MAJOR, MINOR, PATCH) ((MAJOR) * 10000 + \
                                                  (MINOR) * 100 + \
                                                  (PATCH))

#define BLPAPI_SDK_VERSION BLPAPI_MAKE_VERSION(BLPAPI_VERSION_MAJOR, \
                                               BLPAPI_VERSION_MINOR, \
                                               BLPAPI_VERSION_PATCH)

#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus



BLPAPI_EXPORT
void blpapi_getVersionInfo(int *majorVersion,
                           int *minorVersion,
                           int *patchVersion,
                           int *buildVersion);
    // Initialize the 4 parameters with the current release version information
    // which include the following version parts: Major, Minor, Patch, and
    // Build.
#ifdef __cplusplus
}

namespace BloombergLP {

namespace blpapi {
                             // =================
                             // class VersionInfo
                             // =================
class VersionInfo {
    // This class provides a basic primitive version information for the BLPAPI
    // SDK library.  It provides a simple way to access the following version
    // information as 4 integers: Major, Minor, Patch, and Build.

  private:
    // INSTANCE DATA
    int d_major; // BLPAPI SDK library major version
    int d_minor; // BLPAPI SDK library minor version
    int d_patch; // BLPAPI SDK library patch version
    int d_build; // BLPAPI SDK library build version

  public:
    // CREATORS
    VersionInfo();
        // Creates a version identifier object.

    // ACCESSORS
    int majorVersion() const;
        // Returns the BLPAPI SDK library version first identifier, *Major*, as
        // integer.
    int minorVersion() const;
        // Returns the BLPAPI SDK library version second identifier, *Minor*,
        // as integer.
    int patchVersion() const;
        // Returns the BLPAPI SDK library version third identifier, *Patch*, as
        // integer.
    int buildVersion() const;
        // Returns the BLPAPI SDK library version fourth identifier, *Build*,
        // as integer.
};

//=============================================================================
//                         INLINE FUNCTION DEFINITIONS
//=============================================================================

                             // -----------------
                             // class VersionInfo
                             // -----------------
// CREATORS
inline
VersionInfo::VersionInfo()
{
    blpapi_getVersionInfo(&d_major, &d_minor, &d_patch, &d_build);
}

// ACCESSORS
inline
int VersionInfo::majorVersion() const
{
    return d_major;
}

inline
int VersionInfo::minorVersion() const
{
    return d_minor;
}

inline
int VersionInfo::patchVersion() const
{
    return d_patch;
}

inline
int VersionInfo::buildVersion() const
{
    return d_build;
}

}  // close namespace blpapi
}  // close namespace BloombergLP

#endif // ifdef __cplusplus

#endif // INCLUDED_BLPAPI_VERSIONINFO
