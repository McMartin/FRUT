// Copyright (c) 2017 Alain Martin, Matthieu Talbot
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

#include "JuceHeader.h"

using juce::Identifier;

#include <jucer_PresetIDs.h>

#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>


void printError(const std::string& error)
{
  std::cerr << "error: " << error << std::endl;
}


std::string makeValidIdentifier(std::string s)
{
  jassert(!s.empty());
  const std::string allowedChars(
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789");
  for (auto& c : s)
  {
    if (allowedChars.find(c) == std::string::npos)
    {
      c = '_';
    }
  }
  return s;
}


int main(int argc, char* argv[])
{
  // Test initial conditions
  if (argc != 3)
  {
    std::cerr << "usage: Jucer2CMake <jucer_project_file> "
                 "<reproducer_library_path>"
              << std::endl;
    return 1;
  }

  // Parse command line arguments
  std::vector<std::string> args{argv, argv + argc};
  const auto& jucerFilePath = args.at(1);
  const auto& reproducerLibraryPath = args.at(2);

  const auto currentWorkingDirectory =
    juce::File::getCurrentWorkingDirectory().getFullPathName().toStdString();
  const auto cmakeProjectJucerPrefix = makeValidIdentifier(
    juce::File(jucerFilePath).getFileNameWithoutExtension().toStdString() + "_jucer");

  // Load jucer project
  auto file = juce::File{jucerFilePath};
  std::unique_ptr<juce::XmlElement> xml;
  xml.reset(juce::XmlDocument::parse(file));
  if (xml == nullptr || !xml->hasTagName(Ids::JUCERPROJECT.toString()))
  {
    printError(jucerFilePath + " is not a valid Jucer project.");
    return 1;
  }
  auto jucerProject = juce::ValueTree::fromXml(*xml);
  if (!jucerProject.hasType(Ids::JUCERPROJECT))
  {
    printError(jucerFilePath + " is not a valid Jucer project.");
    return 1;
  }

  // Replace values that differ from dot jucer format and dot cmake format.
  std::vector<std::tuple<juce::Identifier, juce::var, juce::var>>
    jucer2cmakeValuesTranslate;
  jucer2cmakeValuesTranslate.emplace_back(
    Ids::projectType, juce::var("guiapp"), juce::var("GUI Application"));
  for (auto& replaceTuple : jucer2cmakeValuesTranslate)
  {
    auto& id = std::get<0>(replaceTuple);
    auto& valueFrom = std::get<1>(replaceTuple);
    auto& valueTo = std::get<2>(replaceTuple);
    if (jucerProject.getProperty(id) == valueFrom)
    {
      jucerProject.setProperty(id, valueTo, nullptr);
    }
  }

  // Open out stream to the CMakeLists file
  std::ofstream out{"CMakeLists.txt"};

  out << "cmake_minimum_required(VERSION 3.4)\n"
      << "\n"
      << "\n"
      << "list(APPEND CMAKE_MODULE_PATH \""
      << "${CMAKE_CURRENT_LIST_DIR}/"
      << juce::File(reproducerLibraryPath)
           .getRelativePathFrom(juce::File(currentWorkingDirectory))
           .replace("\\", "/")
           .toStdString()
      << "\")\n"
      << "include(Reprojucer)\n"
      << "\n"
      << "\n"
      << "if(NOT DEFINED " << cmakeProjectJucerPrefix << "_FILE)\n"
      << "  message(FATAL_ERROR \"" << cmakeProjectJucerPrefix
      << "_FILE must be defined\")\n"
      << "endif()\n"
      << "\n"
      << "get_filename_component(" << cmakeProjectJucerPrefix << "_FILE\n"
      << "  \"${" << cmakeProjectJucerPrefix << "_FILE}\" ABSOLUTE\n"
      << "  BASE_DIR \"${CMAKE_CURRENT_BINARY_DIR}\"\n"
      << ")\n"
      << "\n"
      << "if(NOT EXISTS \"${" << cmakeProjectJucerPrefix << "_FILE}\")\n"
      << "  message(FATAL_ERROR \"No such file: ${" << cmakeProjectJucerPrefix
      << "_FILE}\")\n"
      << "endif()\n"
      << "\n"
      << "get_filename_component(" << cmakeProjectJucerPrefix << "_DIR\n"
      << "  \"${" << cmakeProjectJucerPrefix << "_FILE}\" DIRECTORY\n"
      << ")\n"
      << "\n"
      << "\n"
      << "jucer_project_begin(\n";

  juce::ValueTree jucerProjectDefaults(jucerProject.getType());
  jucerProjectDefaults.setProperty(Ids::maxBinaryFileSize, "Default", nullptr);

  // Replace keys that differ from dot jucer format and dot cmake format.
  std::vector<std::pair<std::string, juce::Identifier>> jucer2cmakeKeysTranslate = {
    {"PROJECT_NAME", Ids::name}, {"PROJECT_VERSION", Ids::version},
    {"COMPANY_NAME", Ids::companyName}, {"COMPANY_WEBSITE", Ids::companyWebsite},
    {"COMPANY_EMAIL", Ids::companyEmail}, {"PROJECT_TYPE", Ids::projectType},
    {"BUNDLE_IDENTIFIER", Ids::bundleIdentifier},
    {"BINARYDATACPP_SIZE_LIMIT", Ids::maxBinaryFileSize},
    {"BINARYDATA_NAMESPACE", Ids::binaryDataNamespace},
    {"PREPROCESSOR_DEFINITIONS", Ids::defines}, // i'm not sure about this one
    {"PROJECT_ID", Ids::ID},
  };
  for (auto& p : jucer2cmakeKeysTranslate)
  {
    if (jucerProject.hasProperty(p.second))
    {
      out << "  " << p.first << " \""
          << jucerProject.getProperty(p.second).toString().toStdString() << "\"\n";
      continue;
    }
    if (jucerProjectDefaults.hasProperty(p.second))
    {
      out << "  " << p.first << " \""
          << jucerProjectDefaults.getProperty(p.second).toString().toStdString()
          << "\"\n";
      continue;
    }
    out << "  # " << p.first << "\n";
  }
  out << ")\n";
  out << "\n";

  const auto& jucerProjectNumChildren = jucerProject.getNumChildren();
  for (auto i = 0; i < jucerProjectNumChildren; ++i)
  {
    auto maingroup = jucerProject.getChild(i);
    if (maingroup.hasType(Ids::MAINGROUP))
    {
      const auto maingroupName =
        maingroup.getProperty(Ids::name).toString().toStdString();
      const auto& maingroupNumChildren = maingroup.getNumChildren();
      for (auto j = 0; j < maingroupNumChildren; ++j)
      {
        auto group = maingroup.getChild(j);
        assert(group.hasType(Ids::GROUP));
        const auto groupName = group.getProperty(Ids::name).toString().toStdString();
        const auto& groupNumChildren = group.getNumChildren();
        out << "jucer_project_files(\"" << (maingroupName + "/" + groupName) << "\"\n";
        for (auto k = 0; k < groupNumChildren; ++k)
        {
          auto file = group.getChild(k);
          assert(file.hasType(Ids::FILE));
          if (file.getProperty(Ids::resource) == juce::var("0"))
          {
            out << "  \"${" << cmakeProjectJucerPrefix << "_DIR"
                << "}/" << file.getProperty(Ids::file).toString() << "\"\n";
          }
        }
        out << ")\n";
      }
    }
  }
  out << "\n";

  std::vector<std::string> moduleList;
  {
    auto child = jucerProject.getChildWithName(Ids::MODULES);
    for (auto i = 0; i < child.getNumChildren(); ++i)
    {
      moduleList.push_back(
        child.getChild(i).getProperty(Ids::ID).toString().toStdString());
    }
  }
  auto modulesPaths = jucerProject.getChildWithName(Ids::EXPORTFORMATS)
                        .getChild(1)
                        .getChildWithName(Ids::MODULEPATHS);
  for (auto& module : moduleList)
  {
    const auto relativeModulePath =
      modulesPaths.getChildWithProperty(Ids::ID, juce::var(juce::String(module)))
        .getProperty(Ids::path)
        .toString();
    const auto modulePath = juce::File(jucerFilePath)
                              .getParentDirectory()
                              .getChildFile(relativeModulePath)
                              .getChildFile(juce::StringRef(module));
    const auto moduleHeader = modulePath.getChildFile(juce::StringRef{module + ".h"});
    juce::StringArray moduleLines, moduleOptions;
    moduleHeader.readLines(moduleLines);
    for (const auto& line : moduleLines)
    {
      if (line.startsWith("/** Config: "))
      {
        auto option = line.substring(12);
        moduleOptions.add(option);
      }
    }
    out << "jucer_project_module(\n"
        << "  " << module << "\n"
        << "  PATH \"${" << cmakeProjectJucerPrefix << "_DIR"
        << "}/" << relativeModulePath << "\"\n";
    for (const auto& option : moduleOptions)
    {
      out << "  # " << option << "\n";
    }
    out << ")\n\n";
  }

  out << "jucer_project_end()" << std::endl;

  return 0;
}
