#ifndef __APPHEADERFILE_@upper_project_id@__
#define __APPHEADERFILE_@upper_project_id@__

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
    const char* const  versionString  = "@JUCER_PROJECT_VERSION@";
    const int          versionNumber  = @JUCER_PROJECT_VERSION_AS_HEX@;
}
#endif

#endif   // __APPHEADERFILE_@upper_project_id@__
