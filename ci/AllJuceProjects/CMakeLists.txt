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

cmake_minimum_required(VERSION 3.4)


if(NOT DEFINED JUCE_VERSION)
  message(FATAL_ERROR "JUCE_VERSION must be defined")
endif()


if(JUCE_VERSION STREQUAL "4.2.0")
  set(JUCER_VST_SDK_FOLDER
    "${CMAKE_CURRENT_LIST_DIR}/../third-party/vstsdk365_12_11_2015_build_67/VST3 SDK"
  )

  if(APPLE OR WIN32)
    set(JUCER_VST3_SDK_FOLDER
      "${CMAKE_CURRENT_LIST_DIR}/../third-party/vstsdk365_12_11_2015_build_67/VST3 SDK"
    )
  endif()
else()
  if(APPLE OR WIN32)
    set(JUCER_VST3_SDK_FOLDER
      "${CMAKE_CURRENT_LIST_DIR}/../third-party/vstsdk366_27_06_2016_build_61/VST3 SDK"
    )
  endif()
endif()


set(JUCE_ROOT "${CMAKE_CURRENT_LIST_DIR}/../tmp/JUCE-${JUCE_VERSION}")


add_subdirectory(
  "${CMAKE_CURRENT_LIST_DIR}/../../Jucer2Reprojucer/generated/JUCE-${JUCE_VERSION}"
  "JUCE-${JUCE_VERSION}"
)
