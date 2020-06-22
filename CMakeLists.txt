# Copyright (C) 2018-2019  Alain Martin
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

project(FRUT)


add_subdirectory(Jucer2CMake)


set(JUCE_modules_DIRS "${JUCE_ROOT}/modules")

add_subdirectory(cmake/tools)

install(FILES "${CMAKE_CURRENT_LIST_DIR}/cmake/Reprojucer.cmake" DESTINATION "FRUT/cmake")

install(DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/cmake/data" DESTINATION "FRUT/cmake")


add_subdirectory(deprecated/Jucer2Reprojucer)
