#!/usr/bin/env bash

# Build into separate build folder.
cd game
set -e
mkdir -p build
pushd build >> /dev/null
	cmake -DCMAKE_BUILD_TYPE=Debug ..
	make -j4

popd >> /dev/null

# Run the program.
cd build
cd src
./Game_Demo