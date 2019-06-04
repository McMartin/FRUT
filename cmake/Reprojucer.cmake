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


set(Reprojucer.cmake_DIR "${CMAKE_CURRENT_LIST_DIR}")
set(Reprojucer_templates_DIR "${Reprojucer.cmake_DIR}/templates")

set(Reprojucer_supported_exporters
  "Xcode (MacOSX)"
  "Visual Studio 2019"
  "Visual Studio 2017"
  "Visual Studio 2015"
  "Visual Studio 2013"
  "Linux Makefile"
  "Code::Blocks (Windows)"
  "Code::Blocks (Linux)"
)
set(Reprojucer_supported_exporters_conditions
  "APPLE"
  "MSVC_VERSION\;GREATER\;1919"
  "MSVC_VERSION\;GREATER\;1909\;AND\;MSVC_VERSION\;LESS\;1920"
  "MSVC_VERSION\;EQUAL\;1900"
  "MSVC_VERSION\;EQUAL\;1800"
  "CMAKE_HOST_SYSTEM_NAME\;STREQUAL\;Linux\;AND\;NOT\;CMAKE_EXTRA_GENERATOR\;STREQUAL\;CodeBlocks"
  "WIN32\;AND\;NOT\;MSVC"
  "CMAKE_HOST_SYSTEM_NAME\;STREQUAL\;Linux\;AND\;CMAKE_EXTRA_GENERATOR\;STREQUAL\;CodeBlocks"
)


function(jucer_project_begin)

  _FRUT_parse_arguments("JUCER_VERSION;PROJECT_FILE;PROJECT_ID" "" "${ARGN}")

  if(DEFINED _JUCER_VERSION)
    set(JUCER_VERSION "${_JUCER_VERSION}" PARENT_SCOPE)
  endif()

  if(DEFINED _PROJECT_FILE)
    if(NOT EXISTS "${_PROJECT_FILE}")
      message(FATAL_ERROR "No such JUCE project file: ${_PROJECT_FILE}")
    endif()
    get_filename_component(project_dir "${_PROJECT_FILE}" DIRECTORY)
    set(JUCER_PROJECT_DIR "${project_dir}" PARENT_SCOPE)
  else()
    set(JUCER_PROJECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)
  endif()

  if(DEFINED _PROJECT_ID)
    set(JUCER_PROJECT_ID "${_PROJECT_ID}" PARENT_SCOPE)
  endif()

endfunction()


function(jucer_project_settings)

  set(single_value_keywords
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
    "INCLUDE_BINARYDATA"
    "BINARYDATA_NAMESPACE"
    "CXX_LANGUAGE_STANDARD"
    "POST_EXPORT_SHELL_COMMAND_MACOS_LINUX"
    "POST_EXPORT_SHELL_COMMAND_WINDOWS"
  )
  set(multi_value_keywords "PREPROCESSOR_DEFINITIONS" "HEADER_SEARCH_PATHS")

  _FRUT_parse_arguments("${single_value_keywords}" "${multi_value_keywords}" "${ARGN}")

  if(NOT DEFINED _PROJECT_NAME)
    message(FATAL_ERROR "Missing PROJECT_NAME argument")
  endif()

  if(DEFINED _PROJECT_VERSION)
    string(REGEX MATCH ".+\\..+\\..+(\\..+)?" version_match "${_PROJECT_VERSION}")
    if(NOT _PROJECT_VERSION STREQUAL version_match)
      message(WARNING "The PROJECT_VERSION doesn't seem to be in the format "
        "major.minor.point[.point]"
      )
    endif()
    _FRUT_version_to_hex("${_PROJECT_VERSION}" hex_value)
    set(JUCER_PROJECT_VERSION_AS_HEX "${hex_value}" PARENT_SCOPE)
  endif()

  if(NOT DEFINED _PROJECT_TYPE)
    message(FATAL_ERROR "Missing PROJECT_TYPE argument")
  endif()
  set(project_types "GUI Application" "Console Application" "Static Library"
    "Dynamic Library" "Audio Plug-in"
  )
  if(NOT _PROJECT_TYPE IN_LIST project_types)
    message(FATAL_ERROR "Unsupported project type: \"${_PROJECT_TYPE}\"\n"
      "Supported project types: ${project_types}"
    )
  endif()

  if(DEFINED _BINARYDATACPP_SIZE_LIMIT)
    set(size_limit_descs "Default" "20.0 MB" "10.0 MB" "6.0 MB" "2.0 MB" "1.0 MB"
      "512.0 KB" "256.0 KB" "128.0 KB" "64.0 KB"
    )
    set(size_limits 10240 20480 10240 6144 2048 1024 512 256 128 64)

    list(FIND size_limit_descs "${_BINARYDATACPP_SIZE_LIMIT}" size_limit_index)
    if(size_limit_index EQUAL -1)
      message(FATAL_ERROR "Unsupported value for BINARYDATACPP_SIZE_LIMIT: "
        "\"${_BINARYDATACPP_SIZE_LIMIT}\"\nSupported values: ${size_limit_descs}"
      )
    endif()
    list(GET size_limits ${size_limit_index} _BINARYDATACPP_SIZE_LIMIT)
  endif()

  if(DEFINED _CXX_LANGUAGE_STANDARD)
    set(cxx_lang_standard_descs "C++11" "C++14" "C++17" "Use Latest")
    set(cxx_lang_standards "11" "14" "17" "latest")

    list(FIND cxx_lang_standard_descs "${_CXX_LANGUAGE_STANDARD}" cxx_lang_standard_index)
    if(cxx_lang_standard_index EQUAL -1)
      message(FATAL_ERROR "Unsupported value for CXX_LANGUAGE_STANDARD: "
        "\"${_CXX_LANGUAGE_STANDARD}\"\nSupported values: ${cxx_lang_standard_descs}"
      )
    endif()
    list(GET cxx_lang_standards ${cxx_lang_standard_index} _CXX_LANGUAGE_STANDARD)
  endif()

  if(DEFINED _HEADER_SEARCH_PATHS)
    set(header_search_paths "")
    foreach(path IN LISTS _HEADER_SEARCH_PATHS)
      file(TO_CMAKE_PATH "${path}" path)
      _FRUT_abs_path_based_on_jucer_project_dir(path "${path}")
      list(APPEND header_search_paths "${path}")
    endforeach()
    set(_HEADER_SEARCH_PATHS "${header_search_paths}")
  endif()

  if(DEFINED _POST_EXPORT_SHELL_COMMAND_MACOS_LINUX)
    _FRUT_warn_about_unsupported_setting("POST_EXPORT_SHELL_COMMAND_MACOS_LINUX"
      "Post-Export Shell Command (macOS, Linux)" 499
    )
  endif()

  if(DEFINED _POST_EXPORT_SHELL_COMMAND_WINDOWS)
    _FRUT_warn_about_unsupported_setting("POST_EXPORT_SHELL_COMMAND_WINDOWS"
      "Post-Export Shell Command (Windows)" 500
    )
  endif()

  foreach(keyword IN LISTS single_value_keywords multi_value_keywords)
    if(DEFINED _${keyword})
      set(JUCER_${keyword} "${_${keyword}}" PARENT_SCOPE)
    endif()
  endforeach()

endfunction()


function(jucer_audio_plugin_settings)

  set(plugin_formats_keywords
    "BUILD_VST"
    "BUILD_VST3"
    "BUILD_AUDIOUNIT"
    "BUILD_AUDIOUNIT_V3"
    "BUILD_RTAS"
    "BUILD_AAX"
    "BUILD_STANDALONE_PLUGIN"
    "BUILD_UNITY_PLUGIN"
    "ENABLE_INTER_APP_AUDIO"
  )
  set(plugin_characteristics_keywords
    "PLUGIN_IS_A_SYNTH"
    "PLUGIN_MIDI_INPUT"
    "PLUGIN_MIDI_OUTPUT"
    "MIDI_EFFECT_PLUGIN"
    "KEY_FOCUS"
  )
  set(single_value_keywords
    ${plugin_formats_keywords}
    "PLUGIN_NAME"
    "PLUGIN_DESCRIPTION"
    "PLUGIN_MANUFACTURER"
    "PLUGIN_MANUFACTURER_CODE"
    "PLUGIN_CODE"
    "PLUGIN_CHANNEL_CONFIGURATIONS"
    ${plugin_characteristics_keywords}
    "PLUGIN_AAX_IDENTIFIER"
    "PLUGIN_AU_EXPORT_PREFIX"
    "PLUGIN_AU_MAIN_TYPE"
    "PLUGIN_AU_IS_SANDBOX_SAFE"
    "PLUGIN_VST_NUM_MIDI_INPUTS"
    "PLUGIN_VST_NUM_MIDI_OUTPUTS"
    "PLUGIN_VST_LEGACY_CATEGORY"
    "PLUGIN_VST_CATEGORY"
    "VST_CATEGORY"
  )
  set(multi_value_keywords
    "PLUGIN_FORMATS"
    "PLUGIN_CHARACTERISTICS"
    "PLUGIN_VST3_CATEGORY"
    "PLUGIN_RTAS_CATEGORY"
    "PLUGIN_AAX_CATEGORY"
  )

  _FRUT_parse_arguments("${single_value_keywords}" "${multi_value_keywords}" "${ARGN}")

  if(DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5.0.0)
    if(DEFINED _BUILD_STANDALONE_PLUGIN)
      message(WARNING "BUILD_STANDALONE_PLUGIN is a JUCE 5 feature only")
    endif()
    if(DEFINED _ENABLE_INTER_APP_AUDIO)
      message(WARNING "ENABLE_INTER_APP_AUDIO is a JUCE 5 feature only")
    endif()
  endif()

  if(DEFINED _PLUGIN_FORMATS)
    set(plugin_formats_vars ${plugin_formats_keywords} "BUILD_VST")
    set(plugin_formats_values "VST" "VST3" "AU" "AUv3" "RTAS" "AAX" "Standalone"
      "Unity" "Enable IAA" "VST (Legacy)"
    )
    foreach(index RANGE 9)
      list(GET plugin_formats_vars ${index} format_var)
      if(NOT DEFINED _${format_var})
        list(GET plugin_formats_values ${index} format_value)
        if(format_value IN_LIST _PLUGIN_FORMATS)
          set(_${format_var} ON)
        endif()
      endif()
    endforeach()
  endif()

  if(DEFINED _PLUGIN_CHARACTERISTICS)
    set(extra_rtas_aax_keywords
      "PLUGIN_RTAS_DISABLE_BYPASS"
      "PLUGIN_AAX_DISABLE_BYPASS"
      "PLUGIN_RTAS_DISABLE_MULTI_MONO"
      "PLUGIN_AAX_DISABLE_MULTI_MONO"
    )
    list(APPEND single_value_keywords ${extra_rtas_aax_keywords})

    set(plugin_characteristics_vars
      ${plugin_characteristics_keywords}
      ${extra_rtas_aax_keywords}
    )
    set(plugin_characteristics_values
      "Plugin is a Synth"
      "Plugin MIDI Input"
      "Plugin MIDI Output"
      "MIDI Effect Plugin"
      "Plugin Editor Requires Keyboard Focus"
      "Disable RTAS Bypass"
      "Disable AAX Bypass"
      "Disable RTAS Multi-Mono"
      "Disable AAX Multi-Mono"
    )
    foreach(index RANGE 8)
      list(GET plugin_characteristics_vars ${index} characteristic_var)
      if(NOT DEFINED _${characteristic_var})
        list(GET plugin_characteristics_values ${index} characteristic_value)
        if(characteristic_value IN_LIST _PLUGIN_CHARACTERISTICS)
          set(_${characteristic_var} ON)
        endif()
      endif()
    endforeach()
  endif()

  if(DEFINED _VST_CATEGORY)
    if(NOT (DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5.3.1))
      message(WARNING "VST_CATEGORY is deprecated. Use PLUGIN_VST_CATEGORY instead.")
    endif()
  endif()

  foreach(keyword IN LISTS single_value_keywords multi_value_keywords)
    if(DEFINED _${keyword})
      set(JUCER_${keyword} "${_${keyword}}" PARENT_SCOPE)
    endif()
  endforeach()

endfunction()


function(jucer_project_files source_group_name)

  function(_FRUT_jucer_project_files_assert_x_or_dot input row_number property)
    if(NOT input STREQUAL "x" AND NOT input STREQUAL ".")
      message(FATAL_ERROR "Expected \"x\" or \".\" token for \"${property}\", got "
        "\"${input}\" instead (row ${row_number})"
      )
    endif()
  endfunction()

  set(files "")
  set(compiler_flag_schemes "")
  unset(compile)
  unset(xcode_resource)
  unset(binary_resource)
  unset(path)
  set(row 1)
  foreach(argument IN LISTS ARGN)
    if(NOT DEFINED compile)
      _FRUT_jucer_project_files_assert_x_or_dot("${argument}" ${row} "Compile")
      set(compile "${argument}")
    elseif(NOT DEFINED xcode_resource)
      _FRUT_jucer_project_files_assert_x_or_dot("${argument}" ${row} "Xcode Resource")
      set(xcode_resource "${argument}")
    elseif(NOT DEFINED binary_resource)
      _FRUT_jucer_project_files_assert_x_or_dot("${argument}" ${row} "Binary Resource")
      set(binary_resource "${argument}")
    elseif(NOT DEFINED path)
      if(argument STREQUAL "x" OR argument STREQUAL ".")
        message(FATAL_ERROR
          "Expected path for \"File\", got \"${argument}\" instead (row ${row})"
        )
      endif()
      set(path "${argument}")

      _FRUT_abs_path_based_on_jucer_project_dir(path "${path}")
      list(APPEND files "${path}")

      if(xcode_resource STREQUAL "x")
        list(APPEND JUCER_PROJECT_XCODE_RESOURCES "${path}")
      endif()
      if(binary_resource STREQUAL "x")
        list(APPEND JUCER_PROJECT_RESOURCES "${path}")
      endif()

      get_filename_component(file_extension "${path}" EXT)
      if((NOT file_extension STREQUAL ".h" AND compile STREQUAL ".")
          OR (file_extension STREQUAL ".mm" AND NOT APPLE))
        set_source_files_properties("${path}" PROPERTIES HEADER_FILE_ONLY TRUE)
      endif()
    else()
      if(argument STREQUAL "x" OR argument STREQUAL ".")
        set(compile "${argument}")
      else()
        set(scheme "${argument}")
        list(APPEND compiler_flag_schemes "${scheme}")
        list(APPEND JUCER_COMPILER_FLAG_SCHEME_${scheme}_FILES "${path}")

        unset(compile)
      endif()

      unset(xcode_resource)
      unset(binary_resource)
      unset(path)
      math(EXPR row "${row} + 1")
    endif()
  endforeach()

  string(REPLACE "/" "\\" source_group_name ${source_group_name})
  source_group(${source_group_name} FILES ${files})

  list(APPEND JUCER_PROJECT_FILES ${files})
  set(JUCER_PROJECT_FILES "${JUCER_PROJECT_FILES}" PARENT_SCOPE)
  set(JUCER_PROJECT_RESOURCES "${JUCER_PROJECT_RESOURCES}" PARENT_SCOPE)
  set(JUCER_PROJECT_XCODE_RESOURCES "${JUCER_PROJECT_XCODE_RESOURCES}" PARENT_SCOPE)

  list(APPEND JUCER_COMPILER_FLAG_SCHEMES "${compiler_flag_schemes}")
  list(REMOVE_DUPLICATES JUCER_COMPILER_FLAG_SCHEMES)
  set(JUCER_COMPILER_FLAG_SCHEMES "${JUCER_COMPILER_FLAG_SCHEMES}" PARENT_SCOPE)
  foreach(scheme IN LISTS compiler_flag_schemes)
    set(JUCER_COMPILER_FLAG_SCHEME_${scheme}_FILES
      "${JUCER_COMPILER_FLAG_SCHEME_${scheme}_FILES}" PARENT_SCOPE
    )
  endforeach()

endfunction()


function(jucer_project_module module_name PATH_KEYWORD modules_folder)

  if(NOT PATH_KEYWORD STREQUAL "PATH")
    message(FATAL_ERROR "Invalid second argument. Expected \"PATH\" keyword, "
      "but got \"${PATH_KEYWORD}\" instead."
    )
  endif()

  _FRUT_abs_path_based_on_jucer_project_dir(modules_folder "${modules_folder}")
  if(NOT IS_DIRECTORY "${modules_folder}")
    message(FATAL_ERROR "No such directory: \"${modules_folder}\"")
  endif()

  foreach(extension IN ITEMS ".h" ".hpp" ".hxx")
    set(module_header_file "${modules_folder}/${module_name}/${module_name}${extension}")
    if(EXISTS "${module_header_file}")
      break()
    endif()
  endforeach()
  if(NOT EXISTS "${module_header_file}")
    message(FATAL_ERROR "${modules_folder}/${module_name}/ is not a valid JUCE module")
  endif()

  set(make_juce_code_browsable ON)

  set(extra_keywords "")
  unset(keyword)
  foreach(argument IN LISTS ARGN)
    if(NOT DEFINED keyword)
      set(keyword "${argument}")

      if(NOT keyword STREQUAL "ADD_SOURCE_TO_PROJECT")
        list(APPEND extra_keywords "${keyword}")
      endif()
    else()
      set(value "${argument}")

      if(keyword STREQUAL "ADD_SOURCE_TO_PROJECT")
        set(make_juce_code_browsable "${value}")
      else()
        set(extra_values_${keyword} "${value}")
      endif()
      unset(keyword)
    endif()
  endforeach()

  foreach(keyword IN LISTS extra_keywords)
    if(NOT DEFINED extra_values_${keyword})
      message(WARNING "Keyword \"${keyword}\" doesn't have any associated value")
    endif()
  endforeach()

  list(APPEND JUCER_PROJECT_MODULES ${module_name})
  set(JUCER_PROJECT_MODULES "${JUCER_PROJECT_MODULES}" PARENT_SCOPE)

  list(APPEND JUCER_PROJECT_MODULES_FOLDERS "${modules_folder}")
  set(JUCER_PROJECT_MODULES_FOLDERS "${JUCER_PROJECT_MODULES_FOLDERS}" PARENT_SCOPE)
  set(JUCER_PROJECT_MODULE_${module_name}_PATH "${modules_folder}" PARENT_SCOPE)

  file(GLOB module_src_files
    LIST_DIRECTORIES FALSE
    "${modules_folder}/${module_name}/${module_name}*.cpp"
    "${modules_folder}/${module_name}/${module_name}*.mm"
    "${modules_folder}/${module_name}/${module_name}*.r"
  )

  if(DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5.0.0)
    set(proxy_prefix "")
  else()
    set(proxy_prefix "include_")
  endif()

  set(module_sources "")
  foreach(src_file IN LISTS module_src_files)
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

    if(DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5.0.0)
      if(src_file MATCHES "_Standalone[._]" AND NOT (JUCER_BUILD_AUDIOUNIT_V3 AND APPLE))
        set(to_compile FALSE)
      endif()
    else()
      if(src_file MATCHES "_Standalone[._]" AND NOT JUCER_BUILD_STANDALONE_PLUGIN)
        set(to_compile FALSE)
      endif()
    endif()

    if(NOT (DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5.3.2))
      if(src_file MATCHES "_Unity[._]" AND NOT JUCER_BUILD_UNITY_PLUGIN)
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
        if(NOT objcxx_src_file IN_LIST module_src_files)
          set(to_compile TRUE)
        endif()
      else()
        set(to_compile TRUE)
      endif()
    endif()

    if(to_compile)
      get_filename_component(src_file_basename "${src_file}" NAME)
      if(src_file_extension STREQUAL ".r")
        set(proxied_src_file "${src_file_basename}")
      else()
        set(proxied_src_file "${module_name}/${src_file_basename}")
      endif()
      configure_file("${Reprojucer_templates_DIR}/JuceLibraryCode-Wrapper.cpp"
        "JuceLibraryCode/${proxy_prefix}${src_file_basename}" @ONLY
      )
      list(APPEND module_sources
        "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode/${proxy_prefix}${src_file_basename}"
      )
    endif()
  endforeach()

  set(JUCER_PROJECT_MODULE_${module_name}_SOURCES "${module_sources}" PARENT_SCOPE)

  file(STRINGS "${module_header_file}" config_flags_lines REGEX "/\\*\\* Config: ")
  string(REPLACE "/** Config: " "" module_config_flags "${config_flags_lines}")
  set(JUCER_${module_name}_CONFIG_FLAGS "${module_config_flags}" PARENT_SCOPE)

  foreach(config_flag IN LISTS extra_keywords)
    if(NOT config_flag IN_LIST module_config_flags)
      message(WARNING "Unknown config flag ${config_flag} in module ${module_name}")
    endif()
    set(JUCER_FLAG_${config_flag} "${extra_values_${config_flag}}" PARENT_SCOPE)
  endforeach()

  set(module_info_searchpaths "")
  set(module_info_OSXFrameworks "")
  set(module_info_linuxLibs "")
  set(module_info_linuxPackages "")
  set(module_info_mingwLibs "")
  unset(module_info_minimumCppStandard)

  file(STRINGS "${module_header_file}" all_lines)
  set(in_module_declaration FALSE)
  foreach(line IN LISTS all_lines)
    string(STRIP "${line}" stripped_line)
    if(stripped_line MATCHES "^BEGIN_JUCE_MODULE_DECLARATION")
      set(in_module_declaration TRUE)
      continue()
    elseif(stripped_line MATCHES "^END_JUCE_MODULE_DECLARATION")
      break()
    endif()

    if(in_module_declaration)
      string(FIND "${line}" ":" colon_pos)
      if(NOT colon_pos EQUAL -1)
        string(SUBSTRING "${line}" 0 ${colon_pos} key)
        string(STRIP "${key}" key)
        math(EXPR colon_pos_plus_one "${colon_pos} + 1")
        string(SUBSTRING "${line}" ${colon_pos_plus_one} -1 value)
        string(STRIP "${value}" value)
        set(module_info_${key} "${value}")
      endif()
    endif()
  endforeach()

  string(REPLACE " " ";" search_paths "${module_info_searchpaths}")
  string(REPLACE "," ";" search_paths "${search_paths}")
  foreach(search_path IN LISTS search_paths)
    list(APPEND JUCER_PROJECT_MODULES_INTERNAL_SEARCH_PATHS
      "${modules_folder}/${module_name}/${search_path}"
    )
  endforeach()
  set(JUCER_PROJECT_MODULES_INTERNAL_SEARCH_PATHS
    "${JUCER_PROJECT_MODULES_INTERNAL_SEARCH_PATHS}" PARENT_SCOPE
  )

  string(REPLACE " " ";" osx_frameworks "${module_info_OSXFrameworks}")
  string(REPLACE "," ";" osx_frameworks "${osx_frameworks}")
  list(APPEND JUCER_PROJECT_OSX_FRAMEWORKS ${osx_frameworks})
  set(JUCER_PROJECT_OSX_FRAMEWORKS "${JUCER_PROJECT_OSX_FRAMEWORKS}" PARENT_SCOPE)

  string(REPLACE " " ";" linux_libs "${module_info_linuxLibs}")
  string(REPLACE "," ";" linux_libs "${linux_libs}")
  list(APPEND JUCER_PROJECT_LINUX_LIBS ${linux_libs})
  set(JUCER_PROJECT_LINUX_LIBS "${JUCER_PROJECT_LINUX_LIBS}" PARENT_SCOPE)

  string(REPLACE " " ";" linux_packages "${module_info_linuxPackages}")
  string(REPLACE "," ";" linux_packages "${linux_packages}")
  list(APPEND JUCER_PROJECT_LINUX_PACKAGES ${linux_packages})
  set(JUCER_PROJECT_LINUX_PACKAGES "${JUCER_PROJECT_LINUX_PACKAGES}" PARENT_SCOPE)

  string(REPLACE " " ";" mingw_libs "${module_info_mingwLibs}")
  string(REPLACE "," ";" mingw_libs "${mingw_libs}")
  list(APPEND JUCER_PROJECT_MINGW_LIBS ${mingw_libs})
  set(JUCER_PROJECT_MINGW_LIBS "${JUCER_PROJECT_MINGW_LIBS}" PARENT_SCOPE)

  if(DEFINED module_info_minimumCppStandard)
    unset(project_cxx_standard)
    if(DEFINED JUCER_CXX_LANGUAGE_STANDARD)
      set(project_cxx_standard "${JUCER_CXX_LANGUAGE_STANDARD}")
    elseif(DEFINED JUCER_VERSION)
      if(JUCER_VERSION VERSION_LESS 5.1.0)
        # "C++ Language Standard" didn't exist before JUCE version 5.1.0
      elseif(JUCER_VERSION VERSION_LESS 5.2.1)
        set(project_cxx_standard 11)
      else()
        set(project_cxx_standard 14)
      endif()
    else()
      set(project_cxx_standard 14)
    endif()
    if(DEFINED project_cxx_standard AND NOT (project_cxx_standard STREQUAL "latest")
        AND (module_info_minimumCppStandard STREQUAL "latest"
          OR module_info_minimumCppStandard GREATER project_cxx_standard))
      message(WARNING "${module_name} has a higher C++ language standard requirement"
        " (${module_info_minimumCppStandard}) than your project"
        " (${project_cxx_standard}). To use this module you need to increase the C++"
        " language standard of the project."
      )
    endif()
  endif()

  if(make_juce_code_browsable)
    file(GLOB_RECURSE browsable_files "${modules_folder}/${module_name}/*")
    foreach(file_path IN LISTS browsable_files)
      get_filename_component(file_dir "${file_path}" DIRECTORY)
      string(REPLACE "${modules_folder}" "" rel_file_dir "${file_dir}")
      string(REPLACE "/" "\\" sub_group_name "${rel_file_dir}")
      source_group("Juce Modules${sub_group_name}" FILES "${file_path}")
    endforeach()
    list(APPEND JUCER_PROJECT_MODULES_BROWSABLE_FILES ${browsable_files})
    set(JUCER_PROJECT_MODULES_BROWSABLE_FILES "${JUCER_PROJECT_MODULES_BROWSABLE_FILES}"
      PARENT_SCOPE
    )
  endif()

