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

project("entitlements-generation-plugin-MacOSX")


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../../../cmake")
include(Reprojucer)


set(JUCER_PROJECT_TYPE "Audio Plug-in")


function(test_entitlements_default)
  _FRUT_generate_entitlements_file("default.entitlements" _)
  if(NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/default.entitlements")
    message(FATAL_ERROR "Expected .entitlements file")
  endif()
endfunction()


function(test_entitlements_use_app_sandbox_no_options)
  set(JUCER_USE_APP_SANDBOX ON)
  _FRUT_generate_entitlements_file("UseAppSandbox_NoOptions.entitlements" _)
  if(NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/UseAppSandbox_NoOptions.entitlements")
    message(FATAL_ERROR "Expected .entitlements file")
  endif()
endfunction()


function(test_entitlements_use_app_sandbox_one_option)
  set(JUCER_USE_APP_SANDBOX ON)
  set(JUCER_APP_SANDBOX_OPTIONS "com.apple.security.device.microphone")
  _FRUT_generate_entitlements_file("UseAppSandbox_OneOption.entitlements" _)
endfunction()


function(test_entitlements_use_app_sandbox_several_options)
  set(JUCER_USE_APP_SANDBOX ON)
  set(JUCER_APP_SANDBOX_OPTIONS
    "com.apple.security.assets.music.read-write"
    "com.apple.security.device.microphone"
    "com.apple.security.device.usb"
  )
  _FRUT_generate_entitlements_file("UseAppSandbox_SeveralOptions.entitlements" _)
endfunction()


function(test_entitlements_use_hardened_runtime_no_options)
  set(JUCER_USE_HARDENED_RUNTIME ON)
  _FRUT_generate_entitlements_file("UseHardenedRuntime_NoOptions.entitlements" _)
  if(NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/UseHardenedRuntime_NoOptions.entitlements")
    message(FATAL_ERROR "Expected .entitlements file")
  endif()
endfunction()


function(test_entitlements_use_hardened_runtime_one_option)
  set(JUCER_USE_HARDENED_RUNTIME ON)
  set(JUCER_HARDENED_RUNTIME_OPTIONS "com.apple.security.cs.allow-jit")
  _FRUT_generate_entitlements_file("UseHardenedRuntime_OneOption.entitlements" _)
endfunction()


function(test_entitlements_use_hardened_runtime_several_options)
  set(JUCER_USE_HARDENED_RUNTIME ON)
  set(JUCER_HARDENED_RUNTIME_OPTIONS
    "com.apple.security.cs.allow-jit"
    "com.apple.security.cs.disable-library-validation"
    "com.apple.security.device.audio-input"
  )
  _FRUT_generate_entitlements_file("UseHardenedRuntime_SeveralOptions.entitlements" _)
endfunction()


function(test_entitlements_push_notifications_capability)
  set(JUCER_PUSH_NOTIFICATIONS_CAPABILITY ON)
  _FRUT_generate_entitlements_file("PushNotificationsCapability.entitlements" _)
endfunction()


function(test_entitlements_all_settings)
  set(JUCER_USE_APP_SANDBOX ON)
  set(JUCER_APP_SANDBOX_OPTIONS "com.apple.security.device.microphone")
  set(JUCER_USE_HARDENED_RUNTIME ON)
  set(JUCER_HARDENED_RUNTIME_OPTIONS "com.apple.security.device.audio-input")
  set(JUCER_PUSH_NOTIFICATIONS_CAPABILITY ON)
  _FRUT_generate_entitlements_file("AllSettings.entitlements" _)
endfunction()


test_entitlements_all_settings()
test_entitlements_default()
test_entitlements_push_notifications_capability()
test_entitlements_use_app_sandbox_no_options()
test_entitlements_use_app_sandbox_one_option()
test_entitlements_use_app_sandbox_several_options()
test_entitlements_use_hardened_runtime_no_options()
test_entitlements_use_hardened_runtime_one_option()
test_entitlements_use_hardened_runtime_several_options()
