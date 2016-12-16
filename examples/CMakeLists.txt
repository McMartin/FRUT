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
add_subdirectory("SimpleFFTExample")
add_subdirectory("audio plugin host")

if(EXISTS "${JUCE_DIR}/examples/BLOCKS")
  if(DEFINED MSVC_VERSION AND MSVC_VERSION LESS 1900)
    message(WARNING
      "Not adding BLOCKS examples since they require at least Visual Studio 2015")
  else()
    add_subdirectory("BLOCKS/BlocksDrawing")
    add_subdirectory("BLOCKS/BlocksMonitor")
    add_subdirectory("BLOCKS/BlocksSynth")
  endif()
endif()
