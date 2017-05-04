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

cmake_minimum_required(VERSION 3.4)


if(NOT DEFINED JUCE_examples_DIR)
  message(FATAL_ERROR "JUCE_examples_DIR must be defined")
endif()

get_filename_component(JUCE_examples_DIR
  "${JUCE_examples_DIR}" ABSOLUTE
  BASE_DIR "${CMAKE_BINARY_DIR}"
)

if(NOT IS_DIRECTORY "${JUCE_examples_DIR}")
  message(FATAL_ERROR "No such directory: ${JUCE_examples_DIR}")
endif()


project("examples")


set(AnimationAppExample_jucer_FILE
  "${JUCE_examples_DIR}/AnimationAppExample/AnimationAppExample.jucer"
)
add_subdirectory("AnimationAppExample")


set(AudioAppExample_jucer_FILE
  "${JUCE_examples_DIR}/AudioAppExample/AudioAppExample.jucer"
)
add_subdirectory("AudioAppExample")


set(BouncingBallWavetableDemo_jucer_FILE
  "${JUCE_examples_DIR}/BouncingBallWavetableDemo/BouncingBallWavetableDemo.jucer"
)
add_subdirectory("BouncingBallWavetableDemo")


set(ComponentTutorialExample_jucer_FILE
  "${JUCE_examples_DIR}/ComponentTutorialExample/ComponentTutorialExample.jucer"
)
add_subdirectory("ComponentTutorialExample")


set(JuceDemo_jucer_FILE
  "${JUCE_examples_DIR}/Demo/JuceDemo.jucer"
)
add_subdirectory("Demo")


set(HelloWorld_jucer_FILE
  "${JUCE_examples_DIR}/HelloWorld/HelloWorld.jucer"
)
add_subdirectory("HelloWorld")


set(MPETest_jucer_FILE
  "${JUCE_examples_DIR}/MPETest/MPETest.jucer"
)
add_subdirectory("MPETest")


set(MidiTest_jucer_FILE
  "${JUCE_examples_DIR}/MidiTest/MidiTest.jucer"
)
add_subdirectory("MidiTest")


set(NetworkGraphicsDemo_jucer_FILE
  "${JUCE_examples_DIR}/NetworkGraphicsDemo/NetworkGraphicsDemo.jucer"
)
add_subdirectory("NetworkGraphicsDemo")


set(OSCMonitor_jucer_FILE
  "${JUCE_examples_DIR}/OSCMonitor/OSCMonitor.jucer"
)
add_subdirectory("OSCMonitor")


set(OSCReceiver_jucer_FILE
  "${JUCE_examples_DIR}/OSCReceiver/OSCReceiver.jucer"
)
add_subdirectory("OSCReceiver")


set(OSCSender_jucer_FILE
  "${JUCE_examples_DIR}/OSCSender/OSCSender.jucer"
)
add_subdirectory("OSCSender")


set(OpenGLAppExample_jucer_FILE
  "${JUCE_examples_DIR}/OpenGLAppExample/OpenGLAppExample.jucer"
)
add_subdirectory("OpenGLAppExample")


set(PluckedStringsDemo_jucer_FILE
  "${JUCE_examples_DIR}/PluckedStringsDemo/PluckedStringsDemo.jucer"
)
add_subdirectory("PluckedStringsDemo")


set(Arpeggiator_jucer_FILE
  "${JUCE_examples_DIR}/PlugInSamples/Arpeggiator/Arpeggiator.jucer"
)
add_subdirectory("PlugInSamples/Arpeggiator")


set(GainPlugIn_jucer_FILE
  "${JUCE_examples_DIR}/PlugInSamples/GainPlugIn/GainPlugIn.jucer"
)
add_subdirectory("PlugInSamples/GainPlugIn")


set(MultiOutSynth_jucer_FILE
  "${JUCE_examples_DIR}/PlugInSamples/MultiOutSynth/MultiOutSynth.jucer"
)
add_subdirectory("PlugInSamples/MultiOutSynth")


set(NoiseGate_jucer_FILE
  "${JUCE_examples_DIR}/PlugInSamples/NoiseGate/NoiseGate.jucer"
)
add_subdirectory("PlugInSamples/NoiseGate")


set(Surround_jucer_FILE
  "${JUCE_examples_DIR}/PlugInSamples/Surround/Surround.jucer"
)
add_subdirectory("PlugInSamples/Surround")


set(SimpleFFTExample_jucer_FILE
  "${JUCE_examples_DIR}/SimpleFFTExample/SimpleFFTExample.jucer"
)
add_subdirectory("SimpleFFTExample")


set(JuceDemoPlugin_jucer_FILE
  "${JUCE_examples_DIR}/audio plugin demo/JuceDemoPlugin.jucer"
)
add_subdirectory("audio plugin demo")


set(Plugin_Host_jucer_FILE
  "${JUCE_examples_DIR}/audio plugin host/Plugin Host.jucer"
)
add_subdirectory("audio plugin host")


if(IS_DIRECTORY "${JUCE_examples_DIR}/BLOCKS")
  if(DEFINED MSVC_VERSION AND MSVC_VERSION LESS 1900)
    message(WARNING
      "Not adding BLOCKS examples since they require at least Visual Studio 2015"
    )
  else()
    set(BlocksDrawing_jucer_FILE
      "${JUCE_examples_DIR}/BLOCKS/BlocksDrawing/BlocksDrawing.jucer"
    )
    add_subdirectory("BLOCKS/BlocksDrawing")

    set(BlocksMonitor_jucer_FILE
      "${JUCE_examples_DIR}/BLOCKS/BlocksMonitor/BlocksMonitor.jucer"
    )
    add_subdirectory("BLOCKS/BlocksMonitor")

    set(BlocksSynth_jucer_FILE
      "${JUCE_examples_DIR}/BLOCKS/BlocksSynth/BlocksSynth.jucer"
    )
    add_subdirectory("BLOCKS/BlocksSynth")
  endif()
endif()
