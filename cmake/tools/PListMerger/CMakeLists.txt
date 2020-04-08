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

add_executable(PListMerger "${CMAKE_CURRENT_LIST_DIR}/main.cpp")

set_target_properties(PListMerger PROPERTIES OUTPUT_NAME PListMerger-0.1.0)

target_link_libraries(PListMerger PRIVATE tools_juce_core)


if(built_by_Reprojucer)
  install(TARGETS PListMerger DESTINATION ".")
else()
  install(TARGETS PListMerger DESTINATION "FRUT/cmake/bin")
endif()
