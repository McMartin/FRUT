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


jucer_project_begin(
  PROJECT_NAME "Jucer2CMake"
  PROJECT_VERSION "1.0.0"
  PROJECT_TYPE "Console Application"
)

jucer_project_files("Jucer2CMake"
  "main.cpp"
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

jucer_project_end()


target_compile_definitions(Jucer2CMake PRIVATE DONT_SET_USING_JUCE_NAMESPACE=1)
