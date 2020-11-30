// Copyright (C) 2017  Matthieu Talbot
// Copyright (C) 2017-2020  Alain Martin
// Copyright (C) 2017  Florian Goltz
// Copyright (C) 2019  Johannes Elliesen
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

#if defined(__clang__)
  #pragma clang diagnostic push
  #if __has_warning("-Watomic-implicit-seq-cst")
    #pragma clang diagnostic ignored "-Watomic-implicit-seq-cst"
  #endif
  #pragma clang diagnostic ignored "-Wcast-align"
  #pragma clang diagnostic ignored "-Wcast-qual"
  #pragma clang diagnostic ignored "-Wdocumentation"
  #pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
  #pragma clang diagnostic ignored "-Wextra-semi"
  #pragma clang diagnostic ignored "-Wglobal-constructors"
  #pragma clang diagnostic ignored "-Wimplicit-fallthrough"
  #if __has_warning("-Winconsistent-missing-destructor-override")
    #pragma clang diagnostic ignored "-Winconsistent-missing-destructor-override"
  #endif
  #pragma clang diagnostic ignored "-Wold-style-cast"
  #pragma clang diagnostic ignored "-Wsign-conversion"
  #pragma clang diagnostic ignored "-Wundef"
  #if __has_warning("-Wundefined-func-template")
    #pragma clang diagnostic ignored "-Wundefined-func-template"
  #endif
  #if __has_warning("-Wunused-template")
    #pragma clang diagnostic ignored "-Wunused-template"
  #endif
  #pragma clang diagnostic ignored "-Wweak-vtables"
  #if __has_warning("-Wzero-as-null-pointer-constant")
    #pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
  #endif

#elif defined(__GNUC__)
  #pragma GCC diagnostic push
  #if __GNUC__ >= 6
    #pragma GCC diagnostic ignored "-Wmisleading-indentation"
  #endif
  #if __GNUC__ >= 8
    #pragma GCC diagnostic ignored "-Wclass-memaccess"
  #endif

#elif defined(_MSC_VER)
  #pragma warning(push)
  #pragma warning(disable : 4800)

#endif

#include <argh/argh.h>
#include <juce_core/juce_core.h>

#if defined(__clang__)
  #pragma clang diagnostic pop
#elif defined(__GNUC__)
  #pragma GCC diagnostic pop
#elif defined(_MSC_VER)
  #pragma warning(pop)
#endif

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <locale>
#include <map>
#include <memory>
#include <regex>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>
#include <vector>


#if !defined(WRITE_CRLF_LINE_ENDINGS)
  #error WRITE_CRLF_LINE_ENDINGS must be defined
#endif
#if WRITE_CRLF_LINE_ENDINGS
static const auto kNewLine = "\r\n";
#else
static const auto kNewLine = '\n';
#endif

#if !defined(IS_PAID_OR_GPL)
  #error IS_PAID_OR_GPL must be defined
#endif
#if IS_PAID_OR_GPL
static const auto kDefaultLicenseBasedValue = "OFF";
#else
static const auto kDefaultLicenseBasedValue = "ON";
#endif


namespace
{

struct LineWriter
{
  explicit LineWriter(juce::MemoryOutputStream& stream)
    : mStream(stream)
  {
  }

  LineWriter(const LineWriter&) = delete;
  LineWriter& operator=(const LineWriter&) = delete;

  template <class Head>
  void writeToStream(juce::MemoryOutputStream& stream, Head&& head)
  {
    stream << std::forward<Head>(head);
  }

  template <class Head, class... Tail>
  void writeToStream(juce::MemoryOutputStream& stream, Head&& head, Tail&&... tail)
  {
    stream << std::forward<Head>(head);
    writeToStream(stream, std::forward<Tail>(tail)...);
  }

  template <typename... Args>
  void operator()(Args&&... args)
  {
    writeToStream(mStream, std::forward<Args>(args)..., kNewLine);
  }

private:
  juce::MemoryOutputStream& mStream;
};


void printError(const juce::String& error)
{
  std::cerr << "error: " << error << std::endl;
}


juce::File getChildFileFromWorkingDirectory(const juce::StringRef relativeOrAbsolutePath)
{
  return juce::File::getCurrentWorkingDirectory().getChildFile(relativeOrAbsolutePath);
}


// Matches juce::var::VariantType_String::toBool. This means that `toBoolLikeVar(s)` and
// `bool{juce::var{s}}` are equivalent.
bool toBoolLikeVar(const juce::String& s)
{
  return s.getIntValue() != 0 || s.trim().equalsIgnoreCase("true")
         || s.trim().equalsIgnoreCase("yes");
}


juce::String makeValidIdentifier(juce::String s)
{
  if (s.isEmpty())
  {
    return "unknown";
  }

  s = s.replaceCharacters(".,;:/@", "______")
        .retainCharacters(
          "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789");

  if (juce::CharacterFunctions::isDigit(s[0]))
  {
    s = "_" + s;
  }

  return s;
}


// Matches _FRUT_make_valid_configuration_name in Reprojucer.cmake.
juce::String makeValidConfigurationName(const juce::String& configurationName)
{
  auto validName = configurationName.toStdString();

  validName = std::regex_replace(validName, std::regex{"[^A-Za-z0-9_]+"}, " ");
  validName = juce::String{validName}.trim().toStdString();
  validName = std::regex_replace(validName, std::regex{"[ ]+"}, "_");

  return validName;
}


juce::String cmakePath(const juce::String& path)
{
  return (juce::File::isAbsolutePath(path) ? path : "${CMAKE_CURRENT_LIST_DIR}/" + path)
    .replace("\\", "/");
}


juce::String escape(const juce::String& charsToEscape, juce::String value)
{
  auto pos = 0;

  while ((pos = value.indexOfAnyOf(charsToEscape, pos)) != -1)
  {
    value = value.replaceSection(pos, 0, "\\");
    pos += 2;
  }

  return value;
}


juce::StringArray
convertIdsToStrings(const juce::StringArray& ids,
                    const std::vector<std::pair<juce::String, const char*>>& idsToStrings)
{
  juce::StringArray strings;
  for (const auto& idToString : idsToStrings)
  {
    if (ids.contains(idToString.first))
    {
      strings.add(idToString.second);
    }
  }
  return strings;
}


juce::StringArray parsePreprocessorDefinitions(const juce::String& input)
{
  juce::StringArray definitions;

  for (auto s = input.getCharPointer(); !s.isEmpty();)
  {
    s = s.findEndOfWhitespace();
    juce::String definition;

    while (!s.isEmpty() && !s.isWhitespace() && *s != '=')
    {
      definition << s.getAndAdvance();
    }

    s = s.findEndOfWhitespace();
    juce::String value;

    if (*s == '=')
    {
      ++s;

      while (!s.isEmpty() && *s == ' ')
      {
        ++s;
      }

      while (!s.isEmpty() && !s.isWhitespace())
      {
        if (*s == ',')
        {
          ++s;
          break;
        }

        if (*s == '\\' && (s[1] == ' ' || s[1] == ','))
        {
          ++s;
        }

        value << s.getAndAdvance();
      }
    }

    if (definition.isNotEmpty())
    {
      definitions.add(value.isEmpty() ? definition : definition + '=' + value);
    }
  }

  return definitions;
}


const juce::XmlElement& getFallbackXmlElement() noexcept
{
  static const juce::XmlElement fallbackXmlElement{":"};
  return fallbackXmlElement;
}


const juce::XmlElement& safeGetChildByName(const juce::XmlElement& element,
                                           const juce::StringRef childName)
{
  if (const auto pChild = element.getChildByName(childName))
  {
    return *pChild;
  }

  return getFallbackXmlElement();
}


const juce::XmlElement*
getChildByAttributeRecursively(const juce::XmlElement& parent,
                               const juce::StringRef attributeName,
                               const juce::StringRef attributeValue)
{
  if (const auto pChild = parent.getChildByAttribute(attributeName, attributeValue))
  {
    return pChild;
  }

  for (auto pChild = parent.getFirstChildElement(); pChild != nullptr;
       pChild = pChild->getNextElement())
  {
    if (!pChild->isTextElement())
    {
      if (const auto pGrandChild =
            getChildByAttributeRecursively(*pChild, attributeName, attributeValue))
      {
        return pGrandChild;
      }
    }
  }

  return nullptr;
}


std::unique_ptr<juce::XmlElement> parseProjucerSettings()
{
  const auto projucerSettingsDirectory =
#if defined(JUCE_LINUX) && JUCE_LINUX
    juce::File{"~/.config/Projucer"};
#elif defined(JUCE_MAC) && JUCE_MAC
    juce::File{"~/Library/Application Support/Projucer"};
#elif defined(JUCE_WINDOWS) && JUCE_WINDOWS
    juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
      .getChildFile("Projucer");
#else
  #error Unknown platform
#endif

  return std::unique_ptr<juce::XmlElement>{juce::XmlDocument::parse(
    projucerSettingsDirectory.getChildFile("Projucer.settings"))};
}


void writeUserNotes(LineWriter& wLn, const juce::XmlElement& element)
{
  if (element.hasAttribute("userNotes"))
  {
    wLn("  # NOTES");
    const auto& userNotes = element.getStringAttribute("userNotes");
    for (const auto& line : juce::StringArray::fromLines(userNotes))
    {
      wLn("  #   ", line);
    }
  }
}


struct Arguments
{
  juce::String jucerFilePath;
  juce::String reprojucerFilePath;
  juce::String juceModulesPath;
  juce::String userModulesPath;
  bool relocatable;
};


Arguments parseArguments(const int argc, const char* const argv[])
{
  const juce::StringArray knownFlags{"h", "help", "relocatable"};
  const juce::StringArray knownParams{"juce-modules", "user-modules"};

  argh::parser argumentParser;
  for (const auto& param : knownParams)
  {
    argumentParser.add_param(param.toStdString());
  }
  argumentParser.parse(argc, argv);

  const auto askingForHelp = argumentParser[{"-h", "--help"}];
  auto errorInArguments = false;

  if (!askingForHelp)
  {
    if (argumentParser.size() >= 2 && argumentParser[1] != "reprojucer")
    {
      printError("invalid mode \"" + argumentParser[1] + "\"");
      errorInArguments = true;
    }

    if (argumentParser.size() < 3)
    {
      printError("not enough positional arguments");
      errorInArguments = true;
    }
    else if (argumentParser.size() > 4)
    {
      printError("too many positional arguments");
      errorInArguments = true;
    }
  }

  for (const auto& flag : argumentParser.flags())
  {
    if (knownParams.contains(juce::String{flag}))
    {
      printError("expected one argument for \"" + flag + "\"");
      errorInArguments = true;
    }
    else if (!knownFlags.contains(juce::String{flag}))
    {
      printError("unknown option \"" + flag + "\"");
      errorInArguments = true;
    }
  }

  for (const auto& paramAndValue : argumentParser.params())
  {
    const auto& param = std::get<0>(paramAndValue);
    if (knownFlags.contains(juce::String{param}))
    {
      const auto& value = std::get<1>(paramAndValue);
      printError("unexpected argument \"" + value + "\" for \"" + param + "\"");
      errorInArguments = true;
    }
    else if (!knownParams.contains(juce::String{param}))
    {
      printError("unknown option \"" + param + "\"");
      errorInArguments = true;
    }
  }

  const auto reprojucerUsage =
    "usage: Jucer2CMake reprojucer <jucer_project_file> [<Reprojucer.cmake_file>]\n"
    "                   [--help] [--juce-modules=<path>] [--user-modules=<path>]\n"
    "                   [--relocatable]\n";
  const auto reprojucerHelpText =
    "\n"
    "Converts a .jucer file into a CMakeLists.txt file that uses Reprojucer.cmake.\n"
    "The CMakeLists.txt file is written in the current working directory.\n"
    "\n"
    "    <jucer_project_file>      path to the .jucer file to convert\n"
    "    <Reprojucer.cmake_file>   path to Reprojucer.cmake\n"
    "\n"
    "    -h, --help                show this help message and exit\n"
    "    --juce-modules <path>     global path to JUCE modules\n"
    "    --user-modules <path>     global path to user modules\n"
    "    --relocatable             makes the CMakeLists.txt file independent from\n"
    "                              the location of the .jucer file, but requires\n"
    "                              defining a variable when calling cmake\n";

  if (askingForHelp || errorInArguments)
  {
    std::cerr << reprojucerUsage << std::flush;

    if (askingForHelp)
    {
      std::cerr << reprojucerHelpText << std::flush;
    }

    std::exit(askingForHelp ? 0 : 1);
  }

  auto jucerFilePath = juce::String{argumentParser[2]};

  auto reprojucerFilePath = juce::String{argumentParser[3]};
  if (reprojucerFilePath.isNotEmpty()
      && (!reprojucerFilePath.endsWith("Reprojucer.cmake")
          || !getChildFileFromWorkingDirectory(reprojucerFilePath).existsAsFile()))
  {
    printError("'" + reprojucerFilePath + "' is not a valid Reprojucer.cmake file.");
    std::exit(1);
  }

  auto juceModulesPath = juce::String{argumentParser("--juce-modules").str()};
  if (argumentParser("--juce-modules"))
  {
    if (juceModulesPath.isEmpty()
        || !getChildFileFromWorkingDirectory(juceModulesPath).isDirectory())
    {
      printError("No such directory (--juce-modules): '" + juceModulesPath + "'");
      std::exit(1);
    }
  }

  auto userModulesPath = juce::String{argumentParser("--user-modules").str()};
  if (argumentParser("--user-modules"))
  {
    if (userModulesPath.isEmpty()
        || !getChildFileFromWorkingDirectory(userModulesPath).isDirectory())
    {
      printError("No such directory (--user-modules): '" + userModulesPath + "'");
      std::exit(1);
    }
  }

  return {std::move(jucerFilePath), std::move(reprojucerFilePath),
          std::move(juceModulesPath), std::move(userModulesPath),
          argumentParser["--relocatable"]};
}

} // namespace


