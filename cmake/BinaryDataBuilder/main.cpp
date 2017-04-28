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

#include "Source/jucer_Headers.h"
#include "Source/Project Saving/jucer_ResourceFile.h"

#include <iostream>
#include <string>
#include <vector>


int main(int argc, char* argv[])
{
  if (argc < 3)
  {
    std::cerr
      << "usage: BinaryDataBuilder"
      << " <BinaryData-files-output-dir>"
      << " <BinaryData.cpp-size-limit>"
      << " <BinaryData-namespace>"
      << " <resource-files>..."
      << std::endl;
    return 1;
  }

  std::vector<std::string> args{argv, argv + argc};

  Project project{args.at(1)};

  const int maxSize = std::stoi(args.at(2));

  const std::string& dataNamespace = args.at(3);

  ResourceFile resourceFile{project};
  resourceFile.setClassName(dataNamespace);

  for (auto i = 4u; i < args.size(); ++i)
  {
    resourceFile.addFile(File{args.at(i)});
  }

  Array<File> binaryDataFiles;

  auto result = resourceFile.write(binaryDataFiles, maxSize);

  if (!result.wasOk())
  {
    std::cerr << result.getErrorMessage() << std::endl;
    return 1;
  }

  for (const File& file : binaryDataFiles)
  {
    std::cout << file.getFileName() << ";";
  }
  std::cout << std::flush;

  return 0;
}
