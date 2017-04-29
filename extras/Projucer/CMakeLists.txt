# Copyright (c) 2016 Alain Martin
#
# This file is part of JUCE.cmake.
#
# JUCE.cmake is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# JUCE.cmake is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with JUCE.cmake.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.4)


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../cmake")
include(Reprojucer)


if(NOT DEFINED Projucer_jucer_FILE)
  message(FATAL_ERROR "Projucer_jucer_FILE must be defined")
endif()

get_filename_component(Projucer_jucer_FILE
  "${Projucer_jucer_FILE}" ABSOLUTE
  BASE_DIR "${CMAKE_CURRENT_BINARY_DIR}"
)

if(NOT EXISTS "${Projucer_jucer_FILE}")
  message(FATAL_ERROR "No such file: ${Projucer_jucer_FILE}")
endif()

get_filename_component(Projucer_jucer_DIR
  "${Projucer_jucer_FILE}" DIRECTORY
)


jucer_project_begin(
  PROJECT_NAME "Projucer"
  PROJECT_VERSION "4.3.0"
  # COMPANY_NAME
  # COMPANY_WEBSITE
  # COMPANY_EMAIL
  PROJECT_TYPE "GUI Application"
  BUNDLE_IDENTIFIER "com.juce.theprojucer"
  BINARYDATACPP_SIZE_LIMIT "Default"
  # BINARYDATA_NAMESPACE
  # PREPROCESSOR_DEFINITIONS
  PROJECT_ID "M70qfTRRk"
)

jucer_project_files("Projucer/Application"
  "${Projucer_jucer_DIR}/Source/Application/jucer_AppearanceSettings.cpp"
  "${Projucer_jucer_DIR}/Source/Application/jucer_AppearanceSettings.h"
  # "${Projucer_jucer_DIR}/Source/Application/jucer_Application.cpp"
  "${Projucer_jucer_DIR}/Source/Application/jucer_Application.h"
  "${Projucer_jucer_DIR}/Source/Application/jucer_AutoUpdater.cpp"
  "${Projucer_jucer_DIR}/Source/Application/jucer_AutoUpdater.h"
  "${Projucer_jucer_DIR}/Source/Application/jucer_CommandIDs.h"
  "${Projucer_jucer_DIR}/Source/Application/jucer_CommandLine.cpp"
  "${Projucer_jucer_DIR}/Source/Application/jucer_CommandLine.h"
  "${Projucer_jucer_DIR}/Source/Application/jucer_CommonHeaders.h"
  "${Projucer_jucer_DIR}/Source/Application/jucer_DocumentEditorComponent.cpp"
  "${Projucer_jucer_DIR}/Source/Application/jucer_DocumentEditorComponent.h"
  "${Projucer_jucer_DIR}/Source/Application/jucer_DownloadCompileEngineThread.cpp"
  "${Projucer_jucer_DIR}/Source/Application/jucer_DownloadCompileEngineThread.h"
  "${Projucer_jucer_DIR}/Source/Application/jucer_EulaDialogue.h"
  "${Projucer_jucer_DIR}/Source/Application/jucer_FilePreviewComponent.h"
  "${Projucer_jucer_DIR}/Source/Application/jucer_GlobalPreferences.cpp"
  "${Projucer_jucer_DIR}/Source/Application/jucer_GlobalPreferences.h"
  "${Projucer_jucer_DIR}/Source/Application/jucer_LoginForm.h"
  "${Projucer_jucer_DIR}/Source/Application/jucer_Main.cpp"
  "${Projucer_jucer_DIR}/Source/Application/jucer_MainWindow.cpp"
  "${Projucer_jucer_DIR}/Source/Application/jucer_MainWindow.h"
  "${Projucer_jucer_DIR}/Source/Application/jucer_OpenDocumentManager.cpp"
  "${Projucer_jucer_DIR}/Source/Application/jucer_OpenDocumentManager.h"
  "${Projucer_jucer_DIR}/Source/Application/jucer_ProjucerLicenses.h"
  "${Projucer_jucer_DIR}/Source/jucer_Headers.h"
)

