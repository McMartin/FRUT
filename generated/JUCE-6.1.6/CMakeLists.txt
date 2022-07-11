# Copyright (C) 2021-2022  Alain Martin
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


project(JUCE-6.1.6)


if(MSVC_VERSION LESS 1900)
  message(WARNING "Skipping JUCE-6.1.6 because none of its sub-projects supports the "
    "\"Visual Studio 2013\" exporter"
  )
  return()
elseif(WIN32 AND NOT MSVC)
  message(WARNING "Skipping JUCE-6.1.6 because none of its sub-projects supports the "
    "\"Code::Blocks (Windows)\" exporter"
  )
  return()
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL Linux
    AND CMAKE_EXTRA_GENERATOR STREQUAL CodeBlocks)
  message(WARNING "Skipping JUCE-6.1.6 because none of its sub-projects supports the "
    "\"Code::Blocks (Linux)\" exporter"
  )
  return()
endif()


# All exporters

set(DemoRunner_jucer_FILE "${JUCE_ROOT}/examples/DemoRunner/DemoRunner.jucer")
add_subdirectory("examples/DemoRunner")

set(AudioPluginHost_jucer_FILE
  "${JUCE_ROOT}/extras/AudioPluginHost/AudioPluginHost.jucer"
)
add_subdirectory("extras/AudioPluginHost")


# All exporters but "Xcode (iOS)"

if(NOT IOS)
  set(Projucer_jucer_FILE "${JUCE_ROOT}/extras/Projucer/Projucer.jucer")
  add_subdirectory("extras/Projucer")
endif()


# All exporters but "Xcode (iOS)" and "Visual Studio 2015"

if(NOT IOS AND NOT MSVC_VERSION EQUAL 1900)
  set(UnitTestRunner_jucer_FILE "${JUCE_ROOT}/extras/UnitTestRunner/UnitTestRunner.jucer")
  add_subdirectory("extras/UnitTestRunner")
endif()


# All exporters but "Visual Studio 2019", "Visual Studio 2017", and "Visual Studio 2015"

if(NOT MSVC_VERSION LESS 1930)
  set(AudioPerformanceTest_jucer_FILE
    "${JUCE_ROOT}/extras/AudioPerformanceTest/AudioPerformanceTest.jucer"
  )
  add_subdirectory("extras/AudioPerformanceTest")

  set(NetworkGraphicsDemo_jucer_FILE
    "${JUCE_ROOT}/extras/NetworkGraphicsDemo/NetworkGraphicsDemo.jucer"
  )
  add_subdirectory("extras/NetworkGraphicsDemo")
endif()


# "Xcode (macOS)", "Visual Studio 2022" and "Linux Makefile" exporters

if((APPLE AND NOT IOS) OR MSVC_VERSION GREATER 1929
    OR CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
  set(BinaryBuilder_jucer_FILE "${JUCE_ROOT}/extras/BinaryBuilder/BinaryBuilder.jucer")
  add_subdirectory("extras/BinaryBuilder")
endif()


# "Visual Studio 2022" exporter only

if(MSVC_VERSION GREATER 1929)
  set(WindowsDLL_jucer_FILE "${JUCE_ROOT}/extras/WindowsDLL/WindowsDLL.jucer")
  add_subdirectory("extras/WindowsDLL")
endif()
