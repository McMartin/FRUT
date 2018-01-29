# Copyright (c) 2016 Alain Martin
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

if(CMAKE_VERSION VERSION_LESS 3.4)
  message(FATAL_ERROR "Reprojucer requires at least CMake version 3.4")
endif()

if(CMAKE_VERSION VERSION_LESS 3.5)
  include(CMakeParseArguments)
endif()


set(Reprojucer.cmake_DIR "${CMAKE_CURRENT_LIST_DIR}")
set(Reprojucer_templates_DIR "${Reprojucer.cmake_DIR}/templates")

set(Reprojucer_supported_exporters
  "Xcode (MacOSX)"
  "Visual Studio 2017"
  "Visual Studio 2015"
  "Visual Studio 2013"
  "Linux Makefile"
)
set(Reprojucer_supported_exporters_conditions
  "APPLE"
  "MSVC_VERSION\;GREATER\;1909"
  "MSVC_VERSION\;EQUAL\;1900"
  "MSVC_VERSION\;EQUAL\;1800"
  "CMAKE_HOST_SYSTEM_NAME\;STREQUAL\;Linux"
)


function(_FRUT_set_Reprojucer_current_exporter)

  unset(current_exporter)

  foreach(exporter_index RANGE 4)
    list(GET Reprojucer_supported_exporters_conditions ${exporter_index} condition)
    if(${condition})
      if(DEFINED current_exporter)
        message(FATAL_ERROR "There is already a current exporter: ${current_exporter}")
      else()
        list(GET Reprojucer_supported_exporters ${exporter_index} exporter)
        set(current_exporter ${exporter})
      endif()
    endif()
  endforeach()

  if(NOT DEFINED current_exporter)
    message(FATAL_ERROR "Reprojucer.cmake doesn't support any export target for your "
      "current platform. It supports the following export targets: "
      "${Reprojucer_supported_exporters}. If you think Reprojucer.cmake should support "
      "another export target, please create an issue on GitHub: "
      "https://github.com/McMartin/FRUT/issues/new"
    )
  endif()

  set(Reprojucer_current_exporter ${current_exporter} PARENT_SCOPE)

endfunction()

_FRUT_set_Reprojucer_current_exporter()


function(jucer_project_begin)

  cmake_parse_arguments(arg "" "JUCER_VERSION;PROJECT_FILE;PROJECT_ID" "" ${ARGN})
  if(NOT "${arg_UNPARSED_ARGUMENTS}" STREQUAL "")
    message(FATAL_ERROR "Unknown arguments: ${arg_UNPARSED_ARGUMENTS}")
  endif()

  if(NOT "${arg_JUCER_VERSION}" STREQUAL "")
    set(JUCER_VERSION "${arg_JUCER_VERSION}" PARENT_SCOPE)
  endif()

  if(NOT "${arg_PROJECT_FILE}" STREQUAL "")
    if(NOT EXISTS "${arg_PROJECT_FILE}")
      message(FATAL_ERROR "No such JUCE project file: ${arg_PROJECT_FILE}")
    endif()
    set(JUCER_PROJECT_FILE "${arg_PROJECT_FILE}" PARENT_SCOPE)

    get_filename_component(project_dir "${arg_PROJECT_FILE}" DIRECTORY)
    set(JUCER_PROJECT_DIR "${project_dir}" PARENT_SCOPE)
  endif()

  if(NOT "${arg_PROJECT_ID}" STREQUAL "")
    set(JUCER_PROJECT_ID "${arg_PROJECT_ID}" PARENT_SCOPE)
  endif()

endfunction()


function(jucer_project_settings)

  if(NOT "PROJECT_NAME" IN_LIST ARGN)
    message(FATAL_ERROR "Missing PROJECT_NAME argument")
  endif()

  if(NOT "PROJECT_TYPE" IN_LIST ARGN)
    message(FATAL_ERROR "Missing PROJECT_TYPE argument")
  endif()

  set(project_setting_tags
    "PROJECT_NAME"
    "PROJECT_VERSION"
    "COMPANY_NAME"
    "COMPANY_COPYRIGHT"
    "COMPANY_WEBSITE"
    "COMPANY_EMAIL"
    "REPORT_JUCE_APP_USAGE"
    "DISPLAY_THE_JUCE_SPLASH_SCREEN"
    "SPLASH_SCREEN_COLOUR"
    "PROJECT_TYPE"
    "BUNDLE_IDENTIFIER"
    "BINARYDATACPP_SIZE_LIMIT"
    "BINARYDATA_NAMESPACE"
    "PREPROCESSOR_DEFINITIONS"
    "HEADER_SEARCH_PATHS"
  )

  set(project_types "GUI Application" "Console Application" "Static Library"
    "Dynamic Library" "Audio Plug-in"
  )

  set(size_limit_descs "Default" "20.0 MB" "10.0 MB" "6.0 MB" "2.0 MB" "1.0 MB" "512.0 KB"
    "256.0 KB" "128.0 KB" "64.0 KB"
  )
  set(size_limits 10240 20480 10240 6144 2048 1024 512 256 128 64)

  unset(tag)
  foreach(element ${ARGN})
    if(NOT DEFINED tag)
      set(tag ${element})

      if(NOT "${tag}" IN_LIST project_setting_tags)
        message(FATAL_ERROR "Unsupported project setting: ${tag}\n"
          "Supported project settings: ${project_setting_tags}"
        )
      endif()
    else()
      set(value ${element})

      if(tag STREQUAL "PROJECT_VERSION")
        string(REGEX MATCH ".+\\..+\\..+(\\..+)?" version_match "${value}")
        if(NOT value STREQUAL version_match)
          message(WARNING
            "The PROJECT_VERSION doesn't seem to be in the format "
            "major.minor.point[.point]"
          )
        endif()
        _FRUT_version_to_hex("${value}" hex_value)
        set(JUCER_PROJECT_VERSION_AS_HEX "${hex_value}" PARENT_SCOPE)

      elseif(tag STREQUAL "PROJECT_TYPE")
        if(NOT "${value}" IN_LIST project_types)
          message(FATAL_ERROR "Unsupported project type: \"${value}\"\n"
            "Supported project types: ${project_types}"
          )
        endif()

      elseif(tag STREQUAL "BINARYDATACPP_SIZE_LIMIT")
        list(FIND size_limit_descs "${value}" size_limit_index)
        if(size_limit_index EQUAL -1)
          message(FATAL_ERROR
            "Unsupported value for BINARYDATACPP_SIZE_LIMIT: \"${value}\"\n"
            "Supported values: ${size_limit_descs}"
          )
        endif()
        list(GET size_limits ${size_limit_index} value)

      elseif(tag STREQUAL "PREPROCESSOR_DEFINITIONS")
        string(REPLACE "\n" ";" value "${value}")

      elseif(tag STREQUAL "HEADER_SEARCH_PATHS")
        string(REPLACE "\\" "/" value "${value}")
        string(REPLACE "\n" ";" value "${value}")
        unset(header_search_paths)
        foreach(path ${value})
          _FRUT_abs_path_based_on_jucer_project_dir("${path}" path)
          list(APPEND header_search_paths "${path}")
        endforeach()
        set(value ${header_search_paths})

      endif()

      set(JUCER_${tag} "${value}" PARENT_SCOPE)

      unset(tag)
    endif()
  endforeach()

endfunction()


function(jucer_audio_plugin_settings)

  set(plugin_setting_tags
    "BUILD_VST"
    "BUILD_VST3"
    "BUILD_AUDIOUNIT"
    "BUILD_AUDIOUNIT_V3"
    "BUILD_RTAS"
    "BUILD_AAX"
    "BUILD_STANDALONE_PLUGIN"
    "PLUGIN_NAME"
    "PLUGIN_DESCRIPTION"
    "PLUGIN_MANUFACTURER"
    "PLUGIN_MANUFACTURER_CODE"
    "PLUGIN_CODE"
    "PLUGIN_CHANNEL_CONFIGURATIONS"
    "PLUGIN_IS_A_SYNTH"
    "PLUGIN_MIDI_INPUT"
    "PLUGIN_MIDI_OUTPUT"
    "MIDI_EFFECT_PLUGIN"
    "KEY_FOCUS"
    "PLUGIN_AU_EXPORT_PREFIX"
    "PLUGIN_AU_MAIN_TYPE"
    "VST_CATEGORY"
    "PLUGIN_RTAS_CATEGORY"
    "PLUGIN_AAX_CATEGORY"
    "PLUGIN_AAX_IDENTIFIER"
  )

  unset(tag)
  foreach(element ${ARGN})
    if(NOT DEFINED tag)
      set(tag ${element})
    else()
      set(value ${element})

      if(NOT "${tag}" IN_LIST plugin_setting_tags)
        message(FATAL_ERROR "Unsupported audio plugin setting: ${tag}\n"
          "Supported audio plugin settings: ${plugin_setting_tags}"
        )

      elseif(tag STREQUAL "BUILD_RTAS" AND value AND (APPLE OR MSVC))
        message(WARNING "Reprojucer.cmake doesn't support building RTAS plugins. If you "
          "would like Reprojucer.cmake to support building RTAS plugins, please leave a "
          "comment on the issue \"Reprojucer.cmake doesn't support building RTAS "
          "plugins\" on GitHub: https://github.com/McMartin/FRUT/issues/266"
        )

      elseif(tag STREQUAL "BUILD_AAX" AND value AND (APPLE OR MSVC))
        message(WARNING "Reprojucer.cmake doesn't support building AAX plugins. If you "
          "would like Reprojucer.cmake to support building AAX plugins, please leave a "
          "comment on the issue \"Reprojucer.cmake doesn't support building AAX "
          "plugins\" on GitHub: https://github.com/McMartin/FRUT/issues/267"
        )

      elseif(tag STREQUAL "BUILD_STANDALONE_PLUGIN" AND DEFINED JUCER_VERSION
          AND JUCER_VERSION VERSION_LESS 5)
        message(WARNING "BUILD_STANDALONE_PLUGIN is a JUCE 5 feature only")

      endif()

      set(JUCER_${tag} "${value}" PARENT_SCOPE)

      unset(tag)
    endif()
  endforeach()

endfunction()


function(jucer_project_files source_group_name)

  function(__check_input input)
    if(NOT input STREQUAL "x" AND NOT input STREQUAL ".")
      message(FATAL_ERROR "Expected x or . token, got ${input} instead")
    endif()
  endfunction()

  unset(compile)
  unset(xcode_resource)
  unset(binary_resource)
  unset(files)
  foreach(element ${ARGN})
    if(NOT DEFINED compile)
      set(compile ${element})
      __check_input("${compile}")
    elseif(NOT DEFINED xcode_resource)
      set(xcode_resource ${element})
      __check_input("${xcode_resource}")
    elseif(NOT DEFINED binary_resource)
      set(binary_resource ${element})
      __check_input("${binary_resource}")
    else()
      set(path ${element})

      _FRUT_abs_path_based_on_jucer_project_dir("${path}" path)
      list(APPEND files "${path}")

      if(xcode_resource STREQUAL "x")
        list(APPEND JUCER_PROJECT_XCODE_RESOURCES "${path}")
      elseif(binary_resource STREQUAL "x")
        list(APPEND JUCER_PROJECT_RESOURCES "${path}")
      else()
        list(APPEND JUCER_PROJECT_SOURCES "${path}")

        get_filename_component(file_extension "${path}" EXT)

        if(NOT file_extension STREQUAL ".h" AND compile STREQUAL ".")
          set_source_files_properties("${path}" PROPERTIES HEADER_FILE_ONLY TRUE)
        endif()

        if(file_extension STREQUAL ".mm" AND NOT APPLE)
          set_source_files_properties("${path}" PROPERTIES HEADER_FILE_ONLY TRUE)
        endif()
      endif()

      unset(compile)
      unset(xcode_resource)
      unset(binary_resource)
    endif()
  endforeach()

  string(REPLACE "/" "\\" source_group_name ${source_group_name})
  source_group(${source_group_name} FILES ${files})

  set(JUCER_PROJECT_SOURCES ${JUCER_PROJECT_SOURCES} PARENT_SCOPE)
  set(JUCER_PROJECT_RESOURCES ${JUCER_PROJECT_RESOURCES} PARENT_SCOPE)
  set(JUCER_PROJECT_XCODE_RESOURCES ${JUCER_PROJECT_XCODE_RESOURCES} PARENT_SCOPE)

endfunction()


function(jucer_project_module module_name PATH_TAG modules_folder)

  list(APPEND JUCER_PROJECT_MODULES ${module_name})
  set(JUCER_PROJECT_MODULES ${JUCER_PROJECT_MODULES} PARENT_SCOPE)

  _FRUT_abs_path_based_on_jucer_project_dir("${modules_folder}" modules_folder)
  if(NOT IS_DIRECTORY "${modules_folder}")
    message(FATAL_ERROR "No such directory: \"${modules_folder}\"")
  endif()
  list(APPEND JUCER_PROJECT_MODULES_FOLDERS "${modules_folder}")
  set(JUCER_PROJECT_MODULES_FOLDERS ${JUCER_PROJECT_MODULES_FOLDERS} PARENT_SCOPE)

  file(GLOB module_src_files
    "${modules_folder}/${module_name}/*.cpp"
    "${modules_folder}/${module_name}/*.mm"
  )

  if(DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5)
    set(proxy_prefix)
  else()
    set(proxy_prefix "include_")
  endif()

  foreach(src_file ${module_src_files})
    unset(to_compile)

    if(  (src_file MATCHES "_AU[._]"   AND NOT (JUCER_BUILD_AUDIOUNIT    AND APPLE))
      OR (src_file MATCHES "_AUv3[._]" AND NOT (JUCER_BUILD_AUDIOUNIT_V3 AND APPLE))
      OR (src_file MATCHES "_AAX[._]"  AND NOT (JUCER_BUILD_AAX          AND (APPLE OR MSVC)))
      OR (src_file MATCHES "_RTAS[._]" AND NOT (JUCER_BUILD_RTAS         AND (APPLE OR MSVC)))
      OR (src_file MATCHES "_VST2[._]" AND NOT (JUCER_BUILD_VST          AND TRUE))
      OR (src_file MATCHES "_VST3[._]" AND NOT (JUCER_BUILD_VST3         AND (APPLE OR MSVC)))
    )
      set(to_compile FALSE)
    endif()

    if(DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5)
      if(src_file MATCHES "_Standalone[._]" AND NOT (JUCER_BUILD_AUDIOUNIT_V3 AND APPLE))
        set(to_compile FALSE)
      endif()
    else()
      if(src_file MATCHES "_Standalone[._]" AND NOT JUCER_BUILD_STANDALONE_PLUGIN)
        set(to_compile FALSE)
      endif()
    endif()

    if(NOT DEFINED to_compile)
      get_filename_component(src_file_extension "${src_file}" EXT)
      if(src_file_extension STREQUAL ".mm")
        if(APPLE)
          set(to_compile TRUE)
        endif()
      elseif(APPLE)
        string(REGEX REPLACE "${src_file_extension}$" ".mm" objcxx_src_file "${src_file}")
        if(NOT "${objcxx_src_file}" IN_LIST module_src_files)
          set(to_compile TRUE)
        endif()
      else()
        set(to_compile TRUE)
      endif()
    endif()

    if(to_compile)
      get_filename_component(src_file_basename "${src_file}" NAME)
      configure_file("${Reprojucer_templates_DIR}/JuceLibraryCode-Wrapper.cpp"
        "JuceLibraryCode/${proxy_prefix}${src_file_basename}"
      )
      list(APPEND JUCER_PROJECT_SOURCES
        "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode/${proxy_prefix}${src_file_basename}"
      )
    endif()
  endforeach()

  set(JUCER_PROJECT_SOURCES ${JUCER_PROJECT_SOURCES} PARENT_SCOPE)

  set(module_header_file "${modules_folder}/${module_name}/${module_name}.h")

  file(STRINGS "${module_header_file}" config_flags_lines REGEX "/\\*\\* Config: ")
  string(REPLACE "/** Config: " "" module_config_flags "${config_flags_lines}")
  set(JUCER_${module_name}_CONFIG_FLAGS ${module_config_flags} PARENT_SCOPE)

  unset(config_flag)
  foreach(element ${ARGN})
    if(NOT DEFINED config_flag)
      set(config_flag ${element})

      if(NOT "${config_flag}" IN_LIST module_config_flags)
        message(WARNING "Unknown config flag ${config_flag} in module ${module_name}")
      endif()
    else()
      set(JUCER_FLAG_${config_flag} ${element} PARENT_SCOPE)
      unset(config_flag)
    endif()
  endforeach()

  if(APPLE)
    file(STRINGS "${module_header_file}" osx_frameworks_line REGEX "OSXFrameworks:")
    string(REPLACE "OSXFrameworks:" "" osx_frameworks_line "${osx_frameworks_line}")
    string(REPLACE " " ";" osx_frameworks "${osx_frameworks_line}")
    string(REPLACE "," ";" osx_frameworks "${osx_frameworks}")
    list(APPEND JUCER_PROJECT_OSX_FRAMEWORKS ${osx_frameworks})
    set(JUCER_PROJECT_OSX_FRAMEWORKS ${JUCER_PROJECT_OSX_FRAMEWORKS} PARENT_SCOPE)
  endif()

  if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    file(STRINGS "${module_header_file}" linux_libs_line REGEX "linuxLibs:")
    string(REPLACE "linuxLibs:" "" linux_libs_line "${linux_libs_line}")
    string(REPLACE " " ";" linux_libs "${linux_libs_line}")
    string(REPLACE "," ";" linux_libs "${linux_libs}")
    list(APPEND JUCER_PROJECT_LINUX_LIBS ${linux_libs})
    set(JUCER_PROJECT_LINUX_LIBS ${JUCER_PROJECT_LINUX_LIBS} PARENT_SCOPE)

    file(STRINGS "${module_header_file}" linux_packages_line REGEX "linuxPackages:")
    string(REPLACE "linuxPackages:" "" linux_packages_line "${linux_packages_line}")
    string(REPLACE " " ";" linux_packages "${linux_packages_line}")
    string(REPLACE "," ";" linux_packages "${linux_packages}")
    list(APPEND JUCER_PROJECT_LINUX_PACKAGES ${linux_packages})
    set(JUCER_PROJECT_LINUX_PACKAGES ${JUCER_PROJECT_LINUX_PACKAGES} PARENT_SCOPE)
  endif()

  file(GLOB_RECURSE browsable_files "${modules_folder}/${module_name}/*")
  foreach(file_path ${browsable_files})
    get_filename_component(file_dir "${file_path}" DIRECTORY)
    string(REPLACE "${modules_folder}" "" rel_file_dir "${file_dir}")
    string(REPLACE "/" "\\" sub_group_name "${rel_file_dir}")
    source_group("Juce Modules${sub_group_name}" FILES "${file_path}")
  endforeach()
  list(APPEND JUCER_PROJECT_BROWSABLE_FILES ${browsable_files})
  set(JUCER_PROJECT_BROWSABLE_FILES ${JUCER_PROJECT_BROWSABLE_FILES} PARENT_SCOPE)