jucer_project_files("Projucer/LiveBuildEngine"
  "${Projucer_jucer_DIR}/Source/LiveBuildEngine/projucer_ActivityList.h"
  "${Projucer_jucer_DIR}/Source/LiveBuildEngine/projucer_ActivityListComponent.h"
  "${Projucer_jucer_DIR}/Source/LiveBuildEngine/projucer_BuildTabStatusComp.h"
  "${Projucer_jucer_DIR}/Source/LiveBuildEngine/projucer_ClassDatabase.h"
  "${Projucer_jucer_DIR}/Source/LiveBuildEngine/projucer_ClientServerMessages.h"
  "${Projucer_jucer_DIR}/Source/LiveBuildEngine/projucer_CompileEngineClient.cpp"
  "${Projucer_jucer_DIR}/Source/LiveBuildEngine/projucer_CompileEngineClient.h"
  "${Projucer_jucer_DIR}/Source/LiveBuildEngine/projucer_CompileEngineDLL.h"
  "${Projucer_jucer_DIR}/Source/LiveBuildEngine/projucer_CompileEngineServer.cpp"
  "${Projucer_jucer_DIR}/Source/LiveBuildEngine/projucer_CompileEngineServer.h"
  "${Projucer_jucer_DIR}/Source/LiveBuildEngine/projucer_ComponentListComp.h"
  "${Projucer_jucer_DIR}/Source/LiveBuildEngine/projucer_CppHelpers.h"
  "${Projucer_jucer_DIR}/Source/LiveBuildEngine/projucer_DiagnosticMessage.h"
  "${Projucer_jucer_DIR}/Source/LiveBuildEngine/projucer_ErrorList.h"
  "${Projucer_jucer_DIR}/Source/LiveBuildEngine/projucer_ErrorListComponent.h"
  "${Projucer_jucer_DIR}/Source/LiveBuildEngine/projucer_LiveCodeBuilderDLL.h"
  "${Projucer_jucer_DIR}/Source/LiveBuildEngine/projucer_MessageIDs.h"
  "${Projucer_jucer_DIR}/Source/LiveBuildEngine/projucer_ProjectBuildInfo.h"
  "${Projucer_jucer_DIR}/Source/LiveBuildEngine/projucer_SourceCodeRange.h"
)

jucer_project_files("Projucer/Code Editor"
  "${Projucer_jucer_DIR}/Source/Code Editor/jucer_LiveBuildCodeEditor.h"
  "${Projucer_jucer_DIR}/Source/Code Editor/jucer_SourceCodeEditor.cpp"
  "${Projucer_jucer_DIR}/Source/Code Editor/jucer_SourceCodeEditor.h"
)

jucer_project_files("Projucer/ComponentEditor"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/jucer_BinaryResources.cpp"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/jucer_BinaryResources.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/jucer_ComponentLayout.cpp"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/jucer_ComponentLayout.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/jucer_GeneratedCode.cpp"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/jucer_GeneratedCode.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/jucer_JucerDocument.cpp"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/jucer_JucerDocument.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/jucer_ObjectTypes.cpp"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/jucer_ObjectTypes.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/jucer_PaintRoutine.cpp"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/jucer_PaintRoutine.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/jucer_UtilityFunctions.h"
)

jucer_project_files("Projucer/ComponentEditor/components"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/components/jucer_ButtonHandler.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/components/jucer_ComboBoxHandler.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/components/jucer_ComponentNameProperty.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/components/jucer_ComponentTypeHandler.cpp"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/components/jucer_ComponentTypeHandler.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/components/jucer_ComponentUndoableAction.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/components/jucer_GenericComponentHandler.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/components/jucer_GroupComponentHandler.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/components/jucer_HyperlinkButtonHandler.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/components/jucer_ImageButtonHandler.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/components/jucer_JucerComponentHandler.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/components/jucer_LabelHandler.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/components/jucer_SliderHandler.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/components/jucer_TabbedComponentHandler.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/components/jucer_TextButtonHandler.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/components/jucer_TextEditorHandler.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/components/jucer_ToggleButtonHandler.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/components/jucer_TreeViewHandler.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/components/jucer_ViewportHandler.h"
)

