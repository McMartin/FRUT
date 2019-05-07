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
