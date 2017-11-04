jucer_project_module
====================

Add a JUCE module to the current JUCE project.

::

  jucer_project_module(
    <module_name>
    PATH <modules_folder>
    [<module_config_flag> <ON|OFF>]...
  )


``<modules_folder>`` must be the path to the folder that contains the module folder, not
the module folder itself. For instance, if you want to add the module ``juce_core`` and
its header is located at ``~/dev/JUCE/modules/juce_core/juce_core.h``, then
``<modules_folder>`` must be ``~/dev/JUCE/modules``.

Example
-------

.. code:: cmake

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

Taken from `the Plugin Host example of JUCE 4.3.1
<../../../generated/JUCE-4.3.1/examples/audio%20plugin%20host/CMakeLists.txt#L82-L95>`_.
