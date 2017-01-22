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

  list(FIND ARGN "PROJECT_TYPE" project_type_tag_index)
  if(project_type_tag_index EQUAL -1)
    message(FATAL_ERROR "Missing PROJECT_TYPE argument")
  endif()

  set(project_setting_tags "PROJECT_VERSION" "COMPANY_NAME" "COMPANY_WEBSITE"
    "COMPANY_EMAIL" "PROJECT_TYPE" "BUNDLE_IDENTIFIER" "PREPROCESSOR_DEFINITIONS"
  )
  set(project_type_descs "GUI Application" "Console Application" "Audio Plug-in")

  foreach(element ${ARGN})
    if(NOT DEFINED tag)
      set(tag ${element})
    else()
      set(value ${element})

      list(FIND project_setting_tags "${tag}" project_setting_index)
      if(project_setting_index EQUAL -1)
        message(FATAL_ERROR "Unsupported project setting: ${tag}\n"
          "Supported project settings: ${project_setting_tags}"
        )
      endif()

      if(tag STREQUAL "PROJECT_VERSION")
        __version_to_hex("${value}" hex_value)
        set(JUCER_PROJECT_VERSION_AS_HEX "${hex_value}" PARENT_SCOPE)
      elseif(tag STREQUAL "PROJECT_TYPE")
        list(FIND project_type_descs "${value}" project_type_index)
        if(project_type_index EQUAL -1)
          message(FATAL_ERROR "Unsupported project type: \"${project_type_desc}\"\n"
            "Supported project types: ${project_type_descs}"
          )
        endif()
        set(project_types "guiapp" "consoleapp" "audioplug")
        list(GET project_types ${project_type_index} value)
      endif()

      set(JUCER_${tag} "${value}" PARENT_SCOPE)

      unset(tag)
    endif()
  endforeach()

endfunction()


function(jucer_audio_plugin_settings)

  set(plugin_setting_tags
    "BUILD_VST"
    # "BUILD_VST3" "BUILD_AUDIOUNIT" "BUILD_AUDIOUNIT_V3" "BUILD_RTAS" "BUILD_AAX"
    "PLUGIN_NAME" "PLUGIN_DESCRIPTION" "PLUGIN_MANUFACTURER" "PLUGIN_MANUFACTURER_CODE"
    "PLUGIN_CODE" "PLUGIN_CHANNEL_CONFIGURATIONS" "PLUGIN_IS_A_SYNTH" "PLUGIN_MIDI_INPUT"
    "PLUGIN_MIDI_OUTPUT" "MIDI_EFFECT_PLUGIN" "KEY_FOCUS"
    # "PLUGIN_AU_EXPORT_PREFIX" "PLUGIN_AU_MAIN_TYPE"
    "JUCER_VST_CATEGORY"
    # "PLUGIN_RTAS_CATEGORY" "PLUGIN_AAX_CATEGORY" "PLUGIN_AAX_IDENTIFIER"
  )

  foreach(element ${ARGN})
    if(NOT DEFINED tag)
      set(tag ${element})
    else()
      set(value ${element})

      list(FIND plugin_setting_tags "${tag}" plugin_setting_index)
      if(plugin_setting_index EQUAL -1)
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

  string(REPLACE "/" "\\" source_group_name ${source_group_name})
  source_group(${source_group_name} FILES ${ARGN})

  list(APPEND JUCER_PROJECT_SOURCES ${ARGN})
  set(JUCER_PROJECT_SOURCES ${JUCER_PROJECT_SOURCES} PARENT_SCOPE)

endfunction()


function(jucer_project_resources source_group_name)

  string(REPLACE "/" "\\" source_group_name ${source_group_name})
  source_group(${source_group_name} FILES ${ARGN})

  list(APPEND JUCER_PROJECT_RESOURCES ${ARGN})
  set(JUCER_PROJECT_RESOURCES ${JUCER_PROJECT_RESOURCES} PARENT_SCOPE)

endfunction()


