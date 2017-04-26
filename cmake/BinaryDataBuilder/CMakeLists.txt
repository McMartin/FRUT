# Copyright (c) 2016 Alain Martin
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required(VERSION 3.0)


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../cmake")
include(Reprojucer)


if(NOT DEFINED Projucer_jucer_FILE)
  message(FATAL_ERROR "Projucer_jucer_FILE must be defined")
endif()

if(NOT EXISTS "${Projucer_jucer_FILE}")
  message(FATAL_ERROR "${Projucer_jucer_FILE} must exist")
endif()

get_filename_component(Projucer_jucer_DIR "${Projucer_jucer_FILE}" DIRECTORY)

file(COPY "${Projucer_jucer_DIR}/Source/jucer_Headers.h"
  DESTINATION "${CMAKE_CURRENT_LIST_DIR}/Source"
)

file(COPY
  "${Projucer_jucer_DIR}/Source/Project Saving/jucer_ResourceFile.cpp"
  "${Projucer_jucer_DIR}/Source/Project Saving/jucer_ResourceFile.h"
  DESTINATION "${CMAKE_CURRENT_LIST_DIR}/Source/Project Saving"
)

file(COPY
  "${Projucer_jucer_DIR}/Source/Utility/jucer_CodeHelpers.cpp"
  "${Projucer_jucer_DIR}/Source/Utility/jucer_CodeHelpers.h"
  "${Projucer_jucer_DIR}/Source/Utility/jucer_FileHelpers.cpp"
  "${Projucer_jucer_DIR}/Source/Utility/jucer_FileHelpers.h"
  "${Projucer_jucer_DIR}/Source/Utility/jucer_MiscUtilities.cpp"
  "${Projucer_jucer_DIR}/Source/Utility/jucer_MiscUtilities.h"
  DESTINATION "${CMAKE_CURRENT_LIST_DIR}/Source/Utility"
)


jucer_project_begin(
  PROJECT_NAME "BinaryDataBuilder"
  PROJECT_VERSION "1.0.0"
  PROJECT_TYPE "Console Application"
)

jucer_project_files("BinaryDataBuilder/Source"
  "Source/Project Saving/jucer_ResourceFile.cpp"
  "Source/Utility/jucer_CodeHelpers.cpp"
  "Source/Utility/jucer_FileHelpers.cpp"
  "Source/Utility/jucer_MiscUtilities.cpp"
  "main.cpp"
)

jucer_project_module(
  juce_core
  PATH "${Projucer_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_cryptography
  PATH "${Projucer_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_data_structures
  PATH "${Projucer_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_events
  PATH "${Projucer_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_graphics
  PATH "${Projucer_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_gui_basics
  PATH "${Projucer_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_gui_extra
  PATH "${Projucer_jucer_DIR}/../../modules"
)

jucer_project_end()


install(TARGETS BinaryDataBuilder DESTINATION "BinaryDataBuilder")
