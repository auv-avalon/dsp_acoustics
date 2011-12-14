
rock_library(dsp_acoustics
    SOURCES empty.cpp xcorr.cpp
    HEADERS FitPolynom.h FIRFilter.h xcorr.h
    DEPS_PKGCONFIG fftw3f #fftw3f fftw3
    )


