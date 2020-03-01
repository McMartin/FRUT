.. # Copyright (C) 2017-2019  Alain Martin
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

    [POST_EXPORT_SHELL_COMMAND_MACOS_LINUX <post_export_shell_command>]
    [POST_EXPORT_SHELL_COMMAND_WINDOWS <post_export_shell_command]
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
