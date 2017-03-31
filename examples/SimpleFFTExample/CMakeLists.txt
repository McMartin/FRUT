# Copyright (c) 2016 Alain Martin
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required(VERSION 3.0)


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../cmake")
include(Reprojucer)


set(JUCE_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../../JUCE" CACHE PATH "Path to JUCE")

set(SimpleFFTExample_jucer_FILE
  "${JUCE_ROOT}/examples/SimpleFFTExample/SimpleFFTExample.jucer"
  CACHE PATH "Path to SimpleFFTExample.jucer"
)

if(NOT EXISTS "${SimpleFFTExample_jucer_FILE}")
  message(FATAL_ERROR "No such file: ${SimpleFFTExample_jucer_FILE}")
endif()

get_filename_component(SimpleFFTExample_jucer_DIR
  "${SimpleFFTExample_jucer_FILE}" DIRECTORY
)


jucer_project_begin(
  PROJECT_NAME "SimpleFFTExample"
  PROJECT_VERSION "1.0.0"
  # COMPANY_NAME
  # COMPANY_WEBSITE
  # COMPANY_EMAIL
  PROJECT_TYPE "GUI Application"
  BUNDLE_IDENTIFIER "com.yourcompany.SimpleFFTExample"
  BINARYDATACPP_SIZE_LIMIT "Default"
  # BINARYDATA_NAMESPACE
  # PREPROCESSOR_DEFINITIONS
  PROJECT_ID "cgcxPd"
)

jucer_project_files("SimpleFFTExample/Source"
  "${SimpleFFTExample_jucer_DIR}/Source/Main.cpp"
  "${SimpleFFTExample_jucer_DIR}/Source/SpectrogramComponent.h"
)

jucer_project_module(
  juce_audio_basics
  PATH "${SimpleFFTExample_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_audio_devices
  PATH "${SimpleFFTExample_jucer_DIR}/../../modules"
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
  PATH "${SimpleFFTExample_jucer_DIR}/../../modules"
  # JUCE_USE_FLAC
  # JUCE_USE_OGGVORBIS
  # JUCE_USE_MP3AUDIOFORMAT
  # JUCE_USE_LAME_AUDIO_FORMAT
  # JUCE_USE_WINDOWS_MEDIA_FORMAT
)

jucer_project_module(
  juce_audio_processors
  PATH "${SimpleFFTExample_jucer_DIR}/../../modules"
  # JUCE_PLUGINHOST_VST
  # JUCE_PLUGINHOST_VST3
  # JUCE_PLUGINHOST_AU
)

jucer_project_module(
  juce_audio_utils
  PATH "${SimpleFFTExample_jucer_DIR}/../../modules"
  # JUCE_USE_CDREADER
  # JUCE_USE_CDBURNER
)

jucer_project_module(
  juce_core
  PATH "${SimpleFFTExample_jucer_DIR}/../../modules"
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
  PATH "${SimpleFFTExample_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_events
  PATH "${SimpleFFTExample_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_graphics
  PATH "${SimpleFFTExample_jucer_DIR}/../../modules"
  # JUCE_USE_COREIMAGE_LOADER
  # JUCE_USE_DIRECTWRITE
)

jucer_project_module(
  juce_gui_basics
  PATH "${SimpleFFTExample_jucer_DIR}/../../modules"
  # JUCE_ENABLE_REPAINT_DEBUGGING
  # JUCE_USE_XSHM
  # JUCE_USE_XRENDER
  # JUCE_USE_XCURSOR
)

jucer_project_module(
  juce_gui_extra
  PATH "${SimpleFFTExample_jucer_DIR}/../../modules"
  # JUCE_WEB_BROWSER
  # JUCE_ENABLE_LIVE_CONSTANT_EDITOR
)

jucer_project_end()
