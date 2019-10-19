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

// Lines 27-49, 52-119, 122-128, and 130-161 of this file were copied from
// https://github.com/WeAreROLI/JUCE/blob/4.2.0/extras/Projucer/Source/Project%20Saving/jucer_ProjectExport_XCode.h

// Lines 164-188, 191-223, 226-232, and 234-265 of this file were copied from
// https://github.com/WeAreROLI/JUCE/blob/5.4.0/extras/Projucer/Source/ProjectSaving/jucer_ProjectExport_Xcode.h


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

    static Image fixMacIconImageSize_v4_2_0 (Drawable& image)
    {
        const int validSizes[] = { 16, 32, 48, 128, 256, 512, 1024 };

        const int w = image.getWidth();
        const int h = image.getHeight();

        int bestSize = 16;

        for (int i = 0; i < numElementsInArray (validSizes); ++i)
        {
            if (w == h && w == validSizes[i])
            {
                bestSize = w;
                break;
            }

            if (jmax (w, h) > validSizes[i])
                bestSize = validSizes[i];
        }

        return rescaleImageForIcon (image, bestSize);
    }

    static void writeOldIconFormat (MemoryOutputStream& out, const Image& image, const char* type, const char* maskType)
    {
        const int w = image.getWidth();
        const int h = image.getHeight();

        out.write (type, 4);
        out.writeIntBigEndian (8 + 4 * w * h);

        const Image::BitmapData bitmap (image, Image::BitmapData::readOnly);

        for (int y = 0; y < h; ++y)
        {
            for (int x = 0; x < w; ++x)
            {
                const Colour pixel (bitmap.getPixelColour (x, y));
                out.writeByte ((char) pixel.getAlpha());
                out.writeByte ((char) pixel.getRed());
                out.writeByte ((char) pixel.getGreen());
                out.writeByte ((char) pixel.getBlue());
            }
        }

        out.write (maskType, 4);
        out.writeIntBigEndian (8 + w * h);

        for (int y = 0; y < h; ++y)
        {
            for (int x = 0; x < w; ++x)
            {
                const Colour pixel (bitmap.getPixelColour (x, y));
                out.writeByte ((char) pixel.getAlpha());
            }
        }
    }

    static void writeNewIconFormat (MemoryOutputStream& out, const Image& image, const char* type)
    {
        MemoryOutputStream pngData;
        PNGImageFormat pngFormat;
        pngFormat.writeImageToStream (image, pngData);

        out.write (type, 4);
        out.writeIntBigEndian (8 + (int) pngData.getDataSize());
        out << pngData;
    }

    void writeIcnsFile_v4_2_0 (const OwnedArray<Drawable>& images, OutputStream& out) const
    {
        MemoryOutputStream data;
        int smallest = 0x7fffffff;
        Drawable* smallestImage = nullptr;

        for (int i = 0; i < images.size(); ++i)
        {
            const Image image (fixMacIconImageSize_v4_2_0 (*images.getUnchecked(i)));
            jassert (image.getWidth() == image.getHeight());

            if (image.getWidth() < smallest)
            {
                smallest = image.getWidth();
                smallestImage = images.getUnchecked(i);
            }

            switch (image.getWidth())
            {
                case 16:   writeOldIconFormat (data, image, "is32", "s8mk"); break;
                case 32:   writeOldIconFormat (data, image, "il32", "l8mk"); break;
                case 48:   writeOldIconFormat (data, image, "ih32", "h8mk"); break;
                case 128:  writeOldIconFormat (data, image, "it32", "t8mk"); break;
                case 256:  writeNewIconFormat (data, image, "ic08"); break;
                case 512:  writeNewIconFormat (data, image, "ic09"); break;
                case 1024: writeNewIconFormat (data, image, "ic10"); break;
                default:   break;
            }
        }

        jassert (data.getDataSize() > 0); // no suitable sized images?

        // If you only supply a 1024 image, the file doesn't work on 10.8, so we need
        // to force a smaller one in there too..
        if (smallest > 512 && smallestImage != nullptr)
            writeNewIconFormat (data, rescaleImageForIcon (*smallestImage, 512), "ic09");

        out.write ("icns", 4);
        out.writeIntBigEndian ((int) data.getDataSize() + 8);
        out << data;
    }


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

    static Image fixMacIconImageSize_v5_4_0 (Drawable& image)
    {
        const int validSizes[] = { 16, 32, 64, 128, 256, 512, 1024 };

        auto w = image.getWidth();
        auto h = image.getHeight();

        int bestSize = 16;

        for (int size : validSizes)
        {
            if (w == h && w == size)
            {
                bestSize = w;
                break;
            }

            if (jmax (w, h) > size)
                bestSize = size;
        }

        return rescaleImageForIcon (image, bestSize);
    }

    static void writeIconData (MemoryOutputStream& out, const Image& image, const char* type)
    {
        MemoryOutputStream pngData;
        PNGImageFormat pngFormat;
        pngFormat.writeImageToStream (image, pngData);

        out.write (type, 4);
        out.writeIntBigEndian (8 + (int) pngData.getDataSize());
        out << pngData;
    }

    void writeIcnsFile_v5_4_0 (const OwnedArray<Drawable>& images, OutputStream& out) const
    {
        MemoryOutputStream data;
        auto smallest = std::numeric_limits<int>::max();
        Drawable* smallestImage = nullptr;

        for (int i = 0; i < images.size(); ++i)
        {
            auto image = fixMacIconImageSize_v5_4_0 (*images.getUnchecked (i));
            jassert (image.getWidth() == image.getHeight());

            if (image.getWidth() < smallest)
            {
                smallest = image.getWidth();
                smallestImage = images.getUnchecked(i);
            }

            switch (image.getWidth())
            {
                case 16:   writeIconData (data, image, "icp4"); break;
                case 32:   writeIconData (data, image, "icp5"); break;
                case 64:   writeIconData (data, image, "icp6"); break;
                case 128:  writeIconData (data, image, "ic07"); break;
                case 256:  writeIconData (data, image, "ic08"); break;
                case 512:  writeIconData (data, image, "ic09"); break;
                case 1024: writeIconData (data, image, "ic10"); break;
                default:   break;
            }
        }

        jassert (data.getDataSize() > 0); // no suitable sized images?

        // If you only supply a 1024 image, the file doesn't work on 10.8, so we need
        // to force a smaller one in there too..
        if (smallest > 512 && smallestImage != nullptr)
            writeIconData (data, rescaleImageForIcon (*smallestImage, 512), "ic09");

        out.write ("icns", 4);
        out.writeIntBigEndian ((int) data.getDataSize() + 8);
        out << data;
    }
