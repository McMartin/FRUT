#
# Reprojucer part
#

jucer_project_begin()

jucer_project_settings(
  PROJECT_NAME "guiCoreTestHost"
  PROJECT_VERSION "1.0.0"
  PROJECT_TYPE "GUI Application"
  BUNDLE_IDENTIFIER "com.yourcompany.guiCoreTestHost"
)

jucer_project_files("guiCoreTestHost"
# Compile   Xcode     Binary    File
#           Resource  Resource
  .         .         .         "src/MainComponent.h"
  x         .         .         "src/MainComponent.cpp"
  x         .         .         "src/Main.cpp"
)

# Same JUCE modules (with same config flags) as guiCore
jucer_project_module(juce_core PATH "${JUCE_ROOT}/modules"
  JUCE_STRICT_REFCOUNTEDPOINTER ON
)
jucer_project_module(juce_data_structures PATH "${JUCE_ROOT}/modules")
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

target_link_libraries(guiCoreTestHost PRIVATE guiCore)
