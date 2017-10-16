jucer_export_target_configuration
=================================

Define the settings specific to a configuration associated with an export target.

::

  jucer_export_target_configuration(
    <exporter>
    NAME <configuration_name>
    DEBUG_MODE <ON|OFF>
    [<setting_tag> <setting_value>]...
  )

``<exporter>`` must be one of the `supported exporters
<../README.rst#supported-export-targets>`_.

``<setting_tag>`` can be one of the following tags:

- ``BINARY_LOCATION``
- ``BINARY_NAME``
- ``EXTRA_LIBRARY_SEARCH_PATHS``
- ``HEADER_SEARCH_PATHS``
- ``OPTIMISATION``
- ``PREPROCESSOR_DEFINITIONS``

When ``<exporter>`` equals ``"Xcode (MacOSX)"``, ``<setting_tag>`` can also be one of the
following tags:

- ``AU_BINARY_LOCATION``
- ``CODE_SIGNING_IDENTITY``
- ``CXX_LANGUAGE_STANDARD``
- ``CXX_LIBRARY``
- ``LINK_TIME_OPTIMISATION``
- ``OSX_ARCHITECTURE``
- ``OSX_BASE_SDK_VERSION``
- ``OSX_DEPLOYMENT_TARGET``
- ``RELAX_IEEE_COMPLIANCE``
- ``STRIP_LOCAL_SYMBOLS``
- ``VST3_BINARY_LOCATION``
- ``VST_BINARY_LOCATION``

When ``<exporter>`` equals ``"Visual Studio 2015"`` or ``"Visual Studio 2013"``,
``<setting_tag>`` can also be one of the following tags:

- ``ARCHITECTURE``
- ``CHARACTER_SET``
- ``GENERATE_MANIFEST``
- ``INCREMENTAL_LINKING``
- ``POSTBUILD_COMMAND``
- ``PREBUILD_COMMAND``
- ``RELAX_IEEE_COMPLIANCE``
- ``RUNTIME_LIBRARY``
- ``TREAT_WARNINGS_AS_ERRORS``
- ``WARNING_LEVEL``
- ``WHOLE_PROGRAM_OPTIMISATION``

When ``<exporter>`` equals ``"Linux Makefile"``, ``<setting_tag>`` can also be one of the
following tags:

- ``ARCHITECTURE``
