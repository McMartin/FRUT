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


namespace Jucer2CMake
{

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
  }
}

} // namespace Jucer2CMake
