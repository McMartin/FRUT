# This file was generated by FRUT's Jucer2CMake from "BouncingBallWavetableDemo.jucer"

cmake_minimum_required(VERSION 3.4)

project("BouncingBallWavetableDemo")


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../../../cmake")
include(Reprojucer)


if(NOT DEFINED BouncingBallWavetableDemo_jucer_FILE)
  message(FATAL_ERROR "BouncingBallWavetableDemo_jucer_FILE must be defined")
endif()

get_filename_component(BouncingBallWavetableDemo_jucer_FILE
  "${BouncingBallWavetableDemo_jucer_FILE}" ABSOLUTE
  BASE_DIR "${CMAKE_BINARY_DIR}"
)


jucer_project_begin(
  JUCER_VERSION "4.2.0"
  PROJECT_FILE "${BouncingBallWavetableDemo_jucer_FILE}"
  PROJECT_ID "fyUrFS"
)

jucer_project_settings(
  PROJECT_NAME "BouncingBallWavetableDemo"
  PROJECT_VERSION "1.0.0"
  PROJECT_TYPE "GUI Application"
  BUNDLE_IDENTIFIER "com.juce.BouncingBallWavetableDemo"
  INCLUDE_BINARYDATA ON
)

jucer_project_files("BouncingBallWavetableDemo/Source"
# Compile   Xcode     Binary    File
#           Resource  Resource
  x         .         .         "Source/MainComponent.cpp"
  x         .         .         "Source/Main.cpp"
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
  # JUCE_USE_CDREADER
  # JUCE_USE_CDBURNER
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
)

jucer_project_module(
  juce_cryptography
  PATH "../../modules"
)

jucer_project_module(
  juce_data_structures
  PATH "../../modules"
)

jucer_project_module(
  juce_events
  PATH "../../modules"
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

jucer_project_module(
  juce_video
  PATH "../../modules"
  # JUCE_DIRECTSHOW
  # JUCE_MEDIAFOUNDATION
  # JUCE_QUICKTIME
  # JUCE_USE_CAMERA
)

jucer_export_target(
  "Xcode (MacOSX)"
)

jucer_export_target_configuration(
  "Xcode (MacOSX)"
  NAME "Debug"
  DEBUG_MODE ON
  BINARY_NAME "WavetableBouncingEditor"
  OPTIMISATION "-O0 (no optimisation)"
  OSX_BASE_SDK_VERSION "Use Default"
  OSX_DEPLOYMENT_TARGET "Use Default"
  OSX_ARCHITECTURE "Use Default"
)

jucer_export_target_configuration(
  "Xcode (MacOSX)"
  NAME "Release"
  DEBUG_MODE OFF
  BINARY_NAME "WavetableBouncingEditor"
  OPTIMISATION "-Os (minimise code size)"
  OSX_BASE_SDK_VERSION "Use Default"
  OSX_DEPLOYMENT_TARGET "Use Default"
  OSX_ARCHITECTURE "Use Default"
)

jucer_export_target(
  "Visual Studio 2013"
)

jucer_export_target_configuration(
  "Visual Studio 2013"
  NAME "Debug"
  DEBUG_MODE ON
  BINARY_NAME "WavetableBouncingEditor"
  OPTIMISATION "No optimisation"
  WARNING_LEVEL "High"
  GENERATE_MANIFEST ON
  # ARCHITECTURE "32-bit"
)

jucer_export_target_configuration(
  "Visual Studio 2013"
  NAME "Release"
  DEBUG_MODE OFF
  BINARY_NAME "WavetableBouncingEditor"
  OPTIMISATION "Minimise size"
  WARNING_LEVEL "High"
  GENERATE_MANIFEST ON
  # ARCHITECTURE "32-bit"
)

jucer_export_target(
  "Linux Makefile"
  TARGET_PROJECT_FOLDER "Builds/Linux" # used by EXTRA_LIBRARY_SEARCH_PATHS
)

jucer_export_target_configuration(
  "Linux Makefile"
  NAME "Debug"
  DEBUG_MODE ON
  BINARY_NAME "WavetableBouncingEditor"
  EXTRA_LIBRARY_SEARCH_PATHS
    "/usr/X11R6/lib/"
  OPTIMISATION "-O0 (no optimisation)"
)

jucer_export_target_configuration(
  "Linux Makefile"
  NAME "Release"
  DEBUG_MODE OFF
  BINARY_NAME "WavetableBouncingEditor"
  EXTRA_LIBRARY_SEARCH_PATHS
    "/usr/X11R6/lib/"
  OPTIMISATION "-Os (minimise code size)"
)

jucer_project_end()
