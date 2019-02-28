#
# Reprojucer part
#

jucer_project_begin()

jucer_project_settings(
  PROJECT_NAME "dspCoreTestHost"
  PROJECT_VERSION "1.0.0"
  PROJECT_TYPE "Console Application"
  BUNDLE_IDENTIFIER "com.yourcompany.dspCoreTestHost"
)

jucer_project_files("dspCoreTestHost"
# Compile   Xcode     Binary    File
#           Resource  Resource
  x         .         .         "src/tst_Reverb.cpp"
)

# Same JUCE modules (with same config flags) as dspCore
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

target_link_libraries(dspCoreTestHost PRIVATE
  dspCore
  GTest::Main  # or GTest::GTest if you want to provide your custom main() function
)
