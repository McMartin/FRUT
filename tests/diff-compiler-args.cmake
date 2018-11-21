# Copyright (c) 2018 Alain Martin
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


macro(parse_script_arguments)

  if(NOT DEFINED jucer_FILE)
    message(FATAL_ERROR "jucer_FILE must be defined")
  endif()
  if(NOT EXISTS ${jucer_FILE})
    message(FATAL_ERROR "No such .jucer file: ${jucer_FILE}")
  endif()
  get_filename_component(jucer_file "${jucer_FILE}" ABSOLUTE)
  get_filename_component(jucer_dir "${jucer_file}" DIRECTORY)
  get_filename_component(jucer_file_name "${jucer_file}" NAME)
  get_filename_component(jucer_file_name_we "${jucer_file_name}" NAME_WE)
  string(REGEX REPLACE "[^A-Za-z0-9]" "_" escaped_jucer_file_name "${jucer_file_name}")

  if(NOT DEFINED exporter)
    message(FATAL_ERROR "exporter must be defined")
  endif()
  if(exporter STREQUAL "LINUX_MAKE")
    set(build_folder "LinuxMakefile")
    set(cmake_generator "Unix Makefiles")
    set(projucer_build_command "@CMAKE_COMMAND@" "-E" "env" "CONFIG=@configuration@"
      "@cmake_make_program@" "-n"
    )
    set(reprojucer_build_command "@CMAKE_COMMAND@" "-E" "env" "VERBOSE=1"
      "@cmake_make_program@"
    )
    set(build_output_regex "\n([^\n]+-o[^\n]+-c[^\n]+foo.cpp[^\n]*)\n")
  elseif(exporter MATCHES "^VS201(3|5|7)$")
    if(exporter STREQUAL "VS2013")
      set(build_folder "VisualStudio2013")
      set(cmake_generator "Visual Studio 12 2013")
    elseif(exporter STREQUAL "VS2015")
      set(build_folder "VisualStudio2015")
      set(cmake_generator "Visual Studio 14 2015")
    elseif(exporter STREQUAL "VS2017")
      set(build_folder "VisualStudio2017")
      set(cmake_generator "Visual Studio 15 2017")
    else()
      message(FATAL_ERROR "There is a bug!")
    endif()
    set(projucer_build_command "@cmake_make_program@" "/v:minimal" "/clp:ShowCommandLine"
      "/property:Configuration=@configuration@" "@jucer_file_name_we@.sln"
    )
    set(reprojucer_build_command "${projucer_build_command}")
    set(build_output_regex "\n  ([^\n]+CL.exe[^\n]+)\n")
  elseif(exporter STREQUAL "XCODE_MAC")
    set(build_folder "MacOSX")
    set(cmake_generator "Xcode")
    set(projucer_build_command "@cmake_make_program@" "-configuration" "@configuration@")
    set(reprojucer_build_command "${projucer_build_command}")
    set(build_output_regex "\n[ ]+([^\n]+-c [^\n]+foo.cpp -o [^\n]+foo.o)\n")
  else()
    message(FATAL_ERROR "Unsupported Projucer exporter: \"${exporter}\"")
  endif()

  if(NOT DEFINED configuration)
    set(configuration "Debug")
  endif()

  message(STATUS ".jucer file: ${jucer_file}")
  message(STATUS "Projucer exporter: ${exporter}")
  message(STATUS "build configuration: ${configuration}")

endmacro()


macro(query_cmake_make_program)

  message(STATUS "Query CMAKE_MAKE_PROGRAM")

  set(cmake_make_program_build_dir
    "${CMAKE_CURRENT_LIST_DIR}/test-utils/cmake_make_program/build/${build_folder}"
  )
  if(NOT IS_DIRECTORY "${cmake_make_program_build_dir}")
    file(MAKE_DIRECTORY "${cmake_make_program_build_dir}")
  endif()

  execute_process(
    COMMAND "${CMAKE_COMMAND}" "../.." "-G" "${cmake_generator}"
    WORKING_DIRECTORY "${cmake_make_program_build_dir}"
    OUTPUT_VARIABLE configure_output
  )

  string(REGEX MATCH "-- CMAKE_MAKE_PROGRAM: ([^\n]+)\n" m "${configure_output}")
  set(cmake_make_program "${CMAKE_MATCH_1}")
  if(NOT cmake_make_program)
    message(FATAL_ERROR "Failed to query CMAKE_MAKE_PROGRAM")
  endif()
  message(STATUS "CMAKE_MAKE_PROGRAM: ${cmake_make_program}")

