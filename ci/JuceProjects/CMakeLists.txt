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


if(NOT DEFINED JUCE_VERSION)
  message(FATAL_ERROR "JUCE_VERSION must be defined")
endif()


project(JuceProjects-${JUCE_VERSION})


if(JUCE_VERSION STREQUAL 4.2.0)
  set(JUCER_VST_SDK_FOLDER
    "${CMAKE_CURRENT_LIST_DIR}/../third-party/vstsdk366_27_06_2016_build_61/VST3 SDK"
  )
endif()

if(APPLE OR WIN32)
  set(JUCER_VST3_SDK_FOLDER
    "${CMAKE_CURRENT_LIST_DIR}/../third-party/vstsdk366_27_06_2016_build_61/VST3 SDK"
  )
endif()


set(JUCE_ROOT "${CMAKE_CURRENT_LIST_DIR}/../tmp/JUCE-${JUCE_VERSION}")
set(generated_JUCE_ROOT
  "${CMAKE_CURRENT_LIST_DIR}/../../Jucer2Reprojucer/generated/JUCE-${JUCE_VERSION}"
)


set(Plugin_Host_jucer_FILE "${JUCE_ROOT}/examples/audio plugin host/Plugin Host.jucer")
add_subdirectory("${generated_JUCE_ROOT}/examples/audio plugin host"
  "Plugin Host"
)

set(JuceDemoPlugin_jucer_FILE "${JUCE_ROOT}/examples/audio plugin demo/JuceDemoPlugin.jucer")
if(NOT MSVC_VERSION EQUAL 1800)
  add_subdirectory("${generated_JUCE_ROOT}/examples/audio plugin demo"
    "JuceDemoPlugin"
  )
endif()

set(Projucer_jucer_FILE "${JUCE_ROOT}/extras/Projucer/Projucer.jucer")
if(NOT JUCE_VERSION STREQUAL "4.2.0"
    OR (NOT MSVC_VERSION EQUAL 1800 AND NOT MSVC_VERSION EQUAL 1900))
  add_subdirectory("${generated_JUCE_ROOT}/extras/Projucer"
    "Projucer"
  )
endif()
