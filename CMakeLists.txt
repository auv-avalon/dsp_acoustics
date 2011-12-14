# CMakeLists.txt has to be located in the project folder and cmake has to be
# executed from 'project/build' with 'cmake ../'.
OPTION(USE_FFTW3 "Use FFTW3 (double-precision) (GPL) as FFT library." OFF)
OPTION(USE_FFTW3F "Use FFTW3 (single-precision) (GPL) as FFT library." ON)
set(CMAKE_MODULE_PATH "${dsp_acoustics_SOURCE_DIR}/CMakeModules;{CMAKE_MODULE_PATH}")


IF(USE_FFTW3F)
  find_package (fftw3f-3 REQUIRED)
  MESSAGE(STATUS "Using FFTW3F (single-precision) (GPL) as FFT library.")

  ADD_DEFINITIONS(-DUSE_FFTW3F)
  SET( FFT_INCLUDE_DIR ${FFTW3F-3_INCLUDE_DIR} )
  SET( FFT_LIBRARY ${FFTW3F-3_LIBRARY} )

  SET(USE_FFTW3 FALSE)
  SET(USE_FFTSS FALSE)
ELSEIF(USE_FFTW3)
  find_package (fftw3-3 REQUIRED)
  MESSAGE(STATUS "Using FFTW3 (double-precision) (GPL) as FFT library.")

  ADD_DEFINITIONS(-DUSE_FFTW3)
  SET( FFT_INCLUDE_DIR ${FFTW3-3_INCLUDE_DIR} )
  SET( FFT_LIBRARY ${FFTW3-3_LIBRARY} )

  SET(USE_FFTW3F FALSE)
  SET(USE_FFTSS FALSE)
ELSE()
  # Error if none of the three is selected.
  MESSAGE("No FFT library selected, you will not be able to generate ocean surfaces.")
ENDIF()





cmake_minimum_required(VERSION 2.6)
include($ENV{ROCK_CMAKE_MACROS}/Rock.cmake)
rock_init(dsp_acoustics 0.1)
rock_standard_layout()



TARGET_LINK_LIBRARIES(
  ${FFT_LIBRARY}
   ${FFTW_LIBRARIES}
)

