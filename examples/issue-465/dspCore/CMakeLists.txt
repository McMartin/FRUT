#
# Reprojucer part
#

jucer_project_begin()

jucer_project_settings(
  PROJECT_NAME "dspCore"
  PROJECT_VERSION "1.0.0"
  PROJECT_TYPE "Static Library"
  BUNDLE_IDENTIFIER "com.yourcompany.dspCore"
)

jucer_project_files("dspCore"
# Compile   Xcode     Binary    File
#           Resource  Resource
  .         .         .         "include/dspCore/Reverb.hpp"
  x         .         .         "src/Reverb.cpp"
)

jucer_project_module(juce_audio_basics PATH "${JUCE_ROOT}/modules")
jucer_project_module(juce_audio_formats PATH "${JUCE_ROOT}/modules")
jucer_project_module(juce_core PATH "${JUCE_ROOT}/modules"
  JUCE_STRICT_REFCOUNTEDPOINTER ON
)
jucer_project_module(juce_dsp PATH "${JUCE_ROOT}/modules")

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

target_include_directories(dspCore PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")