endfunction()


function(jucer_appconfig_header USER_CODE_SECTION_TAG user_code_section)

  if(NOT USER_CODE_SECTION_TAG STREQUAL "USER_CODE_SECTION")
    message(FATAL_ERROR "Invalid second argument. Expected \"USER_CODE_SECTION\", but "
      "got \"${USER_CODE_SECTION}\" instead."
    )
  endif()

  set(JUCER_APPCONFIG_USER_CODE_SECTION "${user_code_section}" PARENT_SCOPE)

endfunction()


function(jucer_export_target exporter)

  if(NOT "${exporter}" IN_LIST Reprojucer_supported_exporters)
    message(FATAL_ERROR "Unsupported exporter: ${exporter}\n"
      "Supported exporters: ${Reprojucer_supported_exporters}"
    )
  endif()
  list(APPEND JUCER_PROJECT_EXPORT_TARGETS "${exporter}")
  set(JUCER_PROJECT_EXPORT_TARGETS ${JUCER_PROJECT_EXPORT_TARGETS} PARENT_SCOPE)

  list(FIND Reprojucer_supported_exporters "${exporter}" exporter_index)
  list(GET Reprojucer_supported_exporters_conditions ${exporter_index} condition)
  if(NOT ${condition})
    return()
  endif()

  set(export_target_settings_tags
    "TARGET_PROJECT_FOLDER"
    "VST_SDK_FOLDER"
    "EXTRA_PREPROCESSOR_DEFINITIONS"
    "EXTRA_COMPILER_FLAGS"
    "EXTRA_LINKER_FLAGS"
    "EXTERNAL_LIBRARIES_TO_LINK"
    "ICON_SMALL"
    "ICON_LARGE"
  )

  if(exporter STREQUAL "Xcode (MacOSX)")
    list(APPEND export_target_settings_tags
      "VST3_SDK_FOLDER"
      "CUSTOM_XCODE_RESOURCE_FOLDERS"
      "EXTRA_FRAMEWORKS"
      "CUSTOM_PLIST"
      "PREBUILD_SHELL_SCRIPT"
      "POSTBUILD_SHELL_SCRIPT"
      "DEVELOPMENT_TEAM_ID"
    )

    if(JUCER_PROJECT_TYPE STREQUAL "GUI Application")
      list(APPEND export_target_settings_tags
        "DOCUMENT_FILE_EXTENSIONS"
      )
    endif()
  endif()

  if(exporter MATCHES "^Visual Studio 201(7|5|3)$")
    list(APPEND export_target_settings_tags
      "VST3_SDK_FOLDER"
      "PLATFORM_TOOLSET"
      "USE_IPP_LIBRARY"
    )

    if(exporter STREQUAL "Visual Studio 2017")
      list(APPEND export_target_settings_tags "CXX_STANDARD_TO_USE")
    endif()
  endif()

  if(exporter STREQUAL "Linux Makefile")
    list(APPEND export_target_settings_tags
      "CXX_STANDARD_TO_USE"
      "PKGCONFIG_LIBRARIES"
    )
  endif()

  unset(tag)
  foreach(element ${ARGN})
    if(NOT DEFINED tag)
      set(tag ${element})

      if(NOT "${tag}" IN_LIST export_target_settings_tags)
        message(FATAL_ERROR "Unsupported export target setting: ${tag}\n"
          "Supported export target settings: ${export_target_settings_tags}"
        )
      endif()
    else()
      set(value ${element})

      if(tag STREQUAL "TARGET_PROJECT_FOLDER")
        string(REPLACE "\\" "/" value "${value}")
        _FRUT_abs_path_based_on_jucer_project_dir("${value}" value)
        set(JUCER_TARGET_PROJECT_FOLDER ${value} PARENT_SCOPE)

      elseif(tag STREQUAL "VST_SDK_FOLDER")
        string(REPLACE "\\" "/" value "${value}")
        _FRUT_abs_path_based_on_jucer_project_dir("${value}" value)
        set(JUCER_VST_SDK_FOLDER ${value} PARENT_SCOPE)

      elseif(tag STREQUAL "VST3_SDK_FOLDER")
        string(REPLACE "\\" "/" value "${value}")
        _FRUT_abs_path_based_on_jucer_project_dir("${value}" value)
        set(JUCER_VST3_SDK_FOLDER ${value} PARENT_SCOPE)

      elseif(tag STREQUAL "EXTRA_PREPROCESSOR_DEFINITIONS")
        string(REPLACE "\n" ";" value "${value}")
        set(JUCER_EXTRA_PREPROCESSOR_DEFINITIONS ${value} PARENT_SCOPE)

      elseif(tag STREQUAL "EXTRA_COMPILER_FLAGS")
        string(REPLACE "\n" " " value "${value}")
        string(REPLACE " " ";" value "${value}")
        set(JUCER_EXTRA_COMPILER_FLAGS ${value} PARENT_SCOPE)

      elseif(tag STREQUAL "EXTRA_LINKER_FLAGS")
        string(REPLACE "\n" " " value "${value}")
        set(JUCER_EXTRA_LINKER_FLAGS ${value} PARENT_SCOPE)

      elseif(tag STREQUAL "EXTERNAL_LIBRARIES_TO_LINK")
        string(REPLACE "\n" ";" value "${value}")
        set(JUCER_EXTERNAL_LIBRARIES_TO_LINK ${value} PARENT_SCOPE)

      elseif(tag STREQUAL "ICON_SMALL")
        if(NOT value STREQUAL "<None>")
          _FRUT_abs_path_based_on_jucer_project_dir("${value}" value)
          set(JUCER_SMALL_ICON ${value} PARENT_SCOPE)
        endif()

      elseif(tag STREQUAL "ICON_LARGE")
        if(NOT value STREQUAL "<None>")
          _FRUT_abs_path_based_on_jucer_project_dir("${value}" value)
          set(JUCER_LARGE_ICON ${value} PARENT_SCOPE)
        endif()

      elseif(tag STREQUAL "CUSTOM_XCODE_RESOURCE_FOLDERS")
        string(REPLACE "\n" ";" value "${value}")
        unset(resource_folders)
        foreach(folder ${value})
          _FRUT_abs_path_based_on_jucer_project_dir("${folder}" abs_folder)
          list(APPEND resource_folders "${abs_folder}")
        endforeach()
        set(JUCER_CUSTOM_XCODE_RESOURCE_FOLDERS ${resource_folders} PARENT_SCOPE)

      elseif(tag STREQUAL "DOCUMENT_FILE_EXTENSIONS")
        string(REPLACE "," ";" value "${value}")
        set(JUCER_DOCUMENT_FILE_EXTENSIONS ${value} PARENT_SCOPE)

      elseif(tag STREQUAL "EXTRA_FRAMEWORKS")
        string(REPLACE "," ";" value "${value}")
        string(REPLACE " " "" value "${value}")
        set(JUCER_EXTRA_FRAMEWORKS ${value} PARENT_SCOPE)

      elseif(tag STREQUAL "CUSTOM_PLIST")
        set(JUCER_CUSTOM_PLIST "${value}" PARENT_SCOPE)

      elseif(tag STREQUAL "PREBUILD_SHELL_SCRIPT")
        set(script_content "${value}")
        configure_file("${Reprojucer_templates_DIR}/script.in" "prebuild.sh" @ONLY)
        set(JUCER_PREBUILD_SHELL_SCRIPT
          "${CMAKE_CURRENT_BINARY_DIR}/prebuild.sh" PARENT_SCOPE
        )

      elseif(tag STREQUAL "POSTBUILD_SHELL_SCRIPT")
        set(script_content "${value}")
        configure_file("${Reprojucer_templates_DIR}/script.in" "postbuild.sh" @ONLY)
        set(JUCER_POSTBUILD_SHELL_SCRIPT
          "${CMAKE_CURRENT_BINARY_DIR}/postbuild.sh" PARENT_SCOPE
        )

      elseif(tag STREQUAL "DEVELOPMENT_TEAM_ID")
        message(WARNING "Reprojucer.cmake doesn't support the setting "
          "DEVELOPMENT_TEAM_ID (\"Development Team ID\" in Projucer). If you would like "
          "Reprojucer.cmake to support this setting, please leave a comment on the issue "
          "\"Reprojucer.cmake doesn't support the setting DEVELOPMENT_TEAM_ID\" on "
          "GitHub: https://github.com/McMartin/FRUT/issues/251"
        )

      elseif(tag STREQUAL "PLATFORM_TOOLSET")
        if((exporter STREQUAL "Visual Studio 2017"
              AND (value STREQUAL "v140" OR value STREQUAL "v140_xp"
                OR value STREQUAL "v141" OR value STREQUAL "v141_xp"))
            OR (exporter STREQUAL "Visual Studio 2015"
              AND (value STREQUAL "v140" OR value STREQUAL "v140_xp"
                OR value STREQUAL "CTP_Nov2013"))
            OR (exporter STREQUAL "Visual Studio 2013"
              AND (value STREQUAL "v120" OR value STREQUAL "v120_xp"
                OR value STREQUAL "Windows7" OR value STREQUAL "CTP_Nov2013")))
          if(NOT value STREQUAL "${CMAKE_VS_PLATFORM_TOOLSET}")
            message(FATAL_ERROR "You must call `cmake -T ${value}` in order to build with"
              " the toolset \"${value}\"."
            )
          endif()
        elseif(NOT value STREQUAL "(default)")
          message(FATAL_ERROR "Unsupported value for PLATFORM_TOOLSET: \"${value}\"")
        endif()

      elseif(tag STREQUAL "USE_IPP_LIBRARY")
        set(ipp_library_values
          "Yes (Default Mode)"
          "Multi-Threaded Static Library"
          "Single-Threaded Static Library"
          "Multi-Threaded DLL"
          "Single-Threaded DLL"
        )
        if("${value}" IN_LIST ipp_library_values)
          message(WARNING "Reprojucer.cmake doesn't support the setting USE_IPP_LIBRARY "
            "(\"Use IPP Library\" in Projucer). If you would like Reprojucer.cmake to "
            "support this setting, please leave a comment on the issue "
            "\"Reprojucer.cmake doesn't support the setting USE_IPP_LIBRARY\" on GitHub: "
            "https://github.com/McMartin/FRUT/issues/252"
          )
        elseif(NOT value STREQUAL "No")
          message(FATAL_ERROR "Unsupported value for USE_IPP_LIBRARY: \"${value}\"")
        endif()

      elseif(tag STREQUAL "CXX_STANDARD_TO_USE"
          AND exporter STREQUAL "Visual Studio 2017")
        if(value STREQUAL "C++14")
          set(JUCER_CXX_STANDARD_TO_USE "14" PARENT_SCOPE)
        elseif(value STREQUAL "Latest C++ Standard")
          set(JUCER_CXX_STANDARD_TO_USE "latest" PARENT_SCOPE)
        elseif(NOT value STREQUAL "(default)")
          message(FATAL_ERROR "Unsupported value for CXX_STANDARD_TO_USE: \"${value}\"")
        endif()

      elseif(tag STREQUAL "CXX_STANDARD_TO_USE" AND exporter STREQUAL "Linux Makefile")
        if(value MATCHES "^C\\+\\+(03|11|14)$")
          set(JUCER_CXX_LANGUAGE_STANDARD ${value} PARENT_SCOPE)
        else()
          message(FATAL_ERROR "Unsupported value for CXX_STANDARD_TO_USE: \"${value}\"")
        endif()

      elseif(tag STREQUAL "PKGCONFIG_LIBRARIES")
        string(REPLACE " " ";" value "${value}")
        set(JUCER_PKGCONFIG_LIBRARIES ${value} PARENT_SCOPE)

      endif()

      unset(tag)
    endif()
  endforeach()

endfunction()


