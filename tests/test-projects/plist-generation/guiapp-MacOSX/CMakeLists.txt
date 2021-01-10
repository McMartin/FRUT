# Copyright (C) 2019, 2021  Alain Martin
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

project("plist-generation-guiapp-MacOSX")


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../../../cmake")
include(Reprojucer)


set(JUCER_PROJECT_NAME "Custom Project Name")
set(JUCER_BUNDLE_IDENTIFIER "com.yourcompany.CustomProjectName")
set(JUCER_PROJECT_VERSION "1.0.0")


add_custom_target(target)


function(test_app_plist_default)
  _FRUT_generate_plist_file(target "App-default" "APPL" "????")
endfunction()

function(test_app_plist_pre_5_2_0)
  set(JUCER_VERSION "5.0.0")
  _FRUT_generate_plist_file(target "App-pre-5.2.0" "APPL" "????")
endfunction()

function(test_app_plist_version)
  set(JUCER_PROJECT_VERSION "1.3.5")
  _FRUT_generate_plist_file(target "App-Version" "APPL" "????")
endfunction()

function(test_app_plist_copyright)
  set(JUCER_COMPANY_COPYRIGHT "Custom Company Copyright")
  _FRUT_generate_plist_file(target "App-Copyright" "APPL" "????")
endfunction()

function(test_app_plist_copyright_pre_5_2_0)
  set(JUCER_VERSION "4.3.1")
  set(JUCER_COMPANY_NAME "Custom Company Name")
  _FRUT_generate_plist_file(target "App-Copyright-pre-5.2.0" "APPL" "????")
endfunction()

function(test_app_plist_bundle_identifier)
  set(JUCER_BUNDLE_IDENTIFIER "com.custom.bundleIdentifier")
  _FRUT_generate_plist_file(target "App-BundleIdentifier" "APPL" "????")
endfunction()

function(test_app_plist_icon_file)
  set(JUCER_ICON_FILE "${CMAKE_CURRENT_BINARY_DIR}/Icon.icns")
  _FRUT_generate_plist_file(target "App-IconFile" "APPL" "????")
endfunction()

function(test_app_plist_microphone_access_default)
  set(JUCER_MICROPHONE_ACCESS ON)
  _FRUT_generate_plist_file(target "App-MicrophoneUsage-default" "APPL" "????")
endfunction()

function(test_app_plist_microphone_access_custom)
  set(JUCER_MICROPHONE_ACCESS ON)
  set(JUCER_MICROPHONE_ACCESS_TEXT "Custom Microphone Access Text")
  _FRUT_generate_plist_file(target "App-MicrophoneUsage-custom" "APPL" "????")
endfunction()

function(test_app_plist_camera_access_default)
  set(JUCER_CAMERA_ACCESS ON)
  _FRUT_generate_plist_file(target "App-CameraUsage-default" "APPL" "????")
endfunction()

function(test_app_plist_camera_access_custom)
  set(JUCER_CAMERA_ACCESS ON)
  set(JUCER_CAMERA_ACCESS_TEXT "Custom Camera Access Text")
  _FRUT_generate_plist_file(target "App-CameraUsage-custom" "APPL" "????")
endfunction()

function(test_app_plist_bluetooth_access_default)
  set(JUCER_BLUETOOTH_ACCESS ON)
  _FRUT_generate_plist_file(target "App-BluetoothUsage-default" "APPL" "????")
endfunction()

function(test_app_plist_bluetooth_access_custom)
  set(JUCER_BLUETOOTH_ACCESS ON)
  set(JUCER_BLUETOOTH_ACCESS_TEXT "Custom Bluetooth Access Text")
  _FRUT_generate_plist_file(target "App-BluetoothUsage-custom" "APPL" "????")
endfunction()

function(test_app_plist_send_apple_events_default)
  set(JUCER_SEND_APPLE_EVENTS ON)
  _FRUT_generate_plist_file(target "App-SendAppleEvents-default" "APPL" "????")
endfunction()

function(test_app_plist_send_apple_events_custom)
  set(JUCER_SEND_APPLE_EVENTS ON)
  set(JUCER_SEND_APPLE_EVENTS_TEXT "Custom Send Apple Events Text")
  _FRUT_generate_plist_file(target "App-SendAppleEvents-custom" "APPL" "????")
endfunction()

function(test_app_plist_document_types)
  set(JUCER_DOCUMENT_FILE_EXTENSIONS ".aiff" "flac" ".wav")
  _FRUT_generate_plist_file(target "App-DocumentTypes" "APPL" "????")
endfunction()

function(test_app_plist_exporter_bundle_identifier)
  set(JUCER_BUNDLE_IDENTIFIER "com.custom.bundleIdentifier")
  set(JUCER_EXPORTER_BUNDLE_IDENTIFIER "com.custom.exporterBundleIdentifier")
  _FRUT_generate_plist_file(target "App-ExporterBundleIdentifier" "APPL" "????")
endfunction()


test_app_plist_bluetooth_access_custom()
test_app_plist_bluetooth_access_default()
test_app_plist_bundle_identifier()
test_app_plist_camera_access_custom()
test_app_plist_camera_access_default()
test_app_plist_copyright()
test_app_plist_copyright_pre_5_2_0()
test_app_plist_default()
test_app_plist_document_types()
test_app_plist_exporter_bundle_identifier()
test_app_plist_icon_file()
test_app_plist_microphone_access_custom()
test_app_plist_microphone_access_default()
test_app_plist_pre_5_2_0()
test_app_plist_send_apple_events_custom()
test_app_plist_send_apple_events_default()
test_app_plist_version()
