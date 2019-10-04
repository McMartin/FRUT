jucer_audio_plugin_settings
===========================

Define the settings specific to an *Audio Plug-in* project.

::

  jucer_audio_plugin_settings(
    [PLUGIN_FORMATS <plugin_format> [<plugin_format> ...]]
    [PLUGIN_CHARACTERISTICS <plugin_characteristic> [<plugin_characteristic> ...]]

    [BUILD_VST <ON|OFF>]
    [BUILD_VST3 <ON|OFF>]
    [BUILD_AUDIOUNIT <ON|OFF>]
    [BUILD_AUDIOUNIT_V3 <ON|OFF>]
    [BUILD_RTAS <ON|OFF>]
    [BUILD_AAX <ON|OFF>]
    [BUILD_STANDALONE_PLUGIN <ON|OFF>]
    [BUILD_UNITY_PLUGIN <ON|OFF>]

    [PLUGIN_NAME <plugin_name>]
    [PLUGIN_DESCRIPTION <plugin_description>]

    [PLUGIN_MANUFACTURER <plugin_manufacturer>]
    [PLUGIN_MANUFACTURER_CODE <plugin_manufacturer_code>]
    [PLUGIN_CODE <plugin_code>]

    [PLUGIN_CHANNEL_CONFIGURATIONS <plugin_channel_configurations>]

    [PLUGIN_IS_A_SYNTH <ON|OFF>]
    [PLUGIN_MIDI_INPUT <ON|OFF>]
    [PLUGIN_MIDI_OUTPUT <ON|OFF>]
    [MIDI_EFFECT_PLUGIN <ON|OFF>]
    [KEY_FOCUS <ON|OFF>]

    [PLUGIN_AAX_IDENTIFIER <plugin_aax_identifier>]
    [PLUGIN_AU_EXPORT_PREFIX <plugin_au_export_prefix>]
    [PLUGIN_AU_MAIN_TYPE <plugin_au_main_type>]
    [PLUGIN_AU_IS_SANDBOX_SAFE <ON|OFF>]

    [PLUGIN_VST_NUM_MIDI_INPUTS <1..16>]
    [PLUGIN_VST_NUM_MIDI_OUTPUTS <1..16>]

    [PLUGIN_VST_CATEGORY <plugin_vst_category>]
    [PLUGIN_VST3_CATEGORY <plugin_vst3_category>]
    [PLUGIN_RTAS_CATEGORY <plugin_rtas_category>]
    [PLUGIN_AAX_CATEGORY <plugin_aax_category>]
    [PLUGIN_VST_LEGACY_CATEGORY <plugin_vst_legacy_category>]
  )

You must call this command when you call :doc:`jucer_project_settings()
<jucer_project_settings>` with ``PROJECT_TYPE "Audio Plug-in"``.


Example
-------

From `the MultiOutSynth example of JUCE 5.2.1 <https://github.com/McMartin/FRUT/blob/
master/generated/JUCE-5.2.1/examples/PlugInSamples/MultiOutSynth/CMakeLists.txt
#L42-L68>`_:

.. code-block:: cmake
  :lineno-start: 42

  jucer_audio_plugin_settings(
    BUILD_VST ON
    BUILD_VST3 ON
    BUILD_AUDIOUNIT ON
    BUILD_AUDIOUNIT_V3 OFF
    BUILD_RTAS OFF
    BUILD_AAX ON
    BUILD_STANDALONE_PLUGIN OFF
    ENABLE_INTER_APP_AUDIO OFF
    PLUGIN_NAME "MultiOutSynth"
    PLUGIN_DESCRIPTION "MultiOutSynth"
    PLUGIN_MANUFACTURER "ROLI Ltd."
    PLUGIN_MANUFACTURER_CODE "ROLI"
    PLUGIN_CODE "MoSy"
    # PLUGIN_CHANNEL_CONFIGURATIONS
    PLUGIN_IS_A_SYNTH ON
    PLUGIN_MIDI_INPUT ON
    PLUGIN_MIDI_OUTPUT OFF
    MIDI_EFFECT_PLUGIN OFF
    KEY_FOCUS OFF
    PLUGIN_AU_EXPORT_PREFIX "MultiOutSynthAU"
    # PLUGIN_AU_MAIN_TYPE
    # VST_CATEGORY
    # PLUGIN_RTAS_CATEGORY
    PLUGIN_AAX_CATEGORY "AAX_ePlugInCategory_SWGenerators"
    PLUGIN_AAX_IDENTIFIER "com.roli.MultiOutSynth"
  )
