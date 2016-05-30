# Copyright (c) 2016 Alain Martin
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.


function(jucer_project_begin project_name)

  project(${project_name})
  set(JUCER_PROJECT_NAME ${project_name} PARENT_SCOPE)

endfunction()


function(jucer_project_files source_group_name)

  string(REPLACE "/" "\\" source_group_name ${source_group_name})
  source_group(${source_group_name} FILES ${ARGN})

  list(APPEND JUCER_PROJECT_SOURCES ${ARGN})
  set(JUCER_PROJECT_SOURCES ${JUCER_PROJECT_SOURCES} PARENT_SCOPE)

endfunction()


function(jucer_project_module module_name PATH_TAG module_path)

  list(APPEND JUCER_PROJECT_MODULES ${module_name})
  set(JUCER_PROJECT_MODULES ${JUCER_PROJECT_MODULES} PARENT_SCOPE)

  list(APPEND JUCER_PROJECT_INCLUDE_DIRS "${module_path}")
  set(JUCER_PROJECT_INCLUDE_DIRS ${JUCER_PROJECT_INCLUDE_DIRS} PARENT_SCOPE)

  set(module_header_file "${module_path}/${module_name}/${module_name}.h")

  file(STRINGS "${module_header_file}" osx_frameworks_line REGEX "OSXFrameworks:")
  string(REPLACE "OSXFrameworks:" "" osx_frameworks_line "${osx_frameworks_line}")
  string(REPLACE " " ";" osx_frameworks "${osx_frameworks_line}")
  list(APPEND JUCER_PROJECT_OSX_FRAMEWORKS ${osx_frameworks})
  set(JUCER_PROJECT_OSX_FRAMEWORKS ${JUCER_PROJECT_OSX_FRAMEWORKS} PARENT_SCOPE)

  file(GLOB_RECURSE browsable_files "${module_path}/${module_name}/*")
  foreach(file_path ${browsable_files})
    get_filename_component(file_dir "${file_path}" DIRECTORY)
    string(REPLACE "${module_path}" "" rel_file_dir "${file_dir}")
    string(REPLACE "/" "\\" sub_group_name "${rel_file_dir}")
    source_group("Juce Modules${sub_group_name}" FILES "${file_path}")
  endforeach()
  list(APPEND JUCER_PROJECT_BROWSABLE_FILES ${browsable_files})
  set(JUCER_PROJECT_BROWSABLE_FILES ${JUCER_PROJECT_BROWSABLE_FILES} PARENT_SCOPE)

endfunction()


function(jucer_project_end)

  foreach(module_name ${JUCER_PROJECT_MODULES})
    string(CONCAT module_available_defines
      "${module_available_defines}"
      "#define JUCE_MODULE_AVAILABLE_${module_name} 1\n"
    )
  endforeach()
  configure_file("${JUCE.cmake_ROOT}/cmake/AppConfig.h" "JuceLibraryCode/AppConfig.h")

  foreach(module_name ${JUCER_PROJECT_MODULES})
    string(CONCAT modules_includes
      "${modules_includes}"
      "#include <${module_name}/${module_name}.h>\n"
    )
  endforeach()
  configure_file("${JUCE.cmake_ROOT}/cmake/JuceHeader.h" "JuceLibraryCode/JuceHeader.h")

  foreach(module_name ${JUCER_PROJECT_MODULES})
    if(APPLE)
      set(extension "mm")
    else()
      set(extension "cpp")
    endif()
    configure_file("${JUCE.cmake_ROOT}/cmake/ModuleWrapper.cpp"
      "JuceLibraryCode/${module_name}.${extension}")
    list(APPEND modules_sources
      "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode/${module_name}.${extension}")
  endforeach()

  source_group("Juce Library Code" FILES
    "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode/AppConfig.h"
    "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode/JuceHeader.h"
    ${modules_sources}
  )

  add_executable(${JUCER_PROJECT_NAME} WIN32 MACOSX_BUNDLE
    ${JUCER_PROJECT_SOURCES}
    "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode/AppConfig.h"
    "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode/JuceHeader.h"
    ${modules_sources}
    ${JUCER_PROJECT_BROWSABLE_FILES}
  )

  set_source_files_properties(${JUCER_PROJECT_BROWSABLE_FILES}
    PROPERTIES HEADER_FILE_ONLY TRUE)

  target_include_directories(${JUCER_PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode"
    ${JUCER_PROJECT_INCLUDE_DIRS}
  )

  if(APPLE)
    target_compile_options(${JUCER_PROJECT_NAME} PRIVATE -std=c++11)
    target_compile_definitions(${JUCER_PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:_DEBUG>)

    list(REMOVE_DUPLICATES JUCER_PROJECT_OSX_FRAMEWORKS)
    foreach(framework_name ${JUCER_PROJECT_OSX_FRAMEWORKS})
      find_library(${framework_name}_framework ${framework_name})
      target_link_libraries(${JUCER_PROJECT_NAME}
        "${${framework_name}_framework}"
      )
    endforeach()
  endif()

endfunction()
