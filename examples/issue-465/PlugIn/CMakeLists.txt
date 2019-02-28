#
# Reprojucer part
#

jucer_project_begin()

jucer_project_settings(
  PROJECT_NAME "PlugIn"
  PROJECT_VERSION "1.0.0"
  PROJECT_TYPE "Audio Plug-in"
  BUNDLE_IDENTIFIER "com.yourcompany.PlugIn"
  PREPROCESSOR_DEFINITIONS
    JUCE_VST3_CAN_REPLACE_VST2=0
)

jucer_audio_plugin_settings(
  PLUGIN_FORMATS
    "VST3"
    "AU"
    "Standalone"
  # PLUGIN_CHARACTERISTICS
  PLUGIN_NAME "PlugIn"
  PLUGIN_DESCRIPTION "PlugIn"
  PLUGIN_MANUFACTURER "yourcompany"
  PLUGIN_MANUFACTURER_CODE "Manu"
  PLUGIN_CODE "Icsm"
  # PLUGIN_CHANNEL_CONFIGURATIONS
  PLUGIN_AU_EXPORT_PREFIX "PlugInAU"
  # PLUGIN_AU_MAIN_TYPE
  PLUGIN_VST3_CATEGORY
    "Fx"
  PLUGIN_VST_LEGACY_CATEGORY "kPlugCategEffect"
)

jucer_project_files("PlugIn"
# Compile   Xcode     Binary    File
#           Resource  Resource
  x         .         .         "src/PluginEditor.cpp"
  .         .         .         "src/PluginEditor.h"
  x         .         .         "src/PluginProcessor.cpp"
  .         .         .         "src/PluginProcessor.h"
)

# Same JUCE config flags as dspCore and guiCore to avoid any weird inconsistencies
jucer_project_module(juce_audio_basics PATH "${JUCE_ROOT}/modules")
jucer_project_module(juce_audio_devices PATH "${JUCE_ROOT}/modules")
jucer_project_module(juce_audio_formats PATH "${JUCE_ROOT}/modules")
jucer_project_module(juce_audio_plugin_client PATH "${JUCE_ROOT}/modules")
jucer_project_module(juce_audio_processors PATH "${JUCE_ROOT}/modules")
jucer_project_module(juce_audio_utils PATH "${JUCE_ROOT}/modules")
jucer_project_module(juce_core PATH "${JUCE_ROOT}/modules"
  JUCE_STRICT_REFCOUNTEDPOINTER ON
)
jucer_project_module(juce_data_structures PATH "${JUCE_ROOT}/modules")
jucer_project_module(juce_dsp PATH "${JUCE_ROOT}/modules")
jucer_project_module(juce_events PATH "${JUCE_ROOT}/modules")
jucer_project_module(juce_graphics PATH "${JUCE_ROOT}/modules")
jucer_project_module(juce_gui_basics PATH "${JUCE_ROOT}/modules")
jucer_project_module(juce_gui_extra PATH "${JUCE_ROOT}/modules")

jucer_export_target("Xcode (MacOSX)")
jucer_export_target_configuration("Xcode (MacOSX)" NAME "Debug" DEBUG_MODE ON)
jucer_export_target_configuration("Xcode (MacOSX)" NAME "Release" DEBUG_MODE OFF)

jucer_export_target("Visual Studio 2017")
jucer_export_target_configuration("Visual Studio 2017" NAME "Debug" DEBUG_MODE ON)
jucer_export_target_configuration("Visual Studio 2017" NAME "Release" DEBUG_MODE OFF)

jucer_export_target("Visual Studio 2015")
jucer_export_target_configuration("Visual Studio 2015" NAME "Debug" DEBUG_MODE ON)
jucer_export_target_configuration("Visual Studio 2015" NAME "Release" DEBUG_MODE OFF)

jucer_export_target("Linux Makefile")
jucer_export_target_configuration("Linux Makefile" NAME "Debug" DEBUG_MODE ON)
jucer_export_target_configuration("Linux Makefile" NAME "Release" DEBUG_MODE OFF)

jucer_project_end()


#
# Standard CMake part
#

target_link_libraries(PlugIn_Shared_Code PUBLIC dspCore guiCore)
