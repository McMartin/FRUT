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


.. _supported-export-targets:

Supported export targets
------------------------

``Reprojucer.cmake`` supports the following Projucer export targets (AKA exporters):

- Xcode (MacOSX)
- Visual Studio 2019
- Visual Studio 2017
- Visual Studio 2015
- Visual Studio 2013
- Linux Makefile
- Code::Blocks (Windows)
- Code::Blocks (Linux)


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
