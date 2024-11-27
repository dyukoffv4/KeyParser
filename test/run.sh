#!/usr/bin/bash

mkdir -p build
cd build
cmake ..
cmake --build .
cd ..

echo -e "\nTEST START\n"

for i in $(ls bin)
do
    ./bin/$i
done