endfunction()


function(jucer_appconfig_header USER_CODE_SECTION_KEYWORD user_code_section)

  if(NOT USER_CODE_SECTION_KEYWORD STREQUAL "USER_CODE_SECTION")
    message(FATAL_ERROR "Invalid second argument. Expected \"USER_CODE_SECTION\" "
      "keyword, but got \"${USER_CODE_SECTION_KEYWORD}\" instead."
    )
  endif()

  set(JUCER_APPCONFIG_USER_CODE_SECTION "${user_code_section}" PARENT_SCOPE)

endfunction()


function(jucer_export_target exporter)

  if(NOT exporter IN_LIST Reprojucer_supported_exporters)
    message(FATAL_ERROR "Unsupported exporter: ${exporter}\n"
      "Supported exporters: ${Reprojucer_supported_exporters}"
    )
  endif()
  list(APPEND JUCER_PROJECT_EXPORT_TARGETS "${exporter}")
  set(JUCER_PROJECT_EXPORT_TARGETS "${JUCER_PROJECT_EXPORT_TARGETS}" PARENT_SCOPE)

  list(FIND Reprojucer_supported_exporters "${exporter}" exporter_index)
  list(GET Reprojucer_supported_exporters_conditions ${exporter_index} condition)
  if(NOT (${condition}))
    return()
  endif()

  set(single_value_keywords
    "TARGET_PROJECT_FOLDER"
    "VST_LEGACY_SDK_FOLDER"
    "VST_SDK_FOLDER"
    "ICON_SMALL"
    "ICON_LARGE"
  )
  set(multi_value_keywords
    "EXTRA_PREPROCESSOR_DEFINITIONS"
    "EXTRA_COMPILER_FLAGS"
    "EXTRA_LINKER_FLAGS"
    "EXTERNAL_LIBRARIES_TO_LINK"
  )

  if(exporter STREQUAL "Xcode (MacOSX)")
    list(APPEND single_value_keywords
      "VST3_SDK_FOLDER"
      "AAX_SDK_FOLDER"
      "RTAS_SDK_FOLDER"
      "USE_APP_SANDBOX"
      "APP_SANDBOX_OPTIONS"
      "USE_HARDENED_RUNTIME"
      "HARDENED_RUNTIME_OPTIONS"
      "MICROPHONE_ACCESS"
      "MICROPHONE_ACCESS_TEXT"
      "CAMERA_ACCESS"
      "CAMERA_ACCESS_TEXT"
      "IN_APP_PURCHASES_CAPABILITY"
      "PUSH_NOTIFICATIONS_CAPABILITY"
      "CUSTOM_PLIST"
      "PLIST_PREPROCESS"
      "PLIST_PREFIX_HEADER"
      "PREBUILD_SHELL_SCRIPT"
      "POSTBUILD_SHELL_SCRIPT"
      "EXPORTER_BUNDLE_IDENTIFIER"
      "DEVELOPMENT_TEAM_ID"
      "KEEP_CUSTOM_XCODE_SCHEMES"
      "USE_HEADERMAP"
    )
    list(APPEND multi_value_keywords
      "CUSTOM_XCODE_RESOURCE_FOLDERS"
      "EXTRA_SYSTEM_FRAMEWORKS"
      "EXTRA_FRAMEWORKS"
      "FRAMEWORK_SEARCH_PATHS"
      "EXTRA_CUSTOM_FRAMEWORKS"
      "EMBEDDED_FRAMEWORKS"
      "XCODE_SUBPROJECTS"
    )

    if(JUCER_PROJECT_TYPE STREQUAL "Audio Plug-in")
      list(APPEND single_value_keywords "ADD_DUPLICATE_RESOURCES_FOLDER_TO_APP_EXTENSION")
    endif()

    if(JUCER_PROJECT_TYPE STREQUAL "GUI Application")
      list(APPEND multi_value_keywords "DOCUMENT_FILE_EXTENSIONS")
    endif()
  endif()

  if(exporter MATCHES "^Visual Studio 201(9|7|5|3)$")
    list(APPEND single_value_keywords
      "VST3_SDK_FOLDER"
      "AAX_SDK_FOLDER"
      "RTAS_SDK_FOLDER"
      "MANIFEST_FILE"
      "PLATFORM_TOOLSET"
      "USE_IPP_LIBRARY"
      "WINDOWS_TARGET_PLATFORM"
    )

    if(exporter STREQUAL "Visual Studio 2017")
      list(APPEND single_value_keywords "CXX_STANDARD_TO_USE")
    endif()
  else()
    list(APPEND single_value_keywords "GNU_COMPILER_EXTENSIONS")
  endif()

  if(exporter STREQUAL "Linux Makefile")
    list(APPEND single_value_keywords "CXX_STANDARD_TO_USE")
    list(APPEND multi_value_keywords "PKGCONFIG_LIBRARIES")
  endif()

  if(exporter STREQUAL "Code::Blocks (Windows)")
    list(APPEND single_value_keywords "TARGET_PLATFORM")
  endif()

  set(compiler_flags_for__prefixed_keywords "")
  foreach(argument IN LISTS ARGN)
    if(argument MATCHES "^COMPILER_FLAGS_FOR_([A-Za-z0-9_]+)$")
      list(APPEND compiler_flags_for__prefixed_keywords "${argument}")
      list(APPEND single_value_keywords "${argument}")
    endif()
  endforeach()

  _FRUT_parse_arguments("${single_value_keywords}" "${multi_value_keywords}" "${ARGN}")

  if(DEFINED _TARGET_PROJECT_FOLDER)
    file(TO_CMAKE_PATH "${_TARGET_PROJECT_FOLDER}" project_folder)
    _FRUT_abs_path_based_on_jucer_project_dir(project_folder "${project_folder}")
    set(JUCER_TARGET_PROJECT_FOLDER "${project_folder}" PARENT_SCOPE)
  endif()

  if(DEFINED _VST_LEGACY_SDK_FOLDER)
    file(TO_CMAKE_PATH "${_VST_LEGACY_SDK_FOLDER}" sdk_folder)
    _FRUT_abs_path_based_on_jucer_project_dir(sdk_folder "${sdk_folder}")
    set(JUCER_VST_LEGACY_SDK_FOLDER "${sdk_folder}" PARENT_SCOPE)
  endif()

  if(DEFINED _VST_SDK_FOLDER)
    file(TO_CMAKE_PATH "${_VST_SDK_FOLDER}" sdk_folder)
    _FRUT_abs_path_based_on_jucer_project_dir(sdk_folder "${sdk_folder}")
    set(JUCER_VST_SDK_FOLDER "${sdk_folder}" PARENT_SCOPE)
  endif()

  if(DEFINED _VST3_SDK_FOLDER)
    file(TO_CMAKE_PATH "${_VST3_SDK_FOLDER}" sdk_folder)
    _FRUT_abs_path_based_on_jucer_project_dir(sdk_folder "${sdk_folder}")
    set(JUCER_VST3_SDK_FOLDER "${sdk_folder}" PARENT_SCOPE)
  endif()

  if(DEFINED _AAX_SDK_FOLDER)
    file(TO_CMAKE_PATH "${_AAX_SDK_FOLDER}" sdk_folder)
    _FRUT_abs_path_based_on_jucer_project_dir(sdk_folder "${sdk_folder}")
    set(JUCER_AAX_SDK_FOLDER "${sdk_folder}" PARENT_SCOPE)
  endif()

  if(DEFINED _RTAS_SDK_FOLDER)
    file(TO_CMAKE_PATH "${_RTAS_SDK_FOLDER}" sdk_folder)
    _FRUT_abs_path_based_on_jucer_project_dir(sdk_folder "${sdk_folder}")
    set(JUCER_RTAS_SDK_FOLDER "${sdk_folder}" PARENT_SCOPE)
  endif()

  if(DEFINED _EXTRA_PREPROCESSOR_DEFINITIONS)
    set(JUCER_EXTRA_PREPROCESSOR_DEFINITIONS "${_EXTRA_PREPROCESSOR_DEFINITIONS}"
      PARENT_SCOPE
    )
  endif()

  if(DEFINED _EXTRA_COMPILER_FLAGS)
    set(JUCER_EXTRA_COMPILER_FLAGS "${_EXTRA_COMPILER_FLAGS}" PARENT_SCOPE)
  endif()

  foreach(keyword IN LISTS compiler_flags_for__prefixed_keywords)
    if(DEFINED _${keyword})
      set(JUCER_${keyword} "${_${keyword}}" PARENT_SCOPE)
    endif()
  endforeach()

  if(DEFINED _EXTRA_LINKER_FLAGS)
    set(JUCER_EXTRA_LINKER_FLAGS "${_EXTRA_LINKER_FLAGS}" PARENT_SCOPE)
  endif()

  if(DEFINED _EXTERNAL_LIBRARIES_TO_LINK)
    set(JUCER_EXTERNAL_LIBRARIES_TO_LINK "${_EXTERNAL_LIBRARIES_TO_LINK}" PARENT_SCOPE)
  endif()

  if(DEFINED _GNU_COMPILER_EXTENSIONS)
    set(JUCER_GNU_COMPILER_EXTENSIONS "${_GNU_COMPILER_EXTENSIONS}" PARENT_SCOPE)
  endif()

  if(DEFINED _ICON_SMALL)
    if(NOT _ICON_SMALL STREQUAL "<None>")
      _FRUT_abs_path_based_on_jucer_project_dir(small_icon "${_ICON_SMALL}")
      if(NOT EXISTS "${small_icon}")
        message(FATAL_ERROR "No such file (ICON_SMALL): ${small_icon}")
      endif()
      set(JUCER_SMALL_ICON "${small_icon}" PARENT_SCOPE)
    endif()
  endif()

  if(DEFINED _ICON_LARGE)
    if(NOT _ICON_LARGE STREQUAL "<None>")
      _FRUT_abs_path_based_on_jucer_project_dir(large_icon "${_ICON_LARGE}")
      if(NOT EXISTS "${large_icon}")
        message(FATAL_ERROR "No such file (ICON_LARGE): ${large_icon}")
      endif()
      set(JUCER_LARGE_ICON "${large_icon}" PARENT_SCOPE)
    endif()
  endif()

  if(DEFINED _CUSTOM_XCODE_RESOURCE_FOLDERS)
    set(resource_folders "")
    foreach(folder IN LISTS _CUSTOM_XCODE_RESOURCE_FOLDERS)
      _FRUT_abs_path_based_on_jucer_project_dir(abs_folder "${folder}")
      list(APPEND resource_folders "${abs_folder}")
    endforeach()
    set(JUCER_CUSTOM_XCODE_RESOURCE_FOLDERS "${resource_folders}" PARENT_SCOPE)
  endif()

  if(DEFINED _ADD_DUPLICATE_RESOURCES_FOLDER_TO_APP_EXTENSION)
    _FRUT_warn_about_unsupported_setting(
      "ADD_DUPLICATE_RESOURCES_FOLDER_TO_APP_EXTENSION"
      "Add Duplicate Resources Folder to App Extension" 441
    )
  endif()

  if(DEFINED _DOCUMENT_FILE_EXTENSIONS)
    set(JUCER_DOCUMENT_FILE_EXTENSIONS "${_DOCUMENT_FILE_EXTENSIONS}" PARENT_SCOPE)
  endif()

  if(DEFINED _USE_APP_SANDBOX AND _USE_APP_SANDBOX)
    _FRUT_warn_about_unsupported_setting("USE_APP_SANDBOX" "Use App Sandbox" 497)
  endif()

  if(DEFINED _APP_SANDBOX_OPTIONS)
    # TODO with USE_APP_SANDBOX
  endif()

  if(DEFINED _USE_HARDENED_RUNTIME AND _USE_HARDENED_RUNTIME)
    _FRUT_warn_about_unsupported_setting(
      "USE_HARDENED_RUNTIME" "Use Hardened Runtime" 496
    )
  endif()

  if(DEFINED _HARDENED_RUNTIME_OPTIONS)
    # TODO with USE_HARDENED_RUNTIME
  endif()

  if(DEFINED _MICROPHONE_ACCESS)
    set(JUCER_MICROPHONE_ACCESS "${_MICROPHONE_ACCESS}" PARENT_SCOPE)
  endif()

  if(DEFINED _MICROPHONE_ACCESS_TEXT)
    set(JUCER_MICROPHONE_ACCESS_TEXT "${_MICROPHONE_ACCESS_TEXT}" PARENT_SCOPE)
  endif()

  if(DEFINED _CAMERA_ACCESS)
    set(JUCER_CAMERA_ACCESS "${_CAMERA_ACCESS}" PARENT_SCOPE)
  endif()

  if(DEFINED _CAMERA_ACCESS_TEXT)
    set(JUCER_CAMERA_ACCESS_TEXT "${_CAMERA_ACCESS_TEXT}" PARENT_SCOPE)
  endif()

  if(DEFINED _IN_APP_PURCHASES_CAPABILITY AND _IN_APP_PURCHASES_CAPABILITY)
    _FRUT_warn_about_unsupported_setting(
      "IN_APP_PURCHASES_CAPABILITY" "In-App Purchases Capability" 395
    )
  endif()

  if(DEFINED _PUSH_NOTIFICATIONS_CAPABILITY AND _PUSH_NOTIFICATIONS_CAPABILITY)
    _FRUT_warn_about_unsupported_setting(
      "PUSH_NOTIFICATIONS_CAPABILITY" "Push Notifications Capability" 396
    )
  endif()

  if(DEFINED _CUSTOM_PLIST)
    set(JUCER_CUSTOM_PLIST "${_CUSTOM_PLIST}" PARENT_SCOPE)
  endif()

  if(DEFINED _PLIST_PREPROCESS AND _PLIST_PREPROCESS)
    _FRUT_warn_about_unsupported_setting("PLIST_PREPROCESS" "PList Preprocess" 394)
  endif()

  if(DEFINED _PLIST_PREFIX_HEADER)
    # TODO with PLIST_PREPROCESS
  endif()

  if(DEFINED _EXTRA_SYSTEM_FRAMEWORKS)
    set(JUCER_EXTRA_SYSTEM_FRAMEWORKS "${_EXTRA_SYSTEM_FRAMEWORKS}" PARENT_SCOPE)
  endif()

  if(DEFINED _EXTRA_FRAMEWORKS)
    set(JUCER_EXTRA_FRAMEWORKS "${_EXTRA_FRAMEWORKS}" PARENT_SCOPE)
  endif()

  if(DEFINED _FRAMEWORK_SEARCH_PATHS)
    set(JUCER_FRAMEWORK_SEARCH_PATHS "${_FRAMEWORK_SEARCH_PATHS}" PARENT_SCOPE)
  endif()

  if(DEFINED _EXTRA_CUSTOM_FRAMEWORKS)
    set(JUCER_EXTRA_CUSTOM_FRAMEWORKS "${_EXTRA_CUSTOM_FRAMEWORKS}" PARENT_SCOPE)
  endif()

  if(DEFINED _EMBEDDED_FRAMEWORKS)
    _FRUT_warn_about_unsupported_setting("EMBEDDED_FRAMEWORKS" "Embedded Frameworks" 443)
  endif()

  if(DEFINED _XCODE_SUBPROJECTS)
    _FRUT_warn_about_unsupported_setting("XCODE_SUBPROJECTS" "Xcode Subprojects" 445)
  endif()

  if(DEFINED _PREBUILD_SHELL_SCRIPT)
    set(script_content "${_PREBUILD_SHELL_SCRIPT}")
    configure_file("${Reprojucer_templates_DIR}/script.in" "prebuild.sh" @ONLY)
    set(JUCER_PREBUILD_SHELL_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/prebuild.sh"
      PARENT_SCOPE
    )
  endif()

  if(DEFINED _POSTBUILD_SHELL_SCRIPT)
    set(script_content "${_POSTBUILD_SHELL_SCRIPT}")
    configure_file("${Reprojucer_templates_DIR}/script.in" "postbuild.sh" @ONLY)
    set(JUCER_POSTBUILD_SHELL_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/postbuild.sh"
      PARENT_SCOPE
    )
  endif()

  if(DEFINED _EXPORTER_BUNDLE_IDENTIFIER)
    _FRUT_warn_about_unsupported_setting(
      "EXPORTER_BUNDLE_IDENTIFIER" "Exporter Bundle Identifier" 498
    )
  endif()

  if(DEFINED _DEVELOPMENT_TEAM_ID)
    _FRUT_warn_about_unsupported_setting("DEVELOPMENT_TEAM_ID" "Development Team ID" 251)
  endif()

  if(DEFINED _KEEP_CUSTOM_XCODE_SCHEMES)
    if(_KEEP_CUSTOM_XCODE_SCHEMES AND CMAKE_GENERATOR STREQUAL "Xcode")
      message(WARNING "KEEP_CUSTOM_XCODE_SCHEMES is ignored. Reprojucer.cmake won't "
        "create any Xcode schemes, so it won't delete any either."
      )
    endif()
  endif()

  if(DEFINED _USE_HEADERMAP)
    if(_USE_HEADERMAP)
      message(WARNING "USE_HEADERMAP is only supported when using the Xcode generator. "
        "You should call `cmake -G Xcode`."
      )
    endif()
    set(JUCER_USE_HEADERMAP "${_USE_HEADERMAP}" PARENT_SCOPE)
  endif()

  if(DEFINED _MANIFEST_FILE)
    _FRUT_warn_about_unsupported_setting("MANIFEST_FILE" "Manifest file" 440)
  endif()

  if(DEFINED _PLATFORM_TOOLSET)
    set(toolset "${_PLATFORM_TOOLSET}")
    if((exporter STREQUAL "Visual Studio 2019"
          AND (toolset STREQUAL "v140" OR toolset STREQUAL "v140_xp"
            OR toolset STREQUAL "v141" OR toolset STREQUAL "v141_xp"
            OR toolset STREQUAL "v142"))
        OR (exporter STREQUAL "Visual Studio 2017"
          AND (toolset STREQUAL "v140" OR toolset STREQUAL "v140_xp"
            OR toolset STREQUAL "v141" OR toolset STREQUAL "v141_xp"))
        OR (exporter STREQUAL "Visual Studio 2015"
          AND (toolset STREQUAL "v140" OR toolset STREQUAL "v140_xp"
            OR toolset STREQUAL "CTP_Nov2013"))
        OR (exporter STREQUAL "Visual Studio 2013"
          AND (toolset STREQUAL "v120" OR toolset STREQUAL "v120_xp"
            OR toolset STREQUAL "Windows7" OR toolset STREQUAL "CTP_Nov2013")))
      if(NOT toolset STREQUAL "${CMAKE_VS_PLATFORM_TOOLSET}")
        message(FATAL_ERROR "You must call `cmake -T ${toolset}` in order to build with"
          " the toolset \"${toolset}\"."
        )
      endif()
    elseif(NOT toolset STREQUAL "(default)")
      message(FATAL_ERROR "Unsupported value for PLATFORM_TOOLSET: \"${toolset}\"")
    endif()
  endif()

  if(DEFINED _USE_IPP_LIBRARY)
    set(ipp_library "${_USE_IPP_LIBRARY}")
    set(ipp_library_values
      "Yes (Default Mode)"
      "Yes (Default Linking)"
      "Multi-Threaded Static Library"
      "Single-Threaded Static Library"
      "Multi-Threaded DLL"
      "Single-Threaded DLL"
    )
    if(ipp_library IN_LIST ipp_library_values)
      _FRUT_warn_about_unsupported_setting("USE_IPP_LIBRARY" "Use IPP Library" 252)
    elseif(NOT ipp_library STREQUAL "No")
      message(FATAL_ERROR "Unsupported value for USE_IPP_LIBRARY: \"${ipp_library}\"")
    endif()
  endif()

  if(DEFINED _WINDOWS_TARGET_PLATFORM)
    set(platform "${_WINDOWS_TARGET_PLATFORM}")
    if(NOT platform STREQUAL CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION)
      message(STATUS "CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION "
        "(${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}) doesn't match "
        "WINDOWS_TARGET_PLATFORM (${platform})."
      )
    endif()
  endif()

  if(DEFINED _CXX_STANDARD_TO_USE AND exporter STREQUAL "Visual Studio 2017")
    set(standard "${_CXX_STANDARD_TO_USE}")
    if(standard STREQUAL "C++14")
      set(JUCER_CXX_STANDARD_TO_USE "14" PARENT_SCOPE)
    elseif(standard STREQUAL "Latest C++ Standard")
      set(JUCER_CXX_STANDARD_TO_USE "latest" PARENT_SCOPE)
    elseif(NOT standard STREQUAL "(default)")
      message(FATAL_ERROR "Unsupported value for CXX_STANDARD_TO_USE: \"${standard}\"")
    endif()
  endif()

  if(DEFINED _CXX_STANDARD_TO_USE AND exporter STREQUAL "Linux Makefile")
    set(standard "${_CXX_STANDARD_TO_USE}")
    if(standard MATCHES "^C\\+\\+(03|11|14)$")
      set(JUCER_CXX_STANDARD_TO_USE "${standard}" PARENT_SCOPE)
    else()
      message(FATAL_ERROR "Unsupported value for CXX_STANDARD_TO_USE: \"${standard}\"")
    endif()
  endif()

  if(DEFINED _PKGCONFIG_LIBRARIES)
    set(JUCER_PKGCONFIG_LIBRARIES "${_PKGCONFIG_LIBRARIES}" PARENT_SCOPE)
  endif()

  if(DEFINED _TARGET_PLATFORM)
    set(target_platform "${_TARGET_PLATFORM}")
    set(target_platform_values "Default" "Windows NT 4.0" "Windows 2000" "Windows XP"
      "Windows Server 2003" "Windows Vista" "Windows Server 2008" "Windows 7" "Windows 8"
      "Windows 8.1" "Windows 10"
    )
    if(NOT target_platform IN_LIST target_platform_values)
      message(FATAL_ERROR "Unsupported value for TARGET_PLATFORM: \"${target_platform}\"")
    endif()
    set(JUCER_TARGET_PLATFORM "${target_platform}" PARENT_SCOPE)
  endif()

endfunction()


