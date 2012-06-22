#ifndef XCORR_H
#define XCORR_H

#include <fftw3.h>

namespace dsp
{
    #if defined(USE_FFTW3)              // double precision
        typedef double fftw_data_type;
    #else //if defined(USE_FFTW3F)      // single precision
        typedef float fftw_data_type;

        // alias the functions so that we can use the same function calls in both
        // the double- and single-precision FFTW paths.
        #define fftw_complex         fftwf_complex
        #define fftw_plan            fftwf_plan
        #define fftw_destroy_plan    fftwf_destroy_plan
        #define fftw_plan_dft_2d     fftwf_plan_dft_2d
        #define fftw_plan_dft_1d     fftwf_plan_dft_1d
        #define fftw_execute         fftwf_execute
        #define fftw_malloc          fftwf_malloc
        #define fftw_free            fftwf_free
    #endif

    class Xcorr{
    public:
        Xcorr(unsigned int signalSize);
        ~Xcorr();
        int calculate(const fftw_data_type *ref,const fftw_data_type *sig, bool show_debug = false);
        double getCorrelation(){return _correlation;};
    protected:
        unsigned int _signalSize;
        //unsigned _bufSize;
        int _lag;
        double _correlation;
        fftw_complex *_data1,*_data2, *_fft_result1,*_fft_result2, *_ifft_result;
        fftw_plan _plan_forward1,_plan_forward2, _plan_backward;
    };
}
#endif