function(jucer_project_module module_name PATH_TAG module_path)

  list(APPEND JUCER_PROJECT_MODULES ${module_name})
  set(JUCER_PROJECT_MODULES ${JUCER_PROJECT_MODULES} PARENT_SCOPE)

  list(APPEND JUCER_PROJECT_INCLUDE_DIRS "${module_path}")
  set(JUCER_PROJECT_INCLUDE_DIRS ${JUCER_PROJECT_INCLUDE_DIRS} PARENT_SCOPE)

  file(GLOB module_src_files
    "${module_path}/${module_name}/*.cpp" "${module_path}/${module_name}/*.mm"
  )

  foreach(src_file ${module_src_files})
    set(to_compile FALSE)

    if(src_file MATCHES "_AAX." OR src_file MATCHES "_AAX_")
      if(JUCER_BUILD_AAX)
        set(to_compile TRUE)
      endif()
    elseif(src_file MATCHES "_AUv3." OR src_file MATCHES "_AUv3_")
      if(JUCER_BUILD_AUDIOUNIT_V3 AND APPLE)
        set(to_compile TRUE)
      endif()
    elseif(src_file MATCHES "_AU." OR src_file MATCHES "_AU_")
      if(JUCER_BUILD_AUDIOUNIT AND APPLE)
        set(to_compile TRUE)
      endif()
    elseif(src_file MATCHES "_RTAS." OR src_file MATCHES "_RTAS_")
      if(JUCER_BUILD_RTAS)
        set(to_compile TRUE)
      endif()
    elseif(src_file MATCHES "_VST2." OR src_file MATCHES "_VST2_")
      if(JUCER_BUILD_VST)
        set(to_compile TRUE)
      endif()
    elseif(src_file MATCHES "_VST3." OR src_file MATCHES "_VST3_")
      if(JUCER_BUILD_VST3)
        set(to_compile TRUE)
      endif()
    elseif(src_file MATCHES "_Standalone." OR src_file MATCHES "_Standalone_")
      if(JUCER_BUILD_AUDIOUNIT_V3 AND APPLE)
        set(to_compile TRUE)
      endif()
    else()
      get_filename_component(src_file_extension "${src_file}" EXT)
      if(src_file_extension STREQUAL ".mm")
        if(APPLE)
          set(to_compile TRUE)
        endif()
      elseif(APPLE)
        string(REGEX REPLACE "${src_file_extension}$" ".mm" objcxx_src_file "${src_file}")
        list(FIND module_src_files "${objcxx_src_file}" index)
        if(index EQUAL -1)
          set(to_compile TRUE)
        endif()
      else()
        set(to_compile TRUE)
      endif()
    endif()

    if(to_compile)
      get_filename_component(src_file_basename "${src_file}" NAME)
      configure_file("${JUCE.cmake_ROOT}/cmake/ModuleWrapper.cpp"
        "JuceLibraryCode/${src_file_basename}"
      )
      list(APPEND JUCER_PROJECT_SOURCES
        "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode/${src_file_basename}"
      )
    endif()
  endforeach()

  set(JUCER_PROJECT_SOURCES ${JUCER_PROJECT_SOURCES} PARENT_SCOPE)

  list(APPEND JUCER_CONFIG_FLAGS ${ARGN})
  set(JUCER_CONFIG_FLAGS ${JUCER_CONFIG_FLAGS} PARENT_SCOPE)

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
  foreach(element ${JUCER_CONFIG_FLAGS})
    if(NOT DEFINED flag_name)
      set(flag_name ${element})
    else()
      set(flag_value ${element})

      string(CONCAT config_flags_defines
        "${config_flags_defines}" "#ifndef    ${flag_name}\n"
      )
      if(flag_value)
        string(CONCAT config_flags_defines
          "${config_flags_defines}" " #define   ${flag_name} 1\n"
        )
      else()
        string(CONCAT config_flags_defines
          "${config_flags_defines}" " #define   ${flag_name} 0\n"
        )
      endif()
      string(CONCAT config_flags_defines "${config_flags_defines}" "#endif\n\n")

      if(flag_name STREQUAL "JUCE_PLUGINHOST_AU")
        if(flag_value)
          list(APPEND JUCER_PROJECT_OSX_FRAMEWORKS "AudioUnit" "CoreAudioKit")
        endif()
      endif()

      unset(flag_name)
    endif()
  endforeach()

  if(JUCER_PROJECT_TYPE STREQUAL "audioplug")
    set(plugin_settings "Build_VST" "Build_VST3" "Build_AU" "Build_AUv3" "Build_RTAS"
      "Build_AAX" "Build_STANDALONE" "Name" "Desc" "Manufacturer" "ManufacturerWebsite"
      "ManufacturerEmail" "ManufacturerCode" "PluginCode" "IsSynth" "WantsMidiInput"
      "ProducesMidiOutput" "IsMidiEffect" "EditorRequiresKeyboardFocus" "Version"
      "VersionCode" "VersionString" "VSTUniqueID" "VSTCategory" "AUMainType" "AUSubType"
      "AUExportPrefix" "AUExportPrefixQuoted" "AUManufacturerCode" "CFBundleIdentifier"
      "RTASCategory" "RTASManufacturerCode" "RTASProductId" "RTASDisableBypass"
      "RTASDisableMultiMono" "AAXIdentifier" "AAXManufacturerCode" "AAXProductId"
      "AAXCategory" "AAXDisableBypass" "AAXDisableMultiMono"
    )

    string(CONCAT audio_plugin_settings_defines
      "//==============================================================================\n"
      "// Audio plugin settings..\n\n"
    )

    foreach(setting_name ${plugin_settings})
      __get_plugin_setting_value("${setting_name}" setting_value)

      string(CONCAT audio_plugin_settings_defines "${audio_plugin_settings_defines}"
        "#ifndef  JucePlugin_${setting_name}\n"
      )
      string(CONCAT audio_plugin_settings_defines "${audio_plugin_settings_defines}"
        " #define JucePlugin_${setting_name}  ${setting_value}\n"
      )
      string(CONCAT audio_plugin_settings_defines "${audio_plugin_settings_defines}"
        "#endif\n"
      )
    endforeach()
  endif()
  configure_file("${JUCE.cmake_ROOT}/cmake/AppConfig.h" "JuceLibraryCode/AppConfig.h")

  list(LENGTH JUCER_PROJECT_RESOURCES resources_count)
  if(resources_count GREATER 0)
    message("Building BinaryDataBuilder for ${JUCER_PROJECT_NAME}")
    try_compile(BinaryDataBuilder
      "${JUCE.cmake_ROOT}/cmake/BinaryDataBuilder/_build"
      "${JUCE.cmake_ROOT}/cmake/BinaryDataBuilder"
      BinaryDataBuilder install
      CMAKE_FLAGS "-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}"
    )
    if(NOT BinaryDataBuilder)
      message(FATAL_ERROR "Failed to build BinaryDataBuilder")
    endif()
    message("BinaryDataBuilder has been successfully built")
    list(APPEND BinaryDataBuilder_args "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode/")
    foreach(element ${JUCER_PROJECT_RESOURCES})
      list(APPEND BinaryDataBuilder_args "${CMAKE_CURRENT_LIST_DIR}/${element}")
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
    set(binary_data_include "#include \"BinaryData.h\"")
  else()
    set(binary_data_include "")
  endif()

  foreach(module_name ${JUCER_PROJECT_MODULES})
    string(CONCAT modules_includes
      "${modules_includes}"
      "#include <${module_name}/${module_name}.h>\n"
    )
  endforeach()
  configure_file("${JUCE.cmake_ROOT}/cmake/JuceHeader.h" "JuceLibraryCode/JuceHeader.h")

  source_group("Juce Library Code"
    REGULAR_EXPRESSION "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode/*"
  )

  if(WIN32)
    string(REPLACE "." "," comma_separated_version_number "${JUCER_PROJECT_VERSION}")
    configure_file("${JUCE.cmake_ROOT}/cmake/resources.rc" "resources.rc")
    list(APPEND ${JUCER_PROJECT_SOURCES} "${CMAKE_CURRENT_BINARY_DIR}/resources.rc")
  endif()

  string(REGEX REPLACE "[^A-Za-z0-9_.+-]" "_" target_name "${JUCER_PROJECT_NAME}")

  set(all_sources
    ${JUCER_PROJECT_SOURCES}
    ${JUCER_PROJECT_RESOURCES}
    "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode/AppConfig.h"
    "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode/JuceHeader.h"
    ${JUCER_PROJECT_BROWSABLE_FILES}
  )

  if(JUCER_PROJECT_TYPE STREQUAL "audioplug")
    if(APPLE)
      foreach(src_file ${JUCER_PROJECT_SOURCES})
        if(src_file MATCHES "_AAX." OR src_file MATCHES "_AAX_")
          list(APPEND AAX_sources "${src_file}")
        elseif(src_file MATCHES "_AUv3." OR src_file MATCHES "_AUv3_")
          list(APPEND AUv3_sources "${src_file}")
        elseif(src_file MATCHES "_AU." OR src_file MATCHES "_AU_")
          list(APPEND AU_sources "${src_file}")
        elseif(src_file MATCHES "_RTAS." OR src_file MATCHES "_RTAS_")
          list(APPEND RTAS_sources "${src_file}")
        elseif(src_file MATCHES "_VST2." OR src_file MATCHES "_VST2_")
          list(APPEND VST_sources "${src_file}")
        elseif(src_file MATCHES "_VST3." OR src_file MATCHES "_VST3_")
          list(APPEND VST3_sources "${src_file}")
        elseif(src_file MATCHES "_Standalone." OR src_file MATCHES "_Standalone_")
          list(APPEND Standalone_sources "${src_file}")
        else()
          list(APPEND SharedCode_sources "${src_file}")
        endif()
      endforeach()

      add_library(${target_name}_Shared_Code STATIC
        ${SharedCode_sources}
        ${JUCER_PROJECT_RESOURCES}
        "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode/AppConfig.h"
        "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode/JuceHeader.h"
        ${JUCER_PROJECT_BROWSABLE_FILES}
      )

      if(JUCER_BUILD_VST)
        add_library(${target_name}_VST MODULE ${VST_sources})
        target_link_libraries(${target_name}_VST PRIVATE ${target_name}_Shared_Code)
        set_target_properties(${target_name}_VST PROPERTIES
          BUNDLE TRUE
          BUNDLE_EXTENSION "vst"
        )
      endif()
    else()
      add_library(${target_name} MODULE ${all_sources})
    endif()
  else()
    add_executable(${target_name} ${all_sources})
  endif()

  if(JUCER_PROJECT_TYPE STREQUAL "guiapp")
    set_target_properties(${target_name} PROPERTIES MACOSX_BUNDLE TRUE)
    if(CMAKE_GENERATOR STREQUAL "Xcode")
      configure_file("${JUCE.cmake_ROOT}/cmake/Info-App-Xcode.plist"
        "Info-App.plist" @ONLY
      )
      set_target_properties(${target_name} PROPERTIES
        XCODE_ATTRIBUTE_INFOPLIST_FILE "${CMAKE_CURRENT_BINARY_DIR}/Info-App.plist"
        XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "${JUCER_BUNDLE_IDENTIFIER}"
      )
    else()
      configure_file("${JUCE.cmake_ROOT}/cmake/Info-App.plist" "Info-App.plist" @ONLY)
      set_target_properties(${target_name} PROPERTIES
        MACOSX_BUNDLE_BUNDLE_NAME "${JUCER_PROJECT_NAME}"
        MACOSX_BUNDLE_GUI_IDENTIFIER "${JUCER_BUNDLE_IDENTIFIER}"
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_BINARY_DIR}/Info-App.plist"
      )
    endif()
    set_target_properties(${target_name} PROPERTIES WIN32_EXECUTABLE TRUE)
  endif()

  set_target_properties(${target_name} PROPERTIES OUTPUT_NAME "${JUCER_PROJECT_NAME}")

  set_source_files_properties(
    ${JUCER_PROJECT_BROWSABLE_FILES}
    ${JUCER_PROJECT_RESOURCES}
    PROPERTIES HEADER_FILE_ONLY TRUE
  )

  target_include_directories(${target_name} PRIVATE
    "${CMAKE_CURRENT_BINARY_DIR}/JuceLibraryCode"
    ${JUCER_PROJECT_INCLUDE_DIRS}
  )

  target_compile_definitions(${target_name} PRIVATE ${JUCER_PREPROCESSOR_DEFINITIONS})

  if(APPLE)
    target_compile_options(${target_name} PRIVATE -std=c++11)
    target_compile_definitions(${target_name} PRIVATE
      $<$<CONFIG:Debug>:_DEBUG=1>
      $<$<CONFIG:Debug>:DEBUG=1>
      $<$<NOT:$<CONFIG:Debug>>:_NDEBUG=1>
      $<$<NOT:$<CONFIG:Debug>>:NDEBUG=1>
    )

    list(REMOVE_DUPLICATES JUCER_PROJECT_OSX_FRAMEWORKS)
    foreach(framework_name ${JUCER_PROJECT_OSX_FRAMEWORKS})
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
  else()
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
  endif()

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

  string(STRIP "${value}" value)
  string(SUBSTRING "${value}" 0 4 value)
  set(dec_value 0)
  foreach(index 0 1 2 3)
    string(SUBSTRING "${value}" ${index} 1 ascii_char)
    string(FIND "${all_ascii_chars}" "${ascii_char}" ascii_code)
    if(ascii_code EQUAL -1)
      message(FATAL_ERROR "${value} cannot contain non-ASCII characters")
    endif()
    math(EXPR dec_value "(${dec_value} << 8) | ((${ascii_code} + 1) & 255)")
  endforeach()

  __dec_to_hex("${dec_value}" hex_value)
  set(${out_hex_value} "${hex_value}" PARENT_SCOPE)

