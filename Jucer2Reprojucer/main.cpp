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


#if !defined(USE_CRLF_LINE_ENDINGS)
#error USE_CRLF_LINE_ENDINGS must be defined
#endif
#if USE_CRLF_LINE_ENDINGS
static const auto kNewLine = "\r\n";
#else
static const auto kNewLine = '\n';
#endif


template <class Head>
void writeToStream(std::ostream& stream, Head&& head)
{
  stream << std::forward<Head>(head);
}


template <class Head, class... Tail>
void writeToStream(std::ostream& stream, Head&& head, Tail&&... tail)
{
  stream << std::forward<Head>(head);
  writeToStream(stream, std::forward<Tail>(tail)...);
}


struct LineWriter
{
  explicit LineWriter(std::ostream& stream)
    : mStream(stream){};

  LineWriter(const LineWriter&) = delete;
  LineWriter& operator=(const LineWriter&) = delete;

  template <typename... Args>
  void operator()(Args&&... args)
  {
    writeToStream(mStream, std::forward<Args>(args)..., kNewLine);
  }

private:
  std::ostream& mStream;
};


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

  return std::accumulate(
    std::next(elements.begin()), elements.end(), *elements.begin(),
    [&sep](const std::string& sum, const std::string& elm) { return sum + sep + elm; });
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


std::string getSetting(const juce::ValueTree& valueTree,
                       const std::string& cmakeTag,
                       const juce::Identifier& property)
{
  if (valueTree.hasProperty(property))
  {
    const auto value = valueTree.getProperty(property).toString();

    if (value.isNotEmpty())
    {
      return cmakeTag + " \"" + escape("\\\";", value.toStdString()) + "\"";
    }
  }

  return "# " + cmakeTag;
}


std::string getOnOffSetting(const juce::ValueTree& valueTree,
                            const std::string& cmakeTag,
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
                                                const juce::Identifier& propertyName,
                                                const juce::var& propertyValue)
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


