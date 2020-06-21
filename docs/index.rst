.. # Copyright (C) 2018-2020  Alain Martin
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

.. image:: ../FRUT.svg
  :target: https://github.com/McMartin/FRUT
  :alt: FRUT

FRUT makes it easy to build `JUCE`_ projects using `CMake`_ instead of `Projucer`_. It
enables more flexibility in project architecture, simplified CI setup, and easier
integration with other JUCE and non-JUCE projects. Converting an existing JUCE project to
FRUT is easy, and you don't need to be a CMake expert to use it!


Contents
--------

FRUT currently contains:

- ``Jucer2CMake``, a console application that converts ``.jucer`` project files into
  ready-to-use ``CMakeLists.txt`` files,

- ``Reprojucer.cmake``, a CMake module that provides high-level functions to reproduce
  how a JUCE project is defined in Projucer,

- ``Jucer2Reprojucer``, a console application that converts ``.jucer`` project files into
  ready-to-use ``CMakeLists.txt`` files that include and use ``Reprojucer.cmake``,

- several ``CMakeLists.txt`` files generated from existing ``.jucer`` project files,
  including:

  - ``examples`` and ``extras`` projects from `JUCE 4.2.0`_ (in `generated/JUCE-4.2.0`_)
  - ``examples`` and ``extras`` projects from `JUCE 4.3.1`_ (in `generated/JUCE-4.3.1`_)
  - ``examples`` and ``extras`` projects from `JUCE 5.0.0`_ (in `generated/JUCE-5.0.0`_)
  - ``examples`` and ``extras`` projects from `JUCE 5.2.1`_ (in `generated/JUCE-5.2.1`_)
  - ``examples`` and ``extras`` projects from `JUCE 5.3.1`_ (in `generated/JUCE-5.3.1`_)
  - ``examples`` and ``extras`` projects from `JUCE 5.4.3`_ (in `generated/JUCE-5.4.3`_)
  - ``examples`` and ``extras`` projects from `JUCE 5.4.7`_ (in `generated/JUCE-5.4.7`_)


User documentation
------------------

.. toctree::
  :maxdepth: 2

  /Reprojucer.cmake/index


.. _CMake: https://cmake.org
.. _JUCE 4.2.0: https://github.com/juce-framework/JUCE/tree/4.2.0
.. _JUCE 4.3.1: https://github.com/juce-framework/JUCE/tree/4.3.1
.. _JUCE 5.0.0: https://github.com/juce-framework/JUCE/tree/5.0.0
.. _JUCE 5.2.1: https://github.com/juce-framework/JUCE/tree/5.2.1
.. _JUCE 5.3.1: https://github.com/juce-framework/JUCE/tree/5.3.1
.. _JUCE 5.4.3: https://github.com/juce-framework/JUCE/tree/5.4.3
.. _JUCE 5.4.7: https://github.com/juce-framework/JUCE/tree/5.4.7
.. _JUCE: https://github.com/juce-framework/JUCE
.. _Projucer: https://juce.com/discover/projucer
.. _generated/JUCE-4.2.0: https://github.com/McMartin/FRUT/tree/master/generated/JUCE-4.2.0
.. _generated/JUCE-4.3.1: https://github.com/McMartin/FRUT/tree/master/generated/JUCE-4.3.1
.. _generated/JUCE-5.0.0: https://github.com/McMartin/FRUT/tree/master/generated/JUCE-5.0.0
.. _generated/JUCE-5.2.1: https://github.com/McMartin/FRUT/tree/master/generated/JUCE-5.2.1
.. _generated/JUCE-5.3.1: https://github.com/McMartin/FRUT/tree/master/generated/JUCE-5.3.1
.. _generated/JUCE-5.4.3: https://github.com/McMartin/FRUT/tree/master/generated/JUCE-5.4.3
.. _generated/JUCE-5.4.7: https://github.com/McMartin/FRUT/tree/master/generated/JUCE-5.4.7
