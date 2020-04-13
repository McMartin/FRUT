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

project("entitlements-generation-guiapp-iOS")


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../../../cmake")
include(Reprojucer)


function(test_entitlements_default)
  _FRUT_generate_entitlements_file(target "default.entitlements" _)
endfunction()


function(test_entitlements_app_groups_capability_empty)
  set(JUCER_APP_GROUPS_CAPABILITY ON)
  set(JUCER_APP_GROUP_ID "")
  _FRUT_generate_entitlements_file(target "AppGroupsCapability-empty.entitlements" _)
endfunction()


function(test_entitlements_app_groups_capability_one_id)
  set(JUCER_APP_GROUPS_CAPABILITY ON)
  set(JUCER_APP_GROUP_ID "first")
  _FRUT_generate_entitlements_file(target "AppGroupsCapability-one_ID.entitlements" _)
endfunction()


function(test_entitlements_app_groups_capability_three_ids)
  set(JUCER_APP_GROUPS_CAPABILITY ON)
  set(JUCER_APP_GROUP_ID "first; second ; third")
  _FRUT_generate_entitlements_file(target "AppGroupsCapability-three_IDs.entitlements" _)
endfunction()


function(test_entitlements_icloud_permissions)
  set(JUCER_ICLOUD_PERMISSIONS ON)
  _FRUT_generate_entitlements_file(target "iCloudPermissions.entitlements" _)
endfunction()


function(test_entitlements_push_notifications_capability)
  set(JUCER_PUSH_NOTIFICATIONS_CAPABILITY ON)
  _FRUT_generate_entitlements_file(target "PushNotificationsCapability.entitlements" _)
endfunction()


test_entitlements_app_groups_capability_empty()
test_entitlements_app_groups_capability_one_id()
test_entitlements_app_groups_capability_three_ids()
test_entitlements_default()
test_entitlements_icloud_permissions()
test_entitlements_push_notifications_capability()
