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

cmake_minimum_required(VERSION 3.4)


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../cmake")
include(Reprojucer)


if(NOT DEFINED JuceDemoPlugin_jucer_FILE)
  message(FATAL_ERROR "JuceDemoPlugin_jucer_FILE must be defined")
endif()

get_filename_component(JuceDemoPlugin_jucer_FILE
  "${JuceDemoPlugin_jucer_FILE}" ABSOLUTE
  BASE_DIR "${CMAKE_CURRENT_BINARY_DIR}"
)

if(NOT EXISTS "${JuceDemoPlugin_jucer_FILE}")
  message(FATAL_ERROR "No such file: ${JuceDemoPlugin_jucer_FILE}")
endif()

get_filename_component(JuceDemoPlugin_jucer_DIR
  "${JuceDemoPlugin_jucer_FILE}" DIRECTORY
)


jucer_project_begin(
  PROJECT_NAME "JuceDemoPlugin"
  PROJECT_VERSION "1.0.0"
  COMPANY_NAME "ROLI Ltd."
  COMPANY_WEBSITE "www.juce.com"
  COMPANY_EMAIL "info@juce.com"
  PROJECT_TYPE "Audio Plug-in"
  BUNDLE_IDENTIFIER "com.juce.JuceDemoPlugin"
  BINARYDATACPP_SIZE_LIMIT "Default"
  # BINARYDATA_NAMESPACE
  # PREPROCESSOR_DEFINITIONS
  PROJECT_ID "0nRd9LLGO"
)

jucer_audio_plugin_settings(
  BUILD_VST ON
  BUILD_AUDIOUNIT ON
  PLUGIN_NAME "Juce Demo Plugin"
  PLUGIN_DESCRIPTION "Juce Demo Plugin"
  PLUGIN_MANUFACTURER "ROLI Ltd."
  PLUGIN_MANUFACTURER_CODE "ROLI"
  PLUGIN_CODE "Jcdm"
  # PLUGIN_CHANNEL_CONFIGURATIONS
  PLUGIN_IS_A_SYNTH FALSE
  PLUGIN_MIDI_INPUT ON
  PLUGIN_MIDI_OUTPUT ON
  MIDI_EFFECT_PLUGIN OFF
  KEY_FOCUS ON
  PLUGIN_AU_EXPORT_PREFIX "JuceDemoProjectAU"
  # PLUGIN_AU_MAIN_TYPE
  # VST_CATEGORY
)

jucer_project_files("JuceDemoPlugin/Source"
  "${JuceDemoPlugin_jucer_DIR}/Source/PluginEditor.cpp"
  "${JuceDemoPlugin_jucer_DIR}/Source/PluginEditor.h"
  "${JuceDemoPlugin_jucer_DIR}/Source/PluginProcessor.cpp"
  "${JuceDemoPlugin_jucer_DIR}/Source/PluginProcessor.h"
  "${JuceDemoPlugin_jucer_DIR}/Source/SinewaveSynth.h"
)

jucer_project_module(
  juce_audio_basics
  PATH "${JuceDemoPlugin_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_audio_devices
  PATH "${JuceDemoPlugin_jucer_DIR}/../../modules"
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
  PATH "${JuceDemoPlugin_jucer_DIR}/../../modules"
  # JUCE_USE_FLAC
  # JUCE_USE_OGGVORBIS
  # JUCE_USE_MP3AUDIOFORMAT
  # JUCE_USE_LAME_AUDIO_FORMAT
  # JUCE_USE_WINDOWS_MEDIA_FORMAT
)

jucer_project_module(
  juce_audio_plugin_client
  PATH "${JuceDemoPlugin_jucer_DIR}/../../modules"
  # JUCE_FORCE_USE_LEGACY_PARAM_IDS
  # JUCE_USE_STUDIO_ONE_COMPATIBLE_PARAMETERS
)

jucer_project_module(
  juce_audio_processors
  PATH "${JuceDemoPlugin_jucer_DIR}/../../modules"
  JUCE_PLUGINHOST_VST OFF
  # JUCE_PLUGINHOST_VST3
  JUCE_PLUGINHOST_AU OFF
)

jucer_project_module(
  juce_audio_utils
  PATH "${JuceDemoPlugin_jucer_DIR}/../../modules"
  # JUCE_USE_CDREADER
  # JUCE_USE_CDBURNER
)

jucer_project_module(
  juce_core
  PATH "${JuceDemoPlugin_jucer_DIR}/../../modules"
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
  PATH "${JuceDemoPlugin_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_events
  PATH "${JuceDemoPlugin_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_graphics
  PATH "${JuceDemoPlugin_jucer_DIR}/../../modules"
  # JUCE_USE_COREIMAGE_LOADER
  # JUCE_USE_DIRECTWRITE
)

jucer_project_module(
  juce_gui_basics
  PATH "${JuceDemoPlugin_jucer_DIR}/../../modules"
  # JUCE_ENABLE_REPAINT_DEBUGGING
  # JUCE_USE_XSHM
  # JUCE_USE_XRENDER
  # JUCE_USE_XCURSOR
)

jucer_project_module(
  juce_gui_extra
  PATH "${JuceDemoPlugin_jucer_DIR}/../../modules"
  # JUCE_WEB_BROWSER
  # JUCE_ENABLE_LIVE_CONSTANT_EDITOR
)

jucer_project_end()
