.. # Copyright (C) 2017-2019, 2022  Alain Martin
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
  | Supported | Exporter                      | CMake requirements and options       |
  +===========+===============================+======================================+
  | ✔️        | Xcode (MacOSX)                |                                      |
  +-----------+-------------------------------+--------------------------------------+
  | ✔️        | Xcode (iOS)                   | version 3.14 minimum                 |
  |           |                               | ``-G Xcode -DCMAKE_SYSTEM_NAME=iOS`` |
  +-----------+-------------------------------+--------------------------------------+
  | ✔️        | Visual Studio 2022            | version 3.21 minimum                 |
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
