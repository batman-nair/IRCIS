#!/bin/sh

CONFIG=Release			# Default Release build

if [[ ! -z $1 ]]
then
    CONFIG=$1
fi

cmake -E make_directory build/$CONFIG
cd build/$CONFIG
cmake -DCMAKE_BUILD_TYPE=${CONFIG} ../../
cmake --build . --config "${CONFIG}"
ctest
