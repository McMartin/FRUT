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

#include "juce_core.hpp"

#include <iostream>
#include <utility>


namespace Jucer2CMake
{

struct Arguments
{
  juce::String mode;
  juce::String jucerFilePath;
  juce::String reprojucerFilePath;
  juce::String juceModulesPath;
  juce::String userModulesPath;
  juce::File outputDir;
  bool relocatable;
};


#if !defined(WRITE_CRLF_LINE_ENDINGS)
  #error WRITE_CRLF_LINE_ENDINGS must be defined
#endif
#if WRITE_CRLF_LINE_ENDINGS
static const auto kNewLine = "\r\n";
#else
static const auto kNewLine = '\n';
#endif


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


inline juce::StringArray
convertIdsToStrings(const juce::StringArray& ids,
                    const std::vector<std::pair<juce::String, const char*>>& idsToStrings)
{
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


inline juce::File
getChildFileFromWorkingDirectory(const juce::StringRef relativeOrAbsolutePath)
{
  return juce::File::getCurrentWorkingDirectory().getChildFile(relativeOrAbsolutePath);
}


inline void printError(const juce::String& error)
{
  std::cerr << "error: " << error << std::endl;
}

} // namespace Jucer2CMake
