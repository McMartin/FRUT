# Copyright (C) 2019-2020  Alain Martin
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

if(NOT DEFINED CMAKE_CONFIGURATION_TYPES)
  if(NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    message(STATUS "Setting CMAKE_BUILD_TYPE to \"Debug\" as it was not specified.")
    set(CMAKE_BUILD_TYPE Debug)
  endif()
endif()


add_executable(XcassetsBuilder
  "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/Source/ProjectSaving/jucer_ProjectExporter.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/Source/Utility/jucer_FileHelpers.cpp"
)

if(APPLE)
  target_sources(XcassetsBuilder PRIVATE
    "${JUCE_modules_DIR}/juce_core/juce_core.mm"
    "${JUCE_modules_DIR}/juce_data_structures/juce_data_structures.mm"
    "${JUCE_modules_DIR}/juce_events/juce_events.mm"
    "${JUCE_modules_DIR}/juce_graphics/juce_graphics.mm"
    "${JUCE_modules_DIR}/juce_gui_basics/juce_gui_basics.mm"
  )
else()
  target_sources(XcassetsBuilder PRIVATE
    "${JUCE_modules_DIR}/juce_core/juce_core.cpp"
    "${JUCE_modules_DIR}/juce_data_structures/juce_data_structures.cpp"
    "${JUCE_modules_DIR}/juce_events/juce_events.cpp"
    "${JUCE_modules_DIR}/juce_graphics/juce_graphics.cpp"
    "${JUCE_modules_DIR}/juce_gui_basics/juce_gui_basics.cpp"
  )
endif()

target_include_directories(XcassetsBuilder PRIVATE "${JUCE_modules_DIR}")

target_compile_definitions(XcassetsBuilder PRIVATE
  $<$<CONFIG:Debug>:DEBUG=1>
  $<$<CONFIG:Debug>:_DEBUG=1>
  $<$<NOT:$<CONFIG:Debug>>:NDEBUG=1>
  JUCE_DISABLE_JUCE_VERSION_PRINTING=1
  JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1
  JUCE_MODULE_AVAILABLE_juce_core=1
  JUCE_MODULE_AVAILABLE_juce_data_structures=1
  JUCE_MODULE_AVAILABLE_juce_events=1
  JUCE_MODULE_AVAILABLE_juce_graphics=1
  JUCE_MODULE_AVAILABLE_juce_gui_basics=1
  JUCE_STANDALONE_APPLICATION=1
  JUCE_USE_CURL=0
)

set_target_properties(XcassetsBuilder PROPERTIES
  CXX_EXTENSIONS OFF
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED ON
  OUTPUT_NAME XcassetsBuilder-0.1.0
)

if(APPLE)
  target_compile_options(XcassetsBuilder PRIVATE "-Wno-deprecated-declarations")

  find_library(Cocoa_framework "Cocoa")
  find_library(IOKit_framework "IOKit")
  find_library(QuartzCore_framework "QuartzCore")

  target_link_libraries(XcassetsBuilder PRIVATE
    ${Cocoa_framework} ${IOKit_framework} ${QuartzCore_framework}
  )
endif()

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
  find_package(PkgConfig REQUIRED)

  function(use_package package)
    pkg_check_modules(${package} "${package}")
    if(NOT ${package}_FOUND)
      message(FATAL_ERROR "pkg-config could not find ${package}")
    endif()
    target_compile_options(XcassetsBuilder PRIVATE ${${package}_CFLAGS})
    target_link_libraries(XcassetsBuilder PRIVATE ${${package}_LIBRARIES})
  endfunction()

  use_package(freetype2)
  use_package(x11)
  use_package(xext)

  target_compile_options(XcassetsBuilder PRIVATE -pthread)
  target_link_libraries(XcassetsBuilder PRIVATE dl pthread)
endif()

if(WIN32 AND NOT MSVC)
  target_compile_options(XcassetsBuilder PRIVATE "-Wa,-mbig-obj")
  target_link_libraries(XcassetsBuilder PRIVATE
    -limm32 -lshlwapi -lversion -lwininet -lwinmm -lws2_32
  )
endif()


if(built_by_Reprojucer)
  install(TARGETS XcassetsBuilder DESTINATION ".")
else()
  install(TARGETS XcassetsBuilder DESTINATION "FRUT/cmake/bin")
endif()
