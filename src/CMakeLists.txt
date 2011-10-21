rock_library(dsp_acoustics
    SOURCES Dummy.cpp
    HEADERS Dummy.hpp)

rock_executable(dsp_acoustics_bin Main.cpp
    DEPS dsp_acoustics)

