#!/usr/bin/bash
if [[ $# == 0 ]]; then exit 1; fi

function Build {
    if [[ $1 == "--clean" ]]; then
        echo "Clearing library binary files ..."
        Clean cmake;
    else
        echo "Creating package ..."
        cmake ./cmake && cmake --build ./cmake && cmake --build ./cmake --target package;
    fi
}

function Tests {
    if [[ $1 == "--clean" ]]; then
        echo "Clearing tests binary files ..."
        Clean test cmake;
    else
        echo "Compile tests ..."
        cmake .test/cmake && cmake --build .test/cmake;
        ./test/cmake/a.out                              # Wrong output!
    fi
}

function Clean {
    for i in "$@"; do cd "$i/" || return 1; done

    if [[ $(find . -maxdepth 1 -name "CMakeLists.txt") == "./CMakeLists.txt" ]]
    then ls -a | grep -v "CMakeLists.txt" | xargs rm -rf
    fi

    for i in "$@"; do cd "../" || return 1; done
}

if [[ $1 == "build" ]]; then Build "$2"; fi
if [[ $1 == "tests" ]]; then Tests "$2"; fi
exit 0
