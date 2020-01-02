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

#pragma once

#include "../jucer_Headers.h"

#include <memory>


class ProjectExporter
{

public:
  ProjectExporter(const File& smallIcon, const File& bigIcon)
    : mSmallIcon{smallIcon}
    , mBigIcon{bigIcon}
  {
  }

  std::unique_ptr<Drawable> getBigIcon() const
  {
    return std::unique_ptr<Drawable>{Drawable::createFromImageFile(mBigIcon)};
  }

  std::unique_ptr<Drawable> getSmallIcon() const
  {
    return std::unique_ptr<Drawable>{Drawable::createFromImageFile(mSmallIcon)};
  }

  Image getBestIconForSize(int size, bool returnNullIfNothingBigEnough) const;

  static Image rescaleImageForIcon(Drawable&, int iconSize);

#include "jucer_ProjectExport_MSVC.h"
#include "jucer_ProjectExport_XCode.h"

private:
  const File mSmallIcon;
  const File mBigIcon;
};
