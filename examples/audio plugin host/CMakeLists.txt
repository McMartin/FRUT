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

if(NOT DEFINED Plugin_Host_jucer)
  message(FATAL_ERROR "Plugin_Host_jucer must be defined")
endif()

get_filename_component(Plugin_Host_jucer_FILE
  "${Plugin_Host_jucer}" REALPATH
  BASE_DIR "${CMAKE_BINARY_DIR}"
)

if(NOT EXISTS "${Plugin_Host_jucer_FILE}")
  message(FATAL_ERROR "No such file: ${Plugin_Host_jucer_FILE}")
endif()

get_filename_component(Plugin_Host_jucer_DIR "${Plugin_Host_jucer_FILE}" DIRECTORY)


if(WIN32)
  set(VST3_SDK_FOLDER "C:/SDKs/VST_SDK/VST3_SDK")
else()
  set(VST3_SDK_FOLDER "~/SDKs/VST_SDK/VST3_SDK")
endif()
set(VST3_SDK_FOLDER "${VST3_SDK_FOLDER}" CACHE PATH "VST3 SDK Folder")


jucer_project_begin(
  PROJECT_NAME "Plugin Host"
  PROJECT_VERSION "1.0.0"
  COMPANY_NAME "ROLI Ltd."
  # COMPANY_WEBSITE
  # COMPANY_EMAIL
  PROJECT_TYPE "GUI Application"
  BUNDLE_IDENTIFIER "com.roli.pluginhost"
  BINARYDATACPP_SIZE_LIMIT "Default"
  # BINARYDATA_NAMESPACE
  # PREPROCESSOR_DEFINITIONS
  PROJECT_ID "NTe0XB0ij"
)

jucer_project_files("Plugin Host/Source"
  "${Plugin_Host_jucer_DIR}/Source/FilterGraph.cpp"
  "${Plugin_Host_jucer_DIR}/Source/FilterGraph.h"
  "${Plugin_Host_jucer_DIR}/Source/GraphEditorPanel.cpp"
  "${Plugin_Host_jucer_DIR}/Source/GraphEditorPanel.h"
  "${Plugin_Host_jucer_DIR}/Source/HostStartup.cpp"
  "${Plugin_Host_jucer_DIR}/Source/InternalFilters.cpp"
  "${Plugin_Host_jucer_DIR}/Source/InternalFilters.h"
  "${Plugin_Host_jucer_DIR}/Source/MainHostWindow.cpp"
  "${Plugin_Host_jucer_DIR}/Source/MainHostWindow.h"
)

if(EXISTS "${Plugin_Host_jucer_DIR}/Source/FilterIOConfiguration.cpp")
  jucer_project_files("Plugin Host/Source"
    "${Plugin_Host_jucer_DIR}/Source/FilterIOConfiguration.cpp"
    "${Plugin_Host_jucer_DIR}/Source/FilterIOConfiguration.h"
  )
endif()

jucer_project_module(
  juce_audio_basics
  PATH "${Plugin_Host_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_audio_devices
  PATH "${Plugin_Host_jucer_DIR}/../../modules"
  # JUCE_ASIO
  JUCE_WASAPI ON
  # JUCE_WASAPI_EXCLUSIVE
  JUCE_DIRECTSOUND ON
  JUCE_ALSA ON
  # JUCE_JACK
  # JUCE_USE_ANDROID_OPENSLES
)

jucer_project_module(
  juce_audio_formats
  PATH "${Plugin_Host_jucer_DIR}/../../modules"
  JUCE_USE_FLAC OFF
  JUCE_USE_OGGVORBIS OFF
  # JUCE_USE_MP3AUDIOFORMAT
  # JUCE_USE_LAME_AUDIO_FORMAT
  # JUCE_USE_WINDOWS_MEDIA_FORMAT
)

jucer_project_module(
  juce_audio_processors
  PATH "${Plugin_Host_jucer_DIR}/../../modules"
  JUCE_PLUGINHOST_VST ON
  JUCE_PLUGINHOST_VST3 ON
  JUCE_PLUGINHOST_AU ON
)

jucer_project_module(
  juce_audio_utils
  PATH "${Plugin_Host_jucer_DIR}/../../modules"
  JUCE_USE_CDREADER OFF
  JUCE_USE_CDBURNER OFF
)

jucer_project_module(
  juce_core
  PATH "${Plugin_Host_jucer_DIR}/../../modules"
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
  PATH "${Plugin_Host_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_data_structures
  PATH "${Plugin_Host_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_events
  PATH "${Plugin_Host_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_graphics
  PATH "${Plugin_Host_jucer_DIR}/../../modules"
  # JUCE_USE_COREIMAGE_LOADER
  # JUCE_USE_DIRECTWRITE
)

jucer_project_module(
  juce_gui_basics
  PATH "${Plugin_Host_jucer_DIR}/../../modules"
  # JUCE_ENABLE_REPAINT_DEBUGGING
  # JUCE_USE_XSHM
  # JUCE_USE_XRENDER
  # JUCE_USE_XCURSOR
)

jucer_project_module(
  juce_gui_extra
  PATH "${Plugin_Host_jucer_DIR}/../../modules"
  JUCE_WEB_BROWSER OFF
  # JUCE_ENABLE_LIVE_CONSTANT_EDITOR
)

jucer_project_module(
  juce_opengl
  PATH "${Plugin_Host_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_video
  PATH "${Plugin_Host_jucer_DIR}/../../modules"
  # JUCE_DIRECTSHOW
  # JUCE_MEDIAFOUNDATION
  JUCE_QUICKTIME OFF
  JUCE_USE_CAMERA OFF
)

jucer_project_end()
