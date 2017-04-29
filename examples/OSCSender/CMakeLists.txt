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

get_filename_component(OSCSender_jucer_FILE
  "${JUCE_ROOT}/examples/OSCSender/OSCSender.jucer" ABSOLUTE CACHE
)

if(NOT EXISTS "${OSCSender_jucer_FILE}")
  message(FATAL_ERROR "No such file: ${OSCSender_jucer_FILE}")
endif()

get_filename_component(OSCSender_jucer_DIR
  "${OSCSender_jucer_FILE}" DIRECTORY
)


jucer_project_begin(
  PROJECT_NAME "OSCSender"
  PROJECT_VERSION "1.0.0"
  # COMPANY_NAME
  # COMPANY_WEBSITE
  # COMPANY_EMAIL
  PROJECT_TYPE "GUI Application"
  BUNDLE_IDENTIFIER "com.yourcompany.OSCSender"
  BINARYDATACPP_SIZE_LIMIT "Default"
  # BINARYDATA_NAMESPACE
  # PREPROCESSOR_DEFINITIONS
  PROJECT_ID "rysVAr"
)

jucer_project_files("OSCSender/Source"
  "${OSCSender_jucer_DIR}/Source/Main.cpp"
  "${OSCSender_jucer_DIR}/Source/MainComponent.h"
)

jucer_project_module(
  juce_core
  PATH "${OSCSender_jucer_DIR}/../../modules"
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
  PATH "${OSCSender_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_events
  PATH "${OSCSender_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_graphics
  PATH "${OSCSender_jucer_DIR}/../../modules"
  # JUCE_USE_COREIMAGE_LOADER
  # JUCE_USE_DIRECTWRITE
)

jucer_project_module(
  juce_gui_basics
  PATH "${OSCSender_jucer_DIR}/../../modules"
  # JUCE_ENABLE_REPAINT_DEBUGGING
  # JUCE_USE_XSHM
  # JUCE_USE_XRENDER
  # JUCE_USE_XCURSOR
)

jucer_project_module(
  juce_osc
  PATH "${OSCSender_jucer_DIR}/../../modules"
)

jucer_project_end()
