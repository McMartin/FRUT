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


project(JuceProjects-4.3.1)


add_subdirectory("../../Jucer2Reprojucer/generated/JUCE-4.3.1/examples/audio plugin host"
  "Plugin Host"
)

add_subdirectory("../../Jucer2Reprojucer/generated/JUCE-4.3.1/extras/Projucer"
  "Projucer"
)

if(NOT MSVC_VERSION EQUAL 1800)
add_subdirectory("../../Jucer2Reprojucer/generated/JUCE-4.3.1/examples/audio plugin demo"
  "JuceDemoPlugin"
)
endif()
