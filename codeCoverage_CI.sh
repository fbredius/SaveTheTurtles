#!/usr/bin/env bash

# Only for use on gitlab.

# Build the testing executable.
echo -e "\e[0;32m[STATUS] Building test executable ...\e[0m"
cd game
rm -r build
rm -r CodeCoverageReport
mkdir build
cd build
cmake ..
make

# Run the unit tests.
echo -e "\e[0;32m[STATUS] Running tests ...\e[0m"
cd testing
test_output=$(./unit-test 2>&1)
test_exitcode=$?
echo "$test_output"
echo -e "\e[0;31m[STATUS] Test runner exitcode = $test_exitcode \e[0m"

if [[ $test_exitcode = 1 ]]; then
  echo -e "\e[0;31m[STATUS] The test suite failed! Fix failing tests before generating code coverage!\e[0m"
  exit 1
fi

if [[ $test_exitcode = 0 ]]; then
  echo -e "\e[0;35m[STATUS] The test suite passed! Continuing code coverage generation process ...\e[0m"
fi

# Generate html test report using lcov.
echo -e "\e[0;32m[STATUS] Generating coverage report ...\e[0m"
cd ..
cd ..
lcov --capture --directory build --output-file coverage.info

# These files are excluded from testing coverage because they are tested by the description-based-tests.
# It is not a good strategy to test visuals using unit tests because verification that something is displayed
# correctly is difficult and can be verified by a hum running the description-based-tests.
lcov -r coverage.info '/src/rendering/RenderWrapper.cpp' -o coverage.info
lcov -r coverage.info '/src/rendering/AssetFactory.cpp' -o coverage.info
lcov -r coverage.info '/src/rendering/AnimatedSprite.cpp' -o coverage.info
lcov -r coverage.info '/src/rendering/Animation.cpp' -o coverage.info
lcov -r coverage.info '/src/rendering/AnimationHandler.cpp' -o coverage.info

# Tests have been written for the AssetLoader and the AnimationData class but unfortunately they fail on CI because SFML has no headless mode.
# The tests can be run by enabling them from testMain.cpp.
# They cover the AssetLoader and the AnimationData class almost entirely thus we excluded it from coverage to get an accurate portrayal of
# what our coverage would be if it ran on CI.
lcov -r coverage.info '/src/rendering/AssetLoader.cpp' -o coverage.info
lcov -r coverage.info '/src/rendering/AnimationData.cpp' -o coverage.info

lcov -r coverage.info '/usr/include/*' -o coverage.info
lcov -r coverage.info '/testing/googletest/*' -o coverage.info
lcov -r coverage.info '/testing/test-src/*' -o coverage.info
lcov -r coverage.info '*.h' -o coverage.info
genhtml coverage.info --output-directory CodeCoverageReport
rm coverage.info
echo -e "\e[0;32m[STATUS] Coverage report generated!\e[0m"