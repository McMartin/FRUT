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

project("entitlements-generation-plugin-iOS")


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../../../cmake")
include(Reprojucer)


set(JUCER_PROJECT_TYPE "Audio Plug-in")


function(test_entitlements_default)
  _FRUT_generate_entitlements_file("default.entitlements" _)
  if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/default.entitlements")
    message(FATAL_ERROR "Unexpected .entitlements file")
  endif()
endfunction()


function(test_entitlements_enable_iaa)
  set(JUCER_ENABLE_INTER_APP_AUDIO ON)
  _FRUT_generate_entitlements_file("EnableIAA.entitlements" _)
endfunction()


function(test_entitlements_push_notifications_capability)
  set(JUCER_PUSH_NOTIFICATIONS_CAPABILITY ON)
  _FRUT_generate_entitlements_file("PushNotificationsCapability.entitlements" _)
  if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/PushNotificationsCapability.entitlements")
    message(FATAL_ERROR "Unexpected .entitlements file")
  endif()
endfunction()


test_entitlements_default()
test_entitlements_enable_iaa()
test_entitlements_push_notifications_capability()
