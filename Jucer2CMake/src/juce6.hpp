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

  const auto writeProjectSettingIfDefined =
    [&jucerProject, &wLn](juce::StringRef attribute, juce::StringRef keyword) {
      if (jucerProject.hasAttribute(attribute))
      {
        wLn("  ", keyword, " \"", jucerProject.getStringAttribute(attribute), "\"");
      }
    };

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

    // TODO: PRODUCT_NAME

    wLn("  VERSION \"" + jucerProject.getStringAttribute("version", "1.0.0") + "\"");

    writeProjectSettingIfDefined("bundleIdentifier", "BUNDLE_ID");

    writeProjectSettingIfDefined("companyName", "COMPANY_NAME");
    writeProjectSettingIfDefined("companyCopyright", "COMPANY_COPYRIGHT");
    writeProjectSettingIfDefined("companyWebsite", "COMPANY_WEBSITE");
    writeProjectSettingIfDefined("companyEmail", "COMPANY_EMAIL");

    if (projectType == "audioplug")
    {
      wLn();
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
      writeProjectSettingIfDefined("pluginDesc", "DESCRIPTION");
      writeProjectSettingIfDefined("pluginManufacturerCode", "PLUGIN_MANUFACTURER_CODE");
      writeProjectSettingIfDefined("pluginCode", "PLUGIN_CODE");

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

      writeProjectSettingIfDefined("aaxIdentifier", "AAX_IDENTIFIER");
      writeProjectSettingIfDefined("pluginAUExportPrefix", "AU_EXPORT_PREFIX");

      if (jucerProject.hasAttribute("pluginAUMainType"))
      {
        wLn("  AU_MAIN_TYPE \"",
            getAUMainTypeConstantFromQuotedFourChars(
              jucerProject.getStringAttribute("pluginAUMainType")),
            "\"");
      }

      if (jucerProject.hasAttribute("pluginAUIsSandboxSafe"))
      {
        wLn("  AU_SANDBOX_SAFE ",
            toBoolLikeVar(jucerProject.getStringAttribute("pluginAUIsSandboxSafe"))
              ? "TRUE"
              : "FALSE");
      }

      writeProjectSettingIfDefined("pluginVSTNumMidiInputs", "VST_NUM_MIDI_INS");
      writeProjectSettingIfDefined("pluginVSTNumMidiOutputs", "VST_NUM_MIDI_OUTS");

      if (jucerProject.hasAttribute("pluginVST3Category"))
      {
        const auto vst3Categories = juce::StringArray::fromTokens(
          jucerProject.getStringAttribute("pluginVST3Category"), ",", {});

        if (vst3Categories.isEmpty())
        {
          wLn("  # VST3_CATEGORIES");
        }
        else
        {
          wLn("  VST3_CATEGORIES \"", vst3Categories.joinIntoString("\" \""), "\"");
        }
      }

      // TODO: AAX_CATEGORY

      writeProjectSettingIfDefined("pluginVSTCategory", "VST2_CATEGORY");

      // TODO: COPY_PLUGIN_AFTER_BUILD
      // TODO: VST3_COPY_DIR
      // TODO: AU_COPY_DIR
      // TODO: AAX_COPY_DIR
      // TODO: UNITY_COPY_DIR
      // TODO: VST_COPY_DIR
    }

    // TODO: ICON_SMALL
    // TODO: ICON_BIG
    // TODO: CUSTOM_XCASSETS_FOLDER
    // TODO: LAUNCH_STORYBOARD_FILE

    // TODO: IPHONE_SCREEN_ORIENTATIONS
    // TODO: IPAD_SCREEN_ORIENTATIONS
    // TODO: FILE_SHARING_ENABLED
    // TODO: DOCUMENT_BROWSER_ENABLED
    // TODO: STATUS_BAR_HIDDEN
    // TODO: DOCUMENT_EXTENSIONS
    // TODO: APP_SANDBOX_ENABLED
    // TODO: APP_SANDBOX_INHERIT
    // TODO: APP_SANDBOX_OPTIONS
    // TODO: HARDENED_RUNTIME_ENABLED
    // TODO: HARDENED_RUNTIME_OPTIONS
    // TODO: MICROPHONE_PERMISSION_ENABLED
    // TODO: MICROPHONE_PERMISSION_TEXT
    // TODO: CAMERA_PERMISSION_ENABLED
    // TODO: CAMERA_PERMISSION_TEXT
    // TODO: BLUETOOTH_PERMISSION_ENABLED
    // TODO: BLUETOOTH_PERMISSION_TEXT
    // TODO: SEND_APPLE_EVENTS_PERMISSION_ENABLED
    // TODO: SEND_APPLE_EVENTS_PERMISSION_TEXT
    // TODO: BACKGROUND_AUDIO_ENABLED
    // TODO: BACKGROUND_BLE_ENABLED
    // TODO: APP_GROUPS_ENABLED
    // TODO: APP_GROUP_IDS
    // TODO: ICLOUD_PERMISSIONS_ENABLED
    // TODO: PUSH_NOTIFICATIONS_ENABLED
    // TODO: SUPPRESS_AU_PLIST_RESOURCE_USAGE
    // TODO: PLIST_TO_MERGE

    // TODO: NEEDS_CURL
    // TODO: NEEDS_STORE_KIT
    // TODO: NEEDS_WEB_BROWSER
    // TODO: PLUGINHOST_AU

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

  juce::StringArray sources;
  juce::StringArray binarySources;

  std::function<void(const juce::XmlElement&)> processGroup =
    [&processGroup, &sources, &binarySources](const juce::XmlElement& group) {
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
          if (file.getStringAttribute("resource").getIntValue() == 1)
          {
            binarySources.add(file.getStringAttribute("file"));
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
  binarySources.sort(kIgnoreCase);

  // target_sources
  {
    wLn("target_sources(", targetName);
    wLn("  PRIVATE");

    for (const auto& item : sources)
    {
      wLn("    \"", item, "\"");
    }

    wLn(")");
    wLn();
  }

  // juce_add_binary_data
  if (!binarySources.isEmpty())
  {
    wLn("juce_add_binary_data(", targetName, "_BinaryData");

    writeProjectSettingIfDefined("binaryDataNamespace", "NAMESPACE");

    wLn("  SOURCES");
    for (const auto& item : binarySources)
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

    if (!binarySources.isEmpty())
    {
      wLn("    ", targetName, "_BinaryData");
    }

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
