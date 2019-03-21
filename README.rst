FRUT
====

|Code_of_Conduct| |AppVeyor| |Azure_Pipelines| |Travis_CI| |Read_the_Docs|

FRUT is a collection of tools dedicated to building `JUCE`_ projects using `CMake`_
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
- Code::Blocks (Windows)
- Code::Blocks (Linux)


Documentation
-------------

You can read the documentation of FRUT on Read the Docs: https://frut.readthedocs.io


Getting started
---------------

Let's consider that you have a copy of `JUCE`_, a copy of `FRUT`_ and a JUCE project
called ``MyGreatProject`` following this folder structure: ::

  <root>
  ├── FRUT/
  ├── JUCE/
  └── MyGreatProject/
      ├── Source/
      └── MyGreatProject.jucer

We first build and install FRUT with CMake: ::

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

  $ cmake .. -G<generator>
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


Contributors
------------

FRUT follows the `all-contributors`_ specification and is brought to you by these awesome
contributors:

.. raw:: html

  <table>
  <tbody align="center">
  <tr>
    <td>
      <a href="https://github.com/McMartin"><img src="https://github.com/McMartin.png" width="100">Alain Martin</a>
      <br />
      <a href="https://github.com/McMartin/FRUT/pulls?q=state%3Amerged+author%3AMcMartin" title="Code">💻</a>
      <a href="https://github.com/McMartin/FRUT/pulls?q=state%3Amerged+reviewed-by%3AMcMartin+-author%3AMcMartin+" title="Pull Request reviews">👀</a>
      <a href="https://github.com/McMartin/FRUT/commits/master/docs?author=McMartin" title="Documentation">📖</a>
    </td>
    <td>
      <a href="https://github.com/MartyLake"><img src="https://github.com/MartyLake.png" width="100">Matthieu Talbot</a>
      <br />
      <a href="https://github.com/McMartin/FRUT/pulls?q=state%3Amerged+reviewed-by%3AMartyLake+-author%3AMartyLake+" title="Pull Request reviews">👀</a>
      <a href="https://github.com/McMartin/FRUT/pulls?q=state%3Amerged+author%3AMartyLake" title="Code">💻</a>
      <a href="https://github.com/McMartin/FRUT/issues?q=is%3Aissue+author%3AMartyLake" title="Bug reports">🐛</a>
    </td>
    <td>
      <a href="https://github.com/gonzaloflirt"><img src="https://github.com/gonzaloflirt.png" width="100">Florian Goltz</a>
      <br />
      <a href="https://github.com/McMartin/FRUT/pulls?q=state%3Amerged+author%3Agonzaloflirt" title="Code">💻</a>
    </td>
    <td>
      <a href="https://github.com/WGuLL"><img src="https://github.com/WGuLL.png" width="100">Fabien Roussel</a>
      <br />
      <a href="https://github.com/McMartin/FRUT/issues?q=is%3Aissue+author%3AWGuLL" title="Bug reports">🐛</a>
      <a href="https://github.com/McMartin/FRUT/pulls?q=state%3Amerged+reviewed-by%3AWGuLL+-author%3AWGuLL+" title="Pull Request reviews">👀</a>
    </td>
    <td>
      <a href="https://github.com/Xav83"><img src="https://github.com/Xav83.png" width="100">Xavier Jouvenot</a>
      <br />
      <a href="https://github.com/McMartin/FRUT/issues?q=is%3Aissue+author%3AXav83" title="Bug reports">🐛</a>
      <a href="https://github.com/McMartin/FRUT/pulls?q=state%3Amerged+reviewed-by%3AXav83+-author%3AXav83+" title="Pull Request reviews">👀</a>
    </td>
    <td>
      <a href="https://github.com/lethal-guitar"><img src="https://github.com/lethal-guitar.png" width="100">Nikolai Wuttke</a>
      <br />
      <a href="https://github.com/McMartin/FRUT/pulls?q=state%3Amerged+reviewed-by%3Alethal-guitar+-author%3Alethal-guitar+" title="Pull Request reviews">👀</a>
    </td>
  </tr>
  <tr>
    <td>
      <a href="https://github.com/PioBeat"><img src="https://github.com/PioBeat.png" width="100">Dominik Grzelak</a>
      <br />
      <a href="https://github.com/McMartin/FRUT/issues?q=is%3Aissue+author%3APioBeat" title="Bug reports">🐛</a>
    </td>
    <td>
      <a href="https://github.com/dscheffer"><img src="https://github.com/dscheffer.png" width="100">Dennis Scheffer</a>
      <br />
      <a href="https://github.com/McMartin/FRUT/issues?q=is%3Aissue+author%3Adscheffer" title="Bug reports">🐛</a>
      <a href="https://github.com/McMartin/FRUT/pulls?q=state%3Amerged+author%3Adscheffer" title="Code">💻</a>
    </td>
    <td>
      <a href="https://github.com/scotchi"><img src="https://github.com/scotchi.png" width="100">Scott Wheeler</a>
      <br />
      <a href="https://github.com/McMartin/FRUT/pulls?q=state%3Amerged+author%3Ascotchi" title="Code">💻</a>
      <a href="https://github.com/McMartin/FRUT/issues?q=is%3Aissue+author%3Ascotchi" title="Bug reports">🐛</a>
    </td>
    <td>
      <a href="https://github.com/IqraShahzad1"><img src="https://github.com/IqraShahzad1.png" width="100">Iqra Shahzad</a>
      <br />
      <a href="https://github.com/McMartin/FRUT/issues?q=is%3Aissue+author%3AIqraShahzad1" title="Bug reports">🐛</a>
    </td>
    <td>
      <a href="https://github.com/rclement"><img src="https://github.com/rclement.png" width="100">Romain Clement</a>
      <br />
      <a href="https://github.com/McMartin/FRUT/issues?q=is%3Aissue+author%3Arclement" title="Bug reports">🐛</a>
    </td>
    <td>
    </td>
  </tr>
  </tbody>
  </table>


