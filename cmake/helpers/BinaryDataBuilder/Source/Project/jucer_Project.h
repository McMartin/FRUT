// Copyright (c) 2016 Alain Martin
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

#pragma once


class Project
{

public:
  Project(String outputDir)
    : binaryDataFilesOuputDir{outputDir}
  {
  }

  String getProjectUID() const
  {
    return {"JUCE.cmake"};
  }

  File getBinaryDataCppFile(int index) const
  {
    if (index > 0)
    {
      return binaryDataFilesOuputDir.getChildFile(
        "BinaryData" + String{index + 1} + ".cpp");
    }

    return binaryDataFilesOuputDir.getChildFile("BinaryData.cpp");
  }

  File getBinaryDataHeaderFile() const
  {
    return binaryDataFilesOuputDir.getChildFile("BinaryData.h");
  }

private:
  const File binaryDataFilesOuputDir;
};
