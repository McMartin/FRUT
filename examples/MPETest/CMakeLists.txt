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


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../..")
include(JUCE)


set(MPETest_jucer_FILE "${JUCE_DIR}/examples/MPETest/MPETest.jucer"
  CACHE PATH "Path to MPETest.jucer"
)

get_filename_component(MPETest_jucer_DIR "${MPETest_jucer_FILE}" DIRECTORY)

file(COPY "${MPETest_jucer_DIR}/Source" DESTINATION "${CMAKE_CURRENT_LIST_DIR}")


jucer_project_begin("MPETest"
  PROJECT_VERSION "1.0.0"
  PROJECT_TYPE "GUI Application"
)

jucer_project_files("MPETest/Source"
  "Source/Main.cpp"
  "Source/MainComponent.h"
  "Source/MPEDemoSynthVoice.h"
  "Source/MPESetupComponent.h"
  "Source/Visualiser.h"
  "Source/ZoneColourPicker.h"
  "Source/ZoneLayoutComponent.h"
)

jucer_project_module(
  juce_audio_basics
  PATH "${MPETest_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_audio_devices
  PATH "${MPETest_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_audio_formats
  PATH "${MPETest_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_audio_processors
  PATH "${MPETest_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_audio_utils
  PATH "${MPETest_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_core
  PATH "${MPETest_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_cryptography
  PATH "${MPETest_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_data_structures
  PATH "${MPETest_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_events
  PATH "${MPETest_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_graphics
  PATH "${MPETest_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_gui_basics
  PATH "${MPETest_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_gui_extra
  PATH "${MPETest_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_opengl
  PATH "${MPETest_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_video
  PATH "${MPETest_jucer_DIR}/../../modules"
)

jucer_project_end()
