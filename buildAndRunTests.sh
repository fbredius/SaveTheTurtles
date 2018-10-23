#!/usr/bin/env bash

# Build the testing executable.
cd game
rm -r build
mkdir build
cd build
cmake ..
make

# Run the unit tests.
cd testing
./unit-test