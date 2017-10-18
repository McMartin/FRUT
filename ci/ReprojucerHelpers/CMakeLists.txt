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


project(ReprojucerHelpers)


add_subdirectory("../../cmake/BinaryDataBuilder"
  "BinaryDataBuilder"
)

add_subdirectory("../../cmake/IconBuilder"
  "IconBuilder"
)

add_subdirectory("../../cmake/PListMerger"
  "PListMerger"
)

add_subdirectory("../../Jucer2Reprojucer"
  "Jucer2Reprojucer"
)
