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
  #pragma clang diagnostic ignored "-Wcast-align"
  #pragma clang diagnostic ignored "-Wcast-qual"
  #pragma clang diagnostic ignored "-Wdocumentation"
  #pragma clang diagnostic ignored "-Wdocumentation-deprecated-sync"
  #pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
  #pragma clang diagnostic ignored "-Wexit-time-destructors"
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

#include "JuceHeader.h"

#include <argh/argh.h>

#if defined(__clang__)
  #pragma clang diagnostic pop
#elif defined(__GNUC__)
  #pragma GCC diagnostic pop
#elif defined(_MSC_VER)
  #pragma warning(pop)
#endif

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <locale>
#include <map>
#include <string>
#include <tuple>
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


struct LineWriter
{
  explicit LineWriter(juce::MemoryOutputStream& stream)
    : mStream(stream)
  {
  }

  LineWriter(const LineWriter&) = delete;
  LineWriter& operator=(const LineWriter&) = delete;

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


/** Sanitizes a string to match the regex ^[A-Za-z0-9_]*$. Sanitization follows these
 *  rules:
 *  1) Invalid characters are replaced with "_".
 *  2) At most one consecutive replacement "_" is kept (if the original name contained
 *     multiple "_" they remain unaltered).
 *  3) No replacement "_" is added at the end or at the beginning.
 */
juce::String sanitizeName(const juce::String& input)
{
  const juce::String validCharacters =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_";
  juce::String output;
  bool mustInsertReplacementChar = false;
  for (auto c = input.getCharPointer(); !c.isEmpty();)
  {
    // keep valid characters
    if (validCharacters.containsChar(*c))
    {
      if (mustInsertReplacementChar)
      {
        output += '_';
        mustInsertReplacementChar = false;
      }
      output += *c;
    }
    // replace invalid characters with "_"
    else
    {
      // don't add "_" at the beginning
      if (output.isNotEmpty())
        mustInsertReplacementChar = true;
    }
    c++;
  }
  return output;
}

/** Sanitizes a configuration name. Valid config names match the regex ^[A-Za-z0-9_]*$.
 *  Sanitization is done with sanitizeName(). An optional digit is added until the
 *  configuration name is unique. The sanitization is synchronized across
 *  the entire project.
 */
juce::String sanitizeConfigurationName(const juce::String& originalName)
{
  static std::map<juce::String, juce::String> sanitizedNameTable;
  static juce::StringArray sanitizedNameList;
  juce::String sanitizedName;

  // check if this name was sanitized before
  if (sanitizedNameTable.find(originalName) != sanitizedNameTable.end())
    return sanitizedNameTable[originalName];

  // need to sanitize
  sanitizedName = sanitizeName(originalName);

  // add digits until unique
  int number = 1;
  auto sanitizedNameWithoutNumber = sanitizedName;
  while (sanitizedNameList.contains(sanitizedName))
    sanitizedName = sanitizedNameWithoutNumber + "_" + juce::String(number++);

  // remember it for later
  sanitizedNameTable[originalName] = sanitizedName;
  sanitizedNameList.add(sanitizedName);

  if (originalName != sanitizedName)
  {
    std::cerr << "warning: Configuration '" << originalName << "' "
              << "contains invalid characters. The name was changed to '" << sanitizedName
              << "'" << std::endl;
  }

  return sanitizedName;
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


juce::String cmakeAbsolutePath(const juce::String& path)
{
  const auto file = juce::File::getCurrentWorkingDirectory().getChildFile(path);
  return (juce::File::isAbsolutePath(path)
            ? file.getFullPathName()
            : "${CMAKE_CURRENT_LIST_DIR}/"
                + file.getRelativePathFrom(juce::File::getCurrentWorkingDirectory()))
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


juce::StringArray parsePreprocessorDefinitions(const juce::String& input)
{
  juce::StringArray output;

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
      output.add(value.isEmpty() ? definition : definition + '=' + value);
    }
  }

  return output;
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
  if (valueTree.hasProperty("userNotes"))
  {
    wLn("  # NOTES");
    const auto userNotes = valueTree.getProperty("userNotes").toString();
    for (const auto& line : juce::StringArray::fromLines(userNotes))
    {
      wLn("  #   ", line);
    }
  }
}

} // namespace


