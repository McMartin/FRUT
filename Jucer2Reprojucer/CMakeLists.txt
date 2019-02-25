# Copyright (c) 2017 Alain Martin, Matthieu Talbot
#
# This file is part of FRUT.
#
# FRUT is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# FRUT is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with FRUT.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.4)

project(Jucer2Reprojucer)


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../cmake")
include(Reprojucer)


if(NOT DEFINED JUCE_ROOT)
  message(FATAL_ERROR "JUCE_ROOT must be defined")
endif()

get_filename_component(JUCE_ROOT
  "${JUCE_ROOT}" ABSOLUTE
  BASE_DIR "${CMAKE_BINARY_DIR}"
)

if(NOT IS_DIRECTORY "${JUCE_ROOT}")
  message(FATAL_ERROR "No such directory: ${JUCE_ROOT}")
endif()


option(WRITE_CRLF_LINE_ENDINGS
  "Write CR+LF line endings in generated CMakeLists.txt files"
  OFF
)


jucer_project_begin()

jucer_project_settings(
  PROJECT_NAME "Jucer2Reprojucer"
  PROJECT_VERSION "1.0.0"
  PROJECT_TYPE "Console Application"
  REPORT_JUCE_APP_USAGE OFF
  DISPLAY_THE_JUCE_SPLASH_SCREEN OFF
  PREPROCESSOR_DEFINITIONS
    "DONT_SET_USING_JUCE_NAMESPACE=1"
    "JUCE_DISABLE_JUCE_VERSION_PRINTING=1"
    "WRITE_CRLF_LINE_ENDINGS=$<BOOL:${WRITE_CRLF_LINE_ENDINGS}>"
  HEADER_SEARCH_PATHS
    "${CMAKE_CURRENT_LIST_DIR}/third-party"
)

jucer_project_files("Jucer2Reprojucer"
  x . . "main.cpp"
)

jucer_project_module(
  juce_core
  PATH "${JUCE_ROOT}/modules"
  ADD_SOURCE_TO_PROJECT OFF
  JUCE_USE_CURL OFF
)

jucer_project_module(
  juce_data_structures
  PATH "${JUCE_ROOT}/modules"
  ADD_SOURCE_TO_PROJECT OFF
)

jucer_project_module(
  juce_events
  PATH "${JUCE_ROOT}/modules"
  ADD_SOURCE_TO_PROJECT OFF
)

jucer_export_target(
  "Xcode (MacOSX)"
  EXTRA_COMPILER_FLAGS
    "-Werror"
    "-Weverything"
    "-Wno-c++98-compat"
    "-Wno-c++98-compat-pedantic"
    "-Wno-padded"
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
  EXTRA_COMPILER_FLAGS
    "/WX"
    "/W4"
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
  EXTRA_COMPILER_FLAGS
    "/WX"
    "/W4"
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
  EXTRA_COMPILER_FLAGS
    "/WX"
    "/W4"
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
  EXTRA_COMPILER_FLAGS
    "-Werror"
    "-Wall"
    "-Wextra"
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
  EXTRA_COMPILER_FLAGS
    "-Werror"
    "-Wall"
    "-Wextra"
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
  EXTRA_COMPILER_FLAGS
    "-Werror"
    "-Wall"
    "-Wextra"
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


# Disable all warnings when compiling JUCE modules
set(JUCE_modules_sources
  ${JUCER_PROJECT_MODULE_juce_core_SOURCES}
  ${JUCER_PROJECT_MODULE_juce_data_structures_SOURCES}
  ${JUCER_PROJECT_MODULE_juce_events_SOURCES}
)
if(MSVC)
  set_source_files_properties(${JUCE_modules_sources} PROPERTIES COMPILE_FLAGS "/W0")
else()
  set_source_files_properties(${JUCE_modules_sources} PROPERTIES COMPILE_FLAGS "-w")
endif()


install(TARGETS Jucer2Reprojucer DESTINATION "FRUT/bin")
