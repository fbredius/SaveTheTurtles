#!/usr/bin/env bash

# Cppcheck installation instructions:
# install cppcheck: sudo apt-get install cppcheck
# install the cppcheck CLion plugin:
# 1. File > Settings > Plugins > Browse repositories > Search & install cppcheck
# 2. Restart CLion
# 3. Get the path to cppcheck using the following command in terminal: which cppcheck
# 4. Go to File > Settings > cppcheck configuration > set cppcheck path to the result of the previous step.
# 5. Cppcheck should now mark errors in CLion & You should be able to run this script for an entire project scan!

# NOTE: Cppcheck warns that --check-config should be added. However this results in a lot of include warnings.
# This happens because Cppcheck is unable to find our dependencies, but CMake is able to do so, thus we ignored this.

echo -e "\e[0;32m[STATUS] Running CppCheck: game/src/*\e[0m"
game_output=$(cppcheck --suppressions-list=CppCheckSuppression.txt --force --quiet --suppress=missingIncludeSystem --enable=warning,performance,information,style ../game/src/* 2>&1)
echo "$game_output"
echo -e "\e[0;32m[STATUS] Finished!\e[0m"

echo " "

echo -e "\e[0;32m[STATUS] Running CppCheck: game/testing/test-src/*\e[0m"
test_output=$(cppcheck --suppressions-list=CppCheckSuppression.txt --force --quiet --suppress=missingIncludeSystem --enable=warning,performance,information,style ../game/testing/test-src/* 2>&1)
echo "$test_output"
echo -e "\e[0;32m[STATUS] Finished!\e[0m"

echo " "

echo -e "\e[0;32m[STATUS] Processing output ...\e[0m"

if [[ $game_output = *"["*"]:"*"("*")"* ]]; then
  echo -e "\e[0;31m[STATUS] CppCheck found an error when checking the src folder!\e[0m"
  exit 13
fi

if [[ $test_output = *"["*"]:"*"("*")"* ]]; then
  echo -e "\e[0;31m[STATUS] CppCheck found an error when checking the test-src folder!\e[0m"
  exit 13
fi

echo -e "\e[0;35m[STATUS] CppCheck passed!\e[0m"
exit 0