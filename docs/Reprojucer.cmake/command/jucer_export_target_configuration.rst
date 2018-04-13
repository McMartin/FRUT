jucer_export_target_configuration
=================================

Define the settings specific to a configuration associated with an export target.

::

  jucer_export_target_configuration(
    <exporter>

    NAME <configuration_name>
    DEBUG_MODE <ON|OFF>

    [BINARY_NAME <binary_name>]
    [BINARY_LOCATION <binary_location>]
    [HEADER_SEARCH_PATHS <search_path> [<search_path> ...]]
    [EXTRA_LIBRARY_SEARCH_PATHS <search_path> [<search_path> ...]]
    [PREPROCESSOR_DEFINITIONS <preprocessor_definition> [<preprocessor_definition> ...]]

    [ENABLE_PLUGIN_COPY_STEP <ON|OFF>]
    [VST_BINARY_LOCATION <binary_location>]
    [VST3_BINARY_LOCATION <binary_location>]
    [AU_BINARY_LOCATION <binary_location>]

    [OPTIMISATION <optimisation>]

    [OSX_BASE_SDK_VERSION <osx_sdk>]
    [OSX_DEPLOYMENT_TARGET <osx_deployment_target>]
    [OSX_ARCHITECTURE <osx_architecture>]
    [CUSTOM_XCODE_FLAGS <xcode_flag> [<xcode_flag> ...]]
    [CXX_LANGUAGE_STANDARD <cxx_standard>]
    [CXX_LIBRARY <cxx_library>]
    [CODE_SIGNING_IDENTITY <code_signing_identity>]
    [LINK_TIME_OPTIMISATION <ON|OFF>]
    [STRIP_LOCAL_SYMBOLS <ON|OFF>]

    [WARNING_LEVEL <warning_level>]
    [TREAT_WARNINGS_AS_ERRORS <ON|OFF>]
    [RUNTIME_LIBRARY <runtime_library>]
    [WHOLE_PROGRAM_OPTIMISATION <whole_program_optimisation>]
    [INCREMENTAL_LINKING <ON|OFF>]
    [PREBUILD_COMMAND <command_content>]
    [POSTBUILD_COMMAND <command_content>]
    [GENERATE_MANIFEST <ON|OFF>]
    [CHARACTER_SET <character_set>]

    [RELAX_IEEE_COMPLIANCE <ON|OFF>]

    [ARCHITECTURE <architecture>]
  )

``<exporter>`` must be one of the :ref:`supported exporters <supported-export-targets>`.

``AU_BINARY_LOCATION``, ``CODE_SIGNING_IDENTITY``, ``CUSTOM_XCODE_FLAGS``,
``CXX_LANGUAGE_STANDARD``, ``CXX_LIBRARY``, ``LINK_TIME_OPTIMISATION``,
``OSX_ARCHITECTURE``, ``OSX_BASE_SDK_VERSION``, ``OSX_DEPLOYMENT_TARGET`` and
``STRIP_LOCAL_SYMBOLS`` are only supported by the ``"Xcode (MacOSX)"`` exporter.

``ENABLE_PLUGIN_COPY_STEP``, ``VST3_BINARY_LOCATION`` and ``VST_BINARY_LOCATION`` are only
supported by the following exporters: ``"Xcode (MacOSX)"``, ``"Visual Studio 2017"``,
``"Visual Studio 2015"`` and ``"Visual Studio 2013"``.

``CHARACTER_SET``, ``GENERATE_MANIFEST``, ``INCREMENTAL_LINKING``, ``POSTBUILD_COMMAND``,
``PREBUILD_COMMAND``, ``RUNTIME_LIBRARY``, ``TREAT_WARNINGS_AS_ERRORS``, ``WARNING_LEVEL``
and ``WHOLE_PROGRAM_OPTIMISATION`` are only supported by the following exporters:
``"Visual Studio 2017"``, ``"Visual Studio 2015"`` and ``"Visual Studio 2013"``.

``RELAX_IEEE_COMPLIANCE`` is only supported by the following exporters:
``"Xcode (MacOSX)"``, ``"Visual Studio 2017"``, ``"Visual Studio 2015"`` and
``"Visual Studio 2013"``.

``ARCHITECTURE`` is only supported by the following exporters: ``"Visual Studio 2017"``,
``"Visual Studio 2015"``, ``"Visual Studio 2013"`` and ``"Linux Makefile"``.
