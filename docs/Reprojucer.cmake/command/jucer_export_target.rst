jucer_export_target
===================

Define the settings specific to an export target.

::

  jucer_export_target(
    <exporter>
    [<keyword> <value>]...
  )

``<exporter>`` must be one of the :ref:`supported exporters <supported-export-targets>`.

``<keyword>`` can be one of the following keywords:

- ``EXTERNAL_LIBRARIES_TO_LINK``
- ``EXTRA_COMPILER_FLAGS``
- ``EXTRA_LINKER_FLAGS``
- ``EXTRA_PREPROCESSOR_DEFINITIONS``
- ``ICON_LARGE``
- ``ICON_SMALL``
- ``TARGET_PROJECT_FOLDER``
- ``VST_SDK_FOLDER``

When ``<exporter>`` equals ``"Xcode (MacOSX)"``, ``<keyword>`` can also be one of the
following keywords:

- ``CUSTOM_PLIST``
- ``CUSTOM_XCODE_RESOURCE_FOLDERS``
- ``DEVELOPMENT_TEAM_ID``
- ``DOCUMENT_FILE_EXTENSIONS`` (only if :doc:`jucer_project_settings()
  <jucer_project_settings>` was called with ``PROJECT_TYPE "GUI Application"``)
- ``EXTRA_FRAMEWORKS``
- ``GNU_COMPILER_EXTENSIONS``
- ``POSTBUILD_SHELL_SCRIPT``
- ``PREBUILD_SHELL_SCRIPT``
- ``VST3_SDK_FOLDER``

When ``<exporter>`` equals ``"Visual Studio 2017"``, ``"Visual Studio 2015"`` or
``"Visual Studio 2013"``, ``<keyword>`` can also be one of the following keywords:

- ``PLATFORM_TOOLSET``
- ``USE_IPP_LIBRARY``
- ``VST3_SDK_FOLDER``

When ``<exporter>`` equals ``"Linux Makefile"``, ``<keyword>`` can also be one of the
following keywords:

- ``CXX_STANDARD_TO_USE``
- ``GNU_COMPILER_EXTENSIONS``
- ``PKGCONFIG_LIBRARIES``
