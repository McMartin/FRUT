# Copyright (c) 2016 Alain Martin
#
# This file is part of JUCE.cmake.
#
# JUCE.cmake is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# JUCE.cmake is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with JUCE.cmake.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.0)


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../cmake")
include(Reprojucer)


get_filename_component(JUCE_ROOT
  "${CMAKE_CURRENT_LIST_DIR}/../../../JUCE" ABSOLUTE CACHE
)

get_filename_component(BouncingBallWavetableDemo_jucer_FILE
  "${JUCE_ROOT}/examples/BouncingBallWavetableDemo/BouncingBallWavetableDemo.jucer"
  ABSOLUTE CACHE
)

if(NOT EXISTS "${BouncingBallWavetableDemo_jucer_FILE}")
  message(FATAL_ERROR "No such file: ${BouncingBallWavetableDemo_jucer_FILE}")
endif()

get_filename_component(BouncingBallWavetableDemo_jucer_DIR
  "${BouncingBallWavetableDemo_jucer_FILE}" DIRECTORY
)


jucer_project_begin(
  PROJECT_NAME "BouncingBallWavetableDemo"
  PROJECT_VERSION "1.0.0"
  # COMPANY_NAME
  # COMPANY_WEBSITE
  # COMPANY_EMAIL
  PROJECT_TYPE "GUI Application"
  BUNDLE_IDENTIFIER "com.juce.BouncingBallWavetableDemo"
  BINARYDATACPP_SIZE_LIMIT "Default"
  # BINARYDATA_NAMESPACE
  # PREPROCESSOR_DEFINITIONS
  PROJECT_ID "fyUrFS"
)

jucer_project_files("BouncingBallWavetableDemo/Source"
  "${BouncingBallWavetableDemo_jucer_DIR}/Source/Main.cpp"
  "${BouncingBallWavetableDemo_jucer_DIR}/Source/MainComponent.cpp"
)

jucer_project_module(
  juce_audio_basics
  PATH "${BouncingBallWavetableDemo_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_audio_devices
  PATH "${BouncingBallWavetableDemo_jucer_DIR}/../../modules"
  # JUCE_ASIO
  # JUCE_WASAPI
  # JUCE_WASAPI_EXCLUSIVE
  # JUCE_DIRECTSOUND
  # JUCE_ALSA
  # JUCE_JACK
  # JUCE_USE_ANDROID_OPENSLES
)

jucer_project_module(
  juce_audio_formats
  PATH "${BouncingBallWavetableDemo_jucer_DIR}/../../modules"
  # JUCE_USE_FLAC
  # JUCE_USE_OGGVORBIS
  # JUCE_USE_MP3AUDIOFORMAT
  # JUCE_USE_LAME_AUDIO_FORMAT
  # JUCE_USE_WINDOWS_MEDIA_FORMAT
)

jucer_project_module(
  juce_audio_processors
  PATH "${BouncingBallWavetableDemo_jucer_DIR}/../../modules"
  # JUCE_PLUGINHOST_VST
  # JUCE_PLUGINHOST_VST3
  # JUCE_PLUGINHOST_AU
)

jucer_project_module(
  juce_audio_utils
  PATH "${BouncingBallWavetableDemo_jucer_DIR}/../../modules"
  # JUCE_USE_CDREADER
  # JUCE_USE_CDBURNER
)

jucer_project_module(
  juce_core
  PATH "${BouncingBallWavetableDemo_jucer_DIR}/../../modules"
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
  juce_cryptography
  PATH "${BouncingBallWavetableDemo_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_data_structures
  PATH "${BouncingBallWavetableDemo_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_events
  PATH "${BouncingBallWavetableDemo_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_graphics
  PATH "${BouncingBallWavetableDemo_jucer_DIR}/../../modules"
  # JUCE_USE_COREIMAGE_LOADER
  # JUCE_USE_DIRECTWRITE
)

jucer_project_module(
  juce_gui_basics
  PATH "${BouncingBallWavetableDemo_jucer_DIR}/../../modules"
  # JUCE_ENABLE_REPAINT_DEBUGGING
  # JUCE_USE_XSHM
  # JUCE_USE_XRENDER
  # JUCE_USE_XCURSOR
)

jucer_project_module(
  juce_gui_extra
  PATH "${BouncingBallWavetableDemo_jucer_DIR}/../../modules"
  # JUCE_WEB_BROWSER
  # JUCE_ENABLE_LIVE_CONSTANT_EDITOR
)

jucer_project_module(
  juce_video
  PATH "${BouncingBallWavetableDemo_jucer_DIR}/../../modules"
  # JUCE_DIRECTSHOW
  # JUCE_MEDIAFOUNDATION
  # JUCE_QUICKTIME
  # JUCE_USE_CAMERA
)

jucer_project_end()
