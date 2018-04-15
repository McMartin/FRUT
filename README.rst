FRUT
====

.. compound::

  |AppVeyor| |Travis_CI| |Read_the_Docs|


``FRUT`` is a collection of tools dedicated to building `JUCE <https://juce.com/>`__
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
------------

- CMake, version 3.4 minimum
- JUCE, version 4.2.0 minimum


Supported export targets
------------------------

``Reprojucer.cmake`` and ``Jucer2Reprojucer`` support the following Projucer export
targets:

- Xcode (MacOSX)
- Visual Studio 2017
- Visual Studio 2015
- Visual Studio 2013
- Linux Makefile


Documentation
-------------

You can read the documentation of FRUT on Read the Docs: http://frut.readthedocs.io/


Getting started
---------------

Let's consider that you have a copy of `JUCE <https://github.com/WeAreROLI/JUCE>`__, a
copy of `FRUT`_ and a JUCE project called ``MyGreatProject`` following this folder
structure: ::

  <root>
  ├── FRUT/
  ├── JUCE/
  └── MyGreatProject/
      ├── Source/
      └── MyGreatProject.jucer

We first build and install ``FRUT`` with CMake: ::

  $ cd <root>/FRUT/

  $ mkdir build && cd build/

  $ cmake .. -DCMAKE_INSTALL_PREFIX=../prefix -DJUCE_ROOT=../../JUCE
  ...
  -- Configuring done
  -- Generating done
  -- Build files have been written to: <root>/FRUT/build

  $ cmake --build . --target install
  ...

Then we convert ``MyGreatProject.jucer`` to a new ``CMakeLists.txt`` file: ::

  $ cd <root>/MyGreatProject/

  $ ../FRUT/prefix/FRUT/bin/Jucer2Reprojucer MyGreatProject.jucer ../FRUT/prefix/FRUT/cmake/Reprojucer.cmake

  <root>/MyGreatProject/CMakeLists.txt has been successfully generated.

Now we can build ``MyGreatProject`` using CMake: ::

  $ cd <root>/MyGreatProject/

  $ mkdir build && cd build/

  $ cmake .. -G<generator> -DMyGreatProject_jucer_FILE=../MyGreatProject.jucer
  ...
  -- Configuring done
  -- Generating done
  -- Build files have been written to: <root>/MyGreatProject/build

  $ cmake --build .
  ...

``<generator>`` can be one of many `CMake Generators`_ supported by your platform,
including Ninja, NMake Makefiles (on Windows), Unix Makefiles (on Linux and macOS), Visual
Studio 2013, 2015 and 2017 (on Windows), and Xcode (on macOS).


Contributing
------------

Contributions to FRUT are very welcomed and you can contribute even if you don't know
anything about CMake. See the `CONTRIBUTING`_ file for more details.


License
-------

|GPLv3|

FRUT is free software: you can redistribute it and/or modify it under the terms of the GNU
General Public License as published by the Free Software Foundation, either version 3 of
the License, or (at your option) any later version.

FRUT is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
`LICENSE`_ file for more details.


.. |AppVeyor| image:: https://ci.appveyor.com/api/projects/status/github/McMartin/frut?branch=master&svg=true
  :target: https://ci.appveyor.com/project/McMartin/frut
  :alt: AppVeyor build status

.. |Travis_CI| image:: https://travis-ci.org/McMartin/FRUT.svg?branch=master
  :target: https://travis-ci.org/McMartin/FRUT
  :alt: Travis CI build status

.. |Read_the_Docs| image:: https://readthedocs.org/projects/frut/badge/?version=latest
  :target: https://frut.readthedocs.io/en/latest/
  :alt: Documentation status

.. |GPLv3| image:: https://www.gnu.org/graphics/gplv3-127x51.png
  :target: https://www.gnu.org/licenses/gpl.html
  :alt: GNU General Public License

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
