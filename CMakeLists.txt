# CMakeLists.txt has to be located in the project folder and cmake has to be
# executed from 'project/build' with 'cmake ../'.
set(CMAKE_MODULE_PATH "${dsp_acoustics_SOURCE_DIR}/CMakeModules;{CMAKE_MODULE_PATH}")

cmake_minimum_required(VERSION 2.6)
include($ENV{ROCK_CMAKE_MACROS}/Rock.cmake)
rock_init(dsp_acoustics 0.1)
rock_standard_layout()

