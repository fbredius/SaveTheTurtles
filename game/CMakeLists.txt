cmake_minimum_required(VERSION 2.6)
project(Game)

add_subdirectory(src)
add_subdirectory(testing)

# Enable debug symbols by default
# must be done before project() statement
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif ()
# (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)

# CPack packaging
include(InstallRequiredSystemLibraries)
include(CPack)