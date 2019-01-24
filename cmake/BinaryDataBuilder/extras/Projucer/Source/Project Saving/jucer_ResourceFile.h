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

// Lines 24-51, 62-72, 75-82, 85, and 87-89 of this file were copied from
// https://github.com/WeAreROLI/JUCE/blob/4.2.0/extras/Projucer/Source/Project%20Saving/jucer_ResourceFile.h


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

#ifndef JUCER_RESOURCEFILE_H_INCLUDED
#define JUCER_RESOURCEFILE_H_INCLUDED

#include "../Project/jucer_Project.h"


enum class ProjucerVersion
{
  v4_2_0,
  v5_0_0,
  v5_3_1,
};


//==============================================================================
class ResourceFile
{
public:
    //==============================================================================
    ResourceFile (Project& project);

    //==============================================================================
    void setClassName (const String& className);

    void addFile (const File& file);

    template <ProjucerVersion>
    Result write (Array<File>& filesCreated, int maxFileSize);

    //==============================================================================
private:
    Array<File> files;
    StringArray variableNames;
    Project& project;
    String className;

    template <ProjucerVersion>
    Result writeHeader (MemoryOutputStream&);
    template <ProjucerVersion>
    Result writeCpp (MemoryOutputStream&, const File& headerFile, int& index, int maxFileSize);
};


#endif   // JUCER_RESOURCEFILE_H_INCLUDED
