// Copyright (C) 2019  Alain Martin
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

#include <iostream>
#include <string>
#include <vector>


int main(int argc, char* argv[])
{
  if (argc < 4)
  {
    std::cerr << "usage: XcassetsBuilder"
              << " <xcassets-output-dir>"
              << " <small-icon-image-file>"
              << " <large-icon-image-file>" << std::endl;
    return 1;
  }

  const std::vector<std::string> args{argv, argv + argc};

  const auto xcassetsBundle = File::getCurrentWorkingDirectory()
                                .getChildFile(juce::String{args.at(1)})
                                .getChildFile("Images.xcassets");
  const auto smallIconImageFile =
    args.at(2) == "<None>"
      ? File{}
      : File::getCurrentWorkingDirectory().getChildFile(juce::String{args.at(2)});
  const auto largeIconImageFile =
    args.at(3) == "<None>"
      ? File{}
      : File::getCurrentWorkingDirectory().getChildFile(juce::String{args.at(3)});

  const ProjectExporter projectExporter{smallIconImageFile, largeIconImageFile};

  juce::ScopedJuceInitialiser_GUI scopedJuceGui;

  const auto appiconsetBundle = xcassetsBundle.getChildFile("AppIcon.appiconset");
  ProjectExporter::overwriteFileIfDifferentOrThrow(
    appiconsetBundle.getChildFile("Contents.json"),
    ProjectExporter::getiOSAppIconContents());
  projectExporter.createiOSIconFiles(appiconsetBundle);

  const auto launchimageBundle = xcassetsBundle.getChildFile("LaunchImage.launchimage");
  ProjectExporter::overwriteFileIfDifferentOrThrow(
    launchimageBundle.getChildFile("Contents.json"),
    ProjectExporter::getiOSLaunchImageContents());
  ProjectExporter::createiOSLaunchImageFiles(launchimageBundle);

  std::cout << xcassetsBundle.getFullPathName() << std::flush;

  return 0;
}
