#!/usr/bin/env bash

if [ -z "$1" ]
  then
    echo "usage: provide path and filename for .jucer file"
    echo "example: /<volumed directoy>/<project name>/<project name>.jucer"
    echo "note: make sure to volume in your juce project with -v"
    exit 1
fi
DIRECTORY=$(dirname "$1")
JUCERFILE=$(basename "$1")
FRUT_CMAKE="/Reprojucer.cmake"

cd ${DIRECTORY}
Jucer2Reprojucer ${JUCERFILE} ${FRUT_CMAKE}

exit 0 