# Copyright (C) 2020  Alain Martin
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


if(NOT DEFINED JUCE_modules_DIRS)
  message(FATAL_ERROR "JUCE_modules_DIRS must be defined")
endif()
list(REMOVE_DUPLICATES JUCE_modules_DIRS)

foreach(modules_dir IN LISTS JUCE_modules_DIRS)
  if(EXISTS "${modules_dir}/juce_core/juce_core.h"
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
  message(FATAL_ERROR "Could not find juce_core, juce_data_structures, juce_events,"
    " juce_graphics and juce_gui_basics when searching in the following directories:"
    " ${JUCE_modules_DIRS}"
  )
endif()

get_filename_component(JUCE_modules_DIR
  "${JUCE_modules_DIR}" ABSOLUTE
  BASE_DIR "${CMAKE_BINARY_DIR}"
)


if(NOT DEFINED CMAKE_CONFIGURATION_TYPES)
  if(NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    message(STATUS "Setting CMAKE_BUILD_TYPE to \"Debug\" as it was not specified.")
    set(CMAKE_BUILD_TYPE Debug)
  endif()
endif()


project(tools)


set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if(built_by_Reprojucer)
  add_subdirectory("${tool_to_build}")
else()
  add_subdirectory(BinaryDataBuilder)
  add_subdirectory(IconBuilder)
  add_subdirectory(PListMerger)
  add_subdirectory(XcassetsBuilder)
endif()