function(jucer_export_target_configuration
  exporter NAME_TAG config DEBUG_MODE_TAG is_debug
)

  if(NOT "${exporter}" IN_LIST Reprojucer_supported_exporters)
    message(FATAL_ERROR "Unsupported exporter: ${exporter}\n"
      "Supported exporters: ${Reprojucer_supported_exporters}"
    )
  endif()

  if(NOT "${exporter}" IN_LIST JUCER_PROJECT_EXPORT_TARGETS)
    message(FATAL_ERROR "You must call jucer_export_target(\"${exporter}\") before "
      "calling jucer_export_target_configuration(\"${exporter}\")."
    )
  endif()

  if(NOT NAME_TAG STREQUAL "NAME")
    message(FATAL_ERROR
      "Invalid second argument. Expected \"NAME\", but got \"${NAME_TAG}\" instead."
    )
  endif()

  if(NOT DEBUG_MODE_TAG STREQUAL "DEBUG_MODE")
    message(FATAL_ERROR "Invalid fourth argument. Expected \"DEBUG_MODE\", "
      "but got \"${DEBUG_MODE_TAG}\" instead."
    )
  endif()

  list(FIND Reprojucer_supported_exporters "${exporter}" exporter_index)
  list(GET Reprojucer_supported_exporters_conditions ${exporter_index} condition)
  if(NOT ${condition})
    return()
  endif()

  if("${config}" IN_LIST JUCER_PROJECT_CONFIGURATIONS)
    message(FATAL_ERROR
      "You cannot call jucer_export_target_configuration("
      "\"${exporter}\" NAME \"${config}\") twice."
    )
  endif()

  list(APPEND JUCER_PROJECT_CONFIGURATIONS ${config})
  set(JUCER_PROJECT_CONFIGURATIONS ${JUCER_PROJECT_CONFIGURATIONS} PARENT_SCOPE)

  set(JUCER_CONFIGURATION_IS_DEBUG_${config} ${is_debug} PARENT_SCOPE)

  set(configuration_settings_tags
    "BINARY_NAME"
    "BINARY_LOCATION"
    "HEADER_SEARCH_PATHS"
    "EXTRA_LIBRARY_SEARCH_PATHS"
    "PREPROCESSOR_DEFINITIONS"
    "OPTIMISATION"
  )

  if(exporter STREQUAL "Xcode (MacOSX)")
    list(APPEND configuration_settings_tags
      "VST_BINARY_LOCATION"
      "VST3_BINARY_LOCATION"
      "AU_BINARY_LOCATION"
      "OSX_BASE_SDK_VERSION"
      "OSX_DEPLOYMENT_TARGET"
      "OSX_ARCHITECTURE"
      "CUSTOM_XCODE_FLAGS"
      "CXX_LANGUAGE_STANDARD"
      "CXX_LIBRARY"
      "CODE_SIGNING_IDENTITY"
      "RELAX_IEEE_COMPLIANCE"
      "LINK_TIME_OPTIMISATION"
      "STRIP_LOCAL_SYMBOLS"
    )
  endif()

  if(exporter MATCHES "^Visual Studio 201(7|5|3)$")
    list(APPEND configuration_settings_tags
      "WARNING_LEVEL"
      "TREAT_WARNINGS_AS_ERRORS"
      "RUNTIME_LIBRARY"
      "WHOLE_PROGRAM_OPTIMISATION"
      "INCREMENTAL_LINKING"
      "PREBUILD_COMMAND"
      "POSTBUILD_COMMAND"
      "GENERATE_MANIFEST"
      "CHARACTER_SET"
      "ARCHITECTURE"
      "RELAX_IEEE_COMPLIANCE"
    )
  endif()

  if(exporter STREQUAL "Linux Makefile")
    list(APPEND configuration_settings_tags
      "ARCHITECTURE"
    )
  endif()

  unset(tag)
  foreach(element ${ARGN})
    if(NOT DEFINED tag)
      set(tag ${element})

      if(NOT "${tag}" IN_LIST configuration_settings_tags)
        message(FATAL_ERROR "Unsupported configuration setting: ${tag}\n"
          "Supported configuration settings: ${configuration_settings_tags}"
        )
      endif()
    else()
      set(value ${element})

      if(tag STREQUAL "BINARY_NAME")
        set(JUCER_BINARY_NAME_${config} ${value} PARENT_SCOPE)

      elseif(tag STREQUAL "BINARY_LOCATION")
        get_filename_component(abs_path "${value}" ABSOLUTE)
        set(JUCER_BINARY_LOCATION_${config} ${abs_path} PARENT_SCOPE)

      elseif(tag STREQUAL "HEADER_SEARCH_PATHS")
        string(REPLACE "\\" "/" value "${value}")
        string(REPLACE "\n" ";" value "${value}")
        unset(header_search_paths)
        foreach(path ${value})
          _FRUT_abs_path_based_on_jucer_project_dir("${path}" path)
          list(APPEND header_search_paths "${path}")
        endforeach()
        set(JUCER_HEADER_SEARCH_PATHS_${config} ${header_search_paths} PARENT_SCOPE)

      elseif(tag STREQUAL "EXTRA_LIBRARY_SEARCH_PATHS")
        string(REPLACE "\\" "/" value "${value}")
        string(REPLACE "\n" ";" value "${value}")
        unset(library_search_paths)
        foreach(path ${value})
          _FRUT_abs_path_based_on_jucer_project_dir("${path}" path)
          list(APPEND library_search_paths "${path}")
        endforeach()
        set(JUCER_EXTRA_LIBRARY_SEARCH_PATHS_${config}
          ${library_search_paths} PARENT_SCOPE
        )

      elseif(tag STREQUAL "PREPROCESSOR_DEFINITIONS")
        string(REPLACE "\n" ";" value "${value}")
        set(JUCER_PREPROCESSOR_DEFINITIONS_${config} ${value} PARENT_SCOPE)

      elseif(tag STREQUAL "OPTIMISATION")
        if(exporter MATCHES "^Visual Studio 201(7|5|3)$")
          if(value STREQUAL "No optimisation")
            set(optimisation_flag "/Od")
          elseif(value STREQUAL "Minimise size")
            set(optimisation_flag "/O1")
          elseif(value STREQUAL "Maximise speed")
            set(optimisation_flag "/Ox")
          else()
            message(FATAL_ERROR "Unsupported value for OPTIMISATION: \"${value}\"")
          endif()
        else()
          if(value STREQUAL "-O0 (no optimisation)")
            set(optimisation_flag "-O0")
          elseif(value STREQUAL "-Os (minimise code size)")
            set(optimisation_flag "-Os")
          elseif(value STREQUAL "-O3 (fastest with safe optimisations)")
            set(optimisation_flag "-O3")
          elseif(value STREQUAL "-O1 (fast)")
            set(optimisation_flag "-O1")
          elseif(value STREQUAL "-O2 (faster)")
            set(optimisation_flag "-O2")
          elseif(value STREQUAL "-Ofast (uses aggressive optimisations)")
            set(optimisation_flag "-Ofast")
          else()
            message(FATAL_ERROR "Unsupported value for OPTIMISATION: \"${value}\"")
          endif()
        endif()
        set(JUCER_OPTIMISATION_FLAG_${config} ${optimisation_flag} PARENT_SCOPE)

      elseif(tag STREQUAL "VST_BINARY_LOCATION")
        set(JUCER_VST_BINARY_LOCATION_${config} ${value} PARENT_SCOPE)

      elseif(tag STREQUAL "VST3_BINARY_LOCATION")
        set(JUCER_VST3_BINARY_LOCATION_${config} ${value} PARENT_SCOPE)

      elseif(tag STREQUAL "AU_BINARY_LOCATION")
        set(JUCER_AU_BINARY_LOCATION_${config} ${value} PARENT_SCOPE)

      elseif(tag STREQUAL "OSX_BASE_SDK_VERSION")
        if(value MATCHES "^10\\.([5-9]|10|11|12) SDK$")
          set(JUCER_OSX_BASE_SDK_VERSION_${config} "10.${CMAKE_MATCH_1}" PARENT_SCOPE)
        elseif(value STREQUAL "Use Default")
          set(JUCER_OSX_BASE_SDK_VERSION_${config} "default" PARENT_SCOPE)
        else()
          message(FATAL_ERROR "Unsupported value for OSX_BASE_SDK_VERSION: \"${value}\"")
        endif()

      elseif(tag STREQUAL "OSX_DEPLOYMENT_TARGET")
        if(value MATCHES "^10\\.([5-9]|10|11|12)$")
          set(JUCER_OSX_DEPLOYMENT_TARGET_${config} "10.${CMAKE_MATCH_1}" PARENT_SCOPE)
        elseif(value STREQUAL "Use Default")
          set(JUCER_OSX_DEPLOYMENT_TARGET_${config} "default" PARENT_SCOPE)
        else()
          message(FATAL_ERROR "Unsupported value for OSX_DEPLOYMENT_TARGET: \"${value}\"")
        endif()

      elseif(tag STREQUAL "OSX_ARCHITECTURE")
        if(value STREQUAL "Native architecture of build machine")
          # Consider as default
        elseif(value STREQUAL "Universal Binary (32-bit)")
          set(JUCER_OSX_ARCHITECTURES_${config} "i386" PARENT_SCOPE)
        elseif(value STREQUAL "Universal Binary (32/64-bit)")
          set(JUCER_OSX_ARCHITECTURES_${config} "x86_64" "i386" PARENT_SCOPE)
        elseif(value STREQUAL "64-bit Intel")
          set(JUCER_OSX_ARCHITECTURES_${config} "x86_64" PARENT_SCOPE)
        elseif(NOT value STREQUAL "Use Default")
          message(FATAL_ERROR "Unsupported value for OSX_ARCHITECTURE: \"${value}\"")
        endif()

      elseif(tag STREQUAL "CUSTOM_XCODE_FLAGS")
        if(NOT CMAKE_GENERATOR STREQUAL "Xcode")
          message(WARNING "CUSTOM_XCODE_FLAGS is only supported when using the Xcode "
            "generator. You should call `cmake -G Xcode`."
          )
        endif()
        string(REGEX REPLACE ", *" ";" value "${value}")
        set(JUCER_CUSTOM_XCODE_FLAGS_${config} ${value} PARENT_SCOPE)

      elseif(tag STREQUAL "CXX_LANGUAGE_STANDARD")
        if(value MATCHES "^(C|GNU)\\+\\+98$" AND DEFINED JUCER_VERSION
            AND JUCER_VERSION VERSION_LESS 5)
          set(JUCER_CXX_LANGUAGE_STANDARD_${config} ${value} PARENT_SCOPE)
        elseif(value MATCHES "^(C|GNU)\\+\\+(11|14)$")
          set(JUCER_CXX_LANGUAGE_STANDARD_${config} ${value} PARENT_SCOPE)
        elseif(NOT value STREQUAL "Use Default")
          message(FATAL_ERROR "Unsupported value for CXX_LANGUAGE_STANDARD: \"${value}\"")
        endif()

      elseif(tag STREQUAL "CXX_LIBRARY")
        if(value STREQUAL "LLVM libc++")
          set(JUCER_CXX_LIBRARY_${config} "libc++" PARENT_SCOPE)
        elseif(value STREQUAL "GNU libstdc++")
          set(JUCER_CXX_LIBRARY_${config} "libstdc++" PARENT_SCOPE)
        elseif(NOT value STREQUAL "Use Default")
          message(FATAL_ERROR "Unsupported value for CXX_LIBRARY: \"${value}\"")
        endif()

      elseif(tag STREQUAL "CODE_SIGNING_IDENTITY")
        if(NOT CMAKE_GENERATOR STREQUAL "Xcode")
          message(WARNING "CODE_SIGNING_IDENTITY is only supported when using the Xcode "
            "generator. You should call `cmake -G Xcode`."
          )
        endif()
        set(JUCER_CODE_SIGNING_IDENTITY_${config} ${value} PARENT_SCOPE)

      elseif(tag STREQUAL "RELAX_IEEE_COMPLIANCE")
        set(JUCER_RELAX_IEEE_COMPLIANCE_${config} ${value} PARENT_SCOPE)

      elseif(tag STREQUAL "LINK_TIME_OPTIMISATION")
        set(JUCER_LINK_TIME_OPTIMISATION_${config} ${value} PARENT_SCOPE)

      elseif(tag STREQUAL "STRIP_LOCAL_SYMBOLS")
        set(JUCER_STRIP_LOCAL_SYMBOLS_${config} ${value} PARENT_SCOPE)

      elseif(tag STREQUAL "WARNING_LEVEL")
        if(value STREQUAL "Low")
          set(level 2)
        elseif(value STREQUAL "Medium")
          set(level 3)
        elseif(value STREQUAL "High")
          set(level 4)
        else()
          message(FATAL_ERROR "Unsupported value for WARNING_LEVEL: \"${value}\"")
        endif()
        set(JUCER_WARNING_LEVEL_FLAG_${config} "/W${level}" PARENT_SCOPE)

      elseif(tag STREQUAL "TREAT_WARNINGS_AS_ERRORS")
        set(JUCER_TREAT_WARNINGS_AS_ERRORS_${config} ${value} PARENT_SCOPE)

      elseif(tag STREQUAL "RUNTIME_LIBRARY")
        if(value STREQUAL "Use DLL runtime")
          if(is_debug)
            set(flag "/MDd")
          else()
            set(flag "/MD")
          endif()
        elseif(value STREQUAL "Use static runtime")
          if(is_debug)
            set(flag "/MTd")
          else()
            set(flag "/MT")
          endif()
        elseif(NOT value STREQUAL "(Default)")
          message(FATAL_ERROR "Unsupported value for RUNTIME_LIBRARY: \"${value}\"")
        endif()
        set(JUCER_RUNTIME_LIBRARY_FLAG_${config} ${flag} PARENT_SCOPE)

      elseif(tag STREQUAL "WHOLE_PROGRAM_OPTIMISATION")
        if(value STREQUAL "Always disable")
          set(JUCER_ALWAYS_DISABLE_WPO_${config} TRUE PARENT_SCOPE)
        elseif(NOT value STREQUAL "Enable when possible")
          message(FATAL_ERROR
            "Unsupported value for WHOLE_PROGRAM_OPTIMISATION: \"${value}\""
          )
        endif()

      elseif(tag STREQUAL "INCREMENTAL_LINKING")
        set(JUCER_INCREMENTAL_LINKING_${config} ${value} PARENT_SCOPE)

      elseif(tag STREQUAL "PREBUILD_COMMAND")
        set(script_content "${value}")
        configure_file("${Reprojucer_templates_DIR}/script.in"
          "prebuild_${config}.cmd" @ONLY
        )
        set(JUCER_PREBUILD_COMMAND_${config}
          "${CMAKE_CURRENT_BINARY_DIR}/prebuild_${config}.cmd" PARENT_SCOPE
        )

      elseif(tag STREQUAL "POSTBUILD_COMMAND")
        set(script_content "${value}")
        configure_file("${Reprojucer_templates_DIR}/script.in"
          "postbuild_${config}.cmd" @ONLY
        )
        set(JUCER_POSTBUILD_COMMAND_${config}
          "${CMAKE_CURRENT_BINARY_DIR}/postbuild_${config}.cmd" PARENT_SCOPE
        )

      elseif(tag STREQUAL "GENERATE_MANIFEST")
        set(JUCER_GENERATE_MANIFEST_${config} ${value} PARENT_SCOPE)

      elseif(tag STREQUAL "CHARACTER_SET")
        set(character_sets "Default" "MultiByte" "Unicode")
        if("${value}" IN_LIST character_sets)
          set(JUCER_CHARACTER_SET_${config} ${value} PARENT_SCOPE)
        else()
          message(FATAL_ERROR "Unsupported value for CHARACTER_SET: \"${value}\"")
        endif()

      elseif(tag STREQUAL "ARCHITECTURE"
          AND exporter MATCHES "^Visual Studio 201(7|5|3)$")
        if(value STREQUAL "32-bit")
          set(wants_x64 FALSE)
        elseif(value STREQUAL "x64")
          set(wants_x64 TRUE)
        else()
          message(FATAL_ERROR "Unsupported value for ARCHITECTURE: \"${value}\"")
        endif()
        if(CMAKE_GENERATOR_PLATFORM STREQUAL "x64" OR CMAKE_GENERATOR MATCHES "Win64")
          set(is_x64 TRUE)
        else()
          set(is_x64 FALSE)
        endif()
        if(wants_x64 AND NOT is_x64)
          message(FATAL_ERROR "You must call `cmake -G\"${CMAKE_GENERATOR} Win64\"` or "
            "`cmake -G\"${CMAKE_GENERATOR}\" -A x64` in order to build for 64-bit."
          )
        elseif(NOT wants_x64 AND is_x64)
          string(FIND "${CMAKE_GENERATOR}" " Win64" length REVERSE)
          string(SUBSTRING "${CMAKE_GENERATOR}" 0 ${length} 32_bit_generator)
          message(FATAL_ERROR "You must call `cmake -G\"${32_bit_generator}\"` or "
            "`cmake -G\"${32_bit_generator}\" -A Win32` in order to build for 32-bit."
          )
        endif()

      elseif(tag STREQUAL "ARCHITECTURE" AND exporter STREQUAL "Linux Makefile")
        if(value STREQUAL "(Default)")
          set(architecture_flag "-march=native")
        elseif(value STREQUAL "32-bit (-m32)")
          set(architecture_flag "-m32")
        elseif(value STREQUAL "64-bit (-m64)")
          set(architecture_flag "-m64")
        elseif(value STREQUAL "ARM v6")
          set(architecture_flag "-march=armv6")
        elseif(value STREQUAL "ARM v7")
          set(architecture_flag "-march=armv7")
        elseif(NOT value STREQUAL "<None>")
          message(FATAL_ERROR "Unsupported value for ARCHITECTURE: \"${value}\"")
        endif()
        set(JUCER_ARCHITECTURE_FLAG_${config} ${architecture_flag} PARENT_SCOPE)

      endif()

      unset(tag)
    endif()
  endforeach()

endfunction()


