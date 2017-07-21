// Copyright (c) 2017 Alain Martin
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

#include "../jucer_Headers.h"


class ProjectExporter
{

public:
  ProjectExporter(const File& smallIcon, const File& bigIcon)
    : mSmallIcon{smallIcon}
    , mBigIcon{bigIcon}
  {
  }

  Drawable* getBigIcon() const
  {
    return Drawable::createFromImageFile(mBigIcon);
  }

  Drawable* getSmallIcon() const
  {
    return Drawable::createFromImageFile(mSmallIcon);
  }

  Image getBestIconForSize(int size, bool returnNullIfNothingBigEnough) const;

  static Image rescaleImageForIcon(Drawable&, int iconSize);

#include "jucer_ProjectExport_MSVC.h"
#include "jucer_ProjectExport_XCode.h"

private:
  const File mSmallIcon;
  const File mBigIcon;
};
