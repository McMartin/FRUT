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


if(NOT DEFINED JUCE_ROOT)
  message(FATAL_ERROR "JUCE_ROOT must be defined")
endif()

get_filename_component(JUCE_ROOT "${JUCE_ROOT}" ABSOLUTE BASE_DIR "${CMAKE_BINARY_DIR}")


project(JUCE-5.2.1)


if(MSVC_VERSION GREATER 1919)
  message(WARNING "Skipping JUCE-5.2.1 because none of its sub-projects supports the "
    "\"Visual Studio 2019\" exporter"
  )
  return()
elseif(WIN32 AND NOT MSVC)
  message(WARNING "Skipping JUCE-5.2.1 because none of its sub-projects supports the "
    "\"Code::Blocks (Windows)\" exporter"
  )
  return()
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL Linux
    AND CMAKE_EXTRA_GENERATOR STREQUAL CodeBlocks)
  message(WARNING "Skipping JUCE-5.2.1 because none of its sub-projects supports the "
    "\"Code::Blocks (Linux)\" exporter"
  )
  return()
endif()


# All exporters

set(JuceDemo_jucer_FILE "${JUCE_ROOT}/examples/Demo/JuceDemo.jucer")
add_subdirectory("examples/Demo")


# All exporters but "Xcode (iOS)"

if(NOT IOS)
  set(Plugin_Host_jucer_FILE "${JUCE_ROOT}/examples/audio plugin host/Plugin Host.jucer")
  add_subdirectory("examples/audio plugin host")

  set(Projucer_jucer_FILE "${JUCE_ROOT}/extras/Projucer/Projucer.jucer")
  add_subdirectory("extras/Projucer")
endif()


# All exporters but "Visual Studio 2015"

if(NOT MSVC_VERSION EQUAL 1900)
  set(NetworkGraphicsDemo_jucer_FILE
    "${JUCE_ROOT}/examples/NetworkGraphicsDemo/NetworkGraphicsDemo.jucer"
  )
  add_subdirectory("examples/NetworkGraphicsDemo")
endif()


# All exporters but "Visual Studio 2013"

if(NOT MSVC_VERSION EQUAL 1800)
  set(BlocksDrawing_jucer_FILE
    "${JUCE_ROOT}/examples/BLOCKS/BlocksDrawing/BlocksDrawing.jucer"
  )
  add_subdirectory("examples/BLOCKS/BlocksDrawing")

  set(BlocksMonitor_jucer_FILE
    "${JUCE_ROOT}/examples/BLOCKS/BlocksMonitor/BlocksMonitor.jucer"
  )
  add_subdirectory("examples/BLOCKS/BlocksMonitor")

  set(BlocksSynth_jucer_FILE "${JUCE_ROOT}/examples/BLOCKS/BlocksSynth/BlocksSynth.jucer")
  add_subdirectory("examples/BLOCKS/BlocksSynth")

  set(HelloWorld_jucer_FILE "${JUCE_ROOT}/examples/HelloWorld/HelloWorld.jucer")
  add_subdirectory("examples/HelloWorld")

  set(MidiTest_jucer_FILE "${JUCE_ROOT}/examples/MidiTest/MidiTest.jucer")
  add_subdirectory("examples/MidiTest")

  set(OSCReceiver_jucer_FILE "${JUCE_ROOT}/examples/OSCReceiver/OSCReceiver.jucer")
  add_subdirectory("examples/OSCReceiver")

  set(OSCSender_jucer_FILE "${JUCE_ROOT}/examples/OSCSender/OSCSender.jucer")
  add_subdirectory("examples/OSCSender")

  set(JuceDemoPlugin_jucer_FILE
    "${JUCE_ROOT}/examples/audio plugin demo/JuceDemoPlugin.jucer"
  )
  add_subdirectory("examples/audio plugin demo")
endif()


# All exporters but "Xcode (iOS)" and "Visual Studio 2015"

if(NOT IOS AND NOT MSVC_VERSION EQUAL 1900)
  set(BouncingBallWavetableDemo_jucer_FILE
    "${JUCE_ROOT}/examples/BouncingBallWavetableDemo/BouncingBallWavetableDemo.jucer"
  )
  add_subdirectory("examples/BouncingBallWavetableDemo")
endif()


# All exporters but "Xcode (iOS)" and "Visual Studio 2013"

if(NOT IOS AND NOT MSVC_VERSION EQUAL 1800)
  set(MPETest_jucer_FILE "${JUCE_ROOT}/examples/MPETest/MPETest.jucer")
  add_subdirectory("examples/MPETest")

  set(OSCMonitor_jucer_FILE "${JUCE_ROOT}/examples/OSCMonitor/OSCMonitor.jucer")
  add_subdirectory("examples/OSCMonitor")

  set(Surround_jucer_FILE "${JUCE_ROOT}/examples/PlugInSamples/Surround/Surround.jucer")
  add_subdirectory("examples/PlugInSamples/Surround")
endif()


# All exporters but "Visual Studio 2017" and "Visual Studio 2013"

if(NOT MSVC_VERSION GREATER 1909 AND NOT MSVC_VERSION EQUAL 1800)
  set(AudioPerformanceTest_jucer_FILE
    "${JUCE_ROOT}/extras/AudioPerformanceTest/AudioPerformanceTest.jucer"
  )
  add_subdirectory("extras/AudioPerformanceTest")
endif()


# All exporters but "Visual Studio 2015" and "Visual Studio 2013"

if(NOT MSVC_VERSION EQUAL 1900 AND NOT MSVC_VERSION EQUAL 1800)
  set(AnalyticsCollection_jucer_FILE
    "${JUCE_ROOT}/examples/AnalyticsCollection/AnalyticsCollection.jucer"
  )
  add_subdirectory("examples/AnalyticsCollection")
