#ifndef WRITEWAVE_H
#define WRITEWAVE_H

#include <sndfile.hh>
#include<iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
                        
class WriteWave{
    
    public:
    WriteWave(const int samplerate, const int channels,const char *filename);
    ~WriteWave();
    void write(const float* data, size_t length );
    protected:
       
     SndfileHandle outfile;  
};

#endif
