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

#include <juce_core/juce_core.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>


int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cerr << "usage: PListMerger"
              << " <first-plist-content>"
              << " <second-plist-content>" << std::endl;
    return 1;
  }

  const std::vector<std::string> args{argv, argv + argc};

  const juce::ScopedPointer<juce::XmlElement> firstPlistElement =
    juce::XmlDocument::parse(args.at(1));
  if (!firstPlistElement || !firstPlistElement->hasTagName("plist"))
  {
    std::cerr << "Invalid first plist content, expected <plist> element" << std::endl;
    return 1;
  }

  const auto firstDictElement = firstPlistElement->getChildByName("dict");
  if (!firstDictElement)
  {
    std::cerr << "Invalid first plist content, expected <dict> element" << std::endl;
    return 1;
  }

  std::vector<std::string> keysInFirstPlist;

  for (auto childElement = firstDictElement->getFirstChildElement();
       childElement != nullptr; childElement = childElement->getNextElement())
  {
    if (childElement->getTagName() != "key" || childElement->getNumChildElements() != 1
        || !childElement->getFirstChildElement()->isTextElement())
    {
      std::cerr << "Invalid first plist content, expected <key> element with only one "
                   "text child element"
                << std::endl;
      return 1;
    }

    const auto key = childElement->getFirstChildElement()->getText().toStdString();

    if (std::find(keysInFirstPlist.begin(), keysInFirstPlist.end(), key)
        != keysInFirstPlist.end())
    {
      std::cerr << "Invalid first plist content, duplicated key \"" << key << "\""
                << std::endl;
      return 1;
    }

    keysInFirstPlist.push_back(key);

    childElement = childElement->getNextElement();
    if (childElement == nullptr)
    {
      std::cerr << "Invalid first plist content, missing value associated with key \""
                << key << "\"" << std::endl;
      return 1;
    }
  }

  const juce::ScopedPointer<juce::XmlElement> secondPlistElement =
    juce::XmlDocument::parse(args.at(2));
  if (!secondPlistElement || !secondPlistElement->hasTagName("plist"))
  {
    std::cerr << "Invalid second plist content, expected <plist> element" << std::endl;
    return 1;
  }

  const auto secondDictElement = secondPlistElement->getChildByName("dict");
  if (!secondDictElement)
  {
    std::cerr << "Invalid second plist content, expected <dict> element" << std::endl;
    return 1;
  }

  for (auto childElement = secondDictElement->getFirstChildElement();
       childElement != nullptr; childElement = childElement->getNextElement())
  {
    if (childElement->getTagName() != "key" || childElement->getNumChildElements() != 1
        || !childElement->getFirstChildElement()->isTextElement())
    {
      std::cerr << "Invalid second plist content, expected <key> element with only one "
                   "text child element"
                << std::endl;
      return 1;
    }

    const auto key = childElement->getFirstChildElement()->getText().toStdString();

    const auto isKeyAlreadyInFirstPlist =
      std::find(keysInFirstPlist.begin(), keysInFirstPlist.end(), key)
      != keysInFirstPlist.end();

    if (!isKeyAlreadyInFirstPlist)
    {
      firstDictElement->addChildElement(new juce::XmlElement(*childElement));
    }

    childElement = childElement->getNextElement();
    if (childElement == nullptr)
    {
      std::cerr << "Invalid second plist content, missing value associated with key \""
                << key << "\"" << std::endl;
      return 1;
    }

    if (!isKeyAlreadyInFirstPlist)
    {
      firstDictElement->addChildElement(new juce::XmlElement(*childElement));
    }
  }

  std::cout << firstPlistElement->createDocument(juce::String{}, false, false)
            << std::flush;

  return 0;
}
