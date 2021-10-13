// Copyright (C) 2021  Alain Martin
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

#if defined(__clang__)
  #pragma clang diagnostic push
  #if __has_warning("-Watomic-implicit-seq-cst")
    #pragma clang diagnostic ignored "-Watomic-implicit-seq-cst"
  #endif
  #pragma clang diagnostic ignored "-Wcast-align"
  #pragma clang diagnostic ignored "-Wcast-qual"
  #pragma clang diagnostic ignored "-Wdocumentation"
  #pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
  #pragma clang diagnostic ignored "-Wextra-semi"
  #pragma clang diagnostic ignored "-Wglobal-constructors"
  #pragma clang diagnostic ignored "-Wimplicit-fallthrough"
  #if __has_warning("-Winconsistent-missing-destructor-override")
    #pragma clang diagnostic ignored "-Winconsistent-missing-destructor-override"
  #endif
  #pragma clang diagnostic ignored "-Wold-style-cast"
  #pragma clang diagnostic ignored "-Wsign-conversion"
  #pragma clang diagnostic ignored "-Wundef"
  #if __has_warning("-Wundefined-func-template")
    #pragma clang diagnostic ignored "-Wundefined-func-template"
  #endif
  #if __has_warning("-Wunused-template")
    #pragma clang diagnostic ignored "-Wunused-template"
  #endif
  #pragma clang diagnostic ignored "-Wweak-vtables"
  #if __has_warning("-Wzero-as-null-pointer-constant")
    #pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
  #endif

#elif defined(__GNUC__)
  #pragma GCC diagnostic push
  #if __GNUC__ >= 6
    #pragma GCC diagnostic ignored "-Wmisleading-indentation"
  #endif
  #if __GNUC__ >= 8
    #pragma GCC diagnostic ignored "-Wclass-memaccess"
  #endif

#elif defined(_MSC_VER)
  #pragma warning(push)

#endif

#include <juce_core/juce_core.h>

#if defined(__clang__)
  #pragma clang diagnostic pop
#elif defined(__GNUC__)
  #pragma GCC diagnostic pop
#elif defined(_MSC_VER)
  #pragma warning(pop)
#endif
