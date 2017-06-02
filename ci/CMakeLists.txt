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

cmake_minimum_required(VERSION 3.4)


project(ci)


add_subdirectory("../Jucer2Reprojucer"
  "Jucer2Reprojucer"
)

if(NOT CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
add_subdirectory("../Jucer2Reprojucer/generated/JUCE/examples/audio plugin host"
  "Plugin Host"
)
endif()

add_subdirectory("../Jucer2Reprojucer/generated/JUCE/examples/NetworkGraphicsDemo"
  "NetworkGraphicsDemo"
)

add_subdirectory("../Jucer2Reprojucer/generated/JUCE/extras/Projucer"
  "Projucer"
)

if(NOT CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
add_subdirectory("../Jucer2Reprojucer/generated/JUCE/examples/audio plugin demo"
  "JuceDemoPlugin"
)
endif()
