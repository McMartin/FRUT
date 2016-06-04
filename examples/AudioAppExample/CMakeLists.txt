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


set(AudioAppExample_jucer_FILE
  "${JUCE_DIR}/examples/AudioAppExample/AudioAppExample.jucer"
  CACHE PATH "Path to AudioAppExample.jucer")

get_filename_component(AudioAppExample_jucer_DIR
  "${AudioAppExample_jucer_FILE}" DIRECTORY)

file(COPY "${AudioAppExample_jucer_DIR}/Source" DESTINATION "${CMAKE_CURRENT_LIST_DIR}")


jucer_project_begin("AudioAppExample")

jucer_project_files("AudioAppExample/Source"
  "Source/Main.cpp"
  "Source/MainComponent.cpp"
)

jucer_project_module(
  juce_audio_basics
  PATH "${AudioAppExample_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_audio_devices
  PATH "${AudioAppExample_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_audio_formats
  PATH "${AudioAppExample_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_audio_processors
  PATH "${AudioAppExample_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_audio_utils
  PATH "${AudioAppExample_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_core
  PATH "${AudioAppExample_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_cryptography
  PATH "${AudioAppExample_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_data_structures
  PATH "${AudioAppExample_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_events
  PATH "${AudioAppExample_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_graphics
  PATH "${AudioAppExample_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_gui_basics
  PATH "${AudioAppExample_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_gui_extra
  PATH "${AudioAppExample_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_opengl
  PATH "${AudioAppExample_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_video
  PATH "${AudioAppExample_jucer_DIR}/../../modules"
)

jucer_project_end()
