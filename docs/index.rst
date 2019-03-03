FRUT
====

``FRUT`` is a collection of tools dedicated to building `JUCE`_ projects using `CMake`_
instead of `Projucer`_.

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
  - ``examples`` and ``extras`` projects from `JUCE 5.2.1`_ (in `generated/JUCE-5.2.1`_)
  - ``examples`` and ``extras`` projects from `JUCE 5.3.1`_ (in `generated/JUCE-5.3.1`_)
  - ``examples`` and ``extras`` projects from `JUCE 5.4.3`_ (in `generated/JUCE-5.4.3`_)


User documentation
------------------

.. toctree::
  :maxdepth: 2

  /Reprojucer.cmake/index


.. _CMake: https://cmake.org/
.. _JUCE 4.2.0: https://github.com/WeAreROLI/JUCE/tree/4.2.0
.. _JUCE 4.3.1: https://github.com/WeAreROLI/JUCE/tree/4.3.1
.. _JUCE 5.0.0: https://github.com/WeAreROLI/JUCE/tree/5.0.0
.. _JUCE 5.2.1: https://github.com/WeAreROLI/JUCE/tree/5.2.1
.. _JUCE 5.3.1: https://github.com/WeAreROLI/JUCE/tree/5.3.1
.. _JUCE 5.4.3: https://github.com/WeAreROLI/JUCE/tree/5.4.3
.. _JUCE: https://juce.com/
.. _Projucer: https://www.juce.com/projucer
.. _generated/JUCE-4.2.0: https://github.com/McMartin/FRUT/tree/master/generated/JUCE-4.2.0
.. _generated/JUCE-4.3.1: https://github.com/McMartin/FRUT/tree/master/generated/JUCE-4.3.1
.. _generated/JUCE-5.0.0: https://github.com/McMartin/FRUT/tree/master/generated/JUCE-5.0.0
.. _generated/JUCE-5.2.1: https://github.com/McMartin/FRUT/tree/master/generated/JUCE-5.2.1
.. _generated/JUCE-5.3.1: https://github.com/McMartin/FRUT/tree/master/generated/JUCE-5.3.1
.. _generated/JUCE-5.4.3: https://github.com/McMartin/FRUT/tree/master/generated/JUCE-5.4.3
