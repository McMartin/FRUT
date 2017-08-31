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

cmake_minimum_required(VERSION 3.4)


set(Reprojucer.cmake_DIR "${CMAKE_CURRENT_LIST_DIR}")
set(Reprojucer_templates_DIR "${Reprojucer.cmake_DIR}/templates")

set(Reprojucer_supported_exporters
  "Xcode (MacOSX)"
  "Visual Studio 2015"
  "Visual Studio 2013"
  "Linux Makefile"
)
set(Reprojucer_supported_exporters_conditions
  "APPLE"
  "MSVC_VERSION\;EQUAL\;1900"
  "MSVC_VERSION\;EQUAL\;1800"
  "CMAKE_HOST_SYSTEM_NAME\;STREQUAL\;Linux"
)


function(__set_Reprojucer_current_exporter)

  foreach(exporter_index RANGE 3)
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
      "https://github.com/McMartin/JUCE.cmake/issues/new"
    )
  endif()

  set(Reprojucer_current_exporter ${current_exporter} PARENT_SCOPE)

endfunction()

__set_Reprojucer_current_exporter()


function(jucer_project_begin)

  set(project_property_tags
    "PROJECT_FILE"
    "PROJECT_ID"
  )

  foreach(element ${ARGN})
    if(NOT DEFINED tag)
      set(tag ${element})

      if(NOT "${tag}" IN_LIST project_property_tags)
        message(FATAL_ERROR "Unsupported project property: ${tag}\n"
          "Supported project properties: ${project_property_tags}"
        )
      endif()
    else()
      set(value ${element})

      if(tag STREQUAL "PROJECT_FILE")
        if(NOT EXISTS "${value}")
          message(FATAL_ERROR "No such JUCE project file: ${value}")
        endif()

        get_filename_component(project_dir "${value}" DIRECTORY)
        set(JUCER_PROJECT_DIR "${project_dir}" PARENT_SCOPE)
      endif()

      set(JUCER_${tag} "${value}" PARENT_SCOPE)

      unset(tag)
    endif()
  endforeach()

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
    "COMPANY_WEBSITE"
    "COMPANY_EMAIL"
    "PROJECT_TYPE"
    "BUNDLE_IDENTIFIER"
    "BINARYDATACPP_SIZE_LIMIT"
    "BINARYDATA_NAMESPACE"
    "PREPROCESSOR_DEFINITIONS"
  )

  set(project_types "GUI Application" "Console Application" "Static Library"
    "Dynamic Library" "Audio Plug-in"
  )

  set(size_limit_descs "Default" "20.0 MB" "10.0 MB" "6.0 MB" "2.0 MB" "1.0 MB" "512.0 KB"
    "256.0 KB" "128.0 KB" "64.0 KB"
  )
  set(size_limits 10240 20480 10240 6144 2048 1024 512 256 128 64)

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
        __version_to_hex("${value}" hex_value)
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
  )

  foreach(element ${ARGN})
    if(NOT DEFINED tag)
      set(tag ${element})
    else()
      set(value ${element})

      if(NOT "${tag}" IN_LIST plugin_setting_tags)
        message(FATAL_ERROR "Unsupported audio plugin setting: ${tag}\n"
          "Supported audio plugin settings: ${plugin_setting_tags}"
        )
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

  foreach(element ${ARGN})
    if(NOT DEFINED compile)
      set(compile ${element})
      __check_input("${compile}")
    elseif(NOT DEFINED xcodeResource)
      set(xcodeResource ${element})
      __check_input("${xcodeResource}")
    elseif(NOT DEFINED binaryResource)
      set(binaryResource ${element})
      __check_input("${binaryResource}")
    else()
      set(path ${element})

      __abs_path_based_on_jucer_project_dir("${path}" path)
      list(APPEND files "${path}")

      if(xcodeResource STREQUAL "x")
        list(APPEND JUCER_PROJECT_XCODE_RESOURCES ${path})
      elseif(binaryResource STREQUAL "x")
        list(APPEND JUCER_PROJECT_RESOURCES ${path})
      else()
        list(APPEND JUCER_PROJECT_SOURCES ${path})

        get_filename_component(file_extension "${path}" EXT)

        if(NOT file_extension STREQUAL ".h" AND compile STREQUAL ".")
          set_source_files_properties("${path}" PROPERTIES HEADER_FILE_ONLY TRUE)
        endif()

        if(file_extension STREQUAL ".mm" AND NOT APPLE)
          set_source_files_properties("${path}" PROPERTIES HEADER_FILE_ONLY TRUE)
        endif()
      endif()

      unset(compile)
      unset(xcodeResource)
      unset(binaryResource)
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

  __abs_path_based_on_jucer_project_dir("${modules_folder}" modules_folder)
  if(NOT IS_DIRECTORY "${modules_folder}")
    message(FATAL_ERROR "No such directory: \"${modules_folder}\"")
  endif()
  list(APPEND JUCER_PROJECT_MODULES_FOLDERS "${modules_folder}")
  set(JUCER_PROJECT_MODULES_FOLDERS ${JUCER_PROJECT_MODULES_FOLDERS} PARENT_SCOPE)

  file(GLOB module_src_files
    "${modules_folder}/${module_name}/*.cpp"
    "${modules_folder}/${module_name}/*.mm"
  )

  foreach(src_file ${module_src_files})
    # See LibraryModule::CompileUnit::isNeededForExporter()
    # in JUCE/extras/Projucer/Source/Project/jucer_Module.cpp
    if(  (src_file MATCHES "_AU[._]"         AND NOT (JUCER_BUILD_AUDIOUNIT    AND APPLE))
      OR (src_file MATCHES "_AUv3[._]"       AND NOT (JUCER_BUILD_AUDIOUNIT_V3 AND APPLE))
      OR (src_file MATCHES "_AAX[._]"        AND NOT (JUCER_BUILD_AAX          AND TRUE ))
      OR (src_file MATCHES "_RTAS[._]"       AND NOT (JUCER_BUILD_RTAS         AND TRUE ))
      OR (src_file MATCHES "_VST2[._]"       AND NOT (JUCER_BUILD_VST          AND TRUE ))
      OR (src_file MATCHES "_VST3[._]"       AND NOT (JUCER_BUILD_VST3         AND TRUE ))
      OR (src_file MATCHES "_Standalone[._]" AND NOT (JUCER_BUILD_AUDIOUNIT_V3 AND APPLE))
    )
      set(to_compile FALSE)
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
        "JuceLibraryCode/${src_file_basename}"
      )
      list(APPEND JUCER_PROJECT_SOURCES
        "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode/${src_file_basename}"
      )
    endif()

    unset(to_compile)
  endforeach()

  set(JUCER_PROJECT_SOURCES ${JUCER_PROJECT_SOURCES} PARENT_SCOPE)

  set(module_header_file "${modules_folder}/${module_name}/${module_name}.h")

  file(STRINGS "${module_header_file}" config_flags_lines REGEX "/\\*\\* Config: ")
  string(REPLACE "/** Config: " "" module_config_flags "${config_flags_lines}")
  set(JUCER_${module_name}_CONFIG_FLAGS ${module_config_flags} PARENT_SCOPE)

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
    list(APPEND JUCER_PROJECT_OSX_FRAMEWORKS ${osx_frameworks})
    set(JUCER_PROJECT_OSX_FRAMEWORKS ${JUCER_PROJECT_OSX_FRAMEWORKS} PARENT_SCOPE)
  endif()

  if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    file(STRINGS "${module_header_file}" linux_libs_line REGEX "linuxLibs:")
    string(REPLACE "linuxLibs:" "" linux_libs_line "${linux_libs_line}")
    string(REPLACE " " ";" linux_libs "${linux_libs_line}")
    list(APPEND JUCER_PROJECT_LINUX_LIBS ${linux_libs})
    set(JUCER_PROJECT_LINUX_LIBS ${JUCER_PROJECT_LINUX_LIBS} PARENT_SCOPE)

    file(STRINGS "${module_header_file}" linux_packages_line REGEX "linuxPackages:")
    string(REPLACE "linuxPackages:" "" linux_packages_line "${linux_packages_line}")
    string(REPLACE " " ";" linux_packages "${linux_packages_line}")
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
      "TARGET_PROJECT_FOLDER"
      "VST3_SDK_FOLDER"
      "CUSTOM_XCODE_RESOURCE_FOLDERS"
      "EXTRA_FRAMEWORKS"
      "CUSTOM_PLIST"
      "PREBUILD_SHELL_SCRIPT"
      "POSTBUILD_SHELL_SCRIPT"
    )

    if(JUCER_PROJECT_TYPE STREQUAL "GUI Application")
      list(APPEND export_target_settings_tags
        "DOCUMENT_FILE_EXTENSIONS"
      )
    endif()
  endif()

  if(exporter STREQUAL "Visual Studio 2015" OR exporter STREQUAL "Visual Studio 2013")
    list(APPEND export_target_settings_tags
      "TARGET_PROJECT_FOLDER"
      "VST3_SDK_FOLDER"
      "PLATFORM_TOOLSET"
    )
  endif()

  if(exporter STREQUAL "Linux Makefile")
    list(APPEND export_target_settings_tags
      "CXX_STANDARD_TO_USE"
      "PKGCONFIG_LIBRARIES"
    )
  endif()

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
        __abs_path_based_on_jucer_project_dir("${value}" value)
        set(JUCER_TARGET_PROJECT_FOLDER ${value} PARENT_SCOPE)

      elseif(tag STREQUAL "VST_SDK_FOLDER")
        string(REPLACE "\\" "/" value "${value}")
        __abs_path_based_on_jucer_project_dir("${value}" value)
        set(JUCER_VST_SDK_FOLDER ${value} PARENT_SCOPE)

      elseif(tag STREQUAL "VST3_SDK_FOLDER")
        string(REPLACE "\\" "/" value "${value}")
        __abs_path_based_on_jucer_project_dir("${value}" value)
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
          __abs_path_based_on_jucer_project_dir("${value}" value)
          set(JUCER_SMALL_ICON ${value} PARENT_SCOPE)
        endif()

      elseif(tag STREQUAL "ICON_LARGE")
        if(NOT value STREQUAL "<None>")
          __abs_path_based_on_jucer_project_dir("${value}" value)
          set(JUCER_LARGE_ICON ${value} PARENT_SCOPE)
        endif()

      elseif(tag STREQUAL "CUSTOM_XCODE_RESOURCE_FOLDERS")
        string(REPLACE "\n" ";" value "${value}")
        set(resource_folders)
        foreach(folder ${value})
          __abs_path_based_on_jucer_project_dir("${folder}" abs_folder)
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

      elseif(tag STREQUAL "PLATFORM_TOOLSET")
        if((exporter STREQUAL "Visual Studio 2015"
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
          message(FATAL_ERROR
            "Unsupported value for PLATFORM_TOOLSET: \"${value}\"\n"
          )
        endif()

      elseif(tag STREQUAL "CXX_STANDARD_TO_USE")
        if(value MATCHES "^C\\+\\+(03|11|14)$")
          set(JUCER_CXX_LANGUAGE_STANDARD ${value} PARENT_SCOPE)
        else()
          message(FATAL_ERROR
            "Unsupported value for CXX_STANDARD_TO_USE: \"${value}\"\n"
          )
        endif()

      elseif(tag STREQUAL "PKGCONFIG_LIBRARIES")
        string(REPLACE " " ";" value "${value}")
        list(APPEND JUCER_PROJECT_LINUX_PACKAGES ${value})
        set(JUCER_PROJECT_LINUX_PACKAGES ${JUCER_PROJECT_LINUX_PACKAGES} PARENT_SCOPE)

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
      "CXX_LANGUAGE_STANDARD"
      "CXX_LIBRARY"
      "RELAX_IEEE_COMPLIANCE"
      "LINK_TIME_OPTIMISATION"
      "STRIP_LOCAL_SYMBOLS"
    )
  endif()

  if(exporter STREQUAL "Visual Studio 2015" OR exporter STREQUAL "Visual Studio 2013")
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

    if(is_debug)
      set(default_runtime_library_flag "/MTd")
    else()
      set(default_runtime_library_flag "/MT")
    endif()
    list(APPEND JUCER_COMPILER_FLAGS
      "$<$<CONFIG:${config}>:${default_runtime_library_flag}>"
    )
  endif()

  if(exporter STREQUAL "Linux Makefile")
    list(APPEND configuration_settings_tags
      "ARCHITECTURE"
    )
  endif()

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
        foreach(path ${value})
          __abs_path_based_on_jucer_project_dir("${path}" path)
          list(APPEND include_directories "${path}")
        endforeach()
        list(APPEND JUCER_INCLUDE_DIRECTORIES
          $<$<CONFIG:${config}>:${include_directories}>
        )
        set(JUCER_INCLUDE_DIRECTORIES ${JUCER_INCLUDE_DIRECTORIES} PARENT_SCOPE)

      elseif(tag STREQUAL "EXTRA_LIBRARY_SEARCH_PATHS")
        string(REPLACE "\\" "/" value "${value}")
        string(REPLACE "\n" ";" value "${value}")
        foreach(path ${value})
          __abs_path_based_on_jucer_project_dir("${path}" path)
          list(APPEND library_search_paths "${path}")
        endforeach()
        set(JUCER_EXTRA_LIBRARY_SEARCH_PATHS_${config}
          ${library_search_paths} PARENT_SCOPE
        )

      elseif(tag STREQUAL "PREPROCESSOR_DEFINITIONS")
        string(REPLACE "\n" ";" value "${value}")
        set(JUCER_PREPROCESSOR_DEFINITIONS_${config} ${value} PARENT_SCOPE)

      elseif(tag STREQUAL "OPTIMISATION")
        if(exporter MATCHES "Visual Studio 201(5|3)")
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
        if(value MATCHES "10\\.([5-9]|10|11|12) SDK")
          set(JUCER_OSX_BASE_SDK_VERSION_${config} "10.${CMAKE_MATCH_1}" PARENT_SCOPE)
        elseif(value STREQUAL "Use Default")
          set(JUCER_OSX_BASE_SDK_VERSION_${config} "default" PARENT_SCOPE)
        else()
          message(FATAL_ERROR
            "Unsupported value for OSX_BASE_SDK_VERSION: \"${value}\"\n"
          )
        endif()

      elseif(tag STREQUAL "OSX_DEPLOYMENT_TARGET")
        if(value MATCHES "10\\.([5-9]|10|11|12)")
          set(JUCER_OSX_DEPLOYMENT_TARGET_${config} "10.${CMAKE_MATCH_1}" PARENT_SCOPE)
        elseif(value STREQUAL "Use Default")
          set(JUCER_OSX_DEPLOYMENT_TARGET_${config} "default" PARENT_SCOPE)
        else()
          message(FATAL_ERROR
            "Unsupported value for OSX_DEPLOYMENT_TARGET: \"${value}\"\n"
          )
        endif()

      elseif(tag STREQUAL "OSX_ARCHITECTURE")
        if(value STREQUAL "Native architecture of build machine")
          # Consider as default
        elseif(value STREQUAL "Universal Binary (32-bit)")
          list(APPEND JUCER_OSX_ARCHITECTURES ${config} "i386")
        elseif(value STREQUAL "Universal Binary (32/64-bit)")
          list(APPEND JUCER_OSX_ARCHITECTURES ${config} "x86_64 i386")
        elseif(value STREQUAL "64-bit Intel")
          list(APPEND JUCER_OSX_ARCHITECTURES ${config} "x86_64")
        elseif(NOT value STREQUAL "Use Default")
          message(FATAL_ERROR "Unsupported value for OSX_ARCHITECTURE: \"${value}\"\n")
        endif()
        set(JUCER_OSX_ARCHITECTURES ${JUCER_OSX_ARCHITECTURES} PARENT_SCOPE)

      elseif(tag STREQUAL "CXX_LANGUAGE_STANDARD")
        if(value MATCHES "^(C|GNU)\\+\\+(98|11|14)$")
          set(JUCER_CXX_LANGUAGE_STANDARD ${value} PARENT_SCOPE)
        elseif(NOT value STREQUAL "Use Default")
          message(FATAL_ERROR
            "Unsupported value for CXX_LANGUAGE_STANDARD: \"${value}\"\n"
          )
        endif()

      elseif(tag STREQUAL "CXX_LIBRARY")
        if(value STREQUAL "LLVM libc++")
          set(JUCER_CXX_LIBRARY_${config} "libc++" PARENT_SCOPE)
        elseif(value STREQUAL "GNU libstdc++")
          set(JUCER_CXX_LIBRARY_${config} "libstdc++" PARENT_SCOPE)
        elseif(NOT value STREQUAL "Use Default")
          message(FATAL_ERROR "Unsupported value for CXX_LIBRARY: \"${value}\"")
        endif()

      elseif(tag STREQUAL "RELAX_IEEE_COMPLIANCE" AND exporter STREQUAL "Xcode (MacOSX)")
        if(value)
          list(APPEND JUCER_COMPILER_FLAGS $<$<CONFIG:${config}>:-ffast-math>)
        endif()

      elseif(tag STREQUAL "LINK_TIME_OPTIMISATION")
        if(value)
          list(APPEND JUCER_COMPILER_FLAGS $<$<CONFIG:${config}>:-flto>)
        endif()

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
          message(FATAL_ERROR "Unsupported value for WARNING_LEVEL: \"${value}\"\n")
        endif()
        list(APPEND JUCER_COMPILER_FLAGS $<$<CONFIG:${config}>:/W${level}>)

      elseif(tag STREQUAL "TREAT_WARNINGS_AS_ERRORS")
        if(value)
          list(APPEND JUCER_COMPILER_FLAGS $<$<CONFIG:${config}>:/WX>)
        endif()

      elseif(tag STREQUAL "RUNTIME_LIBRARY")
        if(value STREQUAL "Use DLL runtime")
          if(is_debug)
            set(runtime_library_flag "/MDd")
          else()
            set(runtime_library_flag "/MD")
          endif()
          list(APPEND JUCER_COMPILER_FLAGS $<$<CONFIG:${config}>:${runtime_library_flag}>)
        elseif(NOT value STREQUAL "Use static runtime" AND NOT value STREQUAL "(Default)")
          message(FATAL_ERROR "Unsupported value for RUNTIME_LIBRARY: \"${value}\"\n")
        endif()

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

      elseif(tag STREQUAL "ARCHITECTURE" AND exporter MATCHES "Visual Studio 201(5|3)")
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

      elseif(tag STREQUAL "RELAX_IEEE_COMPLIANCE"
          AND exporter MATCHES "Visual Studio 201(5|3)")
        if(value)
          list(APPEND JUCER_COMPILER_FLAGS $<$<CONFIG:${config}>:/fp:fast>)
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
          message(FATAL_ERROR "Unsupported value for ARCHITECTURE: \"${value}\"\n")
        endif()
        if(DEFINED architecture_flag)
          list(APPEND JUCER_COMPILER_FLAGS $<$<CONFIG:${config}>:${architecture_flag}>)
        endif()

      endif()

      unset(tag)
    endif()
  endforeach()

  set(JUCER_COMPILER_FLAGS ${JUCER_COMPILER_FLAGS} PARENT_SCOPE)

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
    if(CMAKE_BUILD_TYPE STREQUAL "")
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
    set(config_to_value)
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
    set(config_to_value)
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

  __generate_AppConfig_header()
  __generate_JuceHeader_header()

  if(DEFINED JUCER_SMALL_ICON OR DEFINED JUCER_LARGE_ICON)
    if(APPLE)
      __generate_icon_file("icns" icon_filename)
    elseif(WIN32)
      __generate_icon_file("ico" icon_filename)
    endif()

    if(DEFINED icon_filename)
      set(JUCER_BUNDLE_ICON_FILE ${icon_filename})
    endif()
  endif()

  if(WIN32 AND NOT JUCER_PROJECT_TYPE STREQUAL "Static Library")
    if(DEFINED icon_filename)
      string(APPEND resources_rc_icon_settings
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
    <string>@JUCER_COMPANY_NAME@</string>
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

  set(all_sources
    ${JUCER_PROJECT_SOURCES}
    ${JUCER_PROJECT_RESOURCES}
    ${JUCER_PROJECT_BROWSABLE_FILES}
  )

  if(APPLE)
    list(APPEND all_sources ${JUCER_PROJECT_XCODE_RESOURCES})
  endif()

  set_source_files_properties(${JUCER_PROJECT_XCODE_RESOURCES}
    PROPERTIES MACOSX_PACKAGE_LOCATION "Resources"
  )

  if(JUCER_PROJECT_TYPE STREQUAL "Console Application")
    add_executable(${target} ${all_sources})
    __set_common_target_properties(${target})
    __link_osx_frameworks(${target}
      ${JUCER_PROJECT_OSX_FRAMEWORKS} ${JUCER_EXTRA_FRAMEWORKS}
    )

  elseif(JUCER_PROJECT_TYPE STREQUAL "GUI Application")
    add_executable(${target} ${all_sources})
    set_target_properties(${target} PROPERTIES MACOSX_BUNDLE TRUE)

    if(JUCER_DOCUMENT_FILE_EXTENSIONS)
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

    __generate_plist_file(${target} "App" "APPL" "????"
      "${main_plist_entries}" "${bundle_document_types_entries}"
    )
    set_target_properties(${target} PROPERTIES WIN32_EXECUTABLE TRUE)
    __set_common_target_properties(${target})
    __link_osx_frameworks(${target}
      ${JUCER_PROJECT_OSX_FRAMEWORKS} ${JUCER_EXTRA_FRAMEWORKS}
    )
    __add_xcode_resources(${target} ${JUCER_CUSTOM_XCODE_RESOURCE_FOLDERS})

  elseif(JUCER_PROJECT_TYPE STREQUAL "Static Library")
    add_library(${target} STATIC ${all_sources})
    __set_common_target_properties(${target})

  elseif(JUCER_PROJECT_TYPE STREQUAL "Dynamic Library")
    add_library(${target} SHARED ${all_sources})
    __set_common_target_properties(${target})

  elseif(JUCER_PROJECT_TYPE STREQUAL "Audio Plug-in")
    if(APPLE)
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
        "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode/AppConfig.h"
        "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode/JuceHeader.h"
        ${JUCER_PROJECT_BROWSABLE_FILES}
      )
      __set_common_target_properties(${shared_code_target})
      target_compile_definitions(${shared_code_target} PRIVATE "JUCE_SHARED_CODE=1")
      __set_JucePlugin_Build_defines(${shared_code_target} "SharedCodeTarget")

      if(JUCER_BUILD_VST)
        set(vst_target ${target}_VST)
        add_library(${vst_target} MODULE
          ${VST_sources}
          ${JUCER_PROJECT_XCODE_RESOURCES}
        )
        target_link_libraries(${vst_target} ${shared_code_target})
        __generate_plist_file(${vst_target} "VST" "BNDL" "????"
          "${main_plist_entries}" ""
        )
        __set_bundle_properties(${vst_target} "vst")
        __set_common_target_properties(${vst_target})
        __install_to_plugin_binary_location(${vst_target} "VST"
          "$ENV{HOME}/Library/Audio/Plug-Ins/VST"
        )
        __set_JucePlugin_Build_defines(${vst_target} "VSTPlugIn")
        __link_osx_frameworks(${vst_target}
          ${JUCER_PROJECT_OSX_FRAMEWORKS} ${JUCER_EXTRA_FRAMEWORKS}
        )
        __add_xcode_resources(${vst_target} ${JUCER_CUSTOM_XCODE_RESOURCE_FOLDERS})
      endif()

      if(JUCER_BUILD_VST3)
        set(vst3_target ${target}_VST3)
        add_library(${vst3_target} MODULE
          ${VST3_sources}
          ${JUCER_PROJECT_XCODE_RESOURCES}
        )
        target_link_libraries(${vst3_target} ${shared_code_target})
        __generate_plist_file(${vst3_target} "VST3" "BNDL" "????"
          "${main_plist_entries}" ""
        )
        __set_bundle_properties(${vst3_target} "vst3")
        __set_common_target_properties(${vst3_target})
        __install_to_plugin_binary_location(${vst_target} "VST3"
          "$ENV{HOME}/Library/Audio/Plug-Ins/VST3"
        )
        __set_JucePlugin_Build_defines(${vst3_target} "VST3PlugIn")
        __link_osx_frameworks(${vst3_target}
          ${JUCER_PROJECT_OSX_FRAMEWORKS} ${JUCER_EXTRA_FRAMEWORKS}
        )
        __add_xcode_resources(${vst3_target} ${JUCER_CUSTOM_XCODE_RESOURCE_FOLDERS})
      endif()

      if(JUCER_BUILD_AUDIOUNIT)
        set(au_target ${target}_AU)
        add_library(${au_target} MODULE
          ${AudioUnit_sources}
          ${JUCER_PROJECT_XCODE_RESOURCES}
        )
        target_link_libraries(${au_target} ${shared_code_target})

        __get_au_main_type_code(au_main_type_code)
        __version_to_dec("${JUCER_PROJECT_VERSION}" dec_version)

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

        __generate_plist_file(${au_target} "AU" "BNDL" "????"
          "${main_plist_entries}" "${audio_components_entries}"
        )
        __set_bundle_properties(${au_target} "component")
        __set_common_target_properties(${au_target})
        __install_to_plugin_binary_location(${au_target} "AU"
          "$ENV{HOME}/Library/Audio/Plug-Ins/Components"
        )
        __set_JucePlugin_Build_defines(${au_target} "AudioUnitPlugIn")
        set(au_plugin_osx_frameworks
          ${JUCER_PROJECT_OSX_FRAMEWORKS} ${JUCER_EXTRA_FRAMEWORKS}
          "AudioUnit" "CoreAudioKit"
        )
        __link_osx_frameworks(${au_target} ${au_plugin_osx_frameworks})
        __add_xcode_resources(${au_target} ${JUCER_CUSTOM_XCODE_RESOURCE_FOLDERS})
      endif()

      if(JUCER_BUILD_AUDIOUNIT_V3)
        set(auv3_target ${target}_AUv3_AppExtension)
        add_library(${auv3_target} MODULE
          ${AudioUnitv3_sources}
          ${JUCER_PROJECT_XCODE_RESOURCES}
        )
        target_link_libraries(${auv3_target} ${shared_code_target})

        __get_au_main_type_code(au_main_type_code)
        __version_to_dec("${JUCER_PROJECT_VERSION}" dec_version)
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

        __generate_plist_file(${auv3_target} "AUv3_AppExtension" "XPC!" "????"
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

        # Cannot use __set_bundle_properties() since Projucer sets xcodeIsBundle=false
        # for this target, though it is a bundle...
        set_target_properties(${auv3_target} PROPERTIES
          BUNDLE TRUE
          BUNDLE_EXTENSION "appex"
          XCODE_ATTRIBUTE_WRAPPER_EXTENSION "appex"
        )
        __set_common_target_properties(${auv3_target})
        __set_JucePlugin_Build_defines(${auv3_target} "AudioUnitv3PlugIn")
        set(auv3_plugin_osx_frameworks
          ${JUCER_PROJECT_OSX_FRAMEWORKS} ${JUCER_EXTRA_FRAMEWORKS}
          "AudioUnit" "CoreAudioKit" "AVFoundation"
        )
        __link_osx_frameworks(${auv3_target} ${auv3_plugin_osx_frameworks})
        __add_xcode_resources(${auv3_target} ${JUCER_CUSTOM_XCODE_RESOURCE_FOLDERS})
      endif()

      if(JUCER_BUILD_AUDIOUNIT_V3)
        set(standalone_target ${target}_AUv3_Standalone)
        add_executable(${standalone_target} MACOSX_BUNDLE
          ${Standalone_sources}
          ${JUCER_PROJECT_XCODE_RESOURCES}
        )
        target_link_libraries(${standalone_target} ${shared_code_target})
        add_dependencies(${standalone_target} ${auv3_target})
        __generate_plist_file(${standalone_target} "AUv3_Standalone" "APPL" "????"
          "${main_plist_entries}" ""
        )
        __set_common_target_properties(${standalone_target})
        __set_JucePlugin_Build_defines(${standalone_target} "StandalonePlugIn")
        __link_osx_frameworks(${standalone_target}
          ${JUCER_PROJECT_OSX_FRAMEWORKS} ${JUCER_EXTRA_FRAMEWORKS}
        )
        __add_xcode_resources(${standalone_target} ${JUCER_CUSTOM_XCODE_RESOURCE_FOLDERS})
        install(TARGETS ${auv3_target} COMPONENT _embed_app_extension_in_standalone_app
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
    else()
      add_library(${target} MODULE ${all_sources})
      set_target_properties(${target} PROPERTIES PREFIX "")
      __set_common_target_properties(${target})
    endif()

  else()
    message(FATAL_ERROR "Unknown project type: ${JUCER_PROJECT_TYPE}")

  endif()

endfunction()


function(__abs_path_based_on_jucer_project_dir in_path out_path)

  if(NOT IS_ABSOLUTE "${in_path}" AND NOT DEFINED JUCER_PROJECT_DIR)
    message(FATAL_ERROR "The path \"${in_path}\" must be absolute, unless you give "
      "PROJECT_FILE when calling jucer_project_begin()."
    )
  endif()

  get_filename_component(in_path "${in_path}" ABSOLUTE BASE_DIR "${JUCER_PROJECT_DIR}")
  set(${out_path} ${in_path} PARENT_SCOPE)

endfunction()


function(__generate_AppConfig_header)

  set(max_right_padding 0)
  foreach(module_name ${JUCER_PROJECT_MODULES})
    string(LENGTH "${module_name}" module_name_length)
    if(module_name_length GREATER max_right_padding)
      set(max_right_padding ${module_name_length})
    endif()
  endforeach()
  math(EXPR max_right_padding "${max_right_padding} + 5")

  foreach(module_name ${JUCER_PROJECT_MODULES})
    string(LENGTH "${module_name}" right_padding)
    while(right_padding LESS max_right_padding)
      string(APPEND padding_spaces " ")
      math(EXPR right_padding "${right_padding} + 1")
    endwhile()
    string(APPEND module_available_defines
      "#define JUCE_MODULE_AVAILABLE_${module_name}${padding_spaces} 1\n"
    )
    unset(padding_spaces)

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

    __bool_to_int("${JUCER_BUILD_VST}" Build_VST_value)
    list(APPEND plugin_settings "Build_VST" "${Build_VST_value}")

    __bool_to_int("${JUCER_BUILD_VST3}" Build_VST3_value)
    list(APPEND plugin_settings "Build_VST3" "${Build_VST3_value}")

    __bool_to_int("${JUCER_BUILD_AUDIOUNIT}" Build_AU_value)
    list(APPEND plugin_settings "Build_AU" "${Build_AU_value}")

    __bool_to_int("${JUCER_BUILD_AUDIOUNIT_V3}" Build_AUv3_value)
    list(APPEND plugin_settings "Build_AUv3" "${Build_AUv3_value}")

    __bool_to_int("${JUCER_BUILD_AUDIOUNIT_V3}" Build_STANDALONE_value)
    list(APPEND plugin_settings "Build_STANDALONE" "${Build_STANDALONE_value}")

    list(APPEND plugin_settings "Name" "\"${JUCER_PLUGIN_NAME}\"")
    list(APPEND plugin_settings "Desc" "\"${JUCER_PLUGIN_DESCRIPTION}\"")
    list(APPEND plugin_settings "Manufacturer" "\"${JUCER_PLUGIN_MANUFACTURER}\"")
    list(APPEND plugin_settings "ManufacturerWebsite" "\"${JUCER_COMPANY_WEBSITE}\"")
    list(APPEND plugin_settings "ManufacturerEmail" "\"${JUCER_COMPANY_EMAIL}\"")

    __four_chars_to_hex("${JUCER_PLUGIN_MANUFACTURER_CODE}" ManufacturerCode_value)
    list(APPEND plugin_settings "ManufacturerCode"
      "${ManufacturerCode_value} // '${JUCER_PLUGIN_MANUFACTURER_CODE}'"
    )

    __four_chars_to_hex("${JUCER_PLUGIN_CODE}" PluginCode_value)
    list(APPEND plugin_settings "PluginCode"
      "${PluginCode_value} // '${JUCER_PLUGIN_CODE}'"
    )

    __bool_to_int("${JUCER_PLUGIN_IS_A_SYNTH}" IsSynth_value)
    list(APPEND plugin_settings "IsSynth" "${IsSynth_value}")

    __bool_to_int("${JUCER_PLUGIN_MIDI_INPUT}" WantsMidiInput_value)
    list(APPEND plugin_settings "WantsMidiInput" "${WantsMidiInput_value}")

    __bool_to_int("${JUCER_PLUGIN_MIDI_OUTPUT}" ProducesMidiOutput_value)
    list(APPEND plugin_settings "ProducesMidiOutput" "${ProducesMidiOutput_value}")

    __bool_to_int("${JUCER_MIDI_EFFECT_PLUGIN}" IsMidiEffect_value)
    list(APPEND plugin_settings "IsMidiEffect" "${IsMidiEffect_value}")

    __bool_to_int("${JUCER_KEY_FOCUS}" EditorRequiresKeyboardFocus_value)
    list(APPEND plugin_settings "EditorRequiresKeyboardFocus"
      "${EditorRequiresKeyboardFocus_value}"
    )

    list(APPEND plugin_settings "Version" "${JUCER_PROJECT_VERSION}")

    __version_to_hex("${JUCER_PROJECT_VERSION}" VersionCode_value)
    list(APPEND plugin_settings "VersionCode" "${VersionCode_value}")

    list(APPEND plugin_settings "VersionString" "\"${JUCER_PROJECT_VERSION}\"")

    list(APPEND plugin_settings "VSTUniqueID" "JucePlugin_PluginCode")

    if(NOT DEFINED JUCER_VST_CATEGORY)
      if(JUCER_PLUGIN_IS_A_SYNTH)
        set(VSTCategory_value "kPlugCategSynth")
      else()
        set(VSTCategory_value "kPlugCategEffect")
      endif()
    else()
      set(VSTCategory_value "${JUCER_VST_CATEGORY}")
    endif()
    list(APPEND plugin_settings "VSTCategory" "${VSTCategory_value}")

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
    list(APPEND plugin_settings "AUMainType" "${AUMainType_value}")

    list(APPEND plugin_settings "AUSubType" "JucePlugin_PluginCode")
    list(APPEND plugin_settings "AUExportPrefix" "${JUCER_PLUGIN_AU_EXPORT_PREFIX}")
    list(APPEND plugin_settings "AUExportPrefixQuoted"
      "\"${JUCER_PLUGIN_AU_EXPORT_PREFIX}\""
    )
    list(APPEND plugin_settings "AUManufacturerCode" "JucePlugin_ManufacturerCode")

    list(APPEND plugin_settings "CFBundleIdentifier" "${JUCER_BUNDLE_IDENTIFIER}")

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

      list(APPEND plugin_settings "MaxNumInputChannels" "${max_num_input}")
      list(APPEND plugin_settings "MaxNumOutputChannels" "${max_num_output}")
      list(APPEND plugin_settings "PreferredChannelConfigurations"
        "${JUCER_PLUGIN_CHANNEL_CONFIGURATIONS}"
      )
    endif()

    string(CONCAT audio_plugin_settings_defines
      "//==============================================================================\n"
      "// Audio plugin settings..\n\n"
    )

    foreach(element ${plugin_settings})
      if(NOT DEFINED setting_name)
        set(setting_name "${element}")
      else()
        set(setting_value "${element}")

        string(LENGTH "JucePlugin_${setting_name}" right_padding)
        while(right_padding LESS 32)
          string(APPEND padding_spaces " ")
          math(EXPR right_padding "${right_padding} + 1")
        endwhile()

        string(APPEND audio_plugin_settings_defines
          "#ifndef  JucePlugin_${setting_name}\n"
        )
        string(APPEND audio_plugin_settings_defines
          " #define JucePlugin_${setting_name}${padding_spaces}  ${setting_value}\n"
        )
        string(APPEND audio_plugin_settings_defines "#endif\n")
        unset(padding_spaces)

        unset(setting_name)
      endif()
    endforeach()
  endif()

  string(TOUPPER "${JUCER_PROJECT_ID}" upper_project_id)
  configure_file("${Reprojucer_templates_DIR}/AppConfig.h" "JuceLibraryCode/AppConfig.h")
  list(APPEND JUCER_PROJECT_SOURCES
    "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode/AppConfig.h"
  )

  set(JUCER_PROJECT_SOURCES ${JUCER_PROJECT_SOURCES} PARENT_SCOPE)

endfunction()


function(__generate_JuceHeader_header)

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
      set(project_uid "JUCE.cmake")
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
    if(NOT DEFINED JUCER_BINARYDATA_NAMESPACE)
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

  foreach(module_name ${JUCER_PROJECT_MODULES})
    string(APPEND modules_includes "#include <${module_name}/${module_name}.h>\n")
  endforeach()

  string(TOUPPER "${JUCER_PROJECT_ID}" upper_project_id)
  configure_file("${Reprojucer_templates_DIR}/JuceHeader.h"
    "JuceLibraryCode/JuceHeader.h"
  )
  list(APPEND JUCER_PROJECT_SOURCES
    "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode/JuceHeader.h"
  )

  set(JUCER_PROJECT_SOURCES ${JUCER_PROJECT_SOURCES} PARENT_SCOPE)

endfunction()


function(__generate_icon_file icon_format out_icon_filename)

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

  if(NOT icon_filename STREQUAL "")
    set(${out_icon_filename} ${icon_filename} PARENT_SCOPE)

    list(APPEND JUCER_PROJECT_SOURCES
      "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode/${icon_filename}"
    )
    set(JUCER_PROJECT_SOURCES ${JUCER_PROJECT_SOURCES} PARENT_SCOPE)
  endif()

endfunction()


function(__set_common_target_properties target)

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

    if(DEFINED JUCER_BINARY_LOCATION_${config})
      set(output_directory "${JUCER_BINARY_LOCATION_${config}}")
      set_target_properties(${target} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY_${upper_config} "${output_directory}"
        RUNTIME_OUTPUT_DIRECTORY_${upper_config} "${output_directory}"
      )
    endif()
  endforeach()

  target_include_directories(${target} PRIVATE
    "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode"
    ${JUCER_PROJECT_MODULES_FOLDERS}
    ${JUCER_INCLUDE_DIRECTORIES}
  )

  if((JUCER_BUILD_VST OR JUCER_FLAG_JUCE_PLUGINHOST_VST) AND DEFINED JUCER_VST_SDK_FOLDER)
    if(NOT IS_DIRECTORY "${JUCER_VST_SDK_FOLDER}")
      message(WARNING
        "JUCER_VST_SDK_FOLDER: no such directory \"${JUCER_VST_SDK_FOLDER}\""
      )
    else()
      if(NOT EXISTS "${JUCER_VST_SDK_FOLDER}/public.sdk/source/vst2.x/audioeffectx.h")
        message(WARNING "JUCER_VST_SDK_FOLDER: \"${JUCER_VST_SDK_FOLDER}\" doesn't "
          "seem to contain the VST SDK"
        )
      endif()
    endif()
    target_include_directories(${target} PRIVATE "${JUCER_VST_SDK_FOLDER}")
  endif()

  if((JUCER_BUILD_VST3 OR JUCER_FLAG_JUCE_PLUGINHOST_VST3)
      AND DEFINED JUCER_VST3_SDK_FOLDER)
    if(NOT IS_DIRECTORY "${JUCER_VST3_SDK_FOLDER}")
      message(WARNING
        "JUCER_VST3_SDK_FOLDER: no such directory \"${JUCER_VST3_SDK_FOLDER}\""
      )
    else()
      if(NOT EXISTS "${JUCER_VST3_SDK_FOLDER}/base/source/baseiids.cpp")
        message(WARNING "JUCER_VST3_SDK_FOLDER: \"${JUCER_VST3_SDK_FOLDER}\" doesn't "
          "seem to contain the VST3 SDK"
        )
      endif()
    endif()
    target_include_directories(${target} PRIVATE "${JUCER_VST3_SDK_FOLDER}")
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

  target_compile_options(${target} PRIVATE ${JUCER_COMPILER_FLAGS})
  target_link_libraries(${target} ${JUCER_EXTERNAL_LIBRARIES_TO_LINK})

  if(APPLE)
    set_target_properties(${target} PROPERTIES CXX_EXTENSIONS OFF)
    set_target_properties(${target} PROPERTIES CXX_STANDARD 11)

    if(DEFINED JUCER_CXX_LANGUAGE_STANDARD)
      if(JUCER_CXX_LANGUAGE_STANDARD MATCHES "^GNU\\+\\+$")
        set_target_properties(${target} PROPERTIES CXX_EXTENSIONS ON)
      elseif(JUCER_CXX_LANGUAGE_STANDARD MATCHES "^C\\+\\+$")
        set_target_properties(${target} PROPERTIES CXX_EXTENSIONS OFF)
      endif()
      if(JUCER_CXX_LANGUAGE_STANDARD MATCHES "98$")
        set_target_properties(${target} PROPERTIES CXX_STANDARD 98)
      elseif(JUCER_CXX_LANGUAGE_STANDARD MATCHES "11$")
        set_target_properties(${target} PROPERTIES CXX_STANDARD 11)
      elseif(JUCER_CXX_LANGUAGE_STANDARD MATCHES "14$")
        set_target_properties(${target} PROPERTIES CXX_STANDARD 14)
      endif()
    endif()

    get_target_property(target_type ${target} TYPE)

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

      foreach(path ${JUCER_EXTRA_LIBRARY_SEARCH_PATHS_${config}})
        target_link_libraries(${target}
          $<$<CONFIG:${config}>:-L${path}>
        )
      endforeach()

      if(target_type STREQUAL EXECUTABLE OR target_type STREQUAL MODULE_LIBRARY)
        if(${JUCER_STRIP_LOCAL_SYMBOLS_${config}})
          find_program(strip_exe "strip")
          if(NOT strip_exe)
            message(FATAL_ERROR "Could not find strip program")
          endif()
          list(APPEND strip_command
            "$<$<CONFIG:${config}>:${strip_exe}>"
            "$<$<CONFIG:${config}>:-x>"
            "$<$<CONFIG:${config}>:$<TARGET_FILE:${target}>>"
          )
        endif()
      endif()
    endforeach()

    if(strip_command)
      add_custom_command(TARGET ${target} POST_BUILD COMMAND ${strip_command})
    endif()

    foreach(item ${JUCER_OSX_ARCHITECTURES})
      if(NOT DEFINED config)
        set(config ${item})
      else()
        string(TOUPPER "${config}" upper_config)
        string(REPLACE " " ";" archs "${item}")

        set_target_properties(${target} PROPERTIES
          OSX_ARCHITECTURES_${upper_config} "${archs}"
        )

        unset(config)
      endif()
    endforeach()

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

  elseif(WIN32)
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

      foreach(path ${JUCER_EXTRA_LIBRARY_SEARCH_PATHS_${config}})
        target_link_libraries(${target}
          $<$<CONFIG:${config}>:-LIBPATH:${path}>
        )
      endforeach()

      if(DEFINED JUCER_INCREMENTAL_LINKING_${config})
        if(JUCER_INCREMENTAL_LINKING_${config})
          string(TOUPPER "${config}" upper_config)
          get_target_property(link_flags ${target} LINK_FLAGS_${upper_config})
          if(link_flags)
            string(APPEND link_flags " /INCREMENTAL")
          else()
            set(link_flags "/INCREMENTAL")
          endif()
          set_target_properties(${target} PROPERTIES
            LINK_FLAGS_${upper_config} "${link_flags}"
          )
        endif()
      endif()

      if(DEFINED JUCER_PREBUILD_COMMAND_${config})
        set(prebuild_command ${JUCER_PREBUILD_COMMAND_${config}})
        list(APPEND all_confs_prebuild_command
          $<$<CONFIG:${config}>:${prebuild_command}>
        )
      endif()

      if(DEFINED JUCER_POSTBUILD_COMMAND_${config})
        set(postbuild_command ${JUCER_POSTBUILD_COMMAND_${config}})
        list(APPEND all_confs_postbuild_command
          $<$<CONFIG:${config}>:${postbuild_command}>
        )
      endif()

      if(DEFINED JUCER_GENERATE_MANIFEST_${config})
        if(NOT JUCER_GENERATE_MANIFEST_${config})
          string(TOUPPER "${config}" upper_config)
          get_target_property(link_flags ${target} LINK_FLAGS_${upper_config})
          if(link_flags)
            string(APPEND link_flags " /MANIFEST:NO")
          else()
            set(link_flags "/MANIFEST:NO")
          endif()
          set_target_properties(${target} PROPERTIES
            LINK_FLAGS_${upper_config} "${link_flags}"
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

      foreach(path ${JUCER_EXTRA_LIBRARY_SEARCH_PATHS_${config}})
        target_link_libraries(${target}
          $<$<CONFIG:${config}>:-L${path}>
        )
      endforeach()
    endforeach()

    set(linux_packages ${JUCER_PROJECT_LINUX_PACKAGES})
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
        target_link_libraries(${target} ${${pkg}_LIBRARIES})
      endforeach()
    else()
      if("juce_graphics" IN_LIST JUCER_PROJECT_MODULES)
        target_include_directories(${target} PRIVATE "/usr/include/freetype2")
      endif()
      if(JUCER_FLAG_JUCE_USE_CURL)
        target_link_libraries(${target} "-lcurl")
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
        target_link_libraries(${target} "-l${item}")
      endforeach()
    endif()
  endif()

  target_compile_options(${target} PRIVATE ${JUCER_EXTRA_COMPILER_FLAGS})
  target_link_libraries(${target} ${JUCER_EXTRA_LINKER_FLAGS})

endfunction()


function(__generate_plist_file
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


function(__set_bundle_properties target extension)

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

endfunction()


function(__install_to_plugin_binary_location target plugin_type default_destination)

  foreach(config ${JUCER_PROJECT_CONFIGURATIONS})
    if(DEFINED JUCER_${plugin_type}_BINARY_LOCATION_${config})
      set(destination ${JUCER_${plugin_type}_BINARY_LOCATION_${config}})
    else()
      set(destination ${default_destination})
    endif()
    string(APPEND all_confs_destination "$<$<CONFIG:${config}>:${destination}>")
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


function(__set_JucePlugin_Build_defines target target_type)

  # See XCodeProjectExporter::Target::getTargetSettings()
  # in JUCE/extras/Projucer/Source/Project Saving/jucer_ProjectExport_XCode.h
  set(plugin_types     VST VST3 AudioUnit AudioUnitv3  RTAS AAX Standalone  )
  set(setting_suffixes VST VST3 AUDIOUNIT AUDIOUNIT_V3 RTAS AAX AUDIOUNIT_V3)
  set(define_suffixes  VST VST3 AU        AUv3         RTAS AAX Standalone  )

  foreach(index RANGE 6)
    list(GET setting_suffixes ${index} setting_suffix)
    list(GET plugin_types ${index} plugin_type)
    list(GET define_suffixes ${index} define_suffix)

    if(JUCER_BUILD_${setting_suffix} AND (target_type STREQUAL "SharedCodeTarget"
        OR target_type STREQUAL "${plugin_type}PlugIn"))
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


function(__link_osx_frameworks target)

  set(osx_frameworks ${ARGN})

  if(APPLE)
    if(JUCER_FLAG_JUCE_PLUGINHOST_AU)
      list(APPEND osx_frameworks "AudioUnit" "CoreAudioKit")
    endif()
    list(SORT osx_frameworks)
    list(REMOVE_DUPLICATES osx_frameworks)
    foreach(framework_name ${osx_frameworks})
      find_library(${framework_name}_framework ${framework_name})
      target_link_libraries(${target} "${${framework_name}_framework}")
    endforeach()
  endif()

endfunction()


function(__add_xcode_resources target)

  set(resource_folders ${ARGN})

  if(APPLE)
    foreach(folder ${resource_folders})
      add_custom_command(TARGET ${target} PRE_BUILD
        COMMAND rsync -r "${folder}" "$<TARGET_FILE_DIR:${target}>/../Resources"
      )
    endforeach()
  endif()

endfunction()


function(__bool_to_int bool_value out_int_value)

  if(bool_value)
    set(${out_int_value} 1 PARENT_SCOPE)
  else()
    set(${out_int_value} 0 PARENT_SCOPE)
  endif()

endfunction()


function(__dec_to_hex dec_value out_hex_value)

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


function(__version_to_dec version out_dec_value)

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


function(__version_to_hex version out_hex_value)

  __version_to_dec("${version}" dec_value)
  __dec_to_hex("${dec_value}" hex_value)
  set(${out_hex_value} "${hex_value}" PARENT_SCOPE)

endfunction()


function(__four_chars_to_hex value out_hex_value)

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

  __dec_to_hex("${dec_value}" hex_value)
  set(${out_hex_value} "${hex_value}" PARENT_SCOPE)

endfunction()


function(__get_au_main_type_code out_value)

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