endfunction()


function(__get_plugin_setting_value setting_name out_setting_value)

  if(setting_name STREQUAL "Build_VST")
    __bool_to_int("${JUCER_BUILD_VST}" setting_value)

  elseif(setting_name STREQUAL "Build_VST3")
    __bool_to_int("${JUCER_BUILD_VST3}" setting_value)

  elseif(setting_name STREQUAL "Build_AU")
    __bool_to_int("${JUCER_BUILD_AUDIOUNIT}" setting_value)

  elseif(setting_name STREQUAL "Build_AUv3")
    __bool_to_int("${JUCER_BUILD_AUDIOUNIT_V3}" setting_value)

  elseif(setting_name STREQUAL "Build_RTAS")
    __bool_to_int("${JUCER_BUILD_RTAS}" setting_value)

  elseif(setting_name STREQUAL "Build_AAX")
    __bool_to_int("${JUCER_BUILD_AAX}" setting_value)

  elseif(setting_name STREQUAL "Build_STANDALONE")
    __bool_to_int("${JUCER_BUILD_STANDALONE}" setting_value)

  elseif(setting_name STREQUAL "Name")
    set(setting_value "\"${JUCER_PLUGIN_NAME}\"")

  elseif(setting_name STREQUAL "Desc")
    set(setting_value "\"${JUCER_PLUGIN_DESCRIPTION}\"")

  elseif(setting_name STREQUAL "Manufacturer")
    set(setting_value "\"${JUCER_PLUGIN_MANUFACTURER}\"")

  elseif(setting_name STREQUAL "ManufacturerWebsite")
    set(setting_value "\"${JUCER_COMPANY_WEBSITE}\"")

  elseif(setting_name STREQUAL "ManufacturerEmail")
    set(setting_value "\"${JUCER_COMPANY_EMAIL}\"")

  elseif(setting_name STREQUAL "ManufacturerCode")
    __four_chars_to_hex("${JUCER_PLUGIN_MANUFACTURER_CODE}" hex_value)
    set(setting_value "${hex_value} // '${JUCER_PLUGIN_MANUFACTURER_CODE}'")

  elseif(setting_name STREQUAL "PluginCode")
    __four_chars_to_hex("${JUCER_PLUGIN_CODE}" hex_value)
    set(setting_value "${hex_value} // '${JUCER_PLUGIN_CODE}'")

  elseif(setting_name STREQUAL "IsSynth")
    __bool_to_int("${JUCER_PLUGIN_IS_A_SYNTH}" setting_value)

  elseif(setting_name STREQUAL "WantsMidiInput")
    __bool_to_int("${JUCER_PLUGIN_MIDI_INPUT}" setting_value)

  elseif(setting_name STREQUAL "ProducesMidiOutput")
    __bool_to_int("${JUCER_PLUGIN_MIDI_OUTPUT}" setting_value)

  elseif(setting_name STREQUAL "IsMidiEffect")
    __bool_to_int("${JUCER_MIDI_EFFECT_PLUGIN}" setting_value)

  elseif(setting_name STREQUAL "EditorRequiresKeyboardFocus")
    __bool_to_int("${JUCER_KEY_FOCUS}" setting_value)

  elseif(setting_name STREQUAL "Version")
    set(setting_value "${JUCER_PROJECT_VERSION}")

  elseif(setting_name STREQUAL "VersionCode")
    __version_to_hex("${JUCER_PROJECT_VERSION}" hex_value)
    set(setting_value "${hex_value}")

  elseif(setting_name STREQUAL "VersionString")
    set(setting_value "\"${JUCER_PROJECT_VERSION}\"")

  elseif(setting_name STREQUAL "VSTUniqueID")
    set(setting_value "JucePlugin_PluginCode")

  elseif(setting_name STREQUAL "VSTCategory")
    if(NOT DEFINED JUCER_VST_CATEGORY)
      if(JUCER_PLUGIN_IS_A_SYNTH)
        set(setting_value "kPlugCategSynth")
      else()
        set(setting_value "kPlugCategEffect")
      endif()
    else()
      set(setting_value "${JUCER_VST_CATEGORY}")
    endif()

  elseif(setting_name STREQUAL "AUMainType")
    if(NOT DEFINED JUCER_PLUGIN_AU_MAIN_TYPE)
      if(JUCER_MIDI_EFFECT_PLUGIN)
        set(setting_value "'aumi'")
      elseif(JUCER_PLUGIN_IS_A_SYNTH)
        set(setting_value "kAudioUnitType_MusicDevice")
      elseif(JUCER_PLUGIN_MIDI_INPUT)
        set(setting_value "kAudioUnitType_MusicEffect")
      else()
        set(setting_value "kAudioUnitType_Effect")
      endif()
    else()
      set(setting_value "${JUCER_PLUGIN_AU_MAIN_TYPE}")
    endif()

  elseif(setting_name STREQUAL "AUSubType")
    set(setting_value "JucePlugin_PluginCode")

  elseif(setting_name STREQUAL "AUExportPrefix")
    set(setting_value "${JUCER_PLUGIN_AU_EXPORT_PREFIX}")

  elseif(setting_name STREQUAL "AUExportPrefixQuoted")
    set(setting_value "\"${JUCER_PLUGIN_AU_EXPORT_PREFIX}\"")

  elseif(setting_name STREQUAL "AUManufacturerCode")
    set(setting_value "JucePlugin_ManufacturerCode")

  elseif(setting_name STREQUAL "CFBundleIdentifier")
    set(setting_value "${JUCER_BUNDLE_IDENTIFIER}")

  elseif(setting_name STREQUAL "RTASCategory")
    if(JUCER_PLUGIN_IS_A_SYNTH)
      set(setting_value "ePlugInCategory_SWGenerators")
    elseif(NOT DEFINED JUCER_PLUGIN_RTAS_CATEGORY)
      set(setting_value "ePlugInCategory_None")
    else()
      set(setting_value "${JUCER_PLUGIN_RTAS_CATEGORY}")
    endif()

  elseif(setting_name STREQUAL "RTASManufacturerCode")
    set(setting_value "JucePlugin_ManufacturerCode")

  elseif(setting_name STREQUAL "RTASProductId")
    set(setting_value "JucePlugin_PluginCode")

  elseif(setting_name STREQUAL "RTASDisableBypass")
    set(setting_value 0)

  elseif(setting_name STREQUAL "RTASDisableMultiMono")
    set(setting_value 0)

  elseif(setting_name STREQUAL "AAXIdentifier")
    set(setting_value "${JUCER_PLUGIN_AAX_IDENTIFIER}")

  elseif(setting_name STREQUAL "AAXManufacturerCode")
    set(setting_value "JucePlugin_ManufacturerCode")

  elseif(setting_name STREQUAL "AAXProductId")
    set(setting_value "JucePlugin_PluginCode")

  elseif(setting_name STREQUAL "AAXCategory")
    set(setting_value "${JUCER_PLUGIN_AAX_CATEGORY}")

  elseif(setting_name STREQUAL "AAXDisableBypass")
    set(setting_value 0)

  elseif(setting_name STREQUAL "AAXDisableMultiMono")
    set(setting_value 0)

  endif()

  set(${out_setting_value} "${setting_value}" PARENT_SCOPE)

endfunction()