jucer_project_files("Projucer/ComponentEditor/documents"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/documents/jucer_ButtonDocument.cpp"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/documents/jucer_ButtonDocument.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/documents/jucer_ComponentDocument.cpp"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/documents/jucer_ComponentDocument.h"
)

jucer_project_files("Projucer/ComponentEditor/paintelements"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/paintelements/jucer_ColouredElement.cpp"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/paintelements/jucer_ColouredElement.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/paintelements/jucer_ElementSiblingComponent.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/paintelements/jucer_FillType.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/paintelements/jucer_GradientPointComponent.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/paintelements/jucer_ImageResourceProperty.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/paintelements/jucer_PaintElement.cpp"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/paintelements/jucer_PaintElement.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/paintelements/jucer_PaintElementEllipse.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/paintelements/jucer_PaintElementGroup.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/paintelements/jucer_PaintElementImage.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/paintelements/jucer_PaintElementPath.cpp"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/paintelements/jucer_PaintElementPath.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/paintelements/jucer_PaintElementRectangle.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/paintelements/jucer_PaintElementRoundedRectangle.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/paintelements/jucer_PaintElementText.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/paintelements/jucer_PaintElementUndoableAction.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/paintelements/jucer_PointComponent.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/paintelements/jucer_StrokeType.h"
)

jucer_project_files("Projucer/ComponentEditor/properties"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/properties/jucer_ColourPropertyComponent.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/properties/jucer_ComponentBooleanProperty.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/properties/jucer_ComponentChoiceProperty.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/properties/jucer_ComponentColourProperty.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/properties/jucer_ComponentTextProperty.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/properties/jucer_FilePropertyComponent.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/properties/jucer_FontPropertyComponent.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/properties/jucer_JustificationProperty.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/properties/jucer_PositionPropertyBase.h"
)

jucer_project_files("Projucer/ComponentEditor/ui"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/ui/jucer_ComponentLayoutEditor.cpp"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/ui/jucer_ComponentLayoutEditor.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/ui/jucer_ComponentLayoutPanel.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/ui/jucer_ComponentOverlayComponent.cpp"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/ui/jucer_ComponentOverlayComponent.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/ui/jucer_EditingPanelBase.cpp"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/ui/jucer_EditingPanelBase.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/ui/jucer_JucerCommandIDs.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/ui/jucer_JucerDocumentEditor.cpp"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/ui/jucer_JucerDocumentEditor.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/ui/jucer_PaintRoutineEditor.cpp"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/ui/jucer_PaintRoutineEditor.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/ui/jucer_PaintRoutinePanel.cpp"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/ui/jucer_PaintRoutinePanel.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/ui/jucer_RelativePositionedRectangle.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/ui/jucer_ResourceEditorPanel.cpp"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/ui/jucer_ResourceEditorPanel.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/ui/jucer_SnapGridPainter.h"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/ui/jucer_TestComponent.cpp"
  "${Projucer_jucer_DIR}/Source/ComponentEditor/ui/jucer_TestComponent.h"
)

jucer_project_files("Projucer/Project"
  "${Projucer_jucer_DIR}/Source/Project/jucer_ConfigTree_Base.h"
  "${Projucer_jucer_DIR}/Source/Project/jucer_ConfigTree_Exporter.h"
  "${Projucer_jucer_DIR}/Source/Project/jucer_ConfigTree_Modules.h"
  "${Projucer_jucer_DIR}/Source/Project/jucer_DependencyPathPropertyComponent.cpp"
  "${Projucer_jucer_DIR}/Source/Project/jucer_DependencyPathPropertyComponent.h"
  "${Projucer_jucer_DIR}/Source/Project/jucer_GroupInformationComponent.h"
  "${Projucer_jucer_DIR}/Source/Project/jucer_Module.cpp"
  "${Projucer_jucer_DIR}/Source/Project/jucer_Module.h"
  "${Projucer_jucer_DIR}/Source/Project/jucer_ModulesPanel.h"
  "${Projucer_jucer_DIR}/Source/Project/jucer_Project.cpp"
  "${Projucer_jucer_DIR}/Source/Project/jucer_Project.h"
  # "${Projucer_jucer_DIR}/Source/Project/jucer_ProjectContentComponent.cpp"
  "${Projucer_jucer_DIR}/Source/Project/jucer_ProjectContentComponent.h"
  "${Projucer_jucer_DIR}/Source/Project/jucer_ProjectTree_Base.h"
  "${Projucer_jucer_DIR}/Source/Project/jucer_ProjectTree_File.h"
  "${Projucer_jucer_DIR}/Source/Project/jucer_ProjectTree_Group.h"
  "${Projucer_jucer_DIR}/Source/Project/jucer_ProjectType.h"
  "${Projucer_jucer_DIR}/Source/Project/jucer_TreeItemTypes.h"
)

