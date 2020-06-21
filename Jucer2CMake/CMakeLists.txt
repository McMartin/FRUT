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


if(NOT DEFINED JUCE_ROOT)
  message(FATAL_ERROR "JUCE_ROOT must be defined")
endif()

get_filename_component(JUCE_ROOT "${JUCE_ROOT}" ABSOLUTE BASE_DIR "${CMAKE_BINARY_DIR}")

if(NOT IS_DIRECTORY "${JUCE_ROOT}")
  message(FATAL_ERROR "No such directory: ${JUCE_ROOT}")
endif()


project(Jucer2CMake)


if(NOT DEFINED CMAKE_CONFIGURATION_TYPES)
  if(NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    message(STATUS "Setting CMAKE_BUILD_TYPE to \"Debug\" as it was not specified.")
    set(CMAKE_BUILD_TYPE Debug)
  endif()
endif()


set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_library(Jucer2CMake_JUCE STATIC "")

if(APPLE)
  target_sources(Jucer2CMake_JUCE PRIVATE "${JUCE_ROOT}/modules/juce_core/juce_core.mm")
else()
  target_sources(Jucer2CMake_JUCE PRIVATE "${JUCE_ROOT}/modules/juce_core/juce_core.cpp")
endif()

target_include_directories(Jucer2CMake_JUCE PUBLIC "${JUCE_ROOT}/modules")

target_compile_definitions(Jucer2CMake_JUCE PUBLIC
  $<$<CONFIG:Debug>:DEBUG=1>
  $<$<CONFIG:Debug>:_DEBUG=1>
  $<$<NOT:$<CONFIG:Debug>>:NDEBUG=1>
  JUCE_DISABLE_JUCE_VERSION_PRINTING=1
  JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1
  JUCE_MODULE_AVAILABLE_juce_core=1
  JUCE_STANDALONE_APPLICATION=1
  JUCE_USE_CURL=0
)

if(APPLE)
  target_compile_options(Jucer2CMake_JUCE PRIVATE -Wno-deprecated-declarations)

  find_library(Cocoa_framework "Cocoa")
  find_library(IOKit_framework "IOKit")

  target_link_libraries(Jucer2CMake_JUCE PUBLIC ${Cocoa_framework} ${IOKit_framework})
endif()

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
  target_compile_options(Jucer2CMake_JUCE PUBLIC -pthread)
  target_link_libraries(Jucer2CMake_JUCE PUBLIC dl pthread)
endif()

if(WIN32 AND NOT MSVC)
  target_compile_options(Jucer2CMake_JUCE PRIVATE -Wno-cpp)

  target_link_libraries(Jucer2CMake_JUCE PUBLIC
    -lshlwapi -lversion -lwininet -lwinmm -lws2_32
  )
endif()


option(Jucer2CMake_WRITE_CRLF_LINE_ENDINGS
  "Write CR+LF line endings in generated CMakeLists.txt files"
  OFF
)

set(Jucer2CMake_JUCE_LICENSE "Personal" CACHE STRING
  "JUCE License. Changes the default value for settings that depend on the license."
)
set_property(CACHE Jucer2CMake_JUCE_LICENSE PROPERTY STRINGS
  "Education" "GPL" "Indie" "Personal" "Pro"
)
string(CONCAT is_paid_or_gpl "$<OR:"
  "$<STREQUAL:${Jucer2CMake_JUCE_LICENSE},GPL>,"
  "$<STREQUAL:${Jucer2CMake_JUCE_LICENSE},Indie>,"
  "$<STREQUAL:${Jucer2CMake_JUCE_LICENSE},Pro>>"
)

add_executable(Jucer2CMake "${CMAKE_CURRENT_LIST_DIR}/src/main.cpp")

target_include_directories(Jucer2CMake PRIVATE "${CMAKE_CURRENT_LIST_DIR}/../third-party")

target_compile_definitions(Jucer2CMake PRIVATE
  IS_PAID_OR_GPL=${is_paid_or_gpl}
  WRITE_CRLF_LINE_ENDINGS=$<BOOL:${Jucer2CMake_WRITE_CRLF_LINE_ENDINGS}>
)

target_link_libraries(Jucer2CMake PRIVATE Jucer2CMake_JUCE)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  target_compile_options(Jucer2CMake PRIVATE
    -Werror -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded
  )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  target_compile_options(Jucer2CMake PRIVATE -Werror -Wall -Wextra)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  target_compile_options(Jucer2CMake PRIVATE /WX /W4 /wd4512)
endif()

install(TARGETS Jucer2CMake DESTINATION "FRUT/bin")
