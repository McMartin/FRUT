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


function(test_app_plist_screen_orientation)
  foreach(iphone_screen_orientation IN ITEMS "portraitlandscape" "portrait" "landscape")
    foreach(ipad_screen_orientation IN ITEMS "portraitlandscape" "portrait" "landscape")
      set(JUCER_IPHONE_SCREEN_ORIENTATION "${iphone_screen_orientation}")
      set(JUCER_IPAD_SCREEN_ORIENTATION "${ipad_screen_orientation}")
      _FRUT_generate_plist_file(target
        "App-ScreenOrientation-iPhone_${iphone_screen_orientation}-iPad_${ipad_screen_orientation}"
        "APPL" "????"
      )
    endforeach()
  endforeach()
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


function(test_app_plist_push_notifications_capability)
  set(JUCER_PUSH_NOTIFICATIONS_CAPABILITY ON)
  _FRUT_generate_plist_file(target "App-PushNotificationsCapability" "APPL" "????")
endfunction()


test_app_plist_bundle_identifier()
test_app_plist_camera_access_custom()
test_app_plist_camera_access_default()
test_app_plist_copyright()
test_app_plist_default()
test_app_plist_exporter_bundle_identifier()
test_app_plist_microphone_access_custom()
test_app_plist_microphone_access_default()
test_app_plist_push_notifications_capability()
test_app_plist_screen_orientation()
test_app_plist_version()