License
-------

|GPLv3|

FRUT is free software: you can redistribute it and/or modify it under the terms of the GNU
General Public License as published by the Free Software Foundation, either version 3 of
the License, or (at your option) any later version.

FRUT is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
`LICENSE`_ file for more details.


.. |Code_of_Conduct| image:: https://img.shields.io/badge/code%20of%20conduct-Contributor%20Covenant-blue.svg?style=flat
  :target: CODE_OF_CONDUCT.md
  :alt: Contributor Covenant Code of Conduct

.. |AppVeyor| image:: https://ci.appveyor.com/api/projects/status/github/McMartin/frut?branch=master&svg=true
  :target: https://ci.appveyor.com/project/McMartin/frut
  :alt: AppVeyor build status

.. |Azure_Pipelines| image:: https://dev.azure.com/McMartin/FRUT/_apis/build/status/McMartin.FRUT?branchName=master
  :target: https://dev.azure.com/McMartin/FRUT/_build?definitionId=2
  :alt: Azure Pipelines build status

.. |Travis_CI| image:: https://travis-ci.org/McMartin/FRUT.svg?branch=master
  :target: https://travis-ci.org/McMartin/FRUT
  :alt: Travis CI build status

.. |Read_the_Docs| image:: https://readthedocs.org/projects/frut/badge/?version=latest
  :target: https://frut.readthedocs.io
  :alt: Documentation status

.. |GPLv3| image:: https://www.gnu.org/graphics/gplv3-127x51.png
  :target: https://www.gnu.org/licenses/gpl.html
  :alt: GNU General Public License

.. _CONTRIBUTING: CONTRIBUTING.md
.. _LICENSE: LICENSE
.. _generated/JUCE-4.2.0: generated/JUCE-4.2.0
.. _generated/JUCE-4.3.1: generated/JUCE-4.3.1
.. _generated/JUCE-5.0.0: generated/JUCE-5.0.0
.. _generated/JUCE-5.2.1: generated/JUCE-5.2.1
.. _generated/JUCE-5.3.1: generated/JUCE-5.3.1
.. _generated/JUCE-5.4.3: generated/JUCE-5.4.3

.. _CMake Generators: https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html
.. _CMake: https://cmake.org
.. _FRUT: https://github.com/McMartin/FRUT
.. _JUCE 4.2.0: https://github.com/WeAreROLI/JUCE/tree/4.2.0
.. _JUCE 4.3.1: https://github.com/WeAreROLI/JUCE/tree/4.3.1
.. _JUCE 5.0.0: https://github.com/WeAreROLI/JUCE/tree/5.0.0
.. _JUCE 5.2.1: https://github.com/WeAreROLI/JUCE/tree/5.2.1
.. _JUCE 5.3.1: https://github.com/WeAreROLI/JUCE/tree/5.3.1
.. _JUCE 5.4.3: https://github.com/WeAreROLI/JUCE/tree/5.4.3
.. _JUCE: https://github.com/WeAreROLI/JUCE
.. _Projucer: https://juce.com/discover/projucer
.. _all-contributors: https://github.com/all-contributors/all-contributors
