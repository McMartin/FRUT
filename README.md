# JUCE.cmake

[![Build status](https://ci.appveyor.com/api/projects/status/github/McMartin/JUCE.cmake?branch=master&svg=true)](https://ci.appveyor.com/project/McMartin/juce-cmake)
[![Build Status](https://travis-ci.org/McMartin/JUCE.cmake.svg?branch=master)](https://travis-ci.org/McMartin/JUCE.cmake)

JUCE.cmake is a cmake framework that makes [JUCE](juce.com) compatible with the cmake build system.

## Prerequisites

Install CMAKE for your system: https://cmake.org/download/


Get a copy of JUCE and JUCE.cmake if you don't have them already.

```bash
git clone https://github.com/WeAreROLI/JUCE.git
git clone https://github.com/McMartin/JUCE.cmake.git
```

JUCE.cmake is not yet compatible with JUCE 5, so we'll checkout the 4.3.1 release (commit sha  fc9937d)

```bash
cd JUCE
git checkout fc9937d
cd ..
```

## Getting Started

In this section, we'll see how to make the JUCE's example's HelloWorld jucer project compatible with the cmake build system using the JUCE.cmake framework.


We need a CMakeLists.txt file in order to use the cmake build system. Two options: either convert your dot jucer file, or create it from scratch. In this getting started, we're going to use the Jucer2CMake converter.


### Building Jucer2CMake converter

```bash
cd JUCE.cmake/Jucer2CMake
mkdir build
cd build
cmake .. -DJUCE_ROOT="../../../JUCE"
cmake --build .
# for windows only, so that the rest of this tutorial works on Windows and OSX
mv Debug/Jucer2Cmake.exe .
```

Go to your root folder and create a folder for our new project.

```bash
cd ../../..
mkdir helloWorldExample
cd helloWorldExample
```

Generate the CMakeLists for the HelloWorld project

```bash
 ../JUCE.cmake/Jucer2CMake/build/Jucer2CMake.exe ../JUCE/examples/HelloWorld/HelloWorld.jucer ../JUCE.cmake/cmake/Reprojucer.cmake
# ls must show a unique CMakeLists.txt file.
```

If nothing prints, it means everything went as expected. Now it's just cmake as usual ! :)

```bash
mkdir build
cd build
cmake .. -DHelloWorld_jucer_FILE="../../JUCE/examples/HelloWorld/HelloWorld.jucer"
cmake --build .

# for osx
open ./HelloWorld.app
# for windows
Debug/HelloWorld.exe
```

## License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](LICENSE) file for details
