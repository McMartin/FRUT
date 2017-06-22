# Copyright (c) 2017 Alain Martin
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


if(NOT DEFINED JUCE_modules_DIRS)
  message(FATAL_ERROR "JUCE_modules_DIRS must be defined")
endif()
list(REMOVE_DUPLICATES JUCE_modules_DIRS)

foreach(modules_dir ${JUCE_modules_DIRS})
  if (EXISTS "${modules_dir}/juce_core/juce_core.h"
    AND EXISTS "${modules_dir}/juce_data_structures/juce_data_structures.h"
    AND EXISTS "${modules_dir}/juce_events/juce_events.h"
    AND EXISTS "${modules_dir}/juce_graphics/juce_graphics.h"
    AND EXISTS "${modules_dir}/juce_gui_basics/juce_gui_basics.h"
  )
    set(JUCE_modules_DIR ${modules_dir})
    break()
  endif()
endforeach()

if(NOT DEFINED JUCE_modules_DIR)
  message(FATAL_ERROR "Could not find juce_core, juce_data_structures, juce_events, "
    "juce_graphics and juce_gui_basics when searching in the following directories: "
    "${JUCE_modules_DIRS}"
  )
endif()

get_filename_component(JUCE_modules_DIR
  "${JUCE_modules_DIR}" ABSOLUTE
  BASE_DIR "${CMAKE_BINARY_DIR}"
)


jucer_project_begin()

jucer_project_settings(
  PROJECT_NAME "IconBuilder"
  PROJECT_VERSION "1.0.0"
  PROJECT_TYPE "Console Application"
)

jucer_project_files("IconBuilder"
  x . . "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
  x . . "${CMAKE_CURRENT_LIST_DIR}/Source/Project Saving/jucer_ProjectExporter.cpp"
  x . . "${CMAKE_CURRENT_LIST_DIR}/Source/Utility/jucer_FileHelpers.cpp"
)

jucer_project_module(
  juce_core
  PATH "${JUCE_modules_DIR}"
)

jucer_project_module(
  juce_data_structures
  PATH "${JUCE_modules_DIR}"
)

jucer_project_module(
  juce_events
  PATH "${JUCE_modules_DIR}"
)

jucer_project_module(
  juce_graphics
  PATH "${JUCE_modules_DIR}"
)

jucer_project_module(
  juce_gui_basics
  PATH "${JUCE_modules_DIR}"
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

jucer_project_end()


install(TARGETS IconBuilder DESTINATION "IconBuilder")
