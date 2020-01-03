# Copyright (C) 2016-2019  Alain Martin
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


project(BinaryDataBuilder VERSION 0.3.1)


if(NOT DEFINED CMAKE_CONFIGURATION_TYPES)
  if(NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    message(STATUS "Setting CMAKE_BUILD_TYPE to \"Debug\" as it was not specified.")
    set(CMAKE_BUILD_TYPE Debug)
  endif()
endif()


add_executable(BinaryDataBuilder
  "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/extras/Projucer/Source/Project Saving/jucer_ResourceFile.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/extras/Projucer/Source/Utility/jucer_CodeHelpers.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/extras/Projucer/Source/Utility/jucer_FileHelpers.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/extras/Projucer/Source/Utility/jucer_MiscUtilities.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/modules/juce_gui_extra/juce_gui_extra.cpp"
)

if(APPLE)
  target_sources(BinaryDataBuilder PRIVATE
    "${JUCE_modules_DIR}/juce_core/juce_core.mm"
  )
else()
  target_sources(BinaryDataBuilder PRIVATE
    "${JUCE_modules_DIR}/juce_core/juce_core.cpp"
  )
endif()

target_include_directories(BinaryDataBuilder PRIVATE
  "${JUCE_modules_DIR}"
)

target_compile_definitions(BinaryDataBuilder PRIVATE
  $<$<CONFIG:Debug>:DEBUG=1>
  $<$<CONFIG:Debug>:_DEBUG=1>
  $<$<NOT:$<CONFIG:Debug>>:NDEBUG=1>
  JUCE_DISABLE_JUCE_VERSION_PRINTING=1
  JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1
  JUCE_MODULE_AVAILABLE_juce_core=1
  JUCE_STANDALONE_APPLICATION=1
  JUCE_USE_CURL=0
)

set_target_properties(BinaryDataBuilder PROPERTIES
  CXX_EXTENSIONS OFF
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED ON
  OUTPUT_NAME BinaryDataBuilder-${PROJECT_VERSION}
)

if(APPLE)
  find_library(Cocoa_framework "Cocoa")
  find_library(IOKit_framework "IOKit")

  target_link_libraries(BinaryDataBuilder PRIVATE ${Cocoa_framework} ${IOKit_framework})
endif()

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
  target_compile_options(BinaryDataBuilder PRIVATE -pthread)
  target_link_libraries(BinaryDataBuilder PRIVATE dl pthread)
endif()

if(WIN32 AND NOT MSVC)
  target_link_libraries(BinaryDataBuilder PRIVATE
    -lshlwapi
    -lversion
    -lwininet
    -lwinmm
    -lws2_32
  )
endif()


if(built_by_Reprojucer)
  install(TARGETS BinaryDataBuilder DESTINATION ".")
else()
  install(TARGETS BinaryDataBuilder DESTINATION "FRUT/cmake/bin")
endif()
