// Copyright (c) 2017 Alain Martin
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

#include <iostream>
#include <string>
#include <vector>


int main(int argc, char* argv[])
{
  if (argc < 5)
  {
    std::cerr << "usage: IconBuilder"
              << " <icon-format:icns,ico>"
              << " <icon-file-output-dir>"
              << " <small-icon-image-file>"
              << " <large-icon-image-file>" << std::endl;
    return 1;
  }

  const std::vector<std::string> args{argv, argv + argc};

  const auto& iconFormat = args.at(1);
  const auto outputDir = File{args.at(2)};
  const auto smallIconImageFile = args.at(3) == "<None>" ? File{} : File{args.at(3)};
  const auto largeIconImageFile = args.at(4) == "<None>" ? File{} : File{args.at(4)};

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
      projectExporter.writeIcnsFile(images, outStream);

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
