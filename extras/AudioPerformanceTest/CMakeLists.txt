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


set(AudioPerformanceTest_jucer_FILE
  "${JUCE_DIR}/extras/AudioPerformanceTest/AudioPerformanceTest.jucer"
  CACHE PATH "Path to AudioPerformanceTest.jucer")

get_filename_component(AudioPerformanceTest_jucer_DIR
  "${AudioPerformanceTest_jucer_FILE}" DIRECTORY)

file(COPY "${AudioPerformanceTest_jucer_DIR}/Source"
  DESTINATION "${CMAKE_CURRENT_LIST_DIR}")


jucer_project_begin("AudioPerformanceTest"
  PROJECT_TYPE "GUI Application"
)

jucer_project_files("AudioPerformanceTest/Source"
  "Source/Main.cpp"
  "Source/MainComponent.h"
)

jucer_project_module(
  juce_audio_basics
  PATH "${AudioPerformanceTest_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_audio_devices
  PATH "${AudioPerformanceTest_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_audio_formats
  PATH "${AudioPerformanceTest_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_audio_processors
  PATH "${AudioPerformanceTest_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_audio_utils
  PATH "${AudioPerformanceTest_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_core
  PATH "${AudioPerformanceTest_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_data_structures
  PATH "${AudioPerformanceTest_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_events
  PATH "${AudioPerformanceTest_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_graphics
  PATH "${AudioPerformanceTest_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_gui_basics
  PATH "${AudioPerformanceTest_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_gui_extra
  PATH "${AudioPerformanceTest_jucer_DIR}/../../modules"
)

jucer_project_end()
