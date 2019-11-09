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

    [CUSTOM_XCODE_RESOURCE_FOLDERS <xcode_resource_folder> [<xcode_resource_folder> ...]]  # [3]
    [ADD_DUPLICATE_RESOURCES_FOLDER_TO_APP_EXTENSION <ON|OFF>]  # [4]

    [DOCUMENT_FILE_EXTENSIONS <file_extension> [<file_extension> ...]]  # [6]
    [USE_APP_SANDBOX <ON|OFF>]  # [5]
    [APP_SANDBOX_OPTIONS <app_sandbox_option> [<app_sandbox_option> ...]]  # [5]
    [USE_HARDENED_RUNTIME <ON|OFF>]  # [5]
    [HARDENED_RUNTIME_OPTIONS <hardened_runtime_option> [<hardened_runtime_option> ...]]  # [5]

    [DEVICE_FAMILY <iPhone|iPad|Universal>]  # [7]
    [IPHONE_SCREEN_ORIENTATION <Portrait and Landscape|Portrait|Landscape>]  # [7]
    [IPAD_SCREEN_ORIENTATION <Portrait and Landscape|Portrait|Landscape>]  # [7]
    [FILE_SHARING_ENABLED <ON|OFF>]  # [7]
    [SUPPORT_DOCUMENT_BROWSER <ON|OFF>]  # [7]
    [STATUS_BAR_HIDDEN <ON|OFF>]  # [7]

    [MICROPHONE_ACCESS <ON|OFF>]  # [3]
    [MICROPHONE_ACCESS_TEXT <microphone_access_text>]  # [3]
    [CAMERA_ACCESS <ON|OFF>]  # [3]
    [CAMERA_ACCESS_TEXT <camera_access_text>]  # [3]
    [BLUETOOTH_ACCESS <ON|OFF>]  # [7]
    [BLUETOOTH_ACCESS_TEXT <bluetooth_access_text>]  # [7]

    [IN_APP_PURCHASES_CAPABILITY <ON|OFF>]  # [3]
    [AUDIO_BACKGROUND_CAPABILITY <ON|OFF>]  # [7]
    [BLUETOOTH_MIDI_BACKGROUND_CAPABILITY <ON|OFF>]  # [7]
    [PUSH_NOTIFICATIONS_CAPABILITY <ON|OFF>]  # [3]

    [CUSTOM_PLIST <plist_content>]  # [3]
    [EXTRA_SYSTEM_FRAMEWORKS <osx_framework> [<osx_framework> ...]]  # [3]
    [EXTRA_FRAMEWORKS <osx_framework> [<osx_framework> ...]]  # [3]
    [FRAMEWORK_SEARCH_PATHS <framework_search_path> [<framework_search_path> ...]]  # [3]
    [EXTRA_CUSTOM_FRAMEWORKS <osx_framework> [<osx_framework> ...]]  # [3]
    [PREBUILD_SHELL_SCRIPT <shell_script_content>]  # [3]
    [POSTBUILD_SHELL_SCRIPT <shell_script_content>]  # [3]
    [EXPORTER_BUNDLE_IDENTIFIER <bundle_identifier>]  # [3]
    [DEVELOPMENT_TEAM_ID <development_team_id>]  # [3]
    [USE_HEADERMAP <ON|OFF>]  # [3]

    [PLATFORM_TOOLSET <platform_toolset>]  # [8]
    [USE_IPP_LIBRARY <ipp_library_linking_method>]  # [8]
    [WINDOWS_TARGET_PLATFORM <windows_target_platform>]  # [8]

    [CXX_STANDARD_TO_USE <cxx_standard>]  # [9]
    [PKGCONFIG_LIBRARIES <library> [<library> ...]]  # [9]

    [TARGET_PLATFORM <target_platform>]  # [10]
  )

``<exporter>`` must be one of the :ref:`supported exporters <supported-exporters>`.

Optional keywords support:

- ``[1]``: only supported by the ``"Xcode (MacOSX)"``, ``"Visual Studio 2019"``,
  ``"Visual Studio 2017"``, ``"Visual Studio 2015"``, and ``"Visual Studio 2013"``
  exporters.
- ``[2]``: only supported by the ``"Xcode (MacOSX)"``, ``"Xcode (iOS)"``,
  ``"Linux Makefile"``, ``"Code::Blocks (Windows)"``, and ``"Code::Blocks (Linux)"``
  exporters.
- ``[3]``: only supported by the ``"Xcode (MacOSX)"`` and ``"Xcode (iOS)"`` exporters.
- ``[4]``: only supported by the ``"Xcode (MacOSX)"`` and ``"Xcode (iOS)"`` exporters, on
  ``"Audio Plug-in"`` projects.
- ``[5]``: only supported by the ``"Xcode (MacOSX)"`` exporter.
- ``[6]``: only supported by the ``"Xcode (MacOSX)"`` exporter, on ``"GUI Application"``
  projects.
- ``[7]``: only supported by the ``"Xcode (iOS)"`` exporter.
- ``[8]``: only supported by the ``"Visual Studio 2019"``, ``"Visual Studio 2017"``,
  ``"Visual Studio 2015"``, and ``"Visual Studio 2013"`` exporters.
- ``[9]``: only supported by the ``"Linux Makefile"`` exporter.
- ``[10]``: only supported by the ``"Code::Blocks (Windows)"`` exporter.


Examples
--------

From `the AudioPluginHost extra of JUCE 5.4.3 <https://github.com/McMartin/FRUT/blob/
master/generated/JUCE-5.4.3/extras/AudioPluginHost/CMakeLists.txt#L194-L208>`_:

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


From `the DemoRunner example of JUCE 5.4.3 <https://github.com/McMartin/FRUT/blob/master/
generated/JUCE-5.4.3/examples/DemoRunner/CMakeLists.txt#L369-L377>`_:

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
master/generated/JUCE-5.4.3/extras/Projucer/CMakeLists.txt#L719-L724>`_:

.. code-block:: cmake
  :lineno-start: 719

  jucer_export_target(
    "Linux Makefile"
    # EXTRA_COMPILER_FLAGS
    # EXTRA_LINKER_FLAGS
    ICON_LARGE "Source/BinaryData/Icons/juce_icon.png"
  )
