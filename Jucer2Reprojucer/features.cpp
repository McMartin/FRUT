// Copyright (c) 2017 Alain Martin
//
// This file is part of JUCE.cmake.
//
// JUCE.cmake is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// JUCE.cmake is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with JUCE.cmake.  If not, see <http://www.gnu.org/licenses/>.

#include "features.hpp"


bool getFeature(Feature feature, unsigned long commitSha1)
{
  switch (commitSha1)
  {
  case 0x75cd666: /* o Partially reverted fix for new VST3 SDK 3.6.7 by removing any... */
  case 0x14d6096: /* o Added support for VST3 SDK 3.6.7                                 */
  case 0xf5dafc4: /* o Fixed a bug in an Objective-C drag and drop helper class         */
  case 0x95085c1: /* o Listed all modules under the ISC license in the README           */
  case 0xb09a236: /* o Fixed a leak of OnScreenKeyboard                                 */
  case 0xdc5e039: /* o Fixed multiple file drag and drop on OS X                        */
  case 0x7778383: /* o Added code to only show the onscreen keyboard if Windows is i... */
  case 0x6977128: /* o Fixed a crash in the AAX wrapper when no AAX meters are present  */
  case 0xfc9937d: /* o   JUCE version 4.3.1                                             */
                  /* |\                                                                 */
  case 0x03b0df1: /* | o Re-saved all projects                                          */
  case 0xe0aff60: /* | o Bump version number to 4.3.1                                   */
  case 0x284fc82: /* | o Tidied up some Projucer code that was causing build errors ... */
  case 0xc754f6c: /* | o Fixed the SVG parser for an edge-case path string sequence,... */
  case 0x859567f: /* | o Fixed a compiler warning on iOS                                */
  case 0x64f0027: /* | o Added a workaround for a macOS Sierra font rendering issue ... */
  case 0xa6d3c97: /* | o Fixed a bug on OSX where images with SingleChannel PixelFor... */
  case 0x4474d55: /* | o Fixed an issue where VST2 would report wrong input layout f... */
  case 0xd13be21: /* | o Added support for enabling/disabling sidechains in AAX         */
  case 0xd16123a: /* | o Added a new optional in/out parameter to AudioProcessor::Bu... */
  case 0x4b507cd: /* | o Fixed Xcode exporter macro escapes for '"'                     */
  case 0xd0b6d6c: /* | o Check all mouse sources when exiting modal loop                */
  case 0xce14b66: /* | o Fixed a bug where a mouse enter event wouldn't be sent to t... */
  case 0x06dde9d: /* | o Removed unnecessary check in macOS/iOS URL session cancel      */
  case 0x3370ada: /* | o Fixed a compiler warning on newest version of gcc              */
  case 0xb144923: /* | o Removed multi-character constant which triggered warnings o... */
  case 0xa84c166: /* | o Fixed an issue where an iOS background download task would ... */
  case 0x7e1279e: /* | o Fixed an int sign conversion warning                           */
  case 0x9b1ae84: /* | o Fixed bug when maximum number of plug-in io channels is one    */
  case 0x96ea465: /* | o Resaved all projects                                           */
  case 0x46e21fe: /* | o Updated win32 Process::isForegroundProcess() method to use ... */
  case 0xd0b0644: /* | o Fixed compiler error in AUv3_Wrapper                           */
  case 0xd8ead14: /* | o Removed a duplicate header file                                */
  case 0x1baaddb: /* | o Added code to properly escape non-ascii characters in URLs ... */
  case 0xa554d0e: /* | o Added to the AudioProcessorValueTreeState documentation        */
  case 0xba512e9: /* | o Renamed 'generic' parameter Category enum to something less... */
  case 0xda3f3a8: /* | o Fixed bug creating dynamic libraries with the Linux Makefil... */
  case 0xf2b2fb2: /* | o Fixed JUCE_VST3_CAN_REPLACE_VST2 behaviour                     */
  case 0x7897331: /* | o Added support for level meter parameter categories             */
  case 0x2a98306: /* | o Fixed bug in OSX network code where Content-Type headers we... */
  case 0xfcd5a47: /* | o Added a bit-twiddling helper method: findHighestSetBit()       */
  case 0x0ebd5bc: /* | o Modified the repaint throttling to detect whether the app i... */
  case 0x0d7a77d: /* | o Improved AudioProcessorValueTreeState documentation            */
  case 0x83a4f74: /* | o Added support saving/restoring plugin bus layouts in the au... */
  case 0x101a59a: /* | o Fixed bug updating MacOS system tray icon                      */
  case 0xa6c4a02: /* | o Avoided an unused variable warning in the OSX network code     */
  case 0x4b69d0f: /* | o Fixed buffer size and sample rate changes in iOS background... */
  case 0xc52f147: /* | o Improved a variable name                                       */
  case 0x7d4d853: /* | o Fixed AAX SDK relative path bug on Windows                     */
  case 0x4c1d4f0: /* | o Added method TabbedComponent::moveTab()                        */
  case 0x2ad22dc: /* | o Added workaround to check the state of the on-screen keyboa... */
  case 0x7120d7d: /* | o Fix for HTTPS POST requests with keep-alive failing on OS X... */
  case 0x5849710: /* | o Added iOS support for BLOCKS examples                          */
  case 0x1e5865c: /* | o Restored original iOS sample rate after querying all availa... */
  case 0x25e72a7: /* | o Fixed some issues with the Linux Code::Blocks exporter         */
  case 0xf918827: /* | o Clarified some comments in the Logger class                    */
  case 0x92f3a1e: /* | o Miscellaneous fixes for some PVS static analyser warnings      */
  case 0xf3be41c: /* | o Fixed a bug in the Windows FileChooser where two backslashe... */
  case 0xd4b4780: /* | o Fixed some spelling mistakes in comments                       */
  case 0x7eb534a: /* | o Added method ValueTree::getRoot()                              */
  case 0xa15d79d: /* | o Added new methods Rectangle::withRightX() and withBottomY()    */
  case 0x2c8b98b: /* | o Added windows native code to show and hide the on-screen ke... */
  case 0xf1baf9b: /* | o Added VST hosting support for begin/endParameterChangeGestu... */
  case 0x9e0370c: /* | o Made sure the File::getNonexistentChildFile() method's putN... */
  case 0xeb07aaf: /* | o Added some TRANS macros to some internal OSX menu item strings */
  case 0x5eeaf5a: /* | o Added move semantics to AudioBuffer                            */
  case 0xaae0b15: /* | o Added begin/end iteration to HashMap                           */
  case 0xb184ca9: /* | o Removed an accidentally-committed method from String           */
  case 0xb90077c: /* | o Cleaned up some warnings in the PNG code                       */
  case 0x6f27a1d: /* | o Typo fixes                                                     */
  case 0xab0c519: /* | o Workaround for an android-specific bug in String::formatted()  */
  case 0x158bc98: /* | o Fixed an issue restoring VST3 GUI state                        */
  case 0xa7ee1a7: /* | o Added missing initialiser in Path::Iterator constructor        */
  case 0xaae6471: /* | o Minor compile fix in the javascript parser for VS2013          */
  case 0xaf07915: /* | o Fixed Linux message thread assertion in AudioProcessorValue... */
  case 0x2d96e37: /* | o Changed the default sidechain layout to stereo in the Noise... */
  case 0xdd13702: /* | o Added support for drag and drop of text on OSX, via DragAnd... */
  case 0xb1d3069: /* | o Added a new Projucer Xcode target option "Keep custom Xcode... */
  case 0x7a02a83: /* | o Make sure that the OSX SDK is new enough when compiling AUv... */
  case 0x27c353c: /* | o Fixed an issue when compiling with newer external png libra... */
  case 0x32db13e: /* | o CPP/C/CXX/LDFLAGS should be appended to JUCE_CPPFLAGS etc. ... */
  case 0xae3ee34: /* | o Fixed an issue when including an external and newer version... */
  case 0xa6d3d19: /* | o Fixed a few minor bugs in X11 windowing code                   */
  case 0xf284620: /* | o Whitespace cleanup                                             */
  case 0x3104616: /* | o Added some copy/paste options for module paths to the Proju... */
  case 0xa9b29ba: /* | o Added support for lambdas in var::NativeFunction               */
  case 0x901913a: /* | o Added new method FlexItem::withAlignSelf()                     */
  case 0xaac01b2: /* | o Fixed a couple of spelling mistakes                            */
  case 0xd90ca6c: /* | o Fixed a typo in the OpenGL demo code                           */
  case 0xddaaaa6: /* | o Removed 'const' from the Projucer's component editor's gene... */
  case 0xf85bbb2: /* | o Fixed an issue where resizing a VST2 plug-in on a HiDPI mon... */
  case 0x53ae78f: /* | o Fix for javascript parseInt of strings that start with a ze... */
  case 0xcab4569: /* | o Fixed a typo which lead the MSVC exporter to incorrectly re... */
  case 0x688110f: /* | o Fixed a littlefoot interpreter bug involving 32-bit integer... */
  case 0x8638630: /* | o Fixed unused parameter error in BluetoothMIDISelector overlay  */
  case 0x4b881b0: /* | o Added an exit callback to BluetoothMidiDevicePairingDialogu... */
  case 0x4d02823: /* | o BLOCKS API: Added a few littlefoot function definitions to ... */
  case 0x1354998: /* | o Added C++11 functions to Doxygen                               */
  case 0x21aad5e: /* | o Fixed IO channels when selecting <<none>> as OS X audio device */
  case 0x8567338: /* | o Changed the colour of module icons in the Projucer based on... */
  case 0x2e84129: /* | o Updated the juce_audio_basics, juce_audio_devices, juce_blo... */
  case 0xe91d0f9: /* | o Fixed a bug where the URL's DownloadTask would not indicate... */
  case 0xfdeea46: /* | o Fixed a bug where a downloaded file may have been incomplet... */
  case 0xe31a0b1: /* | o Resaved all projects                                           */
  case 0x8869fce: /* | o Fixed minor documentation typo in a recent PropertiesFile c... */
  case 0x806d99d: /* | o Added an option to have a properties file suffix be appende... */
  case 0x05888a0: /* | o BLOCKS API: Added some functionality (not yet supported in ... */
  case 0xe75128a: /* | o Added a second createMemoryMappedReader method to AudioForm... */
  case 0x1c4b687: /* | o Fixed a bug where the high resolution timer could hang when... */
  case 0x3fa62bc: /* | o Fixed some misnamed Doxygen parameters                         */
  case 0x9f1254a: /* | o Removed JucePlugin_{Max,Min}NumInputChannels from the VST w... */
  case 0x2f06fdf: /* | o Updated the standalone BLOCKS SDK build                        */
  case 0x38a47d4: /* | o Added a call to getToggleState() in ShapeButton::paintButto... */
  case 0xffc687a: /* | o Added TargetExt vcxproj property to MSVC                       */
  case 0x9a130f2: /* | o Set avoidReallocating argument of AudioBuffer::makeCopyOf()... */
  case 0x2952b52: /* | o Renamed instances of FileHelpers to MacFileHelpers in iOS i... */
  case 0xfb08261: /* | o Fixed VS2013 warning in File::NaturalFileComparator struct     */
  case 0x3516e45: /* | o Fixed a typo in littlefoot function argument passing           */
  case 0x8cd578b: /* | o Added the ability to set a custom BinaryData namespace in t... */
  case 0x4d48dfe: /* | o Removed a defunct comment                                      */
  case 0xe8a80ef: /* | o Added Bitwig Studio to PluginHostType                          */
  case 0xa0ed11e: /* | o Made File::NaturalFileComparator::compareElements() method ... */
  case 0xcd752ed: /* | o MidiFile::writeTo() method now returns false if unsuccessful   */
  case 0xf6727f2: /* | o Added label parameter to all AudioParameter constructors       */
  case 0x36f561a: /* | o Changed FileHelpers namespace to MacFileHelpers in juce_mac... */
  case 0xe93eee0: /* | o Added NaturalFileComparator struct to File. Reverted File::... */
  case 0xc5d3c26: /* | o Added workaround to fix a false re-save warning in the Proj... */
  case 0x1ecc686: /* | o Changed order of parameters in FlexItem::Margin constructor... */
  case 0xbd9697b: /* | o Fixed a compiler error on older gcc versions                   */
  case 0xa816a44: /* | o Disabled Ableton Live's plug-in auto-suspend if the plug-in... */
  case 0x8e768fc: /* | o Added method littlefoot::Runner::isProgramValid()              */
  case 0xd6fc589: /* | o Reverted File::compareFilenames() method to not compare fil... */
  case 0xfa7768a: /* | o Fixed Projucer live-build error with LittleFoot compiler       */
  case 0xc545b58: /* | o Added another constructor to FlexItem::Margin                  */
  case 0x17600a2: /* | o Added copy constructor and copy assignment operator to Midi... */
  case 0x8e7ed3a: /* | o Fixed MacOS 10.12 deprecations                                 */
  case 0x18c896f: /* | o Changed Line::getPointAlongLineProportionally method to wor... */
  case 0x910cab7: /* | o Added a WaveLab workaround by adding tmp buffers when WaveL... */
  case 0xb86e133: /* | o Fixed a bug where the AU/AUv3 wrapper would create a spurio... */
  case 0xa3ef455: /* | o Ignored deprecation warnings in the OS X VST3 SDK              */
  case 0x0f01433: /* | o Fixed an uninitialised variable in OnlineUnlockStatus          */
  case 0x0ee2398: /* | o Fixed compile error in recent commit on VS2015                 */
  case 0x92c219f: /* | o OSC: fixed bug where, if one OSCBundle contains multiple OS... */
  case 0x3945f1d: /* | o OSC: added unit tests for round-trip correctness (receiver ... */
  case 0x8313ba7: /* | o Added a workaround for DSP Quattro                             */
  case 0xd91b07c: /* | o Changed depcrecated use of old dragOperationStarted/Ended t... */
  case 0xb5fb095: /* | o Fixed a few VS2008 build errors                                */
  case 0x267cfb8: /* | o Fixed scoping of a conditional                                 */
  case 0x2ec8fb4: /* | o Fixed error restoring VST3 state in FL Studio                  */
  case 0xacf554a: /* | o Fixed erroneous assertion when saving VST3 states              */
  case 0x40dfba1: /* | o MPESynthesiser: Added interface to set tracking mode for pr... */
  case 0x80778a8: /* | o Fixed a couple of compiler warnings in the LittleFoot runne... */
  case 0x7bd5c05: /* | o Fixed touch event bug on Windows where touches weren't bein... */
  case 0x0df1e0b: /* | o Added sourceDetails parameter to dragOperationStarted and d... */
  case 0x2f00cb1: /* | o Tidied up ListenerList tests                                   */
  case 0x1225653: /* | o Fixed bug when setting AudioProcessorValueTreeState values ... */
  case 0x6340e54: /* | o Fixed a bug where StreamingSocket::isLocal would only retur... */
  case 0x1e2cb6f: /* | o Changed documentation for OutputStream::setNewLineString() ... */
  case 0xb517884: /* | o Fixed MSCV2015 compiler warning in VST3 wrapper                */
  case 0xc43ca07: /* | o Fixed a typo in the BLOCKS documentation                       */
  case 0x284bc70: /* | o Added a workaround for WaveLab invisible editor bug            */
  case 0x2f5b8e5: /* | o Changed File::compareFilenames() method to use String::comp... */
  case 0x27dd794: /* | o Added missing const qualifier to AudioDeviceManager::getAud... */
  case 0xda5805f: /* | o Fixed a race condition in iOS DownloadTask implementation      */
  case 0x119640f: /* | o Fixed positioning of VST3 windows in Reaper                    */
  case 0xc0987f2: /* | o Fixed a false positive in ASAN in a recent bug fix commit t... */
  case 0x57fdbd4: /* | o Fixed a potential access after free bug when creating typef... */
  case 0x60b5f72: /* | o Fixed a bug in Windows where clicking outside an applicatio... */
  case 0x7467195: /* | o Minor tidying-up                                               */
  case 0x436f047: /* | o Fixed a bug where the maximum number of channels in VST2 wa... */
  case 0x990b7b3: /* | o Fix for openGL components when global scale factors are app... */
  case 0x8154ccc: /* | o Added a fix for a potential wrap-around bug in BufferingAud... */
  case 0x61fd2a3: /* | o Fixed a MSVC 2015 warning in mp3 audio decoder                 */
  case 0x00c0671: /* | o Added sub-menu support to ComboBoxes                           */
  case 0x6641b9d: /* | o Updated BlocksMonitor example to display correct topology w... */
  case 0x8a93ddc: /* | o Fixed a typo in recent studio one parameter workaround         */
  case 0xee373af: /* | o Added AlertWindow LookAndFeel methods to offer more control... */
  case 0x388251b: /* | o Made message queue throttling more precise by using Time::g... */
  case 0xeba80c3: /* | o Tweaked buffering audio source so it can more easily be use... */
  case 0xe49022b: /* | o Added JUCE_USE_STUDIO_ONE_COMPATIBLE_PARAMETERS option to w... */
  case 0x9a7ee9f: /* | o Added support for iOS custom delegates - use at your own risk  */
  case 0xf93b174: /* | o Fixed error in variable name                                   */
  case 0x37f3481: /* | o Potentially fixed an issue with the message thread being fl... */
  case 0xcff1793: /* | o Exposed some functions when building JUCE as a DLL             */
  case 0xcaa65e8: /* | o Add new URL::downloadToFile method                             */
  case 0xfd86a73: /* | o Fixed DLL compilation of juce_tracktion_marketplace            */
  case 0xf5d3278: /* | o Projucer: avoid failing assertions from File                   */
  case 0x1349f65: /* | o Projucer: the DiagnosticReceiver interface is an implementa... */
  case 0x63a71ff: /* | o Fixed a bug where the VST3 plug-in wrapper would not respec... */
  case 0x1c2d147: /* | o Added LookAndFeel getAlertWindowButtonWidth method to overr... */
  case 0xee4fe9e: /* | o   Updated BlocksSynth example to draw waveshapes on the Lig... */
                  /* | |\                                                               */
  case 0xe384fa7: /* | | o Added Timer to MainComponent to stop touches from trigger... */
  case 0x020f858: /* | | o Doxygen updated for BlocksSynth tutorial and LittleFoot L... */
  case 0x1cc8a95: /* | | o Class comment added to top of WaveshapeProgram.h             */
  case 0x24364ec: /* | | o auto-generated comment removed from top of WaveshapeProgr... */
  case 0x15cab58: /* | | o Xcode strict warnings                                        */
  case 0x8c55f73: /* | | o MSVC warnings                                                */
  case 0xb2d0328: /* | | o Code cleanup & WaveshapeProgram documentation                */
  case 0xe5d6770: /* | | o Working LittleFoot program for waveshape drawing             */
  case 0x91e8d27: /* | | o LED drawing using LittleFoot                                 */
  case 0x39344e0: /* | | o WaveshapeProgram.h added, LittleFoot program started         */
                  /* | |/                                                               */
  case 0x2ae5d22: /* | o Resaved all projects                                           */
  case 0xb82773b: /* | o Fixed older MacOS compatibility for ListenerList tests         */
  case 0x6fb0aa4: /* | o Added a StringArray constructor to var                         */
  case 0xd438fa5: /* | o Fixed issues with AudioProcessorValueTreeState parameter sy... */
  case 0x5a2ebef: /* | o Fixed a bug where the bundle exit function would be called ... */
  case 0xc3ea4f8: /* | o Fixed a warning in MSVC 2013 in JUCE's URL class               */
  case 0x3f72a03: /* | o Fixed whitespace in last commit                                */
  case 0xc2caad4: /* | o Added missing CoInitialize when using ASIO audio device        */
  case 0x8499e29: /* | o Fixed a VS2013 warning in juce_URL                             */
  case 0x3b630f3: /* | o Added new WebInputStream class for more fine-grained contro... */
  case 0x76fa906: /* | o Added a LittleFoot section to the BLOCKS SDK documentation ... */
  case 0xec2fd97: /* | o Changed BLOCKS Doxygen stylesheet to better match developer... */
  case 0x3fb4edb: /* | o BLOCKS SDK: Fixed the order of south and west DNA ports in ... */
  case 0x58bd2b1: /* | o Added a few extra FlexItem convenience methods                 */
  case 0x63eabb4: /* | o Fixed a bug where identical initialiser lines would be removed */
  case 0xaa5d1d1: /* | o Tweaked CoreAudio to allow buffer sizes of 2048                */
  case 0xbfd5605: /* | o Added comments to specify what File::create vs. FileOutputS... */
  case 0x826fdfe: /* | o Added callback to get number of keywaves of an on-screen se... */
  case 0xde84462: /* | o BLOCKS example apps: some cleanup, enabled DUMP_TOPOLOGY flag  */
  case 0x7bb0fe4: /* | o Update copyright and make docstrings more compatible with D... */
  case 0xa85f026: /* | o BLOCKS documentation improvements                              */
  case 0x0464728: /* o | Fixed bug when maximum number of plug-in io channels is one    */
  case 0xa08cedb: /* o | Fix for HTTPS POST requests with keep-alive failing on OS X... */
  case 0x1066a6d: /* o | Added C++11 functions to Doxygen                               */
  case 0xbb56cb2: /* o | Fixed an uninitialised variable in OnlineUnlockStatus          */
  case 0x169b5ed: /* o | Update copyright and improve BLOCKS documentation              */
  case 0xf88013e: /* o |   JUCE version 4.3.0                                           */
                  /* |\ \                                                               */
                  /* | |/                                                               */
  case 0xefd4bb6: /* | o Re-saved all projects                                          */
  case 0x363ceb0: /* | o Bump version number to 4.3.0                                   */
  case 0xb5ef6d7: /* | o Whitespace clean-up                                            */
  case 0x6f6d0af: /* | o Fixed a bug where we didn't search the correct directory fo... */
  case 0xf3c313b: /* | o Added label to BlocksSynth and BlocksDrawing example windows   */
  case 0x73764c2: /* | o Fixed beforeDate in standalone BLOCKS SDK MacOS example        */
  case 0x980ece4: /* | o Projucer: add anchor #indie to the link target for the subs... */
  case 0x76b3689: /* | o Added BLOCKS module                                            */
  case 0x01380b1: /* | o Fixed a typo in the SSE 4.1/4.2 detection logic                */
  case 0x8d02f61: /* | o Projucer: add free-to-use license for the live-build engine    */
  case 0x4aa0f31: /* | o Allow to specify the stack size for each thread in a thread... */
  case 0x439ecc3: /* | o Projucer: workaround broken "Open Recent" submenu              */
  case 0x701fd8a: /* | o Added a workaround for a strange bug with monospace fonts i... */
  case 0x6711016: /* | o Fixed a bug that could make surround plug-ins fail AU valid... */
  case 0x7cdcb19: /* | o Added a convenient function to get all channel sets with a ... */
  case 0xf415b09: /* | o Enabled cookies in the juce curl backend to align the behav... */
  case 0x15bed81: /* | o Fixed a flex box bug where the first item in a list of too ... */
  case 0xae13dd6: /* | o Added an option to escape round brackets in URLs               */
  case 0x75ea6db: /* | o Renamed bounds member variable in Component to "boundsRelat... */
  case 0x5d02569: /* | o Added a method to ThreadPool to query the number of threads... */
  case 0x56c5c8e: /* | o Fixed an issue in CachedValue where setValue would not actu... */
  case 0xb36b64a: /* | o Projucer: simple pop-up menu to copy diagnostic messages to... */
  case 0x26e0aa7: /* | o Fixed a crash on quit when windows are open in the Juce Dem... */
  case 0xe435325: /* | o Fix typo and increase maximum size of the "Member initialis... */
  case 0x1845437: /* | o Only moan about all-lowercase AU manufacturer codes            */
  case 0xc4d36d1: /* | o Whitespace clean-up                                            */
  case 0x76a9b7d: /* | o Projucer: link to privacy policy in EULA and re-save jucer ... */
  case 0x23a248f: /* | o Added method VSTPluginFormat::getPluginInstanceFromVstEffec... */
  case 0xf54b7fb: /* | o Moved the DiscRecording dependency in the module descriptio... */
  case 0xeac52c9: /* | o Fixed a bug in the projucer that would create incorrect pos... */
  case 0x288e56d: /* | o Fixed a bug in the Projucer that would incorrectly mark rel... */
  case 0x5e0efc6: /* | o Projucer: extend interface DiagnosticMessage::handleRecover... */
  case 0xa931b47: /* | o Added a non-sense if statement to BufferingAudioSource to s... */
  case 0x3e7b599: /* | o Moved audio CD burner/reader to juce_audio_utils               */
  case 0xbd3166a: /* | o Updated the URL of the "Forgot Password" button in the Proj... */
  case 0x348dc1f: /* | o Fixed a bug where the Projucer would delete rsrc files in y... */
  case 0xc587d4b: /* | o Minor code style tidy-ups                                      */
  case 0x7d07f51: /* | o Added missing juce_osc dependency on juce_events               */
  case 0xa5e3b7a: /* | o Added check and warning when entering only lowercase AU plu... */
  case 0x84a16f6: /* | o Fixed Projucer to check for the correct file when looking f... */
  case 0x6c52bf5: /* | o Changed the way windows modifiers are probed in an attempt ... */
  case 0x255dc1d: /* | o Fixed documentation typo                                       */
  case 0x0bcc53c: /* | o Removed juce_audio_formats header from juce_audio_devices      */
  case 0xff52d76: /* | o Fixed a bug in DirectoryIterator where the result would be ... */
  case 0xa347689: /* | o Moved simple sound player to audio_utils module                */
  case 0x1fcae36: /* | o Changed flac writer to respect the current stream position ... */
  case 0x29bed6c: /* | o Fixed warnings in cryptographic unit test code                 */
  case 0xc6a75d0: /* | o Changed the way windows modifiers are probed in an attempt ... */
  case 0x2a37ba0: /* | o Added gradle-wrapper when saving projects so that it is now... */
  case 0x676ad43: /* | o Added more compare operators to Identifier class               */
  case 0x9e4741f: /* | o Fixed an AAX bug where the sidechain buffer was not cleared... */
  case 0x14957e7: /* | o Avoided some FLAC debug output being printed if your app de... */
  case 0x62ac568: /* | o Made sure that colours are updated when the look and feel o... */
  case 0x57742a5: /* | o Fixed warning of MSVC compilers in latest commit               */
  case 0x852fe34: /* | o Added support for encrypting/decrypting blocks of data with... */
  case 0xcdb7285: /* | o Fixed AAX post build script to consider SDK paths containin... */
  case 0x285635c: /* | o Added notch and all pass filters                               */
  case 0x9d08f8f: /* | o Fixed a documentation typo.                                    */
  case 0x4a6473a: /* | o Removed the isLocked() method from ReadWriteLock               */
  case 0xf301ed7: /* | o Added an isLocked() method to ReadWriteLock that checks whe... */
  case 0xe03e41b: /* | o Projucer: hard shutdown of the compiler process in release ... */
  case 0xbacf194: /* | o Small tweak to the bluetooth iOS menu to make it as wide as... */
  case 0x773e708: /* | o Removed unecessary path seperators in MSVC post-build scripts  */
  case 0x3bd7ef4: /* | o Projucer: menu adjustments                                     */
  case 0x0ed9003: /* | o Fixed bundle ref initialisation of statically linked VSTs o... */
  case 0x13c501d: /* | o Removed unnecessary memory allocation when assigning a big ... */
  case 0xf374eb6: /* | o Fixed SortedSet::remove() to return the removed element        */
  case 0xc1e2e6a: /* | o Fixed size of bluetooth midi pairing dialogue to accommodat... */
  case 0x8f1d37a: /* | o Fix addAllModulesInFolder to scan only 2 subfolder levels      */
  case 0x46b38d7: /* | o Removed code which is not used anymore                         */
  case 0xedd828a: /* | o Fix memory leak in BluetoothMidiSelectorOverlay                */
  case 0x13c4946: /* | o Changed lowResamplingQuality to kCGInterpolationNone on OS ... */
  case 0x6c39897: /* | o Added 'inclusive' and 'exclusive' to documentation for Rand... */
  case 0x8263d70: /* | o Continue drawing if a negative dashLen was hit                 */
  case 0x7686db6: /* | o Fixed a typo in my latest commit                               */
  case 0x59a47d1: /* | o Fixed possible dead-lock                                       */
  case 0xe8cdc65: /* | o Fixed Array::remove (ElementType*) so that if given an inva... */
  case 0xa440c16: /* | o Added Graphics::ResamplingQuality::noResampling                */
  case 0x0121a02: /* | o Added optional parameter to restore audio settings state in... */
  case 0x321ca8c: /* | o Fixed AUv3_Wrapper include path                                */
  case 0xb24aadc: /* | o Added ColourIds to allow alternate colouring of TreeViewItems  */
  case 0xd892109: /* | o Fixed a warning in recent commit                               */
  case 0x40994fc: /* | o Added checks to make sure that the current interface orient... */
  case 0x848073d: /* | o Always remove silence flag in AU render callback               */
  case 0x27be047: /* | o Allow disabling the main bus in VST3 plug-ins with the revi... */
  case 0x225e8da: /* | o Added waitForNextAudioBlockReady method to BufferingAudioSo... */
  case 0x2104c50: /* | o Added const qualifier to MidiEventHolder argument of MidiMe... */
  case 0x2785059: /* | o Restored non-flushing behaviour of FileOutputStream            */
  case 0x848b400: /* | o Avoided warning in release builds in AudioUnit hosting code    */
  case 0xbf85f4c: /* | o Cache the result of isRunningInAppExtensionSandbox             */
  case 0x495e2bf: /* | o Fixed file flushing for FileOutputStream on Windows            */
  case 0x728bf27: /* | o Projucer: support recoverable error warnings for live-builds   */
  case 0xd19ba05: /* | o Added support for saving audio/midi setting in standalone w... */
  case 0x80ff164: /* | o Added preprocessor macro JucePlugin_VSTChunkStructureVersio... */
  case 0x83d0854: /* | o Added a microphone permission option to the iOS exporter       */
  case 0xfc1214e: /* | o Fixed ambiguous sign in AudioThumbnail implementation          */
  case 0x3fb55ff: /* | o Fixed a compiler error which occured in gcc based compilers    */
  case 0x71de185: /* | o Fixed typo in linux-only Projucer code                         */
  case 0xc98f71d: /* | o Fix Process::setDockIconVisible() may not turn the process ... */
  case 0x21c6fc7: /* | o Added an option to enable/disable pre-buffering in Bufferin... */
  case 0x4b96ffb: /* | o Projucer: restrict menu item "Launch Application" to GUIApp... */
  case 0xa0350e4: /* | o Projucer: add feature download & install live-build engine ... */
  case 0x42b8156: /* | o Fixed crashes in old 32-bit Carbon plugin editor windows wh... */
  case 0xe14895e: /* | o Added macOS version 12 to SystemStats                          */
  case 0xaff5ea5: /* | o Replaced two instances of src[1] with src[0] in the render4... */
  case 0x6f4571b: /* | o Tweaked VST wrapper so that MIDI effect plug-ins work even ... */
  case 0xef50b8d: /* | o Fixed trying to write to invalid input buffer when AUs conn... */
  case 0x679ef3f: /* | o Fixed a bug where VST plug-in editors using OpenGL would no... */
  case 0x4f06ff9: /* | o Third time lucky: hopefully fixed a white-noise issue when ... */
  case 0xc14eb04: /* | o Fixed automation bug when switching between presets in VST ... */
  case 0x5a365ed: /* | o Removed assertion which gets hit by too many VST plug-ins      */
  case 0x8835a24: /* | o Fixed another typo and removed unecessary for loop             */
  case 0x1be76c1: /* | o Added more methods to IIRCoefficients to generate coeffecie... */
  case 0xcbbf76b: /* | o Made sure buffers are prepared before clearing them in the ... */
  case 0xb0457a9: /* | o Fixed a typo in the VST wrapper                                */
  case 0x1b217d1: /* | o Zero out buffers if pulling audio returns an error in the A... */
  case 0x4fa0516: /* | o Revised multibus API and added support for multibus hosting    */
  case 0x680d758: /* o |   JUCE version 4.2.4                                           */
                  /* |\ \                                                               */
                  /* | |/                                                               */
  case 0xebf19aa: /* | o Re-save all projects                                           */
  case 0x4eb8933: /* | o Bump version number to 4.2.4                                   */
  case 0x05b15c4: /* | o If the jucer file does not specify a macOS/iOS deployment t... */
  case 0x0862167: /* | o Fixed a compiler warning for MIDI effect plug-ins              */
  case 0x0c87af8: /* | o Fixed Linux compile error in Projucer                          */
  case 0xacefbcd: /* | o Fixed a typo that was preventing some config settings showi... */
  case 0x9fa0d49: /* | o Added an option JUCE_ALLOW_STATIC_NULL_VARIABLES that can b... */
  case 0xd03755c: /* | o Fixed a potential memory leak                                  */
  case 0x48ac634: /* | o Fixed the MSVC exporter to combine multiple internal post-b... */
  case 0x231a6e4: /* | o Fix some warnings in MSVC                                      */
  case 0x8ec9443: /* | o Projucer various fixes                                         */
  case 0xe35aba3: /* | o Added a standard iterator to NamedValueSet                     */
  case 0x2fd331f: /* | o Modified compile-time error to warn the user that juce_audi... */
  case 0xcbb8536: /* | o Fixed potential crash in mac's legacy WebInputStream implem... */
  case 0xceefc5b: /* | o Avoided a dead-lock in a recent commit to WebInputStream on... */
  case 0x6d56e48: /* | o Add support for retina mouse cursors on OS X                   */
  case 0x5f748bb: /* | o Added a method to get the internal viewport of a PropertyPanel */
  case 0xc67c1d4: /* | o Added missing JUCE_API keywords                                */
  case 0xff40b46: /* | o Added support for automatically creating AAX plugin bundles... */
  case 0xf37787d: /* | o Fixed incorrect comparison in right shift operator of BigIn... */
  case 0xb94a68c: /* | o Array: added new method removeIf to remove objects based on... */
  case 0xcf8d655: /* | o Fixed a possible race condition in mac's implementation of ... */
  case 0x9177a94: /* | o Added a check in Button to detect self-deletion as a side-e... */
  case 0x1f2eaeb: /* | o Added support for accessing the properties of a javascript ... */
  case 0x913a868: /* | o Fixed a crash on exit error in the plug-in host on windows/... */
  case 0x16525c0: /* | o Fixed some typos.                                              */
  case 0xd73f776: /* | o Fixed a bug where FlexBox would still be compiled on older ... */
  case 0x004f829: /* | o Reverted #bea45f84 - this only fixed SVG positioning bugs f... */
  case 0x9276540: /* | o Fix to avoid negative width assertion when drawing fitted t... */
  case 0x56423ae: /* | o Changed Windows windowing so that the resizeStart() and res... */
  case 0xed0c032: /* | o Added a styleChanged flag to enable setText() to take effec... */
  case 0x29320a0: /* | o Changed maxSensibleMidiFileSize in MidiFile from 2mb to 200... */
  case 0xbea45f8: /* | o Fixed SVG child-transform rendering bug.                       */
  case 0xe74849c: /* | o Fixed a missing call to lf.preparePopupMenuWindow in PopupMenu */
  case 0x151e3f7: /* | o Added a LookAndFeel method preparePopupMenuWindow() to allo... */
  case 0x132c1d6: /* | o Removed "C" after all mono channel names                       */
  case 0x1a28e7a: /* | o Fixed modulo zero bug in VST bus arrangement                   */
  case 0x821dea8: /* | o Fixed implicit conversion warnings in BigInteger               */
  case 0xc94be5d: /* | o Fixed an issue where repaints could be dropped when the hos... */
  case 0xf257536: /* | o Fixed #100: If base sdk version is newer than 10.12 then no... */
  case 0x16e6c6f: /* | o Slider and NormalisableRange changed to use standard librar... */
  case 0xc10b042: /* | o Fixed a potential leak in ScopedPointer                        */
  case 0x70530a9: /* | o Added beginParameterChange() and endParameterChange() metho... */
  case 0x19d654d: /* | o Added symmetric skew option to NormalisableRange class. Add... */
  case 0x5c46c65: /* | o Changed the documentation for AudioProcessorValueTreeState:... */
  case 0xf0555e6: /* | o Fixed JucePlugin_IsSynth compiler warning.                     */
  case 0x7be711e: /* | o Added some functions writeLittleEndianBitsInBuffer() and re... */
  case 0xa9d8434: /* | o Resaved all projects                                           */
  case 0xc0c912a: /* | o Modified BigInteger so that small (128-bit) values don't re... */
  case 0x28bb28a: /* | o Fixed a trailing "\" bug in the VS intermediates path          */
  case 0xc23e95d: /* | o Defined HAVE_LROUND for non-MSVC compilers when building FL... */
  case 0x979d3b6: /* | o Fixed crash on internal changes within aggregate audio devi... */
  case 0x17dee9a: /* | o Restore depecated VST opcodes for CarbonWrapperComponents      */
  case 0xddb8796: /* | o Fixed VSTs erroneously wanting midi input                      */
  case 0x1ddd394: /* | o Restore missing VST opcode.                                    */
  case 0xd873d24: /* | o Fix some Xcode warnings in the VST and VST3 wrappers.          */
  case 0xcb6b4b7: /* | o Remove VST editor idle                                         */
  case 0x881dfce: /* | o Fix deadlock in handleRouteChange for iOS audio.               */
  case 0x6ba384f: /* | o Fixed some missing definitions in the VST hosting for Carbo... */
  case 0x7fad254: /* | o Added output level metering to AudioDeviceManager              */
  case 0x1942e3d: /* | o Added begin/end iterator methods for ValueTree, for handy r... */
  case 0xea76779: /* | o modified sliderValueChanged() to notify host only on mouse ... */
  case 0x47c48b2: /* | o Changed Projucer LoginForm "Remember login" button to tick box */
  case 0x9f31d64: /* | o Use a bespoke VST2 interface                                   */
    if (feature == Feature::hasVst2Interface)
      return true;
  case 0x1e9af22: /* | o modified valueChanged() to notify host only on mouse clicks.   */
  case 0x08b2116: /* | o Make the mousemove behaviour of OS X plug-ins the same as t... */
  case 0x48db4fa: /* | o Fix for the copy constructor of DrawableRectangle              */
  case 0x55194a0: /* | o On Windows, suppressed the behaviour that pressing the ALT ... */
  case 0xa46191a: /* | o Native OSX menu bar: avoided some duplicated menu bar flash... */
  case 0xb1ab450: /* | o Avoided a warning in some OSX network code                     */
  case 0xc353ac3: /* | o Avoided a race condition when cancelling HTTP stream on OSX    */
  case 0xc7db4bb: /* | o Avoided a compiler warning on OS X due to unused private fi... */
  case 0x773c963: /* | o Whitespace.                                                    */
  case 0xe2dc460: /* | o Made the Desktop class clear any pending animations before ... */
  case 0x6547452: /* | o Fixed a leak of bubbleMessage object in juce demo              */
  case 0x91f2453: /* | o Fixed a bug in UndoManager when using undoCurrentTransactio... */
  case 0x2417676: /* | o Increased buffer size for names read by the VST host, and c... */
  case 0x3f83eed: /* | o Minor tidying up                                               */
  case 0xe9832ff: /* | o Added a symmetric skew option to Slider                        */
  case 0x79dd695: /* | o added ScopedLock to iOSAudioIODevice::handleRouteChange met... */
  case 0x8e10f16: /* | o Dialog window to ask whether to keep current project or re-... */
  case 0x1470928: /* | o Use effGetProductString when getting the name of a VST-2 pl... */
  case 0x82224d7: /* | o Avoided a couple of warnings in iOS builds with older SDKs     */
  case 0x999f484: /* | o Stopped the ColourSelector sending a change message from it... */
  case 0x3104cbe: /* | o Added an optional notification argument to ColourSelector::... */
  case 0xcbdf707: /* | o Windows touch with CallOutBox bug fix                          */
  case 0xc7b3472: /* | o Partially undo commits #99186e5 and #89d938d and use less-i... */
  case 0x8f7d9f7: /* | o Workaround to avoid an assertion when using File::invokedEx... */
  case 0x59cc979: /* | o Deallocate all nodes in the graph before calling JUCEApplic... */
  case 0x8994f37: /* | o Added some FlexBox layout classes, and a demo page for this... */
  case 0xaa1acb3: /* | o Avoid unnecessary calls to AudioUnitInitialize.                */
  case 0x0159102: /* | o Fix ResamplingQuality documentation                            */
  case 0xb936786: /* | o Remove the recently added shouldReleaseFocusOnMainMenuBarAc... */
  case 0xec9c033: /* | o Add exclusive flag to MemoryMappedFile                         */
  case 0x89d938d: /* | o Make sure to not remove a native component twice               */
  case 0x99186e5: /* | o Call removeFromDesktop when an external host tries to remov... */
  case 0x1631dac: /* | o whitespace                                                     */
  case 0x9062ad9: /* | o Windows 10 update touch events bug fix                         */
  case 0xbc17cb9: /* | o Changed UndoManager so that if a transaction is aborted wit... */
  case 0xaa15039: /* | o Automatically connect midi input devices on mobile in stand... */
  case 0x5ba1723: /* | o Removed the juce_tracktion_marketplace module's dependency ... */
  case 0x3d1b2d6: /* | o Added a cast to some VST code to avoid a C++14 warning         */
  case 0x1a1897b: /* | o Made the OSX HTTP stream respond to the return value of the... */
  case 0x675f594: /* | o fixed silly typo                                               */
  case 0xf0a00bf: /* | o AlertWindow::show() only sets window to always on top if th... */
  case 0x49b9619: /* | o Added a cast to avoid a warning in Array                       */
  case 0xbc77b00: /* | o MIDI network session enabled for iOS simulator                 */
  case 0x17a07a0: /* | o Add a flag to release the focus of a component when the use... */
  case 0xb67b1dd: /* | o Add an issue template for GitHub.                              */
  case 0x7a9c22e: /* | o AlertWindow::show() method sets AlertWindow to be always on... */
  case 0xa744cd3: /* | o Added a bool handler for JUCE_LIVE_CONSTANT                    */
  case 0xc217164: /* | o Add low quality mode for juce CameraDevice                     */
  case 0x26b6f01: /* | o Replace all "zeros" with "nullptr" in code documentation wh... */
  case 0xbf582f6: /* | o Fix errors and warnings on mingw-w64                           */
  case 0x1e41898: /* | o Fix a memory leak in linux ALSA code for MidiInput/MidiOutput  */
  case 0x1053dc5: /* | o Allow having zero programs in VST                              */
  case 0x4cf3c02: /* | o Send parameter changes to processor when received via the E... */
  case 0x3d1dc4a: /* | o Fix bug where previously applied context transformation was... */
  case 0xe6ad9b5: /* | o Add option to not re-allocate memory when making a copy of ... */
  case 0xb50f941: /* | o First connect controller and component and then load the st... */
  case 0xa258295: /* | o Don't repeatedly set the wantsFocus flag if it hasn't changed  */
  case 0x962cfc7: /* | o Clarify what happens when maxNumChars is zero in the TextPr... */
  case 0x9100d1c: /* | o Synthesiser and MPESynthesiser: added an additional paramet... */
  case 0x6fdb7e3: /* | o Also add assembler files to MSVC projects                      */
  case 0x7287830: /* | o Fix AUv3 OS X version compiler error message                   */
  case 0xf5398fc: /* | o Added additional dontScrollToShowThisRange parameter to sel... */
  case 0x9e1edc7: /* | o Add support for hosting staticly compiled VSTs on all platf... */
  case 0x1696c23: /* | o Add support for hosting staticly linked VSTs                   */
  case 0x88736e3: /* | o Make splash screens fullscreen on mobile platforms             */
  case 0x65e1b60: /* | o Add missing overrides in FileTreeComponent                     */
  case 0x4b64584: /* | o Fix small typo                                                 */
  case 0x56d5f7b: /* | o Don't update the rendering graph if the graph is not prepared  */
  case 0x8b1374d: /* | o Don't call processBlock on a AudioProcessorGraph's node if ... */
  case 0xcdf899c: /* | o Don't keep on showing the auto-updater dialog box if the us... */
  case 0x035f865: /* | o Fix steppy scrolling if the content component of a viewport... */
  case 0x142ce5e: /* | o Fix a typo in the previous commit                              */
  case 0x69cd509: /* | o Add missing getNumSteps override to AudioProcessorValueTree... */
  case 0x88803f6: /* | o Don't call methods that require modal loops in Standalone f... */
  case 0xe5f306e: /* | o Fix bad property id when registering AU event listener         */
  case 0xca62e56: /* | o Revert support for hosting your own VST bundles on iOS         */
  case 0xa8a7fa2: /* | o Added a missing newline in the juce_tracktion_marketplace k... */
  case 0x4998930: /* | o Added expiry time support to the juce_tracktion_marketplace... */
  case 0x05835ec: /* | o Generate bitcode bundles on iOS                                */
  case 0x9e6988a: /* | o For drag-and-drop on win32, prioritised file-drag mode over... */
  case 0x97e058b: /* | o Added a couple of proportional Rectangle methods               */
  case 0xd493311: /* | o Only reopen stdout, stderr, stdin if they haven't already b... */
  case 0x70f4b0d: /* | o Add more details to the documentation of the setResizable m... */
  case 0x8b1b855: /* | o Throttle repaints when repainting in a plug-in to avoid ste... */
  case 0x6b8c720: /* | o Tidied up a few things in the Line class, and added a Line:... */
  case 0xa604c03: /* | o Added a definition that's missing in some very obscure win3... */
  case 0xebb60c7: /* | o Add missing new line at end of file                            */
  case 0x6039a19: /* | o Add new line to end of juce_AAX_Modifier_Injector.h            */
  case 0x7600723: /* | o Correctly account for midi ports in Alsa when listing/openi... */
  case 0x872f84d: /* | o Check for hash collisions when generating integer parameter... */
  case 0x5e9c256: /* | o Label attribute of AudioProcessorParamater should be public    */
  case 0xab3e628: /* | o Added mouse-up event pressure handling                         */
  case 0xa050e07: /* | o Fix incorrect parameter id <-> index conversion when hostin... */
  case 0x3ff5ed0: /* | o Reject any inconsistent VST-2 channel layout requests          */
  case 0x3791939: /* | o Fix unused warning in AU plugin client release build           */
  case 0x437cb4e: /* | o Only detect which corner is being resized on the first live... */
  case 0x0df194d: /* | o Added method URL::withNewDomainAndPath()                       */
  case 0xc6249d2: /* | o Clear http headers after redirects                             */
  case 0xb439452: /* | o Changed the Array::remove() method to return void, and adde... */
  case 0xdd245ef: /* | o Have curl follow re-directions automatically                   */
  case 0x332dcac: /* | o Added a method Range::expanded()                               */
  case 0xc9aca28: /* | o Add pkg-config support for linux                               */
  case 0xf83554c: /* | o Add a User-Agent http header to curl requests                  */
  case 0xc552872: /* | o Never define JUCE_SUPPORT_CARBON on iOS                        */
  case 0x01c73ff: /* | o Optimisation for look and feel drawing of zero-size title bars */
  case 0xa700774: /* | o Tidied up a comment                                            */
  case 0xed67e40: /* | o Added a customisable callback object to PopupMenu              */
  case 0x0a4b2ae: /* | o Return parent row number if any of the tree view item's par... */
  case 0x3592d61: /* o | Add an issue template for GitHub.                              */
  case 0x572868f: /* o | Fix incorrect parameter id <-> index conversion when hostin... */
  case 0x41ab0d9: /* o | Never define JUCE_SUPPORT_CARBON on iOS                        */
                  /* |/                                                                 */
  case 0x630ab88: /* o Version number update                                            */
  case 0xb6d78f8: /* o Avoided an analyser warning on iOS                               */
  case 0x89dade2: /* o Fixed a compile problem with the GNU c++ library                 */
  case 0xe7c9cf3: /* o Made iOS build use updated HTTP API, and turned off deprecati... */
  case 0x2f36d03: /* o Do a case-insensitive compare on core http headers in windows... */
  case 0xb729764: /* o Fix a bug in URL::findEndOfScheme                                */
  case 0x6fc1242: /* o Added some constructors to ReferenceCountedObjectPtr and Scop... */
  case 0x38c897f: /* o Return nullptr from URL::createInputStream if a request times... */
  case 0x2ca0c92: /* o Add beginParameterChangeGesture/endParameterChangeGesture sup... */
  case 0xf7f35f5: /* o Add fallback to legacy networking code for pre OS X 10.10        */
  case 0x3461b12: /* o Draw custom line ends if arrow width is zero                     */
  case 0x300485a: /* o Added a Javascript array indexOf method                          */
  case 0x31f935c: /* o Added a new Graphics::drawImage method that takes a Rectangle... */
  case 0x3969000: /* o Avoid a compile error due to steinberg renaming a constant in... */
  case 0x14a9368: /* o AudioDeviceManager::initialiseFromXML(): made the default buf... */
  case 0xe4592d1: /* o Projucer: tweak to the way GCC -D flags are generated            */
  case 0x8a9fbc3: /* o Resaved some project files                                       */
  case 0x5960afd: /* o Cleaned up a few functions in Graphics, and optimised drawEll... */
  case 0x6e0c168: /* o Removed a defunct workaround for an obscure bug in old Window... */
  case 0xd29608a: /* o Minor documentation corrections.                                 */
  case 0xb46be0f: /* o Change projucer version to match JUCE version                    */
  case 0xd9a3e76: /* o Fix a warning in the previous commit                             */
  case 0xf6cd130: /* o Add redirect support to NSURLSession code                        */
  case 0x1a40df9: /* o Revert commit d0c238d: Use getParameterText to respond to AU'... */
  case 0xf7d7f8a: /* o Clear unused outputs when bypassing a plug-in                    */
  case 0x943c89d: /* o Added some extra locking in the destructor of a Mac HTTP stre... */
  case 0x84810b7: /* o Fixed some code that used a C++11 function                       */
  case 0x1861c82: /* o Add window property to JuceAppStartupDelegate                    */
  case 0x2be34b3: /* o Added some linux build files to the .gitignore                   */
  case 0x37cffb7: /* o Added a Javascript array splice method                           */
  case 0x16913f2: /* o Added a unit test                                                */
  case 0x69a7506: /* o Made File remove any redundant "/./" subfolders of paths that... */
  case 0xd0c238d: /* o Use getParameterText to respond to AU's kAudioUnitProperty_Pa... */
  case 0x4097fab: /* o Fixed a silly typo!                                              */
  case 0x021209e: /* o Changed ImagePixelData::clone to return a smart pointer rathe... */
  case 0xf8516f2: /* o Refactored the OSX/iOS HTTP streaming code to use the newer N... */
  case 0x95c66f5: /* o Make sure timer is stopped before releasing a VST plugin         */
  case 0xa33abe6: /* o Added method VSTPluginFormat::createCustomVSTFromMainCall() f... */
  case 0x8b1438a: /* o Made PopupMenu avoid overwriting a custom shortcut key descri... */
  case 0x329e54d: /* o Replace std::move with static_cast to fix errors in old compi... */
  case 0x58f2e1b: /* o Add support for isCurrentlyModal to detect if the current com... */
  case 0x1f6fa5c: /* o Re-save all projects                                             */
  case 0xb1a7441: /* o Align manufacturer code of sample plug-ins in JUCE repo ('ROLI') */
  case 0x70ed6eb: /* o Add original four-char code as comment next to hex literal in... */
  case 0xadfc6e9: /* o Re-save projects                                                 */
  case 0xb6f4d35: /* o Avoid char literals in auto-generated AppConfig.h files          */
  case 0x042bd2b: /* o Fix identifying the scheme portion of a URL                      */
  case 0x72083c1: /* o Do not represent the numpad divide key as a slash key character  */
  case 0x16fde67: /* o Allow write access to all files if a user has effective root ... */
  case 0x074c909: /* o Fix minor typo in OS X function key mappings                     */
  case 0xbb56ba4: /* o Add CRLF between JUCE http headers and user http headers         */
  case 0xcc6f0a1: /* o Replace CR and CRLF endings with unix (LF) style endings in x... */
  case 0x4eec614: /* o Remove default keywords from MidiMessageSequence to be compat... */
  case 0x320c5b6: /* o Add removeChannel to AudioChannelSet                             */
  case 0x5e0d3fd: /* o Add possibility for an InterprocessConnectionServer to bind t... */
  case 0x6810791: /* o Make sure that the legacy AU rez file is correct for midi eff... */
  case 0x54e2351: /* o Fix compile error in AAX wrapper                                 */
  case 0x877d834: /* o Fix compiler error on VS2015 in recent commit                    */
  case 0x9ecc1e9: /* o Fix an issue when hosting Waves plug-ins                         */
  case 0xa0259ce: /* o Add support for plug-ins which do not want to use a resize co... */
  case 0xc484a9e: /* o Fix some minor warnings on OS X 32-bit                           */
  case 0x3ddd2db: /* o Allow access to all URLs in the JUCE Demo                        */
  case 0x8cd6f49: /* o Add program change parameter to VST-3 plug-ins                   */
  case 0xfc69dc0: /* o Changed addIfNotAlreadyThere method of all array classes to r... */
  case 0x9481773: /* o Added *.xcscmblueprint to gitignore (a type of file generated... */
  case 0x1a5f71b: /* o Added some logic to MidiDataConcatenator that avoids assertin... */
  case 0xec40f09: /* o Updated version numbers in modules, and resaved example projects */
  case 0xf58eda8: /* o Added 32K as a supported ASIO sample rate                        */
  case 0xe993eb7: /* o Whitespace.                                                      */
  case 0x920c19d: /* o UnitTest: added new functions expectNotEquals, expectGreaterT... */
  case 0x58ca067: /* o Avoided spacer components in a ToolBar getting keyboard focus    */
  case 0x7abbf67: /* o Tweaked the iOS audio device to make the default buffer size ... */
  case 0x61e71eb: /* o Add incremental linking option for Visual Studio exporters       */
  case 0xe8da23d: /* o Enable live comilation for Windows                               */
  case 0xb841406: /* o Fixed a compiler warning                                         */
  case 0xb4e1e68: /* o UnitTest: added method expectWithinAbsoluteError for floating... */
  case 0x74554a2: /* o Improved performance of some BigInteger methods by adding Mon... */
  case 0x565d664: /* o Fixed a typo in a comment                                        */
  case 0xdb7ada5: /* o Avoided compile problems when the JUCE_USE_CUSTOM_AU3_STANDAL... */
  case 0xdcf9246: /* o Reduced the footprint of MidiMessage, and added some move ope... */
  case 0x23c361a: /* o Fixed a minor compiler warning                                   */
  case 0x3852771: /* o Always send VST3 editor parameter changes to the VST3 processor  */
  case 0x93b8fc1: /* o Reverted MPEZone: added copy constructor and copy assignment ... */
  case 0x42a3145: /* o MPEZone: added copy constructor and copy assignment operator.    */
  case 0xc807cbc: /* o MPEZone: cleaned up and clarified doxygen comments.              */
  case 0xcf39ad4: /* o Fix multiple compile errors on older gcc versions                */
  case 0x8c99b63: /* o Fixed a warning and a wrong indentation in MidiMessage.          */
  case 0x0141d0e: /* o Connecting an output node to a processor is an input and vice... */
  case 0x07bc151: /* o MidiMessage: moved two useful helper functions into the publi... */
  case 0x968cee4: /* o Remove unnecessary code after moving to static shared code li... */
  case 0x1635f47: /* o Modify buttonColourId and buttonOnColourId to be different co... */
  case 0xa714feb: /* o Fix incorrect number of output buses in standalone plug-in cl... */
  case 0x0a040c6: /* o Update avtive ports when opening JACK audio device               */
  case 0x0ea9e33: /* o Enable subclassing of StandaloneFilterApp                        */
  case 0xf5391cd: /* o Only report one midi in & out bus in VST3 wrapper                */
  case 0x437b5c2: /* o Third time lucky?                                                */
  case 0xb003543: /* o Fix typo in last commit                                          */
  case 0xc5a7fe6: /* o Add additional checks to enable/disable AUv3 support             */
  case 0x9069d70: /* o Fix bug in AudioGraphIOProcessor                                 */
  case 0xfe14210: /* o Fix incorrect order of includes in VST_wrapper.mm                */
  case 0x749bffe: /* o Add missing ifdefs from recent commit                            */
  case 0xf11a2b2: /* o Fix modifier keys in AAX plug-ins on Windows 10                  */
  case 0x54243ef: /* o Support hosting your own VST bundles on iOS                      */
  case 0xa3c377b: /* o Move kAudioUnitProperty_MaximumFramesPerSlice setting/query a... */
  case 0x8df262a: /* o Don't use placement new for MemoryBlock assignment operator i... */
  case 0x0d52caf: /* o Make sure AAX wrapper calls releaseResources                     */
  case 0x24a63ae: /* o If ChildProcess is not interested in stdout/stderr, then redi... */
  case 0xbda4b78: /* o Add missing assignment operator to var class                     */
  case 0x4e91fd7: /* o Add an extra byte when copying VST2 strings                      */
  case 0xac9973f: /* o Add support for creating popup menus inside parent components    */
  case 0x77a8b9e: /* o Fix a few warnings                                               */
  case 0x8e53286: /* o Fix some warnings in VS2013 in the recent commits                */
  case 0x4205123: /* o Fix an edge-case if the last stored editor window size is zero   */
  case 0xebb0a15: /* o Add options to make AuidoProcessorEditors resizable with cons... */
  case 0x25f8faf: /* o Add windowIgnoresMouseClicks flag to tooltip windows             */
  case 0xee7a56a: /* o Fix potential crash if DAW probes non-existent bus               */
  case 0xb3ee880: /* o Set kAudioUnitProperty_MaximumFramesPerSlice on iOS              */
  case 0x2c3783e: /* o Workaround for loading Izotope Plug-Ins                          */
  case 0x368ce48: /* o Fix a race condition in the plugin scanner                       */
  case 0x4c900be: /* o Added some init code to enable stdout, stderr, stdin in the s... */
  case 0xc9c6aa0: /* o   Merge branch 'master' into develop                             */
                  /* |\                                                                 */
  case 0xfe37653: /* | o Fixed JUCE_BUILDNUMBER which was not correctly updated to 4... */
  case 0x1bd850d: /* o | Minor tidy up of yesterday's VST3 preset commit                */
  case 0x7eb80b8: /* o | Ensure that AudioUnitUninitialize is called on the message ... */
  case 0x5098232: /* o | Tidy up a few warning                                          */
  case 0x993a2e4: /* o | Do not allow specifying the JUCE folder as a modules folder    */
  case 0xf4086ad: /* o | Support factory programs in VST3 hosting                       */
  case 0x93a41cf: /* o | Call setProcessing (false) when resting a VST3 plug-in         */
  case 0xf0dd881: /* o | Fixed a typo in the projucer's button colour handling          */
  case 0xf5324af: /* o | Added some defenses against parsing corrupted metadata bloc... */
  case 0x1a973a2: /* o | Ignore linux scale factor if it is ten or larger               */
  case 0xfc8c628: /* o | Add more detailed comment on why getting the "getPluginFact... */
  case 0xe12c801: /* o | Always add build scripts - even for Aggregate targets          */
  case 0x4827003: /* o | Made Component::internalFocusLoss pass on the focus-change ... */
  case 0xe13286a: /* o | Minor comment correction                                       */
  case 0x3aee68e: /* o | Added a method to create a ModalCallbackFunction from a lam... */
  case 0x12eeade: /* o | Added some missing 'override's                                 */
  case 0x79dca43: /* o | Workaround for VST-3 plug-ins which do not change their par... */
  case 0x0d96a8b: /* o | Avoided some false negatives in the JSON unit tests            */
  case 0x6b98901: /* o | Fix link errors when build VST plug-ins with VST 3 hosting ... */
  case 0x2c3a2ea: /* o | Simplified some demo plugin code to avoid a display update ... */
  case 0x09d6409: /* o | Refactored some demo code to avoid a problem when using Str... */
  case 0xdbf9156: /* o | Fix RTAS support                                               */
  case 0x6b6dedb: /* o | Add some minor changes to stay compatible with GCC 4.4         */
  case 0x7056128: /* o | Add intersect method to AudioChannelSet                        */
  case 0xaf6198b: /* o | Add support for plug-ins to report the IDs of their parameters */
                  /* |/                                                                 */
  case 0x2c7a0e0: /* o Fixed a wrong preprocessor macro which caused plugin hosting ... */
  case 0xa0835a5: /* o Added a parseFloat method to the Javascript parser               */
  case 0xf2bca43: /* o Fix AUv3 version check for iOS                                   */
  case 0xf36205f: /* o Fixed a couple of comment typos                                  */
  case 0xe602dd6: /* o Improved some comments for MemoryBlock base-64 methods           */
  case 0xd1030fa: /* o Projucer: added some missing 'override' specifiers               */
  case 0x688ac3b: /* o Updated a comment                                                */
  case 0x340f4ce: /* o Android exporter: set buildToolsVersion to 23.0.2                */
  case 0xd6ed722: /* o Updated Android Studio build settings in projects.               */
  case 0x908e1e9: /* o Android Studio exporter: changed default toolchain back to cl... */
  case 0x0162c97: /* o Android exporter: update default settings to gradle-experimen... */
  case 0xd9e3cda: /* o Removed a few references to var::null                            */
  case 0xfe7ebbd: /* o Projucer: Updated the default Visual Studio version to 2015      */
  case 0x708e357: /* o Prevented ColourGradient from having multiple colour stops at 0  */
  case 0xd32c64d: /* o Avoided an unused variable warning                               */
  case 0xaf9491c: /* o Fix typo in previous commit by incorrectly checking the OS X ... */
  case 0x8ababdd: /* o Fixed the SVG parser to stop it creating too many colour stop... */
  case 0x01e2e53: /* o Added an assertion to catch some unexpected behaviour in the ... */
  case 0x0cd926a: /* o Fix a bug which stopped multi-bus VST3 synths from working pr... */
  case 0x7196e57: /* o Don't try to build VST/VST3 hosting code on iOS and add missi... */
  case 0x1cbd0a0: /* o Projucer: stopped files with certain extensions (e.g. txt) be... */
  case 0xe05e9a4: /* o Added an assertion in AudioThumbnail                             */
  case 0xe034958: /* o Added some desktop scale compensation to drag + drop on Windows  */
  case 0xbf48687: /* o Made the OSX window code handle custom desktop scale factors ... */
  case 0xaa3ead5: /* o Resaved a demo component file                                    */
  case 0x6fc08dd: /* o Added a workaround for Android GL glitches when pausing the app  */
  case 0x676b27e: /* o Added a MidiDataConcatenator to the linux MIDI implementation... */
  case 0x0708b7a: /* o Added AAX macro JucePlugin_AAXDisableSaveRestore to set the A... */
  case 0xb9aaa88: /* o Fix for Synthesiser rendering 0 length blocks                    */
  case 0x5188904: /* o Avoided a conversion warning                                     */
  case 0xa0795e8: /* o Avoided an overflow in some extreme edge-case uses of WAVAudi... */
  case 0x49a7ca4: /* o Improved the String::createStringFromData method to fall back... */
  case 0xc1799c9: /* o Fixed a problem with negative values in MidiMessage::getKeySi... */
  case 0x81ff3a8: /* o Projucer: fixed an edge-case where the UI would misbehave if ... */
  case 0xb6e8869: /* o Projucer: Fixed module dependency checker                        */
  case 0x0c3057e: /* o Re-saved projects.                                               */
  case 0x2332c72: /* o Fixed generation of Carbon resources for AU plug-ins (was bro... */
  case 0x2682f7d: /* o Updated ChangeLog.txt                                            */
  case 0x46a090b: /* o Tweaked the login in the Synthesiser and MPESynthesiserBase c... */
  case 0x6ef3f1e: /* o Added asserts to clarify that WebBrowserComponent is not impl... */
  case 0x0d1f621: /* o Fix crash & assertion when running multi-bus plug-ins in Cake... */
  case 0x1675f82: /* o Fixed a mistake in BigInteger::inverseModulo()                   */
  case 0x0b3a11e: /* o Updated version number to 4.2.1                                  */
  case 0x9da130c: /* o Update comment on estimatedSamplesPerBlock parameter used in ... */
  case 0x00dd576: /* o Don't create an asserting VST2 plug-in when creating a new pl... */
  case 0x818287e: /* o Fix assertion when scanning plug-ins on Windows                  */
  case 0x9678eef: /* o Stripped out some defunct VS2003 code                            */
  case 0x6dd2fbe: /* o AUv3: added compiler error if compiling with deployment targe... */
  case 0x0ef8bdb: /* o Android Studio exporter: fixed and refactored the way include... */
  case 0x6047817: /* o Android Studio exporter: changed default toolchain to gcc, as... */
  case 0x26d07ea: /* o Android Studio exporter: do not write a toolchainVersion into... */
  case 0x4ef602f: /* o Fix C++-98 compilation errors in AudioPluginFormat code          */
  case 0xd64396e: /* o Removed rogue file                                               */
  case 0x8920e1b: /* o Resaved projects                                                 */
  case 0xfad8d05: /* o Projucer: improved the set of browsable files that are added ... */
  case 0x1601c90: /* o Fixed some merge-noise in NetworkGraphicsDemo                    */
  case 0x325aeb1: /* o Resaved projects                                                 */
  case 0x753c24c: /* o Fix to KnownPluginList::addToMenu ticks                          */
  case 0x61b2759: /* o Fix plug-in projects with whitespaces in their name              */
  case 0xe130ce1: /* o Projucer: fixed a suffix error when generating linux host pro... */
  case 0x8462849: /* o Re-saved projects.                                               */
  case 0xf2c50b8: /* o Android studio exporter: fixed include paths to work with new... */
  case 0x8dfb1c5: /* o JuceDemoPlugin.jucer: removed wrong AAX path that was saved t... */
  case 0x71abee4: /* o Android Studio exporter: added option to specify build tools ... */
  case 0xbb1fe3d: /* o Android Studio exporter: removed unused build setting.           */
  case 0x2fa6c20: /* o Fixed bug in Android Ant exporter due to mixed up build setti... */
  case 0x1e41e3d: /* o Fixed bug in AAX/RTAS Xcode export: installation paths were m... */
  case 0x4b1cabb: /* o Minor refactoring, clean-up, whitespace.                         */
  case 0x6d69f1b: /* o Added new Xcode option to strip of local symbols (off by defa... */
  case 0xf6efbed: /* o Remove default symbol visibility for JUCE shared code            */
  case 0x37073c3: /* o Using static linking instead of a framework for the shared co... */
  case 0xf5c19ef: /* o Fixed Android build settings of all .jucer files to use new d... */
  case 0xb94d281: /* o Refactored Android exporters to use CachedValue and get rid o... */
  case 0x8f4c4c9: /* o Fixed typo in LADSPA code                                        */
  case 0x4c0bc70: /* o Deleted some defunct Mac PowerPC code                            */
  case 0x42a956c: /* o Added a workaround for an obscure GL race condition when clos... */
  case 0x05f9001: /* o Fixed some MSVC warnings in CachedValue                          */
  case 0x968f838: /* o Added an option to KnownPluginList::addToMenu for specifying ... */
  case 0x3d1cf53: /* o Added colour IDs for ToggleButton tick colour                    */
  case 0xf74338a: /* o Resaved projects                                                 */
  case 0xe030a58: /* o Projucer: fixes for binary paths and Android Studio              */
  case 0xa439c6a: /* o Added new class: CachedValue                                     */
  case 0x905533c: /* o Minor formatting tidyups                                         */
  case 0x45e2b00: /* o Added a method ValueTree::getPropertyPointer()                   */
  case 0x3c68007: /* o Fixed capitalisation                                             */
  case 0x60cd2d7: /* o Projucer: fixed the way the GUI editor generates include paths   */
  case 0x6fc4d5a: /* o Fixed a bug in the Javascript parser involving array derefere... */
  case 0x486cb0c: /* o Projucer: tidied up an include path in a template file           */
  case 0x85a5dfb: /* o Projucer: corrected some help text                               */
  case 0x8343f34: /* o Found a few missing 'override' specifiers                        */
  case 0x6d03314: /* o Projucer: updated EULA                                           */
  case 0x59f2885: /* o Minor refactoring for a bit more clarity in the deprecated Xc... */
  case 0x367f6ee: /* o Fixed bug with wrong checksum for deprecated Xcode post-build... */
  case 0x54d3cfa: /* o Projucer: Fixed a bug that could cause focus to seemingly jum... */
  case 0x544b8d0: /* o Simplified some Projucer GUI editor code + added a missing sl... */
  case 0x06d8d41: /* o Added a method to StandaloneFilterApp to allow customisation ... */
  case 0xeffb421: /* o Projucer: Updated some font use and removed the EULA if runni... */
  case 0x025d04c: /* o Projucer: Added warning when trying to load projects with out... */
  case 0x871c3a9: /* o Removed some code that prevented RTL TextLayouts having their... */
  case 0xbb7189b: /* o Added kerning support to the DirectWrite layout code             */
  case 0x48b35c4: /* o Workaround for long filenames in Process::openDocument on OSX    */
  case 0x82350ba: /* o Re-saved Projucer with itself.                                   */
  case 0x68451ee: /* o Projucer: fixed bug where whitespace in the "additional frame... */
  case 0xcaffbba: /* o Projucer: better workaround missing framework symbols: adding... */
  case 0xc096b1a: /* o Reverted: Projucer: workaround missing framework symbols duri... */
  case 0xe04b7ff: /* o Added some extra time-out settings to the Linux CURL code        */
  case 0xe1205ec: /* o Re-added a WaveLab workaround in the VST wrapper                 */
  case 0xbe3acc9: /* o Tidied up an out of date method in LADSPAPluginFormat            */
  case 0xd80ceb0: /* o PlugInSamples: re-saved projects to fix accidentally added lo... */
  case 0xd9b0b53: /* o Corrected a rogue header comment                                 */
  case 0x7d64417: /* o Re-saved all audio plug-in projects.                             */
  case 0x0eb0f27: /* o Fixed Carbon host support for AudioUnit v2 by fixing the Proj... */
  case 0xef5aefe: /* o Re-saved all example projects that have AAX enabled by default.  */
  case 0x92a1747: /* o Fixed AAX plug-in builds with new module format on OS X.         */
  case 0x5fcea26: /* o Projucer: workaround missing framework symbols during Live bu... */
  case 0x3b65c5c: /* o Projucer AppearanceSettingsTab: added missing resized() funct... */
  case 0xc08375a: /* o added *.VC.db to .gitignore to ignore some files generated by... */
  case 0xe9edecc: /* o Resaved projects                                                 */
  case 0xe1098c8: /* o Projucer: Fixed failure to scan deep file hierarchies when bu... */
  case 0x9abd4ee: /* o Added some extra tolerance settings to some Path methods         */
  case 0x0a0c550: /* o Projucer: made sure that RTAS files are built with stdcall in... */
  case 0xb6261b9: /* o .gitignore: added JUCECompileEngine.dylib to ignored files in... */
  case 0xfb864a5: /* o Corrected some spelling mistakes in comments                     */
  case 0x7d178fe: /* o (Fixed mistake in last commit)                                   */
  case 0x32178f4: /* o Tidied up some AUv3 code                                         */
  case 0x5049bab: /* o Projucer: Fixed a typo that prevented the OSXLibs setting wor... */
  case 0x15f4789: /* o Projucer: Added a sanity-check to prevent compilation if some... */
  case 0x23b1d89: /* o Projucer: made the login form remember the last username         */
  case 0xbc58656: /* o Projucer: made build panel UI update when signing out            */
  case 0x1182aec: /* o Improved a comment                                               */
  case 0xc34f13a: /* o Fixed a linkage problem in some release plugin builds            */
  case 0x26d6cb9: /* o Fixed a few integer conversion warnings.                         */
  case 0x17e2acb: /* o Fixed a couple of AUv3 bugs                                      */
  case 0x9dda845: /* o Changed Projucer deployment target to 10.9 in Debug config (w... */
  case 0x63ef9c1: /* o Fix compilation errors in AU format when compiling for older ... */
  case 0xcdeb0b5: /* o Undo removal of setNumThreads in plug-in scanner                 */
  case 0x44359cb: /* o Fix missing std::nullptr defines                                 */
  case 0x24f05c4: /* o Whitespace.                                                      */
  case 0x9066ed4: /* o Projucer: fixed unreachable code warning in ProjectContentCom... */
  case 0xc77fea9: /* o Projucer Visual Studio: switched to default platform toolset ... */
  case 0x70949aa: /* o Added several new features: support for AudioUnit v3, new sim... */
    switch (feature)
    {
    case Feature::isSupportedCommit:
      return true;
    case Feature::hasVst2Interface:
      return false;
    }
  }

  if (feature == Feature::isSupportedCommit)
    return false;

  return getFeature(feature, kDefaultCommitSha1);
}
