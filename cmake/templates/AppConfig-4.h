/*


*/

#ifndef __JUCE_APPCONFIG_@upper_project_id@__
#define __JUCE_APPCONFIG_@upper_project_id@__

//==============================================================================
// [BEGIN_USER_CODE_SECTION]@user_code_section@// [END_USER_CODE_SECTION]

//==============================================================================
@module_available_defines@
//==============================================================================
#ifndef    JUCE_STANDALONE_APPLICATION
 #ifdef JucePlugin_Build_Standalone
  #define  JUCE_STANDALONE_APPLICATION JucePlugin_Build_Standalone
 #else
  #define  JUCE_STANDALONE_APPLICATION @is_standalone_application@
 #endif
#endif

#define JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED 1
@config_flags_defines@
@audio_plugin_settings_defines@
#endif  // __JUCE_APPCONFIG_@upper_project_id@__
