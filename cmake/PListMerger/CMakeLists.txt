# Copyright (c) 2017 Alain Martin
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

foreach(modules_dir ${JUCE_modules_DIRS})
  if (EXISTS "${modules_dir}/juce_core/juce_core.h")
    set(JUCE_modules_DIR ${modules_dir})
    break()
  endif()
endforeach()

if(NOT DEFINED JUCE_modules_DIR)
  message(FATAL_ERROR "Could not find juce_core when searching in the following "
    "directories: ${JUCE_modules_DIRS}"
  )
endif()

get_filename_component(JUCE_modules_DIR
  "${JUCE_modules_DIR}" ABSOLUTE
  BASE_DIR "${CMAKE_BINARY_DIR}"
)


if(CMAKE_BUILD_TYPE STREQUAL "")
  message(STATUS "Setting CMAKE_BUILD_TYPE to \"Debug\" as it was not specified.")
  set(CMAKE_BUILD_TYPE Debug)
endif()


project(PListMerger VERSION 0.1.0)

add_executable(PListMerger
  "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
)

if(APPLE)
  target_sources(PListMerger PRIVATE
    "${JUCE_modules_DIR}/juce_core/juce_core.mm"
  )
else()
  target_sources(PListMerger PRIVATE
    "${JUCE_modules_DIR}/juce_core/juce_core.cpp"
  )
endif()

target_include_directories(PListMerger PRIVATE
  "${JUCE_modules_DIR}"
)

target_compile_definitions(PListMerger PRIVATE
  $<$<CONFIG:Debug>:DEBUG=1>
  $<$<CONFIG:Debug>:_DEBUG=1>
  $<$<NOT:$<CONFIG:Debug>>:NDEBUG=1>
  JUCE_DISABLE_JUCE_VERSION_PRINTING=1
  JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1
  JUCE_MODULE_AVAILABLE_juce_core=1
  JUCE_STANDALONE_APPLICATION=1
)

set_target_properties(PListMerger PROPERTIES
  CXX_EXTENSIONS OFF
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED ON
  OUTPUT_NAME PListMerger-${PROJECT_VERSION}
)

if(APPLE)
  find_library(Cocoa "Cocoa")
  find_library(IOKit "IOKit")

  target_link_libraries(PListMerger PRIVATE ${Cocoa} ${IOKit})
endif()

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
  target_compile_options(PListMerger PRIVATE -pthread)
  target_link_libraries(PListMerger PRIVATE dl pthread)
endif()


get_cmake_property(is_in_try_compile IN_TRY_COMPILE)
if(is_in_try_compile)
  install(TARGETS PListMerger DESTINATION ".")
else()
  install(TARGETS PListMerger DESTINATION "FRUT/cmake/bin")
endif()
