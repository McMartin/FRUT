# Copyright (c) 2017 Alain Martin
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


if(NOT DEFINED JUCE_VERSION)
  message(FATAL_ERROR "JUCE_VERSION must be defined")
endif()


project(AllJuceProjects-${JUCE_VERSION})


if(JUCE_VERSION STREQUAL 4.2.0)
  set(JUCER_VST_SDK_FOLDER
    "${CMAKE_CURRENT_LIST_DIR}/../third-party/vstsdk365_12_11_2015_build_67/VST3 SDK"
  )

  if(APPLE OR WIN32)
    set(JUCER_VST3_SDK_FOLDER
      "${CMAKE_CURRENT_LIST_DIR}/../third-party/vstsdk365_12_11_2015_build_67/VST3 SDK"
    )
  endif()
else()
  if(APPLE OR WIN32)
    set(JUCER_VST3_SDK_FOLDER
      "${CMAKE_CURRENT_LIST_DIR}/../third-party/vstsdk366_27_06_2016_build_61/VST3 SDK"
    )
  endif()
endif()


set(JUCE_ROOT "${CMAKE_CURRENT_LIST_DIR}/../tmp/JUCE-${JUCE_VERSION}")
set(generated_JUCE_ROOT
  "${CMAKE_CURRENT_LIST_DIR}/../../Jucer2Reprojucer/generated/JUCE-${JUCE_VERSION}"
)


# All exporters

set(JuceDemo_jucer_FILE "${JUCE_ROOT}/examples/Demo/JuceDemo.jucer")
add_subdirectory("${generated_JUCE_ROOT}/examples/Demo"
  "JuceDemo"
)

set(Plugin_Host_jucer_FILE "${JUCE_ROOT}/examples/audio plugin host/Plugin Host.jucer")
add_subdirectory("${generated_JUCE_ROOT}/examples/audio plugin host"
  "Plugin Host"
)

if(JUCE_VERSION STREQUAL "4.3.1")
  set(BlocksDrawing_jucer_FILE
    "${JUCE_ROOT}/examples/BLOCKS/BlocksDrawing/BlocksDrawing.jucer"
  )
  add_subdirectory("${generated_JUCE_ROOT}/examples/BLOCKS/BlocksDrawing"
    "BlocksDrawing"
  )

  set(BlocksMonitor_jucer_FILE
    "${JUCE_ROOT}/examples/BLOCKS/BlocksMonitor/BlocksMonitor.jucer"
  )
  add_subdirectory("${generated_JUCE_ROOT}/examples/BLOCKS/BlocksMonitor"
    "BlocksMonitor"
  )

  set(BlocksSynth_jucer_FILE "${JUCE_ROOT}/examples/BLOCKS/BlocksSynth/BlocksSynth.jucer")
  add_subdirectory("${generated_JUCE_ROOT}/examples/BLOCKS/BlocksSynth"
    "BlocksSynth"
  )

  set(Projucer_jucer_FILE "${JUCE_ROOT}/extras/Projucer/Projucer.jucer")
  add_subdirectory("${generated_JUCE_ROOT}/extras/Projucer"
    "Projucer"
  )
endif()


# All exporters but "Visual Studio 2015"

if(NOT MSVC_VERSION EQUAL 1900)
  set(BouncingBallWavetableDemo_jucer_FILE
    "${JUCE_ROOT}/examples/BouncingBallWavetableDemo/BouncingBallWavetableDemo.jucer"
  )
  add_subdirectory("${generated_JUCE_ROOT}/examples/BouncingBallWavetableDemo"
    "BouncingBallWavetableDemo"
  )

  set(NetworkGraphicsDemo_jucer_FILE
    "${JUCE_ROOT}/examples/NetworkGraphicsDemo/NetworkGraphicsDemo.jucer"
  )
  add_subdirectory("${generated_JUCE_ROOT}/examples/NetworkGraphicsDemo"
    "NetworkGraphicsDemo"
  )
endif()


# All exporters but "Visual Studio 2013"

if(NOT MSVC_VERSION EQUAL 1800)
  set(HelloWorld_jucer_FILE "${JUCE_ROOT}/examples/HelloWorld/HelloWorld.jucer")
  add_subdirectory("${generated_JUCE_ROOT}/examples/HelloWorld"
    "HelloWorld"
  )

  set(MPETest_jucer_FILE "${JUCE_ROOT}/examples/MPETest/MPETest.jucer")
  add_subdirectory("${generated_JUCE_ROOT}/examples/MPETest"
    "MPETest"
  )

  set(MidiTest_jucer_FILE "${JUCE_ROOT}/examples/MidiTest/MidiTest.jucer")
  add_subdirectory("${generated_JUCE_ROOT}/examples/MidiTest"
    "MidiTest"
  )

  set(OSCMonitor_jucer_FILE "${JUCE_ROOT}/examples/OSCMonitor/OSCMonitor.jucer")
  add_subdirectory("${generated_JUCE_ROOT}/examples/OSCMonitor"
    "OSCMonitor"
  )

  set(OSCReceiver_jucer_FILE "${JUCE_ROOT}/examples/OSCReceiver/OSCReceiver.jucer")
  add_subdirectory("${generated_JUCE_ROOT}/examples/OSCReceiver"
    "OSCReceiver"
  )

  set(OSCSender_jucer_FILE "${JUCE_ROOT}/examples/OSCSender/OSCSender.jucer")
  add_subdirectory("${generated_JUCE_ROOT}/examples/OSCSender"
    "OSCSender"
  )

  set(JuceDemoPlugin_jucer_FILE
    "${JUCE_ROOT}/examples/audio plugin demo/JuceDemoPlugin.jucer"
  )
  add_subdirectory("${generated_JUCE_ROOT}/examples/audio plugin demo"
    "JuceDemoPlugin"
  )

  set(AudioPerformanceTest_jucer_FILE
    "${JUCE_ROOT}/extras/AudioPerformanceTest/AudioPerformanceTest.jucer"
  )
  add_subdirectory("${generated_JUCE_ROOT}/extras/AudioPerformanceTest"
    "AudioPerformanceTest"
  )

  set(UnitTestRunner_jucer_FILE "${JUCE_ROOT}/extras/UnitTestRunner/UnitTestRunner.jucer")
  add_subdirectory("${generated_JUCE_ROOT}/extras/UnitTestRunner"
    "UnitTestRunner"
  )
