// Copyright (C) 2021  Alain Martin
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

#pragma once

#include "utils.hpp"

#include "juce_core.hpp"

#include <cstdlib>
#include <functional>


namespace Jucer2CMake
{

inline const juce::XmlElement& getRequiredChild(const juce::XmlElement& element,
                                                const juce::StringRef childName)
{
  if (const auto pChild = element.getChildByName(childName))
  {
    return *pChild;
  }

  printError(element.getTagName() + " element doesn't have " + childName
             + " child element.");
  std::exit(1);
}


inline void writeJuce6CMakeLists(const Arguments&, const juce::XmlElement& jucerProject,
                                 juce::MemoryOutputStream& outputStream)
{
  LineWriter wLn{outputStream};

  const auto& projectType = jucerProject.getStringAttribute("projectType");
  const auto& jucerProjectName = jucerProject.getStringAttribute("name");

  // Preamble
  {
    const auto cmakeVersion = projectType == "audioplug" ? "3.15" : "3.12";

    wLn();
    wLn("cmake_minimum_required(VERSION ", cmakeVersion, ")");
    wLn();
    wLn("project(\"", jucerProjectName, "\")");
    wLn();
    wLn();
    wLn("find_package(JUCE CONFIG REQUIRED)");
    wLn();
    wLn();
  }

  const auto& targetName = jucerProjectName;

  // juce_add_{console_app,gui_app,plugin}
  {
    const auto juceAddFunction = [&projectType]() -> juce::String {
      if (projectType == "guiapp")
        return "juce_add_gui_app";
      if (projectType == "consoleapp")
        return "juce_add_console_app";
      if (projectType == "audioplug")
        return "juce_add_plugin";
      return {};
    }();

    wLn(juceAddFunction, "(", targetName);

    wLn("  VERSION \"1.0.0\"");

    if (projectType == "audioplug")
    {
      wLn("  FORMATS \"AU\" \"VST3\" \"Standalone\"");
    }

    wLn(")");
    wLn();
  }

  // juce_generate_juce_header
  {
    wLn("juce_generate_juce_header(", targetName, ")");
    wLn();
  }

  // target_compile_definitions
  {
    const auto scope = projectType == "audioplug" ? "PUBLIC" : "PRIVATE";
    wLn("target_compile_definitions(", targetName, " ", scope);

    const auto& juceOptions = getRequiredChild(jucerProject, "JUCEOPTIONS");
    for (auto i = 0, numAttributes = juceOptions.getNumAttributes(); i < numAttributes;
         ++i)
    {
      wLn("  ", juceOptions.getAttributeName(i), "=", juceOptions.getAttributeValue(i));
    }

    wLn(")");
    wLn();
  }

  // target_sources
  {
    wLn("target_sources(", targetName, " PRIVATE");

    std::function<void(const juce::XmlElement&)> processGroup =
      [&processGroup, &wLn](const juce::XmlElement& group) {
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

            if (file.getStringAttribute("compile").getIntValue() == 1)
            {
              wLn("  \"", file.getStringAttribute("file"), "\"");
            }
          }
          else
          {
            processGroup(*pFileOrGroup);
          }
        }
      };

    processGroup(getRequiredChild(jucerProject, "MAINGROUP"));

    wLn(")");
    wLn();
  }

  // target_link_libraries
  {
    wLn("target_link_libraries(", targetName, " PRIVATE");

    const auto& modules = getRequiredChild(jucerProject, "MODULES");
    for (auto pModule = modules.getFirstChildElement(); pModule != nullptr;
         pModule = pModule->getNextElement())
    {
      if (pModule->isTextElement())
      {
        continue;
      }

      wLn("  juce::", pModule->getStringAttribute("id"));
    }

    wLn(")");
  }
}

} // namespace Jucer2CMake
