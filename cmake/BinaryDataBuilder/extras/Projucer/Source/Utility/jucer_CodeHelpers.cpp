// Copyright (C) 2017-2018  Alain Martin
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

// Lines 24-243 of this file were copied from
// https://github.com/WeAreROLI/JUCE/blob/4.2.0/extras/Projucer/Source/Utility/jucer_CodeHelpers.cpp


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
#include "jucer_CodeHelpers.h"


//==============================================================================
namespace CodeHelpers
{
    String makeValidIdentifier (String s, bool capitalise, bool removeColons, bool allowTemplates)
    {
        if (s.isEmpty())
            return "unknown";

        if (removeColons)
            s = s.replaceCharacters (".,;:/@", "______");
        else
            s = s.replaceCharacters (".,;/@", "_____");

        for (int i = s.length(); --i > 0;)
            if (CharacterFunctions::isLetter (s[i])
                 && CharacterFunctions::isLetter (s[i - 1])
                 && CharacterFunctions::isUpperCase (s[i])
                 && ! CharacterFunctions::isUpperCase (s[i - 1]))
                s = s.substring (0, i) + " " + s.substring (i);

        String allowedChars ("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_ 0123456789");
        if (allowTemplates)
            allowedChars += "<>";

        if (! removeColons)
            allowedChars += ":";

        StringArray words;
        words.addTokens (s.retainCharacters (allowedChars), false);
        words.trim();

        String n (words[0]);

        if (capitalise)
            n = n.toLowerCase();

        for (int i = 1; i < words.size(); ++i)
        {
            if (capitalise && words[i].length() > 1)
                n << words[i].substring (0, 1).toUpperCase()
                  << words[i].substring (1).toLowerCase();
            else
                n << words[i];
        }

        if (CharacterFunctions::isDigit (n[0]))
            n = "_" + n;

        if (CPlusPlusCodeTokeniser::isReservedKeyword (n))
            n << '_';

        return n;
    }

    String makeBinaryDataIdentifierName (const File& file)
    {
        return makeValidIdentifier (file.getFileName()
                                        .replaceCharacters (" .", "__")
                                        .retainCharacters ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_0123456789"),
                                    false, true, false);
    }

    void writeDataAsCppLiteral (const MemoryBlock& mb, OutputStream& out,
                                bool breakAtNewLines, bool allowStringBreaks)
    {
        const int maxCharsOnLine = 250;

        const unsigned char* data = (const unsigned char*) mb.getData();
        int charsOnLine = 0;

        bool canUseStringLiteral = mb.getSize() < 32768; // MS compilers can't handle big string literals..

        if (canUseStringLiteral)
        {
            unsigned int numEscaped = 0;

            for (size_t i = 0; i < mb.getSize(); ++i)
            {
                const unsigned int num = (unsigned int) data[i];
                if (! ((num >= 32 && num < 127) || num == '\t' || num == '\r' || num == '\n'))
                {
                    if (++numEscaped > mb.getSize() / 4)
                    {
                        canUseStringLiteral = false;
                        break;
                    }
                }
            }
        }

        if (! canUseStringLiteral)
        {
            out << "{ ";

            for (size_t i = 0; i < mb.getSize(); ++i)
            {
                const int num = (int) (unsigned int) data[i];
                out << num << ',';

                charsOnLine += 2;

                if (num >= 10)
                {
                    ++charsOnLine;

                    if (num >= 100)
                        ++charsOnLine;
                }

                if (charsOnLine >= maxCharsOnLine)
                {
                    charsOnLine = 0;
                    out << newLine;
                }
            }

            out << "0,0 };";
        }
        else
        {
            out << "\"";
            CppTokeniserFunctions::writeEscapeChars (out, (const char*) data, (int) mb.getSize(),
                                                     maxCharsOnLine, breakAtNewLines, false, allowStringBreaks);
            out << "\";";
        }
    }

    //==============================================================================
    static unsigned int calculateHash (const String& s, const unsigned int hashMultiplier)
    {
        const char* t = s.toUTF8();
        unsigned int hash = 0;
        while (*t != 0)
            hash = hashMultiplier * hash + (unsigned int) *t++;

        return hash;
    }

    static unsigned int findBestHashMultiplier (const StringArray& strings)
    {
        unsigned int v = 31;

        for (;;)
        {
            SortedSet <unsigned int> hashes;
            bool collision = false;
            for (int i = strings.size(); --i >= 0;)
            {
                const unsigned int hash = calculateHash (strings[i], v);
                if (hashes.contains (hash))
                {
                    collision = true;
                    break;
                }

                hashes.add (hash);
            }

            if (! collision)
                break;

            v += 2;
        }

        return v;
    }

    void createStringMatcher (OutputStream& out, const String& utf8PointerVariable,
                              const StringArray& strings, const StringArray& codeToExecute, const int indentLevel)
    {
        jassert (strings.size() == codeToExecute.size());
        const String indent (String::repeatedString (" ", indentLevel));
        const unsigned int hashMultiplier = findBestHashMultiplier (strings);

        out << indent << "unsigned int hash = 0;" << newLine
            << indent << "if (" << utf8PointerVariable << " != 0)" << newLine
            << indent << "    while (*" << utf8PointerVariable << " != 0)" << newLine
            << indent << "        hash = " << (int) hashMultiplier << " * hash + (unsigned int) *" << utf8PointerVariable << "++;" << newLine
            << newLine
            << indent << "switch (hash)" << newLine
            << indent << "{" << newLine;

        for (int i = 0; i < strings.size(); ++i)
        {
            out << indent << "    case 0x" << hexString8Digits ((int) calculateHash (strings[i], hashMultiplier))
                << ":  " << codeToExecute[i] << newLine;
        }

        out << indent << "    default: break;" << newLine
            << indent << "}" << newLine << newLine;
    }
}
