.. # Copyright (C) 2017-2019  Alain Martin
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

jucer_project_files
===================

Register files to compile or to use as Xcode or binary resources and assign them to a
group.

::

  jucer_project_files(<group_name>
    [<compile> <xcode_resource> <binary_resource> <file_path> [<compiler_flag_scheme>]]...
  )

Use ``/`` in ``<group_name>`` to define group hierarchies. For instance, ``A/B/C`` is a
sub-group of ``A/B``.

``<compile>``, ``<xcode_resource>`` and ``<binary_resource>`` must be equal to ``x`` or
``.``, as shown in the example.


Example
-------

From `the AUv3Synth example of JUCE 5.2.1 <https://github.com/McMartin/FRUT/blob/master/
generated/JUCE-5.2.1/examples/AUv3Synth/CMakeLists.txt#L68-L82>`_:

.. code-block:: cmake
  :lineno-start: 68

  jucer_project_files("AUv3Synth/Source/BinaryData"
  # Compile   Xcode     Binary    File
  #           Resource  Resource
    .         x         .         "Source/BinaryData/power.png"
    .         .         x         "Source/BinaryData/proaudio.path"
    .         .         x         "Source/BinaryData/singing.ogg"
  )

  jucer_project_files("AUv3Synth/Source"
  # Compile   Xcode     Binary    File
  #           Resource  Resource
    .         .         .         "Source/MaterialLookAndFeel.h"
    .         .         .         "Source/AUv3SynthEditor.h"
    x         .         .         "Source/AUv3SynthProcessor.cpp"
  )
