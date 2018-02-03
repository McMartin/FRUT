FRUT
####

.. compound::

  |AppVeyor| |TravisCI|


``FRUT`` is a collection of tools dedicated to building `JUCE <https://juce.com/>`_
projects using `CMake`_ instead of `Projucer`_.

It currently contains:

- ``Reprojucer.cmake``, a CMake module that provides high-level functions to reproduce
  how a JUCE project is defined in Projucer,

- ``Jucer2Reprojucer``, a console application based on JUCE that converts ``.jucer``
  project files into ``CMakeLists.txt`` files that include and use ``Reprojucer.cmake``,

- several ``CMakeLists.txt`` files generated from existing ``.jucer`` project files,
  including:

  - ``examples`` and ``extras`` projects from `JUCE 4.2.0`_ (in `generated/JUCE-4.2.0`_)
  - ``examples`` and ``extras`` projects from `JUCE 4.3.1`_ (in `generated/JUCE-4.3.1`_)
  - ``examples`` and ``extras`` projects from `JUCE 5.0.0`_ (in `generated/JUCE-5.0.0`_)
  - ``examples`` and ``extras`` projects from `JUCE 5.2.0`_ (in `generated/JUCE-5.2.0`_)


Requirements
============

- CMake, version 3.4 minimum
- JUCE, version 4.2.0 minimum (JUCE 5 support is experimental)


Getting started
===============

Let's consider that you have a copy of `JUCE <https://github.com/WeAreROLI/JUCE>`_, a copy
of `FRUT`_ and a JUCE project called ``MyGreatProject`` following this folder structure:
::
  <root>
  ├── FRUT/
  ├── JUCE/
  └── MyGreatProject/
      ├── Source/
      └── MyGreatProject.jucer

We first build ``Jucer2Reprojucer`` with CMake. Since ``Jucer2Reprojucer`` uses the JUCE
modules ``juce_core``, ``juce_data_structures`` and ``juce_events``, we specify where to
find JUCE by defining ``JUCE_ROOT`` when calling ``cmake``.
::
  $ cd <root>/FRUT/Jucer2Reprojucer/

  $ mkdir build && cd build/

  $ cmake .. -DJUCE_ROOT=../../../JUCE
  ...
  -- Configuring done
  -- Generating done
  -- Build files have been written to: <root>/FRUT/Jucer2Reprojucer/build

  $ cmake --build .
  ...

Then we convert ``MyGreatProject.jucer`` to a new ``CMakeLists.txt`` file:
::
  $ cd <root>/MyGreatProject/

  $ ../FRUT/Jucer2Reprojucer/build(/Debug)/Jucer2Reprojucer MyGreatProject.jucer ../FRUT/cmake/Reprojucer.cmake

  <root>/MyGreatProject/CMakeLists.txt has been successfully generated.

Now we can build ``MyGreatProject`` using CMake:
::
  $ cd <root>/MyGreatProject/

  $ mkdir build && cd build/

  $ cmake .. -G<generator> -DMyGreatProject_jucer_FILE=../MyGreatProject.jucer
  ...
  -- Configuring done
  -- Generating done
  -- Build files have been written to: <root>/MyGreatProject/build

  $ cmake --build .
  ...

``<generator>`` can be one of many `CMake Generators`_ supported by your
platform, including Ninja, NMake Makefiles (on Windows only), Unix Makefiles (on Linux and
macOS), Visual Studio 2013, 2015 and 2017 (on Windows only), and Xcode (on macOS only).


Supported export targets
========================

``Reprojucer.cmake`` and ``Jucer2Reprojucer`` support the following Projucer export
targets:

- Xcode (MacOSX)
- Visual Studio 2017
- Visual Studio 2015
- Visual Studio 2013
- Linux Makefile


Contributing
============

Contributions to FRUT are very welcomed and you can contribute even if you don't know
anything about CMake. See the `CONTRIBUTING`_ file for more details.


License
=======

FRUT is free software: you can redistribute it and/or modify it under the terms of
the GNU General Public License as published by the Free Software Foundation, either
version 3 of the License, or (at your option) any later version.

FRUT is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the `LICENSE`_ file for more details.


.. |AppVeyor| image:: https://ci.appveyor.com/api/projects/status/github/McMartin/frut?branch=master&svg=true
    :target: https://ci.appveyor.com/project/McMartin/frut
    :alt: AppVeyor build status

.. |TravisCI| image:: https://travis-ci.org/McMartin/FRUT.svg?branch=master
   :target: https://travis-ci.org/McMartin/FRUT
   :alt: Travis CI build status

.. _CONTRIBUTING: CONTRIBUTING.md
.. _LICENSE: LICENSE
.. _generated/JUCE-4.2.0: generated/JUCE-4.2.0
.. _generated/JUCE-4.3.1: generated/JUCE-4.3.1
.. _generated/JUCE-5.0.0: generated/JUCE-5.0.0
.. _generated/JUCE-5.2.0: generated/JUCE-5.2.0

.. _CMake Generators: https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html
.. _CMake: https://cmake.org/
.. _FRUT: https://github.com/McMartin/FRUT
.. _JUCE 4.2.0: https://github.com/WeAreROLI/JUCE/tree/4.2.0
.. _JUCE 4.3.1: https://github.com/WeAreROLI/JUCE/tree/4.3.1
.. _JUCE 5.0.0: https://github.com/WeAreROLI/JUCE/tree/5.0.0
.. _JUCE 5.2.0: https://github.com/WeAreROLI/JUCE/tree/5.2.0
.. _Projucer: https://www.juce.com/projucer
