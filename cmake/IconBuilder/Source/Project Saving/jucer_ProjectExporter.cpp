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

// clang-format off

// From line 24 onward, the content of this file was copied from
// https://github.com/WeAreROLI/JUCE/blob/4.3.1/extras/Projucer/Source/Project%20Saving/jucer_ProjectExporter.cpp


/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2015 - ROLI Ltd.

   Permission is granted to use this software under the terms of either:
   a) the GPL v2 (or any later version)
   b) the Affero GPL v3

   Details of these licenses can be found at: www.gnu.org/licenses

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

   ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.juce.com for more information.

  ==============================================================================
*/

#include "../jucer_Headers.h"
#include "jucer_ProjectExporter.h"


Image ProjectExporter::getBestIconForSize (int size, bool returnNullIfNothingBigEnough) const
{
    Drawable* im = nullptr;

    ScopedPointer<Drawable> im1 (getSmallIcon());
    ScopedPointer<Drawable> im2 (getBigIcon());

    if (im1 != nullptr && im2 != nullptr)
    {
        if (im1->getWidth() >= size && im2->getWidth() >= size)
            im = im1->getWidth() < im2->getWidth() ? im1 : im2;
        else if (im1->getWidth() >= size)
            im = im1;
        else if (im2->getWidth() >= size)
            im = im2;
    }
    else
    {
        im = im1 != nullptr ? im1 : im2;
    }

    if (im == nullptr)
        return Image();

    if (returnNullIfNothingBigEnough && im->getWidth() < size && im->getHeight() < size)
        return Image();

    return rescaleImageForIcon (*im, size);
}

Image ProjectExporter::rescaleImageForIcon (Drawable& d, const int size)
{
    if (DrawableImage* drawableImage = dynamic_cast<DrawableImage*> (&d))
    {
        Image im = SoftwareImageType().convert (drawableImage->getImage());

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
