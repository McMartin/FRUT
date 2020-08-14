.. # Copyright (C) 2017, 2019  Alain Martin
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
main/generated/JUCE-5.4.3/extras/AudioPluginHost/CMakeLists.txt#L185-L192>`_:

.. code-block:: cmake
  :lineno-start: 185

  jucer_appconfig_header(
    USER_CODE_SECTION
  "
  #ifndef JUCE_ANDROID
   #define JUCE_MODAL_LOOPS_PERMITTED   (! JUCE_IOS)
  #endif
  "
  )
