// Copyright (c) 2017 Alain Martin, Matthieu Talbot
//
// This file is part of FRUT.
//
// FRUT is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// FRUT is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with FRUT.  If not, see <http://www.gnu.org/licenses/>.

#include "JuceHeader.h"

#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <locale>
#include <numeric>
#include <string>
#include <tuple>
#include <vector>


void printError(const std::string& error)
{
  std::cerr << "error: " << error << std::endl;
}


std::string escape(const std::string& charsToEscape, std::string value)
{
  auto pos = std::string::size_type{0};

  while ((pos = value.find_first_of(charsToEscape, pos)) != std::string::npos)
  {
    value.insert(pos, "\\");
    pos += 2;
  }

  return value;
}


std::string join(const std::string& sep, const std::vector<std::string>& elements)
{
  if (elements.empty())
  {
    return {};
  }

  return std::accumulate(std::next(elements.begin()),
    elements.end(),
    *elements.begin(),
    [&sep](const std::string& sum, const std::string& elm)
    {
      return sum + sep + elm;
    });
}


std::vector<std::string> split(const std::string& sep, const std::string& value)
{
  std::vector<std::string> tokens;
  std::string::size_type start = 0u, end = 0u;

  while ((end = value.find(sep, start)) != std::string::npos)
  {
    tokens.push_back(value.substr(start, end - start));
    start = end + sep.length();
  }
  tokens.push_back(value.substr(start));

  return tokens;
}


std::string getSetting(const juce::ValueTree& valueTree, const std::string& cmakeTag,
  const juce::Identifier& property)
{
  if (valueTree.hasProperty(property))
  {
    const auto value = valueTree.getProperty(property).toString().toStdString();

    if (!value.empty())
    {
      return cmakeTag + " \"" + escape("\\\";", value) + "\"";
    }
  }

  return "# " + cmakeTag;
}


std::string getOnOffSetting(const juce::ValueTree& valueTree, const std::string& cmakeTag,
  const juce::Identifier& property)
{
  if (valueTree.hasProperty(property))
  {
    const auto value = bool{valueTree.getProperty(property)};

    return cmakeTag + " " + (value ? "ON" : "OFF");
  }

  return "# " + cmakeTag;
}


juce::ValueTree getChildWithPropertyRecursively(const juce::ValueTree& valueTree,
  const juce::Identifier& propertyName, const juce::var& propertyValue)
{
  const auto child = valueTree.getChildWithProperty(propertyName, propertyValue);

  if (child.isValid())
  {
    return child;
  }

  for (auto i = 0; i < valueTree.getNumChildren(); ++i)
  {
    const auto grandchild =
      getChildWithPropertyRecursively(valueTree.getChild(i), propertyName, propertyValue);

    if (grandchild.isValid())
    {
      return grandchild;
    }
  }

  return {};
}


void writeUserNotes(std::ostream& out, const juce::ValueTree& valueTree)
{
  const auto userNotes = valueTree.getProperty("userNotes").toString().toStdString();
  if (!userNotes.empty())
  {
    out << "  # NOTES\n";
    for (const auto& line : split("\n", userNotes))
    {
      out << "  #   " << line << "\n";
    }
  }
}


std::string getGccOptimisation(int optimisationLevel)
{
  switch (optimisationLevel)
  {
  case 1:
    return "-O0 (no optimisation)";
  case 2:
    return "-Os (minimise code size)";
  case 3:
    return "-O3 (fastest with safe optimisations)";
  case 4:
    return "-O1 (fast)";
  case 5:
    return "-O2 (faster)";
  case 6:
    return "-Ofast (uses aggressive optimisations)";
  }

  return {};
}


std::string getMsvcOptimisation(int optimisationLevel)
{
  switch (optimisationLevel)
  {
  case 1:
    return "No optimisation";
  case 2:
    return "Minimise size";
  case 3:
    return "Maximise speed";
  }

  return {};
}