function(jucer_export_target_configuration
  exporter NAME_KEYWORD config DEBUG_MODE_KEYWORD is_debug
)

  if(NOT exporter IN_LIST Reprojucer_supported_exporters)
    message(FATAL_ERROR "Unsupported exporter: ${exporter}\n"
      "Supported exporters: ${Reprojucer_supported_exporters}"
    )
  endif()

  if(NOT exporter IN_LIST JUCER_PROJECT_EXPORT_TARGETS)
    message(FATAL_ERROR "You must call jucer_export_target(\"${exporter}\") before "
      "calling jucer_export_target_configuration(\"${exporter}\")."
    )
  endif()

  if(NOT NAME_KEYWORD STREQUAL "NAME")
    message(FATAL_ERROR "Invalid second argument. Expected \"NAME\" keyword, "
      "but got \"${NAME_KEYWORD}\" instead."
    )
  endif()

  if(NOT DEBUG_MODE_KEYWORD STREQUAL "DEBUG_MODE")
    message(FATAL_ERROR "Invalid fourth argument. Expected \"DEBUG_MODE\" keyword, "
      "but got \"${DEBUG_MODE_KEYWORD}\" instead."
    )
  endif()

  list(FIND Reprojucer_supported_exporters "${exporter}" exporter_index)
  list(GET Reprojucer_supported_exporters_conditions ${exporter_index} condition)
  if(NOT (${condition}))
    return()
  endif()

  list(APPEND JUCER_PROJECT_CONFIGURATIONS ${config})
  set(JUCER_PROJECT_CONFIGURATIONS "${JUCER_PROJECT_CONFIGURATIONS}" PARENT_SCOPE)

  set(JUCER_CONFIGURATION_IS_DEBUG_${config} "${is_debug}" PARENT_SCOPE)

  set(single_value_keywords "BINARY_NAME" "BINARY_LOCATION" "OPTIMISATION")
  set(multi_value_keywords
    "HEADER_SEARCH_PATHS"
    "EXTRA_LIBRARY_SEARCH_PATHS"
    "PREPROCESSOR_DEFINITIONS"
    "LINK_TIME_OPTIMISATION"
  )

  if(exporter STREQUAL "Xcode (MacOSX)")
    list(APPEND single_value_keywords
      "ENABLE_PLUGIN_COPY_STEP"
      "VST_BINARY_LOCATION"
      "VST3_BINARY_LOCATION"
      "AU_BINARY_LOCATION"
      "RTAS_BINARY_LOCATION"
      "AAX_BINARY_LOCATION"
      "UNITY_BINARY_LOCATION"
      "VST_LEGACY_BINARY_LOCATION"
      "OSX_BASE_SDK_VERSION"
      "OSX_DEPLOYMENT_TARGET"
      "OSX_ARCHITECTURE"
      "CXX_LANGUAGE_STANDARD"
      "CXX_LIBRARY"
      "CODE_SIGNING_IDENTITY"
      "RELAX_IEEE_COMPLIANCE"
      "STRIP_LOCAL_SYMBOLS"
    )
    list(APPEND multi_value_keywords
      "CUSTOM_XCODE_FLAGS"
      "PLIST_PREPROCESSOR_DEFINITIONS"
    )
  endif()

  if(exporter MATCHES "^Visual Studio 201(9|7|5|3)$")
    list(APPEND single_value_keywords
      "ENABLE_PLUGIN_COPY_STEP"
      "VST_BINARY_LOCATION"
      "VST3_BINARY_LOCATION"
      "RTAS_BINARY_LOCATION"
      "AAX_BINARY_LOCATION"
      "UNITY_BINARY_LOCATION"
      "VST_LEGACY_BINARY_LOCATION"
      "DEBUG_INFORMATION_FORMAT"
      "WARNING_LEVEL"
      "TREAT_WARNINGS_AS_ERRORS"
      "RUNTIME_LIBRARY"
      "WHOLE_PROGRAM_OPTIMISATION"
      "MULTI_PROCESSOR_COMPILATION"
      "INCREMENTAL_LINKING"
      "PREBUILD_COMMAND"
      "POSTBUILD_COMMAND"
      "GENERATE_MANIFEST"
      "CHARACTER_SET"
      "ARCHITECTURE"
      "RELAX_IEEE_COMPLIANCE"
    )

    if(NOT is_debug)
      list(APPEND single_value_keywords "FORCE_GENERATION_OF_DEBUG_SYMBOLS")
    endif()
  endif()

  if(exporter STREQUAL "Linux Makefile")
    list(APPEND single_value_keywords "ARCHITECTURE")
  endif()

  if(exporter MATCHES "^Code::Blocks \((Windows|Linux)\)$")
    list(APPEND single_value_keywords "ARCHITECTURE")
  endif()

  _FRUT_parse_arguments("${single_value_keywords}" "${multi_value_keywords}" "${ARGN}")

  if(DEFINED _BINARY_NAME)
    set(JUCER_BINARY_NAME_${config} "${_BINARY_NAME}" PARENT_SCOPE)
  endif()

  if(DEFINED _BINARY_LOCATION)
    get_filename_component(abs_path "${_BINARY_LOCATION}" ABSOLUTE)
    set(JUCER_BINARY_LOCATION_${config} "${abs_path}" PARENT_SCOPE)
  endif()

  if(DEFINED _HEADER_SEARCH_PATHS)
    set(header_search_paths "")
    foreach(path IN LISTS _HEADER_SEARCH_PATHS)
      file(TO_CMAKE_PATH "${path}" path)
      _FRUT_abs_path_based_on_jucer_project_dir(path "${path}")
      list(APPEND header_search_paths "${path}")
    endforeach()
    set(JUCER_HEADER_SEARCH_PATHS_${config} "${header_search_paths}" PARENT_SCOPE)
  endif()

  if(DEFINED _EXTRA_LIBRARY_SEARCH_PATHS)
    set(library_search_paths "")
    foreach(path IN LISTS _EXTRA_LIBRARY_SEARCH_PATHS)
      file(TO_CMAKE_PATH "${path}" path)
      _FRUT_abs_path_based_on_jucer_project_dir(path "${path}")
      list(APPEND library_search_paths "${path}")
    endforeach()
    set(JUCER_EXTRA_LIBRARY_SEARCH_PATHS_${config} "${library_search_paths}" PARENT_SCOPE)
  endif()

  if(DEFINED _PREPROCESSOR_DEFINITIONS)
    set(JUCER_PREPROCESSOR_DEFINITIONS_${config} "${_PREPROCESSOR_DEFINITIONS}"
      PARENT_SCOPE
    )
  endif()

  if(DEFINED _LINK_TIME_OPTIMISATION)
    set(JUCER_LINK_TIME_OPTIMISATION_${config} "${_LINK_TIME_OPTIMISATION}" PARENT_SCOPE)
  endif()

  if(DEFINED _OPTIMISATION)
    set(optimisation "${_OPTIMISATION}")
    if(exporter MATCHES "^Visual Studio 201(9|7|5|3)$")
      if(optimisation STREQUAL "No optimisation")
        set(optimisation_flag "/Od")
      elseif(optimisation STREQUAL "Minimise size")
        set(optimisation_flag "/O1")
      elseif(optimisation STREQUAL "Maximise speed")
        set(optimisation_flag "/Ox")
      else()
        message(FATAL_ERROR "Unsupported value for OPTIMISATION: \"${optimisation}\"")
      endif()
    else()
      if(optimisation STREQUAL "-O0 (no optimisation)")
        set(optimisation_flag "-O0")
      elseif(optimisation STREQUAL "-Os (minimise code size)")
        set(optimisation_flag "-Os")
      elseif(optimisation STREQUAL "-O1 (fast)")
        set(optimisation_flag "-O1")
      elseif(optimisation STREQUAL "-O2 (faster)")
        set(optimisation_flag "-O2")
      elseif(optimisation STREQUAL "-O3 (fastest with safe optimisations)")
        set(optimisation_flag "-O3")
      elseif(optimisation STREQUAL "-Ofast (uses aggressive optimisations)")
        set(optimisation_flag "-Ofast")
      else()
        message(FATAL_ERROR "Unsupported value for OPTIMISATION: \"${optimisation}\"")
      endif()
    endif()
    set(JUCER_OPTIMISATION_FLAG_${config} "${optimisation_flag}" PARENT_SCOPE)
  endif()

  if(DEFINED _ENABLE_PLUGIN_COPY_STEP)
    set(JUCER_ENABLE_PLUGIN_COPY_STEP_${config} "${_ENABLE_PLUGIN_COPY_STEP}"
      PARENT_SCOPE
    )
  endif()

  if(DEFINED _VST_BINARY_LOCATION)
    _FRUT_sanitize_path_in_user_folder(binary_location "${_VST_BINARY_LOCATION}")
    set(JUCER_VST_BINARY_LOCATION_${config} "${binary_location}" PARENT_SCOPE)
  endif()

  if(DEFINED _VST3_BINARY_LOCATION)
    _FRUT_sanitize_path_in_user_folder(binary_location "${_VST3_BINARY_LOCATION}")
    set(JUCER_VST3_BINARY_LOCATION_${config} "${binary_location}" PARENT_SCOPE)
  endif()

  if(DEFINED _AU_BINARY_LOCATION)
    _FRUT_sanitize_path_in_user_folder(binary_location "${_AU_BINARY_LOCATION}")
    set(JUCER_AU_BINARY_LOCATION_${config} "${binary_location}" PARENT_SCOPE)
  endif()

  if(DEFINED _RTAS_BINARY_LOCATION)
    _FRUT_sanitize_path_in_user_folder(binary_location "${_RTAS_BINARY_LOCATION}")
    set(JUCER_RTAS_BINARY_LOCATION_${config} "${binary_location}" PARENT_SCOPE)
  endif()

  if(DEFINED _AAX_BINARY_LOCATION)
    _FRUT_sanitize_path_in_user_folder(binary_location "${_AAX_BINARY_LOCATION}")
    set(JUCER_AAX_BINARY_LOCATION_${config} "${binary_location}" PARENT_SCOPE)
  endif()

  if(DEFINED _UNITY_BINARY_LOCATION)
    _FRUT_sanitize_path_in_user_folder(binary_location "${_UNITY_BINARY_LOCATION}")
    set(JUCER_UNITY_BINARY_LOCATION_${config} "${binary_location}" PARENT_SCOPE)
  endif()

  if(DEFINED _VST_LEGACY_BINARY_LOCATION)
    _FRUT_sanitize_path_in_user_folder(binary_location "${_VST_LEGACY_BINARY_LOCATION}")
    set(JUCER_VST_BINARY_LOCATION_${config} "${binary_location}" PARENT_SCOPE)
  endif()

  if(DEFINED _OSX_BASE_SDK_VERSION)
    set(version "${_OSX_BASE_SDK_VERSION}")
    if(version MATCHES "^10\\.([5-9]|1[0-4]) SDK$")
      set(JUCER_OSX_BASE_SDK_VERSION_${config} "10.${CMAKE_MATCH_1}" PARENT_SCOPE)
    elseif(NOT version STREQUAL "Use Default")
      message(FATAL_ERROR "Unsupported value for OSX_BASE_SDK_VERSION: \"${version}\"")
    endif()
  endif()

  if(DEFINED _OSX_DEPLOYMENT_TARGET)
    set(target "${_OSX_DEPLOYMENT_TARGET}")
    if(target MATCHES "^10\\.([5-9]|1[0-4])$")
      set(JUCER_OSX_DEPLOYMENT_TARGET_${config} "10.${CMAKE_MATCH_1}" PARENT_SCOPE)
    elseif(NOT target STREQUAL "Use Default")
      message(FATAL_ERROR "Unsupported value for OSX_DEPLOYMENT_TARGET: \"${target}\"")
    endif()
  endif()

  if(DEFINED _OSX_ARCHITECTURE)
    set(architecture "${_OSX_ARCHITECTURE}")
    unset(xcode_archs)
    if(architecture STREQUAL "Native architecture of build machine")
      # Consider as default
      unset(osx_architectures)
      if(CMAKE_GENERATOR STREQUAL "Xcode")
        set(xcode_archs "$(NATIVE_ARCH_ACTUAL)")
      endif()
    elseif(architecture STREQUAL "Universal Binary (32-bit)")
      set(osx_architectures "i386")
      if(CMAKE_GENERATOR STREQUAL "Xcode")
        set(xcode_archs "$(ARCHS_STANDARD_32_BIT)")
      endif()
    elseif(architecture STREQUAL "Universal Binary (32/64-bit)")
      set(osx_architectures "x86_64" "i386")
      if(CMAKE_GENERATOR STREQUAL "Xcode")
        set(xcode_archs "$(ARCHS_STANDARD_32_64_BIT)")
      endif()
    elseif(architecture STREQUAL "64-bit Intel")
      set(osx_architectures "x86_64")
      if(CMAKE_GENERATOR STREQUAL "Xcode")
        set(xcode_archs "$(ARCHS_STANDARD_64_BIT)")
      endif()
    elseif(NOT architecture STREQUAL "Use Default")
      message(FATAL_ERROR "Unsupported value for OSX_ARCHITECTURE: \"${architecture}\"")
    endif()
    if(DEFINED osx_architectures)
      set(JUCER_OSX_ARCHITECTURES_${config} "${osx_architectures}" PARENT_SCOPE)
    endif()
    if(DEFINED xcode_archs)
      set(JUCER_XCODE_ARCHS_${config} "${xcode_archs}" PARENT_SCOPE)
    endif()
  endif()

  if(DEFINED _CUSTOM_XCODE_FLAGS)
    if(NOT CMAKE_GENERATOR STREQUAL "Xcode")
      message(WARNING "CUSTOM_XCODE_FLAGS is only supported when using the Xcode "
        "generator. You should call `cmake -G Xcode`."
      )
    endif()
    set(JUCER_CUSTOM_XCODE_FLAGS_${config} "${_CUSTOM_XCODE_FLAGS}" PARENT_SCOPE)
  endif()

  if(DEFINED _CXX_LANGUAGE_STANDARD)
    set(standard "${_CXX_LANGUAGE_STANDARD}")
    if(standard MATCHES "^(C|GNU)\\+\\+98$"
        AND DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5.0.0)
      set(JUCER_CXX_LANGUAGE_STANDARD_${config} "${standard}" PARENT_SCOPE)
    elseif(standard MATCHES "^(C|GNU)\\+\\+(11|14)$")
      set(JUCER_CXX_LANGUAGE_STANDARD_${config} "${standard}" PARENT_SCOPE)
    elseif(NOT standard STREQUAL "Use Default")
      message(FATAL_ERROR "Unsupported value for CXX_LANGUAGE_STANDARD: \"${standard}\"")
    endif()
  endif()

  if(DEFINED _CXX_LIBRARY)
    set(cxx_library "${_CXX_LIBRARY}")
    if(cxx_library STREQUAL "LLVM libc++")
      set(JUCER_CXX_LIBRARY_${config} "libc++" PARENT_SCOPE)
    elseif(cxx_library STREQUAL "GNU libstdc++")
      set(JUCER_CXX_LIBRARY_${config} "libstdc++" PARENT_SCOPE)
    elseif(NOT cxx_library STREQUAL "Use Default")
      message(FATAL_ERROR "Unsupported value for CXX_LIBRARY: \"${cxx_library}\"")
    endif()
  endif()

  if(DEFINED _PLIST_PREPROCESSOR_DEFINITIONS)
    # TODO with PLIST_PREPROCESS
  endif()

  if(DEFINED _CODE_SIGNING_IDENTITY)
    if(NOT CMAKE_GENERATOR STREQUAL "Xcode")
      message(WARNING "CODE_SIGNING_IDENTITY is only supported when using the Xcode "
        "generator. You should call `cmake -G Xcode`."
      )
    endif()
    set(JUCER_CODE_SIGNING_IDENTITY_${config} "${_CODE_SIGNING_IDENTITY}" PARENT_SCOPE)
  endif()

  if(DEFINED _RELAX_IEEE_COMPLIANCE)
    set(JUCER_RELAX_IEEE_COMPLIANCE_${config} "${_RELAX_IEEE_COMPLIANCE}" PARENT_SCOPE)
  endif()

  if(DEFINED _STRIP_LOCAL_SYMBOLS)
    set(JUCER_STRIP_LOCAL_SYMBOLS_${config} "${_STRIP_LOCAL_SYMBOLS}" PARENT_SCOPE)
  endif()

  if(DEFINED _DEBUG_INFORMATION_FORMAT)
    set(format "${_DEBUG_INFORMATION_FORMAT}")
    if(format STREQUAL "C7 Compatible (/Z7)")
      set(format_flag "/Z7")
    elseif(format STREQUAL "Program Database (/Zi)")
      set(format_flag "/Zi")
    elseif(format STREQUAL "Program Database for Edit And Continue (/ZI)")
      set(format_flag "/ZI")
    elseif(NOT format STREQUAL "None")
      message(FATAL_ERROR "Unsupported value for DEBUG_INFORMATION_FORMAT: \"${format}\"")
    endif()
    set(JUCER_DEBUG_INFORMATION_FORMAT_FLAG_${config} "${format_flag}" PARENT_SCOPE)
  endif()

  if(DEFINED _WARNING_LEVEL)
    set(warning_level "${_WARNING_LEVEL}")
    if(warning_level STREQUAL "Low")
      set(level 2)
    elseif(warning_level STREQUAL "Medium")
      set(level 3)
    elseif(warning_level STREQUAL "High")
      set(level 4)
    else()
      message(FATAL_ERROR "Unsupported value for WARNING_LEVEL: \"${warning_level}\"")
    endif()
    set(JUCER_WARNING_LEVEL_FLAG_${config} "/W${level}" PARENT_SCOPE)
  endif()

  if(DEFINED _TREAT_WARNINGS_AS_ERRORS)
    set(JUCER_TREAT_WARNINGS_AS_ERRORS_${config} "${_TREAT_WARNINGS_AS_ERRORS}"
      PARENT_SCOPE
    )
  endif()

  if(DEFINED _RUNTIME_LIBRARY)
    set(library "${_RUNTIME_LIBRARY}")
    if(library STREQUAL "Use DLL runtime")
      if(is_debug)
        set(flag "/MDd")
      else()
        set(flag "/MD")
      endif()
    elseif(library STREQUAL "Use static runtime")
      if(is_debug)
        set(flag "/MTd")
      else()
        set(flag "/MT")
      endif()
    elseif(NOT library STREQUAL "(Default)")
      message(FATAL_ERROR "Unsupported value for RUNTIME_LIBRARY: \"${library}\"")
    endif()
    set(JUCER_RUNTIME_LIBRARY_FLAG_${config} "${flag}" PARENT_SCOPE)
  endif()

  if(DEFINED _WHOLE_PROGRAM_OPTIMISATION)
    set(optimisation "${_WHOLE_PROGRAM_OPTIMISATION}")
    if(optimisation STREQUAL "Always disable")
      set(JUCER_ALWAYS_DISABLE_WPO_${config} TRUE PARENT_SCOPE)
    elseif(NOT optimisation STREQUAL "Enable when possible")
      message(FATAL_ERROR
        "Unsupported value for WHOLE_PROGRAM_OPTIMISATION: \"${optimisation}\""
      )
    endif()
  endif()

  if(DEFINED _MULTI_PROCESSOR_COMPILATION)
    set(JUCER_MULTI_PROCESSOR_COMPILATION_${config}
      "${_MULTI_PROCESSOR_COMPILATION}" PARENT_SCOPE
    )
  endif()

  if(DEFINED _INCREMENTAL_LINKING)
    set(JUCER_INCREMENTAL_LINKING_${config} "${_INCREMENTAL_LINKING}" PARENT_SCOPE)
  endif()

  if(DEFINED _FORCE_GENERATION_OF_DEBUG_SYMBOLS)
    set(JUCER_FORCE_GENERATION_OF_DEBUG_SYMBOLS_${config}
      "${_FORCE_GENERATION_OF_DEBUG_SYMBOLS}" PARENT_SCOPE
    )
  endif()

  if(DEFINED _PREBUILD_COMMAND)
    set(script_content "${_PREBUILD_COMMAND}")
    configure_file("${Reprojucer_templates_DIR}/script.in"
      "prebuild_${config}.cmd" @ONLY
    )
    set(JUCER_PREBUILD_COMMAND_${config}
      "${CMAKE_CURRENT_BINARY_DIR}/prebuild_${config}.cmd" PARENT_SCOPE
    )
  endif()

  if(DEFINED _POSTBUILD_COMMAND)
    set(script_content "${_POSTBUILD_COMMAND}")
    configure_file("${Reprojucer_templates_DIR}/script.in"
      "postbuild_${config}.cmd" @ONLY
    )
    set(JUCER_POSTBUILD_COMMAND_${config}
      "${CMAKE_CURRENT_BINARY_DIR}/postbuild_${config}.cmd" PARENT_SCOPE
    )
  endif()

  if(DEFINED _GENERATE_MANIFEST)
    set(JUCER_GENERATE_MANIFEST_${config} "${_GENERATE_MANIFEST}" PARENT_SCOPE)
  endif()

  if(DEFINED _CHARACTER_SET)
    set(character_sets "Default" "MultiByte" "Unicode")
    if(_CHARACTER_SET IN_LIST character_sets)
      set(JUCER_CHARACTER_SET_${config} "${_CHARACTER_SET}" PARENT_SCOPE)
    else()
      message(FATAL_ERROR "Unsupported value for CHARACTER_SET: \"${_CHARACTER_SET}\"")
    endif()
  endif()

  if(DEFINED _ARCHITECTURE AND exporter MATCHES "^Visual Studio 201(9|7|5|3)$")
    if(_ARCHITECTURE STREQUAL "32-bit")
      set(wants_x64 FALSE)
    elseif(_ARCHITECTURE STREQUAL "x64")
      set(wants_x64 TRUE)
    else()
      message(FATAL_ERROR "Unsupported value for ARCHITECTURE: \"${_ARCHITECTURE}\"")
    endif()
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
      set(is_x64 TRUE)
    else()
      set(is_x64 FALSE)
    endif()
    set(error_message "")
    if(wants_x64 AND NOT is_x64)
      set(error_message
        "Cannot honor \"ARCHITECTURE x64\" while targeting the Win32 architecture. "
      )
      if(CMAKE_GENERATOR MATCHES "^Visual Studio")
        string(APPEND error_message "You must call `cmake -G\"${CMAKE_GENERATOR} Win64\"`"
          " or `cmake -G\"${CMAKE_GENERATOR}\" -A x64` in order to build for x64."
        )
      else()
        string(APPEND error_message "You must use a compiler that targets x64 in order to"
          " build for x64."
        )
      endif()
    elseif(NOT wants_x64 AND is_x64)
      set(error_message
        "Cannot honor \"ARCHITECTURE 32-bit\" while targeting the x64 architecture. "
      )
      if(CMAKE_GENERATOR MATCHES "^Visual Studio")
      string(FIND "${CMAKE_GENERATOR}" " Win64" length REVERSE)
      string(SUBSTRING "${CMAKE_GENERATOR}" 0 ${length} 32_bit_generator)
        string(APPEND error_message "You must call `cmake -G\"${32_bit_generator}\"` or"
        " `cmake -G\"${32_bit_generator}\" -A Win32` in order to build for 32-bit."
        )
      else()
        string(APPEND error_message "You must use a compiler that targets x86 in order to"
          " build for 32-bit."
        )
      endif()
    endif()
    if(error_message)
      message(FATAL_ERROR "${error_message}")
    endif()
  endif()

  if(DEFINED _ARCHITECTURE AND exporter STREQUAL "Linux Makefile")
    set(architecture "${_ARCHITECTURE}")
    if(architecture STREQUAL "(Default)" OR architecture STREQUAL "Native")
      set(architecture_flag "-march=native")
    elseif(architecture STREQUAL "<None>")
      set(architecture_flag "")
    elseif(architecture STREQUAL "32-bit (-m32)")
      set(architecture_flag "-m32")
    elseif(architecture STREQUAL "64-bit (-m64)")
      set(architecture_flag "-m64")
    elseif(architecture STREQUAL "ARM v6")
      set(architecture_flag "-march=armv6")
    elseif(architecture STREQUAL "ARM v7")
      set(architecture_flag "-march=armv7")
    else()
      message(FATAL_ERROR "Unsupported value for ARCHITECTURE: \"${architecture}\"")
    endif()
    set(JUCER_ARCHITECTURE_FLAG_${config} "${architecture_flag}" PARENT_SCOPE)
  endif()

  if(DEFINED _ARCHITECTURE AND exporter MATCHES "^Code::Blocks \((Windows|Linux)\)$")
    set(architecture "${_ARCHITECTURE}")
    if(architecture STREQUAL "32-bit (-m32)")
      set(architecture_flag "-m32")
    elseif(architecture STREQUAL "64-bit (-m64)")
      set(architecture_flag "-m64")
    elseif(architecture STREQUAL "ARM v6")
      set(architecture_flag "-march=armv6")
    elseif(architecture STREQUAL "ARM v7")
      set(architecture_flag "-march=armv7")
    else()
      message(FATAL_ERROR "Unsupported value for ARCHITECTURE: \"${architecture}\"")
    endif()
    set(JUCER_ARCHITECTURE_FLAG_${config} "${architecture_flag}" PARENT_SCOPE)
  endif()

endfunction()


