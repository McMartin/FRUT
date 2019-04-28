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


Example
-------

From `the DemoRunner example of JUCE 5.4.3 <https://github.com/McMartin/FRUT/blob/master/
generated/JUCE-5.4.3/examples/DemoRunner/CMakeLists.txt#L28-L43>`_:

.. code-block:: cmake
  :lineno-start: 28

  jucer_project_settings(
    PROJECT_NAME "DemoRunner"
    PROJECT_VERSION "5.4.3"
    COMPANY_NAME "ROLI Ltd."
    COMPANY_COPYRIGHT "Copyright (c) 2018 - ROLI Ltd."
    COMPANY_WEBSITE "https://www.juce.com/"
    COMPANY_EMAIL "info@juce.com"
    REPORT_JUCE_APP_USAGE ON # Required for closed source applications without an Indie or Pro JUCE license
    DISPLAY_THE_JUCE_SPLASH_SCREEN ON # Required for closed source applications without an Indie or Pro JUCE license
    PROJECT_TYPE "GUI Application"
    BUNDLE_IDENTIFIER "com.juce.demorunner"
    CXX_LANGUAGE_STANDARD "C++14"
    PREPROCESSOR_DEFINITIONS
      "JUCE_DEMO_RUNNER=1"
      "JUCE_UNIT_TESTS=1"
  )
