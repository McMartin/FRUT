# This file was generated by FRUT's Jucer2CMake from "MidiTest.jucer"

cmake_minimum_required(VERSION 3.4)

project("MidiTest")


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../../../cmake")
include(Reprojucer)


if(NOT DEFINED MidiTest_jucer_FILE)
  message(FATAL_ERROR "MidiTest_jucer_FILE must be defined")
endif()

get_filename_component(MidiTest_jucer_FILE
  "${MidiTest_jucer_FILE}" ABSOLUTE
  BASE_DIR "${CMAKE_BINARY_DIR}"
)


jucer_project_begin(
  JUCER_VERSION "5.0.0"
  PROJECT_FILE "${MidiTest_jucer_FILE}"
  PROJECT_ID "wHE0ay"
)

jucer_project_settings(
  PROJECT_NAME "MidiTest"
  PROJECT_VERSION "1.0.0"
  REPORT_JUCE_APP_USAGE OFF # Required for closed source applications without an Indie or Pro JUCE license
  DISPLAY_THE_JUCE_SPLASH_SCREEN OFF # Required for closed source applications without an Indie or Pro JUCE license
  SPLASH_SCREEN_COLOUR "Dark"
  PROJECT_TYPE "GUI Application"
  BUNDLE_IDENTIFIER "com.yourcompany.MidiTest"
  INCLUDE_BINARYDATA ON
)

jucer_project_files("MidiTest/Source"
# Compile   Xcode     Binary    File
#           Resource  Resource
  x         .         .         "Source/Main.cpp"
  x         .         .         "Source/MainComponent.cpp"
  .         .         .         "Source/MainComponent.h"
)

jucer_project_module(
  juce_audio_basics
  PATH "../../modules"
)

jucer_project_module(
  juce_audio_devices
  PATH "../../modules"
  # JUCE_ASIO
  # JUCE_WASAPI
  # JUCE_WASAPI_EXCLUSIVE
  # JUCE_DIRECTSOUND
  # JUCE_ALSA
  # JUCE_JACK
  # JUCE_USE_ANDROID_OPENSLES
  # JUCE_USE_WINRT_MIDI
)

jucer_project_module(
  juce_audio_formats
  PATH "../../modules"
  # JUCE_USE_FLAC
  # JUCE_USE_OGGVORBIS
  # JUCE_USE_MP3AUDIOFORMAT
  # JUCE_USE_LAME_AUDIO_FORMAT
  # JUCE_USE_WINDOWS_MEDIA_FORMAT
)

jucer_project_module(
  juce_audio_processors
  PATH "../../modules"
  # JUCE_PLUGINHOST_VST
  # JUCE_PLUGINHOST_VST3
  # JUCE_PLUGINHOST_AU
)

jucer_project_module(
  juce_audio_utils
  PATH "../../modules"
  # JUCE_USE_CDREADER
  # JUCE_USE_CDBURNER
)

jucer_project_module(
  juce_core
  PATH "../../modules"
  # JUCE_FORCE_DEBUG
  # JUCE_LOG_ASSERTIONS
  # JUCE_CHECK_MEMORY_LEAKS
  # JUCE_DONT_AUTOLINK_TO_WIN32_LIBRARIES
  # JUCE_INCLUDE_ZLIB_CODE
  # JUCE_USE_CURL
  # JUCE_CATCH_UNHANDLED_EXCEPTIONS
  # JUCE_ALLOW_STATIC_NULL_VARIABLES
)

jucer_project_module(
  juce_data_structures
  PATH "../../modules"
)

jucer_project_module(
  juce_events
  PATH "../../modules"
  # JUCE_EXECUTE_APP_SUSPEND_ON_IOS_BACKGROUND_TASK
)

jucer_project_module(
  juce_graphics
  PATH "../../modules"
  # JUCE_USE_COREIMAGE_LOADER
  # JUCE_USE_DIRECTWRITE
)

jucer_project_module(
  juce_gui_basics
  PATH "../../modules"
  # JUCE_ENABLE_REPAINT_DEBUGGING
  # JUCE_USE_XSHM
  # JUCE_USE_XRENDER
  # JUCE_USE_XCURSOR
)

jucer_project_module(
  juce_gui_extra
  PATH "../../modules"
  # JUCE_WEB_BROWSER
  # JUCE_ENABLE_LIVE_CONSTANT_EDITOR
)

jucer_export_target(
  "Visual Studio 2015"
)

