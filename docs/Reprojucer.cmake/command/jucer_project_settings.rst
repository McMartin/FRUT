jucer_project_settings
======================

Define the settings specific to a JUCE project.

::

  jucer_project_settings(
    PROJECT_NAME <project_name>
    PROJECT_TYPE <GUI Application |
                  Console Application |
                  Static Library |
                  Dynamic Library |
                  Audio Plug-in>
    [PROJECT_VERSION <project_version>]

    [COMPANY_NAME <company_name>]
    [COMPANY_COPYRIGHT <company_copyright>]
    [COMPANY_WEBSITE <company_website>]
    [COMPANY_EMAIL <company_email>]

    [REPORT_JUCE_APP_USAGE <ON|OFF>]
    [DISPLAY_THE_JUCE_SPLASH_SCREEN <ON|OFF>]
    [SPLASH_SCREEN_COLOUR <splash_screen_colour>]

    [BUNDLE_IDENTIFIER <bundle_identifier>]

    [BINARYDATACPP_SIZE_LIMIT <binarydatacpp_size_limit>]
    [INCLUDE_BINARYDATA <ON|OFF>]
    [BINARYDATA_NAMESPACE <binarydata_namespace>]

    [CXX_LANGUAGE_STANDARD <cxx_language_standard>]
    [PREPROCESSOR_DEFINITIONS <preprocessor_definition> [<preprocessor_definition> ...]]
    [HEADER_SEARCH_PATHS <header_search_path> [<header_search_path> ...]]
  )