jucer_project_files("Projucer/Project Saving"
  "${Projucer_jucer_DIR}/Source/Project Saving/jucer_ProjectExport_AndroidAnt.h"
  "${Projucer_jucer_DIR}/Source/Project Saving/jucer_ProjectExport_AndroidBase.h"
  "${Projucer_jucer_DIR}/Source/Project Saving/jucer_ProjectExport_AndroidStudio.h"
  "${Projucer_jucer_DIR}/Source/Project Saving/jucer_ProjectExport_CodeBlocks.h"
  "${Projucer_jucer_DIR}/Source/Project Saving/jucer_ProjectExport_Make.h"
  "${Projucer_jucer_DIR}/Source/Project Saving/jucer_ProjectExport_MSVC.h"
  "${Projucer_jucer_DIR}/Source/Project Saving/jucer_ProjectExport_XCode.h"
  "${Projucer_jucer_DIR}/Source/Project Saving/jucer_ProjectExporter.cpp"
  "${Projucer_jucer_DIR}/Source/Project Saving/jucer_ProjectExporter.h"
  "${Projucer_jucer_DIR}/Source/Project Saving/jucer_ProjectSaver.cpp"
  "${Projucer_jucer_DIR}/Source/Project Saving/jucer_ProjectSaver.h"
  "${Projucer_jucer_DIR}/Source/Project Saving/jucer_ResourceFile.cpp"
  "${Projucer_jucer_DIR}/Source/Project Saving/jucer_ResourceFile.h"
)

jucer_project_files("Projucer/Utility"
  "${Projucer_jucer_DIR}/Source/Utility/jucer_CodeHelpers.cpp"
  "${Projucer_jucer_DIR}/Source/Utility/jucer_CodeHelpers.h"
  "${Projucer_jucer_DIR}/Source/Utility/jucer_ColourPropertyComponent.h"
  "${Projucer_jucer_DIR}/Source/Utility/jucer_Colours.h"
  "${Projucer_jucer_DIR}/Source/Utility/jucer_DialogLookAndFeel.h"
  "${Projucer_jucer_DIR}/Source/Utility/jucer_FileHelpers.cpp"
  "${Projucer_jucer_DIR}/Source/Utility/jucer_FileHelpers.h"
  "${Projucer_jucer_DIR}/Source/Utility/jucer_FilePathPropertyComponent.h"
  "${Projucer_jucer_DIR}/Source/Utility/jucer_FloatingToolWindow.h"
  "${Projucer_jucer_DIR}/Source/Utility/jucer_Icons.cpp"
  "${Projucer_jucer_DIR}/Source/Utility/jucer_Icons.h"
  "${Projucer_jucer_DIR}/Source/Utility/jucer_JucerTreeViewBase.cpp"
  "${Projucer_jucer_DIR}/Source/Utility/jucer_JucerTreeViewBase.h"
  "${Projucer_jucer_DIR}/Source/Utility/jucer_MiscUtilities.cpp"
  "${Projucer_jucer_DIR}/Source/Utility/jucer_MiscUtilities.h"
  "${Projucer_jucer_DIR}/Source/Utility/jucer_PresetIDs.h"
  "${Projucer_jucer_DIR}/Source/Utility/jucer_ProjucerLookAndFeel.cpp"
  "${Projucer_jucer_DIR}/Source/Utility/jucer_ProjucerLookAndFeel.h"
  "${Projucer_jucer_DIR}/Source/Utility/jucer_RelativePath.h"
  "${Projucer_jucer_DIR}/Source/Utility/jucer_SlidingPanelComponent.cpp"
  "${Projucer_jucer_DIR}/Source/Utility/jucer_SlidingPanelComponent.h"
  "${Projucer_jucer_DIR}/Source/Utility/jucer_StoredSettings.cpp"
  "${Projucer_jucer_DIR}/Source/Utility/jucer_StoredSettings.h"
  "${Projucer_jucer_DIR}/Source/Utility/jucer_SVGPathDataComponent.h"
  "${Projucer_jucer_DIR}/Source/Utility/jucer_TranslationTool.h"
  "${Projucer_jucer_DIR}/Source/Utility/jucer_UTF8Component.h"
  "${Projucer_jucer_DIR}/Source/Utility/jucer_ValueSourceHelpers.h"
)

