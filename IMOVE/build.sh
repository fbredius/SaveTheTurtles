#!/bin/bash
set -e
mkdir -p build
pushd build >> /dev/null
	cmake -DCMAKE_BUILD_TYPE=Debug ..
	make -j4
#	make
popd >> /dev/null