function(jucer_project_end)

  if(NOT "${Reprojucer_current_exporter}" IN_LIST JUCER_PROJECT_EXPORT_TARGETS)
    message(FATAL_ERROR
      "You must call jucer_export_target(\"${Reprojucer_current_exporter}\") before "
      "calling jucer_project_end()."
    )
  endif()

  if(NOT JUCER_PROJECT_CONFIGURATIONS)
    message(FATAL_ERROR "You must call "
      "jucer_export_target_configuration(\"${Reprojucer_current_exporter}\") before "
      "calling jucer_project_end()."
    )
  endif()

  if(NOT DEFINED CMAKE_CONFIGURATION_TYPES)
    if("${CMAKE_BUILD_TYPE}" STREQUAL "")
      list(GET JUCER_PROJECT_CONFIGURATIONS 0 first_configuration)
      message(STATUS
        "Setting CMAKE_BUILD_TYPE to \"${first_configuration}\" as it was not specified."
      )
      set(CMAKE_BUILD_TYPE ${first_configuration})
      set(CMAKE_BUILD_TYPE ${first_configuration} PARENT_SCOPE)
    elseif(NOT "${CMAKE_BUILD_TYPE}" IN_LIST JUCER_PROJECT_CONFIGURATIONS)
      message(FATAL_ERROR "Undefined build configuration: ${CMAKE_BUILD_TYPE}\n"
        "Defined build configurations: ${JUCER_PROJECT_CONFIGURATIONS}"
      )
    endif()
  endif()

  set(CMAKE_CONFIGURATION_TYPES ${JUCER_PROJECT_CONFIGURATIONS} PARENT_SCOPE)

  if(DEFINED JUCER_OSX_BASE_SDK_VERSION_${CMAKE_BUILD_TYPE})
    set(osx_base_sdk_version ${JUCER_OSX_BASE_SDK_VERSION_${CMAKE_BUILD_TYPE}})
  else()
    unset(all_confs_osx_base_sdk_version)
    unset(config_to_value)
    foreach(config ${JUCER_PROJECT_CONFIGURATIONS})
      if(DEFINED JUCER_OSX_BASE_SDK_VERSION_${config})
        list(APPEND all_confs_osx_base_sdk_version
          ${JUCER_OSX_BASE_SDK_VERSION_${config}}
        )
        string(APPEND config_to_value "  ${config}: "
          "\"${JUCER_OSX_BASE_SDK_VERSION_${config}}\"\n"
        )
      endif()
    endforeach()
    if(all_confs_osx_base_sdk_version)
      list(GET all_confs_osx_base_sdk_version 0 osx_base_sdk_version)
      list(REMOVE_DUPLICATES all_confs_osx_base_sdk_version)
      list(LENGTH all_confs_osx_base_sdk_version all_confs_osx_base_sdk_version_length)
      if(NOT all_confs_osx_base_sdk_version_length EQUAL 1)
        message(STATUS "Different values for OSX_BASE_SDK_VERSION:\n${config_to_value}"
          "Falling back to the first value: \"${osx_base_sdk_version}\"."
        )
      endif()
    endif()
  endif()
  if(osx_base_sdk_version AND NOT osx_base_sdk_version STREQUAL "default")
    if(CMAKE_GENERATOR STREQUAL "Xcode")
      set(CMAKE_OSX_SYSROOT "macosx${osx_base_sdk_version}")
    else()
      execute_process(
        COMMAND "xcrun" "--sdk" "macosx${osx_base_sdk_version}" "--show-sdk-path"
        OUTPUT_VARIABLE sysroot
        OUTPUT_STRIP_TRAILING_WHITESPACE
      )
      if(NOT IS_DIRECTORY "${sysroot}")
        message(WARNING
          "Running `xcrun --sdk macosx${osx_base_sdk_version} --show-sdk-path` "
          "didn't output a valid directory."
        )
      else()
        set(CMAKE_OSX_SYSROOT ${sysroot} PARENT_SCOPE)
      endif()
    endif()
  endif()

  if(DEFINED JUCER_OSX_DEPLOYMENT_TARGET_${CMAKE_BUILD_TYPE})
    set(osx_deployment_target ${JUCER_OSX_DEPLOYMENT_TARGET_${CMAKE_BUILD_TYPE}})
  else()
    unset(all_confs_osx_deployment_target)
    unset(config_to_value)
    foreach(config ${JUCER_PROJECT_CONFIGURATIONS})
      if(DEFINED JUCER_OSX_DEPLOYMENT_TARGET_${config})
        list(APPEND all_confs_osx_deployment_target
          ${JUCER_OSX_DEPLOYMENT_TARGET_${config}}
        )
        string(APPEND config_to_value "  ${config}: "
          "\"${JUCER_OSX_DEPLOYMENT_TARGET_${config}}\"\n"
        )
      endif()
    endforeach()
    if(all_confs_osx_deployment_target)
      list(GET all_confs_osx_deployment_target 0 osx_deployment_target)
      list(REMOVE_DUPLICATES all_confs_osx_deployment_target)
      list(LENGTH all_confs_osx_deployment_target all_confs_osx_deployment_target_length)
      if(NOT all_confs_osx_deployment_target_length EQUAL 1)
        message(STATUS "Different values for OSX_DEPLOYMENT_TARGET:\n${config_to_value}"
          "Falling back to the first value: \"${osx_deployment_target}\"."
        )
      endif()
    endif()
  endif()
  if(osx_deployment_target AND NOT osx_deployment_target STREQUAL "default")
    set(CMAKE_OSX_DEPLOYMENT_TARGET "${osx_deployment_target}" PARENT_SCOPE)
  endif()

  project(${JUCER_PROJECT_NAME})

  _FRUT_generate_AppConfig_header()
  _FRUT_generate_JuceHeader_header()

  if(DEFINED JUCER_SMALL_ICON OR DEFINED JUCER_LARGE_ICON)
    if(APPLE)
      _FRUT_generate_icon_file("icns" icon_filename)
    elseif(WIN32)
      _FRUT_generate_icon_file("ico" icon_filename)
    endif()

    if(DEFINED icon_filename)
      set(JUCER_BUNDLE_ICON_FILE ${icon_filename})
    endif()
  endif()

  if(WIN32 AND NOT JUCER_PROJECT_TYPE STREQUAL "Static Library")
    if(DEFINED JUCER_COMPANY_COPYRIGHT
        OR NOT(DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5.2))
      set(resources_rc_legal_copyright
        "\n      VALUE \"LegalCopyright\",  \"${JUCER_COMPANY_COPYRIGHT}\\0\""
      )
    endif()
    if(DEFINED icon_filename)
      string(CONCAT resources_rc_icon_settings
        "\nIDI_ICON1 ICON DISCARDABLE \"${icon_filename}\""
        "\nIDI_ICON2 ICON DISCARDABLE \"${icon_filename}\""
      )
    endif()

    string(REPLACE "." "," comma_separated_version_number "${JUCER_PROJECT_VERSION}")
    configure_file("${Reprojucer_templates_DIR}/resources.rc"
      "JuceLibraryCode/resources.rc"
    )
    list(APPEND JUCER_PROJECT_SOURCES
      "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode/resources.rc"
    )
  endif()

  source_group("Juce Library Code"
    REGULAR_EXPRESSION "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode/*"
  )

  set_source_files_properties(
    ${JUCER_PROJECT_BROWSABLE_FILES}
    ${JUCER_PROJECT_RESOURCES}
    PROPERTIES HEADER_FILE_ONLY TRUE
  )

  if(DEFINED JUCER_COMPANY_COPYRIGHT
      OR NOT(DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5.2))
    set(ns_human_readable_copyright "${JUCER_COMPANY_COPYRIGHT}")
  else()
    set(ns_human_readable_copyright "${JUCER_COMPANY_NAME}")
  endif()

  set(main_plist_entries "
    <key>CFBundleExecutable</key>
    <string>@bundle_executable@</string>
    <key>CFBundleIconFile</key>
    <string>@JUCER_BUNDLE_ICON_FILE@</string>
    <key>CFBundleIdentifier</key>
    <string>@bundle_identifier@</string>
    <key>CFBundleName</key>
    <string>@JUCER_PROJECT_NAME@</string>
    <key>CFBundleDisplayName</key>
    <string>@JUCER_PROJECT_NAME@</string>
    <key>CFBundlePackageType</key>
    <string>@bundle_package_type@</string>
    <key>CFBundleSignature</key>
    <string>@bundle_signature@</string>
    <key>CFBundleShortVersionString</key>
    <string>@JUCER_PROJECT_VERSION@</string>
    <key>CFBundleVersion</key>
    <string>@JUCER_PROJECT_VERSION@</string>
    <key>NSHumanReadableCopyright</key>
    <string>@ns_human_readable_copyright@</string>
    <key>NSHighResolutionCapable</key>
    <true/>"
  )

  if(JUCER_CUSTOM_PLIST)
    set(PListMerger_version "0.1.0")
    find_program(PListMerger_exe "PListMerger-${PListMerger_version}"
      PATHS "${Reprojucer.cmake_DIR}/bin"
      NO_DEFAULT_PATH
    )
    if(NOT PListMerger_exe)
      message(STATUS "Building PListMerger")
      try_compile(PListMerger
        "${Reprojucer.cmake_DIR}/PListMerger/_build/${CMAKE_GENERATOR}"
        "${Reprojucer.cmake_DIR}/PListMerger"
        PListMerger install
        CMAKE_FLAGS
        "-DJUCE_modules_DIRS=${JUCER_PROJECT_MODULES_FOLDERS}"
        "-DCMAKE_INSTALL_PREFIX=${Reprojucer.cmake_DIR}/bin"
      )
      if(NOT PListMerger)
        message(FATAL_ERROR "Failed to build PListMerger")
      endif()
      message(STATUS "PListMerger has been successfully built")
      find_program(PListMerger_exe "PListMerger-${PListMerger_version}"
        PATHS "${Reprojucer.cmake_DIR}/bin"
        NO_DEFAULT_PATH
      )
      if(NOT PListMerger_exe)
        message(FATAL_ERROR "Could not find PListMerger-${PListMerger_version}")
      endif()
    endif()

    execute_process(
      COMMAND
      "${PListMerger_exe}"
      "${JUCER_CUSTOM_PLIST}"
      "<plist><dict>${main_plist_entries}</dict></plist>"
      OUTPUT_VARIABLE PListMerger_output
      OUTPUT_STRIP_TRAILING_WHITESPACE
      RESULT_VARIABLE PListMerger_return_code
    )
    if(NOT PListMerger_return_code EQUAL 0)
      message(FATAL_ERROR "Error when executing PListMerger")
    endif()

    if(WIN32)
      string(REPLACE "\r\n" "\n" PListMerger_output "${PListMerger_output}")
    endif()
    string(REPLACE "<plist>\n  <dict>" "" PListMerger_output "${PListMerger_output}")
    string(REPLACE "\n  </dict>\n</plist>" "" PListMerger_output "${PListMerger_output}")
    set(main_plist_entries "${PListMerger_output}")
  endif()

  string(REGEX REPLACE "[^A-Za-z0-9_.+-]" "_" target "${JUCER_PROJECT_NAME}")

  if(NOT APPLE)
    unset(JUCER_PROJECT_XCODE_RESOURCES)
  endif()
  set_source_files_properties(${JUCER_PROJECT_XCODE_RESOURCES}
    PROPERTIES MACOSX_PACKAGE_LOCATION "Resources"
  )

  set(all_sources
    ${JUCER_PROJECT_SOURCES}
    ${JUCER_PROJECT_RESOURCES}
    ${JUCER_PROJECT_BROWSABLE_FILES}
    ${JUCER_PROJECT_XCODE_RESOURCES}
  )

  if(JUCER_PROJECT_TYPE STREQUAL "Console Application")
    add_executable(${target} ${all_sources})
    _FRUT_set_output_directory_properties(${target} "ConsoleApp")
    _FRUT_set_common_target_properties(${target})
    _FRUT_link_osx_frameworks(${target})
    _FRUT_set_custom_xcode_flags(${target})

  elseif(JUCER_PROJECT_TYPE STREQUAL "GUI Application")
    add_executable(${target} WIN32 MACOSX_BUNDLE ${all_sources})

    if(JUCER_DOCUMENT_FILE_EXTENSIONS)
      unset(bundle_type_extensions)
      foreach(type_extension ${JUCER_DOCUMENT_FILE_EXTENSIONS})
        if(type_extension MATCHES "^\\.")
          string(SUBSTRING "${type_extension}" 1 -1 type_extension)
        endif()
        string(APPEND bundle_type_extensions
          "\n          <string>${type_extension}</string>"
        )
      endforeach()
      list(GET JUCER_DOCUMENT_FILE_EXTENSIONS 0 first_type_extension)
      if(first_type_extension MATCHES "^\\.")
        string(SUBSTRING "${first_type_extension}" 1 -1 first_type_extension)
      endif()

      set(bundle_document_types_entries "
    <key>CFBundleDocumentTypes</key>
    <array>
      <dict>
        <key>CFBundleTypeExtensions</key>
        <array>${bundle_type_extensions}
        </array>
        <key>CFBundleTypeName</key>
        <string>${first_type_extension}</string>
        <key>CFBundleTypeRole</key>
        <string>Editor</string>
        <key>CFBundleTypeIconFile</key>
        <string>Icon</string>
        <key>NSPersistentStoreTypeKey</key>
        <string>XML</string>
      </dict>
    </array>"
      )
    endif()

    _FRUT_generate_plist_file(${target} "App" "APPL" "????"
      "${main_plist_entries}" "${bundle_document_types_entries}"
    )
    _FRUT_set_output_directory_properties(${target} "App")
    _FRUT_set_common_target_properties(${target})
    _FRUT_link_osx_frameworks(${target})
    _FRUT_add_xcode_resources(${target})
    _FRUT_set_custom_xcode_flags(${target})

  elseif(JUCER_PROJECT_TYPE STREQUAL "Static Library")
    add_library(${target} STATIC ${all_sources})
    _FRUT_set_output_directory_properties(${target} "Static Library")
    _FRUT_set_common_target_properties(${target})
    _FRUT_set_custom_xcode_flags(${target})

  elseif(JUCER_PROJECT_TYPE STREQUAL "Dynamic Library")
    add_library(${target} SHARED ${all_sources})
    _FRUT_set_output_directory_properties(${target} "Dynamic Library")
    _FRUT_set_common_target_properties(${target})
    _FRUT_set_custom_xcode_flags(${target})

  elseif(JUCER_PROJECT_TYPE STREQUAL "Audio Plug-in")
    if(NOT APPLE AND DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5)
      add_library(${target} MODULE ${all_sources})
      set_target_properties(${target} PROPERTIES PREFIX "")
      _FRUT_set_output_directory_properties(${target} "")
      _FRUT_set_common_target_properties(${target})

      if(JUCER_BUILD_VST3 AND MSVC)
        add_custom_command(TARGET ${target} POST_BUILD
          COMMAND
          "${CMAKE_COMMAND}" "-E" "copy_if_different"
          "$<TARGET_FILE:${target}>"
          "$<TARGET_FILE_DIR:${target}>/${target}.vst3"
        )
      endif()
    else()
      unset(AudioUnit_sources)
      unset(AudioUnitv3_sources)
      unset(AAX_sources)
      unset(RTAS_sources)
      unset(VST_sources)
      unset(VST3_sources)
      unset(Standalone_sources)
      unset(SharedCode_sources)
      foreach(src_file ${JUCER_PROJECT_SOURCES})
        # See XCodeProjectExporter::getTargetTypeFromFilePath()
        # in JUCE/extras/Projucer/Source/Project Saving/jucer_ProjectExport_XCode.h
        if(src_file MATCHES "_AU[._]")
          list(APPEND AudioUnit_sources "${src_file}")
        elseif(src_file MATCHES "_AUv3[._]")
          list(APPEND AudioUnitv3_sources "${src_file}")
        elseif(src_file MATCHES "_AAX[._]")
          list(APPEND AAX_sources "${src_file}")
        elseif(src_file MATCHES "_RTAS[._]")
          list(APPEND RTAS_sources "${src_file}")
        elseif(src_file MATCHES "_VST2[._]")
          list(APPEND VST_sources "${src_file}")
        elseif(src_file MATCHES "_VST3[._]")
          list(APPEND VST3_sources "${src_file}")
        elseif(src_file MATCHES "_Standalone[._]")
          list(APPEND Standalone_sources "${src_file}")
        else()
          list(APPEND SharedCode_sources "${src_file}")
        endif()
      endforeach()

      set(shared_code_target ${target}_Shared_Code)
      add_library(${shared_code_target} STATIC
        ${SharedCode_sources}
        ${JUCER_PROJECT_RESOURCES}
        ${JUCER_PROJECT_XCODE_RESOURCES}
        ${JUCER_PROJECT_BROWSABLE_FILES}
      )
      _FRUT_set_output_directory_properties(${shared_code_target} "Shared Code")
      _FRUT_set_common_target_properties(${shared_code_target})
      target_compile_definitions(${shared_code_target} PRIVATE "JUCE_SHARED_CODE=1")
      _FRUT_set_JucePlugin_Build_defines(${shared_code_target} "SharedCodeTarget")
      _FRUT_set_custom_xcode_flags(${shared_code_target})

      if(JUCER_BUILD_VST)
        set(vst_target ${target}_VST)
        add_library(${vst_target} MODULE
          ${VST_sources}
          ${JUCER_PROJECT_XCODE_RESOURCES}
        )
        target_link_libraries(${vst_target} PRIVATE ${shared_code_target})
        _FRUT_generate_plist_file(${vst_target} "VST" "BNDL" "????"
          "${main_plist_entries}" ""
        )
        _FRUT_set_bundle_properties(${vst_target} "vst")
        _FRUT_set_output_directory_properties(${vst_target} "VST")
        _FRUT_set_common_target_properties(${vst_target})
        if(APPLE)
          _FRUT_install_to_plugin_binary_location(${vst_target} "VST"
            "$ENV{HOME}/Library/Audio/Plug-Ins/VST"
          )
        endif()
        _FRUT_set_JucePlugin_Build_defines(${vst_target} "VSTPlugIn")
        _FRUT_link_osx_frameworks(${vst_target})
        _FRUT_add_xcode_resources(${vst_target})
        _FRUT_set_custom_xcode_flags(${vst_target})
        unset(vst_target)
      endif()

      if(JUCER_BUILD_VST3 AND (APPLE OR MSVC))
        set(vst3_target ${target}_VST3)
        add_library(${vst3_target} MODULE
          ${VST3_sources}
          ${JUCER_PROJECT_XCODE_RESOURCES}
        )
        target_link_libraries(${vst3_target} PRIVATE ${shared_code_target})
        _FRUT_generate_plist_file(${vst3_target} "VST3" "BNDL" "????"
          "${main_plist_entries}" ""
        )
        _FRUT_set_bundle_properties(${vst3_target} "vst3")
        if(MSVC)
          set_property(TARGET ${vst3_target} PROPERTY SUFFIX ".vst3")
        endif()
        _FRUT_set_output_directory_properties(${vst3_target} "VST3")
        _FRUT_set_common_target_properties(${vst3_target})
        if(APPLE)
          _FRUT_install_to_plugin_binary_location(${vst3_target} "VST3"
            "$ENV{HOME}/Library/Audio/Plug-Ins/VST3"
          )
        endif()
        _FRUT_set_JucePlugin_Build_defines(${vst3_target} "VST3PlugIn")
        _FRUT_link_osx_frameworks(${vst3_target})
        _FRUT_add_xcode_resources(${vst3_target})
        _FRUT_set_custom_xcode_flags(${vst3_target})
        unset(vst3_target)
      endif()

      if(JUCER_BUILD_AUDIOUNIT AND APPLE)
        set(au_target ${target}_AU)
        add_library(${au_target} MODULE
          ${AudioUnit_sources}
          ${JUCER_PROJECT_XCODE_RESOURCES}
        )
        target_link_libraries(${au_target} PRIVATE ${shared_code_target})

        _FRUT_get_au_main_type_code(au_main_type_code)
        _FRUT_version_to_dec("${JUCER_PROJECT_VERSION}" dec_version)

        set(audio_components_entries "
    <key>AudioComponents</key>
    <array>
      <dict>
        <key>name</key>
        <string>@JUCER_PLUGIN_MANUFACTURER@: @JUCER_PLUGIN_NAME@</string>
        <key>description</key>
        <string>@JUCER_PLUGIN_DESCRIPTION@</string>
        <key>factoryFunction</key>
        <string>@JUCER_PLUGIN_AU_EXPORT_PREFIX@Factory</string>
        <key>manufacturer</key>
        <string>@JUCER_PLUGIN_MANUFACTURER_CODE@</string>
        <key>type</key>
        <string>${au_main_type_code}</string>
        <key>subtype</key>
        <string>@JUCER_PLUGIN_CODE@</string>
        <key>version</key>
        <integer>${dec_version}</integer>
      </dict>
    </array>"
        )

        _FRUT_generate_plist_file(${au_target} "AU" "BNDL" "????"
          "${main_plist_entries}" "${audio_components_entries}"
        )
        _FRUT_set_bundle_properties(${au_target} "component")
        _FRUT_set_output_directory_properties(${au_target} "AU")
        _FRUT_set_common_target_properties(${au_target})
        _FRUT_install_to_plugin_binary_location(${au_target} "AU"
          "$ENV{HOME}/Library/Audio/Plug-Ins/Components"
        )
        _FRUT_set_JucePlugin_Build_defines(${au_target} "AudioUnitPlugIn")
        _FRUT_link_osx_frameworks(${au_target} "AudioUnit" "CoreAudioKit")
        _FRUT_add_xcode_resources(${au_target})
        _FRUT_set_custom_xcode_flags(${au_target})
        unset(au_target)
      endif()

      if(JUCER_BUILD_AUDIOUNIT_V3 AND APPLE)
        set(auv3_target ${target}_AUv3_AppExtension)
        add_library(${auv3_target} MODULE
          ${AudioUnitv3_sources}
          ${JUCER_PROJECT_XCODE_RESOURCES}
        )
        target_link_libraries(${auv3_target} PRIVATE ${shared_code_target})

        _FRUT_get_au_main_type_code(au_main_type_code)
        _FRUT_version_to_dec("${JUCER_PROJECT_VERSION}" dec_version)
        if(JUCER_PLUGIN_IS_A_SYNTH)
          set(tag "Synth")
        else()
          set(tag "Effects")
        endif()

        set(ns_extension_entries "
    <key>NSExtension</key>
    <dict>
      <key>NSExtensionPrincipalClass</key>
      <string>@JUCER_PLUGIN_AU_EXPORT_PREFIX@FactoryAUv3</string>
      <key>NSExtensionPointIdentifier</key>
      <string>com.apple.AudioUnit-UI</string>
      <key>NSExtensionAttributes</key>
      <dict>
        <key>AudioComponents</key>
        <array>
          <dict>
            <key>name</key>
            <string>@JUCER_PLUGIN_MANUFACTURER@: @JUCER_PLUGIN_NAME@</string>
            <key>description</key>
            <string>@JUCER_PLUGIN_DESCRIPTION@</string>
            <key>factoryFunction</key>
            <string>@JUCER_PLUGIN_AU_EXPORT_PREFIX@FactoryAUv3</string>
            <key>manufacturer</key>
            <string>@JUCER_PLUGIN_MANUFACTURER_CODE@</string>
            <key>type</key>
            <string>${au_main_type_code}</string>
            <key>subtype</key>
            <string>@JUCER_PLUGIN_CODE@</string>
            <key>version</key>
            <integer>${dec_version}</integer>
            <key>sandboxSafe</key>
            <true/>
            <key>tags</key>
            <array>
              <string>${tag}</string>
            </array>
          </dict>
        </array>
      </dict>
    </dict>"
        )

        _FRUT_generate_plist_file(${auv3_target} "AUv3_AppExtension" "XPC!" "????"
          "${main_plist_entries}" "${ns_extension_entries}"
        )

        # com.yourcompany.NewProject -> com.yourcompany.NewProject.NewProjectAUv3
        string(REPLACE "." ";" bundle_id_parts "${JUCER_BUNDLE_IDENTIFIER}")
        list(LENGTH bundle_id_parts bundle_id_parts_length)
        math(EXPR bundle_id_parts_last_index "${bundle_id_parts_length} - 1")
        list(GET bundle_id_parts ${bundle_id_parts_last_index} bundle_id_last_part)
        list(APPEND bundle_id_parts "${bundle_id_last_part}AUv3")
        string(REPLACE ";" "." bundle_id "${bundle_id_parts}")
        if(CMAKE_GENERATOR STREQUAL "Xcode")
          set_target_properties(${auv3_target} PROPERTIES
            XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "${bundle_id}"
          )
        else()
          set_target_properties(${auv3_target} PROPERTIES
            MACOSX_BUNDLE_GUI_IDENTIFIER "${bundle_id}"
          )
        endif()

        # Cannot use _FRUT_set_bundle_properties() since Projucer sets xcodeIsBundle=false
        # for this target, though it is a bundle...
        set_target_properties(${auv3_target} PROPERTIES
          BUNDLE TRUE
          BUNDLE_EXTENSION "appex"
          XCODE_ATTRIBUTE_WRAPPER_EXTENSION "appex"
        )
        _FRUT_set_output_directory_properties(${auv3_target} "AUv3 AppExtension")
        _FRUT_set_common_target_properties(${auv3_target})
        _FRUT_set_JucePlugin_Build_defines(${auv3_target} "AudioUnitv3PlugIn")
        _FRUT_link_osx_frameworks(
          ${auv3_target} "AudioUnit" "CoreAudioKit" "AVFoundation"
        )
        _FRUT_add_xcode_resources(${auv3_target})
        _FRUT_set_custom_xcode_flags(${auv3_target})
        unset(auv3_target)
      endif()

      if(JUCER_BUILD_AUDIOUNIT_V3
          AND DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5)
        set(juce4_standalone ON)
      endif()

      if(juce4_standalone OR (JUCER_BUILD_STANDALONE_PLUGIN
          AND NOT (DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5)))
        if(juce4_standalone)
          set(standalone_target ${target}_AUv3_Standalone)
        else()
          set(standalone_target ${target}_StandalonePlugin)
        endif()
        add_executable(${standalone_target} WIN32 MACOSX_BUNDLE
          ${Standalone_sources}
          ${JUCER_PROJECT_XCODE_RESOURCES}
        )
        target_link_libraries(${standalone_target} PRIVATE ${shared_code_target})
        if(juce4_standalone)
          _FRUT_generate_plist_file(${standalone_target} "AUv3_Standalone" "APPL" "????"
            "${main_plist_entries}" ""
          )
        else()
          _FRUT_generate_plist_file(${standalone_target} "Standalone_Plugin" "APPL" "????"
            "${main_plist_entries}" ""
          )
        endif()
        _FRUT_set_output_directory_properties(${standalone_target} "Standalone Plugin")
        _FRUT_set_common_target_properties(${standalone_target})
        _FRUT_set_JucePlugin_Build_defines(${standalone_target} "StandalonePlugIn")
        _FRUT_link_osx_frameworks(${standalone_target})
        _FRUT_add_xcode_resources(${standalone_target})
        if(TARGET ${target}_AUv3_AppExtension)
          add_dependencies(${standalone_target} ${target}_AUv3_AppExtension)
          install(TARGETS ${target}_AUv3_AppExtension
            COMPONENT _embed_app_extension_in_standalone_app
            DESTINATION "$<TARGET_FILE_DIR:${standalone_target}>/../PlugIns"
          )
          add_custom_command(TARGET ${standalone_target} POST_BUILD
            COMMAND
            "${CMAKE_COMMAND}"
            "-DCMAKE_INSTALL_CONFIG_NAME=$<CONFIG>"
            "-DCMAKE_INSTALL_COMPONENT=_embed_app_extension_in_standalone_app"
            "-P" "${CMAKE_CURRENT_BINARY_DIR}/cmake_install.cmake"
          )
        endif()
        _FRUT_set_custom_xcode_flags(${standalone_target})
        unset(standalone_target)
      endif()
    endif()

  else()
    message(FATAL_ERROR "Unknown project type: ${JUCER_PROJECT_TYPE}")

  endif()

