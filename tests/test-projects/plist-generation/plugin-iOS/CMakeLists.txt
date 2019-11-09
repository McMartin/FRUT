# Copyright (c) 2019 Alain Martin
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

project("plist-generation-plugin-iOS")


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../../../cmake")
include(Reprojucer)


set(JUCER_PROJECT_NAME "Custom Project Name")
set(JUCER_BUNDLE_IDENTIFIER "com.yourcompany.CustomProjectName")
set(JUCER_PROJECT_VERSION "1.0.0")

set(JUCER_PLUGIN_NAME "Custom Project Name")
set(JUCER_PLUGIN_DESCRIPTION "Custom Project Name")
set(JUCER_PLUGIN_MANUFACTURER "yourcompany")
set(JUCER_PLUGIN_MANUFACTURER_CODE "Manu")
set(JUCER_PLUGIN_CODE "Ypdf")
set(JUCER_PLUGIN_AU_EXPORT_PREFIX "CustomProjectNameAU")


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


add_custom_target(target_StandalonePlugin)

function(test_Standalone_Plugin_plist_default)
  _FRUT_generate_plist_file(target_StandalonePlugin "Standalone_Plugin-default" "APPL" "????")
endfunction()


test_AUv3_AppExtension_plist_Synth()
test_AUv3_AppExtension_plist_default()
test_AUv3_AppExtension_plist_exporter_bundle_identifier()
test_AUv3_AppExtension_plist_factoryFunction()
test_Standalone_Plugin_plist_default()
