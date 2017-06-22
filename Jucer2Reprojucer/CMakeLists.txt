# Copyright (c) 2017 Alain Martin, Matthieu Talbot
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


jucer_project_begin()

jucer_project_settings(
  PROJECT_NAME "Jucer2Reprojucer"
  PROJECT_VERSION "1.0.0"
  PROJECT_TYPE "Console Application"
  PREPROCESSOR_DEFINITIONS "DONT_SET_USING_JUCE_NAMESPACE=1"
)

jucer_project_files("Jucer2Reprojucer"
  x . . "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
)

jucer_project_module(
  juce_core
  PATH "${JUCE_ROOT}/modules"
)

jucer_project_module(
  juce_data_structures
  PATH "${JUCE_ROOT}/modules"
)

jucer_project_module(
  juce_events
  PATH "${JUCE_ROOT}/modules"
)

jucer_export_target(
  "Xcode (MacOSX)"
  EXTRA_COMPILER_FLAGS "-Werror -Weverything
    -Wno-c++98-compat
    -Wno-c++98-compat-pedantic
    -Wno-cast-align
    -Wno-comma
    -Wno-covered-switch-default
    -Wno-date-time
    -Wno-deprecated
    -Wno-disabled-macro-expansion
    -Wno-documentation
    -Wno-documentation-unknown-command
    -Wno-exit-time-destructors
    -Wno-extra-semi
    -Wno-float-equal
    -Wno-global-constructors
    -Wno-implicit-fallthrough
    -Wno-missing-noreturn
    -Wno-missing-prototypes
    -Wno-missing-variable-declarations
    -Wno-nullable-to-nonnull-conversion
    -Wno-old-style-cast
    -Wno-padded
    -Wno-reorder
    -Wno-reserved-id-macro
    -Wno-strict-selector-match
    -Wno-switch-enum
    -Wno-undeclared-selector
    -Wno-undef
    -Wno-undefined-reinterpret-cast
    -Wno-unknown-warning-option
    -Wno-weak-vtables
  "
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
  "Visual Studio 2015"
  EXTRA_COMPILER_FLAGS "/WX /W4 /wd4592"
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
  EXTRA_COMPILER_FLAGS "/WX /W4"
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
  EXTRA_COMPILER_FLAGS "-Werror -Wall -Wextra"
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

jucer_project_end()