endfunction()


function(_FRUT_abs_path_based_on_jucer_project_dir in_path out_path)

  if(NOT IS_ABSOLUTE "${in_path}" AND NOT DEFINED JUCER_PROJECT_DIR)
    message(FATAL_ERROR "The path \"${in_path}\" must be absolute, unless you give "
      "PROJECT_FILE when calling jucer_project_begin()."
    )
  endif()

  get_filename_component(in_path "${in_path}" ABSOLUTE BASE_DIR "${JUCER_PROJECT_DIR}")
  set(${out_path} ${in_path} PARENT_SCOPE)

endfunction()


function(_FRUT_generate_AppConfig_header)

  if(DEFINED JUCER_APPCONFIG_USER_CODE_SECTION)
    set(user_code_section "\n${JUCER_APPCONFIG_USER_CODE_SECTION}\n")
    if("${user_code_section}" STREQUAL "\n\n")
      set(user_code_section "\n")
    endif()
  else()
    string(CONCAT user_code_section "\n\n// (You can call jucer_appconfig_header() to "
      "add your own code to this section)\n\n"
    )
  endif()

  if(DEFINED JUCER_DISPLAY_THE_JUCE_SPLASH_SCREEN
      AND NOT JUCER_DISPLAY_THE_JUCE_SPLASH_SCREEN)
    set(display_splash_screen 0)
  else()
    set(display_splash_screen 1)
  endif()
  if(DEFINED JUCER_REPORT_JUCE_APP_USAGE AND NOT JUCER_REPORT_JUCE_APP_USAGE)
    set(report_app_usage 0)
  else()
    set(report_app_usage 1)
  endif()
  if(DEFINED JUCER_SPLASH_SCREEN_COLOUR
      AND NOT JUCER_SPLASH_SCREEN_COLOUR STREQUAL "Dark")
    set(use_dark_splash_screen 0)
  else()
    set(use_dark_splash_screen 1)
  endif()

  set(max_right_padding 0)
  foreach(module_name ${JUCER_PROJECT_MODULES})
    string(LENGTH "${module_name}" module_name_length)
    if(module_name_length GREATER max_right_padding)
      set(max_right_padding ${module_name_length})
    endif()
  endforeach()
  math(EXPR max_right_padding "${max_right_padding} + 5")

  unset(module_available_defines)
  unset(config_flags_defines)
  foreach(module_name ${JUCER_PROJECT_MODULES})
    string(LENGTH "${module_name}" right_padding)
    unset(padding_spaces)
    while(right_padding LESS max_right_padding)
      string(APPEND padding_spaces " ")
      math(EXPR right_padding "${right_padding} + 1")
    endwhile()
    string(APPEND module_available_defines
      "#define JUCE_MODULE_AVAILABLE_${module_name}${padding_spaces} 1\n"
    )

    if(DEFINED JUCER_${module_name}_CONFIG_FLAGS)
      string(APPEND config_flags_defines
        "//=============================================================================="
        "\n// ${module_name} flags:\n\n"
      )
    endif()
    foreach(config_flag ${JUCER_${module_name}_CONFIG_FLAGS})
      string(APPEND config_flags_defines "#ifndef    ${config_flag}\n")
      if(NOT DEFINED JUCER_FLAG_${config_flag})
        string(APPEND config_flags_defines " //#define ${config_flag}\n")
      elseif(JUCER_FLAG_${config_flag})
        string(APPEND config_flags_defines " #define   ${config_flag} 1\n")
      else()
        string(APPEND config_flags_defines " #define   ${config_flag} 0\n")
      endif()
      string(APPEND config_flags_defines "#endif\n\n")
    endforeach()
  endforeach()

  set(is_standalone_application 1)

  if(JUCER_PROJECT_TYPE STREQUAL "Dynamic Library")
    set(is_standalone_application 0)

  elseif(JUCER_PROJECT_TYPE STREQUAL "Audio Plug-in")
    set(is_standalone_application 0)

    # See ProjectSaver::writePluginCharacteristicsFile()
    # in JUCE/extras/Projucer/Source/Project Saving/jucer_ProjectSaver.cpp

    set(audio_plugin_setting_names
      "Build_VST" "Build_VST3" "Build_AU" "Build_AUv3" "Build_RTAS" "Build_AAX"
      "Build_STANDALONE"
      "Name" "Desc" "Manufacturer" "ManufacturerWebsite" "ManufacturerEmail"
      "ManufacturerCode" "PluginCode"
      "IsSynth" "WantsMidiInput" "ProducesMidiOutput" "IsMidiEffect"
      "EditorRequiresKeyboardFocus"
      "Version" "VersionCode" "VersionString"
      "VSTUniqueID" "VSTCategory"
      "AUMainType" "AUSubType" "AUExportPrefix" "AUExportPrefixQuoted"
      "AUManufacturerCode"
      "CFBundleIdentifier"
      "RTASCategory" "RTASManufacturerCode" "RTASProductId" "RTASDisableBypass"
      "RTASDisableMultiMono"
      "AAXIdentifier" "AAXManufacturerCode" "AAXProductId" "AAXCategory"
      "AAXDisableBypass" "AAXDisableMultiMono"
    )

    _FRUT_bool_to_int("${JUCER_BUILD_VST}" Build_VST_value)
    _FRUT_bool_to_int("${JUCER_BUILD_VST3}" Build_VST3_value)
    _FRUT_bool_to_int("${JUCER_BUILD_AUDIOUNIT}" Build_AU_value)
    _FRUT_bool_to_int("${JUCER_BUILD_AUDIOUNIT_V3}" Build_AUv3_value)
    _FRUT_bool_to_int("OFF" Build_RTAS_value) # Not yet supported
    _FRUT_bool_to_int("OFF" Build_AAX_value) # Not yet supported
    if(DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5)
      _FRUT_bool_to_int("${JUCER_BUILD_AUDIOUNIT_V3}" Build_STANDALONE_value)
    else()
      _FRUT_bool_to_int("${JUCER_BUILD_STANDALONE_PLUGIN}" Build_STANDALONE_value)
    endif()

    set(Name_value "\"${JUCER_PLUGIN_NAME}\"")
    set(Desc_value "\"${JUCER_PLUGIN_DESCRIPTION}\"")
    set(Manufacturer_value "\"${JUCER_PLUGIN_MANUFACTURER}\"")
    set(ManufacturerWebsite_value "\"${JUCER_COMPANY_WEBSITE}\"")
    set(ManufacturerEmail_value "\"${JUCER_COMPANY_EMAIL}\"")

    _FRUT_four_chars_to_hex("${JUCER_PLUGIN_MANUFACTURER_CODE}" hex_manufacturer_code)
    set(ManufacturerCode_value
      "${hex_manufacturer_code} // '${JUCER_PLUGIN_MANUFACTURER_CODE}'"
    )
    _FRUT_four_chars_to_hex("${JUCER_PLUGIN_CODE}" hex_plugin_code)
    set(PluginCode_value "${hex_plugin_code} // '${JUCER_PLUGIN_CODE}'")

    _FRUT_bool_to_int("${JUCER_PLUGIN_IS_A_SYNTH}" IsSynth_value)
    _FRUT_bool_to_int("${JUCER_PLUGIN_MIDI_INPUT}" WantsMidiInput_value)
    _FRUT_bool_to_int("${JUCER_PLUGIN_MIDI_OUTPUT}" ProducesMidiOutput_value)
    _FRUT_bool_to_int("${JUCER_MIDI_EFFECT_PLUGIN}" IsMidiEffect_value)
    _FRUT_bool_to_int("${JUCER_KEY_FOCUS}" EditorRequiresKeyboardFocus_value)

    set(Version_value "${JUCER_PROJECT_VERSION}")
    _FRUT_version_to_hex("${JUCER_PROJECT_VERSION}" VersionCode_value)
    set(VersionString_value "\"${JUCER_PROJECT_VERSION}\"")

    set(VSTUniqueID_value "JucePlugin_PluginCode")
    if(NOT DEFINED JUCER_VST_CATEGORY)
      if(JUCER_PLUGIN_IS_A_SYNTH)
        set(VSTCategory_value "kPlugCategSynth")
      else()
        set(VSTCategory_value "kPlugCategEffect")
      endif()
    else()
      set(VSTCategory_value "${JUCER_VST_CATEGORY}")
    endif()

    if(NOT DEFINED JUCER_PLUGIN_AU_MAIN_TYPE)
      if(JUCER_MIDI_EFFECT_PLUGIN)
        set(AUMainType_value "'aumi'")
      elseif(JUCER_PLUGIN_IS_A_SYNTH)
        set(AUMainType_value "kAudioUnitType_MusicDevice")
      elseif(JUCER_PLUGIN_MIDI_INPUT)
        set(AUMainType_value "kAudioUnitType_MusicEffect")
      else()
        set(AUMainType_value "kAudioUnitType_Effect")
      endif()
    else()
      set(AUMainType_value "${JUCER_PLUGIN_AU_MAIN_TYPE}")
    endif()
    set(AUSubType_value "JucePlugin_PluginCode")
    set(AUExportPrefix_value "${JUCER_PLUGIN_AU_EXPORT_PREFIX}")
    set(AUExportPrefixQuoted_value "\"${JUCER_PLUGIN_AU_EXPORT_PREFIX}\"")
    set(AUManufacturerCode_value "JucePlugin_ManufacturerCode")

    set(CFBundleIdentifier_value "${JUCER_BUNDLE_IDENTIFIER}")

    if(JUCER_PLUGIN_IS_A_SYNTH)
      set(RTASCategory_value "ePlugInCategory_SWGenerators")
    elseif(NOT DEFINED JUCER_PLUGIN_RTAS_CATEGORY)
      set(RTASCategory_value "ePlugInCategory_None")
    else()
      set(RTASCategory_value "${JUCER_PLUGIN_RTAS_CATEGORY}")
    endif()
    set(RTASManufacturerCode_value "JucePlugin_ManufacturerCode")
    set(RTASProductId_value "JucePlugin_PluginCode")
    _FRUT_bool_to_int("${JUCER_PLUGIN_RTAS_DISABLE_BYPASS}" RTASDisableBypass_value)
    _FRUT_bool_to_int("${JUCER_PLUGIN_RTAS_DISABLE_MULTI_MONO}"
      RTASDisableMultiMono_value
    )

    set(AAXIdentifier_value "${JUCER_PLUGIN_AAX_IDENTIFIER}")
    set(AAXManufacturerCode_value "JucePlugin_ManufacturerCode")
    set(AAXProductId_value "JucePlugin_PluginCode")
    set(AAXCategory_value "${JUCER_PLUGIN_AAX_CATEGORY}")
    _FRUT_bool_to_int("${JUCER_PLUGIN_AAX_DISABLE_BYPASS}" AAXDisableBypass_value)
    _FRUT_bool_to_int("${JUCER_PLUGIN_AAX_DISABLE_MULTI_MONO}" AAXDisableMultiMono_value)

    string(LENGTH "${JUCER_PLUGIN_CHANNEL_CONFIGURATIONS}" plugin_channel_config_length)
    if(plugin_channel_config_length GREATER 0)
      # See countMaxPluginChannels()
      # in JUCE/extras/Projucer/Source/Project Saving/jucer_ProjectSaver.cpp
      string(REGEX REPLACE "[, {}]" ";" configs "${JUCER_PLUGIN_CHANNEL_CONFIGURATIONS}")
      set(max_num_input 0)
      set(max_num_output 0)
      set(is_input TRUE)
      foreach(element ${configs})
        if(is_input)
          if(element GREATER max_num_input)
            set(max_num_input "${element}")
          endif()
          set(is_input FALSE)
        else()
          if(element GREATER max_num_output)
            set(max_num_output "${element}")
          endif()
          set(is_input TRUE)
        endif()
      endforeach()

      list(APPEND audio_plugin_setting_names
        "MaxNumInputChannels" "MaxNumOutputChannels" "PreferredChannelConfigurations"
      )
      set(MaxNumInputChannels_value "${max_num_input}")
      set(MaxNumOutputChannels_value "${max_num_output}")
      set(PreferredChannelConfigurations_value "${JUCER_PLUGIN_CHANNEL_CONFIGURATIONS}")
    endif()

    string(CONCAT audio_plugin_settings_defines
      "\n"
      "//==============================================================================\n"
      "// Audio plugin settings..\n\n"
    )

    foreach(setting_name ${audio_plugin_setting_names})
      string(LENGTH "JucePlugin_${setting_name}" right_padding)
      unset(padding_spaces)
      while(right_padding LESS 32)
        string(APPEND padding_spaces " ")
        math(EXPR right_padding "${right_padding} + 1")
      endwhile()

      string(APPEND audio_plugin_settings_defines
        "#ifndef  JucePlugin_${setting_name}\n"
        " #define JucePlugin_${setting_name}${padding_spaces}  ${${setting_name}_value}\n"
        "#endif\n"
      )
    endforeach()
  endif()

  if(DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5)
    string(TOUPPER "${JUCER_PROJECT_ID}" upper_project_id)
    set(template_file "${Reprojucer_templates_DIR}/AppConfig-4.h")
  else()
    set(template_file "${Reprojucer_templates_DIR}/AppConfig.h")
  endif()
  configure_file("${template_file}" "JuceLibraryCode/AppConfig.h")
  list(APPEND JUCER_PROJECT_SOURCES
    "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode/AppConfig.h"
  )

  set(JUCER_PROJECT_SOURCES ${JUCER_PROJECT_SOURCES} PARENT_SCOPE)

