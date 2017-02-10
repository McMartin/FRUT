# Copyright (c) 2016 Alain Martin
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required(VERSION 3.0)


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../cmake")
include(Reprojucer)


set(JUCE_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../../JUCE" CACHE PATH "Path to JUCE")

set(Projucer_jucer_FILE "${JUCE_ROOT}/extras/Projucer/Projucer.jucer"
  CACHE PATH "Path to Projucer.jucer"
)

get_filename_component(Projucer_jucer_DIR "${Projucer_jucer_FILE}" DIRECTORY)

file(COPY "${Projucer_jucer_DIR}/Source" DESTINATION "${CMAKE_CURRENT_LIST_DIR}")


jucer_project_begin(
  PROJECT_NAME "Projucer"
  PROJECT_VERSION "4.3.0"
  # COMPANY_NAME
  # COMPANY_WEBSITE
  # COMPANY_EMAIL
  PROJECT_TYPE "GUI Application"
  BUNDLE_IDENTIFIER "com.juce.theprojucer"
  BINARYDATACPP_SIZE_LIMIT "Default"
  # PREPROCESSOR_DEFINITIONS
  PROJECT_ID "M70qfTRRk"
)

jucer_project_files("Projucer/Application"
  "Source/Application/jucer_AppearanceSettings.cpp"
  "Source/Application/jucer_AppearanceSettings.h"
  # "Source/Application/jucer_Application.cpp"
  "Source/Application/jucer_Application.h"
  "Source/Application/jucer_AutoUpdater.cpp"
  "Source/Application/jucer_AutoUpdater.h"
  "Source/Application/jucer_CommandIDs.h"
  "Source/Application/jucer_CommandLine.cpp"
  "Source/Application/jucer_CommandLine.h"
  "Source/Application/jucer_CommonHeaders.h"
  "Source/Application/jucer_DocumentEditorComponent.cpp"
  "Source/Application/jucer_DocumentEditorComponent.h"
  "Source/Application/jucer_DownloadCompileEngineThread.cpp"
  "Source/Application/jucer_DownloadCompileEngineThread.h"
  "Source/Application/jucer_EulaDialogue.h"
  "Source/Application/jucer_FilePreviewComponent.h"
  "Source/Application/jucer_GlobalPreferences.cpp"
  "Source/Application/jucer_GlobalPreferences.h"
  "Source/Application/jucer_LoginForm.h"
  "Source/Application/jucer_Main.cpp"
  "Source/Application/jucer_MainWindow.cpp"
  "Source/Application/jucer_MainWindow.h"
  "Source/Application/jucer_OpenDocumentManager.cpp"
  "Source/Application/jucer_OpenDocumentManager.h"
  "Source/Application/jucer_ProjucerLicenses.h"
  "Source/jucer_Headers.h"
)

jucer_project_files("Projucer/LiveBuildEngine"
  "Source/LiveBuildEngine/projucer_ActivityList.h"
  "Source/LiveBuildEngine/projucer_ActivityListComponent.h"
  "Source/LiveBuildEngine/projucer_BuildTabStatusComp.h"
  "Source/LiveBuildEngine/projucer_ClassDatabase.h"
  "Source/LiveBuildEngine/projucer_ClientServerMessages.h"
  "Source/LiveBuildEngine/projucer_CompileEngineClient.cpp"
  "Source/LiveBuildEngine/projucer_CompileEngineClient.h"
  "Source/LiveBuildEngine/projucer_CompileEngineDLL.h"
  "Source/LiveBuildEngine/projucer_CompileEngineServer.cpp"
  "Source/LiveBuildEngine/projucer_CompileEngineServer.h"
  "Source/LiveBuildEngine/projucer_ComponentListComp.h"
  "Source/LiveBuildEngine/projucer_CppHelpers.h"
  "Source/LiveBuildEngine/projucer_DiagnosticMessage.h"
  "Source/LiveBuildEngine/projucer_ErrorList.h"
  "Source/LiveBuildEngine/projucer_ErrorListComponent.h"
  "Source/LiveBuildEngine/projucer_LiveCodeBuilderDLL.h"
  "Source/LiveBuildEngine/projucer_MessageIDs.h"
  "Source/LiveBuildEngine/projucer_ProjectBuildInfo.h"
  "Source/LiveBuildEngine/projucer_SourceCodeRange.h"
)

jucer_project_files("Projucer/Code Editor"
  "Source/Code Editor/jucer_LiveBuildCodeEditor.h"
  "Source/Code Editor/jucer_SourceCodeEditor.cpp"
  "Source/Code Editor/jucer_SourceCodeEditor.h"
)

jucer_project_files("Projucer/ComponentEditor"
  "Source/ComponentEditor/jucer_BinaryResources.cpp"
  "Source/ComponentEditor/jucer_BinaryResources.h"
  "Source/ComponentEditor/jucer_ComponentLayout.cpp"
  "Source/ComponentEditor/jucer_ComponentLayout.h"
  "Source/ComponentEditor/jucer_GeneratedCode.cpp"
  "Source/ComponentEditor/jucer_GeneratedCode.h"
  "Source/ComponentEditor/jucer_JucerDocument.cpp"
  "Source/ComponentEditor/jucer_JucerDocument.h"
  "Source/ComponentEditor/jucer_ObjectTypes.cpp"
  "Source/ComponentEditor/jucer_ObjectTypes.h"
  "Source/ComponentEditor/jucer_PaintRoutine.cpp"
  "Source/ComponentEditor/jucer_PaintRoutine.h"
  "Source/ComponentEditor/jucer_UtilityFunctions.h"
)

jucer_project_files("Projucer/ComponentEditor/components"
  "Source/ComponentEditor/components/jucer_ButtonHandler.h"
  "Source/ComponentEditor/components/jucer_ComboBoxHandler.h"
  "Source/ComponentEditor/components/jucer_ComponentNameProperty.h"
  "Source/ComponentEditor/components/jucer_ComponentTypeHandler.cpp"
  "Source/ComponentEditor/components/jucer_ComponentTypeHandler.h"
  "Source/ComponentEditor/components/jucer_ComponentUndoableAction.h"
  "Source/ComponentEditor/components/jucer_GenericComponentHandler.h"
  "Source/ComponentEditor/components/jucer_GroupComponentHandler.h"
  "Source/ComponentEditor/components/jucer_HyperlinkButtonHandler.h"
  "Source/ComponentEditor/components/jucer_ImageButtonHandler.h"
  "Source/ComponentEditor/components/jucer_JucerComponentHandler.h"
  "Source/ComponentEditor/components/jucer_LabelHandler.h"
  "Source/ComponentEditor/components/jucer_SliderHandler.h"
  "Source/ComponentEditor/components/jucer_TabbedComponentHandler.h"
  "Source/ComponentEditor/components/jucer_TextButtonHandler.h"
  "Source/ComponentEditor/components/jucer_TextEditorHandler.h"
  "Source/ComponentEditor/components/jucer_ToggleButtonHandler.h"
  "Source/ComponentEditor/components/jucer_TreeViewHandler.h"
  "Source/ComponentEditor/components/jucer_ViewportHandler.h"
)

jucer_project_files("Projucer/ComponentEditor/documents"
  "Source/ComponentEditor/documents/jucer_ButtonDocument.cpp"
  "Source/ComponentEditor/documents/jucer_ButtonDocument.h"
  "Source/ComponentEditor/documents/jucer_ComponentDocument.cpp"
  "Source/ComponentEditor/documents/jucer_ComponentDocument.h"
)

jucer_project_files("Projucer/ComponentEditor/paintelements"
  "Source/ComponentEditor/paintelements/jucer_ColouredElement.cpp"
  "Source/ComponentEditor/paintelements/jucer_ColouredElement.h"
  "Source/ComponentEditor/paintelements/jucer_ElementSiblingComponent.h"
  "Source/ComponentEditor/paintelements/jucer_FillType.h"
  "Source/ComponentEditor/paintelements/jucer_GradientPointComponent.h"
  "Source/ComponentEditor/paintelements/jucer_ImageResourceProperty.h"
  "Source/ComponentEditor/paintelements/jucer_PaintElement.cpp"
  "Source/ComponentEditor/paintelements/jucer_PaintElement.h"
  "Source/ComponentEditor/paintelements/jucer_PaintElementEllipse.h"
  "Source/ComponentEditor/paintelements/jucer_PaintElementGroup.h"
  "Source/ComponentEditor/paintelements/jucer_PaintElementImage.h"
  "Source/ComponentEditor/paintelements/jucer_PaintElementPath.cpp"
  "Source/ComponentEditor/paintelements/jucer_PaintElementPath.h"
  "Source/ComponentEditor/paintelements/jucer_PaintElementRectangle.h"
  "Source/ComponentEditor/paintelements/jucer_PaintElementRoundedRectangle.h"
  "Source/ComponentEditor/paintelements/jucer_PaintElementText.h"
  "Source/ComponentEditor/paintelements/jucer_PaintElementUndoableAction.h"
  "Source/ComponentEditor/paintelements/jucer_PointComponent.h"
  "Source/ComponentEditor/paintelements/jucer_StrokeType.h"
)

jucer_project_files("Projucer/ComponentEditor/properties"
  "Source/ComponentEditor/properties/jucer_ColourPropertyComponent.h"
  "Source/ComponentEditor/properties/jucer_ComponentBooleanProperty.h"
  "Source/ComponentEditor/properties/jucer_ComponentChoiceProperty.h"
  "Source/ComponentEditor/properties/jucer_ComponentColourProperty.h"
  "Source/ComponentEditor/properties/jucer_ComponentTextProperty.h"
  "Source/ComponentEditor/properties/jucer_FilePropertyComponent.h"
  "Source/ComponentEditor/properties/jucer_FontPropertyComponent.h"
  "Source/ComponentEditor/properties/jucer_JustificationProperty.h"
  "Source/ComponentEditor/properties/jucer_PositionPropertyBase.h"
)

jucer_project_files("Projucer/ComponentEditor/ui"
  "Source/ComponentEditor/ui/jucer_ComponentLayoutEditor.cpp"
  "Source/ComponentEditor/ui/jucer_ComponentLayoutEditor.h"
  "Source/ComponentEditor/ui/jucer_ComponentLayoutPanel.h"
  "Source/ComponentEditor/ui/jucer_ComponentOverlayComponent.cpp"
  "Source/ComponentEditor/ui/jucer_ComponentOverlayComponent.h"
  "Source/ComponentEditor/ui/jucer_EditingPanelBase.cpp"
  "Source/ComponentEditor/ui/jucer_EditingPanelBase.h"
  "Source/ComponentEditor/ui/jucer_JucerCommandIDs.h"
  "Source/ComponentEditor/ui/jucer_JucerDocumentEditor.cpp"
  "Source/ComponentEditor/ui/jucer_JucerDocumentEditor.h"
  "Source/ComponentEditor/ui/jucer_PaintRoutineEditor.cpp"
  "Source/ComponentEditor/ui/jucer_PaintRoutineEditor.h"
  "Source/ComponentEditor/ui/jucer_PaintRoutinePanel.cpp"
  "Source/ComponentEditor/ui/jucer_PaintRoutinePanel.h"
  "Source/ComponentEditor/ui/jucer_RelativePositionedRectangle.h"
  "Source/ComponentEditor/ui/jucer_ResourceEditorPanel.cpp"
  "Source/ComponentEditor/ui/jucer_ResourceEditorPanel.h"
  "Source/ComponentEditor/ui/jucer_SnapGridPainter.h"
  "Source/ComponentEditor/ui/jucer_TestComponent.cpp"
  "Source/ComponentEditor/ui/jucer_TestComponent.h"
)

jucer_project_files("Projucer/Project"
  "Source/Project/jucer_ConfigTree_Base.h"
  "Source/Project/jucer_ConfigTree_Exporter.h"
  "Source/Project/jucer_ConfigTree_Modules.h"
  "Source/Project/jucer_DependencyPathPropertyComponent.cpp"
  "Source/Project/jucer_DependencyPathPropertyComponent.h"
  "Source/Project/jucer_GroupInformationComponent.h"
  "Source/Project/jucer_Module.cpp"
  "Source/Project/jucer_Module.h"
  "Source/Project/jucer_ModulesPanel.h"
  "Source/Project/jucer_Project.cpp"
  "Source/Project/jucer_Project.h"
  # "Source/Project/jucer_ProjectContentComponent.cpp"
  "Source/Project/jucer_ProjectContentComponent.h"
  "Source/Project/jucer_ProjectTree_Base.h"
  "Source/Project/jucer_ProjectTree_File.h"
  "Source/Project/jucer_ProjectTree_Group.h"
  "Source/Project/jucer_ProjectType.h"
  "Source/Project/jucer_TreeItemTypes.h"
)

jucer_project_files("Projucer/Project Saving"
  "Source/Project Saving/jucer_ProjectExport_AndroidAnt.h"
  "Source/Project Saving/jucer_ProjectExport_AndroidBase.h"
  "Source/Project Saving/jucer_ProjectExport_AndroidStudio.h"
  "Source/Project Saving/jucer_ProjectExport_CodeBlocks.h"
  "Source/Project Saving/jucer_ProjectExport_Make.h"
  "Source/Project Saving/jucer_ProjectExport_MSVC.h"
  "Source/Project Saving/jucer_ProjectExport_XCode.h"
  "Source/Project Saving/jucer_ProjectExporter.cpp"
  "Source/Project Saving/jucer_ProjectExporter.h"
  "Source/Project Saving/jucer_ProjectSaver.cpp"
  "Source/Project Saving/jucer_ProjectSaver.h"
  "Source/Project Saving/jucer_ResourceFile.cpp"
  "Source/Project Saving/jucer_ResourceFile.h"
)

jucer_project_files("Projucer/Utility"
  "Source/Utility/jucer_CodeHelpers.cpp"
  "Source/Utility/jucer_CodeHelpers.h"
  "Source/Utility/jucer_ColourPropertyComponent.h"
  "Source/Utility/jucer_Colours.h"
  "Source/Utility/jucer_DialogLookAndFeel.h"
  "Source/Utility/jucer_FileHelpers.cpp"
  "Source/Utility/jucer_FileHelpers.h"
  "Source/Utility/jucer_FilePathPropertyComponent.h"
  "Source/Utility/jucer_FloatingToolWindow.h"
  "Source/Utility/jucer_Icons.cpp"
  "Source/Utility/jucer_Icons.h"
  "Source/Utility/jucer_JucerTreeViewBase.cpp"
  "Source/Utility/jucer_JucerTreeViewBase.h"
  "Source/Utility/jucer_MiscUtilities.cpp"
  "Source/Utility/jucer_MiscUtilities.h"
  "Source/Utility/jucer_PresetIDs.h"
  "Source/Utility/jucer_ProjucerLookAndFeel.cpp"
  "Source/Utility/jucer_ProjucerLookAndFeel.h"
  "Source/Utility/jucer_RelativePath.h"
  "Source/Utility/jucer_SlidingPanelComponent.cpp"
  "Source/Utility/jucer_SlidingPanelComponent.h"
  "Source/Utility/jucer_StoredSettings.cpp"
  "Source/Utility/jucer_StoredSettings.h"
  "Source/Utility/jucer_SVGPathDataComponent.h"
  "Source/Utility/jucer_TranslationTool.h"
  "Source/Utility/jucer_UTF8Component.h"
  "Source/Utility/jucer_ValueSourceHelpers.h"
)

jucer_project_files("Projucer/Wizards"
  "Source/Wizards/jucer_NewFileWizard.cpp"
  "Source/Wizards/jucer_NewFileWizard.h"
  "Source/Wizards/jucer_NewProjectWizard.h"
  "Source/Wizards/jucer_NewProjectWizardClasses.cpp"
  "Source/Wizards/jucer_NewProjectWizardClasses.h"
  "Source/Wizards/jucer_NewProjectWizardComponent.h"
  "Source/Wizards/jucer_ProjectWizard_Animated.h"
  "Source/Wizards/jucer_ProjectWizard_AudioApp.h"
  "Source/Wizards/jucer_ProjectWizard_AudioPlugin.h"
  "Source/Wizards/jucer_ProjectWizard_Blank.h"
  "Source/Wizards/jucer_ProjectWizard_Console.h"
  "Source/Wizards/jucer_ProjectWizard_DLL.h"
  "Source/Wizards/jucer_ProjectWizard_GUIApp.h"
  "Source/Wizards/jucer_ProjectWizard_openGL.h"
  "Source/Wizards/jucer_ProjectWizard_StaticLibrary.h"
  "Source/Wizards/jucer_StartPageComponent.h"
  "Source/Wizards/jucer_TemplateThumbnailsComponent.h"
)

jucer_project_resources("Projucer/BinaryData/gradle"
  "Source/BinaryData/gradle/gradle-wrapper.jar"
  "Source/BinaryData/gradle/gradlew"
  "Source/BinaryData/gradle/gradlew.bat"
  "Source/BinaryData/gradle/LICENSE"
)

jucer_project_resources("Projucer/BinaryData/templates"
  "Source/BinaryData/jucer_AnimatedComponentTemplate.cpp"
  "Source/BinaryData/jucer_AudioComponentTemplate.cpp"
  "Source/BinaryData/jucer_AudioPluginEditorTemplate.cpp"
  "Source/BinaryData/jucer_AudioPluginEditorTemplate.h"
  "Source/BinaryData/jucer_AudioPluginFilterTemplate.cpp"
  "Source/BinaryData/jucer_AudioPluginFilterTemplate.h"
  "Source/BinaryData/jucer_ComponentTemplate.cpp"
  "Source/BinaryData/jucer_ComponentTemplate.h"
  "Source/BinaryData/jucer_ContentCompTemplate.cpp"
  "Source/BinaryData/jucer_ContentCompTemplate.h"
  "Source/BinaryData/jucer_InlineComponentTemplate.h"
  "Source/BinaryData/jucer_MainConsoleAppTemplate.cpp"
  "Source/BinaryData/jucer_MainTemplate_NoWindow.cpp"
  "Source/BinaryData/jucer_MainTemplate_SimpleWindow.cpp"
  "Source/BinaryData/jucer_MainTemplate_Window.cpp"
  "Source/BinaryData/jucer_NewComponentTemplate.cpp"
  "Source/BinaryData/jucer_NewComponentTemplate.h"
  "Source/BinaryData/jucer_NewCppFileTemplate.cpp"
  "Source/BinaryData/jucer_NewCppFileTemplate.h"
  "Source/BinaryData/jucer_NewInlineComponentTemplate.h"
  "Source/BinaryData/jucer_OpenGLComponentTemplate.cpp"
)

jucer_project_resources("Projucer/BinaryData"
  "Source/BinaryData/background_logo.svg"
  "Source/BinaryData/background_tile.png"
  "Source/BinaryData/colourscheme_dark.xml"
  "Source/BinaryData/colourscheme_light.xml"
  "Source/BinaryData/juce_icon.png"
  "Source/BinaryData/projectIconAndroid.png"
  "Source/BinaryData/projectIconCodeblocks.png"
  "Source/BinaryData/projectIconLinuxMakefile.png"
  "Source/BinaryData/projectIconVisualStudio.png"
  "Source/BinaryData/projectIconXcode.png"
  "Source/BinaryData/projectIconXcodeIOS.png"
  "Source/BinaryData/projucer_EULA.txt"
  "Source/BinaryData/projucer_login_bg.svg"
  "Source/BinaryData/RecentFilesMenuTemplate.nib"
  "Source/BinaryData/wizard_AnimatedApp.svg"
  "Source/BinaryData/wizard_AudioApp.svg"
  "Source/BinaryData/wizard_AudioPlugin.svg"
  "Source/BinaryData/wizard_ConsoleApp.svg"
  "Source/BinaryData/wizard_DLL.svg"
  "Source/BinaryData/wizard_GUI.svg"
  "Source/BinaryData/wizard_Highlight.svg"
  "Source/BinaryData/wizard_Openfile.svg"
  "Source/BinaryData/wizard_OpenGL.svg"
  "Source/BinaryData/wizard_StaticLibrary.svg"
)

jucer_project_module(
  juce_core
  PATH "${Projucer_jucer_DIR}/../../modules"
  # JUCE_FORCE_DEBUG
  JUCE_LOG_ASSERTIONS ON
  # JUCE_CHECK_MEMORY_LEAKS
  # JUCE_DONT_AUTOLINK_TO_WIN32_LIBRARIES
  # JUCE_INCLUDE_ZLIB_CODE
  JUCE_USE_CURL ON
  # JUCE_CATCH_UNHANDLED_EXCEPTIONS
  # JUCE_ALLOW_STATIC_NULL_VARIABLES
)

jucer_project_module(
  juce_cryptography
  PATH "${Projucer_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_data_structures
  PATH "${Projucer_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_events
  PATH "${Projucer_jucer_DIR}/../../modules"
)

jucer_project_module(
  juce_graphics
  PATH "${Projucer_jucer_DIR}/../../modules"
  # JUCE_USE_COREIMAGE_LOADER
  # JUCE_USE_DIRECTWRITE
)

jucer_project_module(
  juce_gui_basics
  PATH "${Projucer_jucer_DIR}/../../modules"
  # JUCE_ENABLE_REPAINT_DEBUGGING
  # JUCE_USE_XSHM
  # JUCE_USE_XRENDER
  # JUCE_USE_XCURSOR
)

jucer_project_module(
  juce_gui_extra
  PATH "${Projucer_jucer_DIR}/../../modules"
  # JUCE_WEB_BROWSER
  # JUCE_ENABLE_LIVE_CONSTANT_EDITOR
)

jucer_project_end()
