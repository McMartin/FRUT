# This file was generated by FRUT's Jucer2CMake from "consoleapp543.jucer"

cmake_minimum_required(VERSION 3.4)

project("consoleapp543")


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../../../cmake")
include(Reprojucer)


set(consoleapp543_jucer_FILE
  "${CMAKE_CURRENT_LIST_DIR}/consoleapp543.jucer"
)


jucer_project_begin(
  JUCER_VERSION "5.4.3"
  PROJECT_FILE "${consoleapp543_jucer_FILE}"
  PROJECT_ID "ybulR4"
)

jucer_project_settings(
  PROJECT_NAME "consoleapp543"
  PROJECT_VERSION "1.0.0"
  REPORT_JUCE_APP_USAGE OFF # Required for closed source applications without an Indie or Pro JUCE license
  DISPLAY_THE_JUCE_SPLASH_SCREEN OFF # Required for closed source applications without an Indie or Pro JUCE license
  PROJECT_TYPE "Console Application"
  BUNDLE_IDENTIFIER "com.yourcompany.consoleapp543"
  CXX_LANGUAGE_STANDARD "C++14"
)

jucer_project_files("consoleapp543/Source"
# Compile   Xcode     Binary    File
#           Resource  Resource
  x         .         .         "Source/foo.cpp"
)

jucer_export_target(
  "Xcode (MacOSX)"
)

jucer_export_target_configuration(
  "Xcode (MacOSX)"
  NAME "Debug"
  DEBUG_MODE ON
)

jucer_export_target_configuration(
  "Xcode (MacOSX)"
  NAME "Release"
  DEBUG_MODE OFF
)

jucer_export_target(
  "Visual Studio 2017"
)

jucer_export_target_configuration(
  "Visual Studio 2017"
  NAME "Debug"
  DEBUG_MODE ON
)

jucer_export_target_configuration(
  "Visual Studio 2017"
  NAME "Release"
  DEBUG_MODE OFF
)

jucer_export_target(
  "Visual Studio 2015"
)

jucer_export_target_configuration(
  "Visual Studio 2015"
  NAME "Debug"
  DEBUG_MODE ON
)

jucer_export_target_configuration(
  "Visual Studio 2015"
  NAME "Release"
  DEBUG_MODE OFF
)

jucer_export_target(
  "Visual Studio 2013"
)

jucer_export_target_configuration(
  "Visual Studio 2013"
  NAME "Debug"
  DEBUG_MODE ON
)

jucer_export_target_configuration(
  "Visual Studio 2013"
  NAME "Release"
  DEBUG_MODE OFF
)

jucer_export_target(
  "Linux Makefile"
)

jucer_export_target_configuration(
  "Linux Makefile"
  NAME "Debug"
  DEBUG_MODE ON
)

jucer_export_target_configuration(
  "Linux Makefile"
  NAME "Release"
  DEBUG_MODE OFF
)

jucer_export_target(
  "Code::Blocks (Windows)"
)

jucer_export_target_configuration(
  "Code::Blocks (Windows)"
  NAME "Debug"
  DEBUG_MODE ON
)

jucer_export_target_configuration(
  "Code::Blocks (Windows)"
  NAME "Release"
  DEBUG_MODE OFF
)

jucer_export_target(
  "Code::Blocks (Linux)"
)

jucer_export_target_configuration(
  "Code::Blocks (Linux)"
  NAME "Debug"
  DEBUG_MODE ON
)

jucer_export_target_configuration(
  "Code::Blocks (Linux)"
  NAME "Release"
  DEBUG_MODE OFF
)

jucer_project_end()