endmacro()


macro(generate_reprojucer_build_system)

  message(STATUS "Generate build system with Reprojucer")

  set(reprojucer_build_dir "${jucer_dir}/build/${build_folder}")
  if(NOT IS_DIRECTORY "${reprojucer_build_dir}")
    file(MAKE_DIRECTORY "${reprojucer_build_dir}")
  endif()

  execute_process(
    COMMAND "${CMAKE_COMMAND}" "../.." "-G" "${cmake_generator}"
    "-DCMAKE_BUILD_TYPE=${configuration}"
    "-D${escaped_jucer_file_name}_FILE=${jucer_file}"
    WORKING_DIRECTORY "${reprojucer_build_dir}"
    RESULT_VARIABLE cmake_result
  )
  if(NOT cmake_result EQUAL 0)
    message(FATAL_ERROR "")
  endif()

endmacro()


macro(touch_file_to_compile)

  execute_process(
    COMMAND "${CMAKE_COMMAND}" "-E" "touch_nocreate" "${jucer_dir}/Source/foo.cpp"
  )

endmacro()


macro(do_build)

  execute_process(
    COMMAND ${build_command}
    WORKING_DIRECTORY "${build_working_dir}"
    OUTPUT_VARIABLE build_output
    RESULT_VARIABLE build_result
  )
  if(NOT build_result EQUAL 0)
    message("${build_output}")
    message(FATAL_ERROR "")
  endif()

  string(REGEX MATCH "${build_output_regex}" m "${build_output}")
  set(compiler_cmd "${CMAKE_MATCH_1}")

endmacro()


macro(build_with_projucer_build_system)

  message(STATUS "Build with the build system generated by Projucer")

  string(CONFIGURE "${projucer_build_command}" build_command @ONLY)
  set(build_working_dir "${jucer_dir}/Builds/${build_folder}")
  do_build()
  set(projucer_compiler_cmd "${compiler_cmd}")

  if(NOT projucer_compiler_cmd)
    message(FATAL_ERROR "Failed to extract Projucer's compiler command")
  endif()

endmacro()


macro(build_with_reprojucer_build_system)

  message(STATUS "Build with the build system generated by Reprojucer")

  string(CONFIGURE "${reprojucer_build_command}" build_command @ONLY)
  set(build_working_dir "${reprojucer_build_dir}")
  do_build()
  set(reprojucer_compiler_cmd "${compiler_cmd}")

  if(NOT reprojucer_compiler_cmd)
    message(FATAL_ERROR "Failed to extract Reprojucer's compiler command")
  endif()

endmacro()


macro(diff_compiler_arguments)

  message(STATUS "Diff compiler arguments (old: Projucer, new: Reprojucer)")

  if(WIN32)
    separate_arguments(projucer_compiler_args WINDOWS_COMMAND "${projucer_compiler_cmd}")
    separate_arguments(reprojucer_compiler_args WINDOWS_COMMAND
      "${reprojucer_compiler_cmd}"
    )
  else()
    separate_arguments(projucer_compiler_args UNIX_COMMAND "${projucer_compiler_cmd}")
    separate_arguments(reprojucer_compiler_args UNIX_COMMAND "${reprojucer_compiler_cmd}")
  endif()

  include("${CMAKE_CURRENT_LIST_DIR}/test-utils/simplediff/simplediff.cmake")
  diff(projucer_compiler_args reprojucer_compiler_args args_diff)
  print_diff(args_diff)

endmacro()


macro(main)

  parse_script_arguments()
  query_cmake_make_program()
  generate_reprojucer_build_system()
  touch_file_to_compile()
  build_with_projucer_build_system()
  build_with_reprojucer_build_system()
  diff_compiler_arguments()

endmacro()


if(CMAKE_SCRIPT_MODE_FILE STREQUAL CMAKE_CURRENT_LIST_FILE)
  main()
endif()
