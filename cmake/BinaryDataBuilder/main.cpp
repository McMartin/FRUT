// Copyright (c) 2016 Alain Martin
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

#include "Source/jucer_Headers.h"
#include "Source/Project Saving/jucer_ResourceFile.h"

#include <iostream>
#include <string>
#include <vector>


int main(int argc, char* argv[])
{
  if (argc < 5)
  {
    std::cerr << "usage: BinaryDataBuilder"
              << " <BinaryData-files-output-dir>"
              << " <Project-UID>"
              << " <BinaryData.cpp-size-limit>"
              << " <BinaryData-namespace>"
              << " <resource-files>..." << std::endl;
    return 1;
  }

  std::vector<std::string> args{argv, argv + argc};

  Project project{args.at(1), args.at(2)};

  const auto maxSize = [&args]()
  {
    try
    {
      return std::stoi(args.at(3));
    }
    catch (const std::invalid_argument&)
    {
      std::cerr << "Invalid size limit" << std::endl;
      std::exit(1);
    }
  }();

  ResourceFile resourceFile{project};
  resourceFile.setClassName(args.at(4));

  for (auto i = 5u; i < args.size(); ++i)
  {
    resourceFile.addFile(File{args.at(i)});
  }

  Array<File> binaryDataFiles;

  const auto result = resourceFile.write(binaryDataFiles, maxSize);

  if (!result.wasOk())
  {
    std::cerr << result.getErrorMessage() << std::endl;
    return 1;
  }

  for (const auto& file : binaryDataFiles)
  {
    std::cout << file.getFileName() << ";";
  }
  std::cout << std::flush;

  return 0;
}


#if JUCE_LINUX && !JUCE_WEB_BROWSER
// Work-around a linker error in JUCE 5.0.0
namespace juce
{
int juce_gtkWebkitMain(int, const char**)
{
  return 0;
}
}
#endif
