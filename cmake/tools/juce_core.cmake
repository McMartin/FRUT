# Copyright (C) 2017-2020, 2022  Alain Martin
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

add_library(tools_juce_core STATIC "")

if(APPLE)
  target_sources(tools_juce_core PRIVATE "${JUCE_modules_DIR}/juce_core/juce_core.mm")
else()
  target_sources(tools_juce_core PRIVATE "${JUCE_modules_DIR}/juce_core/juce_core.cpp")
endif()

target_include_directories(tools_juce_core PUBLIC "${JUCE_modules_DIR}")

target_compile_definitions(tools_juce_core PUBLIC
  $<$<CONFIG:Debug>:DEBUG=1>
  $<$<CONFIG:Debug>:_DEBUG=1>
  $<$<NOT:$<CONFIG:Debug>>:NDEBUG=1>
  JUCE_DISABLE_JUCE_VERSION_PRINTING=1
  JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1
  JUCE_MODULE_AVAILABLE_juce_core=1
  JUCE_STANDALONE_APPLICATION=1
  JUCE_USE_CURL=0
)

if(APPLE)
  target_compile_options(tools_juce_core PRIVATE
    -Wno-deprecated-declarations
    -Wno-register
  )

  find_library(Cocoa_framework "Cocoa")
  find_library(IOKit_framework "IOKit")

  target_link_libraries(tools_juce_core PUBLIC ${Cocoa_framework} ${IOKit_framework})
endif()

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
  target_compile_options(tools_juce_core PUBLIC -pthread)
  target_link_libraries(tools_juce_core PUBLIC dl pthread)
endif()

if(WIN32 AND NOT MSVC)
  target_compile_options(tools_juce_core PRIVATE -Wno-cpp)

  target_link_libraries(tools_juce_core PUBLIC
    -lshlwapi -lversion -lwininet -lwinmm -lws2_32
  )
endif()