endif()


# "Xcode (MacOSX)", "Xcode (iOS)" and "Visual Studio 2017" exporters

if(APPLE OR MSVC_VERSION GREATER 1909)
  set(AnimationAppExample_jucer_FILE
    "${JUCE_ROOT}/examples/AnimationAppExample/AnimationAppExample.jucer"
  )
  add_subdirectory("examples/AnimationAppExample")

  set(AudioAppExample_jucer_FILE
    "${JUCE_ROOT}/examples/AudioAppExample/AudioAppExample.jucer"
  )
  add_subdirectory("examples/AudioAppExample")

  set(OpenGLAppExample_jucer_FILE
    "${JUCE_ROOT}/examples/OpenGLAppExample/OpenGLAppExample.jucer"
  )
  add_subdirectory("examples/OpenGLAppExample")
endif()


# "Xcode (MacOSX)", "Xcode (iOS)" and "Visual Studio 2015" exporters

if(APPLE OR MSVC_VERSION EQUAL 1900)
  set(PushNotificationsDemo_jucer_FILE
    "${JUCE_ROOT}/examples/PushNotificationsDemo/PushNotificationsDemo.jucer"
  )
  add_subdirectory("examples/PushNotificationsDemo")
endif()


# "Xcode (MacOSX)", "Visual Studio 2017" and "Visual Studio 2015" exporters

if((APPLE AND NOT IOS) OR MSVC_VERSION GREATER 1909 OR MSVC_VERSION EQUAL 1900)
  set(ComponentTutorialExample_jucer_FILE
    "${JUCE_ROOT}/examples/ComponentTutorialExample/ComponentTutorialExample.jucer"
  )
  add_subdirectory("examples/ComponentTutorialExample")

  set(DSP_module_plugin_demo_jucer_FILE
    "${JUCE_ROOT}/examples/DSP module plugin demo/DSP module plugin demo.jucer"
  )
  add_subdirectory("examples/DSP module plugin demo")

  set(Arpeggiator_jucer_FILE
    "${JUCE_ROOT}/examples/PlugInSamples/Arpeggiator/Arpeggiator.jucer"
  )
  add_subdirectory("examples/PlugInSamples/Arpeggiator")

  set(GainPlugIn_jucer_FILE
    "${JUCE_ROOT}/examples/PlugInSamples/GainPlugIn/GainPlugIn.jucer"
  )
  add_subdirectory("examples/PlugInSamples/GainPlugIn")

  set(MultiOutSynth_jucer_FILE
    "${JUCE_ROOT}/examples/PlugInSamples/MultiOutSynth/MultiOutSynth.jucer"
  )
  add_subdirectory("examples/PlugInSamples/MultiOutSynth")

  set(NoiseGate_jucer_FILE
    "${JUCE_ROOT}/examples/PlugInSamples/NoiseGate/NoiseGate.jucer"
  )
  add_subdirectory("examples/PlugInSamples/NoiseGate")
endif()


# "Xcode (MacOSX)", "Visual Studio 2017" and "Visual Studio 2013" exporters

if((APPLE AND NOT IOS) OR MSVC_VERSION GREATER 1909 OR MSVC_VERSION EQUAL 1800)
  set(PluckedStringsDemo_jucer_FILE
    "${JUCE_ROOT}/examples/PluckedStringsDemo/PluckedStringsDemo.jucer"
  )
  add_subdirectory("examples/PluckedStringsDemo")

  set(SimpleFFTExample_jucer_FILE
    "${JUCE_ROOT}/examples/SimpleFFTExample/SimpleFFTExample.jucer"
  )
  add_subdirectory("examples/SimpleFFTExample")
endif()


# "Xcode (MacOSX)", "Visual Studio 2017" and "Linux Makefile" exporters

if((APPLE AND NOT IOS) OR MSVC_VERSION GREATER 1909
    OR CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
  set(DSPDemo_jucer_FILE "${JUCE_ROOT}/examples/DSPDemo/DSPDemo.jucer")
  add_subdirectory("examples/DSPDemo")

  set(UnitTestRunner_jucer_FILE "${JUCE_ROOT}/extras/UnitTestRunner/UnitTestRunner.jucer")
  add_subdirectory("extras/UnitTestRunner")

  set(BinaryBuilder_jucer_FILE "${JUCE_ROOT}/extras/binarybuilder/BinaryBuilder.jucer")
  add_subdirectory("extras/binarybuilder")
endif()


# "Xcode (MacOSX)" and "Xcode (iOS)" exporters

if(APPLE)
  set(AUv3Synth_jucer_FILE "${JUCE_ROOT}/examples/AUv3Synth/AUv3Synth.jucer")
  add_subdirectory("examples/AUv3Synth")

  set(InAppPurchase_jucer_FILE "${JUCE_ROOT}/examples/InAppPurchase/InAppPurchase.jucer")
  add_subdirectory("examples/InAppPurchase")
endif()


# "Xcode (iOS)" exporter only

if(IOS)
  set(InterAppAudioEffect_jucer_FILE
    "${JUCE_ROOT}/examples/PlugInSamples/InterAppAudioEffect/InterAppAudioEffect.jucer"
  )
  add_subdirectory("examples/PlugInSamples/InterAppAudioEffect")
endif()


# "Visual Studio 2017" exporter only

if(MSVC_VERSION GREATER 1909)
  set(jucedll_jucer_FILE "${JUCE_ROOT}/extras/windows dll/jucedll.jucer")
  add_subdirectory("extras/windows dll")
endif()
