# Copyright (c) 2017 Alain Martin
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

if(NOT DEFINED JUCE_ROOT)
  message(FATAL_ERROR "JUCE_ROOT must be defined")
endif()

if(NOT IS_DIRECTORY ${JUCE_ROOT})
  message(FATAL_ERROR "No such directory: ${JUCE_ROOT}")
endif()

get_filename_component(JUCE_ROOT "${JUCE_ROOT}" ABSOLUTE)


if(NOT DEFINED Jucer2CMake_EXE)
  message(FATAL_ERROR "Jucer2CMake_EXE must be defined")
endif()

if(NOT EXISTS ${Jucer2CMake_EXE})
  message(FATAL_ERROR "No such file: ${Jucer2CMake_EXE}")
endif()

get_filename_component(Jucer2CMake_EXE "${Jucer2CMake_EXE}" ABSOLUTE)


file(GLOB_RECURSE jucer_files RELATIVE "${JUCE_ROOT}" "${JUCE_ROOT}/*.jucer")

foreach(jucer_file ${jucer_files})
  get_filename_component(working_dir
    "${CMAKE_CURRENT_LIST_DIR}/JUCE/${jucer_file}" DIRECTORY
  )

  execute_process(WORKING_DIRECTORY ${working_dir}
    COMMAND
    "${Jucer2CMake_EXE}"
    "${JUCE_ROOT}/${jucer_file}"
    "${CMAKE_CURRENT_LIST_DIR}/../../cmake/Reprojucer.cmake"
  )
endforeach()
