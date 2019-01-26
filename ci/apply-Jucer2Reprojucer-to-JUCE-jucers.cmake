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

if(NOT DEFINED JUCE_VERSION)
  message(FATAL_ERROR "JUCE_VERSION must be defined")
endif()

set(JUCE_ROOT "${CMAKE_CURRENT_LIST_DIR}/tmp/JUCE-${JUCE_VERSION}")
if(NOT IS_DIRECTORY "${JUCE_ROOT}")
  message(FATAL_ERROR "No such directory: ${JUCE_ROOT}")
endif()

set(generated_JUCE_ROOT "${CMAKE_CURRENT_LIST_DIR}/../generated/JUCE-${JUCE_VERSION}")
if(NOT IS_DIRECTORY "${generated_JUCE_ROOT}")
  message(FATAL_ERROR "No such directory: ${generated_JUCE_ROOT}")
endif()


if(NOT DEFINED Jucer2Reprojucer_EXE)
  message(FATAL_ERROR "Jucer2Reprojucer_EXE must be defined")
endif()

if(NOT EXISTS ${Jucer2Reprojucer_EXE})
  message(FATAL_ERROR "No such file: ${Jucer2Reprojucer_EXE}")
endif()

get_filename_component(Jucer2Reprojucer_EXE "${Jucer2Reprojucer_EXE}" ABSOLUTE)


file(GLOB_RECURSE jucer_files RELATIVE "${JUCE_ROOT}" "${JUCE_ROOT}/*.jucer")

foreach(jucer_file IN LISTS jucer_files)
  get_filename_component(working_dir "${generated_JUCE_ROOT}/${jucer_file}" DIRECTORY)
  if(NOT IS_DIRECTORY "${working_dir}")
    message(FATAL_ERROR "Cannot change working directory to ${working_dir}")
  endif()

  execute_process(WORKING_DIRECTORY ${working_dir}
    COMMAND
    "${Jucer2Reprojucer_EXE}"
    "${JUCE_ROOT}/${jucer_file}"
    "${CMAKE_CURRENT_LIST_DIR}/../cmake/Reprojucer.cmake"
    "--relocatable"
    RESULT_VARIABLE result
  )

  if(NOT result EQUAL 0)
    message(FATAL_ERROR "Failed to run ${Jucer2Reprojucer_EXE} "
      "${JUCE_ROOT}/${jucer_file} ${CMAKE_CURRENT_LIST_DIR}/../cmake/Reprojucer.cmake"
    )
  endif()
endforeach()
