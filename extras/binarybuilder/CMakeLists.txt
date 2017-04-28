# Copyright (c) 2016 Alain Martin
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

cmake_minimum_required(VERSION 3.0)


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../cmake")
include(Reprojucer)


if(NOT DEFINED BinaryBuilder_jucer)
  message(FATAL_ERROR "BinaryBuilder_jucer must be defined")
endif()

get_filename_component(BinaryBuilder_jucer_FILE
  "${BinaryBuilder_jucer}" REALPATH
  BASE_DIR "${CMAKE_BINARY_DIR}"
)

if(NOT EXISTS "${BinaryBuilder_jucer_FILE}")
  message(FATAL_ERROR "No such file: ${BinaryBuilder_jucer_FILE}")
endif()

get_filename_component(BinaryBuilder_jucer_DIR "${BinaryBuilder_jucer_FILE}" DIRECTORY)


jucer_project_begin(
  PROJECT_NAME "BinaryBuilder"
  PROJECT_VERSION "1.0.0"
  # COMPANY_NAME
  # COMPANY_WEBSITE
  # COMPANY_EMAIL
  PROJECT_TYPE "Console Application"
  BUNDLE_IDENTIFIER "com.rawmaterialsoftware.binarybuilder"
  BINARYDATACPP_SIZE_LIMIT "Default"
  # BINARYDATA_NAMESPACE
  # PREPROCESSOR_DEFINITIONS
  PROJECT_ID "3t6YqETY1"
)

jucer_project_files("BinaryBuilder/Source"
  "${BinaryBuilder_jucer_DIR}/Source/Main.cpp"
)

jucer_project_module(
  juce_core
  PATH "${BinaryBuilder_jucer_DIR}/../../modules"
  # JUCE_FORCE_DEBUG
  # JUCE_LOG_ASSERTIONS
  # JUCE_CHECK_MEMORY_LEAKS
  # JUCE_DONT_AUTOLINK_TO_WIN32_LIBRARIES
  # JUCE_INCLUDE_ZLIB_CODE
  # JUCE_USE_CURL
  # JUCE_CATCH_UNHANDLED_EXCEPTIONS
  # JUCE_ALLOW_STATIC_NULL_VARIABLES
)

jucer_project_end()
