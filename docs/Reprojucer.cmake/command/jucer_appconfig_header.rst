jucer_appconfig_header
======================

Specify the content of the AppConfig.h header that is not defined in the .jucer file.

::

  jucer_appconfig_header(
    USER_CODE_SECTION <user_code_section>
  )


Example
-------

From `the AudioPluginHost extra of JUCE 5.4.3 <https://github.com/McMartin/FRUT/blob/
master/generated/JUCE-5.4.3/extras/AudioPluginHost/CMakeLists.txt#L185-L192>`_:

.. code:: cmake

  jucer_appconfig_header(
    USER_CODE_SECTION
  "
  #ifndef JUCE_ANDROID
   #define JUCE_MODAL_LOOPS_PERMITTED   (! JUCE_IOS)
  #endif
  "
  )
