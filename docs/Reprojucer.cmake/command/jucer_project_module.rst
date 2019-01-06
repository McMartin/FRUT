jucer_project_module
====================

Add a JUCE module to the current JUCE project.

::

  jucer_project_module(
    <module_name>
    PATH <modules_folder>
    [ADD_SOURCE_TO_PROJECT <ON|OFF>]
    [COMPILE_IN_TARGET <target_name>]
    [<module_config_flag> <ON|OFF>]...
  )


``<modules_folder>`` must be the path to the folder that contains the module folder, not
the module folder itself. For instance, if you want to add the module ``juce_core`` and
its header is located at ``~/dev/JUCE/modules/juce_core/juce_core.h``, then
``<modules_folder>`` must be ``~/dev/JUCE/modules``.

``COMPILE_IN_TARGET`` is a feature that doesn't exist in Projucer. It allows you to
compile the source files of the JUCE module in a specific CMake target, instead of with
the code of your project. This is particularly useful when you want to:
 - disable compiler warnings coming from the JUCE module without disabling them on your
own code,
 - speed-up compiling the JUCE module by using pre-compiled headers.


Examples
--------

From `the Plugin Host example of JUCE 5.2.1 <https://github.com/McMartin/FRUT/blob/
master/generated/JUCE-5.2.1/examples/audio%20plugin%20host/CMakeLists.txt#L86-L99>`_:

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

From `Jucer2Reprojucer <https://github.com/McMartin/FRUT/blob/master/Jucer2Reprojucer/
CMakeLists.txt#L79-L85>`_:

.. code:: cmake

  jucer_project_module(
    juce_core
    PATH "${JUCE_ROOT}/modules"
    ADD_SOURCE_TO_PROJECT OFF
    JUCE_USE_CURL OFF
    COMPILE_IN_TARGET "JuceModules"
  )

In this case, ``juce_core.cpp`` is compiled in the ``JuceModules`` target instead of in
the ``Jucer2Reprojucer`` target.
