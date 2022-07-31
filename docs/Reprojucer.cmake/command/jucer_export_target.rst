.. # Copyright (C) 2017-2022  Alain Martin
.. #
.. # This file is part of FRUT.
.. #
.. # FRUT is free software: you can redistribute it and/or modify
.. # it under the terms of the GNU General Public License as published by
.. # the Free Software Foundation, either version 3 of the License, or
.. # (at your option) any later version.
.. #
.. # FRUT is distributed in the hope that it will be useful,
.. # but WITHOUT ANY WARRANTY; without even the implied warranty of
.. # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
.. # GNU General Public License for more details.
.. #
.. # You should have received a copy of the GNU General Public License
.. # along with FRUT.  If not, see <http://www.gnu.org/licenses/>.

jucer_export_target
===================

Define the settings specific to an export target.

::

  jucer_export_target(
    <exporter>

    [TARGET_PROJECT_FOLDER <target_project_folder>]

    [VST_LEGACY_SDK_FOLDER <vst_legacy_sdk_folder>]
    [VST_SDK_FOLDER <vst_sdk_folder>]
    [VST3_SDK_FOLDER <vst3_sdk_folder>]  # [1]

    [EXTRA_PREPROCESSOR_DEFINITIONS <preprocessor_definition> [<preprocessor_definition> ...]]
    [EXTRA_COMPILER_FLAGS <compiler_flag> [<compiler_flag> ...]]
    [EXTRA_LINKER_FLAGS <linker_flag> [<linker_flag> ...]]
    [EXTERNAL_LIBRARIES_TO_LINK <library> [<library> ...]]

    [GNU_COMPILER_EXTENSIONS <ON|OFF>]  # [2]

    [ICON_SMALL <icon_file>]
    [ICON_LARGE <icon_file>]

    [CUSTOM_XCASSETS_FOLDER <xcassets_folder>]  # [7]
    [CUSTOM_LAUNCH_STORYBOARD <launch_storyboard>]  # [7]
    [CUSTOM_XCODE_RESOURCE_FOLDERS <xcode_resource_folder> [<xcode_resource_folder> ...]]  # [3]
    [ADD_DUPLICATE_RESOURCES_FOLDER_TO_APP_EXTENSION <ON|OFF>]  # [4]

    [DOCUMENT_FILE_EXTENSIONS <file_extension> [<file_extension> ...]]  # [6]
    [VALID_ARCHITECTURES <architecture> [<architecture> ...]]  # [5]
    [USE_APP_SANDBOX <ON|OFF>]  # [5]
    [APP_SANDBOX_INHERITANCE <ON|OFF>]  # [5]
    [APP_SANDBOX_OPTIONS <app_sandbox_option> [<app_sandbox_option> ...]]  # [5]
    [USE_HARDENED_RUNTIME <ON|OFF>]  # [5]
    [HARDENED_RUNTIME_OPTIONS <hardened_runtime_option> [<hardened_runtime_option> ...]]  # [5]

    [DEVICE_FAMILY <iPhone|iPad|Universal>]  # [7]
    [IPHONE_SCREEN_ORIENTATION <screen_orientation> [<screen_orientation> ...]]  # [7]
    [IPAD_SCREEN_ORIENTATION <screen_orientation> [<screen_orientation> ...]]  # [7]
    [FILE_SHARING_ENABLED <ON|OFF>]  # [7]
    [SUPPORT_DOCUMENT_BROWSER <ON|OFF>]  # [7]
    [STATUS_BAR_HIDDEN <ON|OFF>]  # [7]

    [MICROPHONE_ACCESS <ON|OFF>]  # [3]
    [MICROPHONE_ACCESS_TEXT <microphone_access_text>]  # [3]
    [CAMERA_ACCESS <ON|OFF>]  # [3]
    [CAMERA_ACCESS_TEXT <camera_access_text>]  # [3]
    [BLUETOOTH_ACCESS <ON|OFF>]  # [3]
    [BLUETOOTH_ACCESS_TEXT <bluetooth_access_text>]  # [3]
    [SEND_APPLE_EVENTS <ON|OFF>]  # [5]
    [SEND_APPLE_EVENTS_TEXT <send_apple_events_text>]  # [5]

    [IN_APP_PURCHASES_CAPABILITY <ON|OFF>]  # [3]
    [CONTENT_SHARING <ON|OFF>]  # [7]
    [AUDIO_BACKGROUND_CAPABILITY <ON|OFF>]  # [7]
    [BLUETOOTH_MIDI_BACKGROUND_CAPABILITY <ON|OFF>]  # [7]
    [APP_GROUPS_CAPABILITY <ON|OFF>]  # [7]
    [ICLOUD_PERMISSIONS <ON|OFF>] # [7]
    [PUSH_NOTIFICATIONS_CAPABILITY <ON|OFF>]  # [3]

    [CUSTOM_PLIST <plist_content>]  # [3]
    [PLIST_PREPROCESS <ON|OFF>]  # [3]
    [PLIST_PREFIX_HEADER <plist_prefix_header>]  # [3]
    [SUPPRESS_AUDIOUNIT_PLIST_RESOURCE_USAGE_KEY <ON|OFF>]  # [3]
    [EXTRA_SYSTEM_FRAMEWORKS <osx_framework> [<osx_framework> ...]]  # [3]
    [EXTRA_FRAMEWORKS <osx_framework> [<osx_framework> ...]]  # [3]
    [FRAMEWORK_SEARCH_PATHS <framework_search_path> [<framework_search_path> ...]]  # [3]
    [EXTRA_CUSTOM_FRAMEWORKS <osx_framework> [<osx_framework> ...]]  # [3]
    [PREBUILD_SHELL_SCRIPT <shell_script_content>]  # [3]
    [POSTBUILD_SHELL_SCRIPT <shell_script_content>]  # [3]
    [EXPORTER_BUNDLE_IDENTIFIER <bundle_identifier>]  # [3]
    [DEVELOPMENT_TEAM_ID <development_team_id>]  # [3]

    [APP_GROUP_ID <app_group_id> [<app_group_id> ...]]  # [7]

    [USE_HEADERMAP <ON|OFF>]  # [3]

    [MANIFEST_FILE <manifest_file>]  # [8]
    [PLATFORM_TOOLSET <platform_toolset>]  # [8]
    [USE_IPP_LIBRARY <ipp_library_linking_method>]  # [8]
    [WINDOWS_TARGET_PLATFORM <windows_target_platform>]  # [8]

    [CXX_STANDARD_TO_USE <cxx_standard>]  # [9]
    [PKGCONFIG_LIBRARIES <library> [<library> ...]]  # [9]

    [TARGET_PLATFORM <target_platform>]  # [10]
  )

