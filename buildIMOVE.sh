#!/bin/bash

# Build IMOVE with Game code included
cd IMOVE
set -e
mkdir -p build
pushd build >> /dev/null
	cmake -DCMAKE_BUILD_TYPE=Debug ..
	make -j4
#	make
popd >> /dev/null