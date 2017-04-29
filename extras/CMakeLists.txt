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


if(NOT DEFINED JUCE_extras_DIR)
  message(FATAL_ERROR "JUCE_extras_DIR must be defined")
endif()

get_filename_component(JUCE_extras_DIR
  "${JUCE_extras_DIR}" ABSOLUTE
  BASE_DIR "${CMAKE_CURRENT_BINARY_DIR}"
)

if(NOT EXISTS "${JUCE_extras_DIR}")
  message(FATAL_ERROR "No such directory: ${JUCE_extras_DIR}")
endif()


project("extras")


set(AudioPerformanceTest_jucer_FILE
  "${JUCE_extras_DIR}/AudioPerformanceTest/AudioPerformanceTest.jucer"
)
add_subdirectory("AudioPerformanceTest")


set(Projucer_jucer_FILE
  "${JUCE_extras_DIR}/Projucer/Projucer.jucer"
)
add_subdirectory("Projucer")


set(UnitTestRunner_jucer_FILE
  "${JUCE_extras_DIR}/UnitTestRunner/UnitTestRunner.jucer"
)
add_subdirectory("UnitTestRunner")


set(BinaryBuilder_jucer_FILE
  "${JUCE_extras_DIR}/binarybuilder/BinaryBuilder.jucer"
)
add_subdirectory("binarybuilder")
