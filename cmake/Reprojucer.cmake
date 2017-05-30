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
)
set(Reprojucer_supported_exporters_conditions
  "APPLE"
  "MSVC_VERSION\;EQUAL\;1900"
  "MSVC_VERSION\;EQUAL\;1800"
)


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

  set(project_types
    "GUI Application" "Console Application" "Static Library" "Audio Plug-in"
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
    "BUILD_AUDIOUNIT"
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

  file(STRINGS "${module_header_file}" osx_frameworks_line REGEX "OSXFrameworks:")
  string(REPLACE "OSXFrameworks:" "" osx_frameworks_line "${osx_frameworks_line}")
  string(REPLACE " " ";" osx_frameworks "${osx_frameworks_line}")
  list(APPEND JUCER_PROJECT_OSX_FRAMEWORKS ${osx_frameworks})
  set(JUCER_PROJECT_OSX_FRAMEWORKS ${JUCER_PROJECT_OSX_FRAMEWORKS} PARENT_SCOPE)

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
  list(APPEND JUCER_EXPORT_TARGETS "${exporter}")
  set(JUCER_EXPORT_TARGETS ${JUCER_EXPORT_TARGETS} PARENT_SCOPE)

  list(FIND Reprojucer_supported_exporters "${exporter}" exporter_index)
  list(GET Reprojucer_supported_exporters_conditions ${exporter_index} condition)
  if(NOT ${condition})
    return()
  endif()

  set(export_target_settings_tags
    "TARGET_PROJECT_FOLDER"
    "VST3_SDK_FOLDER"
    "EXTRA_PREPROCESSOR_DEFINITIONS"
    "EXTRA_COMPILER_FLAGS"
  )

  if(exporter STREQUAL "Xcode (MacOSX)")
    list(APPEND export_target_settings_tags
      "TARGET_PROJECT_FOLDER"
      "PREBUILD_SHELL_SCRIPT"
      "POSTBUILD_SHELL_SCRIPT"
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

      elseif(tag STREQUAL "VST3_SDK_FOLDER")
        string(REPLACE "\\" "/" value "${value}")
        __abs_path_based_on_jucer_project_dir("${value}" value)
        set(JUCER_VST3_SDK_FOLDER ${value} PARENT_SCOPE)

      elseif(tag STREQUAL "EXTRA_PREPROCESSOR_DEFINITIONS")
        string(REPLACE "\n" ";" value "${value}")
        list(APPEND JUCER_PREPROCESSOR_DEFINITIONS ${value})
        set(JUCER_PREPROCESSOR_DEFINITIONS ${JUCER_PREPROCESSOR_DEFINITIONS} PARENT_SCOPE)

      elseif(tag STREQUAL "EXTRA_COMPILER_FLAGS")
        string(REPLACE " " ";" value "${value}")
        list(APPEND JUCER_COMPILER_FLAGS ${value})
        set(JUCER_COMPILER_FLAGS ${JUCER_COMPILER_FLAGS} PARENT_SCOPE)

      elseif(tag STREQUAL "PREBUILD_SHELL_SCRIPT")
        set(script_content "${value}")
        configure_file("${Reprojucer_templates_DIR}/script.sh" "prebuild.sh" @ONLY)
        set(JUCER_PREBUILD_SHELL_SCRIPT
          "${CMAKE_CURRENT_BINARY_DIR}/prebuild.sh" PARENT_SCOPE
        )

      elseif(tag STREQUAL "POSTBUILD_SHELL_SCRIPT")
        set(script_content "${value}")
        configure_file("${Reprojucer_templates_DIR}/script.sh" "postbuild.sh" @ONLY)
        set(JUCER_POSTBUILD_SHELL_SCRIPT
          "${CMAKE_CURRENT_BINARY_DIR}/postbuild.sh" PARENT_SCOPE
        )

      endif()

      unset(tag)
    endif()
  endforeach()

endfunction()


function(jucer_export_target_configuration exporter NAME_TAG configuration_name)

  if(NOT "${exporter}" IN_LIST Reprojucer_supported_exporters)
    message(FATAL_ERROR "Unsupported exporter: ${exporter}\n"
      "Supported exporters: ${Reprojucer_supported_exporters}"
    )
  endif()

  if(NOT "${exporter}" IN_LIST JUCER_EXPORT_TARGETS)
    message(FATAL_ERROR "You must call jucer_export_target(\"${exporter}\") before "
      "calling jucer_export_target_configuration(\"${exporter}\")."
    )
  endif()

  list(FIND Reprojucer_supported_exporters "${exporter}" exporter_index)
  list(GET Reprojucer_supported_exporters_conditions ${exporter_index} condition)
  if(NOT ${condition})
    return()
  endif()

  list(APPEND JUCER_PROJECT_CONFIGURATIONS ${configuration_name})
  set(JUCER_PROJECT_CONFIGURATIONS ${JUCER_PROJECT_CONFIGURATIONS} PARENT_SCOPE)

  set(configuration_settings_tags
    "HEADER_SEARCH_PATHS"
    "PREPROCESSOR_DEFINITIONS"
  )

  if(exporter STREQUAL "Xcode (MacOSX)")
    list(APPEND configuration_settings_tags
      "OSX_BASE_SDK_VERSION"
      "OSX_DEPLOYMENT_TARGET"
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

      if(tag STREQUAL "HEADER_SEARCH_PATHS")
        string(REPLACE "\\" "/" value "${value}")
        string(REPLACE "\n" ";" value "${value}")
        foreach(path ${value})
          __abs_path_based_on_target_project_folder("${path}" path)
          list(APPEND include_directories "${path}")
        endforeach()
        list(APPEND JUCER_INCLUDE_DIRECTORIES
          $<$<CONFIG:${configuration_name}>:${include_directories}>
        )
        set(JUCER_INCLUDE_DIRECTORIES ${JUCER_INCLUDE_DIRECTORIES} PARENT_SCOPE)

      elseif(tag STREQUAL "PREPROCESSOR_DEFINITIONS")
        string(REPLACE "\n" ";" value "${value}")
        list(APPEND JUCER_PREPROCESSOR_DEFINITIONS
          $<$<CONFIG:${configuration_name}>:${value}>
        )
        set(JUCER_PREPROCESSOR_DEFINITIONS ${JUCER_PREPROCESSOR_DEFINITIONS} PARENT_SCOPE)

      elseif(tag STREQUAL "OSX_BASE_SDK_VERSION")
        if(value MATCHES "10\\.([5-9]|10|11|12) SDK")
          if(CMAKE_GENERATOR STREQUAL "Xcode")
            set(CMAKE_OSX_SYSROOT "macosx10.${CMAKE_MATCH_1}" PARENT_SCOPE)
          else()
            execute_process(
              COMMAND "xcrun" "--sdk" "macosx10.${CMAKE_MATCH_1}" "--show-sdk-path"
              OUTPUT_VARIABLE sysroot
              OUTPUT_STRIP_TRAILING_WHITESPACE
            )
            if(NOT IS_DIRECTORY "${sysroot}")
              message(WARNING
                "Running `xcrun --sdk macosx10.${CMAKE_MATCH_1} --show-sdk-path` "
                "didn't output a valid directory."
              )
            else()
              set(CMAKE_OSX_SYSROOT ${sysroot} PARENT_SCOPE)
            endif()
          endif()
        elseif(NOT value STREQUAL "Use Default")
          message(FATAL_ERROR
            "Unsupported value for OSX_BASE_SDK_VERSION: \"${value}\"\n"
          )
        endif()

      elseif(tag STREQUAL "OSX_DEPLOYMENT_TARGET")
        if(value MATCHES "10\\.([5-9]|10|11|12)")
          set(CMAKE_OSX_DEPLOYMENT_TARGET "10.${CMAKE_MATCH_1}" PARENT_SCOPE)
        elseif(NOT value STREQUAL "Use Default")
          message(FATAL_ERROR
            "Unsupported value for OSX_DEPLOYMENT_TARGET: \"${value}\"\n"
          )
        endif()

      endif()

      unset(tag)
    endif()
  endforeach()

endfunction()


function(jucer_project_end)

  foreach(exporter ${Reprojucer_supported_exporters})
    list(FIND Reprojucer_supported_exporters "${exporter}" exporter_index)
    list(GET Reprojucer_supported_exporters_conditions ${exporter_index} condition)
    if(${condition} AND NOT "${exporter}" IN_LIST JUCER_EXPORT_TARGETS)
      message(WARNING "You might want to call jucer_export_target(\"${exporter}\").")
    endif()
  endforeach()

  if(DEFINED JUCER_PROJECT_CONFIGURATIONS)
    set(CMAKE_CONFIGURATION_TYPES ${JUCER_PROJECT_CONFIGURATIONS} PARENT_SCOPE)
  endif()

  project(${JUCER_PROJECT_NAME})

  string(TOUPPER "${JUCER_PROJECT_ID}" upper_project_id)
  __generate_AppConfig_header("${upper_project_id}")
  __generate_JuceHeader_header("${upper_project_id}")

  if(WIN32)
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

  string(REGEX REPLACE "[^A-Za-z0-9_.+-]" "_" target_name "${JUCER_PROJECT_NAME}")

  set(all_sources
    ${JUCER_PROJECT_SOURCES}
    ${JUCER_PROJECT_RESOURCES}
    ${JUCER_PROJECT_XCODE_RESOURCES}
    "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode/AppConfig.h"
    "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode/JuceHeader.h"
    ${JUCER_PROJECT_BROWSABLE_FILES}
  )

  set_source_files_properties(${JUCER_PROJECT_XCODE_RESOURCES}
    PROPERTIES MACOSX_PACKAGE_LOCATION "Resources"
  )

  if(JUCER_PROJECT_TYPE STREQUAL "Console Application")
    add_executable(${target_name} ${all_sources})
    __set_common_target_properties(${target_name})
    __link_osx_frameworks(${target_name} ${JUCER_PROJECT_OSX_FRAMEWORKS})

  elseif(JUCER_PROJECT_TYPE STREQUAL "GUI Application")
    add_executable(${target_name} ${all_sources})
    set_target_properties(${target_name} PROPERTIES MACOSX_BUNDLE TRUE)
    __generate_plist_file(${target_name} "App" "APPL" "????" "")
    set_target_properties(${target_name} PROPERTIES WIN32_EXECUTABLE TRUE)
    __set_common_target_properties(${target_name})
    __link_osx_frameworks(${target_name} ${JUCER_PROJECT_OSX_FRAMEWORKS})

  elseif(JUCER_PROJECT_TYPE STREQUAL "Static Library")
    add_library(${target_name} STATIC ${all_sources})
    __set_common_target_properties(${target_name})

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

      add_library(${target_name}_Shared_Code STATIC
        ${SharedCode_sources}
        ${JUCER_PROJECT_RESOURCES}
        ${JUCER_PROJECT_XCODE_RESOURCES}
        "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode/AppConfig.h"
        "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode/JuceHeader.h"
        ${JUCER_PROJECT_BROWSABLE_FILES}
      )
      __set_common_target_properties(${target_name}_Shared_Code)
      target_compile_definitions(${target_name}_Shared_Code PRIVATE "JUCE_SHARED_CODE=1")
      __set_JucePlugin_Build_defines(${target_name}_Shared_Code "SharedCodeTarget")

      if(JUCER_BUILD_VST)
        set(full_target_name ${target_name}_VST)
        add_library(${full_target_name} MODULE
          ${VST_sources}
          ${JUCER_PROJECT_XCODE_RESOURCES}
        )
        target_link_libraries(${full_target_name} ${target_name}_Shared_Code)
        __generate_plist_file(${full_target_name} "VST" "BNDL" "????" "")
        __set_bundle_properties(${full_target_name} "vst")
        __set_common_target_properties(${full_target_name})
        __set_JucePlugin_Build_defines(${full_target_name} "VSTPlugIn")
        __link_osx_frameworks(${target_name}_VST ${JUCER_PROJECT_OSX_FRAMEWORKS})
      endif()

      if(JUCER_BUILD_AUDIOUNIT)
        set(full_target_name ${target_name}_AU)
        add_library(${full_target_name} MODULE
          ${AudioUnit_sources}
          ${JUCER_PROJECT_XCODE_RESOURCES}
        )
        target_link_libraries(${full_target_name} ${target_name}_Shared_Code)

        if(NOT DEFINED JUCER_PLUGIN_AU_MAIN_TYPE)
          if(JUCER_MIDI_EFFECT_PLUGIN)
            set(au_main_type_code "aumi")
          elseif(JUCER_PLUGIN_IS_A_SYNTH)
            set(au_main_type_code "aumu")
          elseif(JUCER_PLUGIN_MIDI_INPUT)
            set(au_main_type_code "aumf")
          else()
            set(au_main_type_code "aufx")
          endif()
        else()
          set(au_main_type_code "${JUCER_PLUGIN_AU_MAIN_TYPE}")
        endif()

        set(audio_components_entries
          "<key>AudioComponents</key>
          <array>
            <dict>
              <key>description</key>
              <string>${JUCER_PLUGIN_DESCRIPTION}</string>
              <key>factoryFunction</key>
              <string>${JUCER_PLUGIN_AU_EXPORT_PREFIX}Factory</string>
              <key>manufacturer</key>
              <string>${JUCER_PLUGIN_MANUFACTURER_CODE}</string>
              <key>name</key>
              <string>${JUCER_PLUGIN_MANUFACTURER}: ${JUCER_PLUGIN_NAME}</string>
              <key>subtype</key>
              <string>${JUCER_PLUGIN_CODE}</string>
              <key>type</key>
              <string>${au_main_type_code}</string>
              <key>version</key>
              <integer>${JUCER_PROJECT_VERSION_AS_HEX}</integer>
            </dict>
          </array>"
        )

        __generate_plist_file(${full_target_name} "AU" "BNDL" "????" "${audio_components_entries}")
        __set_bundle_properties(${full_target_name} "component")
        __set_common_target_properties(${full_target_name})
        __set_JucePlugin_Build_defines(${full_target_name} "AudioUnitPlugIn")
        set(au_plugin_osx_frameworks
          ${JUCER_PROJECT_OSX_FRAMEWORKS} "AudioUnit" "CoreAudioKit"
        )
        __link_osx_frameworks(${target_name}_AU ${au_plugin_osx_frameworks})
      endif()
    else()
      add_library(${target_name} MODULE ${all_sources})
      __set_common_target_properties(${target_name})
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


function(__abs_path_based_on_target_project_folder in_path out_path)

  if(NOT IS_ABSOLUTE "${in_path}" AND NOT DEFINED JUCER_TARGET_PROJECT_FOLDER)
    message(FATAL_ERROR "The path \"${in_path}\" must be absolute, unless you give "
      "TARGET_PROJECT_FOLDER when calling jucer_export_target()."
    )
  endif()

  get_filename_component(in_path "${in_path}" ABSOLUTE
    BASE_DIR "${JUCER_TARGET_PROJECT_FOLDER}"
  )
  set(${out_path} ${in_path} PARENT_SCOPE)

endfunction()


function(__generate_AppConfig_header project_id)

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

  if(JUCER_PROJECT_TYPE STREQUAL "Audio Plug-in")
    set(is_standalone_application 0)

    # See ProjectSaver::writePluginCharacteristicsFile()
    # in JUCE/extras/Projucer/Source/Project Saving/jucer_ProjectSaver.cpp

    __bool_to_int("${JUCER_BUILD_VST}" Build_VST_value)
    list(APPEND plugin_settings "Build_VST" "${Build_VST_value}")

    __bool_to_int("${JUCER_BUILD_AUDIOUNIT}" Build_AU_value)
    list(APPEND plugin_settings "Build_AU" "${Build_AU_value}")

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

  configure_file("${Reprojucer_templates_DIR}/AppConfig.h" "JuceLibraryCode/AppConfig.h")

endfunction()


function(__generate_JuceHeader_header project_id)

  list(LENGTH JUCER_PROJECT_RESOURCES resources_count)
  if(resources_count GREATER 0)
    list(REMOVE_DUPLICATES JUCER_PROJECT_MODULES_FOLDERS)
    foreach(modules_folder ${JUCER_PROJECT_MODULES_FOLDERS})
      get_filename_component(jucer_file
        "${modules_folder}/../extras/Projucer/Projucer.jucer" ABSOLUTE
      )
      if(EXISTS "${jucer_file}")
        set(Projucer_jucer_FILE "${jucer_file}")
        break()
      endif()
    endforeach()
    if(NOT DEFINED Projucer_jucer_FILE)
      message(FATAL_ERROR "Could not find ../extras/Projucer/Projucer.jucer from "
        "modules folders: ${JUCER_PROJECT_MODULES_FOLDERS}"
      )
    endif()
    message(STATUS "Building BinaryDataBuilder for ${JUCER_PROJECT_NAME}")
    try_compile(BinaryDataBuilder
      "${Reprojucer.cmake_DIR}/BinaryDataBuilder/_build/${CMAKE_GENERATOR}"
      "${Reprojucer.cmake_DIR}/BinaryDataBuilder"
      BinaryDataBuilder install
      CMAKE_FLAGS
      "-DProjucer_jucer_FILE=${Projucer_jucer_FILE}"
      "-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}"
    )
    if(NOT BinaryDataBuilder)
      message(FATAL_ERROR "Failed to build BinaryDataBuilder")
    endif()
    message(STATUS "BinaryDataBuilder has been successfully built")

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
      ${size_limit_in_bytes}
      "${JUCER_BINARYDATA_NAMESPACE}"
    )
    foreach(resource_path ${JUCER_PROJECT_RESOURCES})
      get_filename_component(resource_abs_path "${resource_path}" ABSOLUTE)
      list(APPEND BinaryDataBuilder_args "${resource_abs_path}")
    endforeach()
    execute_process(
      COMMAND "${CMAKE_CURRENT_BINARY_DIR}/BinaryDataBuilder/BinaryDataBuilder"
      ${BinaryDataBuilder_args}
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
    set(JUCER_PROJECT_SOURCES ${JUCER_PROJECT_SOURCES} PARENT_SCOPE)
    set(binary_data_include "#include \"BinaryData.h\"")
  endif()

  foreach(module_name ${JUCER_PROJECT_MODULES})
    string(APPEND modules_includes "#include <${module_name}/${module_name}.h>\n")
  endforeach()

  configure_file("${Reprojucer_templates_DIR}/JuceHeader.h"
    "JuceLibraryCode/JuceHeader.h"
  )

endfunction()


function(__set_common_target_properties target_name)

  set_target_properties(${target_name} PROPERTIES OUTPUT_NAME "${JUCER_PROJECT_NAME}")

  target_include_directories(${target_name} PRIVATE
    "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode"
    ${JUCER_PROJECT_MODULES_FOLDERS}
    ${JUCER_INCLUDE_DIRECTORIES}
  )

  if(JUCER_FLAG_JUCE_PLUGINHOST_VST3)
    if(NOT DEFINED JUCER_VST3_SDK_FOLDER)
      message(FATAL_ERROR "JUCER_VST3_SDK_FOLDER must be defined. Give VST3_SDK_FOLDER "
        "when calling jucer_export_target()."
      )
    endif()
    if(NOT IS_DIRECTORY "${JUCER_VST3_SDK_FOLDER}")
      message(WARNING
        "JUCER_VST3_SDK_FOLDER: no such directory \"${JUCER_VST3_SDK_FOLDER}\""
      )
    endif()
    target_include_directories(${target_name} PRIVATE "${JUCER_VST3_SDK_FOLDER}")
  endif()

  target_compile_definitions(${target_name} PRIVATE ${JUCER_PREPROCESSOR_DEFINITIONS})
  target_compile_options(${target_name} PRIVATE ${JUCER_COMPILER_FLAGS})

  if(APPLE)
    set_target_properties(${target_name} PROPERTIES CXX_STANDARD 11)
    target_compile_definitions(${target_name} PRIVATE
      $<$<CONFIG:Debug>:_DEBUG=1>
      $<$<CONFIG:Debug>:DEBUG=1>
      $<$<NOT:$<CONFIG:Debug>>:_NDEBUG=1>
      $<$<NOT:$<CONFIG:Debug>>:NDEBUG=1>
    )

    if(DEFINED JUCER_PREBUILD_SHELL_SCRIPT)
      if(NOT DEFINED JUCER_TARGET_PROJECT_FOLDER)
        message(FATAL_ERROR "JUCER_TARGET_PROJECT_FOLDER must be defined. Give "
          "TARGET_PROJECT_FOLDER when calling jucer_export_target(\"Xcode (MacOSX)\")."
        )
      endif()
      if(NOT IS_DIRECTORY "${JUCER_TARGET_PROJECT_FOLDER}")
        file(MAKE_DIRECTORY "${JUCER_TARGET_PROJECT_FOLDER}")
      endif()
      add_custom_command(TARGET ${target_name} PRE_BUILD
        COMMAND "/bin/sh" ARGS "${JUCER_PREBUILD_SHELL_SCRIPT}"
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
      add_custom_command(TARGET ${target_name} POST_BUILD
        COMMAND "/bin/sh" ARGS "${JUCER_POSTBUILD_SHELL_SCRIPT}"
        WORKING_DIRECTORY "${JUCER_TARGET_PROJECT_FOLDER}"
      )
    endif()
  endif()

endfunction()


function(__generate_plist_file target_name plist_suffix package_type bundle_signature extra_plist_entries)

  set(plist_filename "Info-${plist_suffix}.plist")
  if(CMAKE_GENERATOR STREQUAL "Xcode")
    configure_file("${Reprojucer_templates_DIR}/Info-Xcode.plist"
      "${plist_filename}" @ONLY
    )
    set_target_properties(${target_name} PROPERTIES
      XCODE_ATTRIBUTE_INFOPLIST_FILE "${CMAKE_CURRENT_BINARY_DIR}/${plist_filename}"
      XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "${JUCER_BUNDLE_IDENTIFIER}"
    )
  else()
    configure_file("${Reprojucer_templates_DIR}/Info.plist" "${plist_filename}" @ONLY)
    set_target_properties(${target_name} PROPERTIES
      MACOSX_BUNDLE_BUNDLE_NAME "${JUCER_PROJECT_NAME}"
      MACOSX_BUNDLE_GUI_IDENTIFIER "${JUCER_BUNDLE_IDENTIFIER}"
      MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_BINARY_DIR}/${plist_filename}"
    )
  endif()

endfunction()


function(__set_bundle_properties target_name extension)

  set_target_properties(${target_name} PROPERTIES
    BUNDLE TRUE
    BUNDLE_EXTENSION "${extension}"
    XCODE_ATTRIBUTE_WRAPPER_EXTENSION "${extension}"
  )

endfunction()


function(__set_JucePlugin_Build_defines target_name target_type)

  # See XCodeProjectExporter::Target::getTargetSettings()
  # in JUCE/extras/Projucer/Source/Project Saving/jucer_ProjectExport_XCode.h
  set(plugin_types     VST VST3 AudioUnit AudioUnitv3  RTAS AAX Standalone)
  set(setting_suffixes VST VST3 AUDIOUNIT AUDIOUNIT_V3 RTAS AAX STANDALONE)
  set(define_suffixes  VST VST3 AU        AUv3         RTAS AAX Standalone)

  foreach(index RANGE 6)
    list(GET setting_suffixes ${index} setting_suffix)
    list(GET plugin_types ${index} plugin_type)
    list(GET define_suffixes ${index} define_suffix)

    if(JUCER_BUILD_${setting_suffix} AND (target_type STREQUAL "SharedCodeTarget"
        OR target_type STREQUAL "${plugin_type}PlugIn"))
      target_compile_definitions(${target_name} PRIVATE
        "JucePlugin_Build_${define_suffix}=1"
      )
    else()
      target_compile_definitions(${target_name} PRIVATE
        "JucePlugin_Build_${define_suffix}=0"
      )
    endif()
  endforeach()

endfunction()


function(__link_osx_frameworks target_name)

  set(osx_frameworks ${ARGN})

  if(APPLE)
    if(JUCER_FLAG_JUCE_PLUGINHOST_AU)
      list(APPEND osx_frameworks "AudioUnit" "CoreAudioKit")
    endif()
    list(REMOVE_DUPLICATES osx_frameworks)
    list(SORT osx_frameworks)
    foreach(framework_name ${osx_frameworks})
      find_library(${framework_name}_framework ${framework_name})
      target_link_libraries(${target_name} "${${framework_name}_framework}")
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


function(__version_to_hex version out_hex_value)

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
