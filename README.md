[![Build status][appveyor-badge]][appveyor]
[![Build Status][travis-ci-badge]][travis-ci]
[![Join the chat at https://gitter.im/JUCE-cmake/Lobby][gitter-badge]][gitter]

# JUCE.cmake

`JUCE.cmake` is a collection of tools dedicated to building [JUCE][juce] projects using
[CMake][cmake] instead of [Projucer][projucer].

It currently contains:

- `Reprojucer.cmake`, a CMake module that provides high-level functions to reproduce how a
JUCE project is defined in Projucer,

- `Jucer2Reprojucer`, a console application based on JUCE that converts `.jucer` project
files into `CMakeLists.txt` files that include and use `Reprojucer.cmake`,

- several `CMakeLists.txt` files generated from existing `.jucer` project files,
including:
  - `examples` and `extras` projects from [JUCE 4.2.0][github-juce-4.2.0] (in
[Jucer2Reprojucer/generated/JUCE-4.2.0](Jucer2Reprojucer/generated/JUCE-4.2.0))
  - `examples` and `extras` projects from [JUCE 4.3.1][github-juce-4.3.1] (in
[Jucer2Reprojucer/generated/JUCE-4.3.1](Jucer2Reprojucer/generated/JUCE-4.3.1))


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
# On Linux and on Windows
cmake .. -DJUCE_ROOT=../../../JUCE

cmake --build .
```

Then we convert `MyGreatProject.jucer` to a new `CMakeLists.txt` file:

```sh
cd <root>/MyGreatProject

# On macOs and on Windows
../JUCE.cmake/Jucer2Reprojucer/build/Debug/Jucer2Reprojucer MyGreatProject.jucer ../JUCE.cmake/cmake/Reprojucer.cmake
# On Linux
../JUCE.cmake/Jucer2Reprojucer/build/Jucer2Reprojucer MyGreatProject.jucer ../JUCE.cmake/cmake/Reprojucer.cmake
```

Now we can build `MyGreatProject` using CMake:

```sh
cd <root>/MyGreatProject

mkdir build && cd build

# On macOs
cmake .. -G Xcode -DMyGreatProject_jucer_FILE=../MyGreatProject.jucer
# On Linux and on Windows
cmake .. -DMyGreatProject_jucer_FILE=../MyGreatProject.jucer

cmake --build .
```


## Supported export targets

`Reprojucer.cmake` and `Jucer2Reprojucer` support the following Projucer export targets:
- Xcode (MacOSX)
- Visual Studio 2015
- Visual Studio 2013
- Linux Makefiles


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
[github-juce-4.2.0]: https://github.com/WeAreROLI/JUCE/tree/4.2.0
[github-juce-4.3.1]: https://github.com/WeAreROLI/JUCE/tree/4.3.1
[github-juce-cmake]: https://github.com/McMartin/JUCE.cmake
[github-juce]: https://github.com/WeAreROLI/JUCE
[gitter-badge]: https://badges.gitter.im/JUCE-cmake/Lobby.svg
[gitter]: https://gitter.im/JUCE-cmake/Lobby?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge
[juce]: https://juce.com/
[projucer]: https://www.juce.com/projucer
[travis-ci-badge]: https://travis-ci.org/McMartin/JUCE.cmake.svg?branch=master
[travis-ci]: https://travis-ci.org/McMartin/JUCE.cmake
