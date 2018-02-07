jucer_export_target
===================

Define the settings specific to an export target.

::

  jucer_export_target(
    <exporter>
    [<setting_tag> <setting_value>]...
  )

``<exporter>`` must be one of the :ref:`supported exporters <supported-export-targets>`.

``<setting_tag>`` can be one of the following tags:

- ``EXTERNAL_LIBRARIES_TO_LINK``
- ``EXTRA_COMPILER_FLAGS``
- ``EXTRA_LINKER_FLAGS``
- ``EXTRA_PREPROCESSOR_DEFINITIONS``
- ``ICON_LARGE``
- ``ICON_SMALL``
- ``TARGET_PROJECT_FOLDER``
- ``VST_SDK_FOLDER``

When ``<exporter>`` equals ``"Xcode (MacOSX)"``, ``<setting_tag>`` can also be one of the
following tags:

- ``CUSTOM_PLIST``
- ``CUSTOM_XCODE_RESOURCE_FOLDERS``
- ``DEVELOPMENT_TEAM_ID``
- ``DOCUMENT_FILE_EXTENSIONS`` (only if :doc:`jucer_project_settings()
  <jucer_project_settings>` was called with ``PROJECT_TYPE "GUI Application"``)
- ``EXTRA_FRAMEWORKS``
- ``POSTBUILD_SHELL_SCRIPT``
- ``PREBUILD_SHELL_SCRIPT``
- ``VST3_SDK_FOLDER``

When ``<exporter>`` equals ``"Visual Studio 2017"``, ``"Visual Studio 2015"`` or
``"Visual Studio 2013"``, ``<setting_tag>`` can also be one of the following tags:

- ``PLATFORM_TOOLSET``
- ``USE_IPP_LIBRARY``
- ``VST3_SDK_FOLDER``

When ``<exporter>`` equals ``"Linux Makefile"``, ``<setting_tag>`` can also be one of the
following tags:

- ``CXX_STANDARD_TO_USE``
- ``PKGCONFIG_LIBRARIES``
