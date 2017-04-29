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

project("examples")

add_subdirectory("AnimationAppExample")
add_subdirectory("AudioAppExample")
add_subdirectory("BouncingBallWavetableDemo")
add_subdirectory("ComponentTutorialExample")
add_subdirectory("Demo")
add_subdirectory("HelloWorld")
add_subdirectory("MPETest")
add_subdirectory("MidiTest")
add_subdirectory("NetworkGraphicsDemo")
add_subdirectory("OSCMonitor")
add_subdirectory("OSCReceiver")
add_subdirectory("OSCSender")
add_subdirectory("OpenGLAppExample")
add_subdirectory("PluckedStringsDemo")
add_subdirectory("PlugInSamples/Arpeggiator")
add_subdirectory("PlugInSamples/GainPlugIn")
add_subdirectory("PlugInSamples/MultiOutSynth")
add_subdirectory("PlugInSamples/NoiseGate")
add_subdirectory("PlugInSamples/Surround")
add_subdirectory("SimpleFFTExample")
add_subdirectory("audio plugin demo")
add_subdirectory("audio plugin host")

if(EXISTS "${JUCE_ROOT}/examples/BLOCKS")
  if(DEFINED MSVC_VERSION AND MSVC_VERSION LESS 1900)
    message(WARNING
      "Not adding BLOCKS examples since they require at least Visual Studio 2015"
    )
  else()
    add_subdirectory("BLOCKS/BlocksDrawing")
    add_subdirectory("BLOCKS/BlocksMonitor")
    add_subdirectory("BLOCKS/BlocksSynth")
  endif()
endif()
