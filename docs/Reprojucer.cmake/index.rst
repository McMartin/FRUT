Reprojucer.cmake
================

``Reprojucer.cmake`` is a CMake module that provides high-level functions to reproduce how
a JUCE project is defined in Projucer.


Using ``Reprojucer.cmake``
--------------------------

Add the following lines at the top of your CMakeLists.txt file to use
``Reprojucer.cmake``:

.. code:: cmake

  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/<relative_path_to_FRUT>/cmake")
  include(Reprojucer)


Requirements
------------

``Reprojucer.cmake`` requires CMake version 3.4 minimum.


.. _supported-exporters:

Supported Projucer exporters
----------------------------

``Reprojucer.cmake`` supports the following Projucer exporters (also known as "export
targets"):

.. table::
  :align: left
  :widths: auto

  +-----------+-------------------------------+--------------------------------------+
  | Supported | Exporter                      | CMake requirements                   |
  +===========+===============================+======================================+
  | ✔️        | Xcode (MacOSX)                |                                      |
  +-----------+-------------------------------+--------------------------------------+
  | ❌        | Xcode (iOS)                   |                                      |
  +-----------+-------------------------------+--------------------------------------+
  | ✔️        | Visual Studio 2019            | version 3.14 minimum                 |
  +-----------+-------------------------------+--------------------------------------+
  | ✔️        | Visual Studio 2017            | version 3.7 minimum (3.13.3 when     |
  |           |                               | VS |nbsp| 2019 is installed)         |
  +-----------+-------------------------------+--------------------------------------+
  | ✔️        | Visual Studio 2015            |                                      |
  +-----------+-------------------------------+--------------------------------------+
  | ✔️        | Visual Studio 2013            |                                      |
  +-----------+-------------------------------+--------------------------------------+
  | ✔️        | Linux Makefile                |                                      |
  +-----------+-------------------------------+--------------------------------------+
  | ❌        | Android                       |                                      |
  +-----------+-------------------------------+--------------------------------------+
  | ✔️        | Code::Blocks |nbsp| (Windows) |                                      |
  +-----------+-------------------------------+--------------------------------------+
  | ✔️        | Code::Blocks |nbsp| (Linux)   |                                      |
  +-----------+-------------------------------+--------------------------------------+

.. _Reprojucer-commands:

Commands
---------

``Reprojucer.cmake`` defines the following commands:

.. toctree::
  :maxdepth: 1

  command/jucer_project_begin
  command/jucer_project_settings
  command/jucer_audio_plugin_settings
  command/jucer_project_files
  command/jucer_project_module
  command/jucer_appconfig_header
  command/jucer_export_target
  command/jucer_export_target_configuration
  command/jucer_project_end


.. |nbsp| unicode:: 0xA0
  :trim:
