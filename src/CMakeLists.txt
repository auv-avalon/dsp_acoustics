
rock_library(dsp_acoustics
    SOURCES empty.cpp xcorr.cpp writeWave.cpp GraphUtils.cpp
    HEADERS FitPolynom.h FIRFilter.h xcorr.h zcr.h writeWave.h GraphUtils.h ExtractTarget.h
    DEPS_PKGCONFIG fftw3f opencv #fftw3f fftw3
    DEPS_PKGCONFIG sndfile
    )


