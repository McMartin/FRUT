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


function(jucer_project_files)

  list(APPEND JUCER_PROJECT_SOURCES ${ARGN})
  set(JUCER_PROJECT_SOURCES ${JUCER_PROJECT_SOURCES} PARENT_SCOPE)

endfunction()


function(jucer_project_module module_name PATH_TAG module_path)

  list(APPEND JUCER_PROJECT_MODULES ${module_name})
  set(JUCER_PROJECT_MODULES ${JUCER_PROJECT_MODULES} PARENT_SCOPE)

  list(APPEND JUCER_PROJECT_INCLUDE_DIRS "${module_path}")
  set(JUCER_PROJECT_INCLUDE_DIRS ${JUCER_PROJECT_INCLUDE_DIRS} PARENT_SCOPE)

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
    configure_file("${JUCE.cmake_ROOT}/cmake/ModuleWrapper.cpp"
      "JuceLibraryCode/${module_name}.cpp")
    list(APPEND modules_sources
      "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode/${module_name}.cpp")
  endforeach()

  add_executable(${JUCER_PROJECT_NAME} WIN32
    ${JUCER_PROJECT_SOURCES}
    ${modules_sources}
  )

  target_include_directories(${JUCER_PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode"
    ${JUCER_PROJECT_INCLUDE_DIRS}
  )

endfunction()
