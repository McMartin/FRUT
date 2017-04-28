// Copyright (c) 2016 Alain Martin
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

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

  class Item
  {
  public:
    bool isGroup() const
    {
      return false;
    }

    int getNumChildren() const
    {
      return 0;
    }

    Item getChild(int) const
    {
      return {};
    }

    bool shouldBeAddedToBinaryResources() const
    {
      return false;
    }

    File getFile() const
    {
      return {};
    }
  };

  Item getMainGroup()
  {
    return {};
  }

private:
  const File binaryDataFilesOuputDir;
};
