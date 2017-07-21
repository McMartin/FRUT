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


if(NOT DEFINED JUCE_modules_DIRS)
  message(FATAL_ERROR "JUCE_modules_DIRS must be defined")
endif()
list(REMOVE_DUPLICATES JUCE_modules_DIRS)

foreach(modules_dir ${JUCE_modules_DIRS})
  if (EXISTS "${modules_dir}/juce_core/juce_core.h"
    AND EXISTS "${modules_dir}/juce_cryptography/juce_cryptography.h"
    AND EXISTS "${modules_dir}/juce_data_structures/juce_data_structures.h"
    AND EXISTS "${modules_dir}/juce_events/juce_events.h"
    AND EXISTS "${modules_dir}/juce_graphics/juce_graphics.h"
    AND EXISTS "${modules_dir}/juce_gui_basics/juce_gui_basics.h"
    AND EXISTS "${modules_dir}/juce_gui_extra/juce_gui_extra.h"
  )
    set(JUCE_modules_DIR ${modules_dir})
    break()
  endif()
endforeach()

if(NOT DEFINED JUCE_modules_DIR)
  message(FATAL_ERROR "Could not find juce_core, juce_cryptography, juce_data_structures,"
    " juce_events, juce_graphics, juce_gui_basics and juce_gui_extra when searching in"
    " the following directories: ${JUCE_modules_DIRS}"
  )
endif()

get_filename_component(JUCE_modules_DIR
  "${JUCE_modules_DIR}" ABSOLUTE
  BASE_DIR "${CMAKE_BINARY_DIR}"
)


project(helpers)


include_directories("${JUCE_modules_DIR}")

add_definitions(
  -DJUCE_GLOBAL_MODULE_SETTINGS_INCLUDED
  -DJUCE_MODULE_AVAILABLE_juce_core=1
  -DJUCE_MODULE_AVAILABLE_juce_cryptography=1
  -DJUCE_MODULE_AVAILABLE_juce_data_structures=1
  -DJUCE_MODULE_AVAILABLE_juce_events=1
  -DJUCE_MODULE_AVAILABLE_juce_graphics=1
  -DJUCE_MODULE_AVAILABLE_juce_gui_basics=1
  -DJUCE_MODULE_AVAILABLE_juce_gui_extra=1
  -DJUCE_STANDALONE_APPLICATION=1
)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
  find_package(PkgConfig REQUIRED)

  function(use_package package)
    pkg_check_modules(${package} "${package}")
    if(NOT ${package}_FOUND)
      message(FATAL_ERROR "pkg-config could not find ${package}")
    endif()
    add_compile_options(${${package}_CFLAGS})
    link_libraries(${${package}_LIBRARIES})
  endfunction()

  use_package(freetype2)
  use_package(x11)
  use_package(xext)

  link_libraries(dl pthread)
endif()


add_subdirectory(BinaryDataBuilder)
add_subdirectory(IconBuilder)
