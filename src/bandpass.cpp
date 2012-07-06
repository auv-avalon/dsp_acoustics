#include "bandpass.h"
#include <math.h>

#include <stdio.h>
#include <string.h>

namespace dsp{

Bandpass::Bandpass(unsigned int signalSize):_signalSize(signalSize)
{
    _data1        = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * signalSize );
    _data2        = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * signalSize );

    //  memset(_data1,0,sizeof( fftw_complex ) * signalSize);
    //  memset(_data2,0,sizeof( fftw_complex ) * signalSize);

    _fft_result1  = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * signalSize );
    _fft_result2  = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * signalSize );
    _ifft_result1 = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * signalSize );
    _ifft_result2 = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * signalSize );

    _plan_forward1  = fftw_plan_dft_1d( signalSize, _data1, _fft_result1, FFTW_FORWARD, FFTW_ESTIMATE );
    _plan_forward2  = fftw_plan_dft_1d( signalSize, _data2, _fft_result2, FFTW_FORWARD, FFTW_ESTIMATE );
    _plan_backward1 = fftw_plan_dft_1d( signalSize, _fft_result1, _data1, FFTW_BACKWARD, FFTW_ESTIMATE );
    _plan_backward2 = fftw_plan_dft_1d( signalSize, _fft_result2, _data2, FFTW_BACKWARD, FFTW_ESTIMATE );
}

Bandpass::~Bandpass()
{
    fftw_destroy_plan( _plan_forward1 );
    fftw_destroy_plan( _plan_forward2 );
    fftw_destroy_plan( _plan_backward1 );
    fftw_destroy_plan( _plan_backward2 );
 
    fftw_free( _data1 );
    fftw_free( _fft_result1 );
    fftw_free( _data2 );
    fftw_free( _fft_result2 );
    fftw_free( _ifft_result1 );
    fftw_free( _ifft_result2 );
}

/******************************************************************************
 * input: right and left raw audio data
 * parameter: sampleRate, freq (given output freq),
         freqtolerance (a +/- tolerance around output signal)
 * output: will be writhen in the input arrays
 *****************************************************************************/

void Bandpass::calculate(std::vector<float> *ref,std::vector<float> *sig, int sampleRate, int freq, int freqtolerance)
{
    // not optimmal but well, will maybe optimiezed in the future..
    for(unsigned int i = 0 ; i < _signalSize; i++ )
    {
        _data1[i][0] = ref->at(i);
        _data1[i][1] = 0.0;
    }
    for(unsigned int i = 0 ; i < _signalSize; i++ )
    {
        _data2[i][0] = sig->at(i);
        _data2[i][1] = 0.0;
    }
    
    fftw_execute( _plan_forward1 );
    fftw_execute( _plan_forward2);

    int chunk_size = (sampleRate/2) / _signalSize;
    
    for(unsigned int i = 0 ; i < _signalSize; i++ )
    {
        if ((i + 1) * chunk_size < freq - freqtolerance && (i + 1) * chunk_size > freq + freqtolerance )
        {
            _fft_result1[i][0] = 0.0;
            _fft_result1[i][1] = 0.0;
            
            _fft_result2[i][0] = 0.0;
            _fft_result2[i][1] = 0.0;
        }
    }

    fftw_execute( _plan_backward1 );
    fftw_execute( _plan_backward2 );
}
}
