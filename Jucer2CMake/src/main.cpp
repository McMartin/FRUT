// Copyright (C) 2017-2021  Alain Martin
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

#include "argh.hpp"
#include "juce6.hpp"
#include "juce_core.hpp"
#include "reprojucer.hpp"
#include "utils.hpp"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <memory>
#include <utility>


using namespace Jucer2CMake;


namespace
{

Arguments parseArguments(const int argc, const char* const argv[])
{
  const auto knownModes = juce::StringArray{"juce6", "reprojucer"};

  const auto knownFlags = std::map<juce::String, juce::StringArray>{
    {"juce6", {"h", "help"}},
    {"reprojucer", {"h", "help", "relocatable"}},
  };

  const auto knownParams = std::map<juce::String, juce::StringArray>{
    {"juce6", {}},
    {"reprojucer", {"juce-modules", "user-modules"}},
  };

  argh::parser argumentParser;
  for (const auto& modeAndParams : knownParams)
  {
    for (const auto& param : modeAndParams.second)
    {
      argumentParser.add_param(param.toStdString());
    }
  }
  argumentParser.parse(argc, argv);

  const auto askingForHelp = argumentParser[{"-h", "--help"}];
  auto errorInArguments = false;

  auto mode = juce::String{argumentParser[1]};

  if (!askingForHelp)
  {
    if (argumentParser.size() >= 2 && !knownModes.contains(mode))
    {
      printError("invalid mode \"" + mode + "\"");
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

  if (mode.isNotEmpty())
  {
    for (const auto& flag : argumentParser.flags())
    {
      if (knownParams.at(mode).contains(juce::String{flag}))
      {
        printError("expected one argument for \"" + flag + "\"");
        errorInArguments = true;
      }
      else if (!knownFlags.at(mode).contains(juce::String{flag}))
      {
        printError("unknown option \"" + flag + "\"");
        errorInArguments = true;
      }
    }

    for (const auto& paramAndValue : argumentParser.params())
    {
      const auto& param = std::get<0>(paramAndValue);
      if (knownFlags.at(mode).contains(juce::String{param}))
      {
        const auto& value = std::get<1>(paramAndValue);
        printError("unexpected argument \"" + value + "\" for \"" + param + "\"");
        errorInArguments = true;
      }
      else if (!knownParams.at(mode).contains(juce::String{param}))
      {
        printError("unknown option \"" + param + "\"");
        errorInArguments = true;
      }
    }
  }

  const auto noModeUsage =
    askingForHelp ? "usage: Jucer2CMake <mode> <jucer_project_file> [--help] [<args>]\n"
                  : "usage: Jucer2CMake {juce6,reprojucer} <jucer_project_file> "
                    "[--help] [<args>]\n";
  const auto noModeHelpText =
    "\n"
    "Converts a .jucer file into a CMakeLists.txt file.\n"
    "The CMakeLists.txt file is written in the current working directory.\n"
    "\n"
    "    <mode>                    what the generated CMakeLists.txt uses:\n"
    "      juce6                     - JUCE 6's CMake support\n"
    "      reprojucer                - FRUT's Reprojucer\n"
    "\n"
    "    <jucer_project_file>      path to the .jucer file to convert\n"
    "\n"
    "    -h, --help                show this help message and exit\n";

  const auto juce6Usage = "usage: Jucer2CMake juce6 <jucer_project_file> [--help]\n";
  const auto juce6HelpText =
    "\n"
    "Converts a .jucer file into a CMakeLists.txt file that uses JUCE 6's CMake\n"
    "support.\n"
    "The CMakeLists.txt file is written in the current working directory.\n"
    "\n"
    "    <jucer_project_file>      path to the .jucer file to convert\n"
    "\n"
    "    -h, --help                show this help message and exit\n";

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

  const auto usage = std::map<juce::String, const char*>{
    {"", noModeUsage}, {"juce6", juce6Usage}, {"reprojucer", reprojucerUsage}};
  const auto helpText = std::map<juce::String, const char*>{
    {"", noModeHelpText}, {"juce6", juce6HelpText}, {"reprojucer", reprojucerHelpText}};

  if (askingForHelp || errorInArguments)
  {
    std::cerr << usage.at(mode) << std::flush;

    if (askingForHelp)
    {
      std::cerr << helpText.at(mode) << std::flush;
    }

    std::exit(askingForHelp ? 0 : 1);
  }

  const auto existingFilePath = [&argumentParser](juce::StringRef name, size_t index) {
    if (argumentParser.size() > index)
    {
      auto path = juce::String{argumentParser[index]};
      if (path.isEmpty() || !getChildFileFromWorkingDirectory(path).existsAsFile())
      {
        printError("No such file (" + name + "): '" + path + "'");
        std::exit(1);
      }
      return path;
    }
    return juce::String{};
  };

  auto jucerFilePath = existingFilePath("<jucer_project_file>", 2);
  auto reprojucerFilePath = existingFilePath("<Reprojucer.cmake_file>", 3);

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

  return {std::move(mode),
          std::move(jucerFilePath),
          std::move(reprojucerFilePath),
          std::move(juceModulesPath),
          std::move(userModulesPath),
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

  juce::MemoryOutputStream outputStream;

  outputStream << "# This file was generated by FRUT's Jucer2CMake from \""
               << jucerFile.getFileName() << "\"\n";

  if (args.mode == "juce6")
  {
    writeJuce6CMakeLists(args, jucerProject, outputStream);
  }
  else if (args.mode == "reprojucer")
  {
    writeReprojucerCMakeLists(args, jucerProject, outputStream);
  }
  else
  {
    printError("There is a bug in Jucer2CMake's CLI!");
    return 1;
  }

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
