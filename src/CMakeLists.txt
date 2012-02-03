
rock_library(dsp_acoustics
    SOURCES empty.cpp xcorr.cpp writeWave.cpp
    HEADERS FitPolynom.h FIRFilter.h xcorr.h zcr.h writeWave.h
    DEPS_PKGCONFIG fftw3f #fftw3f fftw3
    DEPS_PKGCONFIG sndfile

    )


