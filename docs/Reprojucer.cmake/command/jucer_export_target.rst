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

    [CUSTOM_XCODE_RESOURCE_FOLDERS <xcode_resource_folder> [<xcode_resource_folders> ...]]
    [DOCUMENT_FILE_EXTENSIONS <file_extension> [<file_extension> ...]]
    [CUSTOM_PLIST <plist_content>]
    [EXTRA_FRAMEWORKS <osx_framework> [<osx_framework> ...]]
    [PREBUILD_SHELL_SCRIPT <shell_script_content>]
    [POSTBUILD_SHELL_SCRIPT <shell_script_content>]
    [USE_HEADERMAP <ON|OFF>]

    [PLATFORM_TOOLSET <platform_toolset>]

    [CXX_STANDARD_TO_USE <cxx_standard>]
    [PKGCONFIG_LIBRARIES <library> [<library> ...]]

    [TARGET_PLATFORM <target_platform>]
  )

``<exporter>`` must be one of the :ref:`supported exporters <supported-export-targets>`.

``VST3_SDK_FOLDER`` is only supported by the following exporters: ``"Xcode (MacOSX)"``,
``"Visual Studio 2017"``, ``"Visual Studio 2015"`` and ``"Visual Studio 2013"``.

``GNU_COMPILER_EXTENSIONS`` is only supported by the following exporters:
``"Xcode (MacOSX)"``, ``"Linux Makefile"`` and ``"Code::Blocks (Windows)"``.

``CUSTOM_PLIST``, ``CUSTOM_XCODE_RESOURCE_FOLDERS``,  ``EXTRA_FRAMEWORKS``,
``POSTBUILD_SHELL_SCRIPT``, ``PREBUILD_SHELL_SCRIPT`` and ``USE_HEADERMAP`` are only
supported by the ``"Xcode (MacOSX)"`` exporter.

``DOCUMENT_FILE_EXTENSIONS`` is only supported by the ``"Xcode (MacOSX)"`` exporter on
``"GUI Application"`` projects.

``PLATFORM_TOOLSET`` is only supported by the following exporters:
``"Visual Studio 2017"``, ``"Visual Studio 2015"`` and ``"Visual Studio 2013"``.

``CXX_STANDARD_TO_USE`` and ``PKGCONFIG_LIBRARIES`` are only supported by the
``"Linux Makefile"`` exporter.

``TARGET_PLATFORM`` is only supported by the ``"Code::Blocks (Windows)"`` exporter.
