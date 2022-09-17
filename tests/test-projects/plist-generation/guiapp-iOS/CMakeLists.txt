# Copyright (C) 2019, 2021-2022  Alain Martin
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

project("plist-generation-iOS-MacOSX")


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../../../cmake")
include(Reprojucer)


set(JUCER_PROJECT_NAME "Custom Project Name")
set(JUCER_BUNDLE_IDENTIFIER "com.yourcompany.CustomProjectName")
set(JUCER_PROJECT_VERSION "1.0.0")


add_custom_target(target)


function(test_app_plist_default)
  _FRUT_generate_plist_file(target "App-default" "APPL" "????")
endfunction()

function(test_app_plist_pre_6_0_8)
  set(JUCER_VERSION "6.0.5")
  _FRUT_generate_plist_file(target "App-pre-6.0.8" "APPL" "????")
endfunction()

function(test_app_plist_version)
  set(JUCER_PROJECT_VERSION "1.3.5")
  _FRUT_generate_plist_file(target "App-Version" "APPL" "????")
endfunction()

function(test_app_plist_copyright)
  set(JUCER_COMPANY_COPYRIGHT "Custom Company Copyright")
  _FRUT_generate_plist_file(target "App-Copyright" "APPL" "????")
endfunction()

function(test_app_plist_bundle_identifier)
  set(JUCER_BUNDLE_IDENTIFIER "com.custom.bundleIdentifier")
  _FRUT_generate_plist_file(target "App-BundleIdentifier" "APPL" "????")
endfunction()

function(test_app_plist_exporter_bundle_identifier)
  set(JUCER_BUNDLE_IDENTIFIER "com.custom.bundleIdentifier")
  set(JUCER_EXPORTER_BUNDLE_IDENTIFIER "com.custom.exporterBundleIdentifier")
  _FRUT_generate_plist_file(target "App-ExporterBundleIdentifier" "APPL" "????")
endfunction()


function(test_app_plist_screen_orientations_default)
  _FRUT_generate_plist_file(target "App-ScreenOrientations-default" "APPL" "????")
endfunction()

function(test_app_plist_screen_orientations_identical)
  set(JUCER_IPHONE_SCREEN_ORIENTATIONS "UIInterfaceOrientationPortrait")
  set(JUCER_IPAD_SCREEN_ORIENTATIONS "UIInterfaceOrientationPortrait")
  _FRUT_generate_plist_file(target "App-ScreenOrientations-identical" "APPL" "????")
endfunction()

function(test_app_plist_screen_orientations_different)
  set(JUCER_IPHONE_SCREEN_ORIENTATIONS
    "UIInterfaceOrientationLandscapeLeft" "UIInterfaceOrientationLandscapeRight"
  )
  _FRUT_generate_plist_file(target "App-ScreenOrientations-different" "APPL" "????")
endfunction()


function(test_app_plist_file_sharing_enabled)
  set(JUCER_FILE_SHARING_ENABLED ON)
  _FRUT_generate_plist_file(target "App-FileSharingEnabled" "APPL" "????")
endfunction()


function(test_app_plist_support_document_browser)
  set(JUCER_SUPPORT_DOCUMENT_BROWSER ON)
  _FRUT_generate_plist_file(target "App-SupportDocumentBrowser" "APPL" "????")
endfunction()


function(test_app_plist_status_bar_hidden)
  set(JUCER_STATUS_BAR_HIDDEN ON)
  _FRUT_generate_plist_file(target "App-StatusBarHidden" "APPL" "????")
endfunction()

function(test_app_plist_status_bar_hidden_pre_6_0_8)
  set(JUCER_VERSION "6.0.5")
  set(JUCER_STATUS_BAR_HIDDEN ON)
  _FRUT_generate_plist_file(target "App-StatusBarHidden-pre-6.0.8" "APPL" "????")
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


function(test_app_plist_audio_background_capability)
  set(JUCER_AUDIO_BACKGROUND_CAPABILITY ON)
  _FRUT_generate_plist_file(target "App-AudioBackgroundCapability" "APPL" "????")
endfunction()

function(test_app_plist_bluetooth_midi_background_capability)
  set(JUCER_BLUETOOTH_MIDI_BACKGROUND_CAPABILITY ON)
  _FRUT_generate_plist_file(target "App-BluetoothMIDIBackgroundCapability" "APPL" "????")
endfunction()

function(test_app_plist_push_notifications_capability)
  set(JUCER_PUSH_NOTIFICATIONS_CAPABILITY ON)
  _FRUT_generate_plist_file(target "App-PushNotificationsCapability" "APPL" "????")
endfunction()

function(test_app_plist_all_background_capabilities)
  set(JUCER_AUDIO_BACKGROUND_CAPABILITY ON)
  set(JUCER_BLUETOOTH_MIDI_BACKGROUND_CAPABILITY ON)
  set(JUCER_PUSH_NOTIFICATIONS_CAPABILITY ON)
  _FRUT_generate_plist_file(target "App-AllBackgroundCapabilities" "APPL" "????")
endfunction()


test_app_plist_all_background_capabilities()
test_app_plist_audio_background_capability()
test_app_plist_bluetooth_access_custom()
test_app_plist_bluetooth_access_default()
test_app_plist_bluetooth_midi_background_capability()
test_app_plist_bundle_identifier()
test_app_plist_camera_access_custom()
test_app_plist_camera_access_default()
test_app_plist_copyright()
test_app_plist_default()
test_app_plist_exporter_bundle_identifier()
test_app_plist_file_sharing_enabled()
test_app_plist_microphone_access_custom()
test_app_plist_microphone_access_default()
test_app_plist_pre_6_0_8()
test_app_plist_push_notifications_capability()
test_app_plist_screen_orientations_default()
test_app_plist_screen_orientations_different()
test_app_plist_screen_orientations_identical()
test_app_plist_status_bar_hidden()
test_app_plist_status_bar_hidden_pre_6_0_8()
test_app_plist_support_document_browser()
test_app_plist_version()
