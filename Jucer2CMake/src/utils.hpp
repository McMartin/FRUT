// Copyright (C) 2021-2022  Alain Martin
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
  juce::String jucerVersion;
  juce::String juceModulesPath;
  juce::String userModulesPath;
  juce::File outputDir;
  bool relocatable;
};


struct ExitException
{
public:
  ExitException(const int returnValue)
    : mReturnValue{returnValue}
  {
  }

  int returnValue() const
  {
    return mReturnValue;
  }

private:
  const int mReturnValue;
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

  template <typename... Args>
  void operator()(Args&&... args)
  {
    if (needsEmptyLine)
    {
      writeToStream(mStream, kNewLine);
      needsEmptyLine = false;
    }

    writeToStream(mStream, std::forward<Args>(args)..., kNewLine);
  }

  bool needsEmptyLine = false;

private:
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

  juce::MemoryOutputStream& mStream;
};


inline juce::StringArray convertIdsToStrings(
  const juce::StringArray& ids,
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


inline juce::String getAUMainTypeConstantFromQuotedFourChars(
  const juce::String& quotedFourChars)
{
  // clang-format off
  if (quotedFourChars == "'aufx'") return "kAudioUnitType_Effect";
  if (quotedFourChars == "'aufc'") return "kAudioUnitType_FormatConverter";
  if (quotedFourChars == "'augn'") return "kAudioUnitType_Generator";
  if (quotedFourChars == "'aumi'") return "kAudioUnitType_MIDIProcessor";
  if (quotedFourChars == "'aumx'") return "kAudioUnitType_Mixer";
  if (quotedFourChars == "'aumu'") return "kAudioUnitType_MusicDevice";
  if (quotedFourChars == "'aumf'") return "kAudioUnitType_MusicEffect";
  if (quotedFourChars == "'auol'") return "kAudioUnitType_OfflineEffect";
  if (quotedFourChars == "'auou'") return "kAudioUnitType_Output";
  if (quotedFourChars == "'aupn'") return "kAudioUnitType_Panner";
  // clang-format on
  return quotedFourChars;
}


inline juce::File getChildFileFromWorkingDirectory(
  const juce::StringRef relativeOrAbsolutePath)
{
  return juce::File::getCurrentWorkingDirectory().getChildFile(relativeOrAbsolutePath);
}


inline void printError(const juce::String& error)
{
  std::cerr << "error: " << error << std::endl;
}


// Matches juce::var::VariantType_String::toBool. This means that `toBoolLikeVar(s)` and
// `bool{juce::var{s}}` are equivalent.
inline bool toBoolLikeVar(const juce::String& s)
{
  return s.getIntValue() != 0 || s.trim().equalsIgnoreCase("true")
         || s.trim().equalsIgnoreCase("yes");
}

} // namespace Jucer2CMake
