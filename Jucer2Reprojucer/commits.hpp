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

#include <initializer_list>


static const auto kDefaultCommitSha1 = 0x75cd666;

static const auto kSupportedCommits = {
  0x75cd666, /* o Partially reverted fix for new VST3 SDK 3.6.7 by removing any depe... */
  0x14d6096, /* o Added support for VST3 SDK 3.6.7                                      */
  0xf5dafc4, /* o Fixed a bug in an Objective-C drag and drop helper class              */
  0x95085c1, /* o Listed all modules under the ISC license in the README                */
  0xb09a236, /* o Fixed a leak of OnScreenKeyboard                                      */
  0xdc5e039, /* o Fixed multiple file drag and drop on OS X                             */
  0x7778383, /* o Added code to only show the onscreen keyboard if Windows is in tab... */
  0x6977128, /* o Fixed a crash in the AAX wrapper when no AAX meters are present       */
  0xfc9937d, /* o   JUCE version 4.3.1                                                  */
             /* |\                                                                      */
  0x03b0df1, /* | o Re-saved all projects                                               */
  0xe0aff60, /* | o Bump version number to 4.3.1                                        */
  0x284fc82, /* | o Tidied up some Projucer code that was causing build errors in VS... */
  0xc754f6c, /* | o Fixed the SVG parser for an edge-case path string sequence, and ... */
  0x859567f, /* | o Fixed a compiler warning on iOS                                     */
  0x64f0027, /* | o Added a workaround for a macOS Sierra font rendering issue which... */
  0xa6d3c97, /* | o Fixed a bug on OSX where images with SingleChannel PixelFormat w... */
  0x4474d55, /* | o Fixed an issue where VST2 would report wrong input layout for pl... */
  0xd13be21, /* | o Added support for enabling/disabling sidechains in AAX              */
  0xd16123a, /* | o Added a new optional in/out parameter to AudioProcessor::Bus::is... */
  0x4b507cd, /* | o Fixed Xcode exporter macro escapes for '"'                          */
  0xd0b6d6c, /* | o Check all mouse sources when exiting modal loop                     */
  0xce14b66, /* | o Fixed a bug where a mouse enter event wouldn't be sent to the Co... */
  0x06dde9d, /* | o Removed unnecessary check in macOS/iOS URL session cancel           */
  0x3370ada, /* | o Fixed a compiler warning on newest version of gcc                   */
  0xb144923, /* | o Removed multi-character constant which triggered warnings on som... */
  0xa84c166, /* | o Fixed an issue where an iOS background download task would not r... */
  0x7e1279e, /* | o Fixed an int sign conversion warning                                */
  0x9b1ae84, /* | o Fixed bug when maximum number of plug-in io channels is one         */
  0x96ea465, /* | o Resaved all projects                                                */
  0x46e21fe, /* | o Updated win32 Process::isForegroundProcess() method to use proce... */
  0xd0b0644, /* | o Fixed compiler error in AUv3_Wrapper                                */
  0xd8ead14, /* | o Removed a duplicate header file                                     */
  0x1baaddb, /* | o Added code to properly escape non-ascii characters in URLs passe... */
  0xa554d0e, /* | o Added to the AudioProcessorValueTreeState documentation             */
  0xba512e9, /* | o Renamed 'generic' parameter Category enum to something less generic */
  0xda3f3a8, /* | o Fixed bug creating dynamic libraries with the Linux Makefile and... */
  0xf2b2fb2, /* | o Fixed JUCE_VST3_CAN_REPLACE_VST2 behaviour                          */
  0x7897331, /* | o Added support for level meter parameter categories                  */
  0x2a98306, /* | o Fixed bug in OSX network code where Content-Type headers were no... */
  0xfcd5a47, /* | o Added a bit-twiddling helper method: findHighestSetBit()            */
  0x0ebd5bc, /* | o Modified the repaint throttling to detect whether the app is a p... */
  0x0d7a77d, /* | o Improved AudioProcessorValueTreeState documentation                 */
  0x83a4f74, /* | o Added support saving/restoring plugin bus layouts in the audio h... */
  0x101a59a, /* | o Fixed bug updating MacOS system tray icon                           */
  0xa6c4a02, /* | o Avoided an unused variable warning in the OSX network code          */
  0x4b69d0f, /* | o Fixed buffer size and sample rate changes in iOS background audio   */
  0xc52f147, /* | o Improved a variable name                                            */
  0x7d4d853, /* | o Fixed AAX SDK relative path bug on Windows                          */
  0x4c1d4f0, /* | o Added method TabbedComponent::moveTab()                             */
  0x2ad22dc, /* | o Added workaround to check the state of the on-screen keyboard on... */
  0x7120d7d, /* | o Fix for HTTPS POST requests with keep-alive failing on OS X vers... */
  0x5849710, /* | o Added iOS support for BLOCKS examples                               */
  0x1e5865c, /* | o Restored original iOS sample rate after querying all available s... */
  0x25e72a7, /* | o Fixed some issues with the Linux Code::Blocks exporter              */
  0xf918827, /* | o Clarified some comments in the Logger class                         */
  0x92f3a1e, /* | o Miscellaneous fixes for some PVS static analyser warnings           */
  0xf3be41c, /* | o Fixed a bug in the Windows FileChooser where two backslashes wou... */
  0xd4b4780, /* | o Fixed some spelling mistakes in comments                            */
  0x7eb534a, /* | o Added method ValueTree::getRoot()                                   */
  0xa15d79d, /* | o Added new methods Rectangle::withRightX() and withBottomY()         */
  0x2c8b98b, /* | o Added windows native code to show and hide the on-screen keyboar... */
  0xf1baf9b, /* | o Added VST hosting support for begin/endParameterChangeGesture, a... */
  0x9e0370c, /* | o Made sure the File::getNonexistentChildFile() method's putNumber... */
  0xeb07aaf, /* | o Added some TRANS macros to some internal OSX menu item strings      */
  0x5eeaf5a, /* | o Added move semantics to AudioBuffer                                 */
  0xaae0b15, /* | o Added begin/end iteration to HashMap                                */
  0xb184ca9, /* | o Removed an accidentally-committed method from String                */
  0xb90077c, /* | o Cleaned up some warnings in the PNG code                            */
  0x6f27a1d, /* | o Typo fixes                                                          */
  0xab0c519, /* | o Workaround for an android-specific bug in String::formatted()       */
  0x158bc98, /* | o Fixed an issue restoring VST3 GUI state                             */
  0xa7ee1a7, /* | o Added missing initialiser in Path::Iterator constructor             */
  0xaae6471, /* | o Minor compile fix in the javascript parser for VS2013               */
  0xaf07915, /* | o Fixed Linux message thread assertion in AudioProcessorValueTreeS... */
  0x2d96e37, /* | o Changed the default sidechain layout to stereo in the NoiseGate ... */
  0xdd13702, /* | o Added support for drag and drop of text on OSX, via DragAndDropC... */
  0xb1d3069, /* | o Added a new Projucer Xcode target option "Keep custom Xcode sche... */
  0x7a02a83, /* | o Make sure that the OSX SDK is new enough when compiling AUv3 tar... */
  0x27c353c, /* | o Fixed an issue when compiling with newer external png libraries     */
  0x32db13e, /* | o CPP/C/CXX/LDFLAGS should be appended to JUCE_CPPFLAGS etc. in li... */
  0xae3ee34, /* | o Fixed an issue when including an external and newer version of zlib */
  0xa6d3d19, /* | o Fixed a few minor bugs in X11 windowing code                        */
  0xf284620, /* | o Whitespace cleanup                                                  */
  0x3104616, /* | o Added some copy/paste options for module paths to the Projucer's... */
  0xa9b29ba, /* | o Added support for lambdas in var::NativeFunction                    */
  0x901913a, /* | o Added new method FlexItem::withAlignSelf()                          */
  0xaac01b2, /* | o Fixed a couple of spelling mistakes                                 */
  0xd90ca6c, /* | o Fixed a typo in the OpenGL demo code                                */
  0xddaaaa6, /* | o Removed 'const' from the Projucer's component editor's generated... */
  0xf85bbb2, /* | o Fixed an issue where resizing a VST2 plug-in on a HiDPI monitor ... */
  0x53ae78f, /* | o Fix for javascript parseInt of strings that start with a zero bu... */
  0xcab4569, /* | o Fixed a typo which lead the MSVC exporter to incorrectly recogni... */
  0x688110f, /* | o Fixed a littlefoot interpreter bug involving 32-bit integer lite... */
  0x8638630, /* | o Fixed unused parameter error in BluetoothMIDISelector overlay       */
  0x4b881b0, /* | o Added an exit callback to BluetoothMidiDevicePairingDialogue::op... */
  0x4d02823, /* | o BLOCKS API: Added a few littlefoot function definitions to the l... */
  0x1354998, /* | o Added C++11 functions to Doxygen                                    */
  0x21aad5e, /* | o Fixed IO channels when selecting <<none>> as OS X audio device      */
  0x8567338, /* | o Changed the colour of module icons in the Projucer based on thei... */
  0x2e84129, /* | o Updated the juce_audio_basics, juce_audio_devices, juce_blocks_b... */
  0xe91d0f9, /* | o Fixed a bug where the URL's DownloadTask would not indicate an e... */
  0xfdeea46, /* | o Fixed a bug where a downloaded file may have been incomplete whe... */
  0xe31a0b1, /* | o Resaved all projects                                                */
  0x8869fce, /* | o Fixed minor documentation typo in a recent PropertiesFile commit    */
  0x806d99d, /* | o Added an option to have a properties file suffix be appended to ... */
  0x05888a0, /* | o BLOCKS API: Added some functionality (not yet supported in publi... */
  0xe75128a, /* | o Added a second createMemoryMappedReader method to AudioFormat an... */
  0x1c4b687, /* | o Fixed a bug where the high resolution timer could hang when stop... */
  0x3fa62bc, /* | o Fixed some misnamed Doxygen parameters                              */
  0x9f1254a, /* | o Removed JucePlugin_{Max,Min}NumInputChannels from the VST wrapper   */
  0x2f06fdf, /* | o Updated the standalone BLOCKS SDK build                             */
  0x38a47d4, /* | o Added a call to getToggleState() in ShapeButton::paintButton() t... */
  0xffc687a, /* | o Added TargetExt vcxproj property to MSVC                            */
  0x9a130f2, /* | o Set avoidReallocating argument of AudioBuffer::makeCopyOf() to t... */
  0x2952b52, /* | o Renamed instances of FileHelpers to MacFileHelpers in iOS implem... */
  0xfb08261, /* | o Fixed VS2013 warning in File::NaturalFileComparator struct          */
  0x3516e45, /* | o Fixed a typo in littlefoot function argument passing                */
  0x8cd578b, /* | o Added the ability to set a custom BinaryData namespace in the Pr... */
  0x4d48dfe, /* | o Removed a defunct comment                                           */
  0xe8a80ef, /* | o Added Bitwig Studio to PluginHostType                               */
  0xa0ed11e, /* | o Made File::NaturalFileComparator::compareElements() method const    */
  0xcd752ed, /* | o MidiFile::writeTo() method now returns false if unsuccessful        */
  0xf6727f2, /* | o Added label parameter to all AudioParameter constructors            */
  0x36f561a, /* | o Changed FileHelpers namespace to MacFileHelpers in juce_mac_File... */
  0xe93eee0, /* | o Added NaturalFileComparator struct to File. Reverted File::compa... */
  0xc5d3c26, /* | o Added workaround to fix a false re-save warning in the Projucer ... */
  0x1ecc686, /* | o Changed order of parameters in FlexItem::Margin constructor to m... */
  0xbd9697b, /* | o Fixed a compiler error on older gcc versions                        */
  0xa816a44, /* | o Disabled Ableton Live's plug-in auto-suspend if the plug-in repo... */
  0x8e768fc, /* | o Added method littlefoot::Runner::isProgramValid()                   */
  0xd6fc589, /* | o Reverted File::compareFilenames() method to not compare filename... */
  0xfa7768a, /* | o Fixed Projucer live-build error with LittleFoot compiler            */
  0xc545b58, /* | o Added another constructor to FlexItem::Margin                       */
  0x17600a2, /* | o Added copy constructor and copy assignment operator to MidiFile     */
  0x8e7ed3a, /* | o Fixed MacOS 10.12 deprecations                                      */
  0x18c896f, /* | o Changed Line::getPointAlongLineProportionally method to work pro... */
  0x910cab7, /* | o Added a WaveLab workaround by adding tmp buffers when WaveLab li... */
  0xb86e133, /* | o Fixed a bug where the AU/AUv3 wrapper would create a spurious in... */
  0xa3ef455, /* | o Ignored deprecation warnings in the OS X VST3 SDK                   */
  0x0f01433, /* | o Fixed an uninitialised variable in OnlineUnlockStatus               */
  0x0ee2398, /* | o Fixed compile error in recent commit on VS2015                      */
  0x92c219f, /* | o OSC: fixed bug where, if one OSCBundle contains multiple OSCBund... */
  0x3945f1d, /* | o OSC: added unit tests for round-trip correctness (receiver to se... */
  0x8313ba7, /* | o Added a workaround for DSP Quattro                                  */
  0xd91b07c, /* | o Changed depcrecated use of old dragOperationStarted/Ended to be ... */
  0xb5fb095, /* | o Fixed a few VS2008 build errors                                     */
  0x267cfb8, /* | o Fixed scoping of a conditional                                      */
  0x2ec8fb4, /* | o Fixed error restoring VST3 state in FL Studio                       */
  0xacf554a, /* | o Fixed erroneous assertion when saving VST3 states                   */
  0x40dfba1, /* | o MPESynthesiser: Added interface to set tracking mode for pressur... */
  0x80778a8, /* | o Fixed a couple of compiler warnings in the LittleFoot runner class  */
  0x7bd5c05, /* | o Fixed touch event bug on Windows where touches weren't being rec... */
  0x0df1e0b, /* | o Added sourceDetails parameter to dragOperationStarted and dragOp... */
  0x2f00cb1, /* | o Tidied up ListenerList tests                                        */
  0x1225653, /* | o Fixed bug when setting AudioProcessorValueTreeState values befor... */
  0x6340e54, /* | o Fixed a bug where StreamingSocket::isLocal would only return tru... */
  0x1e2cb6f, /* | o Changed documentation for OutputStream::setNewLineString() to re... */
  0xb517884, /* | o Fixed MSCV2015 compiler warning in VST3 wrapper                     */
  0xc43ca07, /* | o Fixed a typo in the BLOCKS documentation                            */
  0x284bc70, /* | o Added a workaround for WaveLab invisible editor bug                 */
  0x2f5b8e5, /* | o Changed File::compareFilenames() method to use String::compareNa... */
  0x27dd794, /* | o Added missing const qualifier to AudioDeviceManager::getAudioDev... */
  0xda5805f, /* | o Fixed a race condition in iOS DownloadTask implementation           */
  0x119640f, /* | o Fixed positioning of VST3 windows in Reaper                         */
  0xc0987f2, /* | o Fixed a false positive in ASAN in a recent bug fix commit to OSX... */
  0x57fdbd4, /* | o Fixed a potential access after free bug when creating typefaces ... */
  0x60b5f72, /* | o Fixed a bug in Windows where clicking outside an application aft... */
  0x7467195, /* | o Minor tidying-up                                                    */
  0x436f047, /* | o Fixed a bug where the maximum number of channels in VST2 was lim... */
  0x990b7b3, /* | o Fix for openGL components when global scale factors are applied     */
  0x8154ccc, /* | o Added a fix for a potential wrap-around bug in BufferingAudioSource */
  0x61fd2a3, /* | o Fixed a MSVC 2015 warning in mp3 audio decoder                      */
  0x00c0671, /* | o Added sub-menu support to ComboBoxes                                */
  0x6641b9d, /* | o Updated BlocksMonitor example to display correct topology with c... */
  0x8a93ddc, /* | o Fixed a typo in recent studio one parameter workaround              */
  0xee373af, /* | o Added AlertWindow LookAndFeel methods to offer more control on b... */
  0x388251b, /* | o Made message queue throttling more precise by using Time::getMil... */
  0xeba80c3, /* | o Tweaked buffering audio source so it can more easily be used in ... */
  0xe49022b, /* | o Added JUCE_USE_STUDIO_ONE_COMPATIBLE_PARAMETERS option to workar... */
  0x9a7ee9f, /* | o Added support for iOS custom delegates - use at your own risk       */
  0xf93b174, /* | o Fixed error in variable name                                        */
  0x37f3481, /* | o Potentially fixed an issue with the message thread being flooded... */
  0xcff1793, /* | o Exposed some functions when building JUCE as a DLL                  */
  0xcaa65e8, /* | o Add new URL::downloadToFile method                                  */
  0xfd86a73, /* | o Fixed DLL compilation of juce_tracktion_marketplace                 */
  0xf5d3278, /* | o Projucer: avoid failing assertions from File                        */
  0x1349f65, /* | o Projucer: the DiagnosticReceiver interface is an implementation ... */
  0x63a71ff, /* | o Fixed a bug where the VST3 plug-in wrapper would not respect the... */
  0x1c2d147, /* | o Added LookAndFeel getAlertWindowButtonWidth method to override t... */
  0xee4fe9e, /* | o   Updated BlocksSynth example to draw waveshapes on the Lightpad... */
             /* | |\                                                                    */
  0xe384fa7, /* | | o Added Timer to MainComponent to stop touches from triggering m... */
  0x020f858, /* | | o Doxygen updated for BlocksSynth tutorial and LittleFoot Langua... */
  0x1cc8a95, /* | | o Class comment added to top of WaveshapeProgram.h                  */
  0x24364ec, /* | | o auto-generated comment removed from top of WaveshapeProgram.h     */
  0x15cab58, /* | | o Xcode strict warnings                                             */
  0x8c55f73, /* | | o MSVC warnings                                                     */
  0xb2d0328, /* | | o Code cleanup & WaveshapeProgram documentation                     */
  0xe5d6770, /* | | o Working LittleFoot program for waveshape drawing                  */
  0x91e8d27, /* | | o LED drawing using LittleFoot                                      */
  0x39344e0, /* | | o WaveshapeProgram.h added, LittleFoot program started              */
             /* | |/                                                                    */
  0x2ae5d22, /* | o Resaved all projects                                                */
  0xb82773b, /* | o Fixed older MacOS compatibility for ListenerList tests              */
  0x6fb0aa4, /* | o Added a StringArray constructor to var                              */
  0xd438fa5, /* | o Fixed issues with AudioProcessorValueTreeState parameter synchro... */
  0x5a2ebef, /* | o Fixed a bug where the bundle exit function would be called befor... */
  0xc3ea4f8, /* | o Fixed a warning in MSVC 2013 in JUCE's URL class                    */
  0x3f72a03, /* | o Fixed whitespace in last commit                                     */
  0xc2caad4, /* | o Added missing CoInitialize when using ASIO audio device             */
  0x8499e29, /* | o Fixed a VS2013 warning in juce_URL                                  */
  0x3b630f3, /* | o Added new WebInputStream class for more fine-grained control on ... */
  0x76fa906, /* | o Added a LittleFoot section to the BLOCKS SDK documentation sidebar  */
  0xec2fd97, /* | o Changed BLOCKS Doxygen stylesheet to better match developer.roli... */
  0x3fb4edb, /* | o BLOCKS SDK: Fixed the order of south and west DNA ports in the t... */
  0x58bd2b1, /* | o Added a few extra FlexItem convenience methods                      */
  0x63eabb4, /* | o Fixed a bug where identical initialiser lines would be removed      */
  0xaa5d1d1, /* | o Tweaked CoreAudio to allow buffer sizes of 2048                     */
  0xbfd5605, /* | o Added comments to specify what File::create vs. FileOutputStream... */
  0x826fdfe, /* | o Added callback to get number of keywaves of an on-screen seaboar... */
  0xde84462, /* | o BLOCKS example apps: some cleanup, enabled DUMP_TOPOLOGY flag       */
  0x7bb0fe4, /* | o Update copyright and make docstrings more compatible with Doxygen   */
  0xa85f026, /* | o BLOCKS documentation improvements                                   */
  0x0464728, /* o | Fixed bug when maximum number of plug-in io channels is one         */
  0xa08cedb, /* o | Fix for HTTPS POST requests with keep-alive failing on OS X vers... */
  0x1066a6d, /* o | Added C++11 functions to Doxygen                                    */
  0xbb56cb2, /* o | Fixed an uninitialised variable in OnlineUnlockStatus               */
  0x169b5ed, /* o | Update copyright and improve BLOCKS documentation                   */
  0xf88013e, /* o |   JUCE version 4.3.0                                                */
             /* |\ \                                                                    */
             /* | |/                                                                    */
  0xefd4bb6, /* | o Re-saved all projects                                               */
  0x363ceb0, /* | o Bump version number to 4.3.0                                        */
  0xb5ef6d7, /* | o Whitespace clean-up                                                 */
  0x6f6d0af, /* | o Fixed a bug where we didn't search the correct directory for the... */
  0xf3c313b, /* | o Added label to BlocksSynth and BlocksDrawing example windows        */
  0x73764c2, /* | o Fixed beforeDate in standalone BLOCKS SDK MacOS example             */
  0x980ece4, /* | o Projucer: add anchor #indie to the link target for the subscribe... */
  0x76b3689, /* | o Added BLOCKS module                                                 */
  0x01380b1, /* | o Fixed a typo in the SSE 4.1/4.2 detection logic                     */
  0x8d02f61, /* | o Projucer: add free-to-use license for the live-build engine         */
  0x4aa0f31, /* | o Allow to specify the stack size for each thread in a thread pool... */
  0x439ecc3, /* | o Projucer: workaround broken "Open Recent" submenu                   */
  0x701fd8a, /* | o Added a workaround for a strange bug with monospace fonts in OS ... */
  0x6711016, /* | o Fixed a bug that could make surround plug-ins fail AU validation... */
  0x7cdcb19, /* | o Added a convenient function to get all channel sets with a parti... */
  0xf415b09, /* | o Enabled cookies in the juce curl backend to align the behaviour ... */
  0x15bed81, /* | o Fixed a flex box bug where the first item in a list of too large... */
  0xae13dd6, /* | o Added an option to escape round brackets in URLs                    */
  0x75ea6db, /* | o Renamed bounds member variable in Component to "boundsRelativeTo... */
  0x5d02569, /* | o Added a method to ThreadPool to query the number of threads assi... */
  0x56c5c8e, /* | o Fixed an issue in CachedValue where setValue would not actually ... */
  0xb36b64a, /* | o Projucer: simple pop-up menu to copy diagnostic messages to clip... */
  0x26e0aa7, /* | o Fixed a crash on quit when windows are open in the Juce Demo's w... */
  0xe435325, /* | o Fix typo and increase maximum size of the "Member initialisers" ... */
  0x1845437, /* | o Only moan about all-lowercase AU manufacturer codes                 */
  0xc4d36d1, /* | o Whitespace clean-up                                                 */
  0x76a9b7d, /* | o Projucer: link to privacy policy in EULA and re-save jucer project  */
  0x23a248f, /* | o Added method VSTPluginFormat::getPluginInstanceFromVstEffectInte... */
  0xf54b7fb, /* | o Moved the DiscRecording dependency in the module descriptions fr... */
  0xeac52c9, /* | o Fixed a bug in the projucer that would create incorrect post bui... */
  0x288e56d, /* | o Fixed a bug in the Projucer that would incorrectly mark relative... */
  0x5e0efc6, /* | o Projucer: extend interface DiagnosticMessage::handleRecoverableE... */
  0xa931b47, /* | o Added a non-sense if statement to BufferingAudioSource to surpre... */
  0x3e7b599, /* | o Moved audio CD burner/reader to juce_audio_utils                    */
  0xbd3166a, /* | o Updated the URL of the "Forgot Password" button in the Projucer     */
  0x348dc1f, /* | o Fixed a bug where the Projucer would delete rsrc files in your ~... */
  0xc587d4b, /* | o Minor code style tidy-ups                                           */
  0x7d07f51, /* | o Added missing juce_osc dependency on juce_events                    */
  0xa5e3b7a, /* | o Added check and warning when entering only lowercase AU plugin i... */
  0x84a16f6, /* | o Fixed Projucer to check for the correct file when looking for th... */
  0x6c52bf5, /* | o Changed the way windows modifiers are probed in an attempt to fi... */
  0x255dc1d, /* | o Fixed documentation typo                                            */
  0x0bcc53c, /* | o Removed juce_audio_formats header from juce_audio_devices           */
  0xff52d76, /* | o Fixed a bug in DirectoryIterator where the result would be incon... */
  0xa347689, /* | o Moved simple sound player to audio_utils module                     */
  0x1fcae36, /* | o Changed flac writer to respect the current stream position when ... */
  0x29bed6c, /* | o Fixed warnings in cryptographic unit test code                      */
  0xc6a75d0, /* | o Changed the way windows modifiers are probed in an attempt to fi... */
  0x2a37ba0, /* | o Added gradle-wrapper when saving projects so that it is now poss... */
  0x676ad43, /* | o Added more compare operators to Identifier class                    */
  0x9e4741f, /* | o Fixed an AAX bug where the sidechain buffer was not cleared when... */
  0x14957e7, /* | o Avoided some FLAC debug output being printed if your app defines... */
  0x62ac568, /* | o Made sure that colours are updated when the look and feel of a T... */
  0x57742a5, /* | o Fixed warning of MSVC compilers in latest commit                    */
  0x852fe34, /* | o Added support for encrypting/decrypting blocks of data with Blow... */
  0xcdb7285, /* | o Fixed AAX post build script to consider SDK paths containing whi... */
  0x285635c, /* | o Added notch and all pass filters                                    */
  0x9d08f8f, /* | o Fixed a documentation typo.                                         */
  0x4a6473a, /* | o Removed the isLocked() method from ReadWriteLock                    */
  0xf301ed7, /* | o Added an isLocked() method to ReadWriteLock that checks whether ... */
  0xe03e41b, /* | o Projucer: hard shutdown of the compiler process in release build... */
  0xbacf194, /* | o Small tweak to the bluetooth iOS menu to make it as wide as poss... */
  0x773e708, /* | o Removed unecessary path seperators in MSVC post-build scripts       */
  0x3bd7ef4, /* | o Projucer: menu adjustments                                          */
  0x0ed9003, /* | o Fixed bundle ref initialisation of statically linked VSTs on iOS    */
  0x13c501d, /* | o Removed unnecessary memory allocation when assigning a big integ... */
  0xf374eb6, /* | o Fixed SortedSet::remove() to return the removed element             */
  0xc1e2e6a, /* | o Fixed size of bluetooth midi pairing dialogue to accommodate mor... */
  0x8f1d37a, /* | o Fix addAllModulesInFolder to scan only 2 subfolder levels           */
  0x46b38d7, /* | o Removed code which is not used anymore                              */
  0xedd828a, /* | o Fix memory leak in BluetoothMidiSelectorOverlay                     */
  0x13c4946, /* | o Changed lowResamplingQuality to kCGInterpolationNone on OS X and... */
  0x6c39897, /* | o Added 'inclusive' and 'exclusive' to documentation for Random::n... */
  0x8263d70, /* | o Continue drawing if a negative dashLen was hit                      */
  0x7686db6, /* | o Fixed a typo in my latest commit                                    */
  0x59a47d1, /* | o Fixed possible dead-lock                                            */
  0xe8cdc65, /* | o Fixed Array::remove (ElementType*) so that if given an invalid p... */
  0xa440c16, /* | o Added Graphics::ResamplingQuality::noResampling                     */
  0x0121a02, /* | o Added optional parameter to restore audio settings state in Audi... */
  0x321ca8c, /* | o Fixed AUv3_Wrapper include path                                     */
  0xb24aadc, /* | o Added ColourIds to allow alternate colouring of TreeViewItems       */
  0xd892109, /* | o Fixed a warning in recent commit                                    */
  0x40994fc, /* | o Added checks to make sure that the current interface orientation... */
  0x848073d, /* | o Always remove silence flag in AU render callback                    */
  0x27be047, /* | o Allow disabling the main bus in VST3 plug-ins with the revised m... */
  0x225e8da, /* | o Added waitForNextAudioBlockReady method to BufferingAudioSource     */
  0x2104c50, /* | o Added const qualifier to MidiEventHolder argument of MidiMessage... */
  0x2785059, /* | o Restored non-flushing behaviour of FileOutputStream                 */
  0x848b400, /* | o Avoided warning in release builds in AudioUnit hosting code         */
  0xbf85f4c, /* | o Cache the result of isRunningInAppExtensionSandbox                  */
  0x495e2bf, /* | o Fixed file flushing for FileOutputStream on Windows                 */
  0x728bf27, /* | o Projucer: support recoverable error warnings for live-builds        */
  0xd19ba05, /* | o Added support for saving audio/midi setting in standalone wrappers  */
  0x80ff164, /* | o Added preprocessor macro JucePlugin_VSTChunkStructureVersion to ... */
  0x83d0854, /* | o Added a microphone permission option to the iOS exporter            */
  0xfc1214e, /* | o Fixed ambiguous sign in AudioThumbnail implementation               */
  0x3fb55ff, /* | o Fixed a compiler error which occured in gcc based compilers         */
  0x71de185, /* | o Fixed typo in linux-only Projucer code                              */
  0xc98f71d, /* | o Fix Process::setDockIconVisible() may not turn the process into ... */
  0x21c6fc7, /* | o Added an option to enable/disable pre-buffering in BufferingAudi... */
  0x4b96ffb, /* | o Projucer: restrict menu item "Launch Application" to GUIApplicat... */
  0xa0350e4, /* | o Projucer: add feature download & install live-build engine (incl... */
  0x42b8156, /* | o Fixed crashes in old 32-bit Carbon plugin editor windows when ru... */
  0xe14895e, /* | o Added macOS version 12 to SystemStats                               */
  0xaff5ea5, /* | o Replaced two instances of src[1] with src[0] in the render4Pixel... */
  0x6f4571b, /* | o Tweaked VST wrapper so that MIDI effect plug-ins work even if th... */
  0xef50b8d, /* | o Fixed trying to write to invalid input buffer when AUs connected... */
  0x679ef3f, /* | o Fixed a bug where VST plug-in editors using OpenGL would not wor... */
  0x4f06ff9, /* | o Third time lucky: hopefully fixed a white-noise issue when pulli... */
  0xc14eb04, /* | o Fixed automation bug when switching between presets in VST and VST3 */
  0x5a365ed, /* | o Removed assertion which gets hit by too many VST plug-ins           */
  0x8835a24, /* | o Fixed another typo and removed unecessary for loop                  */
  0x1be76c1, /* | o Added more methods to IIRCoefficients to generate coeffecients f... */
  0xcbbf76b, /* | o Made sure buffers are prepared before clearing them in the AU wr... */
  0xb0457a9, /* | o Fixed a typo in the VST wrapper                                     */
  0x1b217d1, /* | o Zero out buffers if pulling audio returns an error in the AU wra... */
  0x4fa0516, /* | o Revised multibus API and added support for multibus hosting         */
  0x680d758, /* o |   JUCE version 4.2.4                                                */
             /* |\ \                                                                    */
             /* | |/                                                                    */
  0xebf19aa, /* | o Re-save all projects                                                */
  0x4eb8933, /* | o Bump version number to 4.2.4                                        */
  0x05b15c4, /* | o If the jucer file does not specify a macOS/iOS deployment target... */
  0x0862167, /* | o Fixed a compiler warning for MIDI effect plug-ins                   */
  0x0c87af8, /* | o Fixed Linux compile error in Projucer                               */
  0xacefbcd, /* | o Fixed a typo that was preventing some config settings showing up... */
  0x9fa0d49, /* | o Added an option JUCE_ALLOW_STATIC_NULL_VARIABLES that can be use... */
  0xd03755c, /* | o Fixed a potential memory leak                                       */
  0x48ac634, /* | o Fixed the MSVC exporter to combine multiple internal post-build ... */
  0x231a6e4, /* | o Fix some warnings in MSVC                                           */
  0x8ec9443, /* | o Projucer various fixes                                              */
  0xe35aba3, /* | o Added a standard iterator to NamedValueSet                          */
  0x2fd331f, /* | o Modified compile-time error to warn the user that juce_audio_dev... */
  0xcbb8536, /* | o Fixed potential crash in mac's legacy WebInputStream implementation */
  0xceefc5b, /* | o Avoided a dead-lock in a recent commit to WebInputStream on mac     */
  0x6d56e48, /* | o Add support for retina mouse cursors on OS X                        */
  0x5f748bb, /* | o Added a method to get the internal viewport of a PropertyPanel      */
  0xc67c1d4, /* | o Added missing JUCE_API keywords                                     */
  0xff40b46, /* | o Added support for automatically creating AAX plugin bundles on W... */
  0xf37787d, /* | o Fixed incorrect comparison in right shift operator of BigInteger    */
  0xb94a68c, /* | o Array: added new method removeIf to remove objects based on an a... */
  0xcf8d655, /* | o Fixed a possible race condition in mac's implementation of WebIn... */
  0x9177a94, /* | o Added a check in Button to detect self-deletion as a side-effect... */
  0x1f2eaeb, /* | o Added support for accessing the properties of a javascript objec... */
  0x913a868, /* | o Fixed a crash on exit error in the plug-in host on windows/linux    */
  0x16525c0, /* | o Fixed some typos.                                                   */
  0xd73f776, /* | o Fixed a bug where FlexBox would still be compiled on older versi... */
  0x004f829, /* | o Reverted #bea45f84 - this only fixed SVG positioning bugs for li... */
  0x9276540, /* | o Fix to avoid negative width assertion when drawing fitted text i... */
  0x56423ae, /* | o Changed Windows windowing so that the resizeStart() and resizeEn... */
  0xed0c032, /* | o Added a styleChanged flag to enable setText() to take effect aft... */
  0x29320a0, /* | o Changed maxSensibleMidiFileSize in MidiFile from 2mb to 200mb to... */
  0xbea45f8, /* | o Fixed SVG child-transform rendering bug.                            */
  0xe74849c, /* | o Fixed a missing call to lf.preparePopupMenuWindow in PopupMenu      */
  0x151e3f7, /* | o Added a LookAndFeel method preparePopupMenuWindow() to allow the... */
  0x132c1d6, /* | o Removed "C" after all mono channel names                            */
  0x1a28e7a, /* | o Fixed modulo zero bug in VST bus arrangement                        */
  0x821dea8, /* | o Fixed implicit conversion warnings in BigInteger                    */
  0xc94be5d, /* | o Fixed an issue where repaints could be dropped when the host is ... */
  0xf257536, /* | o Fixed #100: If base sdk version is newer than 10.12 then no need... */
  0x16e6c6f, /* | o Slider and NormalisableRange changed to use standard library mat... */
  0xc10b042, /* | o Fixed a potential leak in ScopedPointer                             */
  0x70530a9, /* | o Added beginParameterChange() and endParameterChange() methods to... */
  0x19d654d, /* | o Added symmetric skew option to NormalisableRange class. Added op... */
  0x5c46c65, /* | o Changed the documentation for AudioProcessorValueTreeState::Slid... */
  0xf0555e6, /* | o Fixed JucePlugin_IsSynth compiler warning.                          */
  0x7be711e, /* | o Added some functions writeLittleEndianBitsInBuffer() and readLit... */
  0xa9d8434, /* | o Resaved all projects                                                */
  0xc0c912a, /* | o Modified BigInteger so that small (128-bit) values don't require... */
  0x28bb28a, /* | o Fixed a trailing "\" bug in the VS intermediates path               */
  0xc23e95d, /* | o Defined HAVE_LROUND for non-MSVC compilers when building FLAC libs  */
  0x979d3b6, /* | o Fixed crash on internal changes within aggregate audio devices o... */
  0x17dee9a, /* | o Restore depecated VST opcodes for CarbonWrapperComponents           */
  0xddb8796, /* | o Fixed VSTs erroneously wanting midi input                           */
  0x1ddd394, /* | o Restore missing VST opcode.                                         */
  0xd873d24, /* | o Fix some Xcode warnings in the VST and VST3 wrappers.               */
  0xcb6b4b7, /* | o Remove VST editor idle                                              */
  0x881dfce, /* | o Fix deadlock in handleRouteChange for iOS audio.                    */
  0x6ba384f, /* | o Fixed some missing definitions in the VST hosting for Carbon com... */
  0x7fad254, /* | o Added output level metering to AudioDeviceManager                   */
  0x1942e3d, /* | o Added begin/end iterator methods for ValueTree, for handy range-... */
  0xea76779, /* | o modified sliderValueChanged() to notify host only on mouse clicks   */
  0x47c48b2, /* | o Changed Projucer LoginForm "Remember login" button to tick box      */
  0x9f31d64, /* | o Use a bespoke VST2 interface                                        */
  0x1e9af22, /* | o modified valueChanged() to notify host only on mouse clicks.        */
  0x08b2116, /* | o Make the mousemove behaviour of OS X plug-ins the same as their ... */
  0x48db4fa, /* | o Fix for the copy constructor of DrawableRectangle                   */
  0x55194a0, /* | o On Windows, suppressed the behaviour that pressing the ALT key t... */
  0xa46191a, /* | o Native OSX menu bar: avoided some duplicated menu bar flashes wh... */
  0xb1ab450, /* | o Avoided a warning in some OSX network code                          */
  0xc353ac3, /* | o Avoided a race condition when cancelling HTTP stream on OSX         */
  0xc7db4bb, /* | o Avoided a compiler warning on OS X due to unused private fields ... */
  0x773c963, /* | o Whitespace.                                                         */
  0xe2dc460, /* | o Made the Desktop class clear any pending animations before shutt... */
  0x6547452, /* | o Fixed a leak of bubbleMessage object in juce demo                   */
  0x91f2453, /* | o Fixed a bug in UndoManager when using undoCurrentTransactionOnly    */
  0x2417676, /* | o Increased buffer size for names read by the VST host, and change... */
  0x3f83eed, /* | o Minor tidying up                                                    */
  0xe9832ff, /* | o Added a symmetric skew option to Slider                             */
  0x79dd695, /* | o added ScopedLock to iOSAudioIODevice::handleRouteChange method t... */
  0x8e10f16, /* | o Dialog window to ask whether to keep current project or re-load ... */
  0x1470928, /* | o Use effGetProductString when getting the name of a VST-2 plugin     */
  0x82224d7, /* | o Avoided a couple of warnings in iOS builds with older SDKs          */
  0x999f484, /* | o Stopped the ColourSelector sending a change message from its con... */
  0x3104cbe, /* | o Added an optional notification argument to ColourSelector::setCu... */
  0xcbdf707, /* | o Windows touch with CallOutBox bug fix                               */
  0xc7b3472, /* | o Partially undo commits #99186e5 and #89d938d and use less-intrus... */
  0x8f7d9f7, /* | o Workaround to avoid an assertion when using File::invokedExecuta... */
  0x59cc979, /* | o Deallocate all nodes in the graph before calling JUCEApplication... */
  0x8994f37, /* | o Added some FlexBox layout classes, and a demo page for this in t... */
  0xaa1acb3, /* | o Avoid unnecessary calls to AudioUnitInitialize.                     */
  0x0159102, /* | o Fix ResamplingQuality documentation                                 */
  0xb936786, /* | o Remove the recently added shouldReleaseFocusOnMainMenuBarAccess ... */
  0xec9c033, /* | o Add exclusive flag to MemoryMappedFile                              */
  0x89d938d, /* | o Make sure to not remove a native component twice                    */
  0x99186e5, /* | o Call removeFromDesktop when an external host tries to remove a j... */
  0x1631dac, /* | o whitespace                                                          */
  0x9062ad9, /* | o Windows 10 update touch events bug fix                              */
  0xbc17cb9, /* | o Changed UndoManager so that if a transaction is aborted with und... */
  0xaa15039, /* | o Automatically connect midi input devices on mobile in standalone... */
  0x5ba1723, /* | o Removed the juce_tracktion_marketplace module's dependency on ju... */
  0x3d1b2d6, /* | o Added a cast to some VST code to avoid a C++14 warning              */
  0x1a1897b, /* | o Made the OSX HTTP stream respond to the return value of the open... */
  0x675f594, /* | o fixed silly typo                                                    */
  0xf0a00bf, /* | o AlertWindow::show() only sets window to always on top if there a... */
  0x49b9619, /* | o Added a cast to avoid a warning in Array                            */
  0xbc77b00, /* | o MIDI network session enabled for iOS simulator                      */
  0x17a07a0, /* | o Add a flag to release the focus of a component when the user acc... */
  0xb67b1dd, /* | o Add an issue template for GitHub.                                   */
  0x7a9c22e, /* | o AlertWindow::show() method sets AlertWindow to be always on top ... */
  0xa744cd3, /* | o Added a bool handler for JUCE_LIVE_CONSTANT                         */
  0xc217164, /* | o Add low quality mode for juce CameraDevice                          */
  0x26b6f01, /* | o Replace all "zeros" with "nullptr" in code documentation when re... */
  0xbf582f6, /* | o Fix errors and warnings on mingw-w64                                */
  0x1e41898, /* | o Fix a memory leak in linux ALSA code for MidiInput/MidiOutput       */
  0x1053dc5, /* | o Allow having zero programs in VST                                   */
  0x4cf3c02, /* | o Send parameter changes to processor when received via the EditCo... */
  0x3d1dc4a, /* | o Fix bug where previously applied context transformation was not ... */
  0xe6ad9b5, /* | o Add option to not re-allocate memory when making a copy of an Au... */
  0xb50f941, /* | o First connect controller and component and then load the state i... */
  0xa258295, /* | o Don't repeatedly set the wantsFocus flag if it hasn't changed       */
  0x962cfc7, /* | o Clarify what happens when maxNumChars is zero in the TextPropert... */
  0x9100d1c, /* | o Synthesiser and MPESynthesiser: added an additional parameter to... */
  0x6fdb7e3, /* | o Also add assembler files to MSVC projects                           */
  0x7287830, /* | o Fix AUv3 OS X version compiler error message                        */
  0xf5398fc, /* | o Added additional dontScrollToShowThisRange parameter to selectRa... */
  0x9e1edc7, /* | o Add support for hosting staticly compiled VSTs on all platforms     */
  0x1696c23, /* | o Add support for hosting staticly linked VSTs                        */
  0x88736e3, /* | o Make splash screens fullscreen on mobile platforms                  */
  0x65e1b60, /* | o Add missing overrides in FileTreeComponent                          */
  0x4b64584, /* | o Fix small typo                                                      */
  0x56d5f7b, /* | o Don't update the rendering graph if the graph is not prepared       */
  0x8b1374d, /* | o Don't call processBlock on a AudioProcessorGraph's node if it is... */
  0xcdf899c, /* | o Don't keep on showing the auto-updater dialog box if the user cl... */
  0x035f865, /* | o Fix steppy scrolling if the content component of a viewport was ... */
  0x142ce5e, /* | o Fix a typo in the previous commit                                   */
  0x69cd509, /* | o Add missing getNumSteps override to AudioProcessorValueTree para... */
  0x88803f6, /* | o Don't call methods that require modal loops in Standalone filter... */
  0xe5f306e, /* | o Fix bad property id when registering AU event listener              */
  0xca62e56, /* | o Revert support for hosting your own VST bundles on iOS              */
  0xa8a7fa2, /* | o Added a missing newline in the juce_tracktion_marketplace keygen    */
  0x4998930, /* | o Added expiry time support to the juce_tracktion_marketplace module  */
  0x05835ec, /* | o Generate bitcode bundles on iOS                                     */
  0x9e6988a, /* | o For drag-and-drop on win32, prioritised file-drag mode over text... */
  0x97e058b, /* | o Added a couple of proportional Rectangle methods                    */
  0xd493311, /* | o Only reopen stdout, stderr, stdin if they haven't already been o... */
  0x70f4b0d, /* | o Add more details to the documentation of the setResizable method... */
  0x8b1b855, /* | o Throttle repaints when repainting in a plug-in to avoid steppy a... */
  0x6b8c720, /* | o Tidied up a few things in the Line class, and added a Line::from... */
  0xa604c03, /* | o Added a definition that's missing in some very obscure win32 setups */
  0xebb60c7, /* | o Add missing new line at end of file                                 */
  0x6039a19, /* | o Add new line to end of juce_AAX_Modifier_Injector.h                 */
  0x7600723, /* | o Correctly account for midi ports in Alsa when listing/opening de... */
  0x872f84d, /* | o Check for hash collisions when generating integer parameter ids ... */
  0x5e9c256, /* | o Label attribute of AudioProcessorParamater should be public         */
  0xab3e628, /* | o Added mouse-up event pressure handling                              */
  0xa050e07, /* | o Fix incorrect parameter id <-> index conversion when hosting AUs    */
  0x3ff5ed0, /* | o Reject any inconsistent VST-2 channel layout requests               */
  0x3791939, /* | o Fix unused warning in AU plugin client release build                */
  0x437cb4e, /* | o Only detect which corner is being resized on the first live resi... */
  0x0df194d, /* | o Added method URL::withNewDomainAndPath()                            */
  0xc6249d2, /* | o Clear http headers after redirects                                  */
  0xb439452, /* | o Changed the Array::remove() method to return void, and added an ... */
  0xdd245ef, /* | o Have curl follow re-directions automatically                        */
  0x332dcac, /* | o Added a method Range::expanded()                                    */
  0xc9aca28, /* | o Add pkg-config support for linux                                    */
  0xf83554c, /* | o Add a User-Agent http header to curl requests                       */
  0xc552872, /* | o Never define JUCE_SUPPORT_CARBON on iOS                             */
  0x01c73ff, /* | o Optimisation for look and feel drawing of zero-size title bars      */
  0xa700774, /* | o Tidied up a comment                                                 */
  0xed67e40, /* | o Added a customisable callback object to PopupMenu                   */
  0x0a4b2ae, /* | o Return parent row number if any of the tree view item's paraents... */
  0x3592d61, /* o | Add an issue template for GitHub.                                   */
  0x572868f, /* o | Fix incorrect parameter id <-> index conversion when hosting AUs    */
  0x41ab0d9, /* o | Never define JUCE_SUPPORT_CARBON on iOS                             */
             /* |/                                                                      */
  0x630ab88, /* o Version number update                                                 */
  0xb6d78f8, /* o Avoided an analyser warning on iOS                                    */
  0x89dade2, /* o Fixed a compile problem with the GNU c++ library                      */
  0xe7c9cf3, /* o Made iOS build use updated HTTP API, and turned off deprecation wa... */
  0x2f36d03, /* o Do a case-insensitive compare on core http headers in windows netw... */
  0xb729764, /* o Fix a bug in URL::findEndOfScheme                                     */
  0x6fc1242, /* o Added some constructors to ReferenceCountedObjectPtr and ScopedPoi... */
  0x38c897f, /* o Return nullptr from URL::createInputStream if a request times out     */
  0x2ca0c92, /* o Add beginParameterChangeGesture/endParameterChangeGesture support ... */
  0xf7f35f5, /* o Add fallback to legacy networking code for pre OS X 10.10             */
  0x3461b12, /* o Draw custom line ends if arrow width is zero                          */
  0x300485a, /* o Added a Javascript array indexOf method                               */
  0x31f935c, /* o Added a new Graphics::drawImage method that takes a Rectangle<float>  */
  0x3969000, /* o Avoid a compile error due to steinberg renaming a constant in thei... */
  0x14a9368, /* o AudioDeviceManager::initialiseFromXML(): made the default buffer s... */
  0xe4592d1, /* o Projucer: tweak to the way GCC -D flags are generated                 */
  0x8a9fbc3, /* o Resaved some project files                                            */
  0x5960afd, /* o Cleaned up a few functions in Graphics, and optimised drawEllipse ... */
  0x6e0c168, /* o Removed a defunct workaround for an obscure bug in old Windows PCI... */
  0xd29608a, /* o Minor documentation corrections.                                      */
  0xb46be0f, /* o Change projucer version to match JUCE version                         */
  0xd9a3e76, /* o Fix a warning in the previous commit                                  */
  0xf6cd130, /* o Add redirect support to NSURLSession code                             */
  0x1a40df9, /* o Revert commit d0c238d: Use getParameterText to respond to AU's kAu... */
  0xf7d7f8a, /* o Clear unused outputs when bypassing a plug-in                         */
  0x943c89d, /* o Added some extra locking in the destructor of a Mac HTTP stream im... */
  0x84810b7, /* o Fixed some code that used a C++11 function                            */
  0x1861c82, /* o Add window property to JuceAppStartupDelegate                         */
  0x2be34b3, /* o Added some linux build files to the .gitignore                        */
  0x37cffb7, /* o Added a Javascript array splice method                                */
  0x16913f2, /* o Added a unit test                                                     */
  0x69a7506, /* o Made File remove any redundant "/./" subfolders of paths that it i... */
  0xd0c238d, /* o Use getParameterText to respond to AU's kAudioUnitProperty_Paramet... */
  0x4097fab, /* o Fixed a silly typo!                                                   */
  0x021209e, /* o Changed ImagePixelData::clone to return a smart pointer rather tha... */
  0xf8516f2, /* o Refactored the OSX/iOS HTTP streaming code to use the newer NSURLS... */
  0x95c66f5, /* o Make sure timer is stopped before releasing a VST plugin              */
  0xa33abe6, /* o Added method VSTPluginFormat::createCustomVSTFromMainCall() for lo... */
  0x8b1438a, /* o Made PopupMenu avoid overwriting a custom shortcut key description... */
  0x329e54d, /* o Replace std::move with static_cast to fix errors in old compilers     */
  0x58f2e1b, /* o Add support for isCurrentlyModal to detect if the current componen... */
  0x1f6fa5c, /* o Re-save all projects                                                  */
  0xb1a7441, /* o Align manufacturer code of sample plug-ins in JUCE repo ('ROLI')      */
  0x70ed6eb, /* o Add original four-char code as comment next to hex literal in AppC... */
  0xadfc6e9, /* o Re-save projects                                                      */
  0xb6f4d35, /* o Avoid char literals in auto-generated AppConfig.h files               */
  0x042bd2b, /* o Fix identifying the scheme portion of a URL                           */
  0x72083c1, /* o Do not represent the numpad divide key as a slash key character       */
  0x16fde67, /* o Allow write access to all files if a user has effective root permi... */
  0x074c909, /* o Fix minor typo in OS X function key mappings                          */
  0xbb56ba4, /* o Add CRLF between JUCE http headers and user http headers              */
  0xcc6f0a1, /* o Replace CR and CRLF endings with unix (LF) style endings in xml do... */
  0x4eec614, /* o Remove default keywords from MidiMessageSequence to be compatible ... */
  0x320c5b6, /* o Add removeChannel to AudioChannelSet                                  */
  0x5e0d3fd, /* o Add possibility for an InterprocessConnectionServer to bind to a c... */
  0x6810791, /* o Make sure that the legacy AU rez file is correct for midi effect AUs  */
  0x54e2351, /* o Fix compile error in AAX wrapper                                      */
  0x877d834, /* o Fix compiler error on VS2015 in recent commit                         */
  0x9ecc1e9, /* o Fix an issue when hosting Waves plug-ins                              */
  0xa0259ce, /* o Add support for plug-ins which do not want to use a resize corner     */
  0xc484a9e, /* o Fix some minor warnings on OS X 32-bit                                */
  0x3ddd2db, /* o Allow access to all URLs in the JUCE Demo                             */
  0x8cd6f49, /* o Add program change parameter to VST-3 plug-ins                        */
  0xfc69dc0, /* o Changed addIfNotAlreadyThere method of all array classes to return... */
  0x9481773, /* o Added *.xcscmblueprint to gitignore (a type of file generated by X... */
  0x1a5f71b, /* o Added some logic to MidiDataConcatenator that avoids asserting on ... */
  0xec40f09, /* o Updated version numbers in modules, and resaved example projects      */
  0xf58eda8, /* o Added 32K as a supported ASIO sample rate                             */
  0xe993eb7, /* o Whitespace.                                                           */
  0x920c19d, /* o UnitTest: added new functions expectNotEquals, expectGreaterThan, ... */
  0x58ca067, /* o Avoided spacer components in a ToolBar getting keyboard focus         */
  0x7abbf67, /* o Tweaked the iOS audio device to make the default buffer size play ... */
  0x61e71eb, /* o Add incremental linking option for Visual Studio exporters            */
  0xe8da23d, /* o Enable live comilation for Windows                                    */
  0xb841406, /* o Fixed a compiler warning                                              */
  0xb4e1e68, /* o UnitTest: added method expectWithinAbsoluteError for floating poin... */
  0x74554a2, /* o Improved performance of some BigInteger methods by adding Montgome... */
  0x565d664, /* o Fixed a typo in a comment                                             */
  0xdb7ada5, /* o Avoided compile problems when the JUCE_USE_CUSTOM_AU3_STANDALONE_A... */
  0xdcf9246, /* o Reduced the footprint of MidiMessage, and added some move operator... */
  0x23c361a, /* o Fixed a minor compiler warning                                        */
  0x3852771, /* o Always send VST3 editor parameter changes to the VST3 processor       */
  0x93b8fc1, /* o Reverted MPEZone: added copy constructor and copy assignment opera... */
  0x42a3145, /* o MPEZone: added copy constructor and copy assignment operator.         */
  0xc807cbc, /* o MPEZone: cleaned up and clarified doxygen comments.                   */
  0xcf39ad4, /* o Fix multiple compile errors on older gcc versions                     */
  0x8c99b63, /* o Fixed a warning and a wrong indentation in MidiMessage.               */
  0x0141d0e, /* o Connecting an output node to a processor is an input and vice versa   */
  0x07bc151, /* o MidiMessage: moved two useful helper functions into the public int... */
  0x968cee4, /* o Remove unnecessary code after moving to static shared code library    */
  0x1635f47, /* o Modify buttonColourId and buttonOnColourId to be different colours    */
  0xa714feb, /* o Fix incorrect number of output buses in standalone plug-in client     */
  0x0a040c6, /* o Update avtive ports when opening JACK audio device                    */
  0x0ea9e33, /* o Enable subclassing of StandaloneFilterApp                             */
  0xf5391cd, /* o Only report one midi in & out bus in VST3 wrapper                     */
  0x437b5c2, /* o Third time lucky?                                                     */
  0xb003543, /* o Fix typo in last commit                                               */
  0xc5a7fe6, /* o Add additional checks to enable/disable AUv3 support                  */
  0x9069d70, /* o Fix bug in AudioGraphIOProcessor                                      */
  0xfe14210, /* o Fix incorrect order of includes in VST_wrapper.mm                     */
  0x749bffe, /* o Add missing ifdefs from recent commit                                 */
  0xf11a2b2, /* o Fix modifier keys in AAX plug-ins on Windows 10                       */
  0x54243ef, /* o Support hosting your own VST bundles on iOS                           */
  0xa3c377b, /* o Move kAudioUnitProperty_MaximumFramesPerSlice setting/query after ... */
  0x8df262a, /* o Don't use placement new for MemoryBlock assignment operator in var... */
  0x0d52caf, /* o Make sure AAX wrapper calls releaseResources                          */
  0x24a63ae, /* o If ChildProcess is not interested in stdout/stderr, then redirect ... */
  0xbda4b78, /* o Add missing assignment operator to var class                          */
  0x4e91fd7, /* o Add an extra byte when copying VST2 strings                           */
  0xac9973f, /* o Add support for creating popup menus inside parent components         */
  0x77a8b9e, /* o Fix a few warnings                                                    */
  0x8e53286, /* o Fix some warnings in VS2013 in the recent commits                     */
  0x4205123, /* o Fix an edge-case if the last stored editor window size is zero        */
  0xebb0a15, /* o Add options to make AuidoProcessorEditors resizable with constraints  */
  0x25f8faf, /* o Add windowIgnoresMouseClicks flag to tooltip windows                  */
  0xee7a56a, /* o Fix potential crash if DAW probes non-existent bus                    */
  0xb3ee880, /* o Set kAudioUnitProperty_MaximumFramesPerSlice on iOS                   */
  0x2c3783e, /* o Workaround for loading Izotope Plug-Ins                               */
  0x368ce48, /* o Fix a race condition in the plugin scanner                            */
  0x4c900be, /* o Added some init code to enable stdout, stderr, stdin in the specia... */
  0xc9c6aa0, /* o   Merge branch 'master' into develop                                  */
             /* |\                                                                      */
  0xfe37653, /* | o Fixed JUCE_BUILDNUMBER which was not correctly updated to 4.2.2.    */
  0x1bd850d, /* o | Minor tidy up of yesterday's VST3 preset commit                     */
  0x7eb80b8, /* o | Ensure that AudioUnitUninitialize is called on the message threa... */
  0x5098232, /* o | Tidy up a few warning                                               */
  0x993a2e4, /* o | Do not allow specifying the JUCE folder as a modules folder         */
  0xf4086ad, /* o | Support factory programs in VST3 hosting                            */
  0x93a41cf, /* o | Call setProcessing (false) when resting a VST3 plug-in              */
  0xf0dd881, /* o | Fixed a typo in the projucer's button colour handling               */
  0xf5324af, /* o | Added some defenses against parsing corrupted metadata blocks in... */
  0x1a973a2, /* o | Ignore linux scale factor if it is ten or larger                    */
  0xfc8c628, /* o | Add more detailed comment on why getting the "getPluginFactory" ... */
  0xe12c801, /* o | Always add build scripts - even for Aggregate targets               */
  0x4827003, /* o | Made Component::internalFocusLoss pass on the focus-change cause... */
  0xe13286a, /* o | Minor comment correction                                            */
  0x3aee68e, /* o | Added a method to create a ModalCallbackFunction from a lambda f... */
  0x12eeade, /* o | Added some missing 'override's                                      */
  0x79dca43, /* o | Workaround for VST-3 plug-ins which do not change their paramete... */
  0x0d96a8b, /* o | Avoided some false negatives in the JSON unit tests                 */
  0x6b98901, /* o | Fix link errors when build VST plug-ins with VST 3 hosting support  */
  0x2c3a2ea, /* o | Simplified some demo plugin code to avoid a display update problem  */
  0x09d6409, /* o | Refactored some demo code to avoid a problem when using String::... */
  0xdbf9156, /* o | Fix RTAS support                                                    */
  0x6b6dedb, /* o | Add some minor changes to stay compatible with GCC 4.4              */
  0x7056128, /* o | Add intersect method to AudioChannelSet                             */
  0xaf6198b, /* o | Add support for plug-ins to report the IDs of their parameters      */
             /* |/                                                                      */
  0x2c7a0e0, /* o Fixed a wrong preprocessor macro which caused plugin hosting to br... */
  0xa0835a5, /* o Added a parseFloat method to the Javascript parser                    */
  0xf2bca43, /* o Fix AUv3 version check for iOS                                        */
  0xf36205f, /* o Fixed a couple of comment typos                                       */
  0xe602dd6, /* o Improved some comments for MemoryBlock base-64 methods                */
  0xd1030fa, /* o Projucer: added some missing 'override' specifiers                    */
  0x688ac3b, /* o Updated a comment                                                     */
  0x340f4ce, /* o Android exporter: set buildToolsVersion to 23.0.2                     */
  0xd6ed722, /* o Updated Android Studio build settings in projects.                    */
  0x908e1e9, /* o Android Studio exporter: changed default toolchain back to clang (... */
  0x0162c97, /* o Android exporter: update default settings to gradle-experimental:0... */
  0xd9e3cda, /* o Removed a few references to var::null                                 */
  0xfe7ebbd, /* o Projucer: Updated the default Visual Studio version to 2015           */
  0x708e357, /* o Prevented ColourGradient from having multiple colour stops at 0       */
  0xd32c64d, /* o Avoided an unused variable warning                                    */
  0xaf9491c, /* o Fix typo in previous commit by incorrectly checking the OS X version  */
  0x8ababdd, /* o Fixed the SVG parser to stop it creating too many colour stops in ... */
  0x01e2e53, /* o Added an assertion to catch some unexpected behaviour in the CoreG... */
  0x0cd926a, /* o Fix a bug which stopped multi-bus VST3 synths from working properl... */
  0x7196e57, /* o Don't try to build VST/VST3 hosting code on iOS and add missing he... */
  0x1cbd0a0, /* o Projucer: stopped files with certain extensions (e.g. txt) being a... */
  0xe05e9a4, /* o Added an assertion in AudioThumbnail                                  */
  0xe034958, /* o Added some desktop scale compensation to drag + drop on Windows       */
  0xbf48687, /* o Made the OSX window code handle custom desktop scale factors when ... */
  0xaa3ead5, /* o Resaved a demo component file                                         */
  0x6fc08dd, /* o Added a workaround for Android GL glitches when pausing the app       */
  0x676b27e, /* o Added a MidiDataConcatenator to the linux MIDI implementation, to ... */
  0x0708b7a, /* o Added AAX macro JucePlugin_AAXDisableSaveRestore to set the AAX_eP... */
  0xb9aaa88, /* o Fix for Synthesiser rendering 0 length blocks                         */
  0x5188904, /* o Avoided a conversion warning                                          */
  0xa0795e8, /* o Avoided an overflow in some extreme edge-case uses of WAVAudioFormat  */
  0x49a7ca4, /* o Improved the String::createStringFromData method to fall back to a... */
  0xc1799c9, /* o Fixed a problem with negative values in MidiMessage::getKeySignatu... */
  0x81ff3a8, /* o Projucer: fixed an edge-case where the UI would misbehave if you'r... */
  0xb6e8869, /* o Projucer: Fixed module dependency checker                             */
  0x0c3057e, /* o Re-saved projects.                                                    */
  0x2332c72, /* o Fixed generation of Carbon resources for AU plug-ins (was broken b... */
  0x2682f7d, /* o Updated ChangeLog.txt                                                 */
  0x46a090b, /* o Tweaked the login in the Synthesiser and MPESynthesiserBase classe... */
  0x6ef3f1e, /* o Added asserts to clarify that WebBrowserComponent is not implement... */
  0x0d1f621, /* o Fix crash & assertion when running multi-bus plug-ins in Cakewalk     */
  0x1675f82, /* o Fixed a mistake in BigInteger::inverseModulo()                        */
  0x0b3a11e, /* o Updated version number to 4.2.1                                       */
  0x9da130c, /* o Update comment on estimatedSamplesPerBlock parameter used in the p... */
  0x00dd576, /* o Don't create an asserting VST2 plug-in when creating a new plug-in... */
  0x818287e, /* o Fix assertion when scanning plug-ins on Windows                       */
  0x9678eef, /* o Stripped out some defunct VS2003 code                                 */
  0x6dd2fbe, /* o AUv3: added compiler error if compiling with deployment target < 1... */
  0x0ef8bdb, /* o Android Studio exporter: fixed and refactored the way include path... */
  0x6047817, /* o Android Studio exporter: changed default toolchain to gcc, as clan... */
  0x26d07ea, /* o Android Studio exporter: do not write a toolchainVersion into the ... */
  0x4ef602f, /* o Fix C++-98 compilation errors in AudioPluginFormat code               */
  0xd64396e, /* o Removed rogue file                                                    */
  0x8920e1b, /* o Resaved projects                                                      */
  0xfad8d05, /* o Projucer: improved the set of browsable files that are added to ta... */
  0x1601c90, /* o Fixed some merge-noise in NetworkGraphicsDemo                         */
  0x325aeb1, /* o Resaved projects                                                      */
  0x753c24c, /* o Fix to KnownPluginList::addToMenu ticks                               */
  0x61b2759, /* o Fix plug-in projects with whitespaces in their name                   */
  0xe130ce1, /* o Projucer: fixed a suffix error when generating linux host projects    */
  0x8462849, /* o Re-saved projects.                                                    */
  0xf2c50b8, /* o Android studio exporter: fixed include paths to work with new modu... */
  0x8dfb1c5, /* o JuceDemoPlugin.jucer: removed wrong AAX path that was saved there ... */
  0x71abee4, /* o Android Studio exporter: added option to specify build tools optio... */
  0xbb1fe3d, /* o Android Studio exporter: removed unused build setting.                */
  0x2fa6c20, /* o Fixed bug in Android Ant exporter due to mixed up build settings.     */
  0x1e41e3d, /* o Fixed bug in AAX/RTAS Xcode export: installation paths were mixed up. */
  0x4b1cabb, /* o Minor refactoring, clean-up, whitespace.                              */
  0x6d69f1b, /* o Added new Xcode option to strip of local symbols (off by default).    */
  0xf6efbed, /* o Remove default symbol visibility for JUCE shared code                 */
  0x37073c3, /* o Using static linking instead of a framework for the shared code of... */
  0xf5c19ef, /* o Fixed Android build settings of all .jucer files to use new defaul... */
  0xb94d281, /* o Refactored Android exporters to use CachedValue and get rid of som... */
  0x8f4c4c9, /* o Fixed typo in LADSPA code                                             */
  0x4c0bc70, /* o Deleted some defunct Mac PowerPC code                                 */
  0x42a956c, /* o Added a workaround for an obscure GL race condition when closing c... */
  0x05f9001, /* o Fixed some MSVC warnings in CachedValue                               */
  0x968f838, /* o Added an option to KnownPluginList::addToMenu for specifying a tic... */
  0x3d1cf53, /* o Added colour IDs for ToggleButton tick colour                         */
  0xf74338a, /* o Resaved projects                                                      */
  0xe030a58, /* o Projucer: fixes for binary paths and Android Studio                   */
  0xa439c6a, /* o Added new class: CachedValue                                          */
  0x905533c, /* o Minor formatting tidyups                                              */
  0x45e2b00, /* o Added a method ValueTree::getPropertyPointer()                        */
  0x3c68007, /* o Fixed capitalisation                                                  */
  0x60cd2d7, /* o Projucer: fixed the way the GUI editor generates include paths        */
  0x6fc4d5a, /* o Fixed a bug in the Javascript parser involving array dereferencing    */
  0x486cb0c, /* o Projucer: tidied up an include path in a template file                */
  0x85a5dfb, /* o Projucer: corrected some help text                                    */
  0x8343f34, /* o Found a few missing 'override' specifiers                             */
  0x6d03314, /* o Projucer: updated EULA                                                */
  0x59f2885, /* o Minor refactoring for a bit more clarity in the deprecated Xcode p... */
  0x367f6ee, /* o Fixed bug with wrong checksum for deprecated Xcode post-build script. */
  0x54d3cfa, /* o Projucer: Fixed a bug that could cause focus to seemingly jump aro... */
  0x544b8d0, /* o Simplified some Projucer GUI editor code + added a missing slider ... */
  0x06d8d41, /* o Added a method to StandaloneFilterApp to allow customisation of th... */
  0xeffb421, /* o Projucer: Updated some font use and removed the EULA if running wi... */
  0x025d04c, /* o Projucer: Added warning when trying to load projects with out-of-d... */
  0x871c3a9, /* o Removed some code that prevented RTL TextLayouts having their boun... */
  0xbb7189b, /* o Added kerning support to the DirectWrite layout code                  */
  0x48b35c4, /* o Workaround for long filenames in Process::openDocument on OSX         */
  0x82350ba, /* o Re-saved Projucer with itself.                                        */
  0x68451ee, /* o Projucer: fixed bug where whitespace in the "additional frameworks... */
  0xcaffbba, /* o Projucer: better workaround missing framework symbols: adding fram... */
  0xc096b1a, /* o Reverted: Projucer: workaround missing framework symbols during Li... */
  0xe04b7ff, /* o Added some extra time-out settings to the Linux CURL code             */
  0xe1205ec, /* o Re-added a WaveLab workaround in the VST wrapper                      */
  0xbe3acc9, /* o Tidied up an out of date method in LADSPAPluginFormat                 */
  0xd80ceb0, /* o PlugInSamples: re-saved projects to fix accidentally added local A... */
  0xd9b0b53, /* o Corrected a rogue header comment                                      */
  0x7d64417, /* o Re-saved all audio plug-in projects.                                  */
  0x0eb0f27, /* o Fixed Carbon host support for AudioUnit v2 by fixing the Projucer'... */
  0xef5aefe, /* o Re-saved all example projects that have AAX enabled by default.       */
  0x92a1747, /* o Fixed AAX plug-in builds with new module format on OS X.              */
  0x5fcea26, /* o Projucer: workaround missing framework symbols during Live build: ... */
  0x3b65c5c, /* o Projucer AppearanceSettingsTab: added missing resized() function; ... */
  0xc08375a, /* o added *.VC.db to .gitignore to ignore some files generated by Visu... */
  0xe9edecc, /* o Resaved projects                                                      */
  0xe1098c8, /* o Projucer: Fixed failure to scan deep file hierarchies when buildin... */
  0x9abd4ee, /* o Added some extra tolerance settings to some Path methods              */
  0x0a0c550, /* o Projucer: made sure that RTAS files are built with stdcall in MSVC    */
  0xb6261b9, /* o .gitignore: added JUCECompileEngine.dylib to ignored files in case... */
  0xfb864a5, /* o Corrected some spelling mistakes in comments                          */
  0x7d178fe, /* o (Fixed mistake in last commit)                                        */
  0x32178f4, /* o Tidied up some AUv3 code                                              */
  0x5049bab, /* o Projucer: Fixed a typo that prevented the OSXLibs setting working     */
  0x15f4789, /* o Projucer: Added a sanity-check to prevent compilation if some modu... */
  0x23b1d89, /* o Projucer: made the login form remember the last username              */
  0xbc58656, /* o Projucer: made build panel UI update when signing out                 */
  0x1182aec, /* o Improved a comment                                                    */
  0xc34f13a, /* o Fixed a linkage problem in some release plugin builds                 */
  0x26d6cb9, /* o Fixed a few integer conversion warnings.                              */
  0x17e2acb, /* o Fixed a couple of AUv3 bugs                                           */
  0x9dda845, /* o Changed Projucer deployment target to 10.9 in Debug config (was 'd... */
  0x63ef9c1, /* o Fix compilation errors in AU format when compiling for older OS ta... */
  0xcdeb0b5, /* o Undo removal of setNumThreads in plug-in scanner                      */
  0x44359cb, /* o Fix missing std::nullptr defines                                      */
  0x24f05c4, /* o Whitespace.                                                           */
  0x9066ed4, /* o Projucer: fixed unreachable code warning in ProjectContentComponen... */
  0xc77fea9, /* o Projucer Visual Studio: switched to default platform toolset witho... */
  0x70949aa, /* o Added several new features: support for AudioUnit v3, new simplifi... */
};
