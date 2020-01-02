// Copyright (C) 2017  Alain Martin
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

#include "Source/Project Saving/jucer_ProjectExporter.h"
#include "Source/Utility/jucer_FileHelpers.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>


int main(int argc, char* argv[])
{
  if (argc < 6)
  {
    std::cerr << "usage: IconBuilder"
              << " <Projucer-version>"
              << " <icon-format:icns,ico>"
              << " <icon-file-output-dir>"
              << " <small-icon-image-file>"
              << " <large-icon-image-file>" << std::endl;
    return 1;
  }

  const std::vector<std::string> args{argv, argv + argc};

  using Version = std::tuple<int, int, int>;

  const auto jucerVersion = [&args]() {
    if (args.at(1) == "latest")
    {
      return Version{5, 4, 0};
    }

    const auto versionTokens = StringArray::fromTokens(String{args.at(1)}, ".", {});
    if (versionTokens.size() != 3)
    {
      std::cerr << "Invalid Projucer version" << std::endl;
      std::exit(1);
    }

    try
    {
      return Version{std::stoi(versionTokens[0].toStdString()),
                     std::stoi(versionTokens[1].toStdString()),
                     std::stoi(versionTokens[2].toStdString())};
    }
    catch (const std::invalid_argument&)
    {
      std::cerr << "Invalid Projucer version" << std::endl;
      std::exit(1);
    }
  }();

  const auto& iconFormat = args.at(2);
  const auto outputDir =
    File::getCurrentWorkingDirectory().getChildFile(juce::String{args.at(3)});
  const auto smallIconImageFile =
    args.at(4) == "<None>"
      ? File{}
      : File::getCurrentWorkingDirectory().getChildFile(juce::String{args.at(4)});
  const auto largeIconImageFile =
    args.at(5) == "<None>"
      ? File{}
      : File::getCurrentWorkingDirectory().getChildFile(juce::String{args.at(5)});

  const ProjectExporter projectExporter{smallIconImageFile, largeIconImageFile};

  juce::ScopedJuceInitialiser_GUI scopedJuceGui;

  if (iconFormat == "icns")
  {
    OwnedArray<Drawable> images;

    auto bigIcon = projectExporter.getBigIcon();
    if (bigIcon)
    {
      images.add(bigIcon.release());
    }

    auto smallIcon = projectExporter.getSmallIcon();
    if (smallIcon)
    {
      images.add(smallIcon.release());
    }

    if (images.size() > 0)
    {
      const auto iconFile = outputDir.getChildFile("Icon.icns");

      MemoryOutputStream outStream;
      if (jucerVersion < Version{5, 4, 0})
      {
        projectExporter.writeIcnsFile_v4_2_0(images, outStream);
      }
      else
      {
        projectExporter.writeIcnsFile_v5_4_0(images, outStream);
      }

      if (!FileHelpers::overwriteFileWithNewDataIfDifferent(iconFile, outStream))
      {
        return 1;
      }

      std::cout << iconFile.getFileName() << std::flush;
    }
  }
  else if (iconFormat == "ico")
  {
    Array<Image> images;

    for (const auto& size : {16, 32, 48, 256})
    {
      const auto image = projectExporter.getBestIconForSize(size, true);

      if (image.isValid())
      {
        images.add(image);
      }
    }

    if (images.size() > 0)
    {
      const auto iconFile = outputDir.getChildFile("icon.ico");

      MemoryOutputStream outStream;
      projectExporter.writeIconFile(images, outStream);

      if (!FileHelpers::overwriteFileWithNewDataIfDifferent(iconFile, outStream))
      {
        return 1;
      }

      std::cout << iconFile.getFileName() << std::flush;
    }
  }
  else
  {
    std::cerr << "Unsupported icon format \"" << iconFormat << "\"" << std::endl;
    return 1;
  }

  return 0;
}