void writeUserNotes(LineWriter& wLn, const juce::ValueTree& valueTree)
{
  const auto userNotes = valueTree.getProperty("userNotes").toString();
  if (userNotes.isNotEmpty())
  {
    wLn("  # NOTES");
    for (const auto& line : split("\n", userNotes.toStdString()))
    {
      wLn("  #   ", line);
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

  const auto jucerVersionAsTuple = [&jucerVersionTokens, &jucerFilePath]() {
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

  std::ofstream out{"CMakeLists.txt", std::ios_base::out | std::ios_base::binary};
  LineWriter wLn{out};

  const auto convertSetting = [&wLn](const juce::ValueTree& valueTree,
                                     const juce::Identifier& property,
                                     const std::string& cmakeKeyword) {

    const auto value = valueTree.getProperty(property).toString().toStdString();

    if (value.empty())
    {
      wLn("  # ", cmakeKeyword);
    }
    else
    {
      wLn("  ", cmakeKeyword, " \"", escape("\\\";", value), "\"");
    }
  };

  const auto convertOnOffSetting = [&wLn](const juce::ValueTree& valueTree,
                                          const juce::Identifier& property,
                                          const std::string& cmakeKeyword) {

    const auto value = valueTree.getProperty(property);

    if (value.isVoid())
    {
      wLn("  # ", cmakeKeyword);
    }
    else
    {
      wLn("  ", cmakeKeyword, " ", (bool{value} ? "ON" : "OFF"));
    }
  };

  const auto jucerFileName = jucerFile.getFileName();

  // Preamble
  {
    wLn("# This file was generated by Jucer2Reprojucer from \"", jucerFileName, "\"");
    wLn();
    wLn("cmake_minimum_required(VERSION 3.4)");
    wLn();
    wLn();
  }

  // include(Reprojucer)
  {
    const auto& reprojucerFilePath = args.at(2);

    wLn("list(APPEND CMAKE_MODULE_PATH \"${CMAKE_CURRENT_LIST_DIR}/",
        juce::File{juce::File::getCurrentWorkingDirectory().getChildFile(
                     juce::String{reprojucerFilePath})}
          .getParentDirectory()
          .getRelativePathFrom(juce::File::getCurrentWorkingDirectory())
          .replace("\\", "/"),
        "\")");
    wLn("include(Reprojucer)");
    wLn();
    wLn();
  }

  std::string escapedJucerFileName = jucerFileName.toStdString();
  std::replace_if(escapedJucerFileName.begin(), escapedJucerFileName.end(),
                  [](const std::string::value_type& c) {
                    return !(std::isalpha(c, std::locale{"C"})
                             || std::isdigit(c, std::locale{"C"}));
                  },
                  '_');

  // get_filename_component()
  {
    wLn("if(NOT DEFINED ", escapedJucerFileName, "_FILE)");
    wLn("  message(FATAL_ERROR \"", escapedJucerFileName, "_FILE must be defined\")");
    wLn("endif()");
    wLn();
    wLn("get_filename_component(", escapedJucerFileName, "_FILE");
    wLn("  \"${", escapedJucerFileName, "_FILE}\" ABSOLUTE");
    wLn("  BASE_DIR \"${CMAKE_BINARY_DIR}\"");
    wLn(")");
    wLn();
    wLn();
  }

  // jucer_project_begin()
  {
    wLn("jucer_project_begin(");
    wLn("  JUCER_VERSION \"", jucerVersion, "\"");
    wLn("  PROJECT_FILE \"${", escapedJucerFileName, "_FILE}\"");
    convertSetting(jucerProject, "id", "PROJECT_ID");
    wLn(")");
    wLn();
  }

  const auto projectType = jucerProject.getProperty("projectType").toString();

  // jucer_project_settings()
  {
    wLn("jucer_project_settings(");
    convertSetting(jucerProject, "name", "PROJECT_NAME");
    convertSetting(jucerProject, "version", "PROJECT_VERSION");
    convertSetting(jucerProject, "companyName", "COMPANY_NAME");

    if (jucerProject.hasProperty("companyCopyright"))
    {
      convertSetting(jucerProject, "companyCopyright", "COMPANY_COPYRIGHT");
    }

    convertSetting(jucerProject, "companyWebsite", "COMPANY_WEBSITE");
    convertSetting(jucerProject, "companyEmail", "COMPANY_EMAIL");

    const auto licenseRequiredTagline =
      "Required for closed source applications without an Indie or Pro JUCE license";

    if (jucerProject.hasProperty("reportAppUsage"))
    {
      wLn("  ", getOnOffSetting(jucerProject, "REPORT_JUCE_APP_USAGE", "reportAppUsage"),
          " # ", licenseRequiredTagline);
    }

    if (jucerProject.hasProperty("displaySplashScreen"))
    {
      wLn("  ",
          getOnOffSetting(jucerProject, "DISPLAY_THE_JUCE_SPLASH_SCREEN",
                          "displaySplashScreen"),
          " # ", licenseRequiredTagline);
    }

    if (jucerProject.hasProperty("splashScreenColour"))
    {
      convertSetting(jucerProject, "splashScreenColour", "SPLASH_SCREEN_COLOUR");
    }

    const auto projectTypeDescription = [&projectType]() -> std::string {
      if (projectType == "guiapp")
        return "GUI Application";

      if (projectType == "consoleapp")
        return "Console Application";

      if (projectType == "library")
        return "Static Library";

      if (projectType == "dll")
        return "Dynamic Library";

      if (projectType == "audioplug")
        return "Audio Plug-in";

      return {};
    }();
    wLn("  PROJECT_TYPE \"", projectTypeDescription, "\"");

    convertSetting(jucerProject, "bundleIdentifier", "BUNDLE_IDENTIFIER");

    const auto maxBinaryFileSize = [&jucerProject]() -> std::string {
      if (jucerProject.getProperty("maxBinaryFileSize").toString().isEmpty())
        return "Default";

      const auto value = int{jucerProject.getProperty("maxBinaryFileSize")};
      return juce::File::descriptionOfSizeInBytes(value).toStdString();
    }();
    wLn("  BINARYDATACPP_SIZE_LIMIT \"", maxBinaryFileSize, "\"");

    convertSetting(jucerProject, "binaryDataNamespace", "BINARYDATA_NAMESPACE");
    convertSetting(jucerProject, "defines", "PREPROCESSOR_DEFINITIONS");

    if (jucerProject.hasProperty("headerPath"))
    {
      convertSetting(jucerProject, "headerPath", "HEADER_SEARCH_PATHS");
    }

    writeUserNotes(wLn, jucerProject);

    wLn(")");
    wLn();

    // jucer_audio_plugin_settings()
    if (projectType == "audioplug")
    {
      wLn("jucer_audio_plugin_settings(");
      convertOnOffSetting(jucerProject, "buildVST", "BUILD_VST");
      convertOnOffSetting(jucerProject, "buildVST3", "BUILD_VST3");
      convertOnOffSetting(jucerProject, "buildAU", "BUILD_AUDIOUNIT");
      convertOnOffSetting(jucerProject, "buildAUv3", "BUILD_AUDIOUNIT_V3");
      convertOnOffSetting(jucerProject, "buildRTAS", "BUILD_RTAS");
      convertOnOffSetting(jucerProject, "buildAAX", "BUILD_AAX");
      if (jucerVersionAsTuple >= Version{5, 0, 0})
      {
        convertOnOffSetting(jucerProject, "buildStandalone", "BUILD_STANDALONE_PLUGIN");
      }
      convertSetting(jucerProject, "pluginName", "PLUGIN_NAME");
      convertSetting(jucerProject, "pluginDesc", "PLUGIN_DESCRIPTION");
      convertSetting(jucerProject, "pluginManufacturer", "PLUGIN_MANUFACTURER");
      convertSetting(jucerProject, "pluginManufacturerCode", "PLUGIN_MANUFACTURER_CODE");
      convertSetting(jucerProject, "pluginCode", "PLUGIN_CODE");
      convertSetting(jucerProject, "pluginChannelConfigs",
                     "PLUGIN_CHANNEL_CONFIGURATIONS");
      convertOnOffSetting(jucerProject, "pluginIsSynth", "PLUGIN_IS_A_SYNTH");
      convertOnOffSetting(jucerProject, "pluginWantsMidiIn", "PLUGIN_MIDI_INPUT");
      convertOnOffSetting(jucerProject, "pluginProducesMidiOut", "PLUGIN_MIDI_OUTPUT");
      convertOnOffSetting(jucerProject, "pluginIsMidiEffectPlugin", "MIDI_EFFECT_PLUGIN");
      convertOnOffSetting(jucerProject, "pluginEditorRequiresKeys", "KEY_FOCUS");
      convertSetting(jucerProject, "pluginAUExportPrefix", "PLUGIN_AU_EXPORT_PREFIX");
      convertSetting(jucerProject, "pluginAUMainType", "PLUGIN_AU_MAIN_TYPE");
      convertSetting(jucerProject, "pluginVSTCategory", "VST_CATEGORY");
      convertSetting(jucerProject, "pluginRTASCategory", "PLUGIN_RTAS_CATEGORY");
      convertSetting(jucerProject, "pluginAAXCategory", "PLUGIN_AAX_CATEGORY");
      convertSetting(jucerProject, "aaxIdentifier", "PLUGIN_AAX_IDENTIFIER");
      wLn(")");
      wLn();
    }
  }

  // jucer_project_files()
  {
    const auto writeFiles =
      [&wLn](const std::string& fullGroupName,
             const std::vector<std::tuple<bool, bool, bool, std::string>>& files) {
        if (!files.empty())
        {
          const auto nineSpaces = "         ";

          wLn("jucer_project_files(\"", fullGroupName, "\"");
          wLn("# Compile   Xcode     Binary");
          wLn("#           Resource  Resource");

          for (const auto& file : files)
          {
            const auto compile = std::get<0>(file);
            const auto xcodeResource = std::get<1>(file);
            const auto binaryResource = std::get<2>(file);
            const auto path = std::get<3>(file);

            wLn("  ", (compile ? "x" : "."), nineSpaces, (xcodeResource ? "x" : "."),
                nineSpaces, (binaryResource ? "x" : "."), nineSpaces, "\"", path, "\"");
          }

          wLn(")");
          wLn();
        }
      };

    std::vector<std::string> groupNames;

    std::function<void(const juce::ValueTree&)> processGroup =
      [&groupNames, &processGroup, &writeFiles](const juce::ValueTree& group) {
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

      wLn("jucer_project_module(");
      wLn("  ", moduleName);
      wLn("  PATH \"", relativeModulePath, "\"");

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
          const auto optionValue = modulesOptions.getProperty(moduleOption).toString();

          if (optionValue == "enabled")
          {
            wLn("  ", moduleOption, " ON");
          }
          else if (optionValue == "disabled")
          {
            wLn("  ", moduleOption, " OFF");
          }
          else
          {
            wLn("  # ", moduleOption);
          }
        }
      }

      wLn(")");
      wLn();
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

    const auto kDefaultProjucerUserCodeSectionComment = std::vector<std::string>{
      "",
      "// (You can add your own code in this section, and the Projucer will not "
      "overwrite it)",
      ""};

    if (userCodeSectionLines != kDefaultProjucerUserCodeSectionComment)
    {
      wLn("jucer_appconfig_header(");
      wLn("  USER_CODE_SECTION");
      wLn("\"", escape("\\\"", join("\n", userCodeSectionLines)), "\"");
      wLn(")");
      wLn();
    }
  }

  // jucer_export_target() and jucer_export_target_configuration()
  {
    const auto supportedExporters = {std::make_pair("XCODE_MAC", "Xcode (MacOSX)"),
                                     std::make_pair("VS2017", "Visual Studio 2017"),
                                     std::make_pair("VS2015", "Visual Studio 2015"),
                                     std::make_pair("VS2013", "Visual Studio 2013"),
                                     std::make_pair("LINUX_MAKE", "Linux Makefile")};

    for (const auto& element : supportedExporters)
    {
      const auto exporter =
        jucerProject.getChildWithName("EXPORTFORMATS").getChildWithName(element.first);
      if (exporter.isValid())
      {
        const auto exporterType = exporter.getType().toString();
        const auto configurations = exporter.getChildWithName("CONFIGURATIONS");

        wLn("jucer_export_target(");
        wLn("  \"", element.second, "\"");

        if (exporterType == "XCODE_MAC"
            && (!exporter.getProperty("prebuildCommand").toString().isEmpty()
                || !exporter.getProperty("postbuildCommand").toString().isEmpty()))
        {
          wLn("  TARGET_PROJECT_FOLDER \"",
              exporter.getProperty("targetFolder").toString(),
              "\"  # only used by PREBUILD_SHELL_SCRIPT and POSTBUILD_SHELL_SCRIPT");
        }

        const auto isVSExporter = exporterType == "VS2017" || exporterType == "VS2015"
                                  || exporterType == "VS2013";

        if (isVSExporter)
        {
          const auto needsTargetFolder = [&configurations]() {
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
            wLn("  TARGET_PROJECT_FOLDER \"",
                exporter.getProperty("targetFolder").toString(),
                "\" # only used by PREBUILD_COMMAND and POSTBUILD_COMMAND");
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
                 .toString()
               == "enabled";

        if (!hasVst2Interface && (isVstAudioPlugin || isVstPluginHost))
        {
          convertSetting(exporter, "vstFolder", "VST_SDK_FOLDER");
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
                 .toString()
               == "enabled";

        if (supportsVst3 && (isVst3AudioPlugin || isVst3PluginHost))
        {
          convertSetting(exporter, "vst3Folder", "VST3_SDK_FOLDER");
        }

        convertSetting(exporter, "extraDefs", "EXTRA_PREPROCESSOR_DEFINITIONS");
        convertSetting(exporter, "extraCompilerFlags", "EXTRA_COMPILER_FLAGS");
        convertSetting(exporter, "extraLinkerFlags", "EXTRA_LINKER_FLAGS");
        convertSetting(exporter, "externalLibraries", "EXTERNAL_LIBRARIES_TO_LINK");

        const auto mainGroup = jucerProject.getChildWithName("MAINGROUP");

        const auto getIconFilePath =
          [&mainGroup, &exporter](const juce::Identifier& propertyName) -> std::string {
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

        wLn("  ICON_SMALL \"", (smallIconPath.empty() ? "<None>" : smallIconPath), "\"");
        wLn("  ICON_LARGE \"", (bigIconPath.empty() ? "<None>" : bigIconPath), "\"");

        if (exporterType == "XCODE_MAC")
        {
          convertSetting(exporter, "customXcodeResourceFolders",
                         "CUSTOM_XCODE_RESOURCE_FOLDERS");

          if (projectType == "guiapp")
          {
            convertSetting(exporter, "documentExtensions", "DOCUMENT_FILE_EXTENSIONS");
          }

          convertSetting(exporter, "customPList", "CUSTOM_PLIST");
          convertSetting(exporter, "extraFrameworks", "EXTRA_FRAMEWORKS");
          convertSetting(exporter, "prebuildCommand", "PREBUILD_SHELL_SCRIPT");
          convertSetting(exporter, "postbuildCommand", "POSTBUILD_SHELL_SCRIPT");

          if (exporter.hasProperty("iosDevelopmentTeamID"))
          {
            convertSetting(exporter, "iosDevelopmentTeamID", "DEVELOPMENT_TEAM_ID");
          }
        }

        if (isVSExporter)
        {
          const auto toolset = exporter.getProperty("toolset").toString();
          if (toolset.isEmpty())
          {
            wLn("  # PLATFORM_TOOLSET \"(default)\"");
          }
          else
          {
            wLn("  # PLATFORM_TOOLSET \"", toolset, "\"");
          }

          if (exporter.hasProperty("IPPLibrary"))
          {
            const auto useIppLibrary = [&exporter]() -> std::string {
              const auto value = exporter.getProperty("IPPLibrary").toString();

              if (value.isEmpty())
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
              wLn("  # USE_IPP_LIBRARY");
            }
            else
            {
              wLn("  USE_IPP_LIBRARY \"", useIppLibrary, "\"");
            }
          }

          if (exporterType == "VS2017" && exporter.hasProperty("cppLanguageStandard"))
          {
            const auto cppLanguageStandard = [&exporter]() -> std::string {
              const auto value = exporter.getProperty("cppLanguageStandard").toString();

              if (value.isEmpty())
                return "(default)";
              if (value == "stdcpp14")
                return "C++14";
              if (value == "stdcpplatest")
                return "Latest C++ Standard";
              return {};
            }();

            if (cppLanguageStandard.empty())
            {
              wLn("  # CXX_STANDARD_TO_USE");
            }
            else
            {
              wLn("  CXX_STANDARD_TO_USE \"", cppLanguageStandard, "\"");
            }
          }
        }

        if (exporterType == "LINUX_MAKE")
        {
          const auto cppLanguageStandard = [&exporter]() -> std::string {
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
            wLn("  # CXX_STANDARD_TO_USE");
          }
          else
          {
            wLn("  CXX_STANDARD_TO_USE \"", cppLanguageStandard, "\"");
          }

          convertSetting(exporter, "linuxExtraPkgConfig", "PKGCONFIG_LIBRARIES");
        }

        writeUserNotes(wLn, exporter);

        wLn(")");
        wLn();

        for (auto i = 0; i < configurations.getNumChildren(); ++i)
        {
          const auto configuration = configurations.getChild(i);

          wLn("jucer_export_target_configuration(");
          wLn("  \"", element.second, "\"");
          wLn("  NAME \"", configuration.getProperty("name").toString(), "\"");
          wLn("  DEBUG_MODE ",
              (bool{configuration.getProperty("isDebug")} ? "ON" : "OFF"));

          convertSetting(configuration, "targetName", "BINARY_NAME");
          convertSetting(configuration, "binaryPath", "BINARY_LOCATION");

          const auto isAbsolutePath = [](const juce::String& path) {
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
            wLn("  # HEADER_SEARCH_PATHS");
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

            wLn("  HEADER_SEARCH_PATHS \"",
                escape("\\", join("\n", absOrRelToJucerFileDirPaths)), "\"");
          }

          const auto libraryPath =
            configuration.getProperty("libraryPath").toString().toStdString();
          if (libraryPath.empty())
          {
            wLn("  # EXTRA_LIBRARY_SEARCH_PATHS");
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

            wLn("  EXTRA_LIBRARY_SEARCH_PATHS \"",
                escape("\\", join("\n", absOrRelToJucerFileDirPaths)), "\"");
          }

          convertSetting(configuration, "defines", "PREPROCESSOR_DEFINITIONS");

          const auto optimisation = [&configuration, &isVSExporter]() -> std::string {
            const auto value = configuration.getProperty("optimisation");

            if (value.isVoid())
              return {};

            if (isVSExporter)
              return getMsvcOptimisation(value);

            return getGccOptimisation(value);
          }();

          if (optimisation.empty())
          {
            wLn("  # OPTIMISATION");
          }
          else
          {
            wLn("  OPTIMISATION \"", optimisation, "\"");
          }

          if (exporterType == "XCODE_MAC")
          {
            if (isVstAudioPlugin)
            {
              convertSetting(configuration, "xcodeVstBinaryLocation",
                             "VST_BINARY_LOCATION");
            }

            if (isVst3AudioPlugin)
            {
              convertSetting(configuration, "xcodeVst3BinaryLocation",
                             "VST3_BINARY_LOCATION");
            }

            if (bool{jucerProject.getProperty("buildAU")})
            {
              convertSetting(configuration, "xcodeAudioUnitBinaryLocation",
                             "AU_BINARY_LOCATION");
            }

            const auto sdks = {"10.5 SDK", "10.6 SDK",  "10.7 SDK",  "10.8 SDK",
                               "10.9 SDK", "10.10 SDK", "10.11 SDK", "10.12 SDK"};

            const auto osxSDK =
              configuration.getProperty("osxSDK").toString().toStdString();
            if (osxSDK == "default")
            {
              wLn("  OSX_BASE_SDK_VERSION \"Use Default\"");
            }
            else if (std::find(sdks.begin(), sdks.end(), osxSDK) != sdks.end())
            {
              wLn("  OSX_BASE_SDK_VERSION \"", osxSDK, "\"");
            }
            else
            {
              wLn("  # OSX_BASE_SDK_VERSION");
            }

            const auto osxCompatibility =
              configuration.getProperty("osxCompatibility").toString().toStdString();
            if (osxCompatibility == "default")
            {
              wLn("  OSX_DEPLOYMENT_TARGET \"Use Default\"");
            }
            else if (std::find(sdks.begin(), sdks.end(), osxCompatibility) != sdks.end())
            {
              wLn("  OSX_DEPLOYMENT_TARGET \"",
                  osxCompatibility.substr(0, osxCompatibility.length() - 4), "\"");
            }
            else
            {
              wLn("  # OSX_DEPLOYMENT_TARGET");
            }

            const auto osxArchitecture = [&configuration]() -> std::string {
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
              wLn("  # OSX_ARCHITECTURE");
            }
            else
            {
              wLn("  OSX_ARCHITECTURE \"", osxArchitecture, "\"");
            }

            convertSetting(configuration, "customXcodeFlags", "CUSTOM_XCODE_FLAGS");

            const auto cppLanguageStandard = [&configuration]() -> std::string {
              const auto value =
                configuration.getProperty("cppLanguageStandard").toString();

              if (value.isEmpty())
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
              wLn("  # CXX_LANGUAGE_STANDARD");
            }
            else
            {
              wLn("  CXX_LANGUAGE_STANDARD \"", cppLanguageStandard, "\"");
            }

            const auto cppLibType = [&configuration]() -> std::string {
              const auto value = configuration.getProperty("cppLibType").toString();

              if (value.isEmpty())
                return "Use Default";

              if (value == "libc++")
                return "LLVM libc++";

              if (value == "libstdc++")
                return "GNU libstdc++";

              return {};
            }();

            if (cppLibType.empty())
            {
              wLn("  # CXX_LIBRARY");
            }
            else
            {
              wLn("  CXX_LIBRARY \"", cppLibType, "\"");
            }

            convertSetting(configuration, "codeSigningIdentity", "CODE_SIGNING_IDENTITY");
            convertOnOffSetting(configuration, "fastMath", "RELAX_IEEE_COMPLIANCE");
            convertOnOffSetting(configuration, "linkTimeOptimisation",
                                "LINK_TIME_OPTIMISATION");
            convertOnOffSetting(configuration, "stripLocalSymbols",
                                "STRIP_LOCAL_SYMBOLS");
          }

          if (isVSExporter)
          {
            const auto warningLevel = [&configuration]() -> std::string {
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
            wLn("  WARNING_LEVEL \"", warningLevel, "\"");

            convertOnOffSetting(configuration, "warningsAreErrors",
                                "TREAT_WARNINGS_AS_ERRORS");

            const auto runtimeLibrary = [&configuration]() -> std::string {
              const auto value = configuration.getProperty("useRuntimeLibDLL").toString();

              if (value.isEmpty())
                return "(Default)";

              if (value == "0")
                return "Use static runtime";

              if (value == "1")
                return "Use DLL runtime";

              return {};
            }();

            if (runtimeLibrary.empty())
            {
              wLn("  # RUNTIME_LIBRARY");
            }
            else
            {
              wLn("  RUNTIME_LIBRARY \"", runtimeLibrary, "\"");
            }

            if (configuration.getProperty("wholeProgramOptimisation")
                  .toString()
                  .isEmpty())
            {
              wLn("  WHOLE_PROGRAM_OPTIMISATION \"Enable when possible\"");
            }
            else if (int{configuration.getProperty("wholeProgramOptimisation")} > 0)
            {
              wLn("  WHOLE_PROGRAM_OPTIMISATION \"Always disable\"");
            }
            else
            {
              wLn("  # WHOLE_PROGRAM_OPTIMISATION");
            }

            convertOnOffSetting(configuration, "enableIncrementalLinking",
                                "INCREMENTAL_LINKING");
            convertSetting(configuration, "prebuildCommand", "PREBUILD_COMMAND");
            convertSetting(configuration, "postbuildCommand", "POSTBUILD_COMMAND");
            convertOnOffSetting(configuration, "generateManifest", "GENERATE_MANIFEST");

            const auto characterSet =
              configuration.getProperty("characterSet").toString();

            if (characterSet.isEmpty())
            {
              wLn("  CHARACTER_SET \"Default\"");
            }
            else
            {
              wLn("  CHARACTER_SET \"", characterSet, "\"");
            }

            const auto winArchitecture =
              configuration.getProperty("winArchitecture").toString();

            if (winArchitecture.isEmpty())
            {
              wLn("  # ARCHITECTURE");
            }
            else
            {
              wLn("  # ARCHITECTURE \"", winArchitecture, "\"");
            }

            convertOnOffSetting(configuration, "fastMath", "RELAX_IEEE_COMPLIANCE");
          }

          if (exporterType == "LINUX_MAKE")
          {
            if (!configuration.hasProperty("linuxArchitecture"))
            {
              wLn("  ARCHITECTURE \"(Default)\"");
            }
            else
            {
              const auto architecture = [&configuration]() -> std::string {
                const auto value =
                  configuration.getProperty("linuxArchitecture").toString();

                if (value.isEmpty())
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
                wLn("  # ARCHITECTURE");
              }
              else
              {
                wLn("  ARCHITECTURE \"", architecture, "\"");
              }
            }
          }

          writeUserNotes(wLn, configuration);

          wLn(")");
          wLn();
        }
      }
    }
  }

  out << "jucer_project_end()" << kNewLine << std::flush;

  std::cout << juce::File::getCurrentWorkingDirectory()
                 .getChildFile("CMakeLists.txt")
                 .getFullPathName()
            << " has been successfully generated." << std::endl;

  return 0;
}
