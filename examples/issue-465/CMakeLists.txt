cmake_minimum_required(VERSION 3.4)


if(NOT DEFINED JUCE_ROOT)
  message(FATAL_ERROR "JUCE_ROOT must be defined")
endif()

get_filename_component(JUCE_ROOT
  "${JUCE_ROOT}" ABSOLUTE
  BASE_DIR "${CMAKE_BINARY_DIR}"
)

if(NOT IS_DIRECTORY "${JUCE_ROOT}")
  message(FATAL_ERROR "No such directory: ${JUCE_ROOT}")
endif()


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../cmake")
include(Reprojucer)

enable_testing()
find_package(GTest REQUIRED)

add_subdirectory(dspCore)
add_subdirectory(dspCoreTestHost)
add_subdirectory(guiCore)
add_subdirectory(guiCoreTestHost)
add_subdirectory(PlugIn)
