# ------ Don't Touch Zone ------

cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 11)

# Enable debug symbols by default
# must be done before project() statement
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif ()
# (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)

# ------ End Don't Touch Zone ------

# ------- Add src files here -------

file(GLOB GLOB_FILES_TO_INCLUDE
        Main.cpp
        Main.h
        control/*.h
        control/*.cpp
        control/gameStates/*.h
        control/gameStates/*.cpp
        gameEntities/*.h
        gameEntities/*.cpp
        gameEntities/turtle/*.h
        gameEntities/turtle/*.cpp
        gameEntities/turtle/turtleStates/*.h
        gameEntities/turtle/turtleStates/*.cpp
        rendering/*.h
        rendering/*.cpp
        storage/*.h
        storage/*.cpp
        utility/*.h
        utility/*.cpp
        )

# ------- End -------

# ------ Don't Touch Zone ------

# Define sources and executable
set(EXECUTABLE_NAME "Game_Demo")
add_executable(${EXECUTABLE_NAME}
        Main.cpp Main.h ${GLOB_FILES_TO_INCLUDE})

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2 REQUIRED network audio graphics window system)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif ()

# Install target
install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)

# CPack packaging
include(InstallRequiredSystemLibraries)
include(CPack)

# ------ End Don't Touch Zone ------