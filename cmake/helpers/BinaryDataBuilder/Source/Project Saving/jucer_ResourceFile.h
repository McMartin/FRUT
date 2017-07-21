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

// clang-format off

// From line 24 onward, the content of this file was copied from
// https://github.com/WeAreROLI/JUCE/blob/4.3.1/extras/Projucer/Source/Project%20Saving/jucer_ResourceFile.h


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


//==============================================================================
class ResourceFile
{
public:
    //==============================================================================
    ResourceFile (Project& project);
    ~ResourceFile();

    //==============================================================================
    void setClassName (const String& className);
    String getClassName() const       { return className; }

    void addFile (const File& file);
    String getDataVariableFor (const File& file) const;
    String getSizeVariableFor (const File& file) const;

    int getNumFiles() const                 { return files.size(); }
    const File& getFile (int index) const   { return files.getReference (index); }

    int64 getTotalDataSize() const;

    Result write (Array<File>& filesCreated, int maxFileSize);

    //==============================================================================
private:
    Array<File> files;
    StringArray variableNames;
    Project& project;
    String className;

    Result writeHeader (MemoryOutputStream&);
    Result writeCpp (MemoryOutputStream&, const File& headerFile, int& index, int maxFileSize);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ResourceFile)
};


#endif   // JUCER_RESOURCEFILE_H_INCLUDED