endif()


# "Xcode (MacOSX)" and "Visual Studio 2015" exporters

if(APPLE OR MSVC_VERSION EQUAL 1900)
  set(Arpeggiator_jucer_FILE
    "${JUCE_ROOT}/examples/PlugInSamples/Arpeggiator/Arpeggiator.jucer"
  )
  add_subdirectory("${generated_JUCE_ROOT}/examples/PlugInSamples/Arpeggiator"
    "Arpeggiator"
  )

  set(GainPlugIn_jucer_FILE
    "${JUCE_ROOT}/examples/PlugInSamples/GainPlugIn/GainPlugIn.jucer"
  )
  add_subdirectory("${generated_JUCE_ROOT}/examples/PlugInSamples/GainPlugIn"
    "GainPlugIn"
  )

  set(MultiOutSynth_jucer_FILE
    "${JUCE_ROOT}/examples/PlugInSamples/MultiOutSynth/MultiOutSynth.jucer"
  )
  add_subdirectory("${generated_JUCE_ROOT}/examples/PlugInSamples/MultiOutSynth"
    "MultiOutSynth"
  )

  set(NoiseGate_jucer_FILE
    "${JUCE_ROOT}/examples/PlugInSamples/NoiseGate/NoiseGate.jucer"
  )
  add_subdirectory("${generated_JUCE_ROOT}/examples/PlugInSamples/NoiseGate"
    "NoiseGate"
  )
endif()


# "Xcode (MacOSX)" and "Visual Studio 2013" exporters

if(APPLE OR MSVC_VERSION EQUAL 1800)
  set(PluckedStringsDemo_jucer_FILE
    "${JUCE_ROOT}/examples/PluckedStringsDemo/PluckedStringsDemo.jucer"
  )
  add_subdirectory("${generated_JUCE_ROOT}/examples/PluckedStringsDemo"
    "PluckedStringsDemo"
  )

  set(SimpleFFTExample_jucer_FILE
    "${JUCE_ROOT}/examples/SimpleFFTExample/SimpleFFTExample.jucer"
  )
  add_subdirectory("${generated_JUCE_ROOT}/examples/SimpleFFTExample"
    "SimpleFFTExample"
  )
endif()


# "Xcode (MacOSX)" and "Linux Makefile" exporters

if(APPLE OR CMAKE_HOST_SYSTEM_NAME STREQUAL Linux)
  set(Surround_jucer_FILE "${JUCE_ROOT}/examples/PlugInSamples/Surround/Surround.jucer")
  add_subdirectory("${generated_JUCE_ROOT}/examples/PlugInSamples/Surround"
    "Surround"
  )

  set(BinaryBuilder_jucer_FILE "${JUCE_ROOT}/extras/binarybuilder/BinaryBuilder.jucer")
  add_subdirectory("${generated_JUCE_ROOT}/extras/binarybuilder"
    "BinaryBuilder"
  )

  if(JUCE_VERSION STREQUAL "4.2.0")
    set(Projucer_jucer_FILE "${JUCE_ROOT}/extras/Projucer/Projucer.jucer")
    add_subdirectory("${generated_JUCE_ROOT}/extras/Projucer"
      "Projucer"
    )
  endif()
endif()


# "Xcode (MacOSX)" exporter only

if(APPLE)
  set(AUv3Synth_jucer_FILE "${JUCE_ROOT}/examples/AUv3Synth/AUv3Synth.jucer")
  add_subdirectory("${generated_JUCE_ROOT}/examples/AUv3Synth"
    "AUv3Synth"
  )

  set(AnimationAppExample_jucer_FILE
    "${JUCE_ROOT}/examples/AnimationAppExample/AnimationAppExample.jucer"
  )
  add_subdirectory("${generated_JUCE_ROOT}/examples/AnimationAppExample"
    "AnimationAppExample"
  )

  set(AudioAppExample_jucer_FILE
    "${JUCE_ROOT}/examples/AudioAppExample/AudioAppExample.jucer"
  )
  add_subdirectory("${generated_JUCE_ROOT}/examples/AudioAppExample"
    "AudioAppExample"
  )

  set(ComponentTutorialExample_jucer_FILE
    "${JUCE_ROOT}/examples/ComponentTutorialExample/ComponentTutorialExample.jucer"
  )
  add_subdirectory("${generated_JUCE_ROOT}/examples/ComponentTutorialExample"
    "ComponentTutorialExample"
  )

  set(OpenGLAppExample_jucer_FILE
    "${JUCE_ROOT}/examples/OpenGLAppExample/OpenGLAppExample.jucer"
  )
  add_subdirectory("${generated_JUCE_ROOT}/examples/OpenGLAppExample"
    "OpenGLAppExample"
  )
endif()
