.. # Copyright (C) 2017-2020  Alain Martin
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

jucer_project_module
====================

Add a JUCE module to the current JUCE project.

::

  jucer_project_module(
    <module_name>
    PATH <modules_folder>
    [ADD_SOURCE_TO_PROJECT <ON|OFF>]
    [<module_config_flag> <ON|OFF>]...
  )


``<modules_folder>`` must be the path to the folder that contains the module folder, not
the module folder itself. For instance, if you want to add the module ``juce_core`` and
its header is located at ``~/dev/JUCE/modules/juce_core/juce_core.h``, then
``<modules_folder>`` must be ``~/dev/JUCE/modules``.


Example
-------

From `the Plugin Host example of JUCE 5.2.1 <https://github.com/McMartin/FRUT/blob/
main/generated/JUCE-5.2.1/examples/audio%20plugin%20host/CMakeLists.txt#L86-L99>`_:

.. code-block:: cmake
  :lineno-start: 86

  jucer_project_module(
    juce_audio_processors
    PATH "../../modules"
    JUCE_PLUGINHOST_VST ON
    JUCE_PLUGINHOST_VST3 ON
    JUCE_PLUGINHOST_AU ON
  )

  jucer_project_module(
    juce_audio_utils
    PATH "../../modules"
    JUCE_USE_CDREADER OFF
    JUCE_USE_CDBURNER OFF
  )
