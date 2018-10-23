#!/bin/bash
set -e
mkdir -p ./config/
./build/bin/calibration ./config/test.yml 1 1920 1200
