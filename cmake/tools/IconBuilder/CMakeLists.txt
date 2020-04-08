# Copyright (C) 2017-2020  Alain Martin
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

add_executable(IconBuilder
  "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/Source/Project Saving/jucer_ProjectExporter.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/Source/Utility/jucer_FileHelpers.cpp"
)

set_target_properties(IconBuilder PROPERTIES OUTPUT_NAME IconBuilder-0.2.0)

target_link_libraries(IconBuilder PRIVATE tools_juce_gui_basics)


if(built_by_Reprojucer)
  install(TARGETS IconBuilder DESTINATION ".")
else()
  install(TARGETS IconBuilder DESTINATION "FRUT/cmake/bin")
endif()