jucer_export_target_configuration(
  "Visual Studio 2015"
  NAME "Debug"
  DEBUG_MODE ON
  BINARY_NAME "MidiTest"
  OPTIMISATION "No optimisation"
  WARNING_LEVEL "High"
  GENERATE_MANIFEST ON
  # ARCHITECTURE "32-bit"
)

jucer_export_target_configuration(
  "Visual Studio 2015"
  NAME "Release"
  DEBUG_MODE OFF
  BINARY_NAME "MidiTest"
  OPTIMISATION "Maximise speed"
  WARNING_LEVEL "High"
  GENERATE_MANIFEST ON
  # ARCHITECTURE "32-bit"
)

jucer_export_target(
  "Visual Studio 2017"
)

jucer_export_target_configuration(
  "Visual Studio 2017"
  NAME "Debug"
  DEBUG_MODE ON
  BINARY_NAME "MidiTest"
  OPTIMISATION "No optimisation"
  WARNING_LEVEL "High"
  GENERATE_MANIFEST ON
  # ARCHITECTURE "x64"
)

jucer_export_target_configuration(
  "Visual Studio 2017"
  NAME "Release"
  DEBUG_MODE OFF
  BINARY_NAME "MidiTest"
  OPTIMISATION "Maximise speed"
  WARNING_LEVEL "High"
  GENERATE_MANIFEST ON
  # ARCHITECTURE "x64"
)

jucer_export_target(
  "Linux Makefile"
  TARGET_PROJECT_FOLDER "Builds/LinuxMakefile" # used by EXTRA_LIBRARY_SEARCH_PATHS
  # EXTRA_COMPILER_FLAGS
)

jucer_export_target_configuration(
  "Linux Makefile"
  NAME "Debug"
  DEBUG_MODE ON
  BINARY_NAME "MidiTest"
  EXTRA_LIBRARY_SEARCH_PATHS
    "/usr/X11R6/lib/"
  OPTIMISATION "-O0 (no optimisation)"
)

jucer_export_target_configuration(
  "Linux Makefile"
  NAME "Release"
  DEBUG_MODE OFF
  BINARY_NAME "MidiTest"
  EXTRA_LIBRARY_SEARCH_PATHS
    "/usr/X11R6/lib/"
  OPTIMISATION "-O3 (fastest with safe optimisations)"
)

jucer_export_target(
  "Xcode (MacOSX)"
  EXTRA_COMPILER_FLAGS
    "-std=c++14"
    "-Wreorder"
    "-Wconstant-conversion"
    "-Wint-conversion"
    "-Woverloaded-virtual"
    "-Wuninitialized"
    "-Wunused-parameter"
    "-Wshorten-64-to-32"
    "-Wstrict-aliasing"
    "-Wshadow"
    "-Wconversion"
    "-Wsign-compare"
    "-Wsign-conversion"
)

jucer_export_target_configuration(
  "Xcode (MacOSX)"
  NAME "Debug"
  DEBUG_MODE ON
  BINARY_NAME "MidiTest"
  OPTIMISATION "-O0 (no optimisation)"
  OSX_BASE_SDK_VERSION "Use Default"
  OSX_DEPLOYMENT_TARGET "10.9"
  OSX_ARCHITECTURE "Use Default"
)

jucer_export_target_configuration(
  "Xcode (MacOSX)"
  NAME "Release"
  DEBUG_MODE OFF
  BINARY_NAME "MidiTest"
  OPTIMISATION "-O3 (fastest with safe optimisations)"
  OSX_BASE_SDK_VERSION "Use Default"
  OSX_DEPLOYMENT_TARGET "10.9"
  OSX_ARCHITECTURE "Use Default"
)

jucer_export_target(
  "Xcode (iOS)"
  IPHONE_SCREEN_ORIENTATION "Portrait and Landscape"
)

jucer_export_target_configuration(
  "Xcode (iOS)"
  NAME "Debug"
  DEBUG_MODE ON
  BINARY_NAME "MidiTest"
  OPTIMISATION "-O0 (no optimisation)"
  IOS_DEPLOYMENT_TARGET "8.0"
)

jucer_export_target_configuration(
  "Xcode (iOS)"
  NAME "Release"
  DEBUG_MODE OFF
  BINARY_NAME "MidiTest"
  OPTIMISATION "-O3 (fastest with safe optimisations)"
  IOS_DEPLOYMENT_TARGET "8.0"
)

jucer_project_end()