int main(int argc, char* argv[])
{
  const std::vector<std::string> knownFlags{"h", "help", "relocatable"};
  const std::vector<std::string> knownParams{"juce-modules", "user-modules"};

  argh::parser argumentParser;
  for (const auto& param : knownParams)
  {
    argumentParser.add_param(param);
  }
  argumentParser.parse(argc, argv);

  const auto askingForHelp = argumentParser[{"-h", "--help"}];
  auto errorInArguments = false;

  for (const auto& flag : argumentParser.flags())
  {
    if (std::find(knownFlags.begin(), knownFlags.end(), flag) == knownFlags.end())
    {
      printError("unknown option \"" + flag + "\"");
      errorInArguments = true;
    }
  }

  for (const auto& paramAndValue : argumentParser.params())
  {
    const auto& param = std::get<0>(paramAndValue);
    if (std::find(knownParams.begin(), knownParams.end(), param) == knownParams.end())
    {
      printError("unknown option \"" + param + "\"");
      errorInArguments = true;
    }
  }

  if (!askingForHelp)
  {
    if (argumentParser.size() < 2)
    {
      printError("not enough positional arguments");
      errorInArguments = true;
    }
    else if (argumentParser.size() > 3)
    {
      printError("too many positional arguments");
      errorInArguments = true;
    }
  }

  if (askingForHelp || errorInArguments)
  {
    std::cerr
      << "usage: Jucer2Reprojucer [-h] [--juce-modules=<path>] [--user-modules=<path>]\n"
      << "                        [--relocatable]\n"
      << "                        <jucer_project_file> [<Reprojucer.cmake_file>]\n"
      << "\n"
      << "Converts a .jucer file into a CMakeLists.txt file that uses Reprojucer.cmake.\n"
      << "The CMakeLists.txt file is written in the current working directory.\n"
      << "\n"
      << "    <jucer_project_file>      path to the .jucer file to convert\n"
      << "    <Reprojucer.cmake_file>   path to Reprojucer.cmake\n"
      << "\n"
      << "    -h, --help                show this help message and exit\n"
      << "    --juce-modules <path>     global path to JUCE modules\n"
      << "    --user-modules <path>     global path to user modules\n"
      << "    --relocatable             makes the CMakeLists.txt file independent from\n"
      << "                              the location of the .jucer file, but requires\n"
      << "                              defining a variable when calling cmake\n"
      << std::endl;
    return askingForHelp ? 0 : 1;
  }

  const auto jucerFilePath = juce::String{argumentParser[1]};
  const auto jucerFile =
    juce::File::getCurrentWorkingDirectory().getChildFile(jucerFilePath);

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
  const auto jucerVersionTokens = juce::StringArray::fromTokens(jucerVersion, ".", {});
  if (jucerVersionTokens.size() != 3)
  {
    printError(jucerFilePath + " is not a valid Jucer project.");
    return 1;
  }

  using Version = std::tuple<int, int, int>;

  const auto jucerVersionAsTuple = [&jucerVersionTokens, &jucerFilePath]() {
    try
    {
      return Version{std::stoi(jucerVersionTokens[0].toStdString()),
                     std::stoi(jucerVersionTokens[1].toStdString()),
                     std::stoi(jucerVersionTokens[2].toStdString())};
    }
    catch (const std::invalid_argument&)
    {
      printError(jucerFilePath + " is not a valid Jucer project.");
      std::exit(1);
    }
  }();

  const auto reprojucerFilePath = juce::String{argumentParser[2]};
  const auto reprojucerFile =
    reprojucerFilePath.isNotEmpty()
      ? juce::File::getCurrentWorkingDirectory().getChildFile(reprojucerFilePath)
      : juce::File{};

  if (reprojucerFilePath.isNotEmpty()
      && (!reprojucerFile.existsAsFile()
          || !reprojucerFile.getFileName().endsWith("Reprojucer.cmake")))
  {
    printError(reprojucerFilePath + " is not a valid Reprojucer.cmake file.");
    return 1;
  }

  const auto juceModulesPath = juce::String{argumentParser("--juce-modules").str()};
  const auto juceModules =
    juce::File::getCurrentWorkingDirectory().getChildFile(juceModulesPath);
  if (!juceModules.isDirectory())
  {
    printError("No such directory (--juce-modules): " + juceModulesPath);
    return 1;
  }

  const auto userModulesPath = juce::String{argumentParser("--user-modules").str()};
  const auto userModules =
    juce::File::getCurrentWorkingDirectory().getChildFile(userModulesPath);
  if (!userModules.isDirectory())
  {
    printError("No such directory (--user-modules): " + userModulesPath);
    return 1;
  }

  juce::MemoryOutputStream outputStream;
  LineWriter wLn{outputStream};

  const auto convertSetting =
    [&wLn](const juce::ValueTree& valueTree, const juce::Identifier& property,
           const juce::String& cmakeKeyword,
           std::function<juce::String(const juce::var&)> converterFn) {
      if (!converterFn)
      {
        converterFn = [](const juce::var& v) { return v.toString(); };
      }

      const auto value = converterFn(valueTree.getProperty(property));

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
    [&convertSetting](const juce::ValueTree& valueTree, const juce::Identifier& property,
                      const juce::String& cmakeKeyword,
                      std::function<juce::String(const juce::var&)> converterFn) {
      if (valueTree.hasProperty(property))
      {
        convertSetting(valueTree, property, cmakeKeyword, std::move(converterFn));
      }
    };

  const auto convertSettingWithDefault =
    [&convertSetting](const juce::ValueTree& valueTree, const juce::Identifier& property,
                      const juce::String& cmakeKeyword,
                      const juce::String& defaultValue) {
      convertSetting(valueTree, property, cmakeKeyword,
                     [&defaultValue](const juce::var& v) -> juce::String {
                       return v.isVoid() ? defaultValue : v.toString();
                     });
    };

  const auto convertOnOffSetting =
    [&wLn](const juce::ValueTree& valueTree, const juce::Identifier& property,
           const juce::String& cmakeKeyword,
           std::function<juce::String(const juce::var&)> converterFn) {
      if (!converterFn)
      {
        converterFn = [](const juce::var& v) -> juce::String {
          return v.isVoid() ? "" : bool{v} ? "ON" : "OFF";
        };
      }

      const auto value = converterFn(valueTree.getProperty(property));

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
    [&convertOnOffSetting](const juce::ValueTree& valueTree,
                           const juce::Identifier& property,
                           const juce::String& cmakeKeyword,
                           std::function<juce::String(const juce::var&)> converterFn) {
      if (valueTree.hasProperty(property))
      {
        convertOnOffSetting(valueTree, property, cmakeKeyword, std::move(converterFn));
      }
    };

  const auto convertOnOffSettingWithDefault =
    [&convertOnOffSetting](const juce::ValueTree& valueTree,
                           const juce::Identifier& property,
                           const juce::String& cmakeKeyword, bool defaultValue) {
      convertOnOffSetting(valueTree, property, cmakeKeyword,
                          [defaultValue](const juce::var& v) -> juce::String {
                            return v.isVoid() ? (defaultValue ? "ON" : "OFF")
                                              : (bool{v} ? "ON" : "OFF");
                          });
    };

  const auto convertSettingAsList =
    [&wLn](const juce::ValueTree& valueTree, const juce::Identifier& property,
           const juce::String& cmakeKeyword,
           std::function<juce::StringArray(const juce::var&)> converterFn) {
      if (!converterFn)
      {
        converterFn = [](const juce::var& v) {
          return juce::StringArray::fromLines(v.toString());
        };
      }

      auto value = converterFn(valueTree.getProperty(property));
      value.removeEmptyStrings();

      if (value.isEmpty())
      {
        wLn("  # ", cmakeKeyword);
      }
      else
      {
        wLn("  ", cmakeKeyword);

        for (const auto& item : value)
        {
          wLn("    \"", escape("\\\";", item.trimCharactersAtEnd("\\")), "\"");
        }
      }
    };

  const auto convertSettingAsListIfDefined =
    [&convertSettingAsList](
      const juce::ValueTree& valueTree, const juce::Identifier& property,
      const juce::String& cmakeKeyword,
      std::function<juce::StringArray(const juce::var&)> converterFn) {
      if (valueTree.hasProperty(property))
      {
        convertSettingAsList(valueTree, property, cmakeKeyword, std::move(converterFn));
      }
    };

  const auto convertIdsToStrings =
    [](const juce::var& v,
       const std::vector<std::pair<juce::String, const char*>>& idsToStrings) {
      const auto ids = juce::StringArray::fromTokens(v.toString(), ",", {});
      juce::StringArray strings;
      for (const auto& idToString : idsToStrings)
      {
        if (ids.contains(idToString.first))
        {
          strings.add(idToString.second);
        }
      }
      return strings;
    };

  const auto jucerFileName = jucerFile.getFileName();
  const auto jucerProjectName = jucerProject.getProperty("name").toString();

  // Preamble
  {
    wLn("# This file was generated by Jucer2Reprojucer from \"", jucerFileName, "\"");
    wLn();
    wLn("cmake_minimum_required(VERSION 3.4)");
    wLn();
    wLn("project(\"", jucerProjectName, "\")");
    wLn();
    wLn();
  }

  // include(Reprojucer)
  {
    if (reprojucerFilePath.isNotEmpty())
    {
      wLn("list(APPEND CMAKE_MODULE_PATH \"${CMAKE_CURRENT_LIST_DIR}/",
          reprojucerFile.getParentDirectory()
            .getRelativePathFrom(juce::File::getCurrentWorkingDirectory())
            .replace("\\", "/"),
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

  auto escapedJucerFileName = sanitizeName(jucerFileName);
  const auto jucerFileCMakeVar = juce::String{escapedJucerFileName} + "_FILE";

  // get_filename_component() or set(*_FILE)
  {
    if (argumentParser["--relocatable"])
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
      wLn("  \"", cmakeAbsolutePath(jucerFilePath), "\"");
      wLn(")");
    }
    wLn();
    wLn();
  }

  // set({JUCE,USER}_MODULES_GLOBAL_PATH)
  {
    auto shouldAddEmptyLines = false;

    if (juceModulesPath.isNotEmpty())
    {
      wLn("set(JUCE_MODULES_GLOBAL_PATH \"", cmakeAbsolutePath(juceModulesPath), "\")");
      shouldAddEmptyLines = true;
    }

    if (userModulesPath.isNotEmpty())
    {
      wLn("set(USER_MODULES_GLOBAL_PATH \"", cmakeAbsolutePath(userModulesPath), "\")");
      shouldAddEmptyLines = true;
    }

    if (shouldAddEmptyLines)
    {
      wLn();
      wLn();
    }
  }

  // jucer_project_begin()
  {
    wLn("jucer_project_begin(");
    wLn("  JUCER_VERSION \"", jucerVersion, "\"");
    wLn("  PROJECT_FILE \"${", jucerFileCMakeVar, "}\"");
    convertSetting(jucerProject, "id", "PROJECT_ID", {});
    wLn(")");
    wLn();
  }

  const auto projectType = jucerProject.getProperty("projectType").toString();

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
      const auto booleanWithLicenseRequiredTagline = [](const juce::var& v) {
        const auto value =
          v.isVoid() ? kDefaultLicenseBasedValue : (bool{v} ? "ON" : "OFF");
        return juce::String{value}
               + " # Required for closed source applications without an Indie or Pro "
                 "JUCE license";
      };
      convertOnOffSetting(jucerProject, "reportAppUsage", "REPORT_JUCE_APP_USAGE",
                          booleanWithLicenseRequiredTagline);
      convertOnOffSetting(jucerProject, "displaySplashScreen",
                          "DISPLAY_THE_JUCE_SPLASH_SCREEN",
                          booleanWithLicenseRequiredTagline);
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
      const auto companyNameString = jucerProject.getProperty("companyName").toString();
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
                            [](const juce::var& v) -> juce::String {
                              if (v.toString().isEmpty())
                                return "Default";
                              return juce::File::descriptionOfSizeInBytes(int{v});
                            });
    if (jucerProject.hasProperty("includeBinaryInJuceHeader"))
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

    if (jucerProject.hasProperty("cppLanguageStandard"))
    {
      convertSetting(jucerProject, "cppLanguageStandard", "CXX_LANGUAGE_STANDARD",
                     [](const juce::var& v) -> juce::String {
                       const auto value = v.toString();

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

    convertSettingAsListIfDefined(
      jucerProject, "defines", "PREPROCESSOR_DEFINITIONS",
      [](const juce::var& v) { return parsePreprocessorDefinitions(v.toString()); });
    convertSettingAsListIfDefined(
      jucerProject, "headerPath", "HEADER_SEARCH_PATHS", [](const juce::var& v) {
        return juce::StringArray::fromTokens(v.toString(), ";\r\n", {});
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
        convertSettingAsList(
          jucerProject, "pluginFormats", "PLUGIN_FORMATS",
          [&jucerVersionAsTuple, &convertIdsToStrings, &vstIsLegacy](const juce::var& v) {
            if (v.isVoid())
            {
              return juce::StringArray{vstIsLegacy ? "VST3" : "VST", "AU", "Standalone"};
            }
            const auto supportsUnity = jucerVersionAsTuple >= Version{5, 3, 2};
            return convertIdsToStrings(
              v, {{vstIsLegacy ? "" : "buildVST", vstIsLegacy ? "" : "VST"},
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

        convertSettingAsList(
          jucerProject, "pluginCharacteristicsValue", "PLUGIN_CHARACTERISTICS",
          [&convertIdsToStrings](const juce::var& v) {
            if (v.isVoid())
            {
              return juce::StringArray{};
            }
            return convertIdsToStrings(
              v, {{"pluginIsSynth", "Plugin is a Synth"},
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
        convertOnOffSettingWithDefault(jucerProject, "buildVST", "BUILD_VST", true);
        convertOnOffSettingWithDefault(jucerProject, "buildVST3", "BUILD_VST3", false);
        convertOnOffSettingWithDefault(jucerProject, "buildAU", "BUILD_AUDIOUNIT", true);
        convertOnOffSettingWithDefault(jucerProject, "buildAUv3", "BUILD_AUDIOUNIT_V3",
                                       false);
        convertOnOffSettingWithDefault(jucerProject, "buildRTAS", "BUILD_RTAS", false);
        convertOnOffSettingWithDefault(jucerProject, "buildAAX", "BUILD_AAX", false);
        if (jucerVersionAsTuple >= Version{5, 0, 0})
        {
          convertOnOffSettingWithDefault(jucerProject, "buildStandalone",
                                         "BUILD_STANDALONE_PLUGIN", false);
          convertOnOffSettingWithDefault(jucerProject, "enableIAA",
                                         "ENABLE_INTER_APP_AUDIO", false);
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
        const auto projectId = jucerProject.getProperty("id").toString();
        const auto s = makeValidIdentifier(projectId + projectId) + "xxxx";
        return s.substring(0, 1).toUpperCase() + s.substring(1, 4).toLowerCase();
      }();
      convertSettingWithDefault(jucerProject, "pluginCode", "PLUGIN_CODE",
                                defaultPluginCode);

      convertSetting(jucerProject, "pluginChannelConfigs",
                     "PLUGIN_CHANNEL_CONFIGURATIONS", {});

      const auto pluginCharacteristics = juce::StringArray::fromTokens(
        jucerProject.getProperty("pluginCharacteristicsValue").toString(), ",", {});

      const auto isSynthAudioPlugin =
        jucerVersionAsTuple >= Version{5, 3, 1}
          ? pluginCharacteristics.contains("pluginIsSynth")
          : jucerProject.hasProperty("pluginIsSynth")
              && bool{jucerProject.getProperty("pluginIsSynth")};

      if (jucerVersionAsTuple < Version{5, 3, 1})
      {
        wLn(juce::String{"  PLUGIN_IS_A_SYNTH "} + (isSynthAudioPlugin ? "ON" : "OFF"));
        convertOnOffSettingWithDefault(jucerProject, "pluginWantsMidiIn",
                                       "PLUGIN_MIDI_INPUT", false);
        convertOnOffSettingWithDefault(jucerProject, "pluginProducesMidiOut",
                                       "PLUGIN_MIDI_OUTPUT", false);
        convertOnOffSettingWithDefault(jucerProject, "pluginIsMidiEffectPlugin",
                                       "MIDI_EFFECT_PLUGIN", false);
        convertOnOffSettingWithDefault(jucerProject, "pluginEditorRequiresKeys",
                                       "KEY_FOCUS", false);
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
        convertSetting(jucerProject, "pluginAUMainType", "PLUGIN_AU_MAIN_TYPE",
                       [&pluginCharacteristics](const juce::var& v) -> juce::String {
                         if (v.isVoid())
                         {
                           if (pluginCharacteristics.contains("pluginIsMidiEffectPlugin"))
                             return "kAudioUnitType_MIDIProcessor"; // 'aumi'

                           if (pluginCharacteristics.contains("pluginIsSynth"))
                             return "kAudioUnitType_MusicDevice"; // 'aumu'

                           if (pluginCharacteristics.contains("pluginWantsMidiIn"))
                             return "kAudioUnitType_MusicEffect"; // 'aumf'

                           return "kAudioUnitType_Effect"; // 'aufx'
                         }

                         const auto value = v.toString();
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
        convertSetting(jucerProject, "pluginAUMainType", "PLUGIN_AU_MAIN_TYPE", {});
      }
      convertOnOffSettingIfDefined(jucerProject, "pluginAUIsSandboxSafe",
                                   "PLUGIN_AU_IS_SANDBOX_SAFE", {});

      if (jucerProject.hasProperty("pluginVSTNumMidiInputs")
          || (jucerVersionAsTuple >= Version{5, 4, 2}
              && pluginCharacteristics.contains("pluginWantsMidiIn")))
      {
        convertSettingWithDefault(jucerProject, "pluginVSTNumMidiInputs",
                                  "PLUGIN_VST_NUM_MIDI_INPUTS", "16");
      }
      if (jucerProject.hasProperty("pluginVSTNumMidiOutputs")
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

      if (jucerProject.hasProperty("pluginVST3Category")
          || jucerVersionAsTuple >= Version{5, 3, 1})
      {
        convertSettingAsList(
          jucerProject, "pluginVST3Category", "PLUGIN_VST3_CATEGORY",
          [isSynthAudioPlugin](const juce::var& v) {
            if (v.isVoid())
            {
              return isSynthAudioPlugin ? juce::StringArray{"Instrument", "Synth"}
                                        : juce::StringArray{"Fx"};
            }
            auto vst3_category = juce::StringArray::fromTokens(v.toString(), ",", {});
            if (vst3_category.contains("Instrument"))
            {
              vst3_category.move(vst3_category.indexOf("Instrument"), 0);
            }
            if (vst3_category.contains("Fx"))
            {
              vst3_category.move(vst3_category.indexOf("Fx"), 0);
            }
            return vst3_category;
          });
      }

      if (jucerVersionAsTuple >= Version{5, 3, 1})
      {
        convertSettingAsList(
          jucerProject, "pluginRTASCategory", "PLUGIN_RTAS_CATEGORY",
          [isSynthAudioPlugin, &convertIdsToStrings](const juce::var& v) {
            if (v.isVoid())
            {
              return juce::StringArray{isSynthAudioPlugin ? "ePlugInCategory_SWGenerators"
                                                          : "ePlugInCategory_None"};
            }
            return convertIdsToStrings(v, {{"0", "ePlugInCategory_None"},
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
        convertSettingAsList(
          jucerProject, "pluginAAXCategory", "PLUGIN_AAX_CATEGORY",
          [isSynthAudioPlugin,
           &convertIdsToStrings](const juce::var& v) -> juce::StringArray {
            if (v.isVoid())
            {
              return juce::StringArray{isSynthAudioPlugin
                                         ? "AAX_ePlugInCategory_SWGenerators"
                                         : "AAX_ePlugInCategory_None"};
            }
            return convertIdsToStrings(v, {{"0", "AAX_ePlugInCategory_None"},
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
      jucerProject.getProperty("compilerFlagSchemes").toString().isNotEmpty();

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

    std::function<void(const juce::ValueTree&)> processGroup =
      [&groupNames, &processGroup, &writeFiles](const juce::ValueTree& group) {
        groupNames.add(group.getProperty("name").toString());

        const auto fullGroupName = groupNames.joinIntoString("/");

        std::vector<File> files;

        for (auto i = 0; i < group.getNumChildren(); ++i)
        {
          const auto fileOrGroup = group.getChild(i);

          if (fileOrGroup.hasType("FILE"))
          {
            const auto& file = fileOrGroup;

            files.push_back({int{file.getProperty("compile")} == 1,
                             int{file.getProperty("xcodeResource")} == 1,
                             int{file.getProperty("resource")} == 1,
                             file.getProperty("file").toString(),
                             file.getProperty("compilerFlagScheme").toString()});
          }
          else
          {
            writeFiles(fullGroupName, files);
            files.clear();

            processGroup(fileOrGroup);
          }
        }

        writeFiles(fullGroupName, files);

        groupNames.strings.removeLast();
      };

    processGroup(jucerProject.getChildWithName("MAINGROUP"));
  }

  // jucer_project_module()
  {
    const auto modulePaths = jucerProject.getChildWithName("EXPORTFORMATS")
                               .getChild(0)
                               .getChildWithName("MODULEPATHS");

    const auto modules = jucerProject.getChildWithName("MODULES");
    for (auto i = 0; i < modules.getNumChildren(); ++i)
    {
      const auto& module = modules.getChild(i);
      const auto moduleName = module.getProperty("id").toString();

      const auto useGlobalPath = bool{module.getProperty("useGlobalPath")};
      const auto isJuceModule = moduleName.startsWith("juce_");

      if (useGlobalPath)
      {
        if (isJuceModule && juceModulesPath.isEmpty())
        {
          printError("The module " + moduleName.toStdString()
                     + " requires a global path. You should pass the JUCE modules global "
                       "path using --juce-modules.");
        }
        if (!isJuceModule && userModulesPath.isEmpty())
        {
          printError("The module " + moduleName.toStdString()
                     + " requires a global path. You should pass the user modules global "
                       "path using --user-modules.");
        }
      }

      const auto relativeModulePath =
        modulePaths.getChildWithProperty("id", moduleName).getProperty("path").toString();

      wLn("jucer_project_module(");
      wLn("  ", moduleName);
      wLn("  PATH \"",
          useGlobalPath ? (isJuceModule ? "${JUCE_MODULES_GLOBAL_PATH}"
                                        : "${USER_MODULES_GLOBAL_PATH}")
                        : relativeModulePath.replace("\\", "/"),
          "\"");

      const auto moduleHeader =
        (useGlobalPath ? (isJuceModule ? juceModules : userModules)
                       : jucerFile.getParentDirectory().getChildFile(relativeModulePath))
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

      const auto modulesOptions = jucerProject.getChildWithName("JUCEOPTIONS");

      for (const auto& line : moduleHeaderLines)
      {
        if (line.startsWith("/** Config: "))
        {
          const auto moduleOption = line.substring(12);
          const auto optionValue = modulesOptions.getProperty(moduleOption).toString();

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

    const auto kDefaultProjucerUserCodeSectionComment = juce::StringArray{
      "",
      "// (You can add your own code in this section, and the Projucer will not "
      "overwrite it)",
      ""};

    if (userCodeSectionLines != kDefaultProjucerUserCodeSectionComment)
    {
      wLn("jucer_appconfig_header(");
      wLn("  USER_CODE_SECTION");
      wLn("\"", escape("\\\"", userCodeSectionLines.joinIntoString("\n")), "\"");
      wLn(")");
      wLn();
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

    const auto exportFormats = jucerProject.getChildWithName("EXPORTFORMATS");
    for (auto iExporter = 0; iExporter < exportFormats.getNumChildren(); ++iExporter)
    {
      const auto exporter = exportFormats.getChild(iExporter);
      const auto exporterType = exporter.getType().toString();

      if (!supportedExporters.contains(exporterType))
      {
        continue;
      }

      const auto exporterName = exporterNames.at(exporterType);
      const auto configurations = exporter.getChildWithName("CONFIGURATIONS");

      wLn("jucer_export_target(");
      wLn("  \"", exporterName, "\"");

      const auto isXcodeExporter =
        exporterType == "XCODE_MAC" || exporterType == "XCODE_IPHONE";

      if (isXcodeExporter
          && (exporter.hasProperty("prebuildCommand")
              || exporter.hasProperty("postbuildCommand")))
      {
        wLn("  TARGET_PROJECT_FOLDER \"", exporter.getProperty("targetFolder").toString(),
            "\"  # only used by PREBUILD_SHELL_SCRIPT and POSTBUILD_SHELL_SCRIPT");
      }

      const auto isVSExporter = exporterType == "VS2019" || exporterType == "VS2017"
                                || exporterType == "VS2015" || exporterType == "VS2013";

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

      const auto isAudioPlugin = projectType == "audioplug";
      const auto pluginFormats = juce::StringArray::fromTokens(
        jucerProject.getProperty("pluginFormats").toString(), ",", {});
      const auto hasJuceAudioProcessorsModule =
        jucerProject.getChildWithName("MODULES")
          .getChildWithProperty("id", "juce_audio_processors")
          .isValid();

      const auto hasVst2Interface = jucerVersionAsTuple > Version{4, 2, 3};
      const auto isVstAudioPlugin = isAudioPlugin
                                    && (pluginFormats.contains("buildVST")
                                        || bool{jucerProject.getProperty("buildVST")});
      const auto pluginHostVstOption = jucerProject.getChildWithName("JUCEOPTIONS")
                                         .getProperty("JUCE_PLUGINHOST_VST")
                                         .toString();
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
      const auto isVst3AudioPlugin = isAudioPlugin
                                     && (pluginFormats.contains("buildVST3")
                                         || bool{jucerProject.getProperty("buildVST3")});
      const auto pluginHostVst3Option = jucerProject.getChildWithName("JUCEOPTIONS")
                                          .getProperty("JUCE_PLUGINHOST_VST3")
                                          .toString();
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
            || bool{jucerProject.getProperty("buildAAX")})
        {
          convertSetting(exporter, "aaxFolder", "AAX_SDK_FOLDER", {});
        }

        if (pluginFormats.contains("buildRTAS")
            || bool{jucerProject.getProperty("buildRTAS")})
        {
          convertSetting(exporter, "rtasFolder", "RTAS_SDK_FOLDER", {});
        }
      }

      convertSettingAsListIfDefined(
        exporter, "extraDefs", "EXTRA_PREPROCESSOR_DEFINITIONS",
        [](const juce::var& v) { return parsePreprocessorDefinitions(v.toString()); });
      convertSettingAsListIfDefined(
        exporter, "extraCompilerFlags", "EXTRA_COMPILER_FLAGS", [](const juce::var& v) {
          return juce::StringArray::fromTokens(v.toString(), false);
        });

      const auto compilerFlagSchemesArray = juce::StringArray::fromTokens(
        jucerProject.getProperty("compilerFlagSchemes").toString(), ",", {});
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

      convertSettingAsListIfDefined(
        exporter, "extraLinkerFlags", "EXTRA_LINKER_FLAGS", [](const juce::var& v) {
          return juce::StringArray::fromTokens(v.toString(), false);
        });
      convertSettingAsListIfDefined(exporter, "externalLibraries",
                                    "EXTERNAL_LIBRARIES_TO_LINK", {});

      convertOnOffSettingIfDefined(exporter, "enableGNUExtensions",
                                   "GNU_COMPILER_EXTENSIONS", {});

      const auto convertIcon = [&jucerProject](const juce::var& v) -> juce::String {
        const auto fileId = v.toString();

        if (fileId.isNotEmpty())
        {
          const auto file = getChildWithPropertyRecursively(
            jucerProject.getChildWithName("MAINGROUP"), "id", fileId);

          if (file.isValid())
          {
            return file.getProperty("file").toString();
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
          [](const juce::var& v) {
            auto folders = juce::StringArray::fromLines(v.toString());
            folders.trim();
            folders.removeEmptyStrings();
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
                                [](const juce::var& v) -> juce::String {
                                  const auto value = v.toString();

                                  if (value == "1")
                                    return "iPhone";

                                  if (value == "2")
                                    return "iPad";

                                  if (value == "1,2")
                                    return "Universal";

                                  return value;
                                });

        const auto screenOrientationFn = [](const juce::var& v) -> juce::String {
          const auto value = v.toString();

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
            [](const juce::var& v) {
              return juce::StringArray::fromTokens(v.toString(), ",", {});
            });
        }

        convertOnOffSettingIfDefined(exporter, "appSandbox", "USE_APP_SANDBOX", {});
        convertSettingAsListIfDefined(
          exporter, "appSandboxOptions", "APP_SANDBOX_OPTIONS",
          [&convertIdsToStrings](const juce::var& v) {
            return convertIdsToStrings(
              v,
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
                "File Access: Movies Folder (Read/Write)"}});
          });

        convertOnOffSettingIfDefined(exporter, "hardenedRuntime", "USE_HARDENED_RUNTIME",
                                     {});
        if (jucerVersionAsTuple >= Version{5, 4, 4})
        {
          convertSettingAsListIfDefined(
            exporter, "hardenedRuntimeOptions", "HARDENED_RUNTIME_OPTIONS",
            [&convertIdsToStrings](const juce::var& v) {
              return convertIdsToStrings(
                v,
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
            [&convertIdsToStrings](const juce::var& v) {
              return convertIdsToStrings(
                v,
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
        const auto convertPrefixHeader = [&jucerFile, &exporter](const juce::var& v) {
          const auto value = v.toString();

          if (value.isEmpty())
            return juce::String{};

          const auto jucerFileDir = jucerFile.getParentDirectory();
          const auto targetProjectDir =
            jucerFileDir.getChildFile(exporter.getProperty("targetFolder").toString());

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
          [](const juce::var& v) {
            auto frameworks = juce::StringArray::fromTokens(v.toString(), ",;", "\"'");
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
          exporter, "iosAppGroupsId", "APP_GROUP_ID", [](const juce::var& v) {
            auto groups = juce::StringArray::fromTokens(v.toString(), ";", {});
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

        if (exporter.hasProperty("toolset"))
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
        }

        convertSettingIfDefined(
          exporter, "IPPLibrary", "USE_IPP_LIBRARY",
          [&jucerVersionAsTuple](const juce::var& v) -> juce::String {
            const auto value = v.toString();

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
                                  [](const juce::var& v) -> juce::String {
                                    const auto value = v.toString();

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
                                [](const juce::var& v) -> juce::String {
                                  const auto value = v.toString();

                                  if (value == "-std=c++03")
                                    return "C++03";

                                  if (value == "-std=c++11")
                                    return "C++11";

                                  if (value == "-std=c++14")
                                    return "C++14";

                                  return {};
                                });

        convertSettingAsListIfDefined(
          exporter, "linuxExtraPkgConfig", "PKGCONFIG_LIBRARIES", [](const juce::var& v) {
            return juce::StringArray::fromTokens(v.toString(), " ", "\"'");
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

        convertSettingIfDefined(exporter, "codeBlocksWindowsTarget", "TARGET_PLATFORM",
                                [&windowsTargets](const juce::var& v) -> juce::String {
                                  const auto value = v.toString();

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

      for (auto i = 0; i < configurations.getNumChildren(); ++i)
      {
        const auto configuration = configurations.getChild(i);

        const auto originalConfigName = configuration.getProperty("name").toString();
        const auto sanitizedConfigName = sanitizeConfigurationName(originalConfigName);

        wLn("jucer_export_target_configuration(");
        wLn("  \"", exporterName, "\"");
        if (originalConfigName != sanitizedConfigName)
        {
          const auto jucerFileBasename = jucerFile.getFileName();
          wLn("  NAME \"", sanitizedConfigName, "\" # \"", originalConfigName, "\" in ",
              jucerFileBasename);
        }
        else
          wLn("  NAME \"", sanitizedConfigName, "\"");

        const auto isDebug = bool{configuration.getProperty("isDebug")};
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
          jucerFileDir.getChildFile(exporter.getProperty("targetFolder").toString());

        const auto convertSearchPaths =
          [&isAbsolutePath, &jucerFileDir,
           &targetProjectDir](const juce::var& v) -> juce::StringArray {
          const auto searchPaths = v.toString();

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

        convertSettingAsListIfDefined(
          configuration, "defines", "PREPROCESSOR_DEFINITIONS",
          [](const juce::var& v) { return parsePreprocessorDefinitions(v.toString()); });

        convertOnOffSettingIfDefined(configuration, "linkTimeOptimisation",
                                     "LINK_TIME_OPTIMISATION", {});

        if (!configuration.hasProperty("linkTimeOptimisation") && isVSExporter && !isDebug
            && jucerVersionAsTuple >= Version{5, 2, 0})
        {
          convertOnOffSettingIfDefined(configuration, "wholeProgramOptimisation",
                                       "LINK_TIME_OPTIMISATION", [](const juce::var& v) {
                                         if (int{v} == 0)
                                           return "ON";

                                         return "OFF";
                                       });
        }

        if (isXcodeExporter)
        {
          convertSettingIfDefined(configuration, "recommendedWarnings",
                                  "ADD_RECOMMENDED_COMPILER_WARNING_FLAGS",
                                  [](const juce::var& v) -> juce::String {
                                    const auto value = v.toString();

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
                                  [](const juce::var& v) -> juce::String {
                                    const auto value = v.toString();

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

        convertSettingIfDefined(configuration, "optimisation", "OPTIMISATION",
                                [&isVSExporter](const juce::var& v) -> juce::String {
                                  if (isVSExporter)
                                  {
                                    switch (int{v})
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

                                  switch (int{v})
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
            if (configuration.hasProperty("xcodeVstBinaryLocation"))
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

            if (configuration.hasProperty(oldProperty))
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
                                  [&sdks](const juce::var& v) -> juce::String {
                                    const auto value = v.toString();

                                    if (value == "default")
                                      return "Use Default";

                                    if (sdks.contains(value))
                                      return value;

                                    return {};
                                  });

          convertSettingIfDefined(configuration, "osxCompatibility",
                                  "OSX_DEPLOYMENT_TARGET",
                                  [&sdks](const juce::var& v) -> juce::String {
                                    const auto value = v.toString();

                                    if (value == "default")
                                      return "Use Default";

                                    if (sdks.contains(value))
                                      return value.substring(0, value.length() - 4);

                                    return {};
                                  });

          convertSettingIfDefined(configuration, "osxArchitecture", "OSX_ARCHITECTURE",
                                  [](const juce::var& v) -> juce::String {
                                    const auto value = v.toString();

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
            [](const juce::var& v) {
              auto customFlags = juce::StringArray::fromTokens(v.toString(), ",", "\"'");
              customFlags.removeEmptyStrings();

              for (auto& flag : customFlags)
              {
                flag = flag.upToFirstOccurrenceOf("=", false, false).trim() + " = "
                       + flag.fromFirstOccurrenceOf("=", false, false).trim();
              }

              return customFlags;
            });

          convertSettingAsListIfDefined(
            configuration, "plistPreprocessorDefinitions",
            "PLIST_PREPROCESSOR_DEFINITIONS", [](const juce::var& v) {
              return parsePreprocessorDefinitions(v.toString());
            });

          convertSettingIfDefined(configuration, "cppLanguageStandard",
                                  "CXX_LANGUAGE_STANDARD",
                                  [](const juce::var& v) -> juce::String {
                                    const auto value = v.toString();

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
                                  [](const juce::var& v) -> juce::String {
                                    const auto value = v.toString();

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
                                  [](const juce::var& v) -> juce::String {
                                    switch (int{v})
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
                                  [](const juce::var& v) -> juce::String {
                                    const auto value = v.toString();

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
                                    [](const juce::var& v) -> juce::String {
                                      if (v.toString().isEmpty())
                                        return "Enable when possible";

                                      if (int{v} > 0)
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
                                  [](const juce::var& v) -> juce::String {
                                    const auto value = v.toString();

                                    if (value.isEmpty())
                                      return "Default";

                                    return value;
                                  });

          if (configuration.hasProperty("winArchitecture"))
          {
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
          }

          convertSettingIfDefined(
            configuration, "debugInformationFormat", "DEBUG_INFORMATION_FORMAT",
            [](const juce::var& v) -> juce::String {
              const auto value = v.toString();

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
                                  [](const juce::var& v) -> juce::String {
                                    const auto value = v.toString();

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

        const auto codeBlocksArchitecture = [](const juce::var& v) -> juce::String {
          const auto value = v.toString();

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
          if (jucerProject.hasProperty("windowsCodeBlocksArchitecture")
              || jucerVersionAsTuple >= Version{5, 0, 0})
          {
            convertSetting(configuration, "windowsCodeBlocksArchitecture", "ARCHITECTURE",
                           codeBlocksArchitecture);
          }
        }

        if (exporterType == "CODEBLOCKS_LINUX")
        {
          if (jucerProject.hasProperty("linuxCodeBlocksArchitecture")
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

  const auto outputFile =
    juce::File::getCurrentWorkingDirectory().getChildFile("CMakeLists.txt");

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
