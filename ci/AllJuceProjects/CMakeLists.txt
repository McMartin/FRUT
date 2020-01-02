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


if(NOT DEFINED JUCE_VERSION)
  message(FATAL_ERROR "JUCE_VERSION must be defined")
endif()


set(JUCE_ROOT "${CMAKE_CURRENT_LIST_DIR}/../tmp/JUCE-${JUCE_VERSION}")
if(NOT IS_DIRECTORY "${JUCE_ROOT}")
  message(FATAL_ERROR "No such directory: ${JUCE_ROOT}")
endif()


project(AllJuceProjects)

add_subdirectory(
  "${CMAKE_CURRENT_LIST_DIR}/../../generated/JUCE-${JUCE_VERSION}"
  "JUCE-${JUCE_VERSION}"
)
