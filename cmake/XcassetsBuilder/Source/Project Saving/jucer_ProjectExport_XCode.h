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

// Lines 27-54, 56-57, 60-149, 151-201, and 204-239 of this file were copied from
// https://github.com/WeAreROLI/JUCE/blob/5.2.1/extras/Projucer/Source/ProjectSaving/jucer_ProjectExport_Xcode.h

// Lines 242-291 of this file were copied from
// https://github.com/WeAreROLI/JUCE/blob/5.2.1/extras/Projucer/Source/ProjectSaving/jucer_ProjectExporter.h


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

    void getIconImages (OwnedArray<Drawable>& images) const
    {
        std::unique_ptr<Drawable> bigIcon (getBigIcon());
        if (bigIcon != nullptr)
            images.add (bigIcon.release());

        std::unique_ptr<Drawable> smallIcon (getSmallIcon());
        if (smallIcon != nullptr)
            images.add (smallIcon.release());
    }

    void createiOSIconFiles (File appIconSet) const
    {
        OwnedArray<Drawable> images;
        getIconImages (images);

        if (images.size() > 0)
        {
            for (auto& type : getiOSAppIconTypes())
            {
                auto image = rescaleImageForIcon (*images.getFirst(), type.size);

                if (image.hasAlphaChannel())
                {
                    Image background (Image::RGB, image.getWidth(), image.getHeight(), false);
                    Graphics g (background);
                    g.fillAll (Colours::white);

                    g.drawImageWithin (image, 0, 0, image.getWidth(), image.getHeight(),
                                       RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize);

                    image = background;
                }

                MemoryOutputStream pngData;
                PNGImageFormat pngFormat;
                pngFormat.writeImageToStream (image, pngData);

                overwriteFileIfDifferentOrThrow (appIconSet.getChildFile (type.filename), pngData);
            }
        }
    }

    struct AppIconType
    {
        const char* idiom;
        const char* sizeString;
        const char* filename;
        const char* scale;
        int size;
    };

    static Array<AppIconType> getiOSAppIconTypes()
    {
        AppIconType types[] =
        {
            { "iphone",          "20x20",     "Icon-Notification-20@2x.png",       "2x", 40   },
            { "iphone",          "20x20",     "Icon-Notification-20@3x.png",       "3x", 60   },
            { "iphone",          "29x29",     "Icon-29.png",                       "1x", 29   },
            { "iphone",          "29x29",     "Icon-29@2x.png",                    "2x", 58   },
            { "iphone",          "29x29",     "Icon-29@3x.png",                    "3x", 87   },
            { "iphone",          "40x40",     "Icon-Spotlight-40@2x.png",          "2x", 80   },
            { "iphone",          "40x40",     "Icon-Spotlight-40@3x.png",          "3x", 120  },
            { "iphone",          "57x57",     "Icon.png",                          "1x", 57   },
            { "iphone",          "57x57",     "Icon@2x.png",                       "2x", 114  },
            { "iphone",          "60x60",     "Icon-60@2x.png",                    "2x", 120  },
            { "iphone",          "60x60",     "Icon-@3x.png",                      "3x", 180  },
            { "ipad",            "20x20",     "Icon-Notifications-20.png",         "1x", 20   },
            { "ipad",            "20x20",     "Icon-Notifications-20@2x.png",      "2x", 40   },
            { "ipad",            "29x29",     "Icon-Small-1.png",                  "1x", 29   },
            { "ipad",            "29x29",     "Icon-Small@2x-1.png",               "2x", 58   },
            { "ipad",            "40x40",     "Icon-Spotlight-40.png",             "1x", 40   },
            { "ipad",            "40x40",     "Icon-Spotlight-40@2x-1.png",        "2x", 80   },
            { "ipad",            "50x50",     "Icon-Small-50.png",                 "1x", 50   },
            { "ipad",            "50x50",     "Icon-Small-50@2x.png",              "2x", 100  },
            { "ipad",            "72x72",     "Icon-72.png",                       "1x", 72   },
            { "ipad",            "72x72",     "Icon-72@2x.png",                    "2x", 144  },
            { "ipad",            "76x76",     "Icon-76.png",                       "1x", 76   },
            { "ipad",            "76x76",     "Icon-76@2x.png",                    "2x", 152  },
            { "ipad",            "83.5x83.5", "Icon-83.5@2x.png",                  "2x", 167  },
            { "ios-marketing",   "1024x1024", "Icon-AppStore-1024.png",            "1x", 1024 }
        };

        return Array<AppIconType> (types, numElementsInArray (types));
    }

    static String getiOSAppIconContents()
    {
        var images;

        for (auto& type : getiOSAppIconTypes())
        {
            DynamicObject::Ptr d = new DynamicObject();
            d->setProperty ("idiom",    type.idiom);
            d->setProperty ("size",     type.sizeString);
            d->setProperty ("filename", type.filename);
            d->setProperty ("scale",    type.scale);
            images.append (var (d.get()));
        }

        return getiOSAssetContents (images);
    }

    struct ImageType
    {
        const char* orientation;
        const char* idiom;
        const char* subtype;
        const char* extent;
        const char* scale;
        const char* filename;
        int width;
        int height;
    };

    static Array<ImageType> getiOSLaunchImageTypes()
    {
        ImageType types[] =
        {
            { "portrait", "iphone", nullptr,      "full-screen", "2x", "LaunchImage-iphone-2x.png",         640, 960 },
            { "portrait", "iphone", "retina4",    "full-screen", "2x", "LaunchImage-iphone-retina4.png",    640, 1136 },
            { "portrait", "ipad",   nullptr,      "full-screen", "1x", "LaunchImage-ipad-portrait-1x.png",  768, 1024 },
            { "landscape","ipad",   nullptr,      "full-screen", "1x", "LaunchImage-ipad-landscape-1x.png", 1024, 768 },
            { "portrait", "ipad",   nullptr,      "full-screen", "2x", "LaunchImage-ipad-portrait-2x.png",  1536, 2048 },
            { "landscape","ipad",   nullptr,      "full-screen", "2x", "LaunchImage-ipad-landscape-2x.png", 2048, 1536 }
        };

        return Array<ImageType> (types, numElementsInArray (types));
    }

    static String getiOSLaunchImageContents()
    {
        const Array<ImageType> types (getiOSLaunchImageTypes());
        var images;

        for (int i = 0; i < types.size(); ++i)
        {
            const ImageType& type = types.getReference(i);

            DynamicObject::Ptr d = new DynamicObject();
            d->setProperty ("orientation", type.orientation);
            d->setProperty ("idiom", type.idiom);
            d->setProperty ("extent",  type.extent);
            d->setProperty ("minimum-system-version", "7.0");
            d->setProperty ("scale", type.scale);
            d->setProperty ("filename", type.filename);

            if (type.subtype != nullptr)
                d->setProperty ("subtype", type.subtype);

            images.append (var (d.get()));
        }

        return getiOSAssetContents (images);
    }

    static void createiOSLaunchImageFiles (const File& launchImageSet)
    {
        const Array<ImageType> types (getiOSLaunchImageTypes());

        for (int i = 0; i < types.size(); ++i)
        {
            const ImageType& type = types.getReference(i);

            Image image (Image::ARGB, type.width, type.height, true); // (empty black image)
            image.clear (image.getBounds(), Colours::black);

            MemoryOutputStream pngData;
            PNGImageFormat pngFormat;
            pngFormat.writeImageToStream (image, pngData);
            overwriteFileIfDifferentOrThrow (launchImageSet.getChildFile (type.filename), pngData);
        }
    }

    static String getiOSAssetContents (var images)
    {
        DynamicObject::Ptr v (new DynamicObject());

        var info (new DynamicObject());
        info.getDynamicObject()->setProperty ("version", 1);
        info.getDynamicObject()->setProperty ("author", "xcode");

        v->setProperty ("images", images);
        v->setProperty ("info", info);

        return JSON::toString (var (v.get()));
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

    class SaveError
    {
    public:
        SaveError (const String& error) : message (error)
        {}

        SaveError (const File& fileThatFailedToWrite)
            : message ("Can't write to the file: " + fileThatFailedToWrite.getFullPathName())
        {}

        String message;
    };

    static void overwriteFileIfDifferentOrThrow (const File& file, const MemoryOutputStream& newData)
    {
        if (! FileHelpers::overwriteFileWithNewDataIfDifferent (file, newData))
            throw SaveError (file);
    }

    static void overwriteFileIfDifferentOrThrow (const File& file, const String& newData)
    {
        if (! FileHelpers::overwriteFileWithNewDataIfDifferent (file, newData))
            throw SaveError (file);
    }
