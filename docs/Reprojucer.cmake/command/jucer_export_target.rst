jucer_export_target
===================

Define the settings specific to an export target.

::

  jucer_export_target(
    <exporter>

    [TARGET_PROJECT_FOLDER <target_project_folder>]

    [VST_SDK_FOLDER <vst_sdk_folder>]
    [VST3_SDK_FOLDER <vst3_sdk_folder>]

    [EXTRA_PREPROCESSOR_DEFINITIONS <preprocessor_definition> [<preprocessor_definition> ...]]
    [EXTRA_COMPILER_FLAGS <compiler_flag> [<compiler_flag> ...]]
    [EXTRA_LINKER_FLAGS <linker_flag> [<linker_flag> ...]]
    [EXTERNAL_LIBRARIES_TO_LINK <library> [<library> ...]]

    [GNU_COMPILER_EXTENSIONS <ON|OFF>]

    [ICON_SMALL <icon_file>]
    [ICON_LARGE <icon_file>]

    [CUSTOM_XCODE_RESOURCE_FOLDERS <xcode_resource_folder> [<xcode_resource_folder> ...]]
    [DOCUMENT_FILE_EXTENSIONS <file_extension> [<file_extension> ...]]
    [MICROPHONE_ACCESS <ON|OFF>]
    [MICROPHONE_ACCESS_TEXT <microphone_access_text>]
    [CAMERA_ACCESS <ON|OFF>]
    [CAMERA_ACCESS_TEXT <camera_access_text>]
    [CUSTOM_PLIST <plist_content>]
    [EXTRA_FRAMEWORKS <osx_framework> [<osx_framework> ...]]
    [PREBUILD_SHELL_SCRIPT <shell_script_content>]
    [POSTBUILD_SHELL_SCRIPT <shell_script_content>]
    [USE_HEADERMAP <ON|OFF>]

    [PLATFORM_TOOLSET <platform_toolset>]
    [WINDOWS_TARGET_PLATFORM <windows_target_platform>]

    [CXX_STANDARD_TO_USE <cxx_standard>]
    [PKGCONFIG_LIBRARIES <library> [<library> ...]]

    [TARGET_PLATFORM <target_platform>]
  )

``<exporter>`` must be one of the :ref:`supported exporters <supported-export-targets>`.

``VST3_SDK_FOLDER`` is only supported by the following exporters: ``"Xcode (MacOSX)"``,
``"Visual Studio 2017"``, ``"Visual Studio 2015"`` and ``"Visual Studio 2013"``.

``GNU_COMPILER_EXTENSIONS`` is only supported by the following exporters:
``"Xcode (MacOSX)"``, ``"Linux Makefile"``, ``"Code::Blocks (Windows)"`` and
``"Code::Blocks (Linux)"``.

``CAMERA_ACCESS``, ``CAMERA_ACCESS_TEXT``, ``CUSTOM_PLIST``,
``CUSTOM_XCODE_RESOURCE_FOLDERS``,  ``EXTRA_FRAMEWORKS``, ``MICROPHONE_ACCESS``,
``MICROPHONE_ACCESS_TEXT``, ``POSTBUILD_SHELL_SCRIPT``, ``PREBUILD_SHELL_SCRIPT`` and
``USE_HEADERMAP`` are only supported by the ``"Xcode (MacOSX)"`` exporter.

``DOCUMENT_FILE_EXTENSIONS`` is only supported by the ``"Xcode (MacOSX)"`` exporter on
``"GUI Application"`` projects.

``PLATFORM_TOOLSET`` and ``WINDOWS_TARGET_PLATFORM`` are only supported by the following
exporters: ``"Visual Studio 2017"``, ``"Visual Studio 2015"`` and
``"Visual Studio 2013"``.

``CXX_STANDARD_TO_USE`` and ``PKGCONFIG_LIBRARIES`` are only supported by the
``"Linux Makefile"`` exporter.

``TARGET_PLATFORM`` is only supported by the ``"Code::Blocks (Windows)"`` exporter.
