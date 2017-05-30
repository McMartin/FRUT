[![Build status][appveyor-badge]][appveyor]
[![Build Status][travis-ci-badge]][travis-ci]

# JUCE.cmake

`JUCE.cmake` is a collection of tools dedicated to building [JUCE][juce] projects using
[CMake][cmake] instead of the [Projucer][projucer].

It currently contains:

- `Reprojucer.cmake`, a CMake module that provides high-level functions to reproduce how a
JUCE project is defined in the Projucer,

- `Jucer2Reprojucer`, a console application based on JUCE that converts `.jucer` project
files into `CMakeLists.txt` files that include and use `Reprojucer.cmake`,

- several `CMakeLists.txt` files to build JUCE's `examples` and `extras` projects (in
[Jucer2Reprojucer/generated/JUCE](Jucer2Reprojucer/generated/JUCE)).


## Requirements

- CMake, version 3.4 minimum
- JUCE, version 4.2.0 to 4.3.1 (JUCE 5 is not supported yet)


## Getting started

Let's consider that you have a copy of [JUCE][github-juce], a copy of
[JUCE.cmake][github-juce-cmake] and a JUCE project called `MyGreatProject` following this
folder structure:

```
    <root>
    ├── JUCE/
    ├── JUCE.cmake/
    └── MyGreatProject/
        ├── Source/
        └── MyGreatProject.jucer
```

We first build `Jucer2Reprojucer` with CMake. Since `Jucer2Reprojucer` uses the JUCE
modules `juce_core`, `juce_data_structures` and `juce_events`, we specify where to find
JUCE by defining `JUCE_ROOT` when calling `cmake`.

```sh
cd <root>/JUCE.cmake/Jucer2Reprojucer

mkdir build && cd build

# On macOS
cmake .. -G Xcode -DJUCE_ROOT=../../../JUCE
# On Windows
cmake .. -DJUCE_ROOT=../../../JUCE

cmake --build .
```

Then we convert `MyGreatProject.jucer` to a new `CMakeLists.txt` file:

```sh
cd <root>/MyGreatProject

../JUCE.cmake/Jucer2Reprojucer/build/Debug/Jucer2Reprojucer MyGreatProject.jucer ../JUCE.cmake/cmake/Reprojucer.cmake
```

Now we can build `MyGreatProject` using CMake:

```sh
cd <root>/MyGreatProject

mkdir build && cd build

# On macOs
cmake .. -G Xcode -DMyGreatProject_jucer_FILE=../MyGreatProject.jucer
# On Windows
cmake .. -DMyGreatProject_jucer_FILE=../MyGreatProject.jucer

cmake --build .
```


## License

JUCE.cmake is free software: you can redistribute it and/or modify it under the terms of
the GNU General Public License as published by the Free Software Foundation, either
version 3 of the License, or (at your option) any later version.

JUCE.cmake is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the [LICENSE](LICENSE) file for more details.


[appveyor-badge]: https://ci.appveyor.com/api/projects/status/github/McMartin/JUCE.cmake?branch=master&svg=true
[appveyor]: https://ci.appveyor.com/project/McMartin/juce-cmake
[cmake]: https://cmake.org/
[github-juce-cmake]: https://github.com/McMartin/JUCE.cmake
[github-juce]: https://github.com/WeAreROLI/JUCE
[juce]: https://juce.com/
[projucer]: https://www.juce.com/projucer
[travis-ci-badge]: https://travis-ci.org/McMartin/JUCE.cmake.svg?branch=master
[travis-ci]: https://travis-ci.org/McMartin/JUCE.cmake
