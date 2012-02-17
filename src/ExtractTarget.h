#ifndef __EXTRACTTARGET_H
#define __EXTRACTTARGET_H

/*
 * ExtractTarget.h - description
 *
 * Copyright (C) 2012 aduda
 * German Research Center for Artificial Intelligence
 * DFKI RIC Bremen 2007-2012
 *
 */
#include <vector>
#include <algorithm>

namespace dsp 
{
    //Adaptive difference operations 
    //see: Extraction of Radar Target Length Based on High Resolution Range Profile
    //
    // size = left|right size of the operator window size*2 must be smaller than the number of the elements 
    // index = variable used for storing the extracted position
    // length = variable usde for storing the extracted length 
    template<class InputIterator,class AccumulatorType>
        void extractTarget(InputIterator first,InputIterator last, const int size ,int &index,int &length)
        {
            std::vector<AccumulatorType> temp;
            AccumulatorType left_value = 0;
            AccumulatorType right_value = 0;
    
            InputIterator iter_right = first;
            InputIterator iter_middle = first;
            InputIterator iter_left = first;
            for(int i=0;i < size;++iter_right,++i)
                left_value-= *iter_right;

            iter_middle = iter_right;

            for(int i=0;i < size;++iter_right,++i)
                right_value += *iter_right;

            temp.push_back(left_value+right_value);

            //iter_right is pointing to the next element which was not used so far
            //iter_middle is pointing to the first left elemet of the right side of the operator
            //iter_left is pointing to the first left element of the left side of the operator
            for(;iter_right != last;++iter_right,++iter_left,++iter_middle)
            {
                //move the operator one step to the right side
                left_value += *iter_left;
                left_value -= *iter_middle;
                right_value -= *iter_middle;
                right_value += *iter_right;
                temp.push_back(left_value+right_value);
            }

            //find min and max value 
            typename std::vector<AccumulatorType >::iterator iter_min;
            iter_min = std::min_element(temp.begin(),temp.end());
            typename std::vector<AccumulatorType >::iterator iter_max;
            iter_max = std::max_element(temp.begin(),temp.end());

            index = iter_max-temp.begin()+size;
            length = iter_min - temp.begin()-index+size;
        }
};

#endif 

