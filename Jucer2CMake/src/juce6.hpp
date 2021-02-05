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


inline bool hasModule(const juce::XmlElement& modules, const juce::StringRef moduleId)
{
  for (auto pModule = modules.getFirstChildElement(); pModule != nullptr;
       pModule = pModule->getNextElement())
  {
    if (pModule->isTextElement())
    {
      continue;
    }

    if (pModule->getStringAttribute("id") == moduleId)
    {
      return true;
    }
  }

  return false;
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

    wLn("  VERSION \"" + jucerProject.getStringAttribute("version", "1.0.0") + "\"");

    const auto writeProjectSettingIfDefined =
      [&jucerProject, &wLn](juce::StringRef attribute, juce::StringRef keyword) {
        if (jucerProject.hasAttribute(attribute))
        {
          wLn("  ", keyword, " \"", jucerProject.getStringAttribute(attribute), "\"");
        }
      };

    writeProjectSettingIfDefined("bundleIdentifier", "BUNDLE_ID");

    writeProjectSettingIfDefined("companyName", "COMPANY_NAME");
    writeProjectSettingIfDefined("companyCopyright", "COMPANY_COPYRIGHT");
    writeProjectSettingIfDefined("companyWebsite", "COMPANY_WEBSITE");
    writeProjectSettingIfDefined("companyEmail", "COMPANY_EMAIL");

    if (projectType == "audioplug")
    {
      const auto formats =
        jucerProject.hasAttribute("pluginFormats")
          ? convertIdsToStrings(
            juce::StringArray::fromTokens(
              jucerProject.getStringAttribute("pluginFormats"), ",", {}),
            {{"buildVST3", "VST3"},
             {"buildAU", "AU"},
             {"buildAUv3", "AUv3"},
             {"buildAAX", "AAX"},
             {"buildStandalone", "Standalone"},
             {"buildUnity", "Unity"},
             {"buildVST", "VST"}})
          : juce::StringArray{"VST3", "AU", "Standalone"};

      if (formats.isEmpty())
      {
        wLn("  # FORMATS");
      }
      else
      {
        wLn("  FORMATS \"", formats.joinIntoString("\" \""), "\"");
      }

      writeProjectSettingIfDefined("pluginName", "PLUGIN_NAME");
      writeProjectSettingIfDefined("pluginManufacturerCode", "PLUGIN_MANUFACTURER_CODE");
      writeProjectSettingIfDefined("pluginCode", "PLUGIN_CODE");
      writeProjectSettingIfDefined("pluginDesc", "DESCRIPTION");

      const auto characteristics = juce::StringArray::fromTokens(
        jucerProject.getStringAttribute("pluginCharacteristicsValue"), ",", {});
      if (characteristics.contains("pluginIsSynth"))
      {
        wLn("  IS_SYNTH TRUE");
      }
      if (characteristics.contains("pluginWantsMidiIn"))
      {
        wLn("  NEEDS_MIDI_INPUT TRUE");
      }
      if (characteristics.contains("pluginProducesMidiOut"))
      {
        wLn("  NEEDS_MIDI_OUTPUT TRUE");
      }
      if (characteristics.contains("pluginIsMidiEffectPlugin"))
      {
        wLn("  IS_MIDI_EFFECT TRUE");
      }
      if (characteristics.contains("pluginEditorRequiresKeys"))
      {
        wLn("  EDITOR_WANTS_KEYBOARD_FOCUS TRUE");
      }
      if (characteristics.contains("pluginAAXDisableBypass"))
      {
        wLn("  DISABLE_AAX_BYPASS TRUE");
      }
      if (characteristics.contains("pluginAAXDisableMultiMono"))
      {
        wLn("  DISABLE_AAX_MULTI_MONO TRUE");
      }
    }

    wLn(")");
    wLn();
  }

  // juce_generate_juce_header
  {
    wLn("juce_generate_juce_header(", targetName, ")");
    wLn();
  }

  const auto kIgnoreCase = false;

  // target_compile_definitions
  {
    const auto scope = projectType == "audioplug" ? "PUBLIC" : "PRIVATE";
    wLn("target_compile_definitions(", targetName);
    wLn("  ", scope);

    juce::StringArray compileDefinitions;

    const auto& juceOptions = getRequiredChild(jucerProject, "JUCEOPTIONS");
    for (auto i = 0, numAttributes = juceOptions.getNumAttributes(); i < numAttributes;
         ++i)
    {
      compileDefinitions.add(juceOptions.getAttributeName(i) + "="
                             + juceOptions.getAttributeValue(i));
    }

    const auto& modules = getRequiredChild(jucerProject, "MODULES");
    if (hasModule(modules, "juce_core"))
    {
      compileDefinitions.add("JUCE_USE_CURL=0");
    }
    if (hasModule(modules, "juce_gui_extra"))
    {
      compileDefinitions.add("JUCE_WEB_BROWSER=0");
    }

    compileDefinitions.sort(kIgnoreCase);

    for (const auto& item : compileDefinitions)
    {
      wLn("    ", item);
    }

    wLn(")");
    wLn();
  }

  // target_sources
  {
    wLn("target_sources(", targetName);
    wLn("  PRIVATE");

    juce::StringArray sources;

    std::function<void(const juce::XmlElement&)> processGroup =
      [&processGroup, &sources](const juce::XmlElement& group) {
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
              sources.add(file.getStringAttribute("file"));
            }
          }
          else
          {
            processGroup(*pFileOrGroup);
          }
        }
      };

    processGroup(getRequiredChild(jucerProject, "MAINGROUP"));

    sources.sort(kIgnoreCase);

    for (const auto& item : sources)
    {
      wLn("    \"", item, "\"");
    }

    wLn(")");
    wLn();
  }

  // target_link_libraries
  {
    wLn("target_link_libraries(", targetName);
    wLn("  PRIVATE");

    const auto& modules = getRequiredChild(jucerProject, "MODULES");
    for (auto pModule = modules.getFirstChildElement(); pModule != nullptr;
         pModule = pModule->getNextElement())
    {
      if (pModule->isTextElement())
      {
        continue;
      }

      wLn("    juce::", pModule->getStringAttribute("id"));
    }

    wLn("  PUBLIC");
    wLn("    juce::juce_recommended_config_flags");
    wLn("    juce::juce_recommended_lto_flags");
    wLn("    juce::juce_recommended_warning_flags");

    wLn(")");
  }
}

} // namespace Jucer2CMake
