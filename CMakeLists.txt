# CMakeLists.txt has to be located in the project folder and cmake has to be
# executed from 'project/build' with 'cmake ../'.
OPTION(USE_FFTW3 "Use FFTW3 (double-precision) (GPL) as FFT library." OFF)
OPTION(USE_FFTW3F "Use FFTW3 (single-precision) (GPL) as FFT library." ON)

cmake_minimum_required(VERSION 2.6)
include($ENV{ROCK_CMAKE_MACROS}/Rock.cmake)
rock_init(dsp_acoustics 0.1)
rock_standard_layout()
