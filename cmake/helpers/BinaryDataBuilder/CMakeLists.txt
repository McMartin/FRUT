# Copyright (c) 2016 Alain Martin
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

add_executable(BinaryDataBuilder
  "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/Source/Project Saving/jucer_ResourceFile.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/Source/Utility/jucer_CodeHelpers.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/Source/Utility/jucer_FileHelpers.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/Source/Utility/jucer_MiscUtilities.cpp"
)

if(APPLE)
  target_sources(BinaryDataBuilder PRIVATE
    "${JUCE_modules_DIR}/juce_core/juce_core.mm"
    "${JUCE_modules_DIR}/juce_cryptography/juce_cryptography.mm"
    "${JUCE_modules_DIR}/juce_data_structures/juce_data_structures.mm"
    "${JUCE_modules_DIR}/juce_events/juce_events.mm"
    "${JUCE_modules_DIR}/juce_graphics/juce_graphics.mm"
    "${JUCE_modules_DIR}/juce_gui_basics/juce_gui_basics.mm"
    "${JUCE_modules_DIR}/juce_gui_extra/juce_gui_extra.mm"
  )
else()
  target_sources(BinaryDataBuilder PRIVATE
    "${JUCE_modules_DIR}/juce_core/juce_core.cpp"
    "${JUCE_modules_DIR}/juce_cryptography/juce_cryptography.cpp"
    "${JUCE_modules_DIR}/juce_data_structures/juce_data_structures.cpp"
    "${JUCE_modules_DIR}/juce_events/juce_events.cpp"
    "${JUCE_modules_DIR}/juce_graphics/juce_graphics.cpp"
    "${JUCE_modules_DIR}/juce_gui_basics/juce_gui_basics.cpp"
    "${JUCE_modules_DIR}/juce_gui_extra/juce_gui_extra.cpp"
  )
endif()


install(TARGETS BinaryDataBuilder DESTINATION ".")

add_custom_target(install-BinaryDataBuilder
  ${CMAKE_COMMAND}
  -DCMAKE_INSTALL_CONFIG_NAME=$<CONFIG>
  -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_install.cmake
  DEPENDS BinaryDataBuilder
)
