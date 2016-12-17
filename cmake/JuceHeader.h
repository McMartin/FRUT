#ifndef __APPHEADERFILE__
#define __APPHEADERFILE__

#include "AppConfig.h"

@modules_includes@
@binary_data_include@

#if ! DONT_SET_USING_JUCE_NAMESPACE
 // If your code uses a lot of JUCE classes, then this will obviously save you
 // a lot of typing, but can be disabled by setting DONT_SET_USING_JUCE_NAMESPACE.
 using namespace juce;
#endif

#if ! JUCE_DONT_DECLARE_PROJECTINFO
namespace ProjectInfo
{
    const char* const  projectName    = "@JUCER_PROJECT_NAME@";
    const char* const  versionString  = "1.0.0";
    const int          versionNumber  = 0x10000;
}
#endif

#endif   // __APPHEADERFILE__