endfunction()


function(_FRUT_generate_JuceHeader_header)

  list(LENGTH JUCER_PROJECT_RESOURCES resources_count)
  if(resources_count GREATER 0)
    set(BinaryDataBuilder_version "0.1.0")
    find_program(BinaryDataBuilder_exe "BinaryDataBuilder-${BinaryDataBuilder_version}"
      PATHS "${Reprojucer.cmake_DIR}/bin"
      NO_DEFAULT_PATH
    )
    if(NOT BinaryDataBuilder_exe)
      message(STATUS "Building BinaryDataBuilder")
      try_compile(BinaryDataBuilder
        "${Reprojucer.cmake_DIR}/BinaryDataBuilder/_build/${CMAKE_GENERATOR}"
        "${Reprojucer.cmake_DIR}/BinaryDataBuilder"
        BinaryDataBuilder install
        CMAKE_FLAGS
        "-DJUCE_modules_DIRS=${JUCER_PROJECT_MODULES_FOLDERS}"
        "-DCMAKE_INSTALL_PREFIX=${Reprojucer.cmake_DIR}/bin"
      )
      if(NOT BinaryDataBuilder)
        message(FATAL_ERROR "Failed to build BinaryDataBuilder")
      endif()
      message(STATUS "BinaryDataBuilder has been successfully built")
      find_program(BinaryDataBuilder_exe "BinaryDataBuilder-${BinaryDataBuilder_version}"
        PATHS "${Reprojucer.cmake_DIR}/bin"
        NO_DEFAULT_PATH
      )
      if(NOT BinaryDataBuilder_exe)
        message(FATAL_ERROR
          "Could not find BinaryDataBuilder-${BinaryDataBuilder_version}"
        )
      endif()
    endif()

    if(NOT JUCER_PROJECT_ID)
      set(project_uid "FRUT")
    else()
      set(project_uid "${JUCER_PROJECT_ID}")
    endif()
    if(NOT DEFINED JUCER_BINARYDATACPP_SIZE_LIMIT)
      set(JUCER_BINARYDATACPP_SIZE_LIMIT 10240)
    endif()
    math(EXPR size_limit_in_bytes "${JUCER_BINARYDATACPP_SIZE_LIMIT} * 1024")
    if(NOT DEFINED size_limit_in_bytes)
      message(FATAL_ERROR "Error when computing size_limit_in_bytes = "
        "${JUCER_BINARYDATACPP_SIZE_LIMIT} * 1024"
      )
    endif()
    if("${JUCER_BINARYDATA_NAMESPACE}" STREQUAL "")
      set(JUCER_BINARYDATA_NAMESPACE "BinaryData")
    endif()
    set(BinaryDataBuilder_args
      "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode/"
      "${project_uid}"
      ${size_limit_in_bytes}
      "${JUCER_BINARYDATA_NAMESPACE}"
    )
    foreach(resource_path ${JUCER_PROJECT_RESOURCES})
      get_filename_component(resource_abs_path "${resource_path}" ABSOLUTE)
      list(APPEND BinaryDataBuilder_args "${resource_abs_path}")
    endforeach()
    execute_process(
      COMMAND "${BinaryDataBuilder_exe}" ${BinaryDataBuilder_args}
      OUTPUT_VARIABLE binary_data_filenames
      RESULT_VARIABLE BinaryDataBuilder_return_code
    )
    if(NOT BinaryDataBuilder_return_code EQUAL 0)
      message(FATAL_ERROR "Error when executing BinaryDataBuilder")
    endif()

    foreach(filename ${binary_data_filenames})
      list(APPEND JUCER_PROJECT_SOURCES
        "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode/${filename}"
      )
    endforeach()
    set(binary_data_include "#include \"BinaryData.h\"")
  endif()

  unset(modules_includes)
  foreach(module_name ${JUCER_PROJECT_MODULES})
    string(APPEND modules_includes "#include <${module_name}/${module_name}.h>\n")
  endforeach()

  if(DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5)
    string(TOUPPER "${JUCER_PROJECT_ID}" upper_project_id)
    set(template_file "${Reprojucer_templates_DIR}/JuceHeader-4.h")
  else()
    set(template_file "${Reprojucer_templates_DIR}/JuceHeader.h")
  endif()
  configure_file("${template_file}" "JuceLibraryCode/JuceHeader.h")
  list(APPEND JUCER_PROJECT_SOURCES
    "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode/JuceHeader.h"
  )

  set(JUCER_PROJECT_SOURCES ${JUCER_PROJECT_SOURCES} PARENT_SCOPE)

endfunction()


function(_FRUT_generate_icon_file icon_format out_icon_filename)

  set(IconBuilder_version "0.1.0")
  find_program(IconBuilder_exe "IconBuilder-${IconBuilder_version}"
    PATHS "${Reprojucer.cmake_DIR}/bin"
    NO_DEFAULT_PATH
  )
  if(NOT IconBuilder_exe)
    message(STATUS "Building IconBuilder")
    try_compile(IconBuilder
      "${Reprojucer.cmake_DIR}/IconBuilder/_build/${CMAKE_GENERATOR}"
      "${Reprojucer.cmake_DIR}/IconBuilder"
      IconBuilder install
      CMAKE_FLAGS
      "-DJUCE_modules_DIRS=${JUCER_PROJECT_MODULES_FOLDERS}"
      "-DCMAKE_INSTALL_PREFIX=${Reprojucer.cmake_DIR}/bin"
    )
    if(NOT IconBuilder)
      message(FATAL_ERROR "Failed to build IconBuilder")
    endif()
    message(STATUS "IconBuilder has been successfully built")
    find_program(IconBuilder_exe "IconBuilder-${IconBuilder_version}"
      PATHS "${Reprojucer.cmake_DIR}/bin"
      NO_DEFAULT_PATH
    )
    if(NOT IconBuilder_exe)
      message(FATAL_ERROR "Could not find IconBuilder-${IconBuilder_version}")
    endif()
  endif()

  set(IconBuilder_args "${icon_format}" "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode/")
  if(DEFINED JUCER_SMALL_ICON)
    list(APPEND IconBuilder_args "${JUCER_SMALL_ICON}")
  else()
    list(APPEND IconBuilder_args "<None>")
  endif()
  if(DEFINED JUCER_LARGE_ICON)
    list(APPEND IconBuilder_args "${JUCER_LARGE_ICON}")
  else()
    list(APPEND IconBuilder_args "<None>")
  endif()

  execute_process(
    COMMAND "${IconBuilder_exe}" ${IconBuilder_args}
    OUTPUT_VARIABLE icon_filename
    RESULT_VARIABLE IconBuilder_return_code
  )
  if(NOT IconBuilder_return_code EQUAL 0)
    message(FATAL_ERROR "Error when executing IconBuilder")
  endif()

  if(NOT "${icon_filename}" STREQUAL "")
    set(${out_icon_filename} ${icon_filename} PARENT_SCOPE)

    list(APPEND JUCER_PROJECT_SOURCES
      "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode/${icon_filename}"
    )
    set(JUCER_PROJECT_SOURCES ${JUCER_PROJECT_SOURCES} PARENT_SCOPE)
  endif()

endfunction()


