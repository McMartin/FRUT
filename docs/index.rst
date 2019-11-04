FRUT
====

FRUT makes it easy to build `JUCE`_ projects using `CMake`_ instead of `Projucer`_. It
enables more flexibility in project architecture, simplified CI setup, and easier
integration with other JUCE and non-JUCE projects. Converting an existing JUCE project to
FRUT is easy, and you don't need to be a CMake expert to use it!


Contents
--------

FRUT currently contains:

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
  - ``examples`` and ``extras`` projects from `JUCE 5.4.4`_ (in `generated/JUCE-5.4.4`_)


User documentation
------------------

.. toctree::
  :maxdepth: 2

  /Reprojucer.cmake/index


.. _CMake: https://cmake.org
.. _JUCE 4.2.0: https://github.com/WeAreROLI/JUCE/tree/4.2.0
.. _JUCE 4.3.1: https://github.com/WeAreROLI/JUCE/tree/4.3.1
.. _JUCE 5.0.0: https://github.com/WeAreROLI/JUCE/tree/5.0.0
.. _JUCE 5.2.1: https://github.com/WeAreROLI/JUCE/tree/5.2.1
.. _JUCE 5.3.1: https://github.com/WeAreROLI/JUCE/tree/5.3.1
.. _JUCE 5.4.3: https://github.com/WeAreROLI/JUCE/tree/5.4.3
.. _JUCE 5.4.4: https://github.com/WeAreROLI/JUCE/tree/5.4.4
.. _JUCE: https://github.com/WeAreROLI/JUCE
.. _Projucer: https://juce.com/discover/projucer
.. _generated/JUCE-4.2.0: https://github.com/McMartin/FRUT/tree/master/generated/JUCE-4.2.0
.. _generated/JUCE-4.3.1: https://github.com/McMartin/FRUT/tree/master/generated/JUCE-4.3.1
.. _generated/JUCE-5.0.0: https://github.com/McMartin/FRUT/tree/master/generated/JUCE-5.0.0
.. _generated/JUCE-5.2.1: https://github.com/McMartin/FRUT/tree/master/generated/JUCE-5.2.1
.. _generated/JUCE-5.3.1: https://github.com/McMartin/FRUT/tree/master/generated/JUCE-5.3.1
.. _generated/JUCE-5.4.3: https://github.com/McMartin/FRUT/tree/master/generated/JUCE-5.4.3
.. _generated/JUCE-5.4.4: https://github.com/McMartin/FRUT/tree/master/generated/JUCE-5.4.4