int main(int argc, char* argv[])
{
  const auto args = parseArguments(argc, argv);

  const auto jucerFile = getChildFileFromWorkingDirectory(args.jucerFilePath);

  const auto pJucerProject =
    std::unique_ptr<juce::XmlElement>{juce::XmlDocument::parse(jucerFile)};
  if (pJucerProject == nullptr || !pJucerProject->hasTagName("JUCERPROJECT"))
  {
    printError("'" + args.jucerFilePath + "' is not a valid Jucer project.");
    return 1;
  }

  const auto& jucerProject = *pJucerProject;

  const auto jucerVersion = jucerProject.getStringAttribute("jucerVersion", "6.0.0");
  const auto jucerVersionTokens = juce::StringArray::fromTokens(jucerVersion, ".", {});
  if (jucerVersionTokens.size() != 3)
  {
    printError("'" + args.jucerFilePath + "' is not a valid Jucer project.");
    return 1;
  }

  using Version = std::tuple<int, int, int>;

  const auto jucerVersionAsTuple = [&jucerVersionTokens, &args]() {
    try
    {
      return Version{std::stoi(jucerVersionTokens[0].toStdString()),
                     std::stoi(jucerVersionTokens[1].toStdString()),
                     std::stoi(jucerVersionTokens[2].toStdString())};
    }
    catch (const std::invalid_argument&)
    {
      printError("'" + args.jucerFilePath + "' is not a valid Jucer project.");
      std::exit(1);
    }
  }();

  auto needsJuceModulesGlobalPath = false;
  auto needsUserModulesGlobalPath = false;

  if (const auto pModules = jucerProject.getChildByName("MODULES"))
  {
    for (auto pModule = pModules->getFirstChildElement(); pModule != nullptr;
         pModule = pModule->getNextElement())
    {
      if (pModule->isTextElement())
      {
        continue;
      }

      if (toBoolLikeVar(pModule->getStringAttribute("useGlobalPath")))
      {
        if (pModule->getStringAttribute("id").startsWith("juce_"))
        {
          needsJuceModulesGlobalPath = true;
        }
        else
        {
          needsUserModulesGlobalPath = true;
        }
      }
    }
  }

  const auto shouldParseProjucerSettings =
    (needsJuceModulesGlobalPath && args.juceModulesPath.isEmpty())
    || (needsUserModulesGlobalPath && args.userModulesPath.isEmpty());

  const auto pProjucerSettings =
    shouldParseProjucerSettings ? parseProjucerSettings() : nullptr;

  const auto pProjucerGlobalPaths = [&pProjucerSettings]() -> juce::XmlElement* {
    if (pProjucerSettings && pProjucerSettings->hasTagName("PROPERTIES"))
    {
      if (const auto pValue =
            pProjucerSettings->getChildByAttribute("name", "PROJECT_DEFAULT_SETTINGS"))
      {
        return pValue->getChildByName("PROJECT_DEFAULT_SETTINGS");
      }
    }
    return nullptr;
  }();

  const auto& juceModulesGlobalPath =
    args.juceModulesPath.isNotEmpty()
      ? args.juceModulesPath
      : needsJuceModulesGlobalPath && pProjucerGlobalPaths
          ? pProjucerGlobalPaths->getStringAttribute("defaultJuceModulePath")
          : juce::String{};

  if (needsJuceModulesGlobalPath && juceModulesGlobalPath.isEmpty())
  {
    printError(
      "At least one JUCE module used in " + args.jucerFilePath
      + " relies on the global \"JUCE Modules\" path set in Projucer. You must "
        "provide this path using --juce-modules=\"<global-JUCE-modules-path>\".");
    return 1;
  }

  const auto& userModulesGlobalPath =
    args.userModulesPath.isNotEmpty()
      ? args.userModulesPath
      : needsUserModulesGlobalPath && pProjucerGlobalPaths
          ? pProjucerGlobalPaths->getStringAttribute("defaultUserModulePath")
          : juce::String{};

  if (needsUserModulesGlobalPath && userModulesGlobalPath.isEmpty())
  {
    printError(
      "At least one user module used in " + args.jucerFilePath
      + " relies on the global \"User Modules\" path set in Projucer. You must "
        "provide this path using --user-modules=\"<global-user-modules-path>\".");
    return 1;
  }

  juce::MemoryOutputStream outputStream;
  LineWriter wLn{outputStream};

  const auto convertSetting =
    [&wLn](const juce::XmlElement& element, const juce::StringRef attributeName,
           const juce::String& cmakeKeyword,
           const std::function<juce::String(const juce::String&)>& converterFn) {
      const auto value = converterFn
                           ? converterFn(element.getStringAttribute(attributeName))
                           : element.getStringAttribute(attributeName);

      if (value.isEmpty())
      {
        wLn("  # ", cmakeKeyword);
      }
      else
      {
        wLn("  ", cmakeKeyword, " \"", escape("\\\";", value.trimCharactersAtEnd("\\")),
            "\"");
      }
    };

  const auto convertSettingIfDefined =
    [&convertSetting](
      const juce::XmlElement& element, const juce::StringRef attributeName,
      const juce::String& cmakeKeyword,
      const std::function<juce::String(const juce::String&)>& converterFn) {
      if (element.hasAttribute(attributeName))
      {
        convertSetting(element, attributeName, cmakeKeyword, converterFn);
      }
    };

  const auto convertSettingWithDefault =
    [&convertSetting](
      const juce::XmlElement& element, const juce::StringRef attributeName,
      const juce::String& cmakeKeyword, const juce::String& defaultValue) {
      if (element.hasAttribute(attributeName))
      {
        convertSetting(element, attributeName, cmakeKeyword, {});
      }
      else
      {
        convertSetting(element, attributeName, cmakeKeyword,
                       [&defaultValue](const juce::String&) { return defaultValue; });
      }
    };

  const auto convertOnOffSetting =
    [&wLn](const juce::XmlElement& element, const juce::StringRef attributeName,
           const juce::String& cmakeKeyword,
           const std::function<juce::String(const juce::String&)>& converterFn) {
      const auto value =
        converterFn
          ? converterFn(element.getStringAttribute(attributeName))
          : juce::String{
            element.hasAttribute(attributeName)
              ? toBoolLikeVar(element.getStringAttribute(attributeName)) ? "ON" : "OFF"
              : ""};

      if (value.isEmpty())
      {
        wLn("  # ", cmakeKeyword);
      }
      else
      {
        wLn("  ", cmakeKeyword, " ", value);
      }
    };

  const auto convertOnOffSettingIfDefined =
    [&convertOnOffSetting](
      const juce::XmlElement& element, const juce::StringRef attributeName,
      const juce::String& cmakeKeyword,
      const std::function<juce::String(const juce::String&)>& converterFn) {
      if (element.hasAttribute(attributeName))
      {
        convertOnOffSetting(element, attributeName, cmakeKeyword, converterFn);
      }
    };

  const auto convertOnOffSettingWithDefault = [&convertOnOffSetting](
                                                const juce::XmlElement& element,
                                                const juce::StringRef attributeName,
                                                const juce::String& cmakeKeyword,
                                                const juce::String& defaultValue) {
    if (element.hasAttribute(attributeName))
    {
      convertOnOffSetting(element, attributeName, cmakeKeyword, {});
    }
    else
    {
      convertOnOffSetting(element, attributeName, cmakeKeyword,
                          [&defaultValue](const juce::String&) { return defaultValue; });
    }
  };

  const auto convertSettingAsList =
    [&wLn](const juce::XmlElement& element, const juce::StringRef attributeName,
           const juce::String& cmakeKeyword,
           const std::function<juce::StringArray(const juce::String&)>& converterFn) {
      const auto values =
        converterFn
          ? converterFn(element.getStringAttribute(attributeName))
          : juce::StringArray::fromLines(element.getStringAttribute(attributeName));

      if (values.isEmpty())
      {
        wLn("  # ", cmakeKeyword);
      }
      else
      {
        wLn("  ", cmakeKeyword);

        for (const auto& item : values)
        {
          if (item.isNotEmpty())
          {
            wLn("    \"", escape("\\\";", item.trimCharactersAtEnd("\\")), "\"");
          }
        }
      }
    };

  const auto convertSettingAsListIfDefined =
    [&convertSettingAsList](
      const juce::XmlElement& element, const juce::StringRef attributeName,
      const juce::String& cmakeKeyword,
      const std::function<juce::StringArray(const juce::String&)>& converterFn) {
      if (element.hasAttribute(attributeName))
      {
        convertSettingAsList(element, attributeName, cmakeKeyword, converterFn);
      }
    };

  const auto jucerFileName = jucerFile.getFileName();
  const auto& jucerProjectName = jucerProject.getStringAttribute("name");

  // Preamble
  {
    wLn("# This file was generated by FRUT's Jucer2CMake from \"", jucerFileName, "\"");
    wLn();
    wLn("cmake_minimum_required(VERSION 3.4)");
    wLn();
    wLn("project(\"", jucerProjectName, "\")");
    wLn();
    wLn();
  }

  // include(Reprojucer)
  {
    if (args.reprojucerFilePath.isNotEmpty())
    {
      const auto relativeReprojucerDirPath =
        getChildFileFromWorkingDirectory(args.reprojucerFilePath)
          .getParentDirectory()
          .getRelativePathFrom(juce::File::getCurrentWorkingDirectory());
      wLn("list(APPEND CMAKE_MODULE_PATH \"", cmakePath(relativeReprojucerDirPath),
          "\")");
    }
    else
    {
      wLn("# list(APPEND CMAKE_MODULE_PATH"
          " \"${CMAKE_CURRENT_LIST_DIR}/<relative_path_to_FRUT>/cmake\")");
    }
    wLn("include(Reprojucer)");
    wLn();
    wLn();
  }

  auto escapedJucerFileName = jucerFileName.toStdString();
  std::replace_if(
    escapedJucerFileName.begin(), escapedJucerFileName.end(),
    [](const std::string::value_type& c) {
      return !(std::isalpha(c, std::locale::classic())
               || std::isdigit(c, std::locale::classic()));
    },
    '_');
  const auto jucerFileCMakeVar = juce::String{escapedJucerFileName} + "_FILE";

  // get_filename_component() or set(*_FILE)
  {
    if (args.relocatable)
    {
      wLn("if(NOT DEFINED ", jucerFileCMakeVar, ")");
      wLn("  message(FATAL_ERROR \"", jucerFileCMakeVar, " must be defined\")");
      wLn("endif()");
      wLn();
      wLn("get_filename_component(", jucerFileCMakeVar);
      wLn("  \"${", jucerFileCMakeVar, "}\" ABSOLUTE");
      wLn("  BASE_DIR \"${CMAKE_BINARY_DIR}\"");
      wLn(")");
    }
    else
    {
      wLn("set(", jucerFileCMakeVar);
      const auto relativeJucerFilePath =
        getChildFileFromWorkingDirectory(args.jucerFilePath)
          .getRelativePathFrom(juce::File::getCurrentWorkingDirectory());
      wLn("  \"", cmakePath(relativeJucerFilePath), "\"");
      wLn(")");
    }
    wLn();
    wLn();
  }

  // set({JUCE,USER}_MODULES_GLOBAL_PATH)
  {
    if (juceModulesGlobalPath.isNotEmpty())
    {
      std::cout << "Using '" << juceModulesGlobalPath
                << "' as global \"JUCE Modules\" path." << std::endl;
      wLn("set(JUCE_MODULES_GLOBAL_PATH \"", cmakePath(juceModulesGlobalPath), "\")");
    }

    if (userModulesGlobalPath.isNotEmpty())
    {
      std::cout << "Using '" << userModulesGlobalPath
                << "' as global \"User Modules\" path." << std::endl;
      wLn("set(USER_MODULES_GLOBAL_PATH \"", cmakePath(userModulesGlobalPath), "\")");
    }

    if (juceModulesGlobalPath.isNotEmpty() || userModulesGlobalPath.isNotEmpty())
    {
      wLn();
      wLn();
    }
  }

  // jucer_project_begin()
  {
    wLn("jucer_project_begin(");
    if (jucerProject.hasAttribute("jucerFormatVersion"))
    {
      wLn("  JUCER_FORMAT_VERSION \"",
          jucerProject.getStringAttribute("jucerFormatVersion"), "\"");
    }
    else
    {
      wLn("  JUCER_VERSION \"", jucerVersion, "\"");
    }
    wLn("  PROJECT_FILE \"${", jucerFileCMakeVar, "}\"");
    convertSetting(jucerProject, "id", "PROJECT_ID", {});
    wLn(")");
    wLn();
  }

  const auto& projectType = jucerProject.getStringAttribute("projectType");

  // jucer_project_settings()
  {
    wLn("jucer_project_settings(");
    convertSetting(jucerProject, "name", "PROJECT_NAME", {});
    convertSettingWithDefault(jucerProject, "version", "PROJECT_VERSION", "1.0.0");

    convertSettingIfDefined(jucerProject, "companyName", "COMPANY_NAME", {});
    convertSettingIfDefined(jucerProject, "companyCopyright", "COMPANY_COPYRIGHT", {});
    convertSettingIfDefined(jucerProject, "companyWebsite", "COMPANY_WEBSITE", {});
    convertSettingIfDefined(jucerProject, "companyEmail", "COMPANY_EMAIL", {});

    if (jucerVersionAsTuple >= Version{5, 0, 0})
    {
      const auto tagLine = juce::String{" # Required for closed source applications"
                                        " without an Indie or Pro JUCE license"};

      if (jucerProject.hasAttribute("reportAppUsage"))
      {
        convertOnOffSetting(jucerProject, "reportAppUsage", "REPORT_JUCE_APP_USAGE",
                            [&tagLine](const juce::String& value) {
                              return (toBoolLikeVar(value) ? "ON" : "OFF") + tagLine;
                            });
      }
      else
      {
        wLn("  REPORT_JUCE_APP_USAGE ", kDefaultLicenseBasedValue, tagLine);
      }

      if (jucerProject.hasAttribute("displaySplashScreen"))
      {
        convertOnOffSetting(jucerProject, "displaySplashScreen",
                            "DISPLAY_THE_JUCE_SPLASH_SCREEN",
                            [&tagLine](const juce::String& value) {
                              return (toBoolLikeVar(value) ? "ON" : "OFF") + tagLine;
                            });
      }
      else
      {
        wLn("  DISPLAY_THE_JUCE_SPLASH_SCREEN ", kDefaultLicenseBasedValue, tagLine);
      }

      convertSettingIfDefined(jucerProject, "splashScreenColour", "SPLASH_SCREEN_COLOUR",
                              {});
    }

    const auto projectTypeDescription = [&projectType]() -> juce::String {
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

    const auto defaultCompanyName = [&jucerProject]() {
      const auto& companyNameString = jucerProject.getStringAttribute("companyName");
      return companyNameString.isEmpty() ? "yourcompany" : companyNameString;
    }();

    const auto defaultBundleIdentifier =
      jucerVersionAsTuple >= Version{5, 4, 0}
        ? "com." + makeValidIdentifier(defaultCompanyName) + "."
            + makeValidIdentifier(jucerProjectName)
        : "com.yourcompany." + makeValidIdentifier(jucerProjectName);

    convertSettingWithDefault(jucerProject, "bundleIdentifier", "BUNDLE_IDENTIFIER",
                              defaultBundleIdentifier);

    convertSettingIfDefined(jucerProject, "maxBinaryFileSize", "BINARYDATACPP_SIZE_LIMIT",
                            [](const juce::String& value) -> juce::String {
                              if (value.isEmpty())
                                return "Default";
                              return juce::File::descriptionOfSizeInBytes(
                                value.getIntValue());
                            });
    if (jucerProject.hasAttribute("includeBinaryInJuceHeader"))
    {
      convertOnOffSetting(jucerProject, "includeBinaryInJuceHeader", "INCLUDE_BINARYDATA",
                          {});
    }
    else
    {
      convertOnOffSettingIfDefined(jucerProject, "includeBinaryInAppConfig",
                                   "INCLUDE_BINARYDATA", {});
    }
    convertSettingIfDefined(jucerProject, "binaryDataNamespace", "BINARYDATA_NAMESPACE",
                            {});

    if (jucerProject.hasAttribute("cppLanguageStandard"))
    {
      convertSetting(jucerProject, "cppLanguageStandard", "CXX_LANGUAGE_STANDARD",
                     [](const juce::String& value) -> juce::String {
                       if (value == "11")
                         return "C++11";

                       if (value == "14")
                         return "C++14";

                       if (value == "17")
                         return "C++17";

                       if (value == "latest")
                         return "Use Latest";

                       return {};
                     });
    }
    else if (jucerVersionAsTuple > Version{5, 2, 0})
    {
      wLn("  ", "CXX_LANGUAGE_STANDARD", " \"C++14\"");
    }
    else if (jucerVersionAsTuple > Version{5, 0, 2})
    {
      wLn("  ", "CXX_LANGUAGE_STANDARD", " \"C++11\"");
    }

    convertSettingAsListIfDefined(jucerProject, "defines", "PREPROCESSOR_DEFINITIONS",
                                  parsePreprocessorDefinitions);
    convertSettingAsListIfDefined(
      jucerProject, "headerPath", "HEADER_SEARCH_PATHS", [](const juce::String& value) {
        return juce::StringArray::fromTokens(value, ";\r\n", {});
      });

    convertSettingIfDefined(jucerProject, "postExportShellCommandPosix",
                            "POST_EXPORT_SHELL_COMMAND_MACOS_LINUX", {});
    convertSettingIfDefined(jucerProject, "postExportShellCommandWin",
                            "POST_EXPORT_SHELL_COMMAND_WINDOWS", {});

    writeUserNotes(wLn, jucerProject);

    wLn(")");
    wLn();

    // jucer_audio_plugin_settings()
    if (projectType == "audioplug")
    {
      wLn("jucer_audio_plugin_settings(");

      const auto vstIsLegacy = jucerVersionAsTuple > Version{5, 3, 2};

      if (jucerVersionAsTuple >= Version{5, 3, 1})
      {
        if (jucerProject.hasAttribute("pluginFormats"))
        {
          convertSettingAsList(
            jucerProject, "pluginFormats", "PLUGIN_FORMATS",
            [&jucerVersionAsTuple, &vstIsLegacy](const juce::String& value) {
              const auto supportsUnity = jucerVersionAsTuple >= Version{5, 3, 2};
              return convertIdsToStrings(
                juce::StringArray::fromTokens(value, ",", {}),
                {{vstIsLegacy ? "" : "buildVST", vstIsLegacy ? "" : "VST"},
                 {"buildVST3", "VST3"},
                 {"buildAU", "AU"},
                 {"buildAUv3", "AUv3"},
                 {"buildRTAS", "RTAS"},
                 {"buildAAX", "AAX"},
                 {"buildStandalone", "Standalone"},
                 {supportsUnity ? "buildUnity" : "", supportsUnity ? "Unity" : ""},
                 {"enableIAA", "Enable IAA"},
                 {vstIsLegacy ? "buildVST" : "", vstIsLegacy ? "VST (Legacy)" : ""}});
            });
        }
        else
        {
          convertSettingAsList(
            jucerProject, "pluginFormats", "PLUGIN_FORMATS",
            [&vstIsLegacy](const juce::String&) {
              return juce::StringArray{vstIsLegacy ? "VST3" : "VST", "AU", "Standalone"};
            });
        }

        if (jucerProject.hasAttribute("pluginCharacteristicsValue"))
        {
          convertSettingAsList(
            jucerProject, "pluginCharacteristicsValue", "PLUGIN_CHARACTERISTICS",
            [](const juce::String& value) {
              return convertIdsToStrings(
                juce::StringArray::fromTokens(value, ",", {}),
                {{"pluginIsSynth", "Plugin is a Synth"},
                 {"pluginWantsMidiIn", "Plugin MIDI Input"},
                 {"pluginProducesMidiOut", "Plugin MIDI Output"},
                 {"pluginIsMidiEffectPlugin", "MIDI Effect Plugin"},
                 {"pluginEditorRequiresKeys", "Plugin Editor Requires Keyboard Focus"},
                 {"pluginRTASDisableBypass", "Disable RTAS Bypass"},
                 {"pluginAAXDisableBypass", "Disable AAX Bypass"},
                 {"pluginRTASDisableMultiMono", "Disable RTAS Multi-Mono"},
                 {"pluginAAXDisableMultiMono", "Disable AAX Multi-Mono"}});
            });
        }
        else
        {
          wLn("  # PLUGIN_CHARACTERISTICS");
        }
      }
      else
      {
        convertOnOffSettingWithDefault(jucerProject, "buildVST", "BUILD_VST", "ON");
        convertOnOffSettingWithDefault(jucerProject, "buildVST3", "BUILD_VST3", "OFF");
        convertOnOffSettingWithDefault(jucerProject, "buildAU", "BUILD_AUDIOUNIT", "ON");
        convertOnOffSettingWithDefault(jucerProject, "buildAUv3", "BUILD_AUDIOUNIT_V3",
                                       "OFF");
        convertOnOffSettingWithDefault(jucerProject, "buildRTAS", "BUILD_RTAS", "OFF");
        convertOnOffSettingWithDefault(jucerProject, "buildAAX", "BUILD_AAX", "OFF");
        if (jucerVersionAsTuple >= Version{5, 0, 0})
        {
          convertOnOffSettingWithDefault(jucerProject, "buildStandalone",
                                         "BUILD_STANDALONE_PLUGIN", "OFF");
          convertOnOffSettingWithDefault(jucerProject, "enableIAA",
                                         "ENABLE_INTER_APP_AUDIO", "OFF");
        }
      }

      convertSettingWithDefault(jucerProject, "pluginName", "PLUGIN_NAME",
                                jucerProjectName);
      convertSettingWithDefault(jucerProject, "pluginDesc", "PLUGIN_DESCRIPTION",
                                jucerProjectName);

      convertSettingWithDefault(jucerProject, "pluginManufacturer", "PLUGIN_MANUFACTURER",
                                defaultCompanyName);
      convertSettingWithDefault(jucerProject, "pluginManufacturerCode",
                                "PLUGIN_MANUFACTURER_CODE", "Manu");

      const auto defaultPluginCode = [&jucerProject]() {
        const auto& projectId = jucerProject.getStringAttribute("id");
        const auto s = makeValidIdentifier(projectId + projectId) + "xxxx";
        return s.substring(0, 1).toUpperCase() + s.substring(1, 4).toLowerCase();
      }();
      convertSettingWithDefault(jucerProject, "pluginCode", "PLUGIN_CODE",
                                defaultPluginCode);

      convertSetting(jucerProject, "pluginChannelConfigs",
                     "PLUGIN_CHANNEL_CONFIGURATIONS", {});

      const auto pluginCharacteristics = juce::StringArray::fromTokens(
        jucerProject.getStringAttribute("pluginCharacteristicsValue"), ",", {});

      const auto isSynthAudioPlugin =
        jucerVersionAsTuple >= Version{5, 3, 1}
          ? pluginCharacteristics.contains("pluginIsSynth")
          : toBoolLikeVar(jucerProject.getStringAttribute("pluginIsSynth"));

      if (jucerVersionAsTuple < Version{5, 3, 1})
      {
        wLn(juce::String{"  PLUGIN_IS_A_SYNTH "} + (isSynthAudioPlugin ? "ON" : "OFF"));
        convertOnOffSettingWithDefault(jucerProject, "pluginWantsMidiIn",
                                       "PLUGIN_MIDI_INPUT", "OFF");
        convertOnOffSettingWithDefault(jucerProject, "pluginProducesMidiOut",
                                       "PLUGIN_MIDI_OUTPUT", "OFF");
        convertOnOffSettingWithDefault(jucerProject, "pluginIsMidiEffectPlugin",
                                       "MIDI_EFFECT_PLUGIN", "OFF");
        convertOnOffSettingWithDefault(jucerProject, "pluginEditorRequiresKeys",
                                       "KEY_FOCUS", "OFF");
      }

      if (jucerVersionAsTuple >= Version{5, 3, 1})
      {
        convertSettingWithDefault(jucerProject, "aaxIdentifier", "PLUGIN_AAX_IDENTIFIER",
                                  defaultBundleIdentifier);
      }
      convertSettingWithDefault(jucerProject, "pluginAUExportPrefix",
                                "PLUGIN_AU_EXPORT_PREFIX",
                                makeValidIdentifier(jucerProjectName) + "AU");
      if (jucerVersionAsTuple >= Version{5, 3, 1})
      {
        if (jucerProject.hasAttribute("pluginAUMainType"))
        {
          convertSetting(jucerProject, "pluginAUMainType", "PLUGIN_AU_MAIN_TYPE",
                         [](const juce::String& value) -> juce::String {
                           // clang-format off
                           if (value == "'aufx'") return "kAudioUnitType_Effect";
                           if (value == "'aufc'") return "kAudioUnitType_FormatConverter";
                           if (value == "'augn'") return "kAudioUnitType_Generator";
                           if (value == "'aumi'") return "kAudioUnitType_MIDIProcessor";
                           if (value == "'aumx'") return "kAudioUnitType_Mixer";
                           if (value == "'aumu'") return "kAudioUnitType_MusicDevice";
                           if (value == "'aumf'") return "kAudioUnitType_MusicEffect";
                           if (value == "'auol'") return "kAudioUnitType_OfflineEffect";
                           if (value == "'auou'") return "kAudioUnitType_Output";
                           if (value == "'aupn'") return "kAudioUnitType_Panner";
                           // clang-format on
                           return value;
                         });
        }
        else
        {
          convertSetting(jucerProject, "pluginAUMainType", "PLUGIN_AU_MAIN_TYPE",
                         [&pluginCharacteristics](const juce::String&) -> juce::String {
                           if (pluginCharacteristics.contains("pluginIsMidiEffectPlugin"))
                             return "kAudioUnitType_MIDIProcessor"; // 'aumi'

                           if (pluginCharacteristics.contains("pluginIsSynth"))
                             return "kAudioUnitType_MusicDevice"; // 'aumu'

                           if (pluginCharacteristics.contains("pluginWantsMidiIn"))
                             return "kAudioUnitType_MusicEffect"; // 'aumf'

                           return "kAudioUnitType_Effect"; // 'aufx'
                         });
        }
      }
      else
      {
        convertSetting(jucerProject, "pluginAUMainType", "PLUGIN_AU_MAIN_TYPE", {});
      }
      convertOnOffSettingIfDefined(jucerProject, "pluginAUIsSandboxSafe",
                                   "PLUGIN_AU_IS_SANDBOX_SAFE", {});

      if (jucerProject.hasAttribute("pluginVSTNumMidiInputs")
          || (jucerVersionAsTuple >= Version{5, 4, 2}
              && pluginCharacteristics.contains("pluginWantsMidiIn")))
      {
        convertSettingWithDefault(jucerProject, "pluginVSTNumMidiInputs",
                                  "PLUGIN_VST_NUM_MIDI_INPUTS", "16");
      }
      if (jucerProject.hasAttribute("pluginVSTNumMidiOutputs")
          || (jucerVersionAsTuple >= Version{5, 4, 2}
              && pluginCharacteristics.contains("pluginProducesMidiOut")))
      {
        convertSettingWithDefault(jucerProject, "pluginVSTNumMidiOutputs",
                                  "PLUGIN_VST_NUM_MIDI_OUTPUTS", "16");
      }

      if (!vstIsLegacy)
      {
        convertSettingWithDefault(
          jucerProject, "pluginVSTCategory",
          jucerVersionAsTuple > Version{5, 3, 0} ? "PLUGIN_VST_CATEGORY" : "VST_CATEGORY",
          jucerVersionAsTuple >= Version{5, 3, 1}
            ? (isSynthAudioPlugin ? "kPlugCategSynth" : "kPlugCategEffect")
            : "");
      }

      if (jucerProject.hasAttribute("pluginVST3Category")
          || jucerVersionAsTuple >= Version{5, 3, 1})
      {
        if (jucerProject.hasAttribute("pluginVST3Category"))
        {
          convertSettingAsList(
            jucerProject, "pluginVST3Category", "PLUGIN_VST3_CATEGORY",
            [](const juce::String& value) {
              auto vst3Categories = juce::StringArray::fromTokens(value, ",", {});
              if (vst3Categories.contains("Instrument"))
              {
                vst3Categories.move(vst3Categories.indexOf("Instrument"), 0);
              }
              if (vst3Categories.contains("Fx"))
              {
                vst3Categories.move(vst3Categories.indexOf("Fx"), 0);
              }
              return vst3Categories;
            });
        }
        else
        {
          convertSettingAsList(jucerProject, "pluginVST3Category", "PLUGIN_VST3_CATEGORY",
                               [isSynthAudioPlugin](const juce::String&) {
                                 return isSynthAudioPlugin
                                          ? juce::StringArray{"Instrument", "Synth"}
                                          : juce::StringArray{"Fx"};
                               });
        }
      }

      if (jucerVersionAsTuple >= Version{5, 3, 1})
      {
        if (jucerProject.hasAttribute("pluginRTASCategory"))
        {
          convertSettingAsList(jucerProject, "pluginRTASCategory", "PLUGIN_RTAS_CATEGORY",
                               [](const juce::String& value) {
                                 return convertIdsToStrings(
                                   juce::StringArray::fromTokens(value, ",", {}),
                                   {{"0", "ePlugInCategory_None"},
                                    {"1", "ePlugInCategory_EQ"},
                                    {"2", "ePlugInCategory_Dynamics"},
                                    {"4", "ePlugInCategory_PitchShift"},
                                    {"8", "ePlugInCategory_Reverb"},
                                    {"16", "ePlugInCategory_Delay"},
                                    {"32", "ePlugInCategory_Modulation"},
                                    {"64", "ePlugInCategory_Harmonic"},
                                    {"128", "ePlugInCategory_NoiseReduction"},
                                    {"256", "ePlugInCategory_Dither"},
                                    {"512", "ePlugInCategory_SoundField"},
                                    {"1024", "ePlugInCategory_HWGenerators"},
                                    {"2048", "ePlugInCategory_SWGenerators"},
                                    {"4096", "ePlugInCategory_WrappedPlugin"},
                                    {"8192", "ePlugInCategory_Effect"}});
                               });
        }
        else
        {
          convertSettingAsList(jucerProject, "pluginRTASCategory", "PLUGIN_RTAS_CATEGORY",
                               [isSynthAudioPlugin](const juce::String&) {
                                 return juce::StringArray{
                                   isSynthAudioPlugin ? "ePlugInCategory_SWGenerators"
                                                      : "ePlugInCategory_None"};
                               });
        }

        if (jucerProject.hasAttribute("pluginAAXCategory"))
        {
          convertSettingAsList(jucerProject, "pluginAAXCategory", "PLUGIN_AAX_CATEGORY",
                               [](const juce::String& value) {
                                 return convertIdsToStrings(
                                   juce::StringArray::fromTokens(value, ",", {}),
                                   {{"0", "AAX_ePlugInCategory_None"},
                                    {"1", "AAX_ePlugInCategory_EQ"},
                                    {"2", "AAX_ePlugInCategory_Dynamics"},
                                    {"4", "AAX_ePlugInCategory_PitchShift"},
                                    {"8", "AAX_ePlugInCategory_Reverb"},
                                    {"16", "AAX_ePlugInCategory_Delay"},
                                    {"32", "AAX_ePlugInCategory_Modulation"},
                                    {"64", "AAX_ePlugInCategory_Harmonic"},
                                    {"128", "AAX_ePlugInCategory_NoiseReduction"},
                                    {"256", "AAX_ePlugInCategory_Dither"},
                                    {"512", "AAX_ePlugInCategory_SoundField"},
                                    {"1024", "AAX_ePlugInCategory_HWGenerators"},
                                    {"2048", "AAX_ePlugInCategory_SWGenerators"},
                                    {"4096", "AAX_ePlugInCategory_WrappedPlugin"},
                                    {"8192", "AAX_EPlugInCategory_Effect"}});
                               });
        }
        else
        {
          convertSettingAsList(jucerProject, "pluginAAXCategory", "PLUGIN_AAX_CATEGORY",
                               [isSynthAudioPlugin](const juce::String&) {
                                 return juce::StringArray{
                                   isSynthAudioPlugin ? "AAX_ePlugInCategory_SWGenerators"
                                                      : "AAX_ePlugInCategory_None"};
                               });
        }
      }
      else
      {
        convertSetting(jucerProject, "pluginRTASCategory", "PLUGIN_RTAS_CATEGORY", {});
        convertSetting(jucerProject, "pluginAAXCategory", "PLUGIN_AAX_CATEGORY", {});
      }

      if (vstIsLegacy)
      {
        convertSettingWithDefault(
          jucerProject, "pluginVSTCategory", "PLUGIN_VST_LEGACY_CATEGORY",
          isSynthAudioPlugin ? "kPlugCategSynth" : "kPlugCategEffect");
      }

      if (jucerVersionAsTuple < Version{5, 3, 1})
      {
        convertSetting(jucerProject, "aaxIdentifier", "PLUGIN_AAX_IDENTIFIER", {});
      }

      wLn(")");
      wLn();
    }
  }

  // jucer_project_files()
  {
    const auto projectHasCompilerFlagSchemes =
      jucerProject.getStringAttribute("compilerFlagSchemes").isNotEmpty();

    struct File
    {
      bool compile;
      bool xcodeResource;
      bool binaryResource;
      juce::String path;
      juce::String compilerFlagScheme;
    };

    const auto writeFiles = [&projectHasCompilerFlagSchemes,
                             &wLn](const juce::String& fullGroupName,
                                   const std::vector<File>& files) {
      if (!files.empty())
      {
        const auto longestPathLength =
          std::max_element(files.begin(), files.end(),
                           [](const File& lhs, const File& rhs) {
                             return lhs.path.length() < rhs.path.length();
                           })
            ->path.length();
        const auto nineSpaces = "         ";

        wLn("jucer_project_files(\"", fullGroupName, "\"");
        if (projectHasCompilerFlagSchemes)
        {
          wLn("# Compile   Xcode     Binary    File",
              juce::String::repeatedString(" ", longestPathLength) + "Compiler Flag");
          wLn("#           Resource  Resource",
              juce::String::repeatedString(" ", longestPathLength + 6) + "Scheme");
        }
        else
        {
          wLn("# Compile   Xcode     Binary    File");
          wLn("#           Resource  Resource");
        }

        for (const auto& file : files)
        {
          const auto compilerFlagScheme =
            file.compilerFlagScheme.isEmpty()
              ? ""
              : juce::String::repeatedString(" ",
                                             longestPathLength - file.path.length() + 2)
                  + "\"" + file.compilerFlagScheme + "\"";
          wLn("  ", (file.compile ? "x" : "."), nineSpaces,
              (file.xcodeResource ? "x" : "."), nineSpaces,
              (file.binaryResource ? "x" : "."), nineSpaces, "\"", file.path, "\"",
              compilerFlagScheme);
        }

        wLn(")");
        wLn();
      }
    };

    juce::StringArray groupNames;

    std::function<void(const juce::XmlElement&)> processGroup =
      [&groupNames, &processGroup, &writeFiles](const juce::XmlElement& group) {
        groupNames.add(group.getStringAttribute("name"));

        const auto fullGroupName = groupNames.joinIntoString("/");

        std::vector<File> files;

        for (auto pFileOrGroup = group.getFirstChildElement(); pFileOrGroup != nullptr;
             pFileOrGroup = pFileOrGroup->getNextElement())
        {
          if (pFileOrGroup->isTextElement())
          {
            continue;
          }

          if (pFileOrGroup->hasTagName("FILE"))
          {
            const auto& file = *pFileOrGroup;

            files.push_back({file.getStringAttribute("compile").getIntValue() == 1,
                             file.getStringAttribute("xcodeResource").getIntValue() == 1,
                             file.getStringAttribute("resource").getIntValue() == 1,
                             file.getStringAttribute("file"),
                             file.getStringAttribute("compilerFlagScheme")});
          }
          else
          {
            writeFiles(fullGroupName, files);
            files.clear();

            processGroup(*pFileOrGroup);
          }
        }

        writeFiles(fullGroupName, files);

        groupNames.strings.removeLast();
      };

    processGroup(safeGetChildByName(jucerProject, "MAINGROUP"));
  }

  // jucer_project_module()
  {
    const auto& modulePaths = [&jucerProject]() -> const juce::XmlElement& {
      const auto& exportFormats = safeGetChildByName(jucerProject, "EXPORTFORMATS");
      for (auto pExporter = exportFormats.getFirstChildElement(); pExporter != nullptr;
           pExporter = pExporter->getNextElement())
      {
        if (!pExporter->isTextElement())
        {
          return safeGetChildByName(*pExporter, "MODULEPATHS");
        }
      }
      return getFallbackXmlElement();
    }();

    const auto juceModules = getChildFileFromWorkingDirectory(juceModulesGlobalPath);
    const auto userModules = getChildFileFromWorkingDirectory(userModulesGlobalPath);

    const auto& modules = safeGetChildByName(jucerProject, "MODULES");
    for (auto pModule = modules.getFirstChildElement(); pModule != nullptr;
         pModule = pModule->getNextElement())
    {
      if (pModule->isTextElement())
      {
        continue;
      }

      const auto& module = *pModule;
      const auto& moduleName = module.getStringAttribute("id");

      const auto useGlobalPath =
        toBoolLikeVar(module.getStringAttribute("useGlobalPath"));
      const auto isJuceModule = moduleName.startsWith("juce_");

      const auto modulePath = [&modulePaths, &moduleName]() -> juce::String {
        if (const auto pModulePath = modulePaths.getChildByAttribute("id", moduleName))
        {
          return pModulePath->getStringAttribute("path");
        }
        return {};
      }();

      wLn("jucer_project_module(");
      wLn("  ", moduleName);
      wLn("  PATH \"",
          useGlobalPath ? (isJuceModule ? "${JUCE_MODULES_GLOBAL_PATH}"
                                        : "${USER_MODULES_GLOBAL_PATH}")
                        : modulePath.replace("\\", "/"),
          "\"");

      const auto moduleHeader =
        (useGlobalPath ? (isJuceModule ? juceModules : userModules)
                       : jucerFile.getParentDirectory().getChildFile(modulePath))
          .getChildFile(moduleName)
          .getChildFile(moduleName + ".h");
      if (!moduleHeader.existsAsFile())
      {
        std::cerr << "warning: Couldn't a find module header for " << moduleName
                  << " module at \"" << moduleHeader.getFullPathName() << "\"."
                  << std::endl;
      }
      juce::StringArray moduleHeaderLines;
      moduleHeader.readLines(moduleHeaderLines);

      const auto& modulesOptions = safeGetChildByName(jucerProject, "JUCEOPTIONS");

      for (const auto& line : moduleHeaderLines)
      {
        if (line.startsWith("/** Config: "))
        {
          const auto moduleOption = line.substring(12);
          const auto& optionValue = modulesOptions.getStringAttribute(moduleOption);

          if (optionValue == "1" || optionValue == "enabled")
          {
            wLn("  ", moduleOption, " ON");
          }
          else if (optionValue == "0" || optionValue == "disabled")
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

    if (!appConfigLines.isEmpty())
    {
      juce::StringArray userCodeSectionLines;

      for (auto i = 0; i < appConfigLines.size(); ++i)
      {
        if (appConfigLines[i].contains("[BEGIN_USER_CODE_SECTION]"))
        {
          for (auto j = i + 1; j < appConfigLines.size()
                               && !appConfigLines[j].contains("[END_USER_CODE_SECTION]");
               ++j)
          {
            userCodeSectionLines.add(appConfigLines[j]);
          }

          break;
        }
      }

      const auto kDefaultProjucerUserCodeSectionCommentLines = juce::StringArray{
        "",
        "// (You can add your own code in this section, and the Projucer will not "
        "overwrite it)",
        ""};

      if (userCodeSectionLines != kDefaultProjucerUserCodeSectionCommentLines)
      {
        wLn("jucer_appconfig_header(");
        wLn("  USER_CODE_SECTION");
        wLn("\"", escape("\\\"", userCodeSectionLines.joinIntoString("\n")), "\"");
        wLn(")");
        wLn();
      }
    }
  }

  // jucer_export_target() and jucer_export_target_configuration()
  {
    const auto supportedExporters = juce::StringArray{
      "XCODE_MAC",  "XCODE_IPHONE",       "VS2019",          "VS2017", "VS2015", "VS2013",
      "LINUX_MAKE", "CODEBLOCKS_WINDOWS", "CODEBLOCKS_LINUX"};
    const auto exporterNames = std::map<juce::String, const char*>{
      {"XCODE_MAC", "Xcode (MacOSX)"},
      {"XCODE_IPHONE", "Xcode (iOS)"},
      {"VS2019", "Visual Studio 2019"},
      {"VS2017", "Visual Studio 2017"},
      {"VS2015", "Visual Studio 2015"},
      {"VS2013", "Visual Studio 2013"},
      {"LINUX_MAKE", "Linux Makefile"},
      {"CODEBLOCKS_WINDOWS", "Code::Blocks (Windows)"},
      {"CODEBLOCKS_LINUX", "Code::Blocks (Linux)"},
    };

    juce::StringPairArray configurationNamesMapping;

    const auto& exportFormats = safeGetChildByName(jucerProject, "EXPORTFORMATS");
    for (auto pExporter = exportFormats.getFirstChildElement(); pExporter != nullptr;
         pExporter = pExporter->getNextElement())
    {
      if (pExporter->isTextElement())
      {
        continue;
      }

      const auto& exporter = *pExporter;
      const auto& exporterType = exporter.getTagName();

      if (!supportedExporters.contains(exporterType))
      {
        continue;
      }

      const auto exporterName = exporterNames.at(exporterType);
      const auto& configurations = safeGetChildByName(exporter, "CONFIGURATIONS");

      wLn("jucer_export_target(");
      wLn("  \"", exporterName, "\"");

      const auto isXcodeExporter =
        exporterType == "XCODE_MAC" || exporterType == "XCODE_IPHONE";

      if (isXcodeExporter
          && (exporter.hasAttribute("prebuildCommand")
              || exporter.hasAttribute("postbuildCommand")))
      {
        wLn("  TARGET_PROJECT_FOLDER \"", exporter.getStringAttribute("targetFolder"),
            "\" # only used by PREBUILD_SHELL_SCRIPT and POSTBUILD_SHELL_SCRIPT");
      }

      const auto isVSExporter = exporterType == "VS2019" || exporterType == "VS2017"
                                || exporterType == "VS2015" || exporterType == "VS2013";

      if (isVSExporter)
      {
        const auto needsTargetFolder = [&configurations]() {
          for (auto pConfiguration = configurations.getFirstChildElement();
               pConfiguration != nullptr;
               pConfiguration = pConfiguration->getNextElement())
          {
            if (pConfiguration->isTextElement())
            {
              continue;
            }

            if (pConfiguration->hasAttribute("prebuildCommand")
                || pConfiguration->hasAttribute("postbuildCommand"))
            {
              return true;
            }
          }
          return false;
        }();

        if (needsTargetFolder)
        {
          wLn("  TARGET_PROJECT_FOLDER \"", exporter.getStringAttribute("targetFolder"),
              "\" # only used by PREBUILD_COMMAND and POSTBUILD_COMMAND");
        }
      }

      const auto isAudioPlugin = projectType == "audioplug";
      const auto pluginFormats = juce::StringArray::fromTokens(
        jucerProject.getStringAttribute("pluginFormats"), ",", {});
      const auto hasJuceAudioProcessorsModule =
        safeGetChildByName(jucerProject, "MODULES")
          .getChildByAttribute("id", "juce_audio_processors")
        != nullptr;

      const auto hasVst2Interface = jucerVersionAsTuple > Version{4, 2, 3};
      const auto isVstAudioPlugin =
        isAudioPlugin
        && (pluginFormats.contains("buildVST")
            || toBoolLikeVar(jucerProject.getStringAttribute("buildVST")));
      const auto& pluginHostVstOption = safeGetChildByName(jucerProject, "JUCEOPTIONS")
                                          .getStringAttribute("JUCE_PLUGINHOST_VST");
      const auto isVstPluginHost =
        hasJuceAudioProcessorsModule
        && (pluginHostVstOption == "enabled" || pluginHostVstOption == "1");

      if (!hasVst2Interface && (isVstAudioPlugin || isVstPluginHost))
      {
        convertSetting(exporter, "vstFolder", "VST_SDK_FOLDER", {});
      }

      const auto vstIsLegacy = jucerVersionAsTuple > Version{5, 3, 2};

      if (vstIsLegacy && (isVstAudioPlugin || isVstPluginHost))
      {
        convertSetting(exporter, "vstLegacyFolder", "VST_LEGACY_SDK_FOLDER", {});
      }

      const auto supportsVst3 = exporterType == "XCODE_MAC" || isVSExporter;
      const auto isVst3AudioPlugin =
        isAudioPlugin
        && (pluginFormats.contains("buildVST3")
            || toBoolLikeVar(jucerProject.getStringAttribute("buildVST3")));
      const auto& pluginHostVst3Option = safeGetChildByName(jucerProject, "JUCEOPTIONS")
                                           .getStringAttribute("JUCE_PLUGINHOST_VST3");
      const auto isVst3PluginHost =
        hasJuceAudioProcessorsModule
        && (pluginHostVst3Option == "enabled" || pluginHostVst3Option == "1");

      if (supportsVst3 && (isVst3AudioPlugin || isVst3PluginHost))
      {
        convertSetting(exporter, "vst3Folder", "VST3_SDK_FOLDER", {});
      }

      const auto supportsAaxRtas = exporterType == "XCODE_MAC" || isVSExporter;

      if (supportsAaxRtas && isAudioPlugin)
      {
        if (pluginFormats.contains("buildAAX")
            || toBoolLikeVar(jucerProject.getStringAttribute("buildAAX")))
        {
          convertSetting(exporter, "aaxFolder", "AAX_SDK_FOLDER", {});
        }

        if (pluginFormats.contains("buildRTAS")
            || toBoolLikeVar(jucerProject.getStringAttribute("buildRTAS")))
        {
          convertSetting(exporter, "rtasFolder", "RTAS_SDK_FOLDER", {});
        }
      }

      convertSettingAsListIfDefined(exporter, "extraDefs",
                                    "EXTRA_PREPROCESSOR_DEFINITIONS",
                                    parsePreprocessorDefinitions);
      convertSettingAsListIfDefined(exporter, "extraCompilerFlags",
                                    "EXTRA_COMPILER_FLAGS",
                                    [](const juce::String& value) {
                                      return juce::StringArray::fromTokens(value, false);
                                    });

      const auto compilerFlagSchemesArray = juce::StringArray::fromTokens(
        jucerProject.getStringAttribute("compilerFlagSchemes"), ",", {});
      // Use a juce::HashMap like Projucer does, in order to get the same ordering.
      juce::HashMap<juce::String, std::tuple<>> compilerFlagSchemesMap;
      for (const auto& scheme : compilerFlagSchemesArray)
      {
        compilerFlagSchemesMap.set(scheme, {});
      }
      for (juce::HashMap<juce::String, std::tuple<>>::Iterator i(compilerFlagSchemesMap);
           i.next();)
      {
        convertSettingIfDefined(exporter, i.getKey(), "COMPILER_FLAGS_FOR_" + i.getKey(),
                                {});
      }

      convertSettingAsListIfDefined(exporter, "extraLinkerFlags", "EXTRA_LINKER_FLAGS",
                                    [](const juce::String& value) {
                                      return juce::StringArray::fromTokens(value, false);
                                    });
      convertSettingAsListIfDefined(exporter, "externalLibraries",
                                    "EXTERNAL_LIBRARIES_TO_LINK", {});

      convertOnOffSettingIfDefined(exporter, "enableGNUExtensions",
                                   "GNU_COMPILER_EXTENSIONS", {});

      const auto convertIcon =
        [&jucerProject](const juce::String& fileId) -> juce::String {
        if (fileId.isNotEmpty())
        {
          if (const auto pFile = getChildByAttributeRecursively(
                safeGetChildByName(jucerProject, "MAINGROUP"), "id", fileId))
          {
            return pFile->getStringAttribute("file");
          }
        }

        return "<None>";
      };

      convertSettingIfDefined(exporter, "smallIcon", "ICON_SMALL", convertIcon);
      convertSettingIfDefined(exporter, "bigIcon", "ICON_LARGE", convertIcon);

      if (exporterType == "XCODE_IPHONE")
      {
        convertSettingIfDefined(exporter, "customXcassetsFolder",
                                "CUSTOM_XCASSETS_FOLDER", {});
        convertSettingIfDefined(exporter, "customLaunchStoryboard",
                                "CUSTOM_LAUNCH_STORYBOARD", {});
      }

      if (isXcodeExporter)
      {
        convertSettingAsListIfDefined(
          exporter, "customXcodeResourceFolders", "CUSTOM_XCODE_RESOURCE_FOLDERS",
          [](const juce::String& value) {
            auto folders = juce::StringArray::fromLines(value);
            folders.trim();
            return folders;
          });

        if (isAudioPlugin)
        {
          convertOnOffSettingIfDefined(exporter, "duplicateAppExResourcesFolder",
                                       "ADD_DUPLICATE_RESOURCES_FOLDER_TO_APP_EXTENSION",
                                       {});
        }
      }

      if (exporterType == "XCODE_IPHONE")
      {
        convertSettingIfDefined(exporter, "iosDeviceFamily", "DEVICE_FAMILY",
                                [](const juce::String& value) -> juce::String {
                                  if (value == "1")
                                    return "iPhone";

                                  if (value == "2")
                                    return "iPad";

                                  if (value == "1,2")
                                    return "Universal";

                                  return value;
                                });

        const auto screenOrientationFn = [](const juce::String& value) -> juce::String {
          if (value == "portraitlandscape")
            return "Portrait and Landscape";

          if (value == "portrait")
            return "Portrait";

          if (value == "landscape")
            return "Landscape";

          return value;
        };
        convertSettingIfDefined(exporter, "iosScreenOrientation",
                                "IPHONE_SCREEN_ORIENTATION", screenOrientationFn);
        convertSettingIfDefined(exporter, "iPadScreenOrientation",
                                "IPAD_SCREEN_ORIENTATION", screenOrientationFn);

        convertOnOffSettingIfDefined(exporter, "UIFileSharingEnabled",
                                     "FILE_SHARING_ENABLED", {});
        convertOnOffSettingIfDefined(exporter, "UISupportsDocumentBrowser",
                                     "SUPPORT_DOCUMENT_BROWSER", {});
        convertOnOffSettingIfDefined(exporter, "UIStatusBarHidden", "STATUS_BAR_HIDDEN",
                                     {});
      }

      if (exporterType == "XCODE_MAC")
      {
        if (projectType == "guiapp")
        {
          convertSettingAsListIfDefined(
            exporter, "documentExtensions", "DOCUMENT_FILE_EXTENSIONS",
            [](const juce::String& value) {
              return juce::StringArray::fromTokens(value, ",", {});
            });
        }

        convertOnOffSettingIfDefined(exporter, "appSandbox", "USE_APP_SANDBOX", {});
        convertOnOffSettingIfDefined(exporter, "appSandboxInheritance",
                                     "APP_SANDBOX_INHERITANCE", {});
        convertSettingAsListIfDefined(
          exporter, "appSandboxOptions", "APP_SANDBOX_OPTIONS",
          [](const juce::String& value) {
            return convertIdsToStrings(
              juce::StringArray::fromTokens(value, ",", {}),
              {{"com.apple.security.network.server",
                "Network: Incoming Connections (Server)"},
               {"com.apple.security.network.client",
                "Network: Outgoing Connections (Client)"},
               {"com.apple.security.device.camera", "Hardware: Camera"},
               {"com.apple.security.device.microphone", "Hardware: Microphone"},
               {"com.apple.security.device.usb", "Hardware: USB"},
               {"com.apple.security.print", "Hardware: Printing"},
               {"com.apple.security.device.bluetooth", "Hardware: Bluetooth"},
               {"com.apple.security.personal-information.addressbook",
                "App Data: Contacts"},
               {"com.apple.security.personal-information.location", "App Data: Location"},
               {"com.apple.security.personal-information.calendars",
                "App Data: Calendar"},
               {"com.apple.security.files.user-selected.read-only",
                "File Access: User Selected File (Read Only)"},
               {"com.apple.security.files.user-selected.read-write",
                "File Access: User Selected File (Read/Write)"},
               {"com.apple.security.files.downloads.read-only",
                "File Access: Downloads Folder (Read Only)"},
               {"com.apple.security.files.downloads.read-write",
                "File Access: Downloads Folder (Read/Write)"},
               {"com.apple.security.files.pictures.read-only",
                "File Access: Pictures Folder (Read Only)"},
               {"com.apple.security.files.pictures.read-write",
                "File Access: Pictures Folder (Read/Write)"},
               {"com.apple.security.assets.music.read-only",
                "File Access: Music Folder (Read Only)"},
               {"com.apple.security.assets.music.read-write",
                "File Access: Music Folder (Read/Write)"},
               {"com.apple.security.assets.movies.read-only",
                "File Access: Movies Folder (Read Only)"},
               {"com.apple.security.assets.movies.read-write",
                "File Access: Movies Folder (Read/Write)"},
               {"com.apple.security.temporary-exception.audio-unit-host",
                "Temporary Exception: Audio Unit Hosting"},
               {"com.apple.security.temporary-exception.mach-lookup.global-name",
                "Temporary Exception: Global Mach Service"},
               {"com.apple.security.temporary-exception.mach-register.global-name",
                "Temporary Exception: Global Mach Service Dynamic Registration"},
               {"com.apple.security.temporary-exception.files.home-relative-path.read-"
                "only",
                "Temporary Exception: Home Directory File Access (Read Only)"},
               {"com.apple.security.temporary-exception.files.home-relative-path.read-"
                "write",
                "Temporary Exception: Home Directory File Access (Read/Write)"},
               {"com.apple.security.temporary-exception.files.absolute-path.read-only",
                "Temporary Exception: Absolute Path File Access (Read Only)"},
               {"com.apple.security.temporary-exception.files.absolute-path.read-write",
                "Temporary Exception: Absolute Path File Access (Read/Write)"},
               {"com.apple.security.temporary-exception.iokit-user-client-class",
                "Temporary Exception: IOKit User Client Class"},
               {"com.apple.security.temporary-exception.shared-preference.read-only",
                "Temporary Exception: Shared Preference Domain (Read Only)"},
               {"com.apple.security.temporary-exception.shared-preference.read-write",
                "Temporary Exception: Shared Preference Domain (Read/Write)"}});
          });

        convertOnOffSettingIfDefined(exporter, "hardenedRuntime", "USE_HARDENED_RUNTIME",
                                     {});
        if (jucerVersionAsTuple >= Version{5, 4, 4})
        {
          convertSettingAsListIfDefined(
            exporter, "hardenedRuntimeOptions", "HARDENED_RUNTIME_OPTIONS",
            [](const juce::String& value) {
              return convertIdsToStrings(
                juce::StringArray::fromTokens(value, ",", {}),
                {{"com.apple.security.cs.allow-jit",
                  "Runtime Exceptions: Allow Execution of JIT-compiled Code"},
                 {"com.apple.security.cs.allow-unsigned-executable-memory",
                  "Runtime Exceptions: Allow Unsigned Executable Memory"},
                 {"com.apple.security.cs.allow-dyld-environment-variables",
                  "Runtime Exceptions: Allow DYLD Environment Variables"},
                 {"com.apple.security.cs.disable-library-validation",
                  "Runtime Exceptions: Disable Library Validation"},
                 {"com.apple.security.cs.disable-executable-page-protection",
                  "Runtime Exceptions: Disable Executable Memory Protection"},
                 {"com.apple.security.cs.debugger", "Runtime Exceptions: Debugging Tool"},
                 {"com.apple.security.device.audio-input",
                  "Resource Access: Audio Input"},
                 {"com.apple.security.device.camera", "Resource Access: Camera"},
                 {"com.apple.security.personal-information.location",
                  "Resource Access: Location"},
                 {"com.apple.security.personal-information.addressbook",
                  "Resource Access: Address Book"},
                 {"com.apple.security.personal-information.calendars",
                  "Resource Access: Calendar"},
                 {"com.apple.security.personal-information.photos-library",
                  "Resource Access: Photos Library"},
                 {"com.apple.security.automation.apple-events",
                  "Resource Access: Apple Events"}});
            });
        }
        else
        {
          convertSettingAsListIfDefined(
            exporter, "hardenedRuntimeOptions", "HARDENED_RUNTIME_OPTIONS",
            [](const juce::String& value) {
              return convertIdsToStrings(
                juce::StringArray::fromTokens(value, ",", {}),
                {{"com.apple.security.cs.allow-jit",
                  "Allow Execution of JIT-compiled Code"},
                 {"com.apple.security.cs.allow-unsigned-executable-memory",
                  "Allow Unsigned Executable Memory"},
                 {"com.apple.security.cs.allow-dyld-environment-variables",
                  "Allow DYLD Environment Variables"},
                 {"com.apple.security.cs.disable-library-validation",
                  "Disable Library Validation"},
                 {"com.apple.security.cs.disable-executable-page-protection",
                  "Disable Executable Memory Protection"},
                 {"com.apple.security.cs.debugger", "Debugging Tool"},
                 {"com.apple.security.device.audio-input", "Audio Input"},
                 {"com.apple.security.device.camera", "Camera"},
                 {"com.apple.security.personal-information.location", "Location"},
                 {"com.apple.security.personal-information.addressbook", "Address Book"},
                 {"com.apple.security.personal-information.calendars", "Calendar"},
                 {"com.apple.security.personal-information.photos-library",
                  "Photos Library"},
                 {"com.apple.security.automation.apple-events", "Apple Events"}});
            });
        }
      }

      if (isXcodeExporter)
      {
        convertOnOffSettingIfDefined(exporter, "microphonePermissionNeeded",
                                     "MICROPHONE_ACCESS", {});
        convertSettingIfDefined(exporter, "microphonePermissionsText",
                                "MICROPHONE_ACCESS_TEXT", {});
        convertOnOffSettingIfDefined(exporter, "cameraPermissionNeeded", "CAMERA_ACCESS",
                                     {});
        convertSettingIfDefined(exporter, "cameraPermissionText", "CAMERA_ACCESS_TEXT",
                                {});
      }

      if (exporterType == "XCODE_IPHONE")
      {
        convertOnOffSettingIfDefined(exporter, "iosBluetoothPermissionNeeded",
                                     "BLUETOOTH_ACCESS", {});
        convertSettingIfDefined(exporter, "iosBluetoothPermissionText",
                                "BLUETOOTH_ACCESS_TEXT", {});
      }

      if (isXcodeExporter)
      {
        convertOnOffSettingIfDefined(exporter, "iosInAppPurchasesValue",
                                     "IN_APP_PURCHASES_CAPABILITY", {});
      }

      if (exporterType == "XCODE_IPHONE")
      {
        convertOnOffSettingIfDefined(exporter, "iosBackgroundAudio",
                                     "AUDIO_BACKGROUND_CAPABILITY", {});
        convertOnOffSettingIfDefined(exporter, "iosBackgroundBle",
                                     "BLUETOOTH_MIDI_BACKGROUND_CAPABILITY", {});
        convertOnOffSettingIfDefined(exporter, "iosAppGroups", "APP_GROUPS_CAPABILITY",
                                     {});
        convertOnOffSettingIfDefined(exporter, "iCloudPermissions", "ICLOUD_PERMISSIONS",
                                     {});
      }

      if (isXcodeExporter)
      {
        convertOnOffSettingIfDefined(exporter, "iosPushNotifications",
                                     "PUSH_NOTIFICATIONS_CAPABILITY", {});

        convertSettingIfDefined(exporter, "customPList", "CUSTOM_PLIST", {});
        convertOnOffSettingIfDefined(exporter, "PListPreprocess", "PLIST_PREPROCESS", {});
        convertOnOffSettingIfDefined(exporter, "pListPreprocess", "PLIST_PREPROCESS", {});
        const auto convertPrefixHeader = [&jucerFile,
                                          &exporter](const juce::String& value) {
          if (value.isEmpty())
            return juce::String{};

          const auto jucerFileDir = jucerFile.getParentDirectory();
          const auto targetProjectDir =
            jucerFileDir.getChildFile(exporter.getStringAttribute("targetFolder"));

          return targetProjectDir.getChildFile(value).getRelativePathFrom(jucerFileDir);
        };
        convertSettingIfDefined(exporter, "PListPrefixHeader", "PLIST_PREFIX_HEADER",
                                convertPrefixHeader);
        convertSettingIfDefined(exporter, "pListPrefixHeader", "PLIST_PREFIX_HEADER",
                                convertPrefixHeader);

        convertSettingAsListIfDefined(
          exporter, "extraFrameworks",
          jucerVersionAsTuple > Version{5, 3, 2} ? "EXTRA_SYSTEM_FRAMEWORKS"
                                                 : "EXTRA_FRAMEWORKS",
          [](const juce::String& value) {
            auto frameworks = juce::StringArray::fromTokens(value, ",;", "\"'");
            frameworks.trim();
            return frameworks;
          });
        convertSettingAsListIfDefined(exporter, "frameworkSearchPaths",
                                      "FRAMEWORK_SEARCH_PATHS", {});
        convertSettingAsListIfDefined(exporter, "extraCustomFrameworks",
                                      "EXTRA_CUSTOM_FRAMEWORKS", {});
        convertSettingAsListIfDefined(exporter, "embeddedFrameworks",
                                      "EMBEDDED_FRAMEWORKS", {});
        convertSettingAsListIfDefined(exporter, "xcodeSubprojects", "XCODE_SUBPROJECTS",
                                      {});
        convertSettingIfDefined(exporter, "prebuildCommand", "PREBUILD_SHELL_SCRIPT", {});
        convertSettingIfDefined(exporter, "postbuildCommand", "POSTBUILD_SHELL_SCRIPT",
                                {});
        convertSettingIfDefined(exporter, "bundleIdentifier",
                                "EXPORTER_BUNDLE_IDENTIFIER", {});
        convertSettingIfDefined(exporter, "iosDevelopmentTeamID", "DEVELOPMENT_TEAM_ID",
                                {});
      }

      if (exporterType == "XCODE_IPHONE")
      {
        convertSettingAsListIfDefined(
          exporter, "iosAppGroupsId", "APP_GROUP_ID", [](const juce::String& value) {
            auto groups = juce::StringArray::fromTokens(value, ";", {});
            groups.trim();
            return groups;
          });
      }

      if (isXcodeExporter)
      {
        convertOnOffSettingIfDefined(exporter, "keepCustomXcodeSchemes",
                                     "KEEP_CUSTOM_XCODE_SCHEMES", {});
        convertOnOffSettingIfDefined(exporter, "useHeaderMap", "USE_HEADERMAP", {});
      }

      if (isVSExporter)
      {
        convertSettingIfDefined(exporter, "msvcManifestFile", "MANIFEST_FILE", {});

        if (exporter.hasAttribute("toolset"))
        {
          const auto& toolset = exporter.getStringAttribute("toolset");
          if (toolset.isEmpty())
          {
            wLn("  # PLATFORM_TOOLSET \"(default)\"");
          }
          else
          {
            wLn("  # PLATFORM_TOOLSET \"", toolset, "\"");
          }
        }

        convertSettingIfDefined(
          exporter, "IPPLibrary", "USE_IPP_LIBRARY",
          [&jucerVersionAsTuple](const juce::String& value) -> juce::String {
            if (value.isEmpty())
              return "No";

            if (value == "true")
              return jucerVersionAsTuple >= Version{5, 2, 1} ? "Yes (Default Mode)"
                                                             : "Yes (Default Linking)";

            if (value == "Parallel_Static")
              return "Multi-Threaded Static Library";

            if (value == "Sequential")
              return "Single-Threaded Static Library";

            if (value == "Parallel_Dynamic")
              return "Multi-Threaded DLL";

            if (value == "Sequential_Dynamic")
              return "Single-Threaded DLL";

            return {};
          });

        convertSettingIfDefined(exporter, "windowsTargetPlatformVersion",
                                "WINDOWS_TARGET_PLATFORM", {});

        if (exporterType == "VS2017")
        {
          convertSettingIfDefined(exporter, "cppLanguageStandard", "CXX_STANDARD_TO_USE",
                                  [](const juce::String& value) -> juce::String {
                                    if (value.isEmpty())
                                      return "(default)";
                                    if (value == "stdcpp14")
                                      return "C++14";
                                    if (value == "stdcpplatest")
                                      return "Latest C++ Standard";
                                    return {};
                                  });
        }
      }

      if (exporterType == "LINUX_MAKE")
      {
        convertSettingIfDefined(exporter, "cppLanguageStandard", "CXX_STANDARD_TO_USE",
                                [](const juce::String& value) -> juce::String {
                                  if (value == "-std=c++03")
                                    return "C++03";

                                  if (value == "-std=c++11")
                                    return "C++11";

                                  if (value == "-std=c++14")
                                    return "C++14";

                                  return {};
                                });

        convertSettingAsListIfDefined(
          exporter, "linuxExtraPkgConfig", "PKGCONFIG_LIBRARIES",
          [](const juce::String& value) {
            return juce::StringArray::fromTokens(value, " ", "\"'");
          });
      }

      if (exporterType == "CODEBLOCKS_WINDOWS")
      {
        const auto windowsTargets = std::map<juce::String, const char*>{
          {"0x0400", "Windows NT 4.0"}, {"0x0500", "Windows 2000"},
          {"0x0501", "Windows XP"},     {"0x0502", "Windows Server 2003"},
          {"0x0600", "Windows Vista"},  {"0x0601", "Windows 7"},
          {"0x0602", "Windows 8"},      {"0x0603", "Windows 8.1"},
          {"0x0A00", "Windows 10"},
        };

        convertSettingIfDefined(
          exporter, "codeBlocksWindowsTarget", "TARGET_PLATFORM",
          [&windowsTargets](const juce::String& value) -> juce::String {
            auto search = windowsTargets.find(value);
            if (search != windowsTargets.end())
            {
              return search->second;
            }

            return {};
          });
      }

      writeUserNotes(wLn, exporter);

      wLn(")");
      wLn();

      for (auto pConfiguration = configurations.getFirstChildElement();
           pConfiguration != nullptr; pConfiguration = pConfiguration->getNextElement())
      {
        if (pConfiguration->isTextElement())
        {
          continue;
        }

        const auto& configuration = *pConfiguration;

        wLn("jucer_export_target_configuration(");
        wLn("  \"", exporterName, "\"");

        const auto originalName = configuration.getStringAttribute("name");
        if (std::regex_match(originalName.toStdString(), std::regex{"^[A-Za-z0-9_]+$"}))
        {
          wLn("  NAME \"", originalName, "\"");
        }
        else
        {
          if (!configurationNamesMapping.containsKey(originalName))
          {
            const auto validNameWithoutSuffix = makeValidConfigurationName(originalName);

            auto numberSuffix = 1;
            auto validName = validNameWithoutSuffix;
            while (configurationNamesMapping.getAllValues().contains(validName))
            {
              validName = validNameWithoutSuffix + "_" + juce::String{numberSuffix++};
            }

            configurationNamesMapping.set(originalName, validName);

            std::cerr << "warning: \"" << originalName
                      << "\" is not a valid CMake build configuration name. It has been "
                         "changed to \""
                      << validName << "\" in the generated CMakeLists.txt file."
                      << std::endl;
          }

          wLn("  NAME \"", configurationNamesMapping[originalName], "\" # originally \"",
              originalName, "\" in ", jucerFileName);
        }

        const auto isDebug = toBoolLikeVar(configuration.getStringAttribute("isDebug"));
        wLn("  DEBUG_MODE ", (isDebug ? "ON" : "OFF"));

        convertSettingIfDefined(configuration, "targetName", "BINARY_NAME", {});
        convertSettingIfDefined(configuration, "binaryPath", "BINARY_LOCATION", {});

        const auto isAbsolutePath = [](const juce::String& path) {
          return path.startsWithChar('/') || path.startsWithChar('~')
                 || path.startsWithChar('$')
                 || (juce::CharacterFunctions::isLetter(path[0]) && path[1] == ':');
        };

        const auto jucerFileDir = jucerFile.getParentDirectory();
        const auto targetProjectDir =
          jucerFileDir.getChildFile(exporter.getStringAttribute("targetFolder"));

        const auto convertSearchPaths =
          [&isAbsolutePath, &jucerFileDir,
           &targetProjectDir](const juce::String& searchPaths) -> juce::StringArray {
          if (searchPaths.isEmpty())
          {
            return {};
          }

          juce::StringArray absOrRelToJucerFileDirPaths;

          for (const auto& path : juce::StringArray::fromTokens(searchPaths, ";\r\n", {}))
          {
            if (path.isEmpty())
            {
              continue;
            }

            const auto unquotedPath = path.unquoted();

            if (isAbsolutePath(unquotedPath))
            {
              absOrRelToJucerFileDirPaths.add(unquotedPath);
            }
            else
            {
              absOrRelToJucerFileDirPaths.add(targetProjectDir.getChildFile(unquotedPath)
                                                .getRelativePathFrom(jucerFileDir));
            }
          }

          return absOrRelToJucerFileDirPaths;
        };

        convertSettingAsListIfDefined(configuration, "headerPath", "HEADER_SEARCH_PATHS",
                                      convertSearchPaths);
        convertSettingAsListIfDefined(configuration, "libraryPath",
                                      "EXTRA_LIBRARY_SEARCH_PATHS", convertSearchPaths);

        convertSettingAsListIfDefined(configuration, "defines",
                                      "PREPROCESSOR_DEFINITIONS",
                                      parsePreprocessorDefinitions);

        convertOnOffSettingIfDefined(configuration, "linkTimeOptimisation",
                                     "LINK_TIME_OPTIMISATION", {});

        if (!configuration.hasAttribute("linkTimeOptimisation") && isVSExporter
            && !isDebug && jucerVersionAsTuple >= Version{5, 2, 0})
        {
          convertOnOffSettingIfDefined(configuration, "wholeProgramOptimisation",
                                       "LINK_TIME_OPTIMISATION",
                                       [](const juce::String& value) {
                                         if (value.getIntValue() == 0)
                                           return "ON";

                                         return "OFF";
                                       });
        }

        if (isXcodeExporter)
        {
          convertSettingIfDefined(configuration, "recommendedWarnings",
                                  "ADD_RECOMMENDED_COMPILER_WARNING_FLAGS",
                                  [](const juce::String& value) -> juce::String {
                                    if (value == "LLVM")
                                      return "Enabled";

                                    if (value.isEmpty())
                                      return "Disabled";

                                    return {};
                                  });
        }
        else if (exporterType == "CODEBLOCKS_LINUX"
                 || exporterType == "CODEBLOCKS_WINDOWS" || exporterType == "LINUX_MAKE")
        {
          convertSettingIfDefined(configuration, "recommendedWarnings",
                                  "ADD_RECOMMENDED_COMPILER_WARNING_FLAGS",
                                  [](const juce::String& value) -> juce::String {
                                    if (value == "GCC")
                                      return "GCC";

                                    if (value == "GCC-7")
                                      return "GCC 7 and below";

                                    if (value == "LLVM")
                                      return "LLVM";

                                    if (value.isEmpty())
                                      return "Disabled";

                                    return {};
                                  });
        }

        convertSettingIfDefined(
          configuration, "optimisation", "OPTIMISATION",
          [&isVSExporter](const juce::String& value) -> juce::String {
            if (isVSExporter)
            {
              switch (value.getIntValue())
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

            switch (value.getIntValue())
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
          });

        if (isXcodeExporter)
        {
          convertOnOffSettingIfDefined(configuration, "enablePluginBinaryCopyStep",
                                       "ENABLE_PLUGIN_COPY_STEP", {});

          if (!vstIsLegacy)
          {
            if (configuration.hasAttribute("xcodeVstBinaryLocation"))
            {
              convertSetting(configuration, "xcodeVstBinaryLocation",
                             "VST_BINARY_LOCATION", {});
            }
            else
            {
              convertSettingIfDefined(configuration, "vstBinaryLocation",
                                      "VST_BINARY_LOCATION", {});
            }
          }

          const auto binaryLocationTuples = {
            std::make_tuple("xcodeVst3BinaryLocation", "vst3BinaryLocation",
                            "VST3_BINARY_LOCATION"),
            std::make_tuple("xcodeAudioUnitBinaryLocation", "auBinaryLocation",
                            "AU_BINARY_LOCATION"),
            std::make_tuple("xcodeRtasBinaryLocation", "rtasBinaryLocation",
                            "RTAS_BINARY_LOCATION"),
            std::make_tuple("xcodeAaxBinaryLocation", "aaxBinaryLocation",
                            "AAX_BINARY_LOCATION"),
          };

          for (const auto& binaryLocationTuple : binaryLocationTuples)
          {
            const auto& oldProperty = std::get<0>(binaryLocationTuple);
            const auto& newProperty = std::get<1>(binaryLocationTuple);
            const auto& cmakeKeyword = std::get<2>(binaryLocationTuple);

            if (configuration.hasAttribute(oldProperty))
            {
              convertSetting(configuration, oldProperty, cmakeKeyword, {});
            }
            else
            {
              convertSettingIfDefined(configuration, newProperty, cmakeKeyword, {});
            }
          }

          convertSettingIfDefined(configuration, "unityPluginBinaryLocation",
                                  "UNITY_BINARY_LOCATION", {});
          if (vstIsLegacy)
          {
            convertSettingIfDefined(configuration, "vstBinaryLocation",
                                    "VST_LEGACY_BINARY_LOCATION", {});
          }
        }

        if (exporterType == "XCODE_IPHONE")
        {
          convertSettingIfDefined(configuration, "iosCompatibility",
                                  "IOS_DEPLOYMENT_TARGET", {});
        }

        if (exporterType == "XCODE_MAC")
        {
          const auto sdks = juce::StringArray{
            "10.5 SDK",  "10.6 SDK",  "10.7 SDK",  "10.8 SDK",  "10.9 SDK",
            "10.10 SDK", "10.11 SDK", "10.12 SDK", "10.13 SDK", "10.14 SDK",
          };

          convertSettingIfDefined(configuration, "osxSDK", "OSX_BASE_SDK_VERSION",
                                  [&sdks](const juce::String& value) -> juce::String {
                                    if (value == "default")
                                      return "Use Default";

                                    if (sdks.contains(value))
                                      return value;

                                    return {};
                                  });

          convertSettingIfDefined(configuration, "osxCompatibility",
                                  "OSX_DEPLOYMENT_TARGET",
                                  [&sdks](const juce::String& value) -> juce::String {
                                    if (value == "default")
                                      return "Use Default";

                                    if (sdks.contains(value))
                                      return value.substring(0, value.length() - 4);

                                    return {};
                                  });

          convertSettingIfDefined(configuration, "osxArchitecture", "OSX_ARCHITECTURE",
                                  [](const juce::String& value) -> juce::String {
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
                                  });
        }

        if (isXcodeExporter)
        {
          convertSettingAsListIfDefined(
            configuration, "customXcodeFlags", "CUSTOM_XCODE_FLAGS",
            [](const juce::String& value) {
              auto customFlags = juce::StringArray::fromTokens(value, ",", "\"'");
              customFlags.removeEmptyStrings();

              for (auto& flag : customFlags)
              {
                flag = flag.upToFirstOccurrenceOf("=", false, false).trim() + " = "
                       + flag.fromFirstOccurrenceOf("=", false, false).trim();
              }

              return customFlags;
            });

          convertSettingAsListIfDefined(configuration, "plistPreprocessorDefinitions",
                                        "PLIST_PREPROCESSOR_DEFINITIONS",
                                        parsePreprocessorDefinitions);

          convertSettingIfDefined(configuration, "cppLanguageStandard",
                                  "CXX_LANGUAGE_STANDARD",
                                  [](const juce::String& value) -> juce::String {
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
                                  });

          convertSettingIfDefined(configuration, "cppLibType", "CXX_LIBRARY",
                                  [](const juce::String& value) -> juce::String {
                                    if (value.isEmpty())
                                      return "Use Default";

                                    if (value == "libc++")
                                      return "LLVM libc++";

                                    if (value == "libstdc++")
                                      return "GNU libstdc++";

                                    return {};
                                  });

          convertSettingIfDefined(configuration, "codeSigningIdentity",
                                  "CODE_SIGNING_IDENTITY", {});
          convertOnOffSettingIfDefined(configuration, "fastMath", "RELAX_IEEE_COMPLIANCE",
                                       {});
          convertOnOffSettingIfDefined(configuration, "stripLocalSymbols",
                                       "STRIP_LOCAL_SYMBOLS", {});
        }

        if (isVSExporter)
        {
          convertOnOffSettingIfDefined(configuration, "enablePluginBinaryCopyStep",
                                       "ENABLE_PLUGIN_COPY_STEP", {});

          if (!vstIsLegacy)
          {
            convertSettingIfDefined(configuration, "vstBinaryLocation",
                                    "VST_BINARY_LOCATION", {});
          }
          convertSettingIfDefined(configuration, "vst3BinaryLocation",
                                  "VST3_BINARY_LOCATION", {});
          convertSettingIfDefined(configuration, "rtasBinaryLocation",
                                  "RTAS_BINARY_LOCATION", {});
          convertSettingIfDefined(configuration, "aaxBinaryLocation",
                                  "AAX_BINARY_LOCATION", {});
          convertSettingIfDefined(configuration, "unityPluginBinaryLocation",
                                  "UNITY_BINARY_LOCATION", {});
          if (vstIsLegacy)
          {
            convertSettingIfDefined(configuration, "vstBinaryLocation",
                                    "VST_LEGACY_BINARY_LOCATION", {});
          }

          convertSettingIfDefined(configuration, "winWarningLevel", "WARNING_LEVEL",
                                  [](const juce::String& value) -> juce::String {
                                    switch (value.getIntValue())
                                    {
                                    case 2:
                                      return "Low";
                                    case 3:
                                      return "Medium";
                                    case 4:
                                      return "High";
                                    }

                                    return "High";
                                  });

          convertOnOffSettingIfDefined(configuration, "warningsAreErrors",
                                       "TREAT_WARNINGS_AS_ERRORS", {});

          convertSettingIfDefined(configuration, "useRuntimeLibDLL", "RUNTIME_LIBRARY",
                                  [](const juce::String& value) -> juce::String {
                                    if (value.isEmpty())
                                      return "(Default)";

                                    if (value == "0")
                                      return "Use static runtime";

                                    if (value == "1")
                                      return "Use DLL runtime";

                                    return {};
                                  });

          if (jucerVersionAsTuple < Version{5, 2, 0})
          {
            convertSettingIfDefined(configuration, "wholeProgramOptimisation",
                                    "WHOLE_PROGRAM_OPTIMISATION",
                                    [](const juce::String& value) -> juce::String {
                                      if (value.isEmpty())
                                        return "Enable when possible";

                                      if (value.getIntValue() > 0)
                                        return "Always disable";

                                      return {};
                                    });
          }

          convertOnOffSettingIfDefined(configuration, "multiProcessorCompilation",
                                       "MULTI_PROCESSOR_COMPILATION", {});
          convertOnOffSettingIfDefined(configuration, "enableIncrementalLinking",
                                       "INCREMENTAL_LINKING", {});

          if (!isDebug)
          {
            convertOnOffSettingIfDefined(configuration, "alwaysGenerateDebugSymbols",
                                         "FORCE_GENERATION_OF_DEBUG_SYMBOLS", {});
          }

          convertSettingIfDefined(configuration, "prebuildCommand", "PREBUILD_COMMAND",
                                  {});
          convertSettingIfDefined(configuration, "postbuildCommand", "POSTBUILD_COMMAND",
                                  {});
          convertOnOffSettingIfDefined(configuration, "generateManifest",
                                       "GENERATE_MANIFEST", {});

          convertSettingIfDefined(configuration, "characterSet", "CHARACTER_SET",
                                  [](const juce::String& value) -> juce::String {
                                    if (value.isEmpty())
                                      return "Default";

                                    return value;
                                  });

          if (configuration.hasAttribute("winArchitecture"))
          {
            const auto& winArchitecture =
              configuration.getStringAttribute("winArchitecture");
            if (winArchitecture.isEmpty())
            {
              wLn("  # ARCHITECTURE");
            }
            else
            {
              wLn("  # ARCHITECTURE \"", winArchitecture, "\"");
            }
          }

          convertSettingIfDefined(
            configuration, "debugInformationFormat", "DEBUG_INFORMATION_FORMAT",
            [](const juce::String& value) -> juce::String {
              if (value == "None")
                return "None";

              if (value == "OldStyle")
                return "C7 Compatible (/Z7)";

              if (value == "ProgramDatabase")
                return "Program Database (/Zi)";

              if (value == "EditAndContinue")
                return "Program Database for Edit And Continue (/ZI)";

              return {};
            });

          convertOnOffSettingIfDefined(configuration, "fastMath", "RELAX_IEEE_COMPLIANCE",
                                       {});
        }

        if (exporterType == "LINUX_MAKE")
        {
          convertSettingIfDefined(configuration, "linuxArchitecture", "ARCHITECTURE",
                                  [](const juce::String& value) -> juce::String {
                                    if (value.isEmpty())
                                      return "<None>";

                                    if (value == "-march=native")
                                      return "Native";

                                    if (value == "-m32")
                                      return "32-bit (-m32)";

                                    if (value == "-m64")
                                      return "64-bit (-m64)";

                                    if (value == "-march=armv6")
                                      return "ARM v6";

                                    if (value == "-march=armv7")
                                      return "ARM v7";

                                    return {};
                                  });
        }

        const auto codeBlocksArchitecture =
          [](const juce::String& value) -> juce::String {
          if (value == "-m32")
            return "32-bit (-m32)";

          if (value == "-m64" || value.isEmpty())
            return "64-bit (-m64)";

          if (value == "-march=armv6")
            return "ARM v6";

          if (value == "-march=armv7")
            return "ARM v7";

          return {};
        };

        if (exporterType == "CODEBLOCKS_WINDOWS")
        {
          if (configuration.hasAttribute("windowsCodeBlocksArchitecture")
              || jucerVersionAsTuple >= Version{5, 0, 0})
          {
            convertSetting(configuration, "windowsCodeBlocksArchitecture", "ARCHITECTURE",
                           codeBlocksArchitecture);
          }
        }

        if (exporterType == "CODEBLOCKS_LINUX")
        {
          if (configuration.hasAttribute("linuxCodeBlocksArchitecture")
              || jucerVersionAsTuple >= Version{5, 0, 0})
          {
            convertSetting(configuration, "linuxCodeBlocksArchitecture", "ARCHITECTURE",
                           codeBlocksArchitecture);
          }
        }

        writeUserNotes(wLn, configuration);

        wLn(")");
        wLn();
      }
    }
  }

  wLn("jucer_project_end()");

  const auto outputFile = getChildFileFromWorkingDirectory("CMakeLists.txt");

  std::unique_ptr<juce::FileInputStream> fileStream{outputFile.createInputStream()};
  if (fileStream)
  {
    juce::MemoryOutputStream fileContents;
    fileContents.writeFromInputStream(*fileStream, -1);

    if (fileContents.getDataSize() == outputStream.getDataSize()
        && std::memcmp(fileContents.getData(), outputStream.getData(),
                       fileContents.getDataSize())
             == 0)
    {
      std::cout << outputFile.getFullPathName() << " is already up-to-date." << std::endl;
      return 0;
    }
  }

  if (outputFile.replaceWithData(outputStream.getData(), outputStream.getDataSize()))
  {
    std::cout << outputFile.getFullPathName() << " has been successfully generated."
              << std::endl;
  }
  else
  {
    printError("Failed to write to " + outputFile.getFullPathName());
    return 1;
  }

  return 0;
}
