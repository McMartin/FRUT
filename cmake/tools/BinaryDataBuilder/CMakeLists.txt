# Copyright (C) 2016-2020  Alain Martin
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

add_executable(BinaryDataBuilder
  "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/extras/Projucer/Source/Project Saving/jucer_ResourceFile.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/extras/Projucer/Source/Utility/jucer_CodeHelpers.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/extras/Projucer/Source/Utility/jucer_FileHelpers.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/extras/Projucer/Source/Utility/jucer_MiscUtilities.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/modules/juce_gui_extra/juce_gui_extra.cpp"
)

set_target_properties(BinaryDataBuilder PROPERTIES OUTPUT_NAME BinaryDataBuilder-0.3.1)

target_link_libraries(BinaryDataBuilder PRIVATE tools_juce_core)


if(built_by_Reprojucer)
  install(TARGETS BinaryDataBuilder DESTINATION ".")
else()
  install(TARGETS BinaryDataBuilder DESTINATION "FRUT/cmake/bin")
endif()
