# Copyright (C) 2019  Alain Martin
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

cmake_minimum_required(VERSION 3.4)

project("plist-generation-plugin-MacOSX")


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../../../cmake")
include(Reprojucer)


set(JUCER_PROJECT_NAME "Custom Project Name")
set(JUCER_BUNDLE_IDENTIFIER "com.yourcompany.CustomProjectName")
set(JUCER_PROJECT_VERSION "1.0.0")

set(JUCER_PLUGIN_NAME "Custom Project Name")
set(JUCER_PLUGIN_DESCRIPTION "Custom Project Name")
set(JUCER_PLUGIN_MANUFACTURER "yourcompany")
set(JUCER_PLUGIN_MANUFACTURER_CODE "Manu")
set(JUCER_PLUGIN_CODE "Hn9v")
set(JUCER_PLUGIN_AU_EXPORT_PREFIX "CustomProjectNameAU")


add_custom_target(target_VST3)

function(test_VST3_plist_default)
  _FRUT_generate_plist_file(target_VST3 "VST3-default" "BNDL" "????")
endfunction()

function(test_VST3_plist_pre_5_2_0)
  set(JUCER_VERSION "5.0.1")
  _FRUT_generate_plist_file(target_VST3 "VST3-pre-5.2.0" "BNDL" "????")
endfunction()


add_custom_target(target_AU)

function(test_AU_plist_default)
  _FRUT_generate_plist_file(target_AU "AU-default" "BNDL" "????")
endfunction()

function(test_AU_plist_factoryFunction)
  set(JUCER_PLUGIN_AU_EXPORT_PREFIX "CustomPluginAUExportPrefix")
  _FRUT_generate_plist_file(target_AU "AU-factoryFunction" "BNDL" "????")
endfunction()

function(test_AU_plist_sandboxSafe)
  set(JUCER_PLUGIN_AU_IS_SANDBOX_SAFE TRUE)
  _FRUT_generate_plist_file(target_AU "AU-sandboxSafe" "BNDL" "????")
endfunction()

function(test_AU_plist_pre_5_4_0)
  set(JUCER_VERSION "5.3.2")
  _FRUT_generate_plist_file(target_AU "AU-pre-5.4.0" "BNDL" "????")
endfunction()


add_custom_target(target_AUv3_AppExtension)

function(test_AUv3_AppExtension_plist_default)
  _FRUT_generate_plist_file(target_AUv3_AppExtension "AUv3_AppExtension-default" "XPC!" "????")
endfunction()

function(test_AUv3_AppExtension_plist_Synth)
  set(JUCER_PLUGIN_IS_A_SYNTH ON)
  _FRUT_generate_plist_file(target_AUv3_AppExtension "AUv3_AppExtension-Synth" "XPC!" "????")
endfunction()

function(test_AUv3_AppExtension_plist_factoryFunction)
  set(JUCER_PLUGIN_AU_EXPORT_PREFIX "CustomPluginAUExportPrefix")
  _FRUT_generate_plist_file(target_AUv3_AppExtension "AUv3_AppExtension-factoryFunction" "XPC!" "????")
endfunction()

function(test_AUv3_AppExtension_plist_exporter_bundle_identifier)
  set(JUCER_BUNDLE_IDENTIFIER "com.custom.bundleIdentifier")
  set(JUCER_EXPORTER_BUNDLE_IDENTIFIER "com.custom.exporterBundleIdentifier")
  _FRUT_generate_plist_file(target_AUv3_AppExtension "AUv3_AppExtension-ExporterBundleIdentifier" "XPC!" "????")
endfunction()

function(test_AUv3_AppExtension_plist_pre_5_0_0)
  set(JUCER_VERSION "4.2.4")
  _FRUT_generate_plist_file(target_AUv3_AppExtension "AUv3_AppExtension-pre-5.0.0" "XPC!" "????")
endfunction()


add_custom_target(target_AUv3_Standalone)

function(test_AUv3_Standalone_plist_pre_5_0_0)
  set(JUCER_VERSION "4.3.1")
  _FRUT_generate_plist_file(target_AUv3_Standalone "AUv3_Standalone-pre-5.0.0" "APPL" "????")
endfunction()


add_custom_target(target_RTAS)

function(test_RTAS_plist_default)
  _FRUT_generate_plist_file(target_RTAS "RTAS-default" "TDMw" "PTul")
endfunction()


add_custom_target(target_AAX)

function(test_AAX_plist_default)
  _FRUT_generate_plist_file(target_AAX "AAX-default" "TDMw" "PTul")
endfunction()


add_custom_target(target_StandalonePlugin)

function(test_Standalone_Plugin_plist_default)
  _FRUT_generate_plist_file(target_StandalonePlugin "Standalone_Plugin-default" "APPL" "????")
endfunction()


add_custom_target(target_Unity_Plugin)

function(test_Unity_Plugin_plist_default)
  _FRUT_generate_plist_file(target_Unity_Plugin "Unity_Plugin-default" "BNDL" "????")
endfunction()


add_custom_target(target_VST)

function(test_VST_plist_default)
  _FRUT_generate_plist_file(target_VST "VST-default" "BNDL" "????")
endfunction()


test_AAX_plist_default()
test_AU_plist_default()
test_AU_plist_factoryFunction()
test_AU_plist_pre_5_4_0()
test_AU_plist_sandboxSafe()
test_AUv3_AppExtension_plist_Synth()
test_AUv3_AppExtension_plist_default()
test_AUv3_AppExtension_plist_factoryFunction()
test_AUv3_AppExtension_plist_pre_5_0_0()
test_AUv3_Standalone_plist_pre_5_0_0()
test_RTAS_plist_default()
test_Standalone_Plugin_plist_default()
test_Unity_Plugin_plist_default()
test_VST3_plist_default()
test_VST3_plist_pre_5_2_0()
test_VST_plist_default()