function(jucer_project_end)

  unset(current_exporter)
  foreach(exporter_index RANGE 7)
    list(GET Reprojucer_supported_exporters_conditions ${exporter_index} condition)
    if(${condition})
      if(DEFINED current_exporter)
        message(FATAL_ERROR "There is already a current exporter: ${current_exporter}")
      else()
        list(GET Reprojucer_supported_exporters ${exporter_index} exporter)
        set(current_exporter "${exporter}")
      endif()
    endif()
  endforeach()

  if(NOT DEFINED current_exporter)
    set(exporters_list "")
    foreach(exporter IN LISTS Reprojucer_supported_exporters)
      string(APPEND exporters_list "\n  - ${exporter}")
    endforeach()
    message(FATAL_ERROR "Reprojucer.cmake doesn't support any export target for your "
      "current platform. It supports the following export targets:${exporters_list}\n"
      "If you think Reprojucer.cmake should support another export target, please create "
      "an issue on GitHub: https://github.com/McMartin/FRUT/issues/new\n"
    )
  endif()

  if(NOT current_exporter IN_LIST JUCER_PROJECT_EXPORT_TARGETS)
    message(FATAL_ERROR "You must call jucer_export_target(\"${current_exporter}\") "
      "before calling jucer_project_end()."
    )
  endif()

  if(NOT JUCER_PROJECT_CONFIGURATIONS)
    message(FATAL_ERROR "You must call "
      "jucer_export_target_configuration(\"${current_exporter}\") before "
      "calling jucer_project_end()."
    )
  endif()

  if(DEFINED CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES "${JUCER_PROJECT_CONFIGURATIONS}" PARENT_SCOPE)
  else()
    if(NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
      list(GET JUCER_PROJECT_CONFIGURATIONS 0 first_configuration)
      message(STATUS
        "Setting CMAKE_BUILD_TYPE to \"${first_configuration}\" as it was not specified."
      )
      set(CMAKE_BUILD_TYPE "${first_configuration}")
      set(CMAKE_BUILD_TYPE "${first_configuration}" PARENT_SCOPE)
    elseif(NOT CMAKE_BUILD_TYPE IN_LIST JUCER_PROJECT_CONFIGURATIONS)
      message(FATAL_ERROR "Undefined build configuration: ${CMAKE_BUILD_TYPE}\n"
        "Defined build configurations: ${JUCER_PROJECT_CONFIGURATIONS}"
      )
    endif()
  endif()

  if(APPLE)
    foreach(config IN LISTS JUCER_PROJECT_CONFIGURATIONS)
      set(sdk_version "${JUCER_OSX_BASE_SDK_VERSION_${config}}")
      execute_process(
        COMMAND "xcrun" "--sdk" "macosx${sdk_version}" "--show-sdk-path"
        OUTPUT_VARIABLE sdk_path
        OUTPUT_STRIP_TRAILING_WHITESPACE
      )
      if(IS_DIRECTORY "${sdk_path}")
        set(JUCER_MACOSX_SDK_PATH_${config} "${sdk_path}")
      else()
        message(WARNING "Running `xcrun --sdk macosx${sdk_version} --show-sdk-path`"
          " didn't output a valid directory."
        )
      endif()
    endforeach()
  endif()

  _FRUT_check_SDK_folders("${current_exporter}")

  _FRUT_generate_AppConfig_header()
  _FRUT_generate_JuceHeader_header()

  if(DEFINED JUCER_SMALL_ICON OR DEFINED JUCER_LARGE_ICON)
    unset(icon_filename)
    if(APPLE)
      _FRUT_generate_icon_file("icns" "${CMAKE_CURRENT_BINARY_DIR}" icon_filename)
    elseif(WIN32)
      _FRUT_generate_icon_file("ico" "${CMAKE_CURRENT_BINARY_DIR}" icon_filename)
    endif()

    if(DEFINED icon_filename)
      set(JUCER_ICON_FILE "${CMAKE_CURRENT_BINARY_DIR}/${icon_filename}")
      source_group("Juce Library Code" FILES "${JUCER_ICON_FILE}")
    endif()
  endif()

  if(WIN32 AND NOT JUCER_PROJECT_TYPE STREQUAL "Static Library")
    set(JUCER_RESOURCES_RC_FILE "${CMAKE_CURRENT_BINARY_DIR}/resources.rc")
    _FRUT_generate_resources_rc_file("${JUCER_RESOURCES_RC_FILE}")
    source_group("Juce Library Code" FILES "${JUCER_RESOURCES_RC_FILE}")
  endif()

  source_group("Juce Library Code"
    REGULAR_EXPRESSION "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode/*"
  )

  string(REGEX REPLACE "[^A-Za-z0-9_.+-]" "_" target "${JUCER_PROJECT_NAME}")

  if(APPLE)
    set_source_files_properties(${JUCER_PROJECT_XCODE_RESOURCES} ${JUCER_ICON_FILE}
      PROPERTIES MACOSX_PACKAGE_LOCATION "Resources"
    )
  else()
    unset(JUCER_PROJECT_XCODE_RESOURCES)
  endif()

  set_source_files_properties(${JUCER_PROJECT_MODULES_BROWSABLE_FILES}
    PROPERTIES HEADER_FILE_ONLY TRUE
  )

  set(modules_sources "")
  foreach(module_name IN LISTS JUCER_PROJECT_MODULES)
    set(module_sources "${JUCER_PROJECT_MODULE_${module_name}_SOURCES}")
    list(APPEND modules_sources ${module_sources})
  endforeach()

  set(all_sources
    ${JUCER_PROJECT_FILES}
    ${modules_sources}
    ${JUCER_PROJECT_MODULES_BROWSABLE_FILES}
    ${JUCER_ICON_FILE}
    ${JUCER_RESOURCES_RC_FILE}
  )

  if(JUCER_PROJECT_TYPE STREQUAL "Console Application")
    add_executable(${target} ${all_sources})
    _FRUT_set_output_directory_properties(${target} "ConsoleApp")
    _FRUT_set_output_name_properties(${target})
    _FRUT_set_compiler_and_linker_settings(${target})
    _FRUT_add_extra_commands(${target} "${current_exporter}")
    _FRUT_link_osx_frameworks(${target})
    _FRUT_set_custom_xcode_flags(${target})

  elseif(JUCER_PROJECT_TYPE STREQUAL "GUI Application")
    add_executable(${target} WIN32 MACOSX_BUNDLE ${all_sources})
    _FRUT_generate_plist_file(${target} "App" "APPL" "????")
    _FRUT_set_output_directory_properties(${target} "App")
    _FRUT_set_output_name_properties(${target})
    _FRUT_set_compiler_and_linker_settings(${target})
    _FRUT_add_extra_commands(${target} "${current_exporter}")
    _FRUT_link_osx_frameworks(${target})
    _FRUT_add_xcode_resource_folders(${target})
    _FRUT_set_custom_xcode_flags(${target})

  elseif(JUCER_PROJECT_TYPE STREQUAL "Static Library")
    add_library(${target} STATIC ${all_sources})
    _FRUT_set_output_directory_properties(${target} "Static Library")
    _FRUT_set_output_name_properties(${target})
    _FRUT_set_compiler_and_linker_settings(${target})
    _FRUT_add_extra_commands(${target} "${current_exporter}")
    _FRUT_set_custom_xcode_flags(${target})

  elseif(JUCER_PROJECT_TYPE STREQUAL "Dynamic Library")
    add_library(${target} SHARED ${all_sources})
    _FRUT_set_output_directory_properties(${target} "Dynamic Library")
    _FRUT_set_output_name_properties(${target})
    _FRUT_set_compiler_and_linker_settings(${target})
    _FRUT_add_extra_commands(${target} "${current_exporter}")
    _FRUT_link_osx_frameworks(${target})
    _FRUT_set_custom_xcode_flags(${target})

  elseif(JUCER_PROJECT_TYPE STREQUAL "Audio Plug-in")
    set(AudioUnit_sources "")
    set(AudioUnitv3_sources "")
    set(AAX_sources "")
    set(RTAS_sources "")
    set(VST_sources "")
    set(VST3_sources "")
    set(Standalone_sources "")
    set(Unity_sources "")
    set(SharedCode_sources "")
    foreach(src_file IN LISTS JUCER_PROJECT_FILES modules_sources)
      # See Project::getTargetTypeFromFilePath()
      # in JUCE/extras/Projucer/Source/Project/jucer_Project.cpp
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
      elseif(src_file MATCHES "_Unity[._]")
        list(APPEND Unity_sources "${src_file}")
      else()
        list(APPEND SharedCode_sources "${src_file}")
      endif()
    endforeach()

    set(shared_code_target "${target}_Shared_Code")
    add_library(${shared_code_target} STATIC
      ${SharedCode_sources}
      ${JUCER_PROJECT_MODULES_BROWSABLE_FILES}
      ${JUCER_ICON_FILE}
      ${JUCER_RESOURCES_RC_FILE}
    )
    _FRUT_set_output_directory_properties(${shared_code_target} "Shared Code")
    _FRUT_set_output_name_properties(${shared_code_target})
    _FRUT_set_compiler_and_linker_settings(${shared_code_target})
    _FRUT_add_extra_commands(${shared_code_target} "${current_exporter}")
    target_compile_definitions(${shared_code_target} PRIVATE "JUCE_SHARED_CODE=1")
    _FRUT_set_JucePlugin_Build_defines(${shared_code_target} "SharedCodeTarget")
    _FRUT_set_custom_xcode_flags(${shared_code_target})

    if(JUCER_BUILD_VST)
      set(vst_target "${target}_VST")
      add_library(${vst_target} MODULE
        ${VST_sources}
        ${JUCER_PROJECT_XCODE_RESOURCES}
        ${JUCER_ICON_FILE}
        ${JUCER_RESOURCES_RC_FILE}
      )
      target_link_libraries(${vst_target} PRIVATE ${shared_code_target})
      _FRUT_generate_plist_file(${vst_target} "VST" "BNDL" "????")
      _FRUT_set_bundle_properties(${vst_target} "vst")
      _FRUT_set_output_directory_properties(${vst_target} "VST")
      _FRUT_set_output_name_properties(${vst_target})
      _FRUT_set_compiler_and_linker_settings(${vst_target})
      _FRUT_add_extra_commands(${vst_target} "${current_exporter}")
      if(APPLE)
        _FRUT_install_to_plugin_binary_location(${vst_target} "VST"
          "$ENV{HOME}/Library/Audio/Plug-Ins/VST"
        )
      elseif(MSVC)
        if(CMAKE_SIZEOF_VOID_P EQUAL 8) # 64-bit
          set(env_var "ProgramW6432")
        else()
          set(env_var "programfiles(x86)")
        endif()
        _FRUT_install_to_plugin_binary_location(${vst_target} "VST"
          "$ENV{${env_var}}/Steinberg/Vstplugins"
        )
      endif()
      _FRUT_set_JucePlugin_Build_defines(${vst_target} "VSTPlugIn")
      _FRUT_link_osx_frameworks(${vst_target})
      _FRUT_add_xcode_resource_folders(${vst_target})
      _FRUT_set_custom_xcode_flags(${vst_target})
      unset(vst_target)
    endif()

    if(JUCER_BUILD_VST3 AND (APPLE OR MSVC))
      set(vst3_target "${target}_VST3")
      add_library(${vst3_target} MODULE
        ${VST3_sources}
        ${JUCER_PROJECT_XCODE_RESOURCES}
        ${JUCER_ICON_FILE}
        ${JUCER_RESOURCES_RC_FILE}
      )
      target_link_libraries(${vst3_target} PRIVATE ${shared_code_target})
      _FRUT_generate_plist_file(${vst3_target} "VST3" "BNDL" "????")
      _FRUT_set_bundle_properties(${vst3_target} "vst3")
      _FRUT_set_output_directory_properties(${vst3_target} "VST3")
      _FRUT_set_output_name_properties(${vst3_target})
      _FRUT_set_compiler_and_linker_settings(${vst3_target})
      _FRUT_add_extra_commands(${vst3_target} "${current_exporter}")
      if(APPLE)
        _FRUT_install_to_plugin_binary_location(${vst3_target} "VST3"
          "$ENV{HOME}/Library/Audio/Plug-Ins/VST3"
        )
      elseif(MSVC)
        set_property(TARGET ${vst3_target} PROPERTY SUFFIX ".vst3")
        if(CMAKE_SIZEOF_VOID_P EQUAL 8) # 64-bit
          set(common_files_env_var "CommonProgramW6432")
        else()
          set(common_files_env_var "CommonProgramFiles(x86)")
        endif()
        _FRUT_install_to_plugin_binary_location(${vst3_target} "VST3"
          "$ENV{${common_files_env_var}}/VST3"
        )
      endif()
      _FRUT_set_JucePlugin_Build_defines(${vst3_target} "VST3PlugIn")
      _FRUT_link_osx_frameworks(${vst3_target})
      _FRUT_add_xcode_resource_folders(${vst3_target})
      _FRUT_set_custom_xcode_flags(${vst3_target})
      unset(vst3_target)
    endif()

    if(JUCER_BUILD_AUDIOUNIT AND APPLE)
      set(au_target "${target}_AU")
      add_library(${au_target} MODULE
        ${AudioUnit_sources}
        ${JUCER_PROJECT_XCODE_RESOURCES}
        ${JUCER_ICON_FILE}
      )
      target_link_libraries(${au_target} PRIVATE ${shared_code_target})

      unset(rez_inputs)
      foreach(src_file IN LISTS AudioUnit_sources)
        get_filename_component(file_extension "${src_file}" EXT)
        if(file_extension STREQUAL ".r")
          list(APPEND rez_inputs "${src_file}")
        endif()
      endforeach()
      if(DEFINED rez_inputs)
        _FRUT_add_Rez_command_to_AU_plugin(${au_target} ${rez_inputs})
      endif()

      _FRUT_generate_plist_file(${au_target} "AU" "BNDL" "????")
      _FRUT_set_bundle_properties(${au_target} "component")
      _FRUT_set_output_directory_properties(${au_target} "AU")
      _FRUT_set_output_name_properties(${au_target})
      _FRUT_set_compiler_and_linker_settings(${au_target})
      _FRUT_add_extra_commands(${au_target} "${current_exporter}")
      _FRUT_install_to_plugin_binary_location(${au_target} "AU"
        "$ENV{HOME}/Library/Audio/Plug-Ins/Components"
      )
      _FRUT_set_JucePlugin_Build_defines(${au_target} "AudioUnitPlugIn")
      _FRUT_link_osx_frameworks(${au_target} "AudioUnit" "CoreAudioKit")
      _FRUT_add_xcode_resource_folders(${au_target})
      _FRUT_set_custom_xcode_flags(${au_target})
      unset(au_target)
    endif()

    if(JUCER_BUILD_AUDIOUNIT_V3 AND APPLE)
      set(auv3_target "${target}_AUv3_AppExtension")
      add_library(${auv3_target} MODULE
        ${AudioUnitv3_sources}
        ${JUCER_PROJECT_XCODE_RESOURCES}
        ${JUCER_ICON_FILE}
      )
      target_link_libraries(${auv3_target} PRIVATE ${shared_code_target})
      _FRUT_generate_plist_file_AUv3(${auv3_target} "AUv3_AppExtension")

      if(CMAKE_GENERATOR STREQUAL "Xcode")
        configure_file("${Reprojucer_templates_DIR}/AUv3.entitlements"
          "${target}.entitlements" COPYONLY
        )
        set_property(TARGET ${auv3_target} PROPERTY
          XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS
          "${CMAKE_CURRENT_BINARY_DIR}/${target}.entitlements"
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
      _FRUT_set_output_name_properties(${auv3_target})
      _FRUT_set_compiler_and_linker_settings(${auv3_target})
      _FRUT_add_extra_commands(${auv3_target} "${current_exporter}")
      _FRUT_set_JucePlugin_Build_defines(${auv3_target} "AudioUnitv3PlugIn")
      _FRUT_link_osx_frameworks(
        ${auv3_target} "AudioUnit" "CoreAudioKit" "AVFoundation"
      )
      _FRUT_add_xcode_resource_folders(${auv3_target})
      _FRUT_set_custom_xcode_flags(${auv3_target})
      unset(auv3_target)
    endif()

    if(JUCER_BUILD_RTAS AND (APPLE OR MSVC))
      set(rtas_target "${target}_RTAS")
      add_library(${rtas_target} MODULE
        ${RTAS_sources}
        ${JUCER_PROJECT_XCODE_RESOURCES}
        ${JUCER_ICON_FILE}
        ${JUCER_RESOURCES_RC_FILE}
      )
      target_link_libraries(${rtas_target} PRIVATE ${shared_code_target})
      _FRUT_generate_plist_file(${rtas_target} "RTAS" "TDMw" "PTul")
      _FRUT_set_bundle_properties(${rtas_target} "dpm")
      _FRUT_set_output_directory_properties(${rtas_target} "RTAS")
      _FRUT_set_output_name_properties(${rtas_target})
      _FRUT_set_compiler_and_linker_settings(${rtas_target})
      _FRUT_add_extra_commands(${rtas_target} "${current_exporter}")
      if(APPLE)
        # See XcodeProjectExporter::XcodeTarget::getTargetExtraHeaderSearchPaths()
        # in JUCE/extras/Projucer/Source/ProjectSaving/jucer_ProjectExport_Xcode.h
        target_include_directories(${rtas_target} PRIVATE
          "$(DEVELOPER_DIR)/Headers/FlatCarbon"
          "$(SDKROOT)/Developer/Headers/FlatCarbon"
        )
        foreach(include_dir IN ITEMS
          "AlturaPorts/TDMPlugIns/PlugInLibrary/Controls"
          "AlturaPorts/TDMPlugIns/PlugInLibrary/CoreClasses"
          "AlturaPorts/TDMPlugIns/PlugInLibrary/DSPClasses"
          "AlturaPorts/TDMPlugIns/PlugInLibrary/EffectClasses"
          "AlturaPorts/TDMPlugIns/PlugInLibrary/MacBuild"
          "AlturaPorts/TDMPlugIns/PlugInLibrary/Meters"
          "AlturaPorts/TDMPlugIns/PlugInLibrary/ProcessClasses"
          "AlturaPorts/TDMPlugIns/PlugInLibrary/ProcessClasses/Interfaces"
          "AlturaPorts/TDMPlugIns/PlugInLibrary/RTASP_Adapt"
          "AlturaPorts/TDMPlugIns/PlugInLibrary/Utilities"
          "AlturaPorts/TDMPlugIns/PlugInLibrary/ViewClasses"
          "AlturaPorts/TDMPlugIns/DSPManager/**"
          "AlturaPorts/TDMPlugIns/SupplementalPlugInLib/Encryption"
          "AlturaPorts/TDMPlugIns/SupplementalPlugInLib/GraphicsExtensions"
          "AlturaPorts/TDMPlugIns/common/**"
          "AlturaPorts/TDMPlugIns/common/PI_LibInterface"
          "AlturaPorts/TDMPlugIns/PACEProtection/**"
          "AlturaPorts/TDMPlugIns/SignalProcessing/**"
          "AlturaPorts/OMS/Headers"
          "AlturaPorts/Fic/Interfaces/**"
          "AlturaPorts/Fic/Source/SignalNets"
          "AlturaPorts/DSIPublicInterface/PublicHeaders"
          "DAEWin/Include"
          "AlturaPorts/DigiPublic/Interfaces"
          "AlturaPorts/DigiPublic"
          "AlturaPorts/NewFileLibs/DOA"
          "AlturaPorts/NewFileLibs/Cmn"
          "xplat/AVX/avx2/avx2sdk/inc"
          "xplat/AVX/avx2/avx2sdk/utils"
        )
          target_include_directories(${rtas_target} PRIVATE
            "${JUCER_RTAS_SDK_FOLDER}/${include_dir}"
          )
        endforeach()
        target_link_libraries(${rtas_target} PRIVATE
          "${JUCER_RTAS_SDK_FOLDER}/MacBag/Libs/Debug/libPluginLibrary.a"
          "${JUCER_RTAS_SDK_FOLDER}/MacBag/Libs/Release/libPluginLibrary.a"
        )

        _FRUT_install_to_plugin_binary_location(${rtas_target} "RTAS"
          "/Library/Application Support/Digidesign/Plug-Ins/"
        )
      elseif(MSVC)
        set_property(TARGET ${rtas_target} PROPERTY SUFFIX ".dpm")
        target_compile_definitions(${rtas_target} PRIVATE
          "JucePlugin_WinBag_path=\"${JUCER_RTAS_SDK_FOLDER}/WinBag\""
        )
        foreach(src_file IN LISTS RTAS_sources)
          get_filename_component(src_file_basename "${src_file}" NAME)
          if(src_file_basename MATCHES "^(include_)?juce_audio_plugin_client_RTAS_")
            # Calling Convention: __stdcall (/Gz)
            set_source_files_properties("${src_file}" PROPERTIES COMPILE_FLAGS "/Gz")
          endif()
        endforeach()
        string(CONCAT module_definition_file
          "${JUCER_PROJECT_MODULE_juce_audio_plugin_client_PATH}/"
          "juce_audio_plugin_client/RTAS/juce_RTAS_WinExports.def"
        )
        target_sources(${rtas_target} PRIVATE "${module_definition_file}")
        string(CONCAT rtas_link_flags
          " /DELAYLOAD:DAE.dll"
          " /DELAYLOAD:DigiExt.dll"
          " /DELAYLOAD:DSI.dll"
          " /DELAYLOAD:PluginLib.dll"
          " /DELAYLOAD:DSPManager.dll"
          " /DELAYLOAD:DSPManagerClientLib.dll"
          " /DELAYLOAD:RTASClientLib.dll"
        )
        set_property(TARGET ${rtas_target} APPEND_STRING PROPERTY
          LINK_FLAGS "${rtas_link_flags}"
        )
        # See MSVCProjectExporterBase::MSVCTargetBase::getExtraSearchPaths()
        # in JUCE/extras/Projucer/Source/ProjectSaving/jucer_ProjectExport_MSVC.h
        foreach(include_dir IN ITEMS
          "AlturaPorts/TDMPlugins/PluginLibrary/EffectClasses"
          "AlturaPorts/TDMPlugins/PluginLibrary/ProcessClasses"
          "AlturaPorts/TDMPlugins/PluginLibrary/ProcessClasses/Interfaces"
          "AlturaPorts/TDMPlugins/PluginLibrary/Utilities"
          "AlturaPorts/TDMPlugins/PluginLibrary/RTASP_Adapt"
          "AlturaPorts/TDMPlugins/PluginLibrary/CoreClasses"
          "AlturaPorts/TDMPlugins/PluginLibrary/Controls"
          "AlturaPorts/TDMPlugins/PluginLibrary/Meters"
          "AlturaPorts/TDMPlugins/PluginLibrary/ViewClasses"
          "AlturaPorts/TDMPlugins/PluginLibrary/DSPClasses"
          "AlturaPorts/TDMPlugins/PluginLibrary/Interfaces"
          "AlturaPorts/TDMPlugins/common"
          "AlturaPorts/TDMPlugins/common/Platform"
          "AlturaPorts/TDMPlugins/common/Macros"
          "AlturaPorts/TDMPlugins/SignalProcessing/Public"
          "AlturaPorts/TDMPlugIns/DSPManager/Interfaces"
          "AlturaPorts/SADriver/Interfaces"
          "AlturaPorts/DigiPublic/Interfaces"
          "AlturaPorts/DigiPublic"
          "AlturaPorts/Fic/Interfaces/DAEClient"
          "AlturaPorts/NewFileLibs/Cmn"
          "AlturaPorts/NewFileLibs/DOA"
          "AlturaPorts/AlturaSource/PPC_H"
          "AlturaPorts/AlturaSource/AppSupport"
          "AvidCode/AVX2sdk/AVX/avx2/avx2sdk/inc"
          "xplat/AVX/avx2/avx2sdk/inc"
        )
          target_include_directories(${rtas_target} PRIVATE
            "${JUCER_RTAS_SDK_FOLDER}/${include_dir}"
          )
        endforeach()

        if(CMAKE_SIZEOF_VOID_P EQUAL 8) # 64-bit
          set(common_files_env_var "CommonProgramW6432")
        else()
          set(common_files_env_var "CommonProgramFiles(x86)")
        endif()
        _FRUT_install_to_plugin_binary_location(${rtas_target} "RTAS"
          "$ENV{${common_files_env_var}}/Digidesign/DAE/Plug-Ins"
        )
      endif()
      _FRUT_set_JucePlugin_Build_defines(${rtas_target} "RTASPlugIn")
      _FRUT_link_osx_frameworks(${rtas_target})
      _FRUT_add_xcode_resource_folders(${rtas_target})
      _FRUT_set_custom_xcode_flags(${rtas_target})
      unset(rtas_target)
    endif()

    if(JUCER_BUILD_AAX AND (APPLE OR MSVC))
      set(aax_target "${target}_AAX")
      add_library(${aax_target} MODULE
        ${AAX_sources}
        ${JUCER_PROJECT_XCODE_RESOURCES}
        ${JUCER_ICON_FILE}
        ${JUCER_RESOURCES_RC_FILE}
      )
      target_link_libraries(${aax_target} PRIVATE ${shared_code_target})
      _FRUT_generate_plist_file(${aax_target} "AAX" "TDMw" "PTul")
      _FRUT_set_bundle_properties(${aax_target} "aaxplugin")
      _FRUT_set_output_directory_properties(${aax_target} "AAX")
      _FRUT_set_output_name_properties(${aax_target})
      _FRUT_set_compiler_and_linker_settings(${aax_target})
      _FRUT_add_extra_commands(${aax_target} "${current_exporter}")
      if(APPLE)
        foreach(config IN LISTS JUCER_PROJECT_CONFIGURATIONS)
          if(JUCER_CONFIGURATION_IS_DEBUG_${config})
            set(aax_config "Debug")
          else()
            set(aax_config "Release")
          endif()
          if(JUCER_CXX_LIBRARY_${config} STREQUAL "libc++"
              OR JUCER_OSX_DEPLOYMENT_TARGET_${config} VERSION_GREATER 10.8)
            set(aax_libcpp "_libcpp")
          else()
            set(aax_libcpp "")
          endif()
          set(aax_lib
            "${JUCER_AAX_SDK_FOLDER}/Libs/${aax_config}/libAAXLibrary${aax_libcpp}.a"
          )
          target_link_libraries(${aax_target} PRIVATE $<$<CONFIG:${config}>:${aax_lib}>)
        endforeach()

        _FRUT_install_to_plugin_binary_location(${aax_target} "AAX"
          "/Library/Application Support/Avid/Audio/Plug-Ins"
        )
      elseif(MSVC)
        set_property(TARGET ${aax_target} PROPERTY SUFFIX ".aaxdll")
        target_compile_definitions(${aax_target} PRIVATE
          "JucePlugin_AAXLibs_path=\"${JUCER_AAX_SDK_FOLDER}/Libs\""
        )

        set(all_confs_output_name "")
        foreach(config IN LISTS JUCER_PROJECT_CONFIGURATIONS)
          string(TOUPPER "${config}" upper_config)
          get_target_property(output_name ${aax_target} OUTPUT_NAME_${upper_config})
          string(APPEND all_confs_output_name $<$<CONFIG:${config}>:${output_name}>)
        endforeach()
        set(all_confs_bundle
          "$<TARGET_FILE_DIR:${aax_target}>/${all_confs_output_name}.aaxplugin"
        )
        if(CMAKE_SIZEOF_VOID_P EQUAL 8) # 64-bit
          set(arch_dir "x64")
        else()
          set(arch_dir "Win32")
        endif()
        add_custom_command(TARGET ${aax_target} PRE_BUILD
          COMMAND
          "${CMAKE_COMMAND}" "-E" "make_directory"
          "${all_confs_bundle}/Contents/${arch_dir}"
        )
        add_custom_command(TARGET ${aax_target} POST_BUILD
          COMMAND
          "${CMAKE_COMMAND}" "-E" "copy_if_different"
          "$<TARGET_FILE:${aax_target}>"
          "${all_confs_bundle}/Contents/${arch_dir}/${all_confs_output_name}.aaxplugin"
        )
        if(DEFINED JUCER_ICON_FILE)
          set(plugin_icon "${JUCER_ICON_FILE}")
        else()
          set(plugin_icon "${JUCER_AAX_SDK_FOLDER}/Utilities/PlugIn.ico")
        endif()
        add_custom_command(TARGET ${aax_target} POST_BUILD
          COMMAND
          "${JUCER_AAX_SDK_FOLDER}/Utilities/CreatePackage.bat"
          "${all_confs_bundle}/Contents/${arch_dir}"
          "${plugin_icon}"
        )

        if(CMAKE_SIZEOF_VOID_P EQUAL 8) # 64-bit
          set(common_files_env_var "CommonProgramW6432")
        else()
          set(common_files_env_var "CommonProgramFiles(x86)")
        endif()
        unset(all_confs_destination)
        foreach(config IN LISTS JUCER_PROJECT_CONFIGURATIONS)
          if(DEFINED JUCER_AAX_BINARY_LOCATION_${config})
            set(destination "${JUCER_AAX_BINARY_LOCATION_${config}}")
          else()
            set(destination "$ENV{${common_files_env_var}}/Avid/Audio/Plug-Ins")
          endif()
          if(JUCER_ENABLE_PLUGIN_COPY_STEP_${config})
            string(APPEND all_confs_destination
              $<$<CONFIG:${config}>:$<SHELL_PATH:${destination}>>
            )
          endif()
        endforeach()
        if(DEFINED all_confs_destination)
          add_custom_command(TARGET ${aax_target} POST_BUILD
            COMMAND
            "xcopy"
            "$<SHELL_PATH:${all_confs_bundle}>"
            "${all_confs_destination}\\${all_confs_output_name}.aaxplugin\\"
            "/E" "/H" "/K" "/R" "/Y"
          )
        endif()
      endif()
      _FRUT_set_JucePlugin_Build_defines(${aax_target} "AAXPlugIn")
      _FRUT_link_osx_frameworks(${aax_target})
      _FRUT_add_xcode_resource_folders(${aax_target})
      _FRUT_set_custom_xcode_flags(${aax_target})
      unset(aax_target)
    endif()

    if(DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5.0.0)
      if(JUCER_BUILD_AUDIOUNIT_V3)
        set(juce4_standalone ON)
      endif()
    elseif(JUCER_BUILD_STANDALONE_PLUGIN)
      set(juce5_standalone ON)
    endif()
    if(juce4_standalone OR juce5_standalone)
      if(juce4_standalone)
        set(standalone_target "${target}_AUv3_Standalone")
      else()
        set(standalone_target "${target}_StandalonePlugin")
      endif()
      add_executable(${standalone_target} WIN32 MACOSX_BUNDLE
        ${Standalone_sources}
        ${JUCER_PROJECT_XCODE_RESOURCES}
        ${JUCER_ICON_FILE}
        ${JUCER_RESOURCES_RC_FILE}
      )
      target_link_libraries(${standalone_target} PRIVATE ${shared_code_target})
      if(juce4_standalone)
        _FRUT_generate_plist_file(${standalone_target} "AUv3_Standalone" "APPL" "????")
      else()
        _FRUT_generate_plist_file(${standalone_target} "Standalone_Plugin" "APPL" "????")
      endif()
      _FRUT_set_output_directory_properties(${standalone_target} "Standalone Plugin")
      _FRUT_set_output_name_properties(${standalone_target})
      _FRUT_set_compiler_and_linker_settings(${standalone_target})
      _FRUT_add_extra_commands(${standalone_target} "${current_exporter}")
      _FRUT_set_JucePlugin_Build_defines(${standalone_target} "StandalonePlugIn")
      _FRUT_link_osx_frameworks(${standalone_target})
      _FRUT_add_xcode_resource_folders(${standalone_target})
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

    if(JUCER_BUILD_UNITY_PLUGIN)
      set(unity_target "${target}_Unity_Plugin")
      add_library(${unity_target} MODULE
        ${Unity_sources}
        ${JUCER_PROJECT_XCODE_RESOURCES}
        ${JUCER_ICON_FILE}
        ${JUCER_RESOURCES_RC_FILE}
      )
      target_link_libraries(${unity_target} PRIVATE ${shared_code_target})
      _FRUT_generate_plist_file(${unity_target} "Unity_Plugin" "BNDL" "????")
      _FRUT_set_bundle_properties(${unity_target} "bundle")
      _FRUT_set_output_directory_properties(${unity_target} "Unity Plugin")
      _FRUT_set_output_name_properties_Unity(${unity_target})
      _FRUT_set_compiler_and_linker_settings(${unity_target})
      _FRUT_add_extra_commands(${unity_target} "${current_exporter}")

      set(project_name "${JUCER_PROJECT_NAME}")
      if(NOT project_name MATCHES "^[Aa][Uu][Dd][Ii][Oo][Pp][Ll][Uu][Gg][Ii][Nn]")
        string(CONCAT project_name "audioplugin_" "${project_name}")
      endif()
      set(plugin_name "${project_name}")
      string(REPLACE " " "_" plugin_class_name "${plugin_name}")
      set(plugin_vendor "${JUCER_PLUGIN_MANUFACTURER}")
      set(plugin_description "${JUCER_PLUGIN_DESCRIPTION}")
      set(unity_script_file
        "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode/${project_name}_UnityScript.cs"
      )
      configure_file("${Reprojucer_templates_DIR}/UnityScript.cs"
        "${unity_script_file}" @ONLY
      )
      if(APPLE)
        target_sources(${unity_target} PRIVATE "${unity_script_file}")
        set_source_files_properties("${unity_script_file}" PROPERTIES
          MACOSX_PACKAGE_LOCATION "."
        )
      else()
        add_custom_command(TARGET ${unity_target} POST_BUILD
          COMMAND "${CMAKE_COMMAND}" "-E" "copy_if_different"
          "${unity_script_file}"
          "$<TARGET_FILE_DIR:${unity_target}>"
        )
      endif()

      if(APPLE)
        _FRUT_install_to_plugin_binary_location(${unity_target} "UNITY" "")
      elseif(MSVC)
        _FRUT_install_to_plugin_binary_location(${unity_target} "UNITY" "")

        unset(all_confs_destination)
        foreach(config IN LISTS JUCER_PROJECT_CONFIGURATIONS)
          if(DEFINED JUCER_UNITY_BINARY_LOCATION_${config}
              AND JUCER_ENABLE_PLUGIN_COPY_STEP_${config})
            set(destination "${JUCER_UNITY_BINARY_LOCATION_${config}}")
            string(APPEND all_confs_destination $<$<CONFIG:${config}>:${destination}>)
          endif()
        endforeach()
        if(DEFINED all_confs_destination)
          install(FILES "${unity_script_file}"
            COMPONENT "_install_${unity_target}_to_UNITY_binary_location"
            DESTINATION ${all_confs_destination}
          )
        endif()
      endif()
      _FRUT_set_JucePlugin_Build_defines(${unity_target} "UnityPlugIn")
      _FRUT_link_osx_frameworks(${unity_target})
      _FRUT_add_xcode_resource_folders(${unity_target})
      _FRUT_set_custom_xcode_flags(${unity_target})
      unset(unity_target)
    endif()

  else()
    message(FATAL_ERROR "Unknown project type: ${JUCER_PROJECT_TYPE}")

  endif()

endfunction()


function(_FRUT_parse_arguments single_value_keywords multi_value_keywords arguments)

  foreach(keyword IN LISTS single_value_keywords multi_value_keywords)
    unset(_${keyword})
  endforeach()

  unset(keyword)

  foreach(argument IN LISTS arguments)
    list(FIND single_value_keywords "${argument}" single_value_index)
    list(FIND multi_value_keywords "${argument}" multi_value_index)

    if(NOT DEFINED keyword)
      if(NOT single_value_index EQUAL -1)
        set(keyword "${argument}")
        set(keyword_type "single")
      elseif(NOT multi_value_index EQUAL -1)
        set(keyword "${argument}")
        set(keyword_type "multi")
      else()
        message(FATAL_ERROR "Unknown keyword: \"${argument}\"")
      endif()
    elseif(keyword_type STREQUAL "single")
      set(_${keyword} "${argument}")
      unset(keyword)
    elseif(keyword_type STREQUAL "multi")
      if(DEFINED _${keyword})
        if(NOT single_value_index EQUAL -1)
          set(keyword "${argument}")
          set(keyword_type "single")
        elseif(NOT multi_value_index EQUAL -1)
          set(keyword "${argument}")
          set(keyword_type "multi")
        else()
          list(APPEND _${keyword} ${argument})
        endif()
      else()
        list(APPEND _${keyword} ${argument})
      endif()
    else()
      message(FATAL_ERROR "There is a bug!")
    endif()
  endforeach()

  foreach(keyword IN LISTS single_value_keywords multi_value_keywords)
    unset(_${keyword} PARENT_SCOPE)
    if(DEFINED _${keyword})
      set(_${keyword} "${_${keyword}}" PARENT_SCOPE)
    endif()
  endforeach()

endfunction()


function(_FRUT_warn_about_unsupported_setting setting projucer_setting issue_number)

  message(WARNING "Reprojucer.cmake doesn't support the setting ${setting} "
    "(\"${projucer_setting}\" in Projucer). If you would like Reprojucer.cmake to "
    "support this setting, please leave a comment on the issue \"Reprojucer.cmake "
    "doesn't support the setting ${setting}\" on GitHub: "
    "https://github.com/McMartin/FRUT/issues/${issue_number}"
  )

endfunction()


function(_FRUT_abs_path_based_on_jucer_project_dir out_path in_path)

  if(NOT IS_ABSOLUTE "${in_path}" AND NOT DEFINED JUCER_PROJECT_DIR)
    message(FATAL_ERROR "Cannot join \"\${JUCER_PROJECT_DIR}\" and \"${in_path}\" to "
      "construct an absolute path because JUCER_PROJECT_DIR is not defined. You should "
      "call jucer_project_begin() first, e.g.:\n"
      "  jucer_project_begin(\n"
      "    PROJECT_FILE \"<path/to/YourProject.jucer>\"\n"
      "  )\n"
      "or if you don't have a .jucer file:\n"
      "  jucer_project_begin()\n"
    )
  endif()

  get_filename_component(in_path "${in_path}" ABSOLUTE BASE_DIR "${JUCER_PROJECT_DIR}")
  set(${out_path} "${in_path}" PARENT_SCOPE)

endfunction()


function(_FRUT_sanitize_path_in_user_folder out_path in_path)

  file(TO_CMAKE_PATH "$ENV{HOME}" user_folder)
  if(NOT user_folder MATCHES "/$")
    string(APPEND user_folder "/")
  endif()

  string(REGEX REPLACE "^~/" "${user_folder}" in_path "${in_path}")
  string(REGEX REPLACE "^\\$\\(HOME\\)/" "${user_folder}" in_path "${in_path}")

  set(${out_path} "${in_path}" PARENT_SCOPE)

endfunction()


function(_FRUT_check_SDK_folders exporter)

  if(JUCER_BUILD_VST OR JUCER_FLAG_JUCE_PLUGINHOST_VST)
    if(DEFINED JUCER_VST_LEGACY_SDK_FOLDER)
      if(NOT IS_DIRECTORY "${JUCER_VST_LEGACY_SDK_FOLDER}")
        message(WARNING "JUCER_VST_LEGACY_SDK_FOLDER: no such directory "
          "\"${JUCER_VST_LEGACY_SDK_FOLDER}\""
        )
      elseif(NOT EXISTS "${JUCER_VST_LEGACY_SDK_FOLDER}/pluginterfaces/vst2.x/aeffect.h")
        message(WARNING "JUCER_VST_LEGACY_SDK_FOLDER: \"${JUCER_VST_LEGACY_SDK_FOLDER}\" "
          "doesn't seem to contain the VST SDK"
        )
      endif()
    elseif(NOT DEFINED JUCER_VERSION OR JUCER_VERSION VERSION_GREATER 5.3.2)
      message(WARNING "JUCER_VST_LEGACY_SDK_FOLDER is not defined. You should give "
        "VST_LEGACY_SDK_FOLDER when calling jucer_export_target(\"${exporter}\")."
      )
    endif()

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
    elseif(DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 4.2.4)
      message(WARNING "JUCER_VST_SDK_FOLDER is not defined. You should give "
        "VST_SDK_FOLDER when calling jucer_export_target(\"${exporter}\")."
      )
    endif()
  endif()

  if(JUCER_BUILD_VST3 OR JUCER_FLAG_JUCE_PLUGINHOST_VST3)
    string(CONCAT juce_internal_vst3_sdk_path
      "${JUCER_PROJECT_MODULE_juce_audio_processors_PATH}/"
      "juce_audio_processors/format_types/VST3_SDK"
    )
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
    elseif((APPLE OR MSVC) AND NOT EXISTS "${juce_internal_vst3_sdk_path}")
      message(WARNING "JUCER_VST3_SDK_FOLDER is not defined. You should give "
        "VST3_SDK_FOLDER when calling jucer_export_target(\"${exporter}\")."
      )
    endif()
  endif()

  if(JUCER_BUILD_RTAS)
    if(DEFINED JUCER_RTAS_SDK_FOLDER)
      set(file_to_check_for
        "AlturaPorts/TDMPlugIns/PlugInLibrary/EffectClasses/CEffectProcessMIDI.cpp"
      )
      if(NOT IS_DIRECTORY "${JUCER_RTAS_SDK_FOLDER}")
        message(WARNING
          "JUCER_RTAS_SDK_FOLDER: no such directory \"${JUCER_RTAS_SDK_FOLDER}\""
        )
      elseif(NOT EXISTS "${JUCER_RTAS_SDK_FOLDER}/${file_to_check_for}")
        message(WARNING "JUCER_RTAS_SDK_FOLDER: \"${JUCER_RTAS_SDK_FOLDER}\" doesn't "
          "seem to contain the RTAS SDK"
        )
      endif()
    elseif(APPLE OR MSVC)
      message(WARNING "JUCER_RTAS_SDK_FOLDER is not defined. You should give "
        "RTAS_SDK_FOLDER when calling jucer_export_target(\"${exporter}\")."
      )
    endif()
  endif()

  if(JUCER_BUILD_AAX)
    if(DEFINED JUCER_AAX_SDK_FOLDER)
      if(NOT IS_DIRECTORY "${JUCER_AAX_SDK_FOLDER}")
        message(WARNING
          "JUCER_AAX_SDK_FOLDER: no such directory \"${JUCER_AAX_SDK_FOLDER}\""
        )
      elseif(NOT EXISTS "${JUCER_AAX_SDK_FOLDER}/Interfaces/AAX_Exports.cpp")
        message(WARNING "JUCER_AAX_SDK_FOLDER: \"${JUCER_AAX_SDK_FOLDER}\" doesn't "
          "seem to contain the AAX SDK"
        )
      endif()
    elseif(APPLE OR MSVC)
      message(WARNING "JUCER_AAX_SDK_FOLDER is not defined. You should give "
        "AAX_SDK_FOLDER when calling jucer_export_target(\"${exporter}\")."
      )
    endif()
  endif()

endfunction()


function(_FRUT_generate_AppConfig_header)

  if(DEFINED JUCER_APPCONFIG_USER_CODE_SECTION)
    set(user_code_section "\n${JUCER_APPCONFIG_USER_CODE_SECTION}\n")
    if(user_code_section STREQUAL "\n\n")
      set(user_code_section "\n")
    endif()
  else()
    string(CONCAT user_code_section "\n\n// (You can get your own code in this section "
      "by calling jucer_appconfig_header)\n\n"
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
  foreach(module_name IN LISTS JUCER_PROJECT_MODULES)
    string(LENGTH "${module_name}" module_name_length)
    if(module_name_length GREATER max_right_padding)
      set(max_right_padding ${module_name_length})
    endif()
  endforeach()
  math(EXPR max_right_padding "${max_right_padding} + 5")

  set(module_available_defines "")
  set(config_flags_defines "")
  foreach(module_name IN LISTS JUCER_PROJECT_MODULES)
    string(LENGTH "${module_name}" right_padding)
    set(padding_spaces "")
    while(right_padding LESS max_right_padding)
      string(APPEND padding_spaces " ")
      math(EXPR right_padding "${right_padding} + 1")
    endwhile()
    string(APPEND module_available_defines
      "#define JUCE_MODULE_AVAILABLE_${module_name}${padding_spaces} 1\n"
    )

    if(JUCER_${module_name}_CONFIG_FLAGS)
      string(APPEND config_flags_defines
        "\n"
        "//=============================================================================="
        "\n// ${module_name} flags:\n"
      )
    endif()
    foreach(config_flag IN LISTS JUCER_${module_name}_CONFIG_FLAGS)
      string(APPEND config_flags_defines "\n#ifndef    ${config_flag}\n")
      if(NOT DEFINED JUCER_FLAG_${config_flag})
        string(APPEND config_flags_defines " //#define ${config_flag}\n")
      elseif(JUCER_FLAG_${config_flag})
        string(APPEND config_flags_defines " #define   ${config_flag} 1\n")
      else()
        string(APPEND config_flags_defines " #define   ${config_flag} 0\n")
      endif()
      string(APPEND config_flags_defines "#endif\n")
    endforeach()
  endforeach()

  set(is_standalone_application 1)

  if(JUCER_PROJECT_TYPE STREQUAL "Dynamic Library")
    set(is_standalone_application 0)

  elseif(JUCER_PROJECT_TYPE STREQUAL "Audio Plug-in")
    set(is_standalone_application 0)

    # See ProjectSaver::writePluginCharacteristicsFile()
    # in JUCE/extras/Projucer/Source/ProjectSaving/jucer_ProjectSaver.cpp

    set(audio_plugin_setting_names
      "Build_VST" "Build_VST3" "Build_AU" "Build_AUv3" "Build_RTAS" "Build_AAX"
    )
    if(DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5.0.0)
      list(APPEND audio_plugin_setting_names "Build_STANDALONE")
    elseif(DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5.1.0)
      list(APPEND audio_plugin_setting_names "Build_Standalone")
      list(APPEND audio_plugin_setting_names "Build_STANDALONE")
    else()
      list(APPEND audio_plugin_setting_names "Build_Standalone")
    endif()
    if(NOT (DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5.3.2))
      list(APPEND audio_plugin_setting_names "Build_Unity")
    endif()
    if(NOT (DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5.0.0))
      list(APPEND audio_plugin_setting_names "Enable_IAA")
    endif()
    list(APPEND audio_plugin_setting_names
      "Name" "Desc" "Manufacturer" "ManufacturerWebsite" "ManufacturerEmail"
      "ManufacturerCode" "PluginCode"
      "IsSynth" "WantsMidiInput" "ProducesMidiOutput" "IsMidiEffect"
      "EditorRequiresKeyboardFocus"
      "Version" "VersionCode" "VersionString"
      "VSTUniqueID" "VSTCategory"
    )
    if(DEFINED JUCER_PLUGIN_VST3_CATEGORY
        OR NOT (DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5.3.1))
      list(APPEND audio_plugin_setting_names "Vst3Category")
    endif()
    list(APPEND audio_plugin_setting_names
      "AUMainType" "AUSubType" "AUExportPrefix" "AUExportPrefixQuoted"
      "AUManufacturerCode"
      "CFBundleIdentifier"
      "RTASCategory" "RTASManufacturerCode" "RTASProductId" "RTASDisableBypass"
      "RTASDisableMultiMono"
      "AAXIdentifier" "AAXManufacturerCode" "AAXProductId" "AAXCategory"
      "AAXDisableBypass" "AAXDisableMultiMono"
    )
    if(NOT (DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5.0.0))
      list(APPEND audio_plugin_setting_names
        "IAAType" "IAASubType" "IAAName"
      )
    endif()
    if(DEFINED JUCER_PLUGIN_VST_NUM_MIDI_INPUTS
        OR DEFINED JUCER_PLUGIN_VST_NUM_MIDI_OUTPUTS
        OR NOT DEFINED JUCER_VERSION
        OR JUCER_VERSION VERSION_GREATER 5.4.1)
      list(APPEND audio_plugin_setting_names "VSTNumMidiInputs" "VSTNumMidiOutputs")
    endif()

    _FRUT_bool_to_int("${JUCER_BUILD_VST}" Build_VST_value)
    _FRUT_bool_to_int("${JUCER_BUILD_VST3}" Build_VST3_value)
    _FRUT_bool_to_int("${JUCER_BUILD_AUDIOUNIT}" Build_AU_value)
    _FRUT_bool_to_int("${JUCER_BUILD_AUDIOUNIT_V3}" Build_AUv3_value)
    _FRUT_bool_to_int("${JUCER_BUILD_RTAS}" Build_RTAS_value)
    _FRUT_bool_to_int("${JUCER_BUILD_AAX}" Build_AAX_value)
    if(DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5.0.0)
      _FRUT_bool_to_int("${JUCER_BUILD_AUDIOUNIT_V3}" Build_STANDALONE_value)
    elseif(DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5.1.0)
      _FRUT_bool_to_int("${JUCER_BUILD_STANDALONE_PLUGIN}" Build_Standalone_value)
      set(Build_STANDALONE_value "JucePlugin_Build_Standalone")
    else()
      _FRUT_bool_to_int("${JUCER_BUILD_STANDALONE_PLUGIN}" Build_Standalone_value)
    endif()
    _FRUT_bool_to_int("${JUCER_BUILD_UNITY_PLUGIN}" Build_Unity_value)
    _FRUT_bool_to_int("${JUCER_ENABLE_INTER_APP_AUDIO}" Enable_IAA_value)

    set(Name_value "\"${JUCER_PLUGIN_NAME}\"")
    set(Desc_value "\"${JUCER_PLUGIN_DESCRIPTION}\"")
    set(Manufacturer_value "\"${JUCER_PLUGIN_MANUFACTURER}\"")
    set(ManufacturerWebsite_value "\"${JUCER_COMPANY_WEBSITE}\"")
    set(ManufacturerEmail_value "\"${JUCER_COMPANY_EMAIL}\"")

    _FRUT_char_literal("${JUCER_PLUGIN_MANUFACTURER_CODE}" ManufacturerCode_value)
    _FRUT_char_literal("${JUCER_PLUGIN_CODE}" PluginCode_value)

    _FRUT_bool_to_int("${JUCER_PLUGIN_IS_A_SYNTH}" IsSynth_value)
    _FRUT_bool_to_int("${JUCER_PLUGIN_MIDI_INPUT}" WantsMidiInput_value)
    _FRUT_bool_to_int("${JUCER_PLUGIN_MIDI_OUTPUT}" ProducesMidiOutput_value)
    _FRUT_bool_to_int("${JUCER_MIDI_EFFECT_PLUGIN}" IsMidiEffect_value)
    _FRUT_bool_to_int("${JUCER_KEY_FOCUS}" EditorRequiresKeyboardFocus_value)

    set(Version_value "${JUCER_PROJECT_VERSION}")
    _FRUT_version_to_hex("${JUCER_PROJECT_VERSION}" VersionCode_value)
    set(VersionString_value "\"${JUCER_PROJECT_VERSION}\"")

    set(VSTUniqueID_value "JucePlugin_PluginCode")
    if(DEFINED JUCER_PLUGIN_VST_LEGACY_CATEGORY)
      set(VSTCategory_value "${JUCER_PLUGIN_VST_LEGACY_CATEGORY}")
    elseif(DEFINED JUCER_PLUGIN_VST_CATEGORY)
      set(VSTCategory_value "${JUCER_PLUGIN_VST_CATEGORY}")
    elseif(DEFINED JUCER_VST_CATEGORY)
      set(VSTCategory_value "${JUCER_VST_CATEGORY}")
    else()
      if(JUCER_PLUGIN_IS_A_SYNTH)
        set(VSTCategory_value "kPlugCategSynth")
      else()
        set(VSTCategory_value "kPlugCategEffect")
      endif()
    endif()

    if(DEFINED JUCER_PLUGIN_VST3_CATEGORY)
      _FRUT_compute_vst3_category(vst3_category)
    else()
      if(JUCER_PLUGIN_IS_A_SYNTH)
        set(vst3_category "Instrument|Synth")
      else()
        set(vst3_category "Fx")
      endif()
    endif()
    set(Vst3Category_value "\"${vst3_category}\"")

    if(DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5.3.1)
      if(NOT DEFINED JUCER_PLUGIN_AU_MAIN_TYPE OR JUCER_PLUGIN_AU_MAIN_TYPE STREQUAL "")
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
    else()
      if(NOT DEFINED JUCER_PLUGIN_AU_MAIN_TYPE OR JUCER_PLUGIN_AU_MAIN_TYPE STREQUAL "")
        if(JUCER_MIDI_EFFECT_PLUGIN)
          set(AUMainType_value "'aumi'")
        elseif(JUCER_PLUGIN_IS_A_SYNTH)
          set(AUMainType_value "'aumu'")
        elseif(JUCER_PLUGIN_MIDI_INPUT)
          set(AUMainType_value "'aumf'")
        else()
          set(AUMainType_value "'aufx'")
        endif()
      else()
        _FRUT_get_au_quoted_four_chars("${JUCER_PLUGIN_AU_MAIN_TYPE}" AUMainType_value)
      endif()
    endif()

    set(AUSubType_value "JucePlugin_PluginCode")
    set(AUExportPrefix_value "${JUCER_PLUGIN_AU_EXPORT_PREFIX}")
    set(AUExportPrefixQuoted_value "\"${JUCER_PLUGIN_AU_EXPORT_PREFIX}\"")
    set(AUManufacturerCode_value "JucePlugin_ManufacturerCode")

    set(CFBundleIdentifier_value "${JUCER_BUNDLE_IDENTIFIER}")

    if(DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5.3.1)
      if(JUCER_PLUGIN_IS_A_SYNTH)
        set(RTASCategory_value "ePlugInCategory_SWGenerators")
      elseif(NOT DEFINED JUCER_PLUGIN_RTAS_CATEGORY)
        set(RTASCategory_value "ePlugInCategory_None")
      else()
        set(RTASCategory_value "${JUCER_PLUGIN_RTAS_CATEGORY}")
      endif()
    else()
      if(DEFINED JUCER_PLUGIN_RTAS_CATEGORY)
        _FRUT_compute_rtas_aax_category("RTAS" "ePlugInCategory" RTASCategory_value)
      else()
        if(JUCER_PLUGIN_IS_A_SYNTH)
          set(RTASCategory_value "2048") # ePlugInCategory_SWGenerators
        else()
          set(RTASCategory_value "0") # ePlugInCategory_None
        endif()
      endif()
    endif()
    set(RTASManufacturerCode_value "JucePlugin_ManufacturerCode")
    set(RTASProductId_value "JucePlugin_PluginCode")
    _FRUT_bool_to_int("${JUCER_PLUGIN_RTAS_DISABLE_BYPASS}" RTASDisableBypass_value)
    _FRUT_bool_to_int("${JUCER_PLUGIN_RTAS_DISABLE_MULTI_MONO}"
      RTASDisableMultiMono_value
    )

    if(NOT DEFINED JUCER_PLUGIN_AAX_IDENTIFIER)
      set(AAXIdentifier_value "${JUCER_BUNDLE_IDENTIFIER}")
    else()
      set(AAXIdentifier_value "${JUCER_PLUGIN_AAX_IDENTIFIER}")
    endif()
    set(AAXManufacturerCode_value "JucePlugin_ManufacturerCode")
    set(AAXProductId_value "JucePlugin_PluginCode")
    if(DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5.3.1)
      if(NOT DEFINED JUCER_PLUGIN_AAX_CATEGORY)
        set(AAXCategory_value "AAX_ePlugInCategory_Dynamics")
      else()
        set(AAXCategory_value "${JUCER_PLUGIN_AAX_CATEGORY}")
      endif()
    else()
      if(DEFINED JUCER_PLUGIN_AAX_CATEGORY)
        _FRUT_compute_rtas_aax_category("AAX" "AAX_ePlugInCategory" AAXCategory_value)
      else()
        if(JUCER_PLUGIN_IS_A_SYNTH)
          set(AAXCategory_value "2048") # AAX_ePlugInCategory_SWGenerators
        else()
          set(AAXCategory_value "0") # AAX_ePlugInCategory_None
        endif()
      endif()
    endif()
    _FRUT_bool_to_int("${JUCER_PLUGIN_AAX_DISABLE_BYPASS}" AAXDisableBypass_value)
    _FRUT_bool_to_int("${JUCER_PLUGIN_AAX_DISABLE_MULTI_MONO}" AAXDisableMultiMono_value)

    if(JUCER_PLUGIN_MIDI_INPUT)
      if(JUCER_PLUGIN_IS_A_SYNTH)
        set(iaa_type_code "auri")
      else()
        set(iaa_type_code "aurm")
      endif()
    else()
      if(JUCER_PLUGIN_IS_A_SYNTH)
        set(iaa_type_code "aurg")
      else()
        set(iaa_type_code "aurx")
      endif()
    endif()
    _FRUT_char_literal("${iaa_type_code}" IAAType_value)
    set(IAASubType_value "JucePlugin_PluginCode")
    set(IAAName_value "\"${JUCER_PLUGIN_MANUFACTURER}: ${JUCER_PLUGIN_NAME}\"")

    if(DEFINED JUCER_PLUGIN_VST_NUM_MIDI_INPUTS)
      set(VSTNumMidiInputs_value "${JUCER_PLUGIN_VST_NUM_MIDI_INPUTS}")
    else()
      set(VSTNumMidiInputs_value "16")
    endif()
    if(DEFINED JUCER_PLUGIN_VST_NUM_MIDI_OUTPUTS)
      set(VSTNumMidiOutputs_value "${JUCER_PLUGIN_VST_NUM_MIDI_OUTPUTS}")
    else()
      set(VSTNumMidiOutputs_value "16")
    endif()

    string(LENGTH "${JUCER_PLUGIN_CHANNEL_CONFIGURATIONS}" plugin_channel_config_length)
    if(plugin_channel_config_length GREATER 0)
      # See countMaxPluginChannels()
      # in JUCE/extras/Projucer/Source/ProjectSaving/jucer_ProjectSaver.cpp
      string(REGEX REPLACE "[, {}]" ";" configs "${JUCER_PLUGIN_CHANNEL_CONFIGURATIONS}")
      set(max_num_input 0)
      set(max_num_output 0)
      set(is_input TRUE)
      foreach(element IN LISTS configs)
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

    foreach(setting_name IN LISTS audio_plugin_setting_names)
      string(LENGTH "JucePlugin_${setting_name}" right_padding)
      set(padding_spaces "")
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

  if(DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5.0.0)
    string(TOUPPER "${JUCER_PROJECT_ID}" upper_project_id)
    set(template_file "${Reprojucer_templates_DIR}/AppConfig-4.h")
  else()
    set(template_file "${Reprojucer_templates_DIR}/AppConfig.h")
  endif()
  configure_file("${template_file}" "JuceLibraryCode/AppConfig.h" @ONLY)
  list(APPEND JUCER_PROJECT_FILES
    "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode/AppConfig.h"
  )

  set(JUCER_PROJECT_FILES "${JUCER_PROJECT_FILES}" PARENT_SCOPE)

endfunction()


function(_FRUT_generate_JuceHeader_header)

  list(LENGTH JUCER_PROJECT_RESOURCES resources_count)
  if(resources_count GREATER 0)
    set(BinaryDataBuilder_file_name "BinaryDataBuilder-0.3.0")
    if(NOT BinaryDataBuilder_exe MATCHES "${BinaryDataBuilder_file_name}")
      unset(BinaryDataBuilder_exe CACHE)
    endif()
    find_program(BinaryDataBuilder_exe "${BinaryDataBuilder_file_name}"
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
      find_program(BinaryDataBuilder_exe "${BinaryDataBuilder_file_name}"
        PATHS "${Reprojucer.cmake_DIR}/bin"
        NO_DEFAULT_PATH
      )
      if(NOT BinaryDataBuilder_exe)
        message(FATAL_ERROR "Could not find ${BinaryDataBuilder_file_name}")
      endif()
    endif()

    if(DEFINED JUCER_VERSION)
      set(projucer_version "${JUCER_VERSION}")
    else()
      set(projucer_version "latest")
    endif()
    if(DEFINED JUCER_PROJECT_ID)
      set(project_uid "${JUCER_PROJECT_ID}")
    else()
      set(project_uid "FRUT")
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
      "${projucer_version}"
      "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode/"
      "${project_uid}"
      ${size_limit_in_bytes}
      "${JUCER_BINARYDATA_NAMESPACE}"
    )
    foreach(resource_path IN LISTS JUCER_PROJECT_RESOURCES)
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

    foreach(filename IN LISTS binary_data_filenames)
      list(APPEND JUCER_PROJECT_FILES
        "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode/${filename}"
      )
    endforeach()
    if(DEFINED JUCER_INCLUDE_BINARYDATA AND NOT JUCER_INCLUDE_BINARYDATA)
      set(binary_data_include "")
    else()
      set(binary_data_include "#include \"BinaryData.h\"\n")
    endif()
  endif()

  set(modules_includes "")
  if(JUCER_PROJECT_MODULES)
    set(modules_includes "\n")
  endif()
  foreach(module_name IN LISTS JUCER_PROJECT_MODULES)
    string(APPEND modules_includes "#include <${module_name}/${module_name}.h>\n")
  endforeach()
  if(JUCER_PROJECT_MODULES)
    string(APPEND modules_includes "\n")
  endif()

  if(DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5.0.0)
    string(TOUPPER "${JUCER_PROJECT_ID}" upper_project_id)
    string(CONCAT include_guard_top
      "#ifndef __APPHEADERFILE_${upper_project_id}__\n"
      "#define __APPHEADERFILE_${upper_project_id}__"
    )
    set(include_guard_bottom "\n\n#endif   // __APPHEADERFILE_${upper_project_id}__")
  else()
    set(include_guard_top "#pragma once")
    set(include_guard_bottom "")
  endif()
  configure_file("${Reprojucer_templates_DIR}/JuceHeader.h"
    "JuceLibraryCode/JuceHeader.h" @ONLY
  )
  list(APPEND JUCER_PROJECT_FILES
    "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode/JuceHeader.h"
  )

  set(JUCER_PROJECT_FILES "${JUCER_PROJECT_FILES}" PARENT_SCOPE)

endfunction()


function(_FRUT_generate_icon_file icon_format icon_file_output_dir out_icon_filename)

  set(IconBuilder_file_name "IconBuilder-0.1.0")
  if(NOT IconBuilder_exe MATCHES "${IconBuilder_file_name}")
    unset(IconBuilder_exe CACHE)
  endif()
  find_program(IconBuilder_exe "${IconBuilder_file_name}"
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
    find_program(IconBuilder_exe "${IconBuilder_file_name}"
      PATHS "${Reprojucer.cmake_DIR}/bin"
      NO_DEFAULT_PATH
    )
    if(NOT IconBuilder_exe)
      message(FATAL_ERROR "Could not find ${IconBuilder_file_name}")
    endif()
  endif()

  set(IconBuilder_args "${icon_format}" "${icon_file_output_dir}")
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
    set(${out_icon_filename} "${icon_filename}" PARENT_SCOPE)
  endif()

endfunction()


function(_FRUT_generate_resources_rc_file output_path)

  set(rc_keys "CompanyName" "LegalCopyright" "FileDescription"
    "FileVersion" "ProductName" "ProductVersion"
  )
  set(rc_values "JUCER_COMPANY_NAME" "JUCER_COMPANY_COPYRIGHT" "JUCER_PROJECT_NAME"
    "JUCER_PROJECT_VERSION" "JUCER_PROJECT_NAME" "JUCER_PROJECT_VERSION"
  )
  set(rc_string_file_info_values "")
  foreach(index RANGE 5)
    list(GET rc_keys ${index} rc_key)
    list(GET rc_values ${index} rc_value)
    if(DEFINED ${rc_value} AND NOT ${rc_value} STREQUAL "")
      string(APPEND rc_string_file_info_values
        "      VALUE \"${rc_key}\",  \"${${rc_value}}\\0\"\n"
      )
    endif()
  endforeach()

  if(JUCER_ICON_FILE)
    get_filename_component(icon_filename "${JUCER_ICON_FILE}" NAME)
    string(CONCAT resources_rc_icon_settings
      "\n"
      "\nIDI_ICON1 ICON DISCARDABLE \"${icon_filename}\""
      "\nIDI_ICON2 ICON DISCARDABLE \"${icon_filename}\""
    )
  endif()

  string(REPLACE "." ";" version_parts "${JUCER_PROJECT_VERSION}")
  list(LENGTH version_parts version_parts_length)
  while(version_parts_length LESS 4)
    list(APPEND version_parts 0)
    list(LENGTH version_parts version_parts_length)
  endwhile()
  string(REPLACE ";" "," comma_separated_version_number "${version_parts}")

  configure_file("${Reprojucer_templates_DIR}/resources.rc" "${output_path}" @ONLY)

endfunction()


function(_FRUT_set_output_directory_properties target subfolder)

  foreach(config IN LISTS JUCER_PROJECT_CONFIGURATIONS)
    unset(output_directory)
    string(TOUPPER "${config}" upper_config)

    if(MSVC AND NOT (DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5.0.0))
      if(DEFINED JUCER_BINARY_LOCATION_${config})
        set(output_directory "${JUCER_BINARY_LOCATION_${config}}/${subfolder}")
      else()
        set(output_directory "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${subfolder}")
      endif()
    elseif(DEFINED JUCER_BINARY_LOCATION_${config})
      set(output_directory "${JUCER_BINARY_LOCATION_${config}}")
    endif()
    if(DEFINED output_directory)
      set_target_properties(${target} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY_${upper_config} "${output_directory}"
        LIBRARY_OUTPUT_DIRECTORY_${upper_config} "${output_directory}"
        RUNTIME_OUTPUT_DIRECTORY_${upper_config} "${output_directory}"
      )
    endif()
  endforeach()

endfunction()


function(_FRUT_set_output_name_properties target)

  foreach(config IN LISTS JUCER_PROJECT_CONFIGURATIONS)
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

endfunction()


function(_FRUT_set_output_name_properties_Unity unity_target)

  # Like _FRUT_set_output_name_properties(${unity_target}), but handles the
  # "audioplugin" prefix as well
  foreach(config IN LISTS JUCER_PROJECT_CONFIGURATIONS)
    string(TOUPPER "${config}" upper_config)

    if(JUCER_BINARY_NAME_${config})
      set(output_name "${JUCER_BINARY_NAME_${config}}")
    else()
      set(output_name "${JUCER_PROJECT_NAME}")
    endif()
    if(NOT output_name MATCHES "^[Aa][Uu][Dd][Ii][Oo][Pp][Ll][Uu][Gg][Ii][Nn]")
      string(CONCAT output_name "audioplugin_" "${output_name}")
    endif()
    set_target_properties(${unity_target} PROPERTIES
      OUTPUT_NAME_${upper_config} "${output_name}"
    )
  endforeach()

endfunction()


function(_FRUT_set_compiler_and_linker_settings target)

  target_include_directories(${target} PRIVATE
    "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode"
    ${JUCER_PROJECT_MODULES_FOLDERS}
    ${JUCER_PROJECT_MODULES_INTERNAL_SEARCH_PATHS}
  )
  foreach(config IN LISTS JUCER_PROJECT_CONFIGURATIONS)
    set(search_paths "${JUCER_HEADER_SEARCH_PATHS_${config}}")
    target_include_directories(${target} PRIVATE $<$<CONFIG:${config}>:${search_paths}>)
  endforeach()
  target_include_directories(${target} PRIVATE ${JUCER_HEADER_SEARCH_PATHS})

  if(JUCER_BUILD_VST OR JUCER_FLAG_JUCE_PLUGINHOST_VST)
    if(DEFINED JUCER_VST_LEGACY_SDK_FOLDER)
      target_include_directories(${target} PRIVATE "${JUCER_VST_LEGACY_SDK_FOLDER}")
    endif()
    if(DEFINED JUCER_VST_SDK_FOLDER)
      target_include_directories(${target} PRIVATE "${JUCER_VST_SDK_FOLDER}")
    endif()
  endif()

  if(JUCER_BUILD_VST3 OR JUCER_FLAG_JUCE_PLUGINHOST_VST3)
    string(CONCAT juce_internal_vst3_sdk_path
      "${JUCER_PROJECT_MODULE_juce_audio_processors_PATH}/"
      "juce_audio_processors/format_types/VST3_SDK"
    )
    if(DEFINED JUCER_VST3_SDK_FOLDER)
      target_include_directories(${target} PRIVATE "${JUCER_VST3_SDK_FOLDER}")
    elseif(EXISTS "${juce_internal_vst3_sdk_path}")
      target_include_directories(${target} PRIVATE "${juce_internal_vst3_sdk_path}")
    endif()
  endif()

  if(JUCER_BUILD_AAX AND DEFINED JUCER_AAX_SDK_FOLDER)
    target_include_directories(${target} PRIVATE
      "${JUCER_AAX_SDK_FOLDER}"
      "${JUCER_AAX_SDK_FOLDER}/Interfaces"
      "${JUCER_AAX_SDK_FOLDER}/Interfaces/ACF"
    )
  endif()

  foreach(config IN LISTS JUCER_PROJECT_CONFIGURATIONS)
    if(JUCER_OPTIMISATION_FLAG_${config})
      target_compile_options(${target} PRIVATE
        $<$<CONFIG:${config}>:${JUCER_OPTIMISATION_FLAG_${config}}>
      )
    endif()
  endforeach()

  _FRUT_set_cxx_language_standard_properties(${target})

  if(APPLE)
    _FRUT_set_compiler_and_linker_settings_APPLE(${target})
  elseif(MSVC)
    _FRUT_set_compiler_and_linker_settings_MSVC(${target})
  elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    _FRUT_set_compiler_and_linker_settings_Linux(${target})
  elseif(WIN32 AND NOT MSVC)
    _FRUT_set_compiler_and_linker_settings_MinGW(${target})
  endif()

  target_compile_definitions(${target} PRIVATE
    ${JUCER_PREPROCESSOR_DEFINITIONS}
    ${JUCER_EXTRA_PREPROCESSOR_DEFINITIONS}
  )
  foreach(config IN LISTS JUCER_PROJECT_CONFIGURATIONS)
    set(definitions "${JUCER_PREPROCESSOR_DEFINITIONS_${config}}")
    target_compile_definitions(${target} PRIVATE $<$<CONFIG:${config}>:${definitions}>)
  endforeach()

  target_compile_options(${target} PRIVATE ${JUCER_EXTRA_COMPILER_FLAGS})

  foreach(scheme IN LISTS JUCER_COMPILER_FLAG_SCHEMES)
    if(DEFINED JUCER_COMPILER_FLAGS_FOR_${scheme})
      set_source_files_properties(${JUCER_COMPILER_FLAG_SCHEME_${scheme}_FILES}
        PROPERTIES COMPILE_FLAGS "${JUCER_COMPILER_FLAGS_FOR_${scheme}}"
      )
    endif()
  endforeach()

  foreach(config IN LISTS JUCER_PROJECT_CONFIGURATIONS)
    foreach(path IN LISTS JUCER_EXTRA_LIBRARY_SEARCH_PATHS_${config})
      if(MSVC)
        target_link_libraries(${target} PRIVATE $<$<CONFIG:${config}>:-LIBPATH:${path}>)
      else()
        target_link_libraries(${target} PRIVATE $<$<CONFIG:${config}>:-L${path}>)
      endif()
    endforeach()
  endforeach()
  target_link_libraries(${target} PRIVATE ${JUCER_EXTRA_LINKER_FLAGS})
  target_link_libraries(${target} PRIVATE ${JUCER_EXTERNAL_LIBRARIES_TO_LINK})

endfunction()


function(_FRUT_set_compiler_and_linker_settings_APPLE target)

  foreach(config IN LISTS JUCER_PROJECT_CONFIGURATIONS)
    if(JUCER_CONFIGURATION_IS_DEBUG_${config})
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
      target_compile_options(${target} PRIVATE
        $<$<CONFIG:${config}>:-stdlib=${JUCER_CXX_LIBRARY_${config}}>
      )
    endif()

    if(JUCER_RELAX_IEEE_COMPLIANCE_${config})
      target_compile_options(${target} PRIVATE $<$<CONFIG:${config}>:-ffast-math>)
    endif()

    if(DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5.2.0)
      if(JUCER_LINK_TIME_OPTIMISATION_${config})
        target_compile_options(${target} PRIVATE $<$<CONFIG:${config}>:-flto>)
      endif()
    else()
      if(JUCER_CONFIGURATION_IS_DEBUG_${config})
        if(JUCER_LINK_TIME_OPTIMISATION_${config})
          target_compile_options(${target} PRIVATE $<$<CONFIG:${config}>:-flto>)
        endif()
      else()
        if(NOT (DEFINED JUCER_LINK_TIME_OPTIMISATION_${config}
                AND NOT JUCER_LINK_TIME_OPTIMISATION_${config}))
          target_compile_options(${target} PRIVATE $<$<CONFIG:${config}>:-flto>)
        endif()
      endif()
    endif()
  endforeach()

  if(target MATCHES "_AUv3_AppExtension$")
    if(CMAKE_GENERATOR STREQUAL "Xcode")
      set_target_properties(${target} PROPERTIES
        XCODE_ATTRIBUTE_ARCHS "$(ARCHS_STANDARD_64_BIT)"
      )
    else()
      set_target_properties(${target} PROPERTIES OSX_ARCHITECTURES "x86_64")
    endif()
  else()
    if(CMAKE_GENERATOR STREQUAL "Xcode")
      set(all_confs_archs "")
      foreach(config IN LISTS JUCER_PROJECT_CONFIGURATIONS)
        if(DEFINED JUCER_XCODE_ARCHS_${config})
          set(xcode_archs "${JUCER_XCODE_ARCHS_${config}}")
          string(APPEND all_confs_archs "$<$<CONFIG:${config}>:${xcode_archs}>")
        endif()
      endforeach()
      set_target_properties(${target} PROPERTIES
        XCODE_ATTRIBUTE_ARCHS "${all_confs_archs}"
      )
    else()
      foreach(config IN LISTS JUCER_PROJECT_CONFIGURATIONS)
        if(DEFINED JUCER_OSX_ARCHITECTURES_${config})
          string(TOUPPER "${config}" upper_config)
          set_target_properties(${target} PROPERTIES
            OSX_ARCHITECTURES_${upper_config} "${JUCER_OSX_ARCHITECTURES_${config}}"
          )
        endif()
      endforeach()
    endif()
  endif()

  if(CMAKE_GENERATOR STREQUAL "Xcode")
    set(all_confs_osx_deployment_target "")
    set(all_confs_sdkroot "")
    foreach(config IN LISTS JUCER_PROJECT_CONFIGURATIONS)
      set(osx_deployment_target "10.11")
      if(DEFINED JUCER_OSX_DEPLOYMENT_TARGET_${config})
        set(osx_deployment_target "${JUCER_OSX_DEPLOYMENT_TARGET_${config}}")
      endif()
      if(target MATCHES "_AUv3_AppExtension$"
          AND osx_deployment_target VERSION_LESS 10.11)
        set(osx_deployment_target "10.11")
        message(STATUS "Set OSX Deployment Target to 10.11 for ${target} in ${config}")
      endif()
      string(APPEND all_confs_osx_deployment_target
        "$<$<CONFIG:${config}>:${osx_deployment_target}>"
      )

      if(DEFINED JUCER_OSX_BASE_SDK_VERSION_${config})
        string(APPEND all_confs_sdkroot
          "$<$<CONFIG:${config}>:macosx${JUCER_OSX_BASE_SDK_VERSION_${config}}>"
        )
      endif()
    endforeach()
    set_target_properties(${target} PROPERTIES
      XCODE_ATTRIBUTE_MACOSX_DEPLOYMENT_TARGET "${all_confs_osx_deployment_target}"
      XCODE_ATTRIBUTE_SDKROOT "${all_confs_sdkroot}"
    )
  else()
    set(osx_deployment_target "10.11")
    if(DEFINED JUCER_OSX_DEPLOYMENT_TARGET_${CMAKE_BUILD_TYPE})
      set(osx_deployment_target "${JUCER_OSX_DEPLOYMENT_TARGET_${CMAKE_BUILD_TYPE}}")
    endif()
    if(target MATCHES "_AUv3_AppExtension$" AND osx_deployment_target VERSION_LESS 10.11)
      set(osx_deployment_target "10.11")
      message(STATUS "Set OSX Deployment Target to 10.11 for ${target}")
    endif()
    target_compile_options(${target} PRIVATE
      "-mmacosx-version-min=${osx_deployment_target}"
    )
    set_property(TARGET ${target} APPEND_STRING PROPERTY
      LINK_FLAGS " -mmacosx-version-min=${osx_deployment_target}"
    )

    set(sysroot "${JUCER_MACOSX_SDK_PATH_${CMAKE_BUILD_TYPE}}")
    if(IS_DIRECTORY "${sysroot}")
      target_compile_options(${target} PRIVATE -isysroot "${sysroot}")
      set_property(TARGET ${target} APPEND_STRING PROPERTY
        LINK_FLAGS " -isysroot ${sysroot}"
      )
    endif()
  endif()

  unset(all_confs_code_sign_identity)
  foreach(config IN LISTS JUCER_PROJECT_CONFIGURATIONS)
    if(NOT JUCER_CODE_SIGNING_IDENTITY_${config} STREQUAL "Mac Developer")
      string(APPEND all_confs_code_sign_identity
        $<$<CONFIG:${config}>:${JUCER_CODE_SIGNING_IDENTITY_${config}}>
      )
    endif()
  endforeach()
  if(DEFINED all_confs_code_sign_identity)
    set_target_properties(${target} PROPERTIES
      XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "${all_confs_code_sign_identity}"
    )
  endif()

  if(CMAKE_GENERATOR STREQUAL "Xcode" AND DEFINED JUCER_USE_HEADERMAP)
    if(JUCER_USE_HEADERMAP)
      set_target_properties(${target} PROPERTIES XCODE_ATTRIBUTE_USE_HEADERMAP "YES")
    else()
      set_target_properties(${target} PROPERTIES XCODE_ATTRIBUTE_USE_HEADERMAP "NO")
    endif()
  endif()

endfunction()


function(_FRUT_set_compiler_and_linker_settings_MSVC target)

  target_compile_definitions(${target} PRIVATE "_CRT_SECURE_NO_WARNINGS")

  if(MSVC_VERSION EQUAL 1800) # Visual Studio 2013
    set(ltcg_link_flag "/LTCG")
  else()
    set(ltcg_link_flag "/LTCG:incremental")
  endif()

  foreach(config IN LISTS JUCER_PROJECT_CONFIGURATIONS)
    string(TOUPPER "${config}" upper_config)

    if(NOT DEFINED JUCER_MULTI_PROCESSOR_COMPILATION_${config}
        OR JUCER_MULTI_PROCESSOR_COMPILATION_${config})
      target_compile_options(${target} PRIVATE $<$<CONFIG:${config}>:/MP>)
    endif()

    if(JUCER_CONFIGURATION_IS_DEBUG_${config})
      target_compile_definitions(${target} PRIVATE
        $<$<CONFIG:${config}>:DEBUG>
        $<$<CONFIG:${config}>:_DEBUG>
      )

      if(JUCER_LINK_TIME_OPTIMISATION_${config})
        target_compile_options(${target} PRIVATE $<$<CONFIG:${config}>:/GL>)
        set_property(TARGET ${target} APPEND_STRING PROPERTY
          LINK_FLAGS_${upper_config} " ${ltcg_link_flag}"
        )
        set_property(TARGET ${target} APPEND_STRING PROPERTY
          STATIC_LIBRARY_FLAGS_${upper_config} " /LTCG"
        )
      endif()
    else()
      target_compile_definitions(${target} PRIVATE $<$<CONFIG:${config}>:NDEBUG>)

      if(NOT JUCER_ALWAYS_DISABLE_WPO_${config}
          AND NOT (DEFINED JUCER_LINK_TIME_OPTIMISATION_${config}
                   AND NOT JUCER_LINK_TIME_OPTIMISATION_${config}))
        target_compile_options(${target} PRIVATE $<$<CONFIG:${config}>:/GL>)
        set_property(TARGET ${target} APPEND_STRING PROPERTY
          LINK_FLAGS_${upper_config} " ${ltcg_link_flag}"
        )
        set_property(TARGET ${target} APPEND_STRING PROPERTY
          STATIC_LIBRARY_FLAGS_${upper_config} " /LTCG"
        )
      endif()
    endif()

    if(NOT DEFINED JUCER_CHARACTER_SET_${config}
        OR JUCER_CHARACTER_SET_${config} STREQUAL "Default")
      target_compile_definitions(${target} PRIVATE $<$<CONFIG:${config}>:_SBCS>)
    elseif(JUCER_CHARACTER_SET_${config} STREQUAL "MultiByte")
      # Nothing to do, this is CMake's default
    elseif(JUCER_CHARACTER_SET_${config} STREQUAL "Unicode")
      target_compile_definitions(${target} PRIVATE
        $<$<CONFIG:${config}>:_UNICODE>
        $<$<CONFIG:${config}>:UNICODE>
      )
    endif()

    if(JUCER_CONFIGURATION_IS_DEBUG_${config})
      set(d_or_empty "d")
    else()
      set(d_or_empty "")
    endif()
    if(DEFINED JUCER_RUNTIME_LIBRARY_FLAG_${config})
      target_compile_options(${target} PRIVATE
        $<$<CONFIG:${config}>:${JUCER_RUNTIME_LIBRARY_FLAG_${config}}>
      )
    elseif(NOT (DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5.2.1))
      target_compile_options(${target} PRIVATE $<$<CONFIG:${config}>:/MD${d_or_empty}>)
    elseif(DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5.0.0
        AND (JUCER_BUILD_VST OR JUCER_BUILD_VST3 OR JUCER_BUILD_RTAS OR JUCER_BUILD_AAX))
      target_compile_options(${target} PRIVATE $<$<CONFIG:${config}>:/MD${d_or_empty}>)
    elseif(JUCER_BUILD_RTAS OR JUCER_BUILD_AAX)
      target_compile_options(${target} PRIVATE $<$<CONFIG:${config}>:/MD${d_or_empty}>)
    else()
      target_compile_options(${target} PRIVATE $<$<CONFIG:${config}>:/MT${d_or_empty}>)
    endif()

    if(DEFINED JUCER_DEBUG_INFORMATION_FORMAT_FLAG_${config})
      target_compile_options(${target} PRIVATE
        $<$<CONFIG:${config}>:${JUCER_DEBUG_INFORMATION_FORMAT_FLAG_${config}}>
      )
    endif()

    if(DEFINED JUCER_WARNING_LEVEL_FLAG_${config})
      target_compile_options(${target} PRIVATE
        $<$<CONFIG:${config}>:${JUCER_WARNING_LEVEL_FLAG_${config}}>
      )
    else()
      target_compile_options(${target} PRIVATE $<$<CONFIG:${config}>:/W4>)
    endif()

    if(JUCER_TREAT_WARNINGS_AS_ERRORS_${config})
      target_compile_options(${target} PRIVATE $<$<CONFIG:${config}>:/WX>)
    endif()

    if(JUCER_RELAX_IEEE_COMPLIANCE_${config})
      target_compile_options(${target} PRIVATE $<$<CONFIG:${config}>:/fp:fast>)
    endif()

    if(DEFINED JUCER_INCREMENTAL_LINKING_${config})
      if(JUCER_INCREMENTAL_LINKING_${config})
        set_property(TARGET ${target} APPEND_STRING PROPERTY
          LINK_FLAGS_${upper_config} " /INCREMENTAL"
        )
      endif()
    endif()

    if(DEFINED JUCER_FORCE_GENERATION_OF_DEBUG_SYMBOLS_${config})
      if(JUCER_FORCE_GENERATION_OF_DEBUG_SYMBOLS_${config})
        set_property(TARGET ${target} APPEND_STRING PROPERTY
          LINK_FLAGS_${upper_config} " /DEBUG"
        )
      endif()
    endif()

    if(DEFINED JUCER_GENERATE_MANIFEST_${config})
      if(NOT JUCER_GENERATE_MANIFEST_${config})
        set_property(TARGET ${target} APPEND_STRING PROPERTY
          LINK_FLAGS_${upper_config} " /MANIFEST:NO"
        )
      endif()
    endif()
  endforeach()

endfunction()


function(_FRUT_set_compiler_and_linker_settings_Linux target)

  target_compile_definitions(${target} PRIVATE "LINUX=1")

  foreach(config IN LISTS JUCER_PROJECT_CONFIGURATIONS)
    if(JUCER_CONFIGURATION_IS_DEBUG_${config})
      target_compile_definitions(${target} PRIVATE
        $<$<CONFIG:${config}>:DEBUG=1>
        $<$<CONFIG:${config}>:_DEBUG=1>
      )
    else()
      target_compile_definitions(${target} PRIVATE $<$<CONFIG:${config}>:NDEBUG=1>)
    endif()

    string(TOUPPER "${config}" upper_config)

    if(JUCER_LINK_TIME_OPTIMISATION_${config})
      target_compile_options(${target} PRIVATE $<$<CONFIG:${config}>:-flto>)
      set_property(TARGET ${target} APPEND_STRING PROPERTY
        LINK_FLAGS_${upper_config} " -flto"
      )
    endif()

    if(CMAKE_EXTRA_GENERATOR STREQUAL "CodeBlocks")
      if(DEFINED JUCER_ARCHITECTURE_FLAG_${config})
        target_compile_options(${target} PRIVATE
          $<$<CONFIG:${config}>:${JUCER_ARCHITECTURE_FLAG_${config}}>
        )
        set_property(TARGET ${target} APPEND_STRING PROPERTY
          LINK_FLAGS_${upper_config} " ${JUCER_ARCHITECTURE_FLAG_${config}}"
        )
      endif()
    else()
      if(DEFINED JUCER_ARCHITECTURE_FLAG_${config})
        target_compile_options(${target} PRIVATE
          $<$<CONFIG:${config}>:${JUCER_ARCHITECTURE_FLAG_${config}}>
        )
      else()
        target_compile_options(${target} PRIVATE $<$<CONFIG:${config}>:-march=native>)
      endif()
    endif()
  endforeach()

  set(linux_packages ${JUCER_PROJECT_LINUX_PACKAGES} ${JUCER_PKGCONFIG_LIBRARIES})
  if(NOT (DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5.0.0)
      AND "juce_gui_extra" IN_LIST JUCER_PROJECT_MODULES
      AND (NOT DEFINED JUCER_FLAG_JUCE_WEB_BROWSER OR JUCER_FLAG_JUCE_WEB_BROWSER))
    list(APPEND linux_packages "webkit2gtk-4.0" "gtk+-x11-3.0")
  endif()
  if((NOT DEFINED JUCER_VERSION OR JUCER_VERSION VERSION_GREATER 5.3.2)
      AND "juce_core" IN_LIST JUCER_PROJECT_MODULES
      AND (NOT DEFINED JUCER_FLAG_JUCE_USE_CURL OR JUCER_FLAG_JUCE_USE_CURL)
      AND NOT JUCER_FLAG_JUCE_LOAD_CURL_SYMBOLS_LAZILY)
    list(APPEND linux_packages "libcurl")
  endif()
  if(linux_packages)
    find_package(PkgConfig REQUIRED)
    list(SORT linux_packages)
    list(REMOVE_DUPLICATES linux_packages)
    unset(missing_packages)
    foreach(pkg IN LISTS linux_packages)
      pkg_check_modules(${pkg} "${pkg}")
      if(NOT ${pkg}_FOUND)
        string(APPEND missing_packages " ${pkg}")
      endif()
      target_compile_options(${target} PRIVATE ${${pkg}_CFLAGS})
      target_link_libraries(${target} PRIVATE ${${pkg}_LIBRARIES})
    endforeach()
    if(DEFINED missing_packages)
      message(FATAL_ERROR "pkg-config could not find the following packages:"
        "${missing_packages}"
      )
    endif()
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
    foreach(item IN LISTS linux_libs)
      if(item STREQUAL "pthread")
        target_compile_options(${target} PRIVATE "-pthread")
      endif()
      target_link_libraries(${target} PRIVATE "-l${item}")
    endforeach()
  endif()

endfunction()


function(_FRUT_set_compiler_and_linker_settings_MinGW target)

  target_compile_definitions(${target} PRIVATE "__MINGW__=1" "__MINGW_EXTENSION=")

  if(DEFINED JUCER_TARGET_PLATFORM AND NOT JUCER_TARGET_PLATFORM STREQUAL "Default")
    set(target_platform_values "Windows NT 4.0" "Windows 2000" "Windows XP"
      "Windows Server 2003" "Windows Vista" "Windows Server 2008" "Windows 7"
      "Windows 8" "Windows 8.1" "Windows 10"
    )
    set(winver_define_values "0x0400" "0x0500" "0x0501"
      "0x0502" "0x0600" "0x0600" "0x0601"
      "0x0602" "0x0603" "0x0A00"
    )
    list(FIND target_platform_values "${JUCER_TARGET_PLATFORM}" target_platform_index)
    if(target_platform_index EQUAL -1)
      message(FATAL_ERROR
        "Unsupported value for JUCER_TARGET_PLATFORM: \"${target_platform}\""
      )
    endif()
    list(GET winver_define_values ${target_platform_index} winver_define_value)
    target_compile_definitions(${target} PRIVATE "WINVER=${winver_define_value}")
  endif()

  foreach(config IN LISTS JUCER_PROJECT_CONFIGURATIONS)
    string(TOUPPER "${config}" upper_config)

    if(JUCER_CONFIGURATION_IS_DEBUG_${config})
      target_compile_definitions(${target} PRIVATE
        $<$<CONFIG:${config}>:DEBUG=1>
        $<$<CONFIG:${config}>:_DEBUG=1>
      )

      target_compile_options(${target} PRIVATE $<$<CONFIG:${config}>:-g>)
    else()
      target_compile_definitions(${target} PRIVATE $<$<CONFIG:${config}>:NDEBUG=1>)

      set_property(TARGET ${target} APPEND_STRING PROPERTY
        LINK_FLAGS_${upper_config} " -s"
      )
    endif()

    if(JUCER_LINK_TIME_OPTIMISATION_${config})
      target_compile_options(${target} PRIVATE $<$<CONFIG:${config}>:-flto>)
      set_property(TARGET ${target} APPEND_STRING PROPERTY
        LINK_FLAGS_${upper_config} " -flto"
      )
    endif()

    if(DEFINED JUCER_ARCHITECTURE_FLAG_${config})
      target_compile_options(${target} PRIVATE
        $<$<CONFIG:${config}>:${JUCER_ARCHITECTURE_FLAG_${config}}>
      )
      set_property(TARGET ${target} APPEND_STRING PROPERTY
        LINK_FLAGS_${upper_config} " ${JUCER_ARCHITECTURE_FLAG_${config}}"
      )
    endif()
  endforeach()

  target_compile_options(${target} PRIVATE "-mstackrealign")

  if(JUCER_PROJECT_MINGW_LIBS)
    target_link_libraries(${target} PRIVATE ${JUCER_PROJECT_MINGW_LIBS})
  endif()

endfunction()


function(_FRUT_set_cxx_language_standard_properties target)

  if(DEFINED JUCER_CXX_LANGUAGE_STANDARD)
    set(cxx_language_standard "${JUCER_CXX_LANGUAGE_STANDARD}")

    if(CMAKE_GENERATOR STREQUAL "Xcode")
      if(cxx_language_standard STREQUAL "latest")
        if(DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5.3.0)
          set(cxx_language_standard "1z")
        else()
          set(cxx_language_standard "17")
        endif()
      endif()
      if(JUCER_GNU_COMPILER_EXTENSIONS)
        set(cxx_language_standard "gnu++${cxx_language_standard}")
      else()
        set(cxx_language_standard "c++${cxx_language_standard}")
      endif()
      set_target_properties(${target} PROPERTIES
        XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "${cxx_language_standard}"
      )

    elseif(MSVC)
      if(MSVC_VERSION EQUAL 1900 OR MSVC_VERSION GREATER 1900) # VS2015 and later
        if(cxx_language_standard STREQUAL "17")
          set(cxx_language_standard "latest")
        endif()
        if(NOT cxx_language_standard STREQUAL "11")
          target_compile_options(${target} PRIVATE "-std:c++${cxx_language_standard}")
        endif()
      endif()

    else()
      if(JUCER_GNU_COMPILER_EXTENSIONS)
        set_target_properties(${target} PROPERTIES CXX_EXTENSIONS ON)
      else()
        set_target_properties(${target} PROPERTIES CXX_EXTENSIONS OFF)
      endif()
      if(cxx_language_standard STREQUAL "latest")
        set(cxx_language_standard "17")
      endif()
      set_target_properties(${target} PROPERTIES CXX_STANDARD ${cxx_language_standard})

    endif()
  else()
    if(APPLE)
      if(CMAKE_GENERATOR STREQUAL "Xcode")
        set(all_confs_cxx_language_standard "")
        foreach(config IN LISTS JUCER_PROJECT_CONFIGURATIONS)
          set(cxx_language_standard "c++0x")
          if(DEFINED JUCER_CXX_LANGUAGE_STANDARD_${config})
            string(TOLOWER cxx_language_standard
              "${JUCER_CXX_LANGUAGE_STANDARD_${config}}"
            )
          endif()
          string(APPEND all_confs_cxx_language_standard
            "$<$<CONFIG:${config}>:${cxx_language_standard}>"
          )
        endforeach()
        set_target_properties(${target} PROPERTIES
          XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "${all_confs_cxx_language_standard}"
        )
      else()
        set_target_properties(${target} PROPERTIES CXX_EXTENSIONS OFF)
        set_target_properties(${target} PROPERTIES CXX_STANDARD 11)

        set(cxx_language_standard "${JUCER_CXX_LANGUAGE_STANDARD_${CMAKE_BUILD_TYPE}}")
        if(cxx_language_standard)
          if(cxx_language_standard MATCHES "^GNU\\+\\+")
            set_target_properties(${target} PROPERTIES CXX_EXTENSIONS ON)
          endif()
          if(cxx_language_standard MATCHES "98$")
            set_target_properties(${target} PROPERTIES CXX_STANDARD 98)
          elseif(cxx_language_standard MATCHES "14$")
            set_target_properties(${target} PROPERTIES CXX_STANDARD 14)
          endif()
        endif()
      endif()

    elseif(MSVC)
      if(MSVC_VERSION EQUAL 1900 OR MSVC_VERSION GREATER 1900) # VS2015 and later
        if(JUCER_CXX_STANDARD_TO_USE STREQUAL "14")
          target_compile_options(${target} PRIVATE "-std:c++14")
        elseif(JUCER_CXX_STANDARD_TO_USE STREQUAL "latest")
          target_compile_options(${target} PRIVATE "-std:c++latest")
        endif()
      endif()

    elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
      set_target_properties(${target} PROPERTIES CXX_EXTENSIONS OFF)
      set_target_properties(${target} PROPERTIES CXX_STANDARD 11)

      if(DEFINED JUCER_CXX_STANDARD_TO_USE)
        if(JUCER_CXX_STANDARD_TO_USE MATCHES "03$")
          set_target_properties(${target} PROPERTIES CXX_STANDARD 98)
        elseif(JUCER_CXX_STANDARD_TO_USE MATCHES "14$")
          set_target_properties(${target} PROPERTIES CXX_STANDARD 14)
        endif()
      endif()

    elseif(WIN32 AND NOT MSVC)
      set_target_properties(${target} PROPERTIES CXX_EXTENSIONS OFF)
      set_target_properties(${target} PROPERTIES CXX_STANDARD 11)

    endif()
  endif()

endfunction()


function(_FRUT_add_extra_commands target exporter)

  if(APPLE)
    _FRUT_add_extra_commands_APPLE(${target} "${exporter}")
  elseif(MSVC)
    _FRUT_add_extra_commands_MSVC(${target} "${exporter}")
  endif()

endfunction()


function(_FRUT_add_extra_commands_APPLE target exporter)

  get_target_property(target_type ${target} TYPE)
  if(target_type STREQUAL "EXECUTABLE" OR target_type STREQUAL "MODULE_LIBRARY")
    unset(all_confs_strip_exe)
    unset(all_confs_strip_opt)
    unset(all_confs_strip_arg)
    foreach(config IN LISTS JUCER_PROJECT_CONFIGURATIONS)
      if(JUCER_STRIP_LOCAL_SYMBOLS_${config})
        find_program(strip_exe "strip")
        if(NOT strip_exe)
          message(FATAL_ERROR "Could not find strip program")
        endif()
        string(APPEND all_confs_strip_exe $<$<CONFIG:${config}>:${strip_exe}>)
        string(APPEND all_confs_strip_opt $<$<CONFIG:${config}>:-x>)
        string(APPEND all_confs_strip_arg $<$<CONFIG:${config}>:$<TARGET_FILE:${target}>>)
      endif()
    endforeach()
    if(DEFINED all_confs_strip_exe)
      add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${all_confs_strip_exe} ${all_confs_strip_opt} ${all_confs_strip_arg}
      )
    endif()
  endif()

  if(DEFINED JUCER_PREBUILD_SHELL_SCRIPT)
    if(NOT DEFINED JUCER_TARGET_PROJECT_FOLDER)
      message(FATAL_ERROR "JUCER_TARGET_PROJECT_FOLDER must be defined. Give "
        "TARGET_PROJECT_FOLDER when calling jucer_export_target(\"${exporter}\")."
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
        "TARGET_PROJECT_FOLDER when calling jucer_export_target(\"${exporter}\")."
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

endfunction()


function(_FRUT_add_extra_commands_MSVC target exporter)

  unset(all_confs_prebuild_command)
  foreach(config IN LISTS JUCER_PROJECT_CONFIGURATIONS)
    if(DEFINED JUCER_PREBUILD_COMMAND_${config})
      set(prebuild_command "${JUCER_PREBUILD_COMMAND_${config}}")
      string(APPEND all_confs_prebuild_command
        $<$<CONFIG:${config}>:${prebuild_command}>
      )
    endif()
  endforeach()
  if(DEFINED all_confs_prebuild_command)
    if(NOT DEFINED JUCER_TARGET_PROJECT_FOLDER)
      message(FATAL_ERROR "JUCER_TARGET_PROJECT_FOLDER must be defined. Give "
        "TARGET_PROJECT_FOLDER when calling jucer_export_target(\"${exporter}\")."
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
  foreach(config IN LISTS JUCER_PROJECT_CONFIGURATIONS)
    if(DEFINED JUCER_POSTBUILD_COMMAND_${config})
      set(postbuild_command "${JUCER_POSTBUILD_COMMAND_${config}}")
      string(APPEND all_confs_postbuild_command
        $<$<CONFIG:${config}>:${postbuild_command}>
      )
    endif()
  endforeach()
  if(DEFINED all_confs_postbuild_command)
    if(NOT DEFINED JUCER_TARGET_PROJECT_FOLDER)
      message(FATAL_ERROR "JUCER_TARGET_PROJECT_FOLDER must be defined. Give "
        "TARGET_PROJECT_FOLDER when calling jucer_export_target(\"${exporter}\")."
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

endfunction()


function(_FRUT_generate_plist_file
  target plist_suffix bundle_package_type bundle_signature
)

  set(plist_filename "Info-${plist_suffix}.plist")
  if(CMAKE_GENERATOR STREQUAL "Xcode")
    set(bundle_executable "\${EXECUTABLE_NAME}")
    set_target_properties(${target} PROPERTIES
      XCODE_ATTRIBUTE_INFOPLIST_FILE "${CMAKE_CURRENT_BINARY_DIR}/${plist_filename}"
      XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "${JUCER_BUNDLE_IDENTIFIER}"
    )
  else()
    set(bundle_executable "\${MACOSX_BUNDLE_BUNDLE_NAME}")
    set_target_properties(${target} PROPERTIES
      MACOSX_BUNDLE_BUNDLE_NAME "${JUCER_PROJECT_NAME}"
      MACOSX_BUNDLE_GUI_IDENTIFIER "${JUCER_BUNDLE_IDENTIFIER}"
      MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_BINARY_DIR}/${plist_filename}"
    )
  endif()

  if(DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5.2.0)
    if(CMAKE_GENERATOR STREQUAL "Xcode")
      set(bundle_identifier "\$(PRODUCT_BUNDLE_IDENTIFIER)")
    else()
      set(bundle_identifier "\${MACOSX_BUNDLE_GUI_IDENTIFIER}")
    endif()
  else()
    set(bundle_identifier "@JUCER_BUNDLE_IDENTIFIER@")
  endif()

  set(plist_entries "")

  if(JUCER_MICROPHONE_ACCESS)
    if(DEFINED JUCER_MICROPHONE_ACCESS_TEXT)
      set(microphone_usage_description "${JUCER_MICROPHONE_ACCESS_TEXT}")
    else()
      string(CONCAT microphone_usage_description "This app requires audio input. If you "
        "do not have an audio interface connected it will use the built-in microphone."
      )
    endif()
    string(APPEND plist_entries "
    <key>NSMicrophoneUsageDescription</key>
    <string>${microphone_usage_description}</string>"
    )
  endif()

  if(JUCER_CAMERA_ACCESS)
    if(DEFINED JUCER_CAMERA_ACCESS_TEXT)
      set(camera_usage_description "${JUCER_CAMERA_ACCESS_TEXT}")
    else()
      string(CONCAT camera_usage_description "This app requires access to the camera to "
        "function correctly."
      )
    endif()
    string(APPEND plist_entries "
    <key>NSCameraUsageDescription</key>
    <string>${camera_usage_description}</string>"
    )
  endif()

  get_filename_component(bundle_icon_file "${JUCER_ICON_FILE}" NAME)

  if(DEFINED JUCER_COMPANY_COPYRIGHT
      OR NOT (DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5.2.0))
    set(ns_human_readable_copyright "@JUCER_COMPANY_COPYRIGHT@")
  else()
    set(ns_human_readable_copyright "@JUCER_COMPANY_NAME@")
  endif()

  string(APPEND plist_entries "
    <key>CFBundleExecutable</key>
    <string>${bundle_executable}</string>
    <key>CFBundleIconFile</key>
    <string>${bundle_icon_file}</string>
    <key>CFBundleIdentifier</key>
    <string>${bundle_identifier}</string>
    <key>CFBundleName</key>
    <string>@JUCER_PROJECT_NAME@</string>
    <key>CFBundleDisplayName</key>
    <string>@JUCER_PROJECT_NAME@</string>
    <key>CFBundlePackageType</key>
    <string>${bundle_package_type}</string>
    <key>CFBundleSignature</key>
    <string>${bundle_signature}</string>
    <key>CFBundleShortVersionString</key>
    <string>@JUCER_PROJECT_VERSION@</string>
    <key>CFBundleVersion</key>
    <string>@JUCER_PROJECT_VERSION@</string>
    <key>NSHumanReadableCopyright</key>
    <string>${ns_human_readable_copyright}</string>
    <key>NSHighResolutionCapable</key>
    <true/>"
  )

  list(LENGTH JUCER_DOCUMENT_FILE_EXTENSIONS file_extensions_length)
  if(file_extensions_length GREATER 0 AND NOT target MATCHES "_AUv3_AppExtension$")
    set(bundle_type_extensions "")
    foreach(type_extension IN LISTS JUCER_DOCUMENT_FILE_EXTENSIONS)
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

    string(APPEND plist_entries "
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

  if(target MATCHES "_AU$")
    _FRUT_get_au_main_type_code(au_main_type_code)
    _FRUT_version_to_dec("${JUCER_PROJECT_VERSION}" dec_version)

    string(APPEND plist_entries "
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
        <integer>${dec_version}</integer>"
    )

    if(JUCER_PLUGIN_AU_IS_SANDBOX_SAFE)
      string(APPEND plist_entries "
        <key>sandboxSafe</key>
        <true/>"
      )
    elseif(NOT (DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5.4.0))
      string(APPEND plist_entries "
        <key>resourceUsage</key>
        <dict>
          <key>network.client</key>
          <true/>
          <key>temporary-exception.files.all.read-write</key>
          <true/>
        </dict>"
      )
    endif()

    string(APPEND plist_entries "
      </dict>
    </array>"
    )
  endif()

  if(target MATCHES "_AUv3_AppExtension$")
    _FRUT_get_au_main_type_code(au_main_type_code)
    _FRUT_version_to_dec("${JUCER_PROJECT_VERSION}" dec_version)

    if(JUCER_PLUGIN_IS_A_SYNTH)
      set(tag "Synth")
    else()
      set(tag "Effects")
    endif()

    string(APPEND plist_entries "
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
  endif()

  if(JUCER_CUSTOM_PLIST)
    set(PListMerger_file_name "PListMerger-0.1.0")
    if(NOT PListMerger_exe MATCHES "${PListMerger_file_name}")
      unset(PListMerger_exe CACHE)
    endif()
    find_program(PListMerger_exe "${PListMerger_file_name}"
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
      find_program(PListMerger_exe "${PListMerger_file_name}"
        PATHS "${Reprojucer.cmake_DIR}/bin"
        NO_DEFAULT_PATH
      )
      if(NOT PListMerger_exe)
        message(FATAL_ERROR "Could not find ${PListMerger_file_name}")
      endif()
    endif()

    execute_process(
      COMMAND
      "${PListMerger_exe}"
      "${JUCER_CUSTOM_PLIST}"
      "<plist><dict>${plist_entries}</dict></plist>"
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
    set(plist_entries "${PListMerger_output}")
  endif()

  string(CONFIGURE "${plist_entries}" plist_entries @ONLY)
  configure_file("${Reprojucer_templates_DIR}/Info.plist" "${plist_filename}" @ONLY)

endfunction()


function(_FRUT_generate_plist_file_AUv3 target plist_suffix)

  # com.yourcompany.NewProject -> com.yourcompany.NewProject.NewProjectAUv3
  string(REPLACE "." ";" bundle_id_parts "${JUCER_BUNDLE_IDENTIFIER}")
  list(LENGTH bundle_id_parts bundle_id_parts_length)
  math(EXPR bundle_id_parts_last_index "${bundle_id_parts_length} - 1")
  list(GET bundle_id_parts ${bundle_id_parts_last_index} bundle_id_last_part)
  list(APPEND bundle_id_parts "${bundle_id_last_part}AUv3")
  string(REPLACE ";" "." JUCER_BUNDLE_IDENTIFIER "${bundle_id_parts}")

  _FRUT_generate_plist_file(${target} "${plist_suffix}" "XPC!" "????")

endfunction()


function(_FRUT_set_bundle_properties target extension)

  if(NOT APPLE)
    return()
  endif()

  set_target_properties(${target} PROPERTIES
    BUNDLE TRUE
    BUNDLE_EXTENSION "${extension}"
    XCODE_ATTRIBUTE_WRAPPER_EXTENSION "${extension}"
  )

  target_sources(${target} PRIVATE "${Reprojucer_templates_DIR}/PkgInfo")
  set_source_files_properties("${Reprojucer_templates_DIR}/PkgInfo"
    PROPERTIES MACOSX_PACKAGE_LOCATION "."
  )

endfunction()


function(_FRUT_install_to_plugin_binary_location target plugin_type default_destination)

  unset(all_confs_destination)
  foreach(config IN LISTS JUCER_PROJECT_CONFIGURATIONS)
    if(DEFINED JUCER_${plugin_type}_BINARY_LOCATION_${config})
      set(destination "${JUCER_${plugin_type}_BINARY_LOCATION_${config}}")
    else()
      set(destination "${default_destination}")
    endif()
    if(NOT destination STREQUAL "")
      if(DEFINED JUCER_ENABLE_PLUGIN_COPY_STEP_${config})
        if(JUCER_ENABLE_PLUGIN_COPY_STEP_${config})
          string(APPEND all_confs_destination $<$<CONFIG:${config}>:${destination}>)
        endif()
      elseif(APPLE)
        string(APPEND all_confs_destination $<$<CONFIG:${config}>:${destination}>)
      endif()
    endif()
  endforeach()

  if(NOT DEFINED all_confs_destination)
    return()
  endif()

  set(component "_install_${target}_to_${plugin_type}_binary_location")

  install(TARGETS ${target} COMPONENT ${component} DESTINATION ${all_confs_destination})

  add_custom_command(TARGET ${target} POST_BUILD
    COMMAND
    "${CMAKE_COMMAND}"
    "-DCMAKE_INSTALL_CONFIG_NAME=$<CONFIG>"
    "-DCMAKE_INSTALL_COMPONENT=${component}"
    "-P" "${CMAKE_CURRENT_BINARY_DIR}/cmake_install.cmake"
  )

endfunction()


function(_FRUT_set_JucePlugin_Build_defines target target_type)

  if(JUCER_BUILD_AUDIOUNIT_V3
      AND DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5.0.0)
    set(JUCER_BUILD_STANDALONE_PLUGIN ON)
  endif()

  set(plugin_types     VST VST3 AudioUnit AudioUnitv3  RTAS AAX Standalone       )
  set(setting_suffixes VST VST3 AUDIOUNIT AUDIOUNIT_V3 RTAS AAX STANDALONE_PLUGIN)
  set(define_suffixes  VST VST3 AU        AUv3         RTAS AAX Standalone       )
  set(range_max 6)

  if(NOT (DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5.3.2))
    list(APPEND plugin_types Unity)
    list(APPEND setting_suffixes UNITY_PLUGIN)
    list(APPEND define_suffixes Unity)
    set(range_max 7)
  endif()

  foreach(index RANGE ${range_max})
    list(GET setting_suffixes ${index} setting_suffix)
    list(GET plugin_types ${index} plugin_type)
    list(GET define_suffixes ${index} define_suffix)

    if(target_type STREQUAL "${plugin_type}PlugIn"
        OR (target_type STREQUAL "SharedCodeTarget" AND JUCER_BUILD_${setting_suffix}))
      target_compile_definitions(${target} PRIVATE "JucePlugin_Build_${define_suffix}=1")
    else()
      target_compile_definitions(${target} PRIVATE "JucePlugin_Build_${define_suffix}=0")
    endif()
  endforeach()

endfunction()


function(_FRUT_link_osx_frameworks target)

  if(NOT APPLE)
    return()
  endif()

  set(osx_frameworks
    ${JUCER_PROJECT_OSX_FRAMEWORKS}
    ${JUCER_EXTRA_SYSTEM_FRAMEWORKS}
    ${JUCER_EXTRA_FRAMEWORKS}
    ${ARGN}
  )
  if(JUCER_FLAG_JUCE_PLUGINHOST_AU)
    list(APPEND osx_frameworks "AudioUnit" "CoreAudioKit")
  endif()

  if(osx_frameworks)
    list(SORT osx_frameworks)
    list(REMOVE_DUPLICATES osx_frameworks)
    if(NOT JUCER_FLAG_JUCE_QUICKTIME)
      list(REMOVE_ITEM osx_frameworks "QuickTime")
    endif()

    foreach(config IN LISTS JUCER_PROJECT_CONFIGURATIONS)
      set(CMAKE_FRAMEWORK_PATH "")
      set(sdk_version "${JUCER_OSX_BASE_SDK_VERSION_${config}}")
      set(sdk_path "${JUCER_MACOSX_SDK_PATH_${config}}")
      if(IS_DIRECTORY "${sdk_path}")
        set(CMAKE_FRAMEWORK_PATH "${sdk_path}/System/Library/Frameworks")
      endif()

      foreach(framework_name IN LISTS osx_frameworks)
        find_library(${framework_name}_framework_${sdk_version} ${framework_name})
        target_link_libraries(${target} PRIVATE
          "$<$<CONFIG:${config}>:${${framework_name}_framework_${sdk_version}}>"
        )
      endforeach()
    endforeach()
  endif()

  if(JUCER_EXTRA_CUSTOM_FRAMEWORKS)
    set(CMAKE_FIND_FRAMEWORK ONLY)
    set(CMAKE_FRAMEWORK_PATH "${JUCER_FRAMEWORK_SEARCH_PATHS}")
    foreach(framework_name IN LISTS JUCER_EXTRA_CUSTOM_FRAMEWORKS)
      string(REGEX REPLACE "\\.framework$" "" framework_name "${framework_name}")
      find_library(${framework_name}_framework ${framework_name})
      target_link_libraries(${target} PRIVATE ${${framework_name}_framework})
    endforeach()
  endif()

endfunction()


function(_FRUT_add_xcode_resource_folders target)

  if(NOT APPLE)
    return()
  endif()

  foreach(folder IN LISTS JUCER_CUSTOM_XCODE_RESOURCE_FOLDERS)
    add_custom_command(TARGET ${target} PRE_BUILD
      COMMAND rsync -r "${folder}" "$<TARGET_FILE_DIR:${target}>/../Resources"
    )
  endforeach()

endfunction()


function(_FRUT_set_custom_xcode_flags target)

  unset(all_flags)
  foreach(config IN LISTS JUCER_PROJECT_CONFIGURATIONS)
    if(DEFINED JUCER_CUSTOM_XCODE_FLAGS_${config})
      foreach(xcode_flag IN LISTS JUCER_CUSTOM_XCODE_FLAGS_${config})
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

  if(DEFINED all_flags)
    list(SORT all_flags)
    list(REMOVE_DUPLICATES all_flags)
    foreach(flag IN LISTS all_flags)
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
      set(hex_char "${hex_unit}")
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


function(_FRUT_char_literal value out_char_literal)

  set(all_ascii_codes "")
  foreach(ascii_code RANGE 1 127)
    list(APPEND all_ascii_codes ${ascii_code})
  endforeach()
  string(ASCII ${all_ascii_codes} all_ascii_chars)

  set(four_chars "${value}")
  if(DEFINED JUCER_VERSION AND NOT JUCER_VERSION VERSION_GREATER 5.2.0)
    string(STRIP "${four_chars}" four_chars)
  endif()
  string(SUBSTRING "${four_chars}" 0 4 four_chars)
  string(LENGTH "${four_chars}" four_chars_length)
  set(dec_value 0)
  foreach(index IN ITEMS 0 1 2 3)
    if(index LESS four_chars_length)
      string(SUBSTRING "${four_chars}" ${index} 1 ascii_char)
      string(FIND "${all_ascii_chars}" "${ascii_char}" ascii_code)
      if(ascii_code EQUAL -1)
        message(FATAL_ERROR "${value} cannot contain non-ASCII characters")
      endif()
      math(EXPR ascii_code "${ascii_code} + 1")
    else()
      set(ascii_code 0)
    endif()
    math(EXPR dec_value "(${dec_value} << 8) | (${ascii_code} & 255)")
  endforeach()

  _FRUT_dec_to_hex("${dec_value}" hex_value)

  set(${out_char_literal} "${hex_value} // '${four_chars}'" PARENT_SCOPE)

endfunction()


function(_FRUT_add_Rez_command_to_AU_plugin au_target)

  find_program(Rez_exe "Rez")
  if(NOT Rez_exe)
    message(WARNING "Could not find Rez tool. Discovery of AU plugins might not work.")
    return()
  endif()

  set(rez_output "${CMAKE_CURRENT_BINARY_DIR}/${JUCER_PROJECT_NAME}.rsrc")

  set(rez_defines "")
  set(rez_archs "")
  set(all_confs_sysroot "")
  foreach(config IN LISTS JUCER_PROJECT_CONFIGURATIONS)
    foreach(osx_architecture IN LISTS JUCER_OSX_ARCHITECTURES_${config})
      list(APPEND rez_defines
        "$<$<CONFIG:${config}>:-d>"
        "$<$<CONFIG:${config}>:${osx_architecture}_YES>"
      )
      list(APPEND rez_archs
        "$<$<CONFIG:${config}>:-arch>"
        "$<$<CONFIG:${config}>:${osx_architecture}>"
      )
    endforeach()

    set(sysroot "${JUCER_MACOSX_SDK_PATH_${config}}")
    if(IS_DIRECTORY "${sysroot}")
      list(APPEND all_confs_sysroot
        "$<$<CONFIG:${config}>:-isysroot>" "$<$<CONFIG:${config}>:${sysroot}>"
      )
    endif()
  endforeach()

  string(CONCAT carbon_include_dir
    "/System/Library/Frameworks/CoreServices.framework/Frameworks/"
    "CarbonCore.framework/Versions/A/Headers"
  )
  string(CONCAT juce_audio_plugin_client_include_dir
    "${JUCER_PROJECT_MODULE_juce_audio_plugin_client_PATH}/"
    "juce_audio_plugin_client"
  )

  add_custom_command(OUTPUT ${rez_output}
    COMMAND
    "${Rez_exe}"
    "-o" "${rez_output}"
    "-d" "SystemSevenOrLater=1"
    "-useDF"
    ${rez_defines}
    ${rez_archs}
    "-i" "${carbon_include_dir}"
    "-i" "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode"
    "-i" "${juce_audio_plugin_client_include_dir}"
    ${all_confs_sysroot}
    ${ARGN}
  )
  set_source_files_properties("${rez_output}" PROPERTIES
    GENERATED TRUE
    MACOSX_PACKAGE_LOCATION "Resources"
  )
  target_sources(${au_target} PRIVATE "${rez_output}")

endfunction()


function(_FRUT_get_au_quoted_four_chars au_enum_case out_var)

  if(au_enum_case STREQUAL "kAudioUnitType_Effect")
    set(quoted_four_chars "'aufx'")
  elseif(au_enum_case STREQUAL "kAudioUnitType_FormatConverter")
    set(quoted_four_chars "'aufc'")
  elseif(au_enum_case STREQUAL "kAudioUnitType_Generator")
    set(quoted_four_chars "'augn'")
  elseif(au_enum_case STREQUAL "kAudioUnitType_MIDIProcessor")
    set(quoted_four_chars "'aumi'")
  elseif(au_enum_case STREQUAL "kAudioUnitType_Mixer")
    set(quoted_four_chars "'aumx'")
  elseif(au_enum_case STREQUAL "kAudioUnitType_MusicDevice")
    set(quoted_four_chars "'aumu'")
  elseif(au_enum_case STREQUAL "kAudioUnitType_MusicEffect")
    set(quoted_four_chars "'aumf'")
  elseif(au_enum_case STREQUAL "kAudioUnitType_OfflineEffect")
    set(quoted_four_chars "'auol'")
  elseif(au_enum_case STREQUAL "kAudioUnitType_Output")
    set(quoted_four_chars "'auou'")
  elseif(au_enum_case STREQUAL "kAudioUnitType_Panner")
    set(quoted_four_chars "'aupn'")
  else()
    message(WARNING "Unknown Audio Unit type \"${au_enum_case}\", not converting it to "
      "four chars literal."
    )
    set(quoted_four_chars "${au_enum_case}")
  endif()

  set(${out_var} "${quoted_four_chars}" PARENT_SCOPE)

endfunction()


function(_FRUT_get_au_main_type_code out_value)

  if(NOT DEFINED JUCER_PLUGIN_AU_MAIN_TYPE OR JUCER_PLUGIN_AU_MAIN_TYPE STREQUAL "")
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
    if(DEFINED JUCER_VERSION AND JUCER_VERSION VERSION_LESS 5.3.1)
      set(code "${JUCER_PLUGIN_AU_MAIN_TYPE}")
    else()
      _FRUT_get_au_quoted_four_chars("${JUCER_PLUGIN_AU_MAIN_TYPE}" quoted_four_chars)
      string(REPLACE "'" "" code "${quoted_four_chars}")
    endif()
  endif()

  set(${out_value} "${code}" PARENT_SCOPE)

endfunction()


function(_FRUT_compute_vst3_category out_var)

  set(categories "${JUCER_PLUGIN_VST3_CATEGORY}")

  # See getVST3CategoryStringFromSelection()
  # in JUCE/extras/Projucer/Source/Project/jucer_Project.cpp
  if(NOT "Fx" IN_LIST categories AND NOT "Instrument" IN_LIST categories)
    if(JUCER_PLUGIN_IS_A_SYNTH)
      list(INSERT categories 0 "Instrument")
    else()
      list(INSERT categories 0 "Fx")
    endif()
  else()
    if("Instrument" IN_LIST categories)
      list(FIND categories "Instrument" Instrument_index)
      list(REMOVE_AT categories ${Instrument_index})
      list(INSERT categories 0 "Instrument")
    endif()
    if("Fx" IN_LIST categories)
      list(FIND categories "Fx" Fx_index)
      list(REMOVE_AT categories ${Fx_index})
      list(INSERT categories 0 "Fx")
    endif()
  endif()

  list(LENGTH categories categories_count)
  if(categories_count EQUAL 1)
    set(vst3_category "${categories}")
  else()
    list(GET categories 0 first_category)
    set(vst3_category "${first_category}")
    math(EXPR categories_max "${categories_count} - 1")
    foreach(index RANGE 1 ${categories_max})
      list(GET categories ${index} category)
      string(APPEND vst3_category "|${category}")
    endforeach()
  endif()

  set(${out_var} "${vst3_category}" PARENT_SCOPE)

endfunction()


function(_FRUT_compute_rtas_aax_category RTAS_or_AAX category_prefix out_var)

  set(category_suffixes
    "None"
    "EQ"
    "Dynamics"
    "PitchShift"
    "Reverb"
    "Delay"
    "Modulation"
    "Harmonic"
    "NoiseReduction"
    "Dither"
    "SoundField"
    "HWGenerators"
    "SWGenerators"
    "WrappedPlugin"
    "Effect"
  )

  set(categories_as_int 0)

  foreach(category IN LISTS JUCER_PLUGIN_${RTAS_or_AAX}_CATEGORY)
    if(NOT category MATCHES "^${category_prefix}_(.+)$"
        OR NOT CMAKE_MATCH_1 IN_LIST category_suffixes)
      message(WARNING "Ignoring unknown ${RTAS_or_AAX} category: ${category}")
      continue()
    endif()
    list(FIND category_suffixes "${CMAKE_MATCH_1}" suffix_index)
    if(suffix_index EQUAL 0)
      set(category_as_int 0)
    else()
      math(EXPR category_as_int "1 << (${suffix_index} - 1)")
    endif()
    math(EXPR categories_as_int "${categories_as_int} | ${category_as_int}")
  endforeach()

  set(${out_var} "${categories_as_int}" PARENT_SCOPE)

endfunction()
