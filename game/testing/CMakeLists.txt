# ------ Don't Touch Zone ------

cmake_minimum_required(VERSION 2.8)
#Turn on C++11 Support and creation of .gcno files that are needed for coverage.
SET(CMAKE_CXX_FLAGS "-std=gnu++11 -g -O0 -Wall -fprofile-arcs -ftest-coverage" ${CMAKE_CXX_FLAGS})
SET(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)

# ------ End Don't Touch Zone ------

# ------- Add test files here -------

file(GLOB GLOB_UNIT_TESTS
        test-src/testMain.cpp
        test-src/unit-tests/control/*.cpp
        test-src/unit-tests/control/gameStates/*.cpp
        test-src/unit-tests/gameEntities/*.cpp
        test-src/unit-tests/gameEntities/turtle/*.cpp
        test-src/unit-tests/gameEntities/turtle/turtleStates/*.cpp
        test-src/unit-tests/storage/*.cpp
        test-src/unit-tests/rendering/*.cpp
        test-src/unit-tests/utility/*.cpp
        test-src/description-based-tests/*.cpp
        test-src/description-based-tests/*.h
        test-src/feature-tests/*.cpp
        test-src/feature-tests/*.h
        )

# ------- End -------

# ------ Don't Touch Zone ------

add_subdirectory(googletest) #Build all the gtest stuff
include_directories(googletest/googletest/include)
include_directories(.)

set(EXECUTABLE_NAME "unit-test")

add_executable(${EXECUTABLE_NAME} ${GLOB_UNIT_TESTS})
target_link_libraries(unit-test gtest gtest_main rt pthread codeToTest gmock_main)

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2 REQUIRED network audio graphics window system)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif ()

# ------ End Don't Touch Zone ------

# ------- Add new code from the game here if you need to access it from tests -------

file(GLOB GLOB_FILES_TO_TEST
        ../src/Main.cpp
        ../src/Main.h
        ../src/control/*.h
        ../src/control/*.cpp
        ../src/control/gameStates/*.h
        ../src/control/gameStates/*.cpp
        ../src/gameEntities/*.h
        ../src/gameEntities/*.cpp
        ../src/gameEntities/turtle/*.h
        ../src/gameEntities/turtle/*.cpp
        ../src/gameEntities/turtle/turtleStates/*.h
        ../src/gameEntities/turtle/turtleStates/*.cpp
        ../src/rendering/*.h
        ../src/rendering/*.cpp
        ../src/storage/*.h
        ../src/storage/*.cpp
        ../src/utility/*.h
        ../src/utility/*.cpp
        )

# ------- End -------

# ------ Don't Touch Zone ------

message(STATUS "------------- Included Files -----------")
message(STATUS ${GLOB_FILES_TO_TEST})
message(STATUS "----------------------------------------")

set(FILES_TO_TEST ${GLOB_FILES_TO_TEST})
add_library(codeToTest ${FILES_TO_TEST})

# ------ End Don't Touch Zone ------