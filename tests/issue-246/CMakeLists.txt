# Copyright (C) 2017  Alain Martin
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


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../cmake")
include(Reprojucer)


jucer_project_begin()

jucer_project_settings(
  PROJECT_NAME "issue-246"
  PROJECT_VERSION "1.0.0"
  PROJECT_TYPE "Console Application"
)

jucer_project_files("issue-246"
  x . . "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
)

jucer_export_target(
  "Visual Studio 2015"
  TARGET_PROJECT_FOLDER "${CMAKE_CURRENT_LIST_DIR}"
)

jucer_export_target_configuration(
  "Visual Studio 2015"
  NAME "Debug"
  DEBUG_MODE ON
  PREBUILD_COMMAND "echo PREBUILD-VS2015-Debug"
  POSTBUILD_COMMAND "echo POSTBUILD-VS2015-Debug"
)

jucer_export_target_configuration(
  "Visual Studio 2015"
  NAME "Release"
  DEBUG_MODE OFF
  PREBUILD_COMMAND "echo PREBUILD-VS2015-Release"
  POSTBUILD_COMMAND "echo POSTBUILD-VS2015-Release"
)

jucer_export_target(
  "Visual Studio 2013"
  TARGET_PROJECT_FOLDER "${CMAKE_CURRENT_LIST_DIR}"
)

jucer_export_target_configuration(
  "Visual Studio 2013"
  NAME "Debug"
  DEBUG_MODE ON
  PREBUILD_COMMAND "echo PREBUILD-VS2013-Debug"
  POSTBUILD_COMMAND "echo POSTBUILD-VS2013-Debug"
)

jucer_export_target_configuration(
  "Visual Studio 2013"
  NAME "Release"
  DEBUG_MODE OFF
  PREBUILD_COMMAND "echo PREBUILD-VS2013-Release"
  POSTBUILD_COMMAND "echo POSTBUILD-VS2013-Release"
)

jucer_project_end()