int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cerr << "usage: Jucer2Reprojucer"
                 " <jucer_project_file>"
                 " <Reprojucer.cmake_file>"
              << std::endl;
    return 1;
  }

  const auto args = std::vector<std::string>{argv, argv + argc};

  const auto& jucerFilePath = args.at(1);
  const auto jucerFile = juce::File{
    juce::File::getCurrentWorkingDirectory().getChildFile(juce::String{jucerFilePath})};

  const auto xml = std::unique_ptr<juce::XmlElement>{juce::XmlDocument::parse(jucerFile)};
  if (xml == nullptr || !xml->hasTagName("JUCERPROJECT"))
  {
    printError(jucerFilePath + " is not a valid Jucer project.");
    return 1;
  }

  const auto jucerProject = juce::ValueTree::fromXml(*xml);
  if (!jucerProject.hasType("JUCERPROJECT"))
  {
    printError(jucerFilePath + " is not a valid Jucer project.");
    return 1;
  }

  const auto jucerVersion = jucerProject.getProperty("jucerVersion").toString();
  const auto jucerVersionTokens = split(".", jucerVersion.toStdString());
  if (jucerVersionTokens.size() != 3u)
  {
    printError(jucerFilePath + " is not a valid Jucer project.");
    return 1;
  }

  using Version = std::tuple<int, int, int>;

  const auto jucerVersionAsTuple = [&jucerVersionTokens, &jucerFilePath]()
  {
    try
    {
      return Version{std::stoi(jucerVersionTokens.at(0)),
        std::stoi(jucerVersionTokens.at(1)),
        std::stoi(jucerVersionTokens.at(2))};
    }
    catch (const std::invalid_argument&)
    {
      printError(jucerFilePath + " is not a valid Jucer project.");
      std::exit(1);
    }
  }();

  std::ofstream out{"CMakeLists.txt"};

  const auto jucerFileName = jucerFile.getFileName().toStdString();

  // Preamble
  {
    out << "# This file was generated by Jucer2Reprojucer from " << jucerFileName << "\n"
        << "\n"
        << "cmake_minimum_required(VERSION 3.4)\n"
        << "\n"
        << "\n";
  }

  // include(Reprojucer)
  {
    const auto& reprojucerFilePath = args.at(2);

    out << "list(APPEND CMAKE_MODULE_PATH \""
        << "${CMAKE_CURRENT_LIST_DIR}/"
        << juce::File{juce::File::getCurrentWorkingDirectory().getChildFile(
                        juce::String{reprojucerFilePath})}
             .getParentDirectory()
             .getRelativePathFrom(juce::File::getCurrentWorkingDirectory())
             .replace("\\", "/")
        << "\")\n"
        << "include(Reprojucer)\n"
        << "\n"
        << "\n";
  }

  std::string escapedJucerFileName = jucerFileName;
  std::replace_if(escapedJucerFileName.begin(),
    escapedJucerFileName.end(),
    [](const std::string::value_type& c)
    {
      return !(std::isalpha(c, std::locale{"C"}) || std::isdigit(c, std::locale{"C"}));
    },
    '_');

  // get_filename_component()
  {
    out << "if(NOT DEFINED " << escapedJucerFileName << "_FILE)\n"
        << "  message(FATAL_ERROR \"" << escapedJucerFileName
        << "_FILE must be defined\")\n"
        << "endif()\n"
        << "\n"
        << "get_filename_component(" << escapedJucerFileName << "_FILE\n"
        << "  \"${" << escapedJucerFileName << "_FILE}\" ABSOLUTE\n"
        << "  BASE_DIR \"${CMAKE_BINARY_DIR}\"\n"
        << ")\n"
        << "\n"
        << "\n";
  }

  // jucer_project_begin()
  {
    out << "jucer_project_begin(\n"
        << "  JUCER_VERSION \"" << jucerVersion << "\"\n"
        << "  PROJECT_FILE \"${" << escapedJucerFileName << "_FILE}\"\n"
        << "  " << getSetting(jucerProject, "PROJECT_ID", "id") << "\n"
        << ")\n"
        << "\n";
  }

  const auto projectType = jucerProject.getProperty("projectType").toString();

  // jucer_project_settings()
  {
    const auto projectSetting = [&jucerProject](
      const std::string& cmakeTag, const juce::Identifier& property)
    {
      return getSetting(jucerProject, cmakeTag, property);
    };

    const auto projectTypeDescription = [&projectType]() -> std::string
    {
      if (projectType == "guiapp")
        return "GUI Application";

      if (projectType == "consoleapp")
        return "Console Application";

      if (projectType == "library")
        return "Static Library";

      if (projectType == "audioplug")
        return "Audio Plug-in";

      return {};
    }();

    const auto maxBinaryFileSize = [&jucerProject]() -> std::string
    {
      if (jucerProject.getProperty("maxBinaryFileSize").toString().isEmpty())
        return "Default";

      const auto value = int{jucerProject.getProperty("maxBinaryFileSize")};
      return juce::File::descriptionOfSizeInBytes(value).toStdString();
    }();

    out << "jucer_project_settings(\n"
        << "  " << projectSetting("PROJECT_NAME", "name") << "\n"
        << "  " << projectSetting("PROJECT_VERSION", "version") << "\n"
        << "  " << projectSetting("COMPANY_NAME", "companyName") << "\n"
        << "  " << projectSetting("COMPANY_WEBSITE", "companyWebsite") << "\n"
        << "  " << projectSetting("COMPANY_EMAIL", "companyEmail") << "\n"
        << "  PROJECT_TYPE \"" << projectTypeDescription << "\"\n"
        << "  " << projectSetting("BUNDLE_IDENTIFIER", "bundleIdentifier") << "\n"
        << "  BINARYDATACPP_SIZE_LIMIT \"" << maxBinaryFileSize << "\"\n"
        << "  " << projectSetting("BINARYDATA_NAMESPACE", "binaryDataNamespace") << "\n"
        << "  " << projectSetting("PREPROCESSOR_DEFINITIONS", "defines") << "\n";

    writeUserNotes(out, jucerProject);

    out << ")\n"
        << "\n";

    // jucer_audio_plugin_settings()
    if (projectType == "audioplug")
    {
      const auto onOffProjectSetting = [&jucerProject](
        const std::string& cmakeTag, const juce::Identifier& property)
      {
        return getOnOffSetting(jucerProject, cmakeTag, property);
      };

      out << "jucer_audio_plugin_settings(\n"
          << "  " << onOffProjectSetting("BUILD_VST", "buildVST") << "\n"
          << "  " << onOffProjectSetting("BUILD_VST3", "buildVST3") << "\n"
          << "  " << onOffProjectSetting("BUILD_AUDIOUNIT", "buildAU") << "\n"
          << "  " << onOffProjectSetting("BUILD_AUDIOUNIT_V3", "buildAUv3") << "\n"
          << "  " << onOffProjectSetting("BUILD_RTAS", "buildRTAS") << "\n"
          << "  " << onOffProjectSetting("BUILD_AAX", "buildAAX") << "\n"
          << "  " << projectSetting("PLUGIN_NAME", "pluginName") << "\n"
          << "  " << projectSetting("PLUGIN_DESCRIPTION", "pluginDesc") << "\n"
          << "  " << projectSetting("PLUGIN_MANUFACTURER", "pluginManufacturer") << "\n"
          << "  " << projectSetting("PLUGIN_MANUFACTURER_CODE", "pluginManufacturerCode")
          << "\n"
          << "  " << projectSetting("PLUGIN_CODE", "pluginCode") << "\n"
          << "  "
          << projectSetting("PLUGIN_CHANNEL_CONFIGURATIONS", "pluginChannelConfigs")
          << "\n"
          << "  " << onOffProjectSetting("PLUGIN_IS_A_SYNTH", "pluginIsSynth") << "\n"
          << "  " << onOffProjectSetting("PLUGIN_MIDI_INPUT", "pluginWantsMidiIn") << "\n"
          << "  " << onOffProjectSetting("PLUGIN_MIDI_OUTPUT", "pluginProducesMidiOut")
          << "\n"
          << "  " << onOffProjectSetting("MIDI_EFFECT_PLUGIN", "pluginIsMidiEffectPlugin")
          << "\n"
          << "  " << onOffProjectSetting("KEY_FOCUS", "pluginEditorRequiresKeys") << "\n"
          << "  " << projectSetting("PLUGIN_AU_EXPORT_PREFIX", "pluginAUExportPrefix")
          << "\n"
          << "  " << projectSetting("PLUGIN_AU_MAIN_TYPE", "pluginAUMainType") << "\n"
          << "  " << projectSetting("VST_CATEGORY", "pluginVSTCategory") << "\n"
          << "  " << projectSetting("PLUGIN_RTAS_CATEGORY", "pluginRTASCategory") << "\n"
          << "  " << projectSetting("PLUGIN_AAX_CATEGORY", "pluginAAXCategory") << "\n"
          << "  " << projectSetting("PLUGIN_AAX_IDENTIFIER", "aaxIdentifier") << "\n"
          << ")\n"
          << "\n";
    }
  }

  // jucer_project_files()
  {
    const auto writeFiles = [&out](const std::string& fullGroupName,
      const std::vector<std::tuple<bool, bool, bool, std::string>>& files)
    {
      if (!files.empty())
      {
        const auto nineSpaces = "         ";

        out << "jucer_project_files(\"" << fullGroupName << "\"\n"
            << "# Compile   Xcode     Binary\n"
            << "#           Resource  Resource\n";

        for (const auto& file : files)
        {
          const auto compile = std::get<0>(file);
          const auto xcodeResource = std::get<1>(file);
          const auto binaryResource = std::get<2>(file);
          const auto path = std::get<3>(file);

          out << "  " << (compile ? "x" : ".") << nineSpaces
              << (xcodeResource ? "x" : ".") << nineSpaces << (binaryResource ? "x" : ".")
              << nineSpaces << "\"" << path << "\"\n";
        }

        out << ")\n"
            << "\n";
      }
    };

    std::vector<std::string> groupNames;

    std::function<void(const juce::ValueTree&)> processGroup =
      [&groupNames, &processGroup, &writeFiles](const juce::ValueTree& group)
    {
      groupNames.push_back(group.getProperty("name").toString().toStdString());

      const auto fullGroupName = join("/", groupNames);

      std::vector<std::tuple<bool, bool, bool, std::string>> files;

      for (auto i = 0; i < group.getNumChildren(); ++i)
      {
        const auto fileOrGroup = group.getChild(i);

        if (fileOrGroup.hasType("FILE"))
        {
          const auto& file = fileOrGroup;

          files.emplace_back(int{file.getProperty("compile")} == 1,
            int{file.getProperty("xcodeResource")} == 1,
            int{file.getProperty("resource")} == 1,
            file.getProperty("file").toString().toStdString());
        }
        else
        {
          writeFiles(fullGroupName, files);
          files.clear();

          processGroup(fileOrGroup);
        }
      }

      writeFiles(fullGroupName, files);

      groupNames.pop_back();
    };

    processGroup(jucerProject.getChildWithName("MAINGROUP"));
  }

  // jucer_project_module()
  {
    std::vector<std::string> moduleNames;
    const auto modules = jucerProject.getChildWithName("MODULES");
    for (auto i = 0; i < modules.getNumChildren(); ++i)
    {
      const auto module = modules.getChild(i);
      moduleNames.push_back(module.getProperty("id").toString().toStdString());
    }

    const auto modulePaths = jucerProject.getChildWithName("EXPORTFORMATS")
                               .getChild(0)
                               .getChildWithName("MODULEPATHS");

    for (const auto& moduleName : moduleNames)
    {
      const auto relativeModulePath =
        modulePaths.getChildWithProperty("id", juce::String{moduleName})
          .getProperty("path")
          .toString();

      out << "jucer_project_module(\n"
          << "  " << moduleName << "\n"
          << "  PATH \"" << relativeModulePath << "\"\n";

      const auto moduleHeader = jucerFile.getParentDirectory()
                                  .getChildFile(relativeModulePath)
                                  .getChildFile(juce::String{moduleName})
                                  .getChildFile(juce::String{moduleName + ".h"});
      juce::StringArray moduleHeaderLines;
      moduleHeader.readLines(moduleHeaderLines);

      const auto modulesOptions = jucerProject.getChildWithName("JUCEOPTIONS");

      for (const auto& line : moduleHeaderLines)
      {
        if (line.startsWith("/** Config: "))
        {
          const auto moduleOption = line.substring(12);
          const auto optionValue = modulesOptions.getProperty(moduleOption);

          if (optionValue == "enabled")
          {
            out << "  " << moduleOption << " ON\n";
          }
          else if (optionValue == "disabled")
          {
            out << "  " << moduleOption << " OFF\n";
          }
          else
          {
            out << "  # " << moduleOption << "\n";
          }
        }
      }

      out << ")\n"
          << "\n";
    }
  }

  // jucer_appconfig_header()
  {
    const auto appConfigFile =
      jucerFile.getSiblingFile("JuceLibraryCode").getChildFile("AppConfig.h");

    juce::StringArray appConfigLines;
    appConfigLines.addLines(appConfigFile.loadFileAsString());

    std::vector<std::string> userCodeSectionLines;

    for (auto i = 0; i < appConfigLines.size(); ++i)
    {
      if (appConfigLines[i].contains("[BEGIN_USER_CODE_SECTION]"))
      {
        for (auto j = i + 1; j < appConfigLines.size()
                             && !appConfigLines[j].contains("[END_USER_CODE_SECTION]");
             ++j)
        {
          userCodeSectionLines.push_back(appConfigLines[j].toStdString());
        }

        break;
      }
    }

    const auto kDefaultProjucerUserCodeSectionComment = std::vector<std::string>{"",
      "// (You can add your own code in this section, and the Projucer will not "
      "overwrite it)",
      ""};

    if (userCodeSectionLines != kDefaultProjucerUserCodeSectionComment)
    {
      out << "jucer_appconfig_header(\n"
          << "  USER_CODE_SECTION\n"
          << "\"" << escape("\\\"", join("\n", userCodeSectionLines)) << "\"\n"
          << ")\n"
          << "\n";
    }
  }

  // jucer_export_target() and jucer_export_target_configuration()
  {
    const auto supportedExporters = {std::make_tuple("XCODE_MAC", "Xcode (MacOSX)"),
      std::make_tuple("VS2015", "Visual Studio 2015"),
      std::make_tuple("VS2013", "Visual Studio 2013"),
      std::make_tuple("LINUX_MAKE", "Linux Makefile")};

    for (const auto& element : supportedExporters)
    {
      const auto exporter = jucerProject.getChildWithName("EXPORTFORMATS")
                              .getChildWithName(std::get<0>(element));
      if (exporter.isValid())
      {
        const auto exporterType = exporter.getType().toString();
        const auto configurations = exporter.getChildWithName("CONFIGURATIONS");

        out << "jucer_export_target(\n"
            << "  \"" << std::get<1>(element) << "\"\n";

        if (exporterType == "XCODE_MAC"
            && (!exporter.getProperty("prebuildCommand").toString().isEmpty()
                 || !exporter.getProperty("postbuildCommand").toString().isEmpty()))
        {
          out << "  TARGET_PROJECT_FOLDER \""
              << exporter.getProperty("targetFolder").toString()
              << "\"  # only used by PREBUILD_SHELL_SCRIPT and POSTBUILD_SHELL_SCRIPT\n";
        }

        const auto isVSExporter = exporterType == "VS2015" || exporterType == "VS2013";

        if (isVSExporter)
        {
          const auto needsTargetFolder = [&configurations]()
          {
            for (auto i = 0; i < configurations.getNumChildren(); ++i)
            {
              const auto configuration = configurations.getChild(i);

              if (configuration.hasProperty("prebuildCommand")
                  || configuration.hasProperty("postbuildCommand"))
              {
                return true;
              }
            }
            return false;
          }();

          if (needsTargetFolder)
          {
            out << "  TARGET_PROJECT_FOLDER \""
                << exporter.getProperty("targetFolder").toString()
                << "\" # only used by PREBUILD_COMMAND and POSTBUILD_COMMAND\n";
          }
        }

        const auto hasVst2Interface = jucerVersionAsTuple > Version{4, 2, 3};
        const auto isVstAudioPlugin =
          projectType == "audioplug" && bool{jucerProject.getProperty("buildVST")};
        const auto isVstPluginHost =
          jucerProject.getChildWithName("MODULES")
            .getChildWithProperty("id", "juce_audio_processors")
            .isValid()
          && jucerProject.getChildWithName("JUCEOPTIONS")
                 .getProperty("JUCE_PLUGINHOST_VST")
               == "enabled";

        if (!hasVst2Interface && (isVstAudioPlugin || isVstPluginHost))
        {
          out << "  " << getSetting(exporter, "VST_SDK_FOLDER", "vstFolder") << "\n";
        }

        const auto supportsVst3 = exporterType == "XCODE_MAC" || isVSExporter;
        const auto isVst3AudioPlugin =
          projectType == "audioplug" && bool{jucerProject.getProperty("buildVST3")};
        const auto isVst3PluginHost =
          jucerProject.getChildWithName("MODULES")
            .getChildWithProperty("id", "juce_audio_processors")
            .isValid()
          && jucerProject.getChildWithName("JUCEOPTIONS")
                 .getProperty("JUCE_PLUGINHOST_VST3")
               == "enabled";

        if (supportsVst3 && (isVst3AudioPlugin || isVst3PluginHost))
        {
          out << "  " << getSetting(exporter, "VST3_SDK_FOLDER", "vst3Folder") << "\n";
        }

        out << "  " << getSetting(exporter, "EXTRA_PREPROCESSOR_DEFINITIONS", "extraDefs")
            << "\n"
            << "  " << getSetting(exporter, "EXTRA_COMPILER_FLAGS", "extraCompilerFlags")
            << "\n"
            << "  " << getSetting(exporter, "EXTRA_LINKER_FLAGS", "extraLinkerFlags")
            << "\n"
            << "  "
            << getSetting(exporter, "EXTERNAL_LIBRARIES_TO_LINK", "externalLibraries")
            << "\n";

        const auto mainGroup = jucerProject.getChildWithName("MAINGROUP");

        const auto getIconFilePath = [&mainGroup, &exporter](
          const juce::Identifier& propertyName) -> std::string
        {
          const auto fileId = exporter.getProperty(propertyName).toString();

          if (!fileId.isEmpty())
          {
            const auto file = getChildWithPropertyRecursively(mainGroup, "id", fileId);

            if (file.isValid())
            {
              return file.getProperty("file").toString().toStdString();
            }
          }

          return {};
        };

        const auto smallIconPath = getIconFilePath("smallIcon");
        const auto bigIconPath = getIconFilePath("bigIcon");

        out << "  ICON_SMALL \"" << (smallIconPath.empty() ? "<None>" : smallIconPath)
            << "\"\n"
            << "  ICON_LARGE \"" << (bigIconPath.empty() ? "<None>" : bigIconPath)
            << "\"\n";

        if (exporterType == "XCODE_MAC")
        {
          out << "  " << getSetting(exporter,
                           "CUSTOM_XCODE_RESOURCE_FOLDERS",
                           "customXcodeResourceFolders")
              << "\n";

          if (projectType == "guiapp")
          {
            out << "  "
                << getSetting(exporter, "DOCUMENT_FILE_EXTENSIONS", "documentExtensions")
                << "\n";
          }

          out << "  " << getSetting(exporter, "CUSTOM_PLIST", "customPList") << "\n"
              << "  " << getSetting(exporter, "EXTRA_FRAMEWORKS", "extraFrameworks")
              << "\n"
              << "  " << getSetting(exporter, "PREBUILD_SHELL_SCRIPT", "prebuildCommand")
              << "\n"
              << "  "
              << getSetting(exporter, "POSTBUILD_SHELL_SCRIPT", "postbuildCommand")
              << "\n";

          if (exporter.hasProperty("iosDevelopmentTeamID"))
          {
            out << "  "
                << getSetting(exporter, "DEVELOPMENT_TEAM_ID", "iosDevelopmentTeamID")
                << "\n";
          }
        }

        if (isVSExporter)
        {
          const auto toolset = exporter.getProperty("toolset").toString().toStdString();
          if (toolset.empty())
          {
            out << "  # PLATFORM_TOOLSET \"(default)\"\n";
          }
          else
          {
            out << "  # PLATFORM_TOOLSET \"" << toolset << "\"\n";
          }

          if (exporter.hasProperty("IPPLibrary"))
          {
            const auto useIppLibrary = [&exporter]() -> std::string
            {
              const auto value = exporter.getProperty("IPPLibrary").toString();

              if (value == "")
                return "No";

              if (value == "true")
                return "Yes (Default Mode)";

              if (value == "Parallel_Static")
                return "Multi-Threaded Static Library";

              if (value == "Sequential")
                return "Single-Threaded Static Library";

              if (value == "Parallel_Dynamic")
                return "Multi-Threaded DLL";

              if (value == "Sequential_Dynamic")
                return "Single-Threaded DLL";

              return {};
            }();

            if (useIppLibrary.empty())
            {
              out << "  # USE_IPP_LIBRARY\n";
            }
            else
            {
              out << "  USE_IPP_LIBRARY \"" << useIppLibrary << "\"\n";
            }
          }
        }

        if (exporterType == "LINUX_MAKE")
        {
          const auto cppLanguageStandard = [&exporter]() -> std::string
          {
            const auto value = exporter.getProperty("cppLanguageStandard").toString();

            if (value == "-std=c++03")
              return "C++03";

            if (value == "-std=c++11")
              return "C++11";

            if (value == "-std=c++14")
              return "C++14";

            return {};
          }();

          if (cppLanguageStandard.empty())
          {
            out << "  # CXX_STANDARD_TO_USE\n";
          }
          else
          {
            out << "  CXX_STANDARD_TO_USE \"" << cppLanguageStandard << "\"\n";
          }

          out << "  "
              << getSetting(exporter, "PKGCONFIG_LIBRARIES", "linuxExtraPkgConfig")
              << "\n";
        }

        writeUserNotes(out, exporter);

        out << ")\n"
            << "\n";

        for (auto i = 0; i < configurations.getNumChildren(); ++i)
        {
          const auto configuration = configurations.getChild(i);

          out << "jucer_export_target_configuration(\n"
              << "  \"" << std::get<1>(element) << "\"\n"
              << "  NAME \"" << configuration.getProperty("name").toString() << "\"\n"
              << "  DEBUG_MODE "
              << (bool{configuration.getProperty("isDebug")} ? "ON" : "OFF") << "\n";

          out << "  " << getSetting(configuration, "BINARY_NAME", "targetName") << "\n"
              << "  " << getSetting(configuration, "BINARY_LOCATION", "binaryPath")
              << "\n";

          const auto isAbsolutePath = [](const juce::String& path)
          {
            return path.startsWithChar('/') || path.startsWithChar('~')
                   || path.startsWithChar('$')
                   || (juce::CharacterFunctions::isLetter(path[0]) && path[1] == ':');
          };

          const auto jucerFileDir = jucerFile.getParentDirectory();
          const auto targetProjectDir =
            jucerFileDir.getChildFile(exporter.getProperty("targetFolder").toString());

          const auto headerPath =
            configuration.getProperty("headerPath").toString().toStdString();
          if (headerPath.empty())
          {
            out << "  # HEADER_SEARCH_PATHS\n";
          }
          else
          {
            std::vector<std::string> absOrRelToJucerFileDirPaths;

            for (const auto& path : split("\n", headerPath))
            {
              if (path.empty())
              {
                continue;
              }

              if (isAbsolutePath(path))
              {
                absOrRelToJucerFileDirPaths.push_back(path);
              }
              else
              {
                absOrRelToJucerFileDirPaths.push_back(
                  targetProjectDir.getChildFile(juce::String{path})
                    .getRelativePathFrom(jucerFileDir)
                    .toStdString());
              }
            }

            out << "  HEADER_SEARCH_PATHS \""
                << escape("\\", join("\n", absOrRelToJucerFileDirPaths)) << "\"\n";
          }

          const auto libraryPath =
            configuration.getProperty("libraryPath").toString().toStdString();
          if (libraryPath.empty())
          {
            out << "  # EXTRA_LIBRARY_SEARCH_PATHS\n";
          }
          else
          {
            std::vector<std::string> absOrRelToJucerFileDirPaths;

            for (const auto& path : split("\n", libraryPath))
            {
              if (path.empty())
              {
                continue;
              }

              if (isAbsolutePath(path))
              {
                absOrRelToJucerFileDirPaths.push_back(path);
              }
              else
              {
                absOrRelToJucerFileDirPaths.push_back(
                  targetProjectDir.getChildFile(juce::String{path})
                    .getRelativePathFrom(jucerFileDir)
                    .toStdString());
              }
            }

            out << "  EXTRA_LIBRARY_SEARCH_PATHS \""
                << escape("\\", join("\n", absOrRelToJucerFileDirPaths)) << "\"\n";
          }

          out << "  " << getSetting(configuration, "PREPROCESSOR_DEFINITIONS", "defines")
              << "\n";

          const auto optimisation = [&configuration, &isVSExporter]() -> std::string
          {
            const auto value = configuration.getProperty("optimisation");

            if (value.isVoid())
              return {};

            if (isVSExporter)
              return getMsvcOptimisation(value);

            return getGccOptimisation(value);
          }();

          if (optimisation.empty())
          {
            out << "  # OPTIMISATION\n";
          }
          else
          {
            out << "  OPTIMISATION \"" << optimisation << "\"\n";
          }

          if (exporterType == "XCODE_MAC")
          {
            if (isVstAudioPlugin)
            {
              out << "  "
                  << getSetting(
                       configuration, "VST_BINARY_LOCATION", "xcodeVstBinaryLocation")
                  << "\n";
            }

            if (isVst3AudioPlugin)
            {
              out << "  "
                  << getSetting(
                       configuration, "VST3_BINARY_LOCATION", "xcodeVst3BinaryLocation")
                  << "\n";
            }

            if (jucerProject.getProperty("buildAU"))
            {
              out << "  " << getSetting(configuration,
                               "AU_BINARY_LOCATION",
                               "xcodeAudioUnitBinaryLocation")
                  << "\n";
            }

            const auto sdks = {"10.5 SDK",
              "10.6 SDK",
              "10.7 SDK",
              "10.8 SDK",
              "10.9 SDK",
              "10.10 SDK",
              "10.11 SDK",
              "10.12 SDK"};

            const auto osxSDK =
              configuration.getProperty("osxSDK").toString().toStdString();
            if (osxSDK == "default")
            {
              out << "  OSX_BASE_SDK_VERSION \"Use Default\"\n";
            }
            else if (std::find(sdks.begin(), sdks.end(), osxSDK) != sdks.end())
            {
              out << "  OSX_BASE_SDK_VERSION \"" << osxSDK << "\"\n";
            }
            else
            {
              out << "  # OSX_BASE_SDK_VERSION\n";
            }

            const auto osxCompatibility =
              configuration.getProperty("osxCompatibility").toString().toStdString();
            if (osxCompatibility == "default")
            {
              out << "  OSX_DEPLOYMENT_TARGET \"Use Default\"\n";
            }
            else if (std::find(sdks.begin(), sdks.end(), osxCompatibility) != sdks.end())
            {
              out << "  OSX_DEPLOYMENT_TARGET \""
                  << osxCompatibility.substr(0, osxCompatibility.length() - 4) << "\"\n";
            }
            else
            {
              out << "  # OSX_DEPLOYMENT_TARGET\n";
            }

            const auto osxArchitecture = [&configuration]() -> std::string
            {
              const auto value = configuration.getProperty("osxArchitecture").toString();

              if (value == "default")
                return "Use Default";

              if (value == "Native")
                return "Native architecture of build machine";

              if (value == "32BitUniversal")
                return "Universal Binary (32-bit)";

              if (value == "64BitUniversal")
                return "Universal Binary (32/64-bit)";

              if (value == "64BitIntel")
                return "64-bit Intel";

              return {};
            }();

            if (osxArchitecture.empty())
            {
              out << "  # OSX_ARCHITECTURE\n";
            }
            else
            {
              out << "  OSX_ARCHITECTURE \"" << osxArchitecture << "\"\n";
            }

            out << "  "
                << getSetting(configuration, "CUSTOM_XCODE_FLAGS", "customXcodeFlags")
                << "\n";

            const auto cppLanguageStandard = [&configuration]() -> std::string
            {
              const auto value =
                configuration.getProperty("cppLanguageStandard").toString();

              if (value == "")
                return "Use Default";

              if (value == "c++98")
                return "C++98";

              if (value == "gnu++98")
                return "GNU++98";

              if (value == "c++11")
                return "C++11";

              if (value == "gnu++11")
                return "GNU++11";

              if (value == "c++14")
                return "C++14";

              if (value == "gnu++14")
                return "GNU++14";

              return {};
            }();

            if (cppLanguageStandard.empty())
            {
              out << "  # CXX_LANGUAGE_STANDARD\n";
            }
            else
            {
              out << "  CXX_LANGUAGE_STANDARD \"" << cppLanguageStandard << "\"\n";
            }

            const auto cppLibType = [&configuration]() -> std::string
            {
              const auto value = configuration.getProperty("cppLibType").toString();

              if (value == "")
                return "Use Default";

              if (value == "libc++")
                return "LLVM libc++";

              if (value == "libstdc++")
                return "GNU libstdc++";

              return {};
            }();

            if (cppLibType.empty())
            {
              out << "  # CXX_LIBRARY\n";
            }
            else
            {
              out << "  CXX_LIBRARY \"" << cppLibType << "\"\n";
            }

            out << "  "
                << getSetting(
                     configuration, "CODE_SIGNING_IDENTITY", "codeSigningIdentity")
                << "\n"
                << "  "
                << getOnOffSetting(configuration, "RELAX_IEEE_COMPLIANCE", "fastMath")
                << "\n"
                << "  "
                << getOnOffSetting(
                     configuration, "LINK_TIME_OPTIMISATION", "linkTimeOptimisation")
                << "\n"
                << "  " << getOnOffSetting(
                             configuration, "STRIP_LOCAL_SYMBOLS", "stripLocalSymbols")
                << "\n";
          }

          if (isVSExporter)
          {
            const auto warningLevel = [&configuration]() -> std::string
            {
              switch (int{configuration.getProperty("winWarningLevel")})
              {
              case 2:
                return "Low";
              case 3:
                return "Medium";
              case 4:
                return "High";
              }

              return "High";
            }();

            out << "  WARNING_LEVEL \"" << warningLevel << "\"\n"
                << "  "
                << getOnOffSetting(
                     configuration, "TREAT_WARNINGS_AS_ERRORS", "warningsAreErrors")
                << "\n";

            const auto runtimeLibrary = [&configuration]() -> std::string
            {
              const auto value = configuration.getProperty("useRuntimeLibDLL").toString();

              if (value == "")
                return "(Default)";

              if (value == "0")
                return "Use static runtime";

              if (value == "1")
                return "Use DLL runtime";

              return {};
            }();

            if (runtimeLibrary.empty())
            {
              out << "  # RUNTIME_LIBRARY\n";
            }
            else
            {
              out << "  RUNTIME_LIBRARY \"" << runtimeLibrary << "\"\n";
            }

            if (configuration.getProperty("wholeProgramOptimisation").toString() == "")
            {
              out << "  WHOLE_PROGRAM_OPTIMISATION \"Enable when possible\"\n";
            }
            else if (int{configuration.getProperty("wholeProgramOptimisation")} > 0)
            {
              out << "  WHOLE_PROGRAM_OPTIMISATION \"Always disable\"\n";
            }
            else
            {
              out << "  # WHOLE_PROGRAM_OPTIMISATION\n";
            }

            out << "  "
                << getOnOffSetting(
                     configuration, "INCREMENTAL_LINKING", "enableIncrementalLinking")
                << "\n"
                << "  "
                << getSetting(configuration, "PREBUILD_COMMAND", "prebuildCommand")
                << "\n"
                << "  "
                << getSetting(configuration, "POSTBUILD_COMMAND", "postbuildCommand")
                << "\n"
                << "  "
                << getOnOffSetting(configuration, "GENERATE_MANIFEST", "generateManifest")
                << "\n";

            const auto characterSet =
              configuration.getProperty("characterSet").toString();

            if (characterSet.isEmpty())
            {
              out << "  CHARACTER_SET \"Default\"\n";
            }
            else
            {
              out << "  CHARACTER_SET \"" << characterSet << "\"\n";
            }

            const auto winArchitecture =
              configuration.getProperty("winArchitecture").toString();

            if (winArchitecture.isEmpty())
            {
              out << "  # ARCHITECTURE\n";
            }
            else
            {
              out << "  # ARCHITECTURE \"" << winArchitecture << "\"\n";
            }

            out << "  "
                << getOnOffSetting(configuration, "RELAX_IEEE_COMPLIANCE", "fastMath")
                << "\n";
          }

          if (exporterType == "LINUX_MAKE")
          {
            if (!configuration.hasProperty("linuxArchitecture"))
            {
              out << "  ARCHITECTURE \"(Default)\"\n";
            }
            else
            {
              const auto architecture = [&configuration]() -> std::string
              {
                const auto value =
                  configuration.getProperty("linuxArchitecture").toString();

                if (value == "")
                  return "<None>";

                if (value == "-m32")
                  return "32-bit (-m32)";

                if (value == "-m64")
                  return "64-bit (-m64)";

                if (value == "-march=armv6")
                  return "ARM v6";

                if (value == "-march=armv7")
                  return "ARM v7";

                return {};
              }();

              if (architecture.empty())
              {
                out << "  # ARCHITECTURE\n";
              }
              else
              {
                out << "  ARCHITECTURE \"" << architecture << "\"\n";
              }
            }
          }

          writeUserNotes(out, configuration);

          out << ")\n"
              << "\n";
        }
      }
    }
  }

  out << "jucer_project_end()" << std::endl;

  std::cout << juce::File::getCurrentWorkingDirectory()
                 .getChildFile("CMakeLists.txt")
                 .getFullPathName()
            << " has been successfully generated." << std::endl;

  return 0;
}
