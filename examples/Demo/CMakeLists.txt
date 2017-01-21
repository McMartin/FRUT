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


set(JUCE_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../JUCE" CACHE PATH "Path to JUCE")

set(JuceDemo_jucer_FILE "${JUCE_DIR}/examples/Demo/JuceDemo.jucer"
  CACHE PATH "Path to JuceDemo.jucer"
)

get_filename_component(JuceDemo_jucer_DIR "${JuceDemo_jucer_FILE}" DIRECTORY)

file(COPY "${JuceDemo_jucer_DIR}/Source" "${JuceDemo_jucer_DIR}/Resources"
  DESTINATION "${CMAKE_CURRENT_LIST_DIR}"
)


jucer_project_begin("JuceDemo"
  PROJECT_VERSION "3.0.0"
  PROJECT_TYPE "GUI Application"
  BUNDLE_IDENTIFIER "com.yourcompany.JuceDemo"
  PREPROCESSOR_DEFINITIONS "JUCE_UNIT_TESTS=1"
)

jucer_project_files("JuceDemo/Source"
  "Source/DemoUtilities.h"
  "Source/IntroScreen.cpp"
  "Source/JuceDemoHeader.h"
  "Source/Main.cpp"
  "Source/MainWindow.cpp"
  "Source/MainWindow.h"
)

jucer_project_files("JuceDemo/Source/Demos"
  "Source/Demos/AnimationDemo.cpp"
  "Source/Demos/AudioLatencyDemo.cpp"
  "Source/Demos/AudioLiveScrollingDisplay.h"
  "Source/Demos/AudioPlaybackDemo.cpp"
  "Source/Demos/AudioRecordingDemo.cpp"
  "Source/Demos/AudioSettingsDemo.cpp"
  "Source/Demos/AudioSynthesiserDemo.cpp"
  "Source/Demos/Box2DDemo.cpp"
  "Source/Demos/CameraDemo.cpp"
  "Source/Demos/ChildProcessDemo.cpp"
  "Source/Demos/CodeEditorDemo.cpp"
  "Source/Demos/ComponentTransformsDemo.cpp"
  "Source/Demos/CryptographyDemo.cpp"
  "Source/Demos/DialogsDemo.cpp"
  "Source/Demos/FontsDemo.cpp"
  "Source/Demos/GraphicsDemo.cpp"
  "Source/Demos/ImagesDemo.cpp"
  "Source/Demos/JavaScript.cpp"
  "Source/Demos/KeyMappingsDemo.cpp"
  "Source/Demos/LiveConstantDemo.cpp"
  "Source/Demos/LookAndFeelDemo.cpp"
  "Source/Demos/MDIDemo.cpp"
  "Source/Demos/MidiDemo.cpp"
  "Source/Demos/MultiTouch.cpp"
  "Source/Demos/MultithreadingDemo.cpp"
  "Source/Demos/NetworkingDemo.cpp"
  "Source/Demos/OpenGLDemo.cpp"
  "Source/Demos/OpenGLDemo2D.cpp"
  "Source/Demos/PropertiesDemo.cpp"
  "Source/Demos/SystemInfoDemo.cpp"
  "Source/Demos/TimersAndEventsDemo.cpp"
  "Source/Demos/UnitTestsDemo.cpp"
  "Source/Demos/ValueTreesDemo.cpp"
  "Source/Demos/VideoDemo.cpp"
  "Source/Demos/WavefrontObjParser.h"
  "Source/Demos/WebBrowserDemo.cpp"
  "Source/Demos/WidgetsDemo.cpp"
  "Source/Demos/WindowsDemo.cpp"
  "Source/Demos/XMLandJSONDemo.cpp"
)

if(EXISTS "${JuceDemo_jucer_DIR}/Source/Demos/FlexBoxDemo.cpp")
  jucer_project_files("JuceDemo/Source/Demos"
    "Source/Demos/FlexBoxDemo.cpp"
  )
endif()

jucer_project_resources("JuceDemo/Source/Resources"
  "Resources/cello.wav"
  "Resources/demo table data.xml"
  "Resources/icons.zip"
  "Resources/juce_icon.png"
  "Resources/juce_module_info"
  "Resources/portmeirion.jpg"
  "Resources/teapot.obj"
  "Resources/tile_background.png"
  "Resources/treedemo.xml"
)

jucer_project_module(
  juce_audio_basics
  PATH "${JuceDemo_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_audio_devices
  PATH "${JuceDemo_jucer_DIR}/../../modules"
  JUCE_WASAPI_EXCLUSIVE ON
)

jucer_project_module(
  juce_audio_formats
  PATH "${JuceDemo_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_audio_processors
  PATH "${JuceDemo_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_audio_utils
  PATH "${JuceDemo_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_box2d
  PATH "${JuceDemo_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_core
  PATH "${JuceDemo_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_cryptography
  PATH "${JuceDemo_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_data_structures
  PATH "${JuceDemo_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_events
  PATH "${JuceDemo_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_graphics
  PATH "${JuceDemo_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_gui_basics
  PATH "${JuceDemo_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_gui_extra
  PATH "${JuceDemo_jucer_DIR}/../../modules"
  JUCE_ENABLE_LIVE_CONSTANT_EDITOR ON
)

jucer_project_module(
  juce_opengl
  PATH "${JuceDemo_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_video
  PATH "${JuceDemo_jucer_DIR}/../../modules"
)

jucer_project_end()
