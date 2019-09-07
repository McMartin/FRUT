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

// Lines 24-108 of this file were copied from
// https://github.com/WeAreROLI/JUCE/blob/4.2.0/extras/Projucer/Source/Utility/jucer_FileHelpers.cpp


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

#include <memory>

//==============================================================================
namespace FileHelpers
{
    static int64 calculateMemoryHashCode (const void* data, const size_t numBytes)
    {
        int64 t = 0;

        for (size_t i = 0; i < numBytes; ++i)
            t = t * 65599 + static_cast<const uint8*> (data)[i];

        return t;
    }

    int64 calculateStreamHashCode (InputStream& in)
    {
        int64 t = 0;

        const int bufferSize = 4096;
        HeapBlock<uint8> buffer;
        buffer.malloc (bufferSize);

        for (;;)
        {
            const int num = in.read (buffer, bufferSize);

            if (num <= 0)
                break;

            for (int i = 0; i < num; ++i)
                t = t * 65599 + buffer[i];
        }

        return t;
    }

    int64 calculateFileHashCode (const File& file)
    {
        std::unique_ptr<FileInputStream> stream{file.createInputStream()};
        return stream != nullptr ? calculateStreamHashCode (*stream) : 0;
    }

    bool overwriteFileWithNewDataIfDifferent (const File& file, const void* data, size_t numBytes)
    {
        if (file.getSize() == (int64) numBytes
              && calculateMemoryHashCode (data, numBytes) == calculateFileHashCode (file))
            return true;

        if (file.exists())
            return file.replaceWithData (data, numBytes);

        return file.getParentDirectory().createDirectory() && file.appendData (data, numBytes);
    }

    bool overwriteFileWithNewDataIfDifferent (const File& file, const MemoryOutputStream& newData)
    {
        return overwriteFileWithNewDataIfDifferent (file, newData.getData(), newData.getDataSize());
    }
}
