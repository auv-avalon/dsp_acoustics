#ifndef __DSPACOUSTICS_HPP
#define __DSPACOUSTICS_HPP

/*
 * DspAcoustics.hpp - description
 *
 * Copyright (C) 2012 aduda
 * German Research Center for Artificial Intelligence
 * DFKI RIC Bremen 2007-2012
 *
 */

#include <vector>
#include <stdint.h>
#include <iostream>

#include "../src/ExtractTarget.h"
void extractTarget(std::vector<char> values, int size, int &index,int &length ,std::vector<int> &result)
{
    result = (dsp::extractTarget<std::vector<char>::iterator,int>(values.begin(),values.end(),size,index,length));
}
#endif 