jucer_project_files("Projucer/Wizards"
  "${Projucer_jucer_DIR}/Source/Wizards/jucer_NewFileWizard.cpp"
  "${Projucer_jucer_DIR}/Source/Wizards/jucer_NewFileWizard.h"
  "${Projucer_jucer_DIR}/Source/Wizards/jucer_NewProjectWizard.h"
  "${Projucer_jucer_DIR}/Source/Wizards/jucer_NewProjectWizardClasses.cpp"
  "${Projucer_jucer_DIR}/Source/Wizards/jucer_NewProjectWizardClasses.h"
  "${Projucer_jucer_DIR}/Source/Wizards/jucer_NewProjectWizardComponent.h"
  "${Projucer_jucer_DIR}/Source/Wizards/jucer_ProjectWizard_Animated.h"
  "${Projucer_jucer_DIR}/Source/Wizards/jucer_ProjectWizard_AudioApp.h"
  "${Projucer_jucer_DIR}/Source/Wizards/jucer_ProjectWizard_AudioPlugin.h"
  "${Projucer_jucer_DIR}/Source/Wizards/jucer_ProjectWizard_Blank.h"
  "${Projucer_jucer_DIR}/Source/Wizards/jucer_ProjectWizard_Console.h"
  "${Projucer_jucer_DIR}/Source/Wizards/jucer_ProjectWizard_DLL.h"
  "${Projucer_jucer_DIR}/Source/Wizards/jucer_ProjectWizard_GUIApp.h"
  "${Projucer_jucer_DIR}/Source/Wizards/jucer_ProjectWizard_openGL.h"
  "${Projucer_jucer_DIR}/Source/Wizards/jucer_ProjectWizard_StaticLibrary.h"
  "${Projucer_jucer_DIR}/Source/Wizards/jucer_StartPageComponent.h"
  "${Projucer_jucer_DIR}/Source/Wizards/jucer_TemplateThumbnailsComponent.h"
)

jucer_project_resources("Projucer/BinaryData/gradle"
  "${Projucer_jucer_DIR}/Source/BinaryData/gradle/gradle-wrapper.jar"
  "${Projucer_jucer_DIR}/Source/BinaryData/gradle/gradlew"
  "${Projucer_jucer_DIR}/Source/BinaryData/gradle/gradlew.bat"
  "${Projucer_jucer_DIR}/Source/BinaryData/gradle/LICENSE"
)

