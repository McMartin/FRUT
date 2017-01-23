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


set(JUCE_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../../JUCE" CACHE PATH "Path to JUCE")

set(SimpleFFTExample_jucer_FILE
  "${JUCE_ROOT}/examples/SimpleFFTExample/SimpleFFTExample.jucer"
  CACHE PATH "Path to SimpleFFTExample.jucer"
)

get_filename_component(SimpleFFTExample_jucer_DIR
  "${SimpleFFTExample_jucer_FILE}" DIRECTORY
)

file(COPY "${SimpleFFTExample_jucer_DIR}/Source" DESTINATION "${CMAKE_CURRENT_LIST_DIR}")


jucer_project_begin("SimpleFFTExample"
  PROJECT_ID "cgcxPd"
  PROJECT_VERSION "1.0.0"
  PROJECT_TYPE "GUI Application"
  BUNDLE_IDENTIFIER "com.yourcompany.SimpleFFTExample"
)

jucer_project_files("SimpleFFTExample/Source"
  "Source/Main.cpp"
  "Source/SpectrogramComponent.h"
)

jucer_project_module(
  juce_audio_basics
  PATH "${SimpleFFTExample_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_audio_devices
  PATH "${SimpleFFTExample_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_audio_formats
  PATH "${SimpleFFTExample_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_audio_processors
  PATH "${SimpleFFTExample_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_audio_utils
  PATH "${SimpleFFTExample_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_core
  PATH "${SimpleFFTExample_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_data_structures
  PATH "${SimpleFFTExample_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_events
  PATH "${SimpleFFTExample_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_graphics
  PATH "${SimpleFFTExample_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_gui_basics
  PATH "${SimpleFFTExample_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_gui_extra
  PATH "${SimpleFFTExample_jucer_DIR}/../../modules"
)

jucer_project_end()
