# Save The Turtles
A collaborative and movement-based game, developed using the IMOVE framework.

# Group: CG1
- Raoul Bruens - 4571010
- Tim Huisman - 4591305
- Mariette Schonfeld - 4474147
- Thijmen Langendam - 4592646
- Maxim Liefaard - 4607171

# Deployment Guide

This section contains guide for setting up the project and using the different runnables and build scripts included in the repository.

## Dependency Setup

The project was based upon Ubuntu 16:04. If you choose to use a later iteration of Ubuntu please note that you could get more updated versions of our dependencies. This can cause unwanted behaviour like compilation issue or different coverage/static analysis results. The studio does not take this into account and we assume that you use Ubuntu 16:04 because ensuring compatability with multiple operating systems is beyond the scope of this project.
Once you have ensured that you use the correct operating system you can automatically install dependencies by running the *installation\_script.sh* script. This took our computers 20 to 30 minutes.
Note that code coverage and Cppcheck have more specific dependencies, refer to their subsections for more details. Clion is needed for running description-based tests.

## Running the game standalone

The game can be run in standalone mode by running the *buildAndRunGame.sh* script.

To change aspects of the game please refer to the Variables.cpp class and the section that documents all these variables.

Note: We plan on using a dedicated config file on release instead of this setup.

## Running the game with IMOVE

The game can be run with IMOVE by running the *buildAndRunIMOVE.sh* script.

To change aspects of the game please refer to the *Variables.cpp* class and the section that documents all these variables.

To change aspects of IMOVE please refer to the official IMOVE documentation. The config files are located in *IMOVE/config*. The currently used config can be found inside the *buildAndRunIMOVE.sh* script. An immediate crash is most often caused by an incorrect *Camera_device* setting.

## Running automated tests

Automated tests can be run using the *buildAndRunTests.sh* script. We use GoogleTest as a testing framework.

## Running description-based tests

Based on feedback we added description-based tests of our rendering aspects in the game. These are located in *testing/test-src/description-based-tests/*. There is a markdown file that describes all test cases. To run the tests look into the testMain.cpp class for instructions and run testMain.cpp using the play button in CLion. 

## Generating code coverage

Code coverage is generated using *lcov*. Please refer to the contents of *generateCodeCoverage.sh* for instructions on how to install this dependency.

Once the dependencies are installed you can run the script, it will only generate coverage if all tests pass and it will automatically open the coverage report in chrome (it assumes you have chrome installed).

The generated report will be located in *game/CodeCoverageReport*.

## Running CppCheck

CppCheck is the tool that we use for static analysis. Please refer to the contents of *runCppCheck.sh* for instructions on how to install CppCheck. The tool runs fairly quickly and prints all warnings in the console.

False positives or useless warnings were suppressed from the *CppCheckSupression.txt* files. If you need motivation for any of the suppression the studio can always be contacted.

## Docker Images used for CI on GitLab:

- [With IMOVE support - GitHub](https://github.com/maximliefaard/docker-ubuntu-imove)
- [With IMOVE support - DockerHub](https://hub.docker.com/r/maximliefaard/docker-ubuntu-imove/)
- [Without IMOVE support - GitHub](https://github.com/maximliefaard/docker-ubuntu-without-imove)
- [Without IMOVE support - DockerHub](https://hub.docker.com/r/maximliefaard/docker-ubuntu-without-imove/)
