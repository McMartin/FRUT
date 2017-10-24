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


Supported export targets
------------------------

``Reprojucer.cmake`` supports the following Projucer export targets (AKA exporters):

- Xcode (MacOSX)
- Visual Studio 2015
- Visual Studio 2013
- Linux Makefiles


Commands
---------

``Reprojucer.cmake`` defines the following commands:

- `jucer_appconfig_header <command/jucer_appconfig_header.rst>`_
- `jucer_audio_plugin_settings <command/jucer_audio_plugin_settings.rst>`_
- `jucer_export_target <command/jucer_export_target.rst>`_
- `jucer_export_target_configuration <command/jucer_export_target_configuration.rst>`_
- `jucer_project_begin <command/jucer_project_begin.rst>`_
- `jucer_project_end <command/jucer_project_end.rst>`_
- `jucer_project_files <command/jucer_project_files.rst>`_
- `jucer_project_module <command/jucer_project_module.rst>`_
- `jucer_project_settings <command/jucer_project_settings.rst>`_
