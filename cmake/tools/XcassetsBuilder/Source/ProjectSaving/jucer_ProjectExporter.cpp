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

// clang-format off

// Lines 24-78 of this file were copied from
// https://github.com/WeAreROLI/JUCE/blob/5.2.1/extras/Projucer/Source/ProjectSaving/jucer_ProjectExporter.cpp


/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2017 - ROLI Ltd.

   JUCE is an open source library subject to commercial or open-source
   licensing.

   By using JUCE, you agree to the terms of both the JUCE 5 End-User License
   Agreement and JUCE 5 Privacy Policy (both updated and effective as of the
   27th April 2017).

   End User License Agreement: www.juce.com/juce-5-licence
   Privacy Policy: www.juce.com/juce-5-privacy-policy

   Or: You may also use this code under the terms of the GPL v3 (see
   www.gnu.org/licenses).

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

#include "jucer_ProjectExporter.h"


Image ProjectExporter::rescaleImageForIcon (Drawable& d, const int size)
{
    if (auto* drawableImage = dynamic_cast<DrawableImage*> (&d))
    {
        auto im = SoftwareImageType().convert (drawableImage->getImage());

        if (size == im.getWidth() && size == im.getHeight())
            return im;

        // (scale it down in stages for better resampling)
        while (im.getWidth() > 2 * size && im.getHeight() > 2 * size)
            im = im.rescaled (im.getWidth() / 2,
                              im.getHeight() / 2);

        Image newIm (Image::ARGB, size, size, true, SoftwareImageType());
        Graphics g (newIm);
        g.drawImageWithin (im, 0, 0, size, size,
                           RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize, false);
        return newIm;
    }

    Image im (Image::ARGB, size, size, true, SoftwareImageType());
    Graphics g (im);
    d.drawWithin (g, im.getBounds().toFloat(), RectanglePlacement::centred, 1.0f);
    return im;
}
