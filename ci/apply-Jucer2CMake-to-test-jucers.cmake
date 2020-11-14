# Copyright (C) 2018-2020  Alain Martin
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

set(tests_DIR "${CMAKE_CURRENT_LIST_DIR}/../tests")
if(NOT IS_DIRECTORY "${tests_DIR}")
  message(FATAL_ERROR "No such directory: ${tests_DIR}")
endif()


if(NOT DEFINED Jucer2CMake_EXE)
  message(FATAL_ERROR "Jucer2CMake_EXE must be defined")
endif()

if(NOT EXISTS ${Jucer2CMake_EXE})
  message(FATAL_ERROR "No such file: ${Jucer2CMake_EXE}")
endif()

get_filename_component(Jucer2CMake_EXE "${Jucer2CMake_EXE}" ABSOLUTE)


file(GLOB_RECURSE jucer_files "${tests_DIR}/*.jucer")

foreach(jucer_file IN LISTS jucer_files)
  get_filename_component(working_dir "${jucer_file}" DIRECTORY)

  execute_process(WORKING_DIRECTORY ${working_dir}
    COMMAND
    "${Jucer2CMake_EXE}"
    "reprojucer"
    "${jucer_file}"
    "${CMAKE_CURRENT_LIST_DIR}/../cmake/Reprojucer.cmake"
    RESULT_VARIABLE result
  )

  if(NOT result EQUAL 0)
    message(FATAL_ERROR "Failed to run ${Jucer2CMake_EXE} "
      "${jucer_file} ${CMAKE_CURRENT_LIST_DIR}/../cmake/Reprojucer.cmake"
    )
  endif()
endforeach()
