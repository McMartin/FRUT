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


juce::StringArray
convertIdsToStrings(const juce::var& v,
                    const std::vector<std::pair<juce::String, const char*>>& idsToStrings)
{
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


juce::ValueTree getChildWithPropertyRecursively(const juce::ValueTree& VT,
                                                const juce::Identifier& propertyName,
                                                const juce::var& propertyValue)
{
  const auto childVT = VT.getChildWithProperty(propertyName, propertyValue);

  if (childVT.isValid())
  {
    return childVT;
  }

  for (auto i = 0; i < VT.getNumChildren(); ++i)
  {
    const auto grandchildVT =
      getChildWithPropertyRecursively(VT.getChild(i), propertyName, propertyValue);

    if (grandchildVT.isValid())
    {
      return grandchildVT;
    }
  }

  return {};
}


void writeUserNotes(LineWriter& wLn, const juce::ValueTree& VT)
{
  if (VT.hasProperty("userNotes"))
  {
    wLn("  # NOTES");
    const auto userNotes = VT.getProperty("userNotes").toString();
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

  const juce::XmlElement fallbackXmlElement{":"};
  const auto safeGetChildByName =
    [&fallbackXmlElement](const juce::XmlElement& element,
                          const juce::StringRef childName) -> const juce::XmlElement& {
    if (const auto pChild = element.getChildByName(childName))
    {
      return *pChild;
    }

    return fallbackXmlElement;
  };

  const auto& jucerProject = *xml;
  const auto jucerProjectVT = juce::ValueTree::fromXml(jucerProject);

  const auto jucerVersion = jucerProjectVT.getProperty("jucerVersion").toString();
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
    [&wLn](const juce::ValueTree& VT, const juce::Identifier& property,
           const juce::String& cmakeKeyword,
           std::function<juce::String(const juce::var&)> converterFn) {
      if (!converterFn)
      {
        converterFn = [](const juce::var& v) { return v.toString(); };
      }

      const auto value = converterFn(VT.getProperty(property));

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
    [&convertSetting](const juce::ValueTree& VT, const juce::Identifier& property,
                      const juce::String& cmakeKeyword,
                      std::function<juce::String(const juce::var&)> converterFn) {
      if (VT.hasProperty(property))
      {
        convertSetting(VT, property, cmakeKeyword, std::move(converterFn));
      }
    };

  const auto convertSettingWithDefault =
    [&convertSetting](const juce::ValueTree& VT, const juce::Identifier& property,
                      const juce::String& cmakeKeyword,
                      const juce::String& defaultValue) {
      convertSetting(VT, property, cmakeKeyword,
                     [&defaultValue](const juce::var& v) -> juce::String {
                       return v.isVoid() ? defaultValue : v.toString();
                     });
    };

  const auto convertOnOffSetting =
    [&wLn](const juce::ValueTree& VT, const juce::Identifier& property,
           const juce::String& cmakeKeyword,
           std::function<juce::String(const juce::var&)> converterFn) {
      if (!converterFn)
      {
        converterFn = [](const juce::var& v) -> juce::String {
          return v.isVoid() ? "" : bool{v} ? "ON" : "OFF";
        };
      }

      const auto value = converterFn(VT.getProperty(property));

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
    [&convertOnOffSetting](const juce::ValueTree& VT,
                           const juce::Identifier& property,
                           const juce::String& cmakeKeyword,
                           std::function<juce::String(const juce::var&)> converterFn) {
      if (VT.hasProperty(property))
      {
        convertOnOffSetting(VT, property, cmakeKeyword, std::move(converterFn));
      }
    };

  const auto convertOnOffSettingWithDefault =
    [&convertOnOffSetting](const juce::ValueTree& VT,
                           const juce::Identifier& property,
                           const juce::String& cmakeKeyword, bool defaultValue) {
      convertOnOffSetting(VT, property, cmakeKeyword,
                          [defaultValue](const juce::var& v) -> juce::String {
                            return v.isVoid() ? (defaultValue ? "ON" : "OFF")
                                              : (bool{v} ? "ON" : "OFF");
                          });
    };

  const auto convertSettingAsList =
    [&wLn](const juce::ValueTree& VT, const juce::Identifier& property,
           const juce::String& cmakeKeyword,
           std::function<juce::StringArray(const juce::var&)> converterFn) {
      if (!converterFn)
      {
        converterFn = [](const juce::var& v) {
          return juce::StringArray::fromLines(v.toString());
        };
      }

      auto value = converterFn(VT.getProperty(property));
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
      const juce::ValueTree& VT, const juce::Identifier& property,
      const juce::String& cmakeKeyword,
      std::function<juce::StringArray(const juce::var&)> converterFn) {
      if (VT.hasProperty(property))
      {
        convertSettingAsList(VT, property, cmakeKeyword, std::move(converterFn));
      }
    };

  const auto jucerFileName = jucerFile.getFileName();
  const auto jucerProjectName = jucerProjectVT.getProperty("name").toString();

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
      const auto relativeJucerFilePath =
        juce::File::getCurrentWorkingDirectory()
          .getChildFile(jucerFilePath)
          .getRelativePathFrom(juce::File::getCurrentWorkingDirectory());
      // On Windows, it is not possible to make a relative path between two drives, so
      // `relativeJucerFilePath` might be absolute if the .jucer file is on another drive.
      const auto jucerFileCMakePath =
        (juce::File::isAbsolutePath(relativeJucerFilePath)
           ? relativeJucerFilePath
           : "${CMAKE_CURRENT_LIST_DIR}/" + relativeJucerFilePath)
          .replace("\\", "/");
      wLn("  \"", jucerFileCMakePath, "\"");
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
    convertSetting(jucerProjectVT, "id", "PROJECT_ID", {});
    wLn(")");
    wLn();
  }

  const auto projectType = jucerProjectVT.getProperty("projectType").toString();

  // jucer_project_settings()
  {
    wLn("jucer_project_settings(");
    convertSetting(jucerProjectVT, "name", "PROJECT_NAME", {});
    convertSettingWithDefault(jucerProjectVT, "version", "PROJECT_VERSION", "1.0.0");

    convertSettingIfDefined(jucerProjectVT, "companyName", "COMPANY_NAME", {});
    convertSettingIfDefined(jucerProjectVT, "companyCopyright", "COMPANY_COPYRIGHT", {});
    convertSettingIfDefined(jucerProjectVT, "companyWebsite", "COMPANY_WEBSITE", {});
    convertSettingIfDefined(jucerProjectVT, "companyEmail", "COMPANY_EMAIL", {});

    if (jucerVersionAsTuple >= Version{5, 0, 0})
    {
      const auto booleanWithLicenseRequiredTagline = [](const juce::var& v) {
        const auto value =
          v.isVoid() ? kDefaultLicenseBasedValue : (bool{v} ? "ON" : "OFF");
        return juce::String{value}
               + " # Required for closed source applications without an Indie or Pro "
                 "JUCE license";
      };
      convertOnOffSetting(jucerProjectVT, "reportAppUsage", "REPORT_JUCE_APP_USAGE",
                          booleanWithLicenseRequiredTagline);
      convertOnOffSetting(jucerProjectVT, "displaySplashScreen",
                          "DISPLAY_THE_JUCE_SPLASH_SCREEN",
                          booleanWithLicenseRequiredTagline);
      convertSettingIfDefined(jucerProjectVT, "splashScreenColour", "SPLASH_SCREEN_COLOUR",
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

    const auto defaultCompanyName = [&jucerProjectVT]() {
      const auto companyNameString = jucerProjectVT.getProperty("companyName").toString();
      return companyNameString.isEmpty() ? "yourcompany" : companyNameString;
    }();

    const auto defaultBundleIdentifier =
      jucerVersionAsTuple >= Version{5, 4, 0}
        ? "com." + makeValidIdentifier(defaultCompanyName) + "."
            + makeValidIdentifier(jucerProjectName)
        : "com.yourcompany." + makeValidIdentifier(jucerProjectName);

    convertSettingWithDefault(jucerProjectVT, "bundleIdentifier", "BUNDLE_IDENTIFIER",
                              defaultBundleIdentifier);

    convertSettingIfDefined(jucerProjectVT, "maxBinaryFileSize", "BINARYDATACPP_SIZE_LIMIT",
                            [](const juce::var& v) -> juce::String {
                              if (v.toString().isEmpty())
                                return "Default";
                              return juce::File::descriptionOfSizeInBytes(int{v});
                            });
    if (jucerProjectVT.hasProperty("includeBinaryInJuceHeader"))
    {
      convertOnOffSetting(jucerProjectVT, "includeBinaryInJuceHeader", "INCLUDE_BINARYDATA",
                          {});
    }
    else
    {
      convertOnOffSettingIfDefined(jucerProjectVT, "includeBinaryInAppConfig",
                                   "INCLUDE_BINARYDATA", {});
    }
    convertSettingIfDefined(jucerProjectVT, "binaryDataNamespace", "BINARYDATA_NAMESPACE",
                            {});

    if (jucerProjectVT.hasProperty("cppLanguageStandard"))
    {
      convertSetting(jucerProjectVT, "cppLanguageStandard", "CXX_LANGUAGE_STANDARD",
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
      jucerProjectVT, "defines", "PREPROCESSOR_DEFINITIONS",
      [](const juce::var& v) { return parsePreprocessorDefinitions(v.toString()); });
    convertSettingAsListIfDefined(
      jucerProjectVT, "headerPath", "HEADER_SEARCH_PATHS", [](const juce::var& v) {
        return juce::StringArray::fromTokens(v.toString(), ";\r\n", {});
      });

    convertSettingIfDefined(jucerProjectVT, "postExportShellCommandPosix",
                            "POST_EXPORT_SHELL_COMMAND_MACOS_LINUX", {});
    convertSettingIfDefined(jucerProjectVT, "postExportShellCommandWin",
                            "POST_EXPORT_SHELL_COMMAND_WINDOWS", {});

    writeUserNotes(wLn, jucerProjectVT);

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
          jucerProjectVT, "pluginFormats", "PLUGIN_FORMATS",
          [&jucerVersionAsTuple, &vstIsLegacy](const juce::var& v) {
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
          jucerProjectVT, "pluginCharacteristicsValue", "PLUGIN_CHARACTERISTICS",
          [](const juce::var& v) {
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
        convertOnOffSettingWithDefault(jucerProjectVT, "buildVST", "BUILD_VST", true);
        convertOnOffSettingWithDefault(jucerProjectVT, "buildVST3", "BUILD_VST3", false);
        convertOnOffSettingWithDefault(jucerProjectVT, "buildAU", "BUILD_AUDIOUNIT", true);
        convertOnOffSettingWithDefault(jucerProjectVT, "buildAUv3", "BUILD_AUDIOUNIT_V3",
                                       false);
        convertOnOffSettingWithDefault(jucerProjectVT, "buildRTAS", "BUILD_RTAS", false);
        convertOnOffSettingWithDefault(jucerProjectVT, "buildAAX", "BUILD_AAX", false);
        if (jucerVersionAsTuple >= Version{5, 0, 0})
        {
          convertOnOffSettingWithDefault(jucerProjectVT, "buildStandalone",
                                         "BUILD_STANDALONE_PLUGIN", false);
          convertOnOffSettingWithDefault(jucerProjectVT, "enableIAA",
                                         "ENABLE_INTER_APP_AUDIO", false);
        }
      }

      convertSettingWithDefault(jucerProjectVT, "pluginName", "PLUGIN_NAME",
                                jucerProjectName);
      convertSettingWithDefault(jucerProjectVT, "pluginDesc", "PLUGIN_DESCRIPTION",
                                jucerProjectName);

      convertSettingWithDefault(jucerProjectVT, "pluginManufacturer", "PLUGIN_MANUFACTURER",
                                defaultCompanyName);
      convertSettingWithDefault(jucerProjectVT, "pluginManufacturerCode",
                                "PLUGIN_MANUFACTURER_CODE", "Manu");

      const auto defaultPluginCode = [&jucerProjectVT]() {
        const auto projectId = jucerProjectVT.getProperty("id").toString();
        const auto s = makeValidIdentifier(projectId + projectId) + "xxxx";
        return s.substring(0, 1).toUpperCase() + s.substring(1, 4).toLowerCase();
      }();
      convertSettingWithDefault(jucerProjectVT, "pluginCode", "PLUGIN_CODE",
                                defaultPluginCode);

      convertSetting(jucerProjectVT, "pluginChannelConfigs",
                     "PLUGIN_CHANNEL_CONFIGURATIONS", {});

      const auto pluginCharacteristics = juce::StringArray::fromTokens(
        jucerProjectVT.getProperty("pluginCharacteristicsValue").toString(), ",", {});

      const auto isSynthAudioPlugin = jucerVersionAsTuple >= Version{5, 3, 1}
                                        ? pluginCharacteristics.contains("pluginIsSynth")
                                        : bool{jucerProjectVT.getProperty("pluginIsSynth")};

      if (jucerVersionAsTuple < Version{5, 3, 1})
      {
        wLn(juce::String{"  PLUGIN_IS_A_SYNTH "} + (isSynthAudioPlugin ? "ON" : "OFF"));
        convertOnOffSettingWithDefault(jucerProjectVT, "pluginWantsMidiIn",
                                       "PLUGIN_MIDI_INPUT", false);
        convertOnOffSettingWithDefault(jucerProjectVT, "pluginProducesMidiOut",
                                       "PLUGIN_MIDI_OUTPUT", false);
        convertOnOffSettingWithDefault(jucerProjectVT, "pluginIsMidiEffectPlugin",
                                       "MIDI_EFFECT_PLUGIN", false);
        convertOnOffSettingWithDefault(jucerProjectVT, "pluginEditorRequiresKeys",
                                       "KEY_FOCUS", false);
      }

      if (jucerVersionAsTuple >= Version{5, 3, 1})
      {
        convertSettingWithDefault(jucerProjectVT, "aaxIdentifier", "PLUGIN_AAX_IDENTIFIER",
                                  defaultBundleIdentifier);
      }
      convertSettingWithDefault(jucerProjectVT, "pluginAUExportPrefix",
                                "PLUGIN_AU_EXPORT_PREFIX",
                                makeValidIdentifier(jucerProjectName) + "AU");
      if (jucerVersionAsTuple >= Version{5, 3, 1})
      {
        convertSetting(jucerProjectVT, "pluginAUMainType", "PLUGIN_AU_MAIN_TYPE",
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
        convertSetting(jucerProjectVT, "pluginAUMainType", "PLUGIN_AU_MAIN_TYPE", {});
      }
      convertOnOffSettingIfDefined(jucerProjectVT, "pluginAUIsSandboxSafe",
                                   "PLUGIN_AU_IS_SANDBOX_SAFE", {});

      if (jucerProjectVT.hasProperty("pluginVSTNumMidiInputs")
          || (jucerVersionAsTuple >= Version{5, 4, 2}
              && pluginCharacteristics.contains("pluginWantsMidiIn")))
      {
        convertSettingWithDefault(jucerProjectVT, "pluginVSTNumMidiInputs",
                                  "PLUGIN_VST_NUM_MIDI_INPUTS", "16");
      }
      if (jucerProjectVT.hasProperty("pluginVSTNumMidiOutputs")
          || (jucerVersionAsTuple >= Version{5, 4, 2}
              && pluginCharacteristics.contains("pluginProducesMidiOut")))
      {
        convertSettingWithDefault(jucerProjectVT, "pluginVSTNumMidiOutputs",
                                  "PLUGIN_VST_NUM_MIDI_OUTPUTS", "16");
      }

      if (!vstIsLegacy)
      {
        convertSettingWithDefault(
          jucerProjectVT, "pluginVSTCategory",
          jucerVersionAsTuple > Version{5, 3, 0} ? "PLUGIN_VST_CATEGORY" : "VST_CATEGORY",
          jucerVersionAsTuple >= Version{5, 3, 1}
            ? (isSynthAudioPlugin ? "kPlugCategSynth" : "kPlugCategEffect")
            : "");
      }

      if (jucerProjectVT.hasProperty("pluginVST3Category")
          || jucerVersionAsTuple >= Version{5, 3, 1})
      {
        convertSettingAsList(
          jucerProjectVT, "pluginVST3Category", "PLUGIN_VST3_CATEGORY",
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
          jucerProjectVT, "pluginRTASCategory", "PLUGIN_RTAS_CATEGORY",
          [isSynthAudioPlugin](const juce::var& v) {
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
          jucerProjectVT, "pluginAAXCategory", "PLUGIN_AAX_CATEGORY",
          [isSynthAudioPlugin](const juce::var& v) -> juce::StringArray {
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
        convertSetting(jucerProjectVT, "pluginRTASCategory", "PLUGIN_RTAS_CATEGORY", {});
        convertSetting(jucerProjectVT, "pluginAAXCategory", "PLUGIN_AAX_CATEGORY", {});
      }

      if (vstIsLegacy)
      {
        convertSettingWithDefault(
          jucerProjectVT, "pluginVSTCategory", "PLUGIN_VST_LEGACY_CATEGORY",
          isSynthAudioPlugin ? "kPlugCategSynth" : "kPlugCategEffect");
      }

      if (jucerVersionAsTuple < Version{5, 3, 1})
      {
        convertSetting(jucerProjectVT, "aaxIdentifier", "PLUGIN_AAX_IDENTIFIER", {});
      }

      wLn(")");
      wLn();
    }
  }

  // jucer_project_files()
  {
    const auto projectHasCompilerFlagSchemes =
      jucerProjectVT.getProperty("compilerFlagSchemes").toString().isNotEmpty();

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
      [&groupNames, &processGroup, &writeFiles](const juce::ValueTree& groupVT) {
        groupNames.add(groupVT.getProperty("name").toString());

        const auto fullGroupName = groupNames.joinIntoString("/");

        std::vector<File> files;

        for (auto i = 0; i < groupVT.getNumChildren(); ++i)
        {
          const auto fileOrGroupVT = groupVT.getChild(i);

          if (fileOrGroupVT.hasType("FILE"))
          {
            const auto& fileVT = fileOrGroupVT;

            files.push_back({int{fileVT.getProperty("compile")} == 1,
                             int{fileVT.getProperty("xcodeResource")} == 1,
                             int{fileVT.getProperty("resource")} == 1,
                             fileVT.getProperty("file").toString(),
                             fileVT.getProperty("compilerFlagScheme").toString()});
          }
          else
          {
            writeFiles(fullGroupName, files);
            files.clear();

            processGroup(fileOrGroupVT);
          }
        }

        writeFiles(fullGroupName, files);

        groupNames.strings.removeLast();
      };

    processGroup(juce::ValueTree::fromXml(safeGetChildByName(jucerProject, "MAINGROUP")));
  }

  // jucer_project_module()
  {
    const auto modulePathsVT = juce::ValueTree::fromXml(safeGetChildByName(jucerProject, "EXPORTFORMATS"))
                               .getChild(0)
                               .getChildWithName("MODULEPATHS");

    const auto modulesVT = juce::ValueTree::fromXml(safeGetChildByName(jucerProject, "MODULES"));
    for (auto i = 0; i < modulesVT.getNumChildren(); ++i)
    {
      const auto moduleVT = modulesVT.getChild(i);
      const auto moduleName = moduleVT.getProperty("id").toString();

      const auto useGlobalPath = bool{moduleVT.getProperty("useGlobalPath")};
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
        modulePathsVT.getChildWithProperty("id", moduleName).getProperty("path").toString();

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

      const auto modulesOptionsVT = juce::ValueTree::fromXml(safeGetChildByName(jucerProject, "JUCEOPTIONS"));

      for (const auto& line : moduleHeaderLines)
      {
        if (line.startsWith("/** Config: "))
        {
          const auto moduleOption = line.substring(12);
          const auto optionValue = modulesOptionsVT.getProperty(moduleOption).toString();

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

    const auto exportFormatsVT = juce::ValueTree::fromXml(safeGetChildByName(jucerProject, "EXPORTFORMATS"));
    for (auto iExporter = 0; iExporter < exportFormatsVT.getNumChildren(); ++iExporter)
    {
      const auto exporterVT = exportFormatsVT.getChild(iExporter);
      const auto exporterType = exporterVT.getType().toString();

      if (!supportedExporters.contains(exporterType))
      {
        continue;
      }

      const auto exporterName = exporterNames.at(exporterType);
      const auto configurationsVT = exporterVT.getChildWithName("CONFIGURATIONS");

      wLn("jucer_export_target(");
      wLn("  \"", exporterName, "\"");

      const auto isXcodeExporter =
        exporterType == "XCODE_MAC" || exporterType == "XCODE_IPHONE";

      if (isXcodeExporter
          && (exporterVT.hasProperty("prebuildCommand")
              || exporterVT.hasProperty("postbuildCommand")))
      {
        wLn("  TARGET_PROJECT_FOLDER \"", exporterVT.getProperty("targetFolder").toString(),
            "\"  # only used by PREBUILD_SHELL_SCRIPT and POSTBUILD_SHELL_SCRIPT");
      }

      const auto isVSExporter = exporterType == "VS2019" || exporterType == "VS2017"
                                || exporterType == "VS2015" || exporterType == "VS2013";

      if (isVSExporter)
      {
        const auto needsTargetFolder = [&configurationsVT]() {
          for (auto i = 0; i < configurationsVT.getNumChildren(); ++i)
          {
            const auto configurationVT = configurationsVT.getChild(i);

            if (configurationVT.hasProperty("prebuildCommand")
                || configurationVT.hasProperty("postbuildCommand"))
            {
              return true;
            }
          }
          return false;
        }();

        if (needsTargetFolder)
        {
          wLn("  TARGET_PROJECT_FOLDER \"",
              exporterVT.getProperty("targetFolder").toString(),
              "\" # only used by PREBUILD_COMMAND and POSTBUILD_COMMAND");
        }
      }

      const auto isAudioPlugin = projectType == "audioplug";
      const auto pluginFormats = juce::StringArray::fromTokens(
        jucerProjectVT.getProperty("pluginFormats").toString(), ",", {});
      const auto hasJuceAudioProcessorsModule =
        juce::ValueTree::fromXml(safeGetChildByName(jucerProject, "MODULES"))
          .getChildWithProperty("id", "juce_audio_processors")
          .isValid();

      const auto hasVst2Interface = jucerVersionAsTuple > Version{4, 2, 3};
      const auto isVstAudioPlugin = isAudioPlugin
                                    && (pluginFormats.contains("buildVST")
                                        || bool{jucerProjectVT.getProperty("buildVST")});
      const auto pluginHostVstOption = juce::ValueTree::fromXml(safeGetChildByName(jucerProject, "JUCEOPTIONS"))
                                         .getProperty("JUCE_PLUGINHOST_VST")
                                         .toString();
      const auto isVstPluginHost =
        hasJuceAudioProcessorsModule
        && (pluginHostVstOption == "enabled" || pluginHostVstOption == "1");

      if (!hasVst2Interface && (isVstAudioPlugin || isVstPluginHost))
      {
        convertSetting(exporterVT, "vstFolder", "VST_SDK_FOLDER", {});
      }

      const auto vstIsLegacy = jucerVersionAsTuple > Version{5, 3, 2};

      if (vstIsLegacy && (isVstAudioPlugin || isVstPluginHost))
      {
        convertSetting(exporterVT, "vstLegacyFolder", "VST_LEGACY_SDK_FOLDER", {});
      }

      const auto supportsVst3 = exporterType == "XCODE_MAC" || isVSExporter;
      const auto isVst3AudioPlugin = isAudioPlugin
                                     && (pluginFormats.contains("buildVST3")
                                         || bool{jucerProjectVT.getProperty("buildVST3")});
      const auto pluginHostVst3Option = juce::ValueTree::fromXml(safeGetChildByName(jucerProject, "JUCEOPTIONS"))
                                          .getProperty("JUCE_PLUGINHOST_VST3")
                                          .toString();
      const auto isVst3PluginHost =
        hasJuceAudioProcessorsModule
        && (pluginHostVst3Option == "enabled" || pluginHostVst3Option == "1");

      if (supportsVst3 && (isVst3AudioPlugin || isVst3PluginHost))
      {
        convertSetting(exporterVT, "vst3Folder", "VST3_SDK_FOLDER", {});
      }

      const auto supportsAaxRtas = exporterType == "XCODE_MAC" || isVSExporter;

      if (supportsAaxRtas && isAudioPlugin)
      {
        if (pluginFormats.contains("buildAAX")
            || bool{jucerProjectVT.getProperty("buildAAX")})
        {
          convertSetting(exporterVT, "aaxFolder", "AAX_SDK_FOLDER", {});
        }

        if (pluginFormats.contains("buildRTAS")
            || bool{jucerProjectVT.getProperty("buildRTAS")})
        {
          convertSetting(exporterVT, "rtasFolder", "RTAS_SDK_FOLDER", {});
        }
      }

      convertSettingAsListIfDefined(
        exporterVT, "extraDefs", "EXTRA_PREPROCESSOR_DEFINITIONS",
        [](const juce::var& v) { return parsePreprocessorDefinitions(v.toString()); });
      convertSettingAsListIfDefined(
        exporterVT, "extraCompilerFlags", "EXTRA_COMPILER_FLAGS", [](const juce::var& v) {
          return juce::StringArray::fromTokens(v.toString(), false);
        });

      const auto compilerFlagSchemesArray = juce::StringArray::fromTokens(
        jucerProjectVT.getProperty("compilerFlagSchemes").toString(), ",", {});
      // Use a juce::HashMap like Projucer does, in order to get the same ordering.
      juce::HashMap<juce::String, std::tuple<>> compilerFlagSchemesMap;
      for (const auto& scheme : compilerFlagSchemesArray)
      {
        compilerFlagSchemesMap.set(scheme, {});
      }
      for (juce::HashMap<juce::String, std::tuple<>>::Iterator i(compilerFlagSchemesMap);
           i.next();)
      {
        convertSettingIfDefined(exporterVT, i.getKey(), "COMPILER_FLAGS_FOR_" + i.getKey(),
                                {});
      }

      convertSettingAsListIfDefined(
        exporterVT, "extraLinkerFlags", "EXTRA_LINKER_FLAGS", [](const juce::var& v) {
          return juce::StringArray::fromTokens(v.toString(), false);
        });
      convertSettingAsListIfDefined(exporterVT, "externalLibraries",
                                    "EXTERNAL_LIBRARIES_TO_LINK", {});

      convertOnOffSettingIfDefined(exporterVT, "enableGNUExtensions",
                                   "GNU_COMPILER_EXTENSIONS", {});

      const auto convertIcon = [&safeGetChildByName, &jucerProject](const juce::var& v) -> juce::String {
        const auto fileId = v.toString();

        if (fileId.isNotEmpty())
        {
          const auto fileVT = getChildWithPropertyRecursively(
            juce::ValueTree::fromXml(safeGetChildByName(jucerProject, "MAINGROUP")), "id", fileId);

          if (fileVT.isValid())
          {
            return fileVT.getProperty("file").toString();
          }
        }

        return "<None>";
      };

      convertSettingIfDefined(exporterVT, "smallIcon", "ICON_SMALL", convertIcon);
      convertSettingIfDefined(exporterVT, "bigIcon", "ICON_LARGE", convertIcon);

      if (exporterType == "XCODE_IPHONE")
      {
        convertSettingIfDefined(exporterVT, "customXcassetsFolder",
                                "CUSTOM_XCASSETS_FOLDER", {});
        convertSettingIfDefined(exporterVT, "customLaunchStoryboard",
                                "CUSTOM_LAUNCH_STORYBOARD", {});
      }

      if (isXcodeExporter)
      {
        convertSettingAsListIfDefined(
          exporterVT, "customXcodeResourceFolders", "CUSTOM_XCODE_RESOURCE_FOLDERS",
          [](const juce::var& v) {
            auto folders = juce::StringArray::fromLines(v.toString());
            folders.trim();
            folders.removeEmptyStrings();
            return folders;
          });

        if (isAudioPlugin)
        {
          convertOnOffSettingIfDefined(exporterVT, "duplicateAppExResourcesFolder",
                                       "ADD_DUPLICATE_RESOURCES_FOLDER_TO_APP_EXTENSION",
                                       {});
        }
      }

      if (exporterType == "XCODE_IPHONE")
      {
        convertSettingIfDefined(exporterVT, "iosDeviceFamily", "DEVICE_FAMILY",
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
        convertSettingIfDefined(exporterVT, "iosScreenOrientation",
                                "IPHONE_SCREEN_ORIENTATION", screenOrientationFn);
        convertSettingIfDefined(exporterVT, "iPadScreenOrientation",
                                "IPAD_SCREEN_ORIENTATION", screenOrientationFn);

        convertOnOffSettingIfDefined(exporterVT, "UIFileSharingEnabled",
                                     "FILE_SHARING_ENABLED", {});
        convertOnOffSettingIfDefined(exporterVT, "UISupportsDocumentBrowser",
                                     "SUPPORT_DOCUMENT_BROWSER", {});
        convertOnOffSettingIfDefined(exporterVT, "UIStatusBarHidden", "STATUS_BAR_HIDDEN",
                                     {});
      }

      if (exporterType == "XCODE_MAC")
      {
        if (projectType == "guiapp")
        {
          convertSettingAsListIfDefined(
            exporterVT, "documentExtensions", "DOCUMENT_FILE_EXTENSIONS",
            [](const juce::var& v) {
              return juce::StringArray::fromTokens(v.toString(), ",", {});
            });
        }

        convertOnOffSettingIfDefined(exporterVT, "appSandbox", "USE_APP_SANDBOX", {});
        convertOnOffSettingIfDefined(exporterVT, "appSandboxInheritance",
                                     "APP_SANDBOX_INHERITANCE", {});
        convertSettingAsListIfDefined(
          exporterVT, "appSandboxOptions", "APP_SANDBOX_OPTIONS", [](const juce::var& v) {
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

        convertOnOffSettingIfDefined(exporterVT, "hardenedRuntime", "USE_HARDENED_RUNTIME",
                                     {});
        if (jucerVersionAsTuple >= Version{5, 4, 4})
        {
          convertSettingAsListIfDefined(
            exporterVT, "hardenedRuntimeOptions", "HARDENED_RUNTIME_OPTIONS",
            [](const juce::var& v) {
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
            exporterVT, "hardenedRuntimeOptions", "HARDENED_RUNTIME_OPTIONS",
            [](const juce::var& v) {
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
        convertOnOffSettingIfDefined(exporterVT, "microphonePermissionNeeded",
                                     "MICROPHONE_ACCESS", {});
        convertSettingIfDefined(exporterVT, "microphonePermissionsText",
                                "MICROPHONE_ACCESS_TEXT", {});
        convertOnOffSettingIfDefined(exporterVT, "cameraPermissionNeeded", "CAMERA_ACCESS",
                                     {});
        convertSettingIfDefined(exporterVT, "cameraPermissionText", "CAMERA_ACCESS_TEXT",
                                {});
      }

      if (exporterType == "XCODE_IPHONE")
      {
        convertOnOffSettingIfDefined(exporterVT, "iosBluetoothPermissionNeeded",
                                     "BLUETOOTH_ACCESS", {});
        convertSettingIfDefined(exporterVT, "iosBluetoothPermissionText",
                                "BLUETOOTH_ACCESS_TEXT", {});
      }

      if (isXcodeExporter)
      {
        convertOnOffSettingIfDefined(exporterVT, "iosInAppPurchasesValue",
                                     "IN_APP_PURCHASES_CAPABILITY", {});
      }

      if (exporterType == "XCODE_IPHONE")
      {
        convertOnOffSettingIfDefined(exporterVT, "iosBackgroundAudio",
                                     "AUDIO_BACKGROUND_CAPABILITY", {});
        convertOnOffSettingIfDefined(exporterVT, "iosBackgroundBle",
                                     "BLUETOOTH_MIDI_BACKGROUND_CAPABILITY", {});
        convertOnOffSettingIfDefined(exporterVT, "iosAppGroups", "APP_GROUPS_CAPABILITY",
                                     {});
        convertOnOffSettingIfDefined(exporterVT, "iCloudPermissions", "ICLOUD_PERMISSIONS",
                                     {});
      }

      if (isXcodeExporter)
      {
        convertOnOffSettingIfDefined(exporterVT, "iosPushNotifications",
                                     "PUSH_NOTIFICATIONS_CAPABILITY", {});

        convertSettingIfDefined(exporterVT, "customPList", "CUSTOM_PLIST", {});
        convertOnOffSettingIfDefined(exporterVT, "PListPreprocess", "PLIST_PREPROCESS", {});
        convertOnOffSettingIfDefined(exporterVT, "pListPreprocess", "PLIST_PREPROCESS", {});
        const auto convertPrefixHeader = [&jucerFile, &exporterVT](const juce::var& v) {
          const auto value = v.toString();

          if (value.isEmpty())
            return juce::String{};

          const auto jucerFileDir = jucerFile.getParentDirectory();
          const auto targetProjectDir =
            jucerFileDir.getChildFile(exporterVT.getProperty("targetFolder").toString());

          return targetProjectDir.getChildFile(value).getRelativePathFrom(jucerFileDir);
        };
        convertSettingIfDefined(exporterVT, "PListPrefixHeader", "PLIST_PREFIX_HEADER",
                                convertPrefixHeader);
        convertSettingIfDefined(exporterVT, "pListPrefixHeader", "PLIST_PREFIX_HEADER",
                                convertPrefixHeader);

        convertSettingAsListIfDefined(
          exporterVT, "extraFrameworks",
          jucerVersionAsTuple > Version{5, 3, 2} ? "EXTRA_SYSTEM_FRAMEWORKS"
                                                 : "EXTRA_FRAMEWORKS",
          [](const juce::var& v) {
            auto frameworks = juce::StringArray::fromTokens(v.toString(), ",;", "\"'");
            frameworks.trim();
            return frameworks;
          });
        convertSettingAsListIfDefined(exporterVT, "frameworkSearchPaths",
                                      "FRAMEWORK_SEARCH_PATHS", {});
        convertSettingAsListIfDefined(exporterVT, "extraCustomFrameworks",
                                      "EXTRA_CUSTOM_FRAMEWORKS", {});
        convertSettingAsListIfDefined(exporterVT, "embeddedFrameworks",
                                      "EMBEDDED_FRAMEWORKS", {});
        convertSettingAsListIfDefined(exporterVT, "xcodeSubprojects", "XCODE_SUBPROJECTS",
                                      {});
        convertSettingIfDefined(exporterVT, "prebuildCommand", "PREBUILD_SHELL_SCRIPT", {});
        convertSettingIfDefined(exporterVT, "postbuildCommand", "POSTBUILD_SHELL_SCRIPT",
                                {});
        convertSettingIfDefined(exporterVT, "bundleIdentifier",
                                "EXPORTER_BUNDLE_IDENTIFIER", {});
        convertSettingIfDefined(exporterVT, "iosDevelopmentTeamID", "DEVELOPMENT_TEAM_ID",
                                {});
      }

      if (exporterType == "XCODE_IPHONE")
      {
        convertSettingAsListIfDefined(
          exporterVT, "iosAppGroupsId", "APP_GROUP_ID", [](const juce::var& v) {
            auto groups = juce::StringArray::fromTokens(v.toString(), ";", {});
            groups.trim();
            return groups;
          });
      }

      if (isXcodeExporter)
      {
        convertOnOffSettingIfDefined(exporterVT, "keepCustomXcodeSchemes",
                                     "KEEP_CUSTOM_XCODE_SCHEMES", {});
        convertOnOffSettingIfDefined(exporterVT, "useHeaderMap", "USE_HEADERMAP", {});
      }

      if (isVSExporter)
      {
        convertSettingIfDefined(exporterVT, "msvcManifestFile", "MANIFEST_FILE", {});

        if (exporterVT.hasProperty("toolset"))
        {
          const auto toolset = exporterVT.getProperty("toolset").toString();
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
          exporterVT, "IPPLibrary", "USE_IPP_LIBRARY",
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

        convertSettingIfDefined(exporterVT, "windowsTargetPlatformVersion",
                                "WINDOWS_TARGET_PLATFORM", {});

        if (exporterType == "VS2017")
        {
          convertSettingIfDefined(exporterVT, "cppLanguageStandard", "CXX_STANDARD_TO_USE",
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
        convertSettingIfDefined(exporterVT, "cppLanguageStandard", "CXX_STANDARD_TO_USE",
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
          exporterVT, "linuxExtraPkgConfig", "PKGCONFIG_LIBRARIES", [](const juce::var& v) {
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

        convertSettingIfDefined(exporterVT, "codeBlocksWindowsTarget", "TARGET_PLATFORM",
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

      writeUserNotes(wLn, exporterVT);

      wLn(")");
      wLn();

      for (auto i = 0; i < configurationsVT.getNumChildren(); ++i)
      {
        const auto configurationVT = configurationsVT.getChild(i);

        wLn("jucer_export_target_configuration(");
        wLn("  \"", exporterName, "\"");
        wLn("  NAME \"", configurationVT.getProperty("name").toString(), "\"");

        const auto isDebug = bool{configurationVT.getProperty("isDebug")};
        wLn("  DEBUG_MODE ", (isDebug ? "ON" : "OFF"));

        convertSettingIfDefined(configurationVT, "targetName", "BINARY_NAME", {});
        convertSettingIfDefined(configurationVT, "binaryPath", "BINARY_LOCATION", {});

        const auto isAbsolutePath = [](const juce::String& path) {
          return path.startsWithChar('/') || path.startsWithChar('~')
                 || path.startsWithChar('$')
                 || (juce::CharacterFunctions::isLetter(path[0]) && path[1] == ':');
        };

        const auto jucerFileDir = jucerFile.getParentDirectory();
        const auto targetProjectDir =
          jucerFileDir.getChildFile(exporterVT.getProperty("targetFolder").toString());

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

        convertSettingAsListIfDefined(configurationVT, "headerPath", "HEADER_SEARCH_PATHS",
                                      convertSearchPaths);
        convertSettingAsListIfDefined(configurationVT, "libraryPath",
                                      "EXTRA_LIBRARY_SEARCH_PATHS", convertSearchPaths);

        convertSettingAsListIfDefined(
          configurationVT, "defines", "PREPROCESSOR_DEFINITIONS",
          [](const juce::var& v) { return parsePreprocessorDefinitions(v.toString()); });

        convertOnOffSettingIfDefined(configurationVT, "linkTimeOptimisation",
                                     "LINK_TIME_OPTIMISATION", {});

        if (!configurationVT.hasProperty("linkTimeOptimisation") && isVSExporter && !isDebug
            && jucerVersionAsTuple >= Version{5, 2, 0})
        {
          convertOnOffSettingIfDefined(configurationVT, "wholeProgramOptimisation",
                                       "LINK_TIME_OPTIMISATION", [](const juce::var& v) {
                                         if (int{v} == 0)
                                           return "ON";

                                         return "OFF";
                                       });
        }

        if (isXcodeExporter)
        {
          convertSettingIfDefined(configurationVT, "recommendedWarnings",
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
          convertSettingIfDefined(configurationVT, "recommendedWarnings",
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

        convertSettingIfDefined(configurationVT, "optimisation", "OPTIMISATION",
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
          convertOnOffSettingIfDefined(configurationVT, "enablePluginBinaryCopyStep",
                                       "ENABLE_PLUGIN_COPY_STEP", {});

          if (!vstIsLegacy)
          {
            if (configurationVT.hasProperty("xcodeVstBinaryLocation"))
            {
              convertSetting(configurationVT, "xcodeVstBinaryLocation",
                             "VST_BINARY_LOCATION", {});
            }
            else
            {
              convertSettingIfDefined(configurationVT, "vstBinaryLocation",
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

            if (configurationVT.hasProperty(oldProperty))
            {
              convertSetting(configurationVT, oldProperty, cmakeKeyword, {});
            }
            else
            {
              convertSettingIfDefined(configurationVT, newProperty, cmakeKeyword, {});
            }
          }

          convertSettingIfDefined(configurationVT, "unityPluginBinaryLocation",
                                  "UNITY_BINARY_LOCATION", {});
          if (vstIsLegacy)
          {
            convertSettingIfDefined(configurationVT, "vstBinaryLocation",
                                    "VST_LEGACY_BINARY_LOCATION", {});
          }
        }

        if (exporterType == "XCODE_IPHONE")
        {
          convertSettingIfDefined(configurationVT, "iosCompatibility",
                                  "IOS_DEPLOYMENT_TARGET", {});
        }

        if (exporterType == "XCODE_MAC")
        {
          const auto sdks = juce::StringArray{
            "10.5 SDK",  "10.6 SDK",  "10.7 SDK",  "10.8 SDK",  "10.9 SDK",
            "10.10 SDK", "10.11 SDK", "10.12 SDK", "10.13 SDK", "10.14 SDK",
          };

          convertSettingIfDefined(configurationVT, "osxSDK", "OSX_BASE_SDK_VERSION",
                                  [&sdks](const juce::var& v) -> juce::String {
                                    const auto value = v.toString();

                                    if (value == "default")
                                      return "Use Default";

                                    if (sdks.contains(value))
                                      return value;

                                    return {};
                                  });

          convertSettingIfDefined(configurationVT, "osxCompatibility",
                                  "OSX_DEPLOYMENT_TARGET",
                                  [&sdks](const juce::var& v) -> juce::String {
                                    const auto value = v.toString();

                                    if (value == "default")
                                      return "Use Default";

                                    if (sdks.contains(value))
                                      return value.substring(0, value.length() - 4);

                                    return {};
                                  });

          convertSettingIfDefined(configurationVT, "osxArchitecture", "OSX_ARCHITECTURE",
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
            configurationVT, "customXcodeFlags", "CUSTOM_XCODE_FLAGS",
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
            configurationVT, "plistPreprocessorDefinitions",
            "PLIST_PREPROCESSOR_DEFINITIONS", [](const juce::var& v) {
              return parsePreprocessorDefinitions(v.toString());
            });

          convertSettingIfDefined(configurationVT, "cppLanguageStandard",
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

          convertSettingIfDefined(configurationVT, "cppLibType", "CXX_LIBRARY",
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

          convertSettingIfDefined(configurationVT, "codeSigningIdentity",
                                  "CODE_SIGNING_IDENTITY", {});
          convertOnOffSettingIfDefined(configurationVT, "fastMath", "RELAX_IEEE_COMPLIANCE",
                                       {});
          convertOnOffSettingIfDefined(configurationVT, "stripLocalSymbols",
                                       "STRIP_LOCAL_SYMBOLS", {});
        }

        if (isVSExporter)
        {
          convertOnOffSettingIfDefined(configurationVT, "enablePluginBinaryCopyStep",
                                       "ENABLE_PLUGIN_COPY_STEP", {});

          if (!vstIsLegacy)
          {
            convertSettingIfDefined(configurationVT, "vstBinaryLocation",
                                    "VST_BINARY_LOCATION", {});
          }
          convertSettingIfDefined(configurationVT, "vst3BinaryLocation",
                                  "VST3_BINARY_LOCATION", {});
          convertSettingIfDefined(configurationVT, "rtasBinaryLocation",
                                  "RTAS_BINARY_LOCATION", {});
          convertSettingIfDefined(configurationVT, "aaxBinaryLocation",
                                  "AAX_BINARY_LOCATION", {});
          convertSettingIfDefined(configurationVT, "unityPluginBinaryLocation",
                                  "UNITY_BINARY_LOCATION", {});
          if (vstIsLegacy)
          {
            convertSettingIfDefined(configurationVT, "vstBinaryLocation",
                                    "VST_LEGACY_BINARY_LOCATION", {});
          }

          convertSettingIfDefined(configurationVT, "winWarningLevel", "WARNING_LEVEL",
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

          convertOnOffSettingIfDefined(configurationVT, "warningsAreErrors",
                                       "TREAT_WARNINGS_AS_ERRORS", {});

          convertSettingIfDefined(configurationVT, "useRuntimeLibDLL", "RUNTIME_LIBRARY",
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
            convertSettingIfDefined(configurationVT, "wholeProgramOptimisation",
                                    "WHOLE_PROGRAM_OPTIMISATION",
                                    [](const juce::var& v) -> juce::String {
                                      if (v.toString().isEmpty())
                                        return "Enable when possible";

                                      if (int{v} > 0)
                                        return "Always disable";

                                      return {};
                                    });
          }

          convertOnOffSettingIfDefined(configurationVT, "multiProcessorCompilation",
                                       "MULTI_PROCESSOR_COMPILATION", {});
          convertOnOffSettingIfDefined(configurationVT, "enableIncrementalLinking",
                                       "INCREMENTAL_LINKING", {});

          if (!isDebug)
          {
            convertOnOffSettingIfDefined(configurationVT, "alwaysGenerateDebugSymbols",
                                         "FORCE_GENERATION_OF_DEBUG_SYMBOLS", {});
          }

          convertSettingIfDefined(configurationVT, "prebuildCommand", "PREBUILD_COMMAND",
                                  {});
          convertSettingIfDefined(configurationVT, "postbuildCommand", "POSTBUILD_COMMAND",
                                  {});
          convertOnOffSettingIfDefined(configurationVT, "generateManifest",
                                       "GENERATE_MANIFEST", {});

          convertSettingIfDefined(configurationVT, "characterSet", "CHARACTER_SET",
                                  [](const juce::var& v) -> juce::String {
                                    const auto value = v.toString();

                                    if (value.isEmpty())
                                      return "Default";

                                    return value;
                                  });

          if (configurationVT.hasProperty("winArchitecture"))
          {
            const auto winArchitecture =
              configurationVT.getProperty("winArchitecture").toString();
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
            configurationVT, "debugInformationFormat", "DEBUG_INFORMATION_FORMAT",
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

          convertOnOffSettingIfDefined(configurationVT, "fastMath", "RELAX_IEEE_COMPLIANCE",
                                       {});
        }

        if (exporterType == "LINUX_MAKE")
        {
          convertSettingIfDefined(configurationVT, "linuxArchitecture", "ARCHITECTURE",
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
          if (configurationVT.hasProperty("windowsCodeBlocksArchitecture")
              || jucerVersionAsTuple >= Version{5, 0, 0})
          {
            convertSetting(configurationVT, "windowsCodeBlocksArchitecture", "ARCHITECTURE",
                           codeBlocksArchitecture);
          }
        }

        if (exporterType == "CODEBLOCKS_LINUX")
        {
          if (configurationVT.hasProperty("linuxCodeBlocksArchitecture")
              || jucerVersionAsTuple >= Version{5, 0, 0})
          {
            convertSetting(configurationVT, "linuxCodeBlocksArchitecture", "ARCHITECTURE",
                           codeBlocksArchitecture);
          }
        }

        writeUserNotes(wLn, configurationVT);

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