jucer_project_resources("Projucer/BinaryData/templates"
  "${Projucer_jucer_DIR}/Source/BinaryData/jucer_AnimatedComponentTemplate.cpp"
  "${Projucer_jucer_DIR}/Source/BinaryData/jucer_AudioComponentTemplate.cpp"
  "${Projucer_jucer_DIR}/Source/BinaryData/jucer_AudioPluginEditorTemplate.cpp"
  "${Projucer_jucer_DIR}/Source/BinaryData/jucer_AudioPluginEditorTemplate.h"
  "${Projucer_jucer_DIR}/Source/BinaryData/jucer_AudioPluginFilterTemplate.cpp"
  "${Projucer_jucer_DIR}/Source/BinaryData/jucer_AudioPluginFilterTemplate.h"
  "${Projucer_jucer_DIR}/Source/BinaryData/jucer_ComponentTemplate.cpp"
  "${Projucer_jucer_DIR}/Source/BinaryData/jucer_ComponentTemplate.h"
  "${Projucer_jucer_DIR}/Source/BinaryData/jucer_ContentCompTemplate.cpp"
  "${Projucer_jucer_DIR}/Source/BinaryData/jucer_ContentCompTemplate.h"
  "${Projucer_jucer_DIR}/Source/BinaryData/jucer_InlineComponentTemplate.h"
  "${Projucer_jucer_DIR}/Source/BinaryData/jucer_MainConsoleAppTemplate.cpp"
  "${Projucer_jucer_DIR}/Source/BinaryData/jucer_MainTemplate_NoWindow.cpp"
  "${Projucer_jucer_DIR}/Source/BinaryData/jucer_MainTemplate_SimpleWindow.cpp"
  "${Projucer_jucer_DIR}/Source/BinaryData/jucer_MainTemplate_Window.cpp"
  "${Projucer_jucer_DIR}/Source/BinaryData/jucer_NewComponentTemplate.cpp"
  "${Projucer_jucer_DIR}/Source/BinaryData/jucer_NewComponentTemplate.h"
  "${Projucer_jucer_DIR}/Source/BinaryData/jucer_NewCppFileTemplate.cpp"
  "${Projucer_jucer_DIR}/Source/BinaryData/jucer_NewCppFileTemplate.h"
  "${Projucer_jucer_DIR}/Source/BinaryData/jucer_NewInlineComponentTemplate.h"
  "${Projucer_jucer_DIR}/Source/BinaryData/jucer_OpenGLComponentTemplate.cpp"
)

jucer_project_resources("Projucer/BinaryData"
  "${Projucer_jucer_DIR}/Source/BinaryData/background_logo.svg"
  "${Projucer_jucer_DIR}/Source/BinaryData/background_tile.png"
  "${Projucer_jucer_DIR}/Source/BinaryData/colourscheme_dark.xml"
  "${Projucer_jucer_DIR}/Source/BinaryData/colourscheme_light.xml"
  "${Projucer_jucer_DIR}/Source/BinaryData/juce_icon.png"
  "${Projucer_jucer_DIR}/Source/BinaryData/projectIconAndroid.png"
  "${Projucer_jucer_DIR}/Source/BinaryData/projectIconCodeblocks.png"
  "${Projucer_jucer_DIR}/Source/BinaryData/projectIconLinuxMakefile.png"
  "${Projucer_jucer_DIR}/Source/BinaryData/projectIconVisualStudio.png"
  "${Projucer_jucer_DIR}/Source/BinaryData/projectIconXcode.png"
  "${Projucer_jucer_DIR}/Source/BinaryData/projectIconXcodeIOS.png"
  "${Projucer_jucer_DIR}/Source/BinaryData/projucer_EULA.txt"
  "${Projucer_jucer_DIR}/Source/BinaryData/projucer_login_bg.svg"
  "${Projucer_jucer_DIR}/Source/BinaryData/RecentFilesMenuTemplate.nib"
  "${Projucer_jucer_DIR}/Source/BinaryData/wizard_AnimatedApp.svg"
  "${Projucer_jucer_DIR}/Source/BinaryData/wizard_AudioApp.svg"
  "${Projucer_jucer_DIR}/Source/BinaryData/wizard_AudioPlugin.svg"
  "${Projucer_jucer_DIR}/Source/BinaryData/wizard_ConsoleApp.svg"
  "${Projucer_jucer_DIR}/Source/BinaryData/wizard_DLL.svg"
  "${Projucer_jucer_DIR}/Source/BinaryData/wizard_GUI.svg"
  "${Projucer_jucer_DIR}/Source/BinaryData/wizard_Highlight.svg"
  "${Projucer_jucer_DIR}/Source/BinaryData/wizard_Openfile.svg"
  "${Projucer_jucer_DIR}/Source/BinaryData/wizard_OpenGL.svg"
  "${Projucer_jucer_DIR}/Source/BinaryData/wizard_StaticLibrary.svg"
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
