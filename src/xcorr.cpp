#include "xcorr.h"
#include <math.h>
#include "GraphUtils.h"

namespace dsp
{

Xcorr::Xcorr(unsigned int signalSize):_signalSize(signalSize)
{
    _data1        = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * signalSize );
    _data2        = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * signalSize );

    //  memset(_data1,0,sizeof( fftw_complex ) * signalSize);
    //  memset(_data2,0,sizeof( fftw_complex ) * signalSize);

    _fft_result1  = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * signalSize );
    _fft_result2  = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * signalSize );
    _ifft_result = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * signalSize );

    _plan_forward1  = fftw_plan_dft_1d( signalSize, _data1, _fft_result1, FFTW_FORWARD, FFTW_ESTIMATE );
    _plan_forward2  = fftw_plan_dft_1d( signalSize, _data2, _fft_result2, FFTW_FORWARD, FFTW_ESTIMATE );
    _plan_backward = fftw_plan_dft_1d( signalSize, _fft_result1, _ifft_result, FFTW_BACKWARD, FFTW_ESTIMATE );
}

Xcorr::~Xcorr()
{
    fftw_destroy_plan( _plan_forward1 );
    fftw_destroy_plan( _plan_forward2 );
    fftw_destroy_plan( _plan_backward );
 
    fftw_free( _data1 );
    fftw_free( _fft_result1 );
    fftw_free( _data2 );
    fftw_free( _fft_result2 );
    fftw_free( _ifft_result );
}

int Xcorr::calculate(const fftw_data_type *ref,const fftw_data_type *sig, bool show_debug)
{
    double na=0;
    // not optimmal but well, will maybe optimiezed in the future..
    for(unsigned int i = 0 ; i < _signalSize; i++ )
    {
        _data1[i][0] = ref[i];
        _data1[i][1] = 0.0;
        na += ref[i] *ref[i];
    }
    double nb=0;
    for(unsigned int i = 0 ; i < _signalSize; i++ )
    {
        _data2[i][0] = sig[i];
        _data2[i][1] = 0.0;
         nb += sig[i] *sig[i];
    }
    fftw_execute( _plan_forward1 );
    fftw_execute( _plan_forward2);

    if(show_debug)
    {
        std::vector<float> a_r1(_signalSize);
        std::vector<float> b_r1(_signalSize);
        std::vector<float> a_r2(_signalSize);
        std::vector<float> b_r2(_signalSize);
        for(unsigned int i = 0 ; i < _signalSize; i++ )
        {
            a_r1[i] = _fft_result1[i][0];
            b_r1[i] = _fft_result1[i][1];
            a_r2[i] = _fft_result2[i][0];
            b_r2[i] = _fft_result2[i][1];
            //std::cout<<"fft 1: " <<_fft_result1[i][0] <<"    "<<_fft_result1[i][1]<<"\n";
            //std::cout<< "fft 2: " << _fft_result2[i][0] <<"    "<<_fft_result2[i][1]<<"\n";
        }
        showFloatGraph("a anteil result 1", a_r1.data(), _signalSize);
        showFloatGraph("b anteil result 1", b_r1.data(), _signalSize);
        showFloatGraph("a anteil result 2", a_r2.data(), _signalSize);
        showFloatGraph("b anteil result 2", b_r2.data(), _signalSize);
    }
    
    // multiply them...
    for(unsigned int i = 0 ; i < _signalSize; i++ ) {
        _fft_result2[i][1] *=-1; // complex conejugat
        double rtemp =  (_fft_result1[i][0] * _fft_result2[i][0] ) -  (_fft_result1[i][1] * _fft_result2[i][1] );
        _fft_result1[i][1] =  (_fft_result1[i][0] * _fft_result2[i][1] ) +  (_fft_result1[i][1] * _fft_result2[i][0] );
        _fft_result1[i][0] = rtemp;
    //cout<< "conv: "<<_fft_result1[i][0]<<"     " <<_fft_result1[i][1] << "\n";
    }

    fftw_execute( _plan_backward );

    // scaling...
    /*
    for(unsigned int i = 0 ; i < _signalSize; i++ )
    {
        _ifft_result[i][0] /= _signalSize;
        _ifft_result[i][1] /= _signalSize;
    }
    */
    
    if(show_debug)
    {
        std::vector<float> a_rf(_signalSize);
        std::vector<float> b_rf(_signalSize);
        for(unsigned int i = 0 ; i < _signalSize; i++ )
        {
            a_rf[i] = _ifft_result[i][0];
            b_rf[i] = _ifft_result[i][1];
        }
        showFloatGraph("a anteil result final", a_rf.data(), _signalSize);
        showFloatGraph("b anteil result final", b_rf.data(), _signalSize,0);
    }
    
    // search max...
    double max =_ifft_result[0][0];
    unsigned int index =0;

    for(unsigned int i = 1 ; i < _signalSize; i++ )
    {
        if(_ifft_result[i][0] > max)
        {
            index = i;
            max =  _ifft_result[i][0];
            //cout<<"new max value :"<<  _ifft_result[i][0] <<" index "<< i <<endl;
        }
       //cout<<  "inv  " << i <<" "<<_ifft_result[i][0] << _ifft_result[i][1] <<"\n";
    }
    _lag = index ;
    int signalHalf  = _signalSize >>1;
    if(_lag > signalHalf )
    { // wrap if to high...
    _lag = index - _signalSize;
    }

    _correlation = ( max /_signalSize) /  (sqrt(na) * sqrt(nb));
    return _lag;
}

}