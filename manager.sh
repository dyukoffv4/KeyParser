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

function Tests {
    cd tests
    if [[ $COMMAND_2 == "all" ]]; then Tests_Cmake; Tests_Run; fi
    if [[ $COMMAND_2 == "cmake" ]]; then Tests_Cmake; fi
    if [[ $COMMAND_2 == "run" ]]; then Tests_Run; fi
    cd ..
}

function Tests_Cmake {
    cmake -B build -S .
    cmake --build build
}

function Tests_Run {
    echo -e "\nTEST START\n"
    for i in $(ls bin); do ./bin/$i; done
}

function Clean {
    if [[ $COMMAND_2 == "all" ]]; then rm -rf build tests/build tests/bin; fi
    if [[ $COMMAND_2 == "cmake" ]]; then rm -rf build/cmake; fi
    if [[ $COMMAND_2 == "build" ]]; then rm -rf build/keyparser; fi
    if [[ $COMMAND_2 == "tests" ]]; then rm -rf tests/build tests/bin; fi
}

cd $CURRENT_PATH
if [[ $COMMAND_1 == "cmake" ]]; then Cmake; fi
if [[ $COMMAND_1 == "build" ]]; then Build; fi
if [[ $COMMAND_1 == "tests" ]]; then Tests; fi
if [[ $COMMAND_1 == "clean" ]]; then Clean; fi