``<exporter>`` must be one of the :ref:`supported exporters <supported-exporters>`.

Optional keywords support:

- ``[1]``: only supported by the ``"Xcode (macOS)"``, ``"Visual Studio 2022"``,
  ``"Visual Studio 2019"``, ``"Visual Studio 2017"``, ``"Visual Studio 2015"``, and
  ``"Visual Studio 2013"`` exporters.
- ``[2]``: only supported by the ``"Xcode (macOS)"``, ``"Xcode (iOS)"``,
  ``"Linux Makefile"``, ``"Code::Blocks (Windows)"``, and ``"Code::Blocks (Linux)"``
  exporters.
- ``[3]``: only supported by the ``"Xcode (macOS)"`` and ``"Xcode (iOS)"`` exporters.
- ``[4]``: only supported by the ``"Xcode (macOS)"`` and ``"Xcode (iOS)"`` exporters, on
  ``"Audio Plug-in"`` projects.
- ``[5]``: only supported by the ``"Xcode (macOS)"`` exporter.
- ``[6]``: only supported by the ``"Xcode (macOS)"`` exporter, on ``"GUI Application"``
  projects.
- ``[7]``: only supported by the ``"Xcode (iOS)"`` exporter.
- ``[8]``: only supported by the ``"Visual Studio 2022"``, ``"Visual Studio 2019"``,
  ``"Visual Studio 2017"``, ``"Visual Studio 2015"``, and ``"Visual Studio 2013"``
  exporters.
- ``[9]``: only supported by the ``"Linux Makefile"`` exporter.
- ``[10]``: only supported by the ``"Code::Blocks (Windows)"`` exporter.


Examples
--------

From `the AudioPluginHost extra of JUCE 5.4.3 <https://github.com/McMartin/FRUT/blob/
main/generated/JUCE-5.4.3/extras/AudioPluginHost/CMakeLists.txt#L194-L208>`_:

.. code-block:: cmake
  :lineno-start: 194

  jucer_export_target(
    "Xcode (MacOSX)"
    # VST3_SDK_FOLDER
    EXTRA_COMPILER_FLAGS
      "-Wall"
      "-Wshadow"
      "-Wstrict-aliasing"
      "-Wconversion"
      "-Wsign-compare"
      "-Woverloaded-virtual"
      "-Wextra-semi"
    ICON_SMALL "Source/JUCEAppIcon.png"
    ICON_LARGE "Source/JUCEAppIcon.png"
    MICROPHONE_ACCESS ON
  )


From `the DemoRunner example of JUCE 5.4.3 <https://github.com/McMartin/FRUT/blob/main/
generated/JUCE-5.4.3/examples/DemoRunner/CMakeLists.txt#L372-L380>`_:

.. code-block:: cmake
  :lineno-start: 325

  jucer_export_target(
    "Visual Studio 2017"
    EXTRA_COMPILER_FLAGS
      "/w44265"
      "/w45038"
      "/w44062"
    ICON_SMALL "Source/JUCEAppIcon.png"
    ICON_LARGE "Source/JUCEAppIcon.png"
  )


From `the Projucer extra of JUCE 5.4.3 <https://github.com/McMartin/FRUT/blob/
main/generated/JUCE-5.4.3/extras/Projucer/CMakeLists.txt#L719-L724>`_:

.. code-block:: cmake
  :lineno-start: 719

  jucer_export_target(
    "Linux Makefile"
    # EXTRA_COMPILER_FLAGS
    # EXTRA_LINKER_FLAGS
    ICON_LARGE "Source/BinaryData/Icons/juce_icon.png"
  )
