# Copyright (C) 2021  Alain Martin
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

if(NOT DEFINED Jucer2CMake_EXE)
  message(FATAL_ERROR "Jucer2CMake_EXE must be defined")
endif()

if(NOT EXISTS ${Jucer2CMake_EXE})
  message(FATAL_ERROR "No such file: ${Jucer2CMake_EXE}")
endif()

get_filename_component(Jucer2CMake_EXE "${Jucer2CMake_EXE}" ABSOLUTE)


file(GLOB_RECURSE jucer_files "${CMAKE_CURRENT_LIST_DIR}/*.jucer")

foreach(jucer_file IN LISTS jucer_files)
  get_filename_component(working_dir "${jucer_file}" DIRECTORY)

  set(command "${Jucer2CMake_EXE}" "juce6" "${jucer_file}")
  execute_process(COMMAND ${command}
    WORKING_DIRECTORY "${working_dir}"
    RESULT_VARIABLE result
  )

  if(NOT result EQUAL 0)
    string(REPLACE ";" " " command_string "${command}")
    message(FATAL_ERROR "Failed to run `${command_string}`")
  endif()
endforeach()
