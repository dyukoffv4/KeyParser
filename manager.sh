#!/usr/bin/bash

COMMAND_1=$1
COMMAND_2=$2
CURRENT_PATH=$(dirname $(realpath $0))

function Cmake {
    mkdir -p build/cmake
    cd build/cmake
    cmake $CURRENT_PATH
    cmake --build .
    cd $CURRENT_PATH
}

function Build {
    bash package_settings/build.sh
}

function Clean {
    if [[ $COMMAND_2 == "all" ]]; then rm -rf build; fi
    if [[ $COMMAND_2 == "cmake" ]]; then rm -rf build/cmake; fi
    if [[ $COMMAND_2 == "build" ]]; then rm -rf build/lkeyparser; fi
}

cd $CURRENT_PATH
if [[ $COMMAND_1 == "cmake" ]]; then Cmake; fi
if [[ $COMMAND_1 == "build" ]]; then Build; fi
if [[ $COMMAND_1 == "clean" ]]; then Clean; fi