function(_FRUT_set_output_directory_properties target subfolder)

  foreach(config ${JUCER_PROJECT_CONFIGURATIONS})
    string(TOUPPER "${config}" upper_config)
    unset(output_directory)

    if(MSVC AND NOT "${subfolder}" STREQUAL ""
        AND NOT (DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5))
      if(DEFINED JUCER_BINARY_LOCATION_${config})
        set(output_directory "${JUCER_BINARY_LOCATION_${config}}/${subfolder}")
      else()
        set(output_directory "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${subfolder}")
      endif()
    elseif(DEFINED JUCER_BINARY_LOCATION_${config})
      set(output_directory "${JUCER_BINARY_LOCATION_${config}}")
    endif()
    if(output_directory)
      set_target_properties(${target} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY_${upper_config} "${output_directory}"
        LIBRARY_OUTPUT_DIRECTORY_${upper_config} "${output_directory}"
        RUNTIME_OUTPUT_DIRECTORY_${upper_config} "${output_directory}"
      )
    endif()
  endforeach()

endfunction()


function(_FRUT_set_common_target_properties target)

  foreach(config ${JUCER_PROJECT_CONFIGURATIONS})
    string(TOUPPER "${config}" upper_config)

    if(JUCER_BINARY_NAME_${config})
      set(output_name "${JUCER_BINARY_NAME_${config}}")
    else()
      set(output_name "${JUCER_PROJECT_NAME}")
    endif()
    set_target_properties(${target} PROPERTIES
      OUTPUT_NAME_${upper_config} "${output_name}"
    )
  endforeach()

  target_include_directories(${target} PRIVATE
    "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode"
    ${JUCER_PROJECT_MODULES_FOLDERS}
  )
  foreach(config ${JUCER_PROJECT_CONFIGURATIONS})
    set(search_paths ${JUCER_HEADER_SEARCH_PATHS_${config}})
    target_include_directories(${target} PRIVATE $<$<CONFIG:${config}>:${search_paths}>)
  endforeach()
  target_include_directories(${target} PRIVATE ${JUCER_HEADER_SEARCH_PATHS})

  if(JUCER_BUILD_VST OR JUCER_FLAG_JUCE_PLUGINHOST_VST)
    if(DEFINED JUCER_VST_SDK_FOLDER)
      if(NOT IS_DIRECTORY "${JUCER_VST_SDK_FOLDER}")
        message(WARNING
          "JUCER_VST_SDK_FOLDER: no such directory \"${JUCER_VST_SDK_FOLDER}\""
        )
      elseif(NOT EXISTS "${JUCER_VST_SDK_FOLDER}/public.sdk/source/vst2.x/audioeffectx.h")
        message(WARNING "JUCER_VST_SDK_FOLDER: \"${JUCER_VST_SDK_FOLDER}\" doesn't seem "
          "to contain the VST SDK"
        )
      endif()
      target_include_directories(${target} PRIVATE "${JUCER_VST_SDK_FOLDER}")
    elseif(DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 4.2.4)
      message(WARNING "JUCER_VST_SDK_FOLDER is not defined. You should give "
        "VST_SDK_FOLDER when calling jucer_export_target()."
      )
    endif()
  endif()

  if(JUCER_BUILD_VST3 OR JUCER_FLAG_JUCE_PLUGINHOST_VST3)
    if(DEFINED JUCER_VST3_SDK_FOLDER)
      if(NOT IS_DIRECTORY "${JUCER_VST3_SDK_FOLDER}")
        message(WARNING
          "JUCER_VST3_SDK_FOLDER: no such directory \"${JUCER_VST3_SDK_FOLDER}\""
        )
      elseif(NOT EXISTS "${JUCER_VST3_SDK_FOLDER}/base/source/baseiids.cpp")
        message(WARNING "JUCER_VST3_SDK_FOLDER: \"${JUCER_VST3_SDK_FOLDER}\" doesn't "
          "seem to contain the VST3 SDK"
        )
      endif()
      target_include_directories(${target} PRIVATE "${JUCER_VST3_SDK_FOLDER}")
    elseif(APPLE OR MSVC)
      message(WARNING "JUCER_VST3_SDK_FOLDER is not defined. You should give "
        "VST3_SDK_FOLDER when calling jucer_export_target()."
      )
    endif()
  endif()

  foreach(config ${JUCER_PROJECT_CONFIGURATIONS})
    if(JUCER_OPTIMISATION_FLAG_${config})
      set(optimisation_flag ${JUCER_OPTIMISATION_FLAG_${config}})
      target_compile_options(${target} PRIVATE
        $<$<CONFIG:${config}>:${optimisation_flag}>
      )
    endif()
  endforeach()

  target_compile_definitions(${target} PRIVATE
    ${JUCER_PREPROCESSOR_DEFINITIONS}
    ${JUCER_EXTRA_PREPROCESSOR_DEFINITIONS}
  )
  foreach(config ${JUCER_PROJECT_CONFIGURATIONS})
    set(definitions ${JUCER_PREPROCESSOR_DEFINITIONS_${config}})
    target_compile_definitions(${target} PRIVATE $<$<CONFIG:${config}>:${definitions}>)
  endforeach()

  target_link_libraries(${target} PRIVATE ${JUCER_EXTERNAL_LIBRARIES_TO_LINK})

  if(APPLE)
    set_target_properties(${target} PROPERTIES CXX_EXTENSIONS OFF)
    set_target_properties(${target} PROPERTIES CXX_STANDARD 11)

    unset(all_confs_cxx_language_standard)
    unset(config_to_value)
    foreach(config ${JUCER_PROJECT_CONFIGURATIONS})
      if(DEFINED JUCER_CXX_LANGUAGE_STANDARD_${config})
        list(APPEND all_confs_cxx_language_standard
          ${JUCER_CXX_LANGUAGE_STANDARD_${config}}
        )
        string(APPEND config_to_value "  ${config}: "
          "\"${JUCER_CXX_LANGUAGE_STANDARD_${config}}\"\n"
        )
      endif()
    endforeach()
    if(all_confs_cxx_language_standard)
      list(GET all_confs_cxx_language_standard 0 cxx_language_standard)
      list(REMOVE_DUPLICATES all_confs_cxx_language_standard)
      list(LENGTH all_confs_cxx_language_standard all_confs_cxx_language_standard_length)
      if(NOT all_confs_cxx_language_standard_length EQUAL 1)
        message(STATUS "Different values for CXX_LANGUAGE_STANDARD:\n${config_to_value}"
          "Falling back to the first value: \"${cxx_language_standard}\"."
        )
      endif()
      if(cxx_language_standard MATCHES "^GNU\\+\\+")
        set_target_properties(${target} PROPERTIES CXX_EXTENSIONS ON)
      elseif(cxx_language_standard MATCHES "^C\\+\\+")
        set_target_properties(${target} PROPERTIES CXX_EXTENSIONS OFF)
      endif()
      if(cxx_language_standard MATCHES "98$")
        set_target_properties(${target} PROPERTIES CXX_STANDARD 98)
      elseif(cxx_language_standard MATCHES "11$")
        set_target_properties(${target} PROPERTIES CXX_STANDARD 11)
      elseif(cxx_language_standard MATCHES "14$")
        set_target_properties(${target} PROPERTIES CXX_STANDARD 14)
      endif()
    endif()

    foreach(config ${JUCER_PROJECT_CONFIGURATIONS})
      if(${JUCER_CONFIGURATION_IS_DEBUG_${config}})
        target_compile_definitions(${target} PRIVATE
          $<$<CONFIG:${config}>:_DEBUG=1>
          $<$<CONFIG:${config}>:DEBUG=1>
        )
      else()
        target_compile_definitions(${target} PRIVATE
          $<$<CONFIG:${config}>:_NDEBUG=1>
          $<$<CONFIG:${config}>:NDEBUG=1>
        )
      endif()

      if(DEFINED JUCER_CXX_LIBRARY_${config})
        set(cxx_library ${JUCER_CXX_LIBRARY_${config}})
        target_compile_options(${target} PRIVATE
          $<$<CONFIG:${config}>:-stdlib=${cxx_library}>
        )
      endif()

      if(JUCER_RELAX_IEEE_COMPLIANCE_${config})
        target_compile_options(${target} PRIVATE $<$<CONFIG:${config}>:-ffast-math>)
      endif()

      if(JUCER_LINK_TIME_OPTIMISATION_${config})
        target_compile_options(${target} PRIVATE $<$<CONFIG:${config}>:-flto>)
      endif()

      if(DEFINED JUCER_OSX_ARCHITECTURES_${config})
        string(TOUPPER "${config}" upper_config)
        set_target_properties(${target} PROPERTIES
          OSX_ARCHITECTURES_${upper_config} "${JUCER_OSX_ARCHITECTURES_${config}}"
        )
      endif()

      foreach(path ${JUCER_EXTRA_LIBRARY_SEARCH_PATHS_${config}})
        target_link_libraries(${target} PRIVATE $<$<CONFIG:${config}>:-L${path}>)
      endforeach()
    endforeach()

    unset(all_confs_code_sign_identity)
    foreach(config ${JUCER_PROJECT_CONFIGURATIONS})
      if(NOT JUCER_CODE_SIGNING_IDENTITY_${config} STREQUAL "Mac Developer")
        set(code_sign_identity ${JUCER_CODE_SIGNING_IDENTITY_${config}})
        string(APPEND all_confs_code_sign_identity
          $<$<CONFIG:${config}>:${code_sign_identity}>
        )
      endif()
    endforeach()
    if(all_confs_code_sign_identity)
      set_target_properties(${target} PROPERTIES
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "${all_confs_code_sign_identity}"
      )
    endif()

    get_target_property(target_type ${target} TYPE)
    unset(all_confs_strip_exe)
    unset(all_confs_strip_opt)
    unset(all_confs_strip_arg)
    foreach(config ${JUCER_PROJECT_CONFIGURATIONS})
      if(target_type STREQUAL "EXECUTABLE" OR target_type STREQUAL "MODULE_LIBRARY")
        if(${JUCER_STRIP_LOCAL_SYMBOLS_${config}})
          find_program(strip_exe "strip")
          if(NOT strip_exe)
            message(FATAL_ERROR "Could not find strip program")
          endif()
          string(APPEND all_confs_strip_exe $<$<CONFIG:${config}>:${strip_exe}>)
          string(APPEND all_confs_strip_opt $<$<CONFIG:${config}>:-x>)
          string(APPEND all_confs_strip_arg
            $<$<CONFIG:${config}>:$<TARGET_FILE:${target}>>
          )
        endif()
      endif()
    endforeach()
    if(all_confs_strip_exe)
      add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${all_confs_strip_exe} ${all_confs_strip_opt} ${all_confs_strip_arg}
      )
    endif()

    if(DEFINED JUCER_PREBUILD_SHELL_SCRIPT)
      if(NOT DEFINED JUCER_TARGET_PROJECT_FOLDER)
        message(FATAL_ERROR "JUCER_TARGET_PROJECT_FOLDER must be defined. Give "
          "TARGET_PROJECT_FOLDER when calling jucer_export_target(\"Xcode (MacOSX)\")."
        )
      endif()
      if(NOT IS_DIRECTORY "${JUCER_TARGET_PROJECT_FOLDER}")
        file(MAKE_DIRECTORY "${JUCER_TARGET_PROJECT_FOLDER}")
      endif()
      add_custom_command(TARGET ${target} PRE_BUILD
        COMMAND "/bin/sh" "${JUCER_PREBUILD_SHELL_SCRIPT}"
        WORKING_DIRECTORY "${JUCER_TARGET_PROJECT_FOLDER}"
      )
    endif()

    if(DEFINED JUCER_POSTBUILD_SHELL_SCRIPT)
      if(NOT DEFINED JUCER_TARGET_PROJECT_FOLDER)
        message(FATAL_ERROR "JUCER_TARGET_PROJECT_FOLDER must be defined. Give "
          "TARGET_PROJECT_FOLDER when calling jucer_export_target(\"Xcode (MacOSX)\")."
        )
      endif()
      if(NOT IS_DIRECTORY "${JUCER_TARGET_PROJECT_FOLDER}")
        file(MAKE_DIRECTORY "${JUCER_TARGET_PROJECT_FOLDER}")
      endif()
      add_custom_command(TARGET ${target} POST_BUILD
        COMMAND "/bin/sh" "${JUCER_POSTBUILD_SHELL_SCRIPT}"
        WORKING_DIRECTORY "${JUCER_TARGET_PROJECT_FOLDER}"
      )
    endif()

  elseif(MSVC)
    target_compile_definitions(${target} PRIVATE "_CRT_SECURE_NO_WARNINGS")
    target_compile_options(${target} PRIVATE "/MP")

    foreach(config ${JUCER_PROJECT_CONFIGURATIONS})
      if(${JUCER_CONFIGURATION_IS_DEBUG_${config}})
        target_compile_definitions(${target} PRIVATE
          $<$<CONFIG:${config}>:DEBUG>
          $<$<CONFIG:${config}>:_DEBUG>
        )
      else()
        target_compile_definitions(${target} PRIVATE
          $<$<CONFIG:${config}>:NDEBUG>
        )

        if(NOT JUCER_ALWAYS_DISABLE_WPO_${config})
          target_compile_options(${target} PRIVATE
            $<$<CONFIG:${config}>:/GL>
          )
        endif()
      endif()

      if(NOT DEFINED JUCER_CHARACTER_SET_${config}
          OR JUCER_CHARACTER_SET_${config} STREQUAL "Default")
        target_compile_definitions(${target} PRIVATE
          $<$<CONFIG:${config}>:_SBCS>
        )
      elseif(JUCER_CHARACTER_SET_${config} STREQUAL "MultiByte")
        # Nothing to do, this is CMake's default
      elseif(JUCER_CHARACTER_SET_${config} STREQUAL "Unicode")
        target_compile_definitions(${target} PRIVATE
          $<$<CONFIG:${config}>:_UNICODE>
          $<$<CONFIG:${config}>:UNICODE>
        )
      endif()

      if(DEFINED JUCER_RUNTIME_LIBRARY_FLAG_${config})
        set(runtime_library_flag ${JUCER_RUNTIME_LIBRARY_FLAG_${config}})
      elseif(JUCER_BUILD_VST OR JUCER_BUILD_VST3)
        if(JUCER_CONFIGURATION_IS_DEBUG_${config})
          set(runtime_library_flag "/MDd")
        else()
          set(runtime_library_flag "/MD")
        endif()
      else()
        if(JUCER_CONFIGURATION_IS_DEBUG_${config})
          set(runtime_library_flag "/MTd")
        else()
          set(runtime_library_flag "/MT")
        endif()
      endif()
      target_compile_options(${target} PRIVATE
        $<$<CONFIG:${config}>:${runtime_library_flag}>
      )

      if(DEFINED JUCER_WARNING_LEVEL_FLAG_${config})
        set(warning_level_flag ${JUCER_WARNING_LEVEL_FLAG_${config}})
      else()
        set(warning_level_flag "/W4")
      endif()
      target_compile_options(${target} PRIVATE
        $<$<CONFIG:${config}>:${warning_level_flag}>
      )

      if(JUCER_TREAT_WARNINGS_AS_ERRORS_${config})
        target_compile_options(${target} PRIVATE $<$<CONFIG:${config}>:/WX>)
      endif()

      if(JUCER_RELAX_IEEE_COMPLIANCE_${config})
        target_compile_options(${target} PRIVATE $<$<CONFIG:${config}>:/fp:fast>)
      endif()

      foreach(path ${JUCER_EXTRA_LIBRARY_SEARCH_PATHS_${config}})
        target_link_libraries(${target} PRIVATE $<$<CONFIG:${config}>:-LIBPATH:${path}>)
      endforeach()

      if(DEFINED JUCER_INCREMENTAL_LINKING_${config})
        if(JUCER_INCREMENTAL_LINKING_${config})
          string(TOUPPER "${config}" upper_config)
          set_property(TARGET ${target}
            APPEND PROPERTY LINK_FLAGS_${upper_config} "/INCREMENTAL"
          )
        endif()
      endif()

      if(DEFINED JUCER_GENERATE_MANIFEST_${config})
        if(NOT JUCER_GENERATE_MANIFEST_${config})
          string(TOUPPER "${config}" upper_config)
          set_property(TARGET ${target}
            APPEND PROPERTY LINK_FLAGS_${upper_config} "/MANIFEST:NO"
          )
        endif()
      endif()
    endforeach()

    unset(all_confs_prebuild_command)
    foreach(config ${JUCER_PROJECT_CONFIGURATIONS})
      if(DEFINED JUCER_PREBUILD_COMMAND_${config})
        set(prebuild_command ${JUCER_PREBUILD_COMMAND_${config}})
        string(APPEND all_confs_prebuild_command
          $<$<CONFIG:${config}>:${prebuild_command}>
        )
      endif()
    endforeach()
    if(all_confs_prebuild_command)
      if(NOT DEFINED JUCER_TARGET_PROJECT_FOLDER)
        message(FATAL_ERROR "JUCER_TARGET_PROJECT_FOLDER must be defined. Give "
          "TARGET_PROJECT_FOLDER when calling jucer_export_target()."
        )
      endif()
      if(NOT IS_DIRECTORY "${JUCER_TARGET_PROJECT_FOLDER}")
        file(MAKE_DIRECTORY "${JUCER_TARGET_PROJECT_FOLDER}")
      endif()
      add_custom_command(TARGET ${target} PRE_BUILD
        COMMAND ${all_confs_prebuild_command}
        WORKING_DIRECTORY "${JUCER_TARGET_PROJECT_FOLDER}"
      )
    endif()

    unset(all_confs_postbuild_command)
    foreach(config ${JUCER_PROJECT_CONFIGURATIONS})
      if(DEFINED JUCER_POSTBUILD_COMMAND_${config})
        set(postbuild_command ${JUCER_POSTBUILD_COMMAND_${config}})
        string(APPEND all_confs_postbuild_command
          $<$<CONFIG:${config}>:${postbuild_command}>
        )
      endif()
    endforeach()
    if(all_confs_postbuild_command)
      if(NOT DEFINED JUCER_TARGET_PROJECT_FOLDER)
        message(FATAL_ERROR "JUCER_TARGET_PROJECT_FOLDER must be defined. Give "
          "TARGET_PROJECT_FOLDER when calling jucer_export_target()."
        )
      endif()
      if(NOT IS_DIRECTORY "${JUCER_TARGET_PROJECT_FOLDER}")
        file(MAKE_DIRECTORY "${JUCER_TARGET_PROJECT_FOLDER}")
      endif()
      add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${all_confs_postbuild_command}
        WORKING_DIRECTORY "${JUCER_TARGET_PROJECT_FOLDER}"
      )
    endif()

    if(JUCER_CXX_STANDARD_TO_USE STREQUAL "14")
      target_compile_options(${target} PRIVATE "-std:c++14")
    elseif(JUCER_CXX_STANDARD_TO_USE STREQUAL "latest")
      target_compile_options(${target} PRIVATE "-std:c++latest")
    endif()

  elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    set_target_properties(${target} PROPERTIES CXX_EXTENSIONS OFF)
    set_target_properties(${target} PROPERTIES CXX_STANDARD 11)

    if(DEFINED JUCER_CXX_LANGUAGE_STANDARD)
      if(JUCER_CXX_LANGUAGE_STANDARD MATCHES "03$")
        set_target_properties(${target} PROPERTIES CXX_STANDARD 98)
      elseif(JUCER_CXX_LANGUAGE_STANDARD MATCHES "11$")
        set_target_properties(${target} PROPERTIES CXX_STANDARD 11)
      elseif(JUCER_CXX_LANGUAGE_STANDARD MATCHES "14$")
        set_target_properties(${target} PROPERTIES CXX_STANDARD 14)
      endif()
    endif()

    foreach(config ${JUCER_PROJECT_CONFIGURATIONS})
      if(${JUCER_CONFIGURATION_IS_DEBUG_${config}})
        target_compile_definitions(${target} PRIVATE
          $<$<CONFIG:${config}>:DEBUG=1>
          $<$<CONFIG:${config}>:_DEBUG=1>
        )
      else()
        target_compile_definitions(${target} PRIVATE
          $<$<CONFIG:${config}>:NDEBUG=1>
        )
      endif()

      if(DEFINED JUCER_ARCHITECTURE_FLAG_${config})
        set(architecture_flag ${JUCER_ARCHITECTURE_FLAG_${config}})
      else()
        set(architecture_flag "-march=native")
      endif()
      target_compile_options(${target} PRIVATE
        $<$<CONFIG:${config}>:${architecture_flag}>
      )

      foreach(path ${JUCER_EXTRA_LIBRARY_SEARCH_PATHS_${config}})
        target_link_libraries(${target} PRIVATE $<$<CONFIG:${config}>:-L${path}>)
      endforeach()
    endforeach()

    set(linux_packages ${JUCER_PROJECT_LINUX_PACKAGES} ${JUCER_PKGCONFIG_LIBRARIES})
    if(linux_packages)
      find_package(PkgConfig REQUIRED)
      list(SORT linux_packages)
      list(REMOVE_DUPLICATES linux_packages)
      foreach(pkg ${linux_packages})
        pkg_check_modules(${pkg} "${pkg}")
        if(NOT ${pkg}_FOUND)
          message(FATAL_ERROR "pkg-config could not find ${pkg}")
        endif()
        target_compile_options(${target} PRIVATE ${${pkg}_CFLAGS})
        target_link_libraries(${target} PRIVATE ${${pkg}_LIBRARIES})
      endforeach()
    else()
      if("juce_graphics" IN_LIST JUCER_PROJECT_MODULES)
        target_include_directories(${target} PRIVATE "/usr/include/freetype2")
      endif()
      if(JUCER_FLAG_JUCE_USE_CURL)
        target_link_libraries(${target} PRIVATE "-lcurl")
      endif()
    endif()

    set(linux_libs ${JUCER_PROJECT_LINUX_LIBS})
    if(linux_libs)
      list(SORT linux_libs)
      list(REMOVE_DUPLICATES linux_libs)
      foreach(item ${linux_libs})
        if(item STREQUAL "pthread")
          target_compile_options(${target} PRIVATE "-pthread")
        endif()
        target_link_libraries(${target} PRIVATE "-l${item}")
      endforeach()
    endif()
  endif()

  target_compile_options(${target} PRIVATE ${JUCER_EXTRA_COMPILER_FLAGS})
  target_link_libraries(${target} PRIVATE ${JUCER_EXTRA_LINKER_FLAGS})

endfunction()


function(_FRUT_generate_plist_file
  target plist_suffix
  bundle_package_type bundle_signature
  main_plist_entries extra_plist_entries
)

  set(plist_filename "Info-${plist_suffix}.plist")
  if(CMAKE_GENERATOR STREQUAL "Xcode")
    set(bundle_executable "\${EXECUTABLE_NAME}")
    set(bundle_identifier "\$(PRODUCT_BUNDLE_IDENTIFIER)")
    set_target_properties(${target} PROPERTIES
      XCODE_ATTRIBUTE_INFOPLIST_FILE "${CMAKE_CURRENT_BINARY_DIR}/${plist_filename}"
      XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "${JUCER_BUNDLE_IDENTIFIER}"
    )
  else()
    set(bundle_executable "\${MACOSX_BUNDLE_BUNDLE_NAME}")
    set(bundle_identifier "\${MACOSX_BUNDLE_GUI_IDENTIFIER}")
    set_target_properties(${target} PROPERTIES
      MACOSX_BUNDLE_BUNDLE_NAME "${JUCER_PROJECT_NAME}"
      MACOSX_BUNDLE_GUI_IDENTIFIER "${JUCER_BUNDLE_IDENTIFIER}"
      MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_BINARY_DIR}/${plist_filename}"
    )
  endif()

  string(CONFIGURE "${main_plist_entries}" main_plist_entries @ONLY)
  string(CONFIGURE "${extra_plist_entries}" extra_plist_entries @ONLY)
  configure_file("${Reprojucer_templates_DIR}/Info.plist" "${plist_filename}" @ONLY)

endfunction()


function(_FRUT_set_bundle_properties target extension)

  if(APPLE)
    set_target_properties(${target} PROPERTIES
      BUNDLE TRUE
      BUNDLE_EXTENSION "${extension}"
      XCODE_ATTRIBUTE_WRAPPER_EXTENSION "${extension}"
    )

    add_custom_command(TARGET ${target} PRE_BUILD
      COMMAND
      "${CMAKE_COMMAND}" "-E" "copy_if_different"
      "${Reprojucer_templates_DIR}/PkgInfo"
      "$<TARGET_FILE_DIR:${target}>/.."
    )
  endif()

endfunction()


function(_FRUT_install_to_plugin_binary_location target plugin_type default_destination)

  unset(all_confs_destination)
  foreach(config ${JUCER_PROJECT_CONFIGURATIONS})
    if(DEFINED JUCER_${plugin_type}_BINARY_LOCATION_${config})
      set(destination ${JUCER_${plugin_type}_BINARY_LOCATION_${config}})
    else()
      set(destination ${default_destination})
    endif()
    string(APPEND all_confs_destination $<$<CONFIG:${config}>:${destination}>)
  endforeach()

  set(component "_install_${target}_to_${plugin_type}_binary_location")

  install(TARGETS ${target} COMPONENT ${component}
    DESTINATION ${all_confs_destination}
  )

  add_custom_command(TARGET ${target} POST_BUILD
    COMMAND
    "${CMAKE_COMMAND}"
    "-DCMAKE_INSTALL_CONFIG_NAME=$<CONFIG>"
    "-DCMAKE_INSTALL_COMPONENT=${component}"
    "-P" "${CMAKE_CURRENT_BINARY_DIR}/cmake_install.cmake"
  )

endfunction()


function(_FRUT_set_JucePlugin_Build_defines target target_type)

  if(JUCER_BUILD_AUDIOUNIT_V3 AND DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5)
    set(JUCER_BUILD_STANDALONE_PLUGIN ON)
  endif()

  set(plugin_types     VST VST3 AudioUnit AudioUnitv3  RTAS AAX Standalone       )
  set(setting_suffixes VST VST3 AUDIOUNIT AUDIOUNIT_V3 RTAS AAX STANDALONE_PLUGIN)
  set(define_suffixes  VST VST3 AU        AUv3         RTAS AAX Standalone       )

  foreach(index RANGE 6)
    list(GET setting_suffixes ${index} setting_suffix)
    list(GET plugin_types ${index} plugin_type)
    list(GET define_suffixes ${index} define_suffix)

    if(target_type STREQUAL "${plugin_type}PlugIn"
        OR (target_type STREQUAL "SharedCodeTarget" AND JUCER_BUILD_${setting_suffix}))
      target_compile_definitions(${target} PRIVATE
        "JucePlugin_Build_${define_suffix}=1"
      )
    else()
      target_compile_definitions(${target} PRIVATE
        "JucePlugin_Build_${define_suffix}=0"
      )
    endif()
  endforeach()

endfunction()


function(_FRUT_link_osx_frameworks target)

  set(osx_frameworks ${JUCER_PROJECT_OSX_FRAMEWORKS} ${JUCER_EXTRA_FRAMEWORKS} ${ARGN})

  if(APPLE)
    if(JUCER_FLAG_JUCE_PLUGINHOST_AU)
      list(APPEND osx_frameworks "AudioUnit" "CoreAudioKit")
    endif()
    list(SORT osx_frameworks)
    list(REMOVE_DUPLICATES osx_frameworks)
    foreach(framework_name ${osx_frameworks})
      find_library(${framework_name}_framework ${framework_name})
      target_link_libraries(${target} PRIVATE "${${framework_name}_framework}")
    endforeach()
  endif()

endfunction()


function(_FRUT_add_xcode_resources target)

  if(APPLE)
    foreach(folder ${JUCER_CUSTOM_XCODE_RESOURCE_FOLDERS})
      add_custom_command(TARGET ${target} PRE_BUILD
        COMMAND rsync -r "${folder}" "$<TARGET_FILE_DIR:${target}>/../Resources"
      )
    endforeach()
  endif()

endfunction()


function(_FRUT_set_custom_xcode_flags target)

  unset(all_flags)

  foreach(config ${JUCER_PROJECT_CONFIGURATIONS})
    if(DEFINED JUCER_CUSTOM_XCODE_FLAGS_${config})
      foreach(xcode_flag ${JUCER_CUSTOM_XCODE_FLAGS_${config}})
        string(REGEX MATCH "^([^= ]+) *= *(.+)" m "${xcode_flag}")
        if(NOT CMAKE_MATCH_0)
          message(FATAL_ERROR "Invalid Xcode flag: \"${xcode_flag}\"")
        endif()
        list(APPEND all_flags "${CMAKE_MATCH_1}")
        set(value "${CMAKE_MATCH_2}")
        string(APPEND all_confs_${CMAKE_MATCH_1} "$<$<CONFIG:${config}>:${value}>")
      endforeach()
    endif()
  endforeach()

  if(all_flags)
    list(SORT all_flags)
    list(REMOVE_DUPLICATES all_flags)
    foreach(flag ${all_flags})
      set_target_properties(${target} PROPERTIES
        XCODE_ATTRIBUTE_${flag} "${all_confs_${flag}}"
      )
    endforeach()
  endif()

endfunction()


function(_FRUT_bool_to_int bool_value out_int_value)

  if(bool_value)
    set(${out_int_value} 1 PARENT_SCOPE)
  else()
    set(${out_int_value} 0 PARENT_SCOPE)
  endif()

endfunction()


function(_FRUT_dec_to_hex dec_value out_hex_value)

  if(dec_value EQUAL 0)
    set(${out_hex_value} "0x0" PARENT_SCOPE)
    return()
  endif()

  if(dec_value LESS 0)
    math(EXPR dec_value "2147483647 ${dec_value} + 1")
  endif()

  while(dec_value GREATER 0)
    math(EXPR hex_unit "${dec_value} & 15")
    if(hex_unit LESS 10)
      set(hex_char ${hex_unit})
    else()
      math(EXPR hex_unit "${hex_unit} + 87")
      string(ASCII ${hex_unit} hex_char)
    endif()
    set(hex_value "${hex_char}${hex_value}")
    math(EXPR dec_value "${dec_value} >> 4")
  endwhile()

  set(${out_hex_value} "0x${hex_value}" PARENT_SCOPE)

endfunction()


function(_FRUT_version_to_dec version out_dec_value)

  string(REPLACE "." ";" segments "${version}")
  list(LENGTH segments segments_size)
  while(segments_size LESS 3)
    list(APPEND segments 0)
    math(EXPR segments_size "${segments_size} + 1")
  endwhile()
  list(GET segments 0 major)
  list(GET segments 1 minor)
  list(GET segments 2 patch)
  math(EXPR dec_value "(${major} << 16) + (${minor} << 8) + ${patch}")
  if(segments_size GREATER 3)
    list(GET segments 3 revision)
    math(EXPR dec_value "${dec_value} << 8 + ${revision}")
  endif()

  set(${out_dec_value} "${dec_value}" PARENT_SCOPE)

endfunction()


function(_FRUT_version_to_hex version out_hex_value)

  _FRUT_version_to_dec("${version}" dec_value)
  _FRUT_dec_to_hex("${dec_value}" hex_value)
  set(${out_hex_value} "${hex_value}" PARENT_SCOPE)

endfunction()


function(_FRUT_four_chars_to_hex value out_hex_value)

  unset(all_ascii_codes)
  foreach(ascii_code RANGE 1 127)
    list(APPEND all_ascii_codes ${ascii_code})
  endforeach()
  string(ASCII ${all_ascii_codes} all_ascii_chars)

  string(STRIP "${value}" four_chars)
  string(SUBSTRING "${four_chars}" 0 4 four_chars)
  set(dec_value 0)
  foreach(index 0 1 2 3)
    string(SUBSTRING "${four_chars}" ${index} 1 ascii_char)
    string(FIND "${all_ascii_chars}" "${ascii_char}" ascii_code)
    if(ascii_code EQUAL -1)
      message(FATAL_ERROR "${value} cannot contain non-ASCII characters")
    endif()
    math(EXPR dec_value "(${dec_value} << 8) | ((${ascii_code} + 1) & 255)")
  endforeach()

  _FRUT_dec_to_hex("${dec_value}" hex_value)
  set(${out_hex_value} "${hex_value}" PARENT_SCOPE)

endfunction()


function(_FRUT_get_au_main_type_code out_value)

  if(NOT DEFINED JUCER_PLUGIN_AU_MAIN_TYPE)
    if(JUCER_MIDI_EFFECT_PLUGIN)
      set(code "aumi")
    elseif(JUCER_PLUGIN_IS_A_SYNTH)
      set(code "aumu")
    elseif(JUCER_PLUGIN_MIDI_INPUT)
      set(code "aumf")
    else()
      set(code "aufx")
    endif()
  else()
    set(code "${JUCER_PLUGIN_AU_MAIN_TYPE}")
  endif()

  set(${out_value} "${code}" PARENT_SCOPE)

endfunction()
