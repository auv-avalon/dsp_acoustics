#include "writeWave.h"
WriteWave::WriteWave(const int samplerate, const int channels,const char *filename)
{
    //const int format=SF_FORMAT_WAV | SF_FORMAT_PCM_16;
    const int format=SF_FORMAT_WAV | SF_FORMAT_FLOAT;
    outfile = SndfileHandle(filename, SFM_WRITE, format, channels, samplerate);
}

WriteWave::~WriteWave(){}

void WriteWave::write(const float* data, size_t length )
{
    outfile.write( data,length);//
}
