
#ifndef __FIRFILTER_H 
#define __FIRFILTER_H

#include <math.h>
#include <cmath>
#include <vector>
#include <iterator>
#include <stdexcept>

namespace dsp
{
    //moving average filter
    //filters all points between [first,last) leaving last untouched 
    //filtered points are symmetrically chosen around the output point
    //all input points are equally weighted
    //window_size must be an odd number otherwise one is subtracted
    //the accumulator holding the sum of the window must be chosen according to the data types
    //the filter size is reduced down to the half at the beginning and end
    //
    //for fixed point types you can use movingAverageFilterSymI instead
    //for floating point types you can use movingAverageFilterSymD instead
    //
    template<class InputIterator, class OutputIterator,class AccumulatorType>
        void movingAverageFilterSym(InputIterator first,InputIterator last,OutputIterator result,unsigned int window_size)
        {
            //initialize variables
            window_size -= 1-window_size%2;                      //forces window_size to be odd
            int half_window_size = window_size*0.5;

            AccumulatorType window_val = 0;
            float weight = 1.0/window_size;
            InputIterator window_end = first;

            //fill the right half of the window and the center
            for(int i=0;i <= half_window_size && window_end != last;++i,++window_end)
                window_val += *window_end;

            //start filtering and fill the left half of the window
            for(int i=half_window_size+1;i< (int)window_size && window_end != last;++i,++window_end,++result)
            {
                *result = window_val/i;
                window_val += *window_end;
            }

            if(window_end == last)
                throw std::runtime_error("Window size is too big and exceeds the number of points!");

            //filter all data until the window reaches the end
            for(;window_end != last;++first,++window_end,++result)
            {
                *result = window_val*weight;
                window_val -= *first;
                window_val += *window_end;
            }

            //filter all remaining data 
            for(int i=0;i <= half_window_size;++i,++first,++result)
            {
                *result = window_val/(window_size-i);
                window_val -= *first;
            }
        }

    //to allow a simple syntax
    //movingAverageFilterSymI(first,last,window_size);
    //use movingAverageFilterSymD for floating point types
    template<class InputIterator, class OutputIterator>
        void movingAverageFilterSymI(InputIterator first,InputIterator last,OutputIterator result,unsigned int window_size)
        {
            movingAverageFilterSym<InputIterator,OutputIterator,unsigned int>(first,last,result,window_size);
        }
    template<class InputIterator, class OutputIterator>
        void movingAverageFilterSymD(InputIterator first,InputIterator last,OutputIterator result,unsigned int window_size)
        {
            movingAverageFilterSym<InputIterator,OutputIterator,double>(first,last,result,window_size);
        }


    //finds local min max
    //dereferenced value of iterator has to be a number
    //returns a vector with iterators for min1 max1 min2 max2 ...
    template<class InputIterator>
        void findLocalMinMax(InputIterator first, InputIterator last,std::vector<InputIterator> &result)
        {
            InputIterator value = first++;
            while(first != last)
            {
                //search min
                for(;first != last && *value > *first;++value,++first);
                result.push_back(value);

                if(first == last)
                    break;

                //search max
                for(;first != last && *value <= *first;++value,++first);
                result.push_back(value);
            }
        }


    enum SearchState
    {
        SEARCHING_MIN1,
        SEARCHING_MAX,
        SEARCHING_MIN2
    };
    template<typename TIter>
        void findLocalMinMax(float min_delta_start,float min_delta_end,TIter start,TIter end,std::vector<TIter> &result)
        {
            SearchState search_state = SEARCHING_MIN1;
            TIter start2 = start+1;
            TIter it_min1 = start;
            TIter it_min2 = start;
            TIter it_max = start;

            while(start2 != end) 
            {
                //    std::cout << "searchstate:"<< search_state<<" :" << *start << " " <<*start2 << std::endl;
                switch(search_state) 
                {
                    // searching for the left maximum 
                case SEARCHING_MIN1:
                    {
                        //check if we found a better minimum 
                        if(*start2 <= *start)
                        {
                            it_min1 = start2;
                            break;
                        }

                        //we have to erase the stored min max min because we found a better min
                        //and go back to find min2
                        if(!result.empty())
                        {
                            int size = result.size();
                            it_min2 = it_min1;
                            it_min1 = result[size-3];
                            it_max = result[size-2];
                            result.pop_back();
                            result.pop_back();
                            result.pop_back();
                            search_state = SEARCHING_MIN2;
                            break;
                        }

                        //found min1
                        it_max = start2;
                        search_state = SEARCHING_MAX;
                        break;
                    }
                    // searching for the maximum 
                case SEARCHING_MAX:
                    {
                        //we found a better minimum 
                        //while looking for maximum
                        if(*start2 <= *it_min1)
                        {
                            it_min1 = start2;
                            search_state = SEARCHING_MIN1;
                            break;
                        }

                        //next value is a better maximum
                        if(*start2 >= *start) 
                        {
                            it_max=start2;
                            break;
                        }

                        //keep on searching if delta is too small
                        if(*it_max-*it_min1 < min_delta_start)
                            break;

                        //found maximum 
                        //search for right minimum
                        it_min2 = start2;
                        search_state = SEARCHING_MIN2;
                        break;
                    }
                    // searching for the right minimum 
                case SEARCHING_MIN2:
                    {
                        //found better min 
                        if(*start2 <= *start) 
                        {
                            it_min2 = start2;
                            break;
                        }

                        //check if we found a better maximum 
                        if(*start2 >= *it_max) 
                        {
                            it_max = start2;
                            search_state = SEARCHING_MAX;
                            break;
                        }

                        //keep on searching if delta is too small
                        if(*it_max-*it_min2 < min_delta_end)
                            break;

                        //we finally found a min max min
                        result.push_back(it_min1);
                        result.push_back(it_max);
                        result.push_back(it_min2);
                        //   std::cout << "store" << *it_min1 << " " << *it_max << " " << *it_min2 << std::endl;

                        it_min1 = it_min2;
                        it_max = start2;
                        search_state = SEARCHING_MAX;
                        break;
                    }
                }
                start=start2;
                ++start2;
            }
            //save last state if valid
            if(search_state == SEARCHING_MIN1 && !result.empty())
                result[result.size()-1] = it_min1;

            if(search_state == SEARCHING_MIN2 && *it_max-*it_min2 >= min_delta_end)
            {
                result.push_back(it_min1);
                result.push_back(it_max);
                result.push_back(it_min2);
            }
        }
        
    /**
     * Finds the next local minimum to to the right side.
     * Retruns the first element if the following graph is rising.
     * @param window_size this span is used to get the slope
     */
    template<class InputIterator>
        InputIterator findFirstRightLocalMin(InputIterator first, InputIterator last, unsigned int window_size = 2)
        {
            InputIterator result = first;
            InputIterator window_begin = first;
            InputIterator window_end = first + window_size;
            while(window_end < last)
            {
                if(*window_begin <= *window_end)
                    break;
                
                if(*result > *window_end)
                    result = window_end;
                
                window_begin++;
                window_end++;
            }
            return result;
        }

    /**
     * Filters the elements proportionally to the average of its values.
     * @param average returns the average of all values
     * @param proportional_threshold min threshold in percent of average
     * @param count_empty_bins count empty bins to build the average
     */
    template<class InputIterator, class OutputIterator, class AccumulatorType>
        void proportionallyThresholdFilter(InputIterator first, InputIterator last, OutputIterator result, 
                                           AccumulatorType &average_value, float proportional_threshold, bool count_empty_bins = true)
        {
            average_value = 0;
            InputIterator start = first;
            unsigned int element_count = 0;
            while(start != last)
            {
                average_value += *start;
                if(count_empty_bins || *start > 0)
                    element_count++;
                start++;
            }
            average_value = average_value / element_count;
            
            AccumulatorType threshold = average_value * proportional_threshold;
            start = first;
            while(start != last)
            {
                if(*start < threshold)
                    *result = 0;
                else
                    *result = *start;
                start++;
                result++;
            }
        }
         
    /**
     * Creates the derivative of a signal.
     * @param window_size this span is used to get the slope
     * @param resolution allows you to divide the values by a factor. (e.g. 0.1 means 10 entries per meter)
     * @throws runtime_error
     */
    template<class InputIterator, class OutputIterator>
        void derivativeSignal(InputIterator first, InputIterator last, OutputIterator result, unsigned int window_size = 2, float resolution = 1)
        {
            if(first == result)
                throw std::runtime_error("derivativeSignal won't work with same iterator for input and output!");
            if(last - first < (int)window_size)
                throw std::runtime_error("The input signal should not have less elements than the window_size.");
                
            unsigned int half_window_size = window_size * 0.5;
            
            InputIterator window_begin = first;
            InputIterator window_end = first + half_window_size;
            while(first != last)
            {
                *result = (*window_end - *window_begin) / (half_window_size * 2 * resolution);
                if(window_begin + half_window_size > first)
                {
                    window_end++;
                }
                else if(window_end + 1 >= last)
                {
                    window_begin++;
                }
                else
                {
                    window_begin++;
                    window_end++;
                }
                result++;
                first++;
            }
        }
        
    /**
     * Accumulates the signal to a normalized limit.
     * @param normalized_limit
     */
    template<class InputIterator, class OutputIterator, class AccumulatorType>
        void accumulateSignal(InputIterator first, InputIterator last, OutputIterator result, AccumulatorType normalized_limit)
        {
            InputIterator it = first;
            AccumulatorType sum = *it;
            while(it != last)
            {
                it++;
                sum += *it;
            }
            it = first;
            while(it != last)
            {
                if(it == first)
                {
                    *result = (*it / sum) * normalized_limit;
                }
                else
                {
                    *result = *(result - 1) + (*it / sum) * normalized_limit;
                }
                result++;
                it++;
            }
        }
        
    /**
     * Inverts the given signal.
     */
    template<class InputIterator, class OutputIterator>
        void invertSignal(InputIterator first, InputIterator last, OutputIterator result)
        {
            while(first != last)
            {
                *result = *first * -1.0;
                result++;
                first++;
            }
        }
        
    /**
     * Flips the given signal. The last element will be the first and so on.
     * @throws runtime_error
     */
    template<class InputIterator, class OutputIterator>
        void flipSignal(InputIterator first, InputIterator last, OutputIterator result)
        {
            if(first == result)
                throw std::runtime_error("flipSignal won't work with same iterator for input and output!");
                
            InputIterator it = last;
            it--;
            while(it != first)
            {
                *result = *it;
                result++;
                it--;
            }
            *result = *first;
        }
        
        
    /**
     * This weights the negativ level difference, positive slope will be set to zero.
     * @param window_size this span is used to get the slope
     * @throws runtime_error
     */
    template<class InputIterator, class OutputIterator>
        void enforceNegativeDifference(InputIterator first, InputIterator last, OutputIterator result, unsigned int window_size)
        {
            if(first == result)
                throw std::runtime_error("derivativeSignal won't work with same iterator for input and output!");
            if(last - first <(int) window_size)
                throw std::runtime_error("The input signal should not have less elements than the window_size.");
                
            unsigned int half_window_size = window_size * 0.5;
            
            InputIterator window_begin = first;
            InputIterator window_end = first + half_window_size;
            while(first != last)
            {
                *result = (*window_end - *window_begin) < 0.0 ? (*window_end - *window_begin) * -1.0 : 0;
                if(window_begin + half_window_size > first)
                {
                    window_end++;
                }
                else if(window_end + 1 >= last)
                {
                    window_begin++;
                }
                else
                {
                    window_begin++;
                    window_end++;
                }
                result++;
                first++;
            }
        }
        
    /**
     * This weights the positive level difference, negative slope will be set to zero.
     * @param window_size this span is used to get the slope
     * @throws runtime_error
     */
    template<class InputIterator, class OutputIterator>
        void enforcePositiveDifference(InputIterator first, InputIterator last, OutputIterator result, unsigned int window_size)
        {
            if(first == result)
                throw std::runtime_error("derivativeSignal won't work with same iterator for input and output!");
            if(last - first < (int)window_size)
                throw std::runtime_error("The input signal should not have less elements than the window_size.");
                
            unsigned int half_window_size = window_size * 0.5;
            
            InputIterator window_begin = first;
            InputIterator window_end = first + half_window_size;
            while(first != last)
            {
                *result = ((*window_end - *window_begin) > 0.0) ? (*window_end - *window_begin) : 0;
                if(window_begin + half_window_size > first)
                {
                    window_end++;
                }
                else if(window_end + 1 >= last)
                {
                    window_begin++;
                }
                else
                {
                    window_begin++;
                    window_end++;
                }
                result++;
                first++;
            }
        }
    
    /**
     * This subtracts signal2 from signal1.
     * @param upper_limit higher values will reduced to this
     * @param lower_limit smaller values will increased to this.
     */
    template<class InputIterator, class OutputIterator, class AccumulatorType>
        void subtractSignal(InputIterator signal1_first, InputIterator signal1_last, InputIterator signal2_first, InputIterator signal2_last, 
                            OutputIterator result, AccumulatorType upper_limit, AccumulatorType lower_limit)
        {
            while(signal1_first != signal1_last && signal2_first != signal2_last)
            {
                *result = *signal1_first - *signal2_first;
                if(*result > upper_limit)
                    *result = upper_limit;
                else if(*result < lower_limit)
                    *result = lower_limit;
                result++;
                signal1_first++;
                signal2_first++;
            }
        }
        
    /**
     * This adds signal2 to signal1.
     * @param upper_limit higher values will reduced to this
     * @param lower_limit smaller values will increased to this.
     */
    template<class InputIterator, class OutputIterator, class AccumulatorType>
        void addSignal(InputIterator signal1_first, InputIterator signal1_last, InputIterator signal2_first, InputIterator signal2_last, 
                            OutputIterator result, AccumulatorType upper_limit, AccumulatorType lower_limit)
        {
            while(signal1_first != signal1_last && signal2_first != signal2_last)
            {
                *result = *signal1_first + *signal2_first;
                if(*result > upper_limit)
                    *result = upper_limit;
                else if(*result < lower_limit)
                    *result = lower_limit;
                result++;
                signal1_first++;
                signal2_first++;
            }
        }
        
    /**
     * Returns a signal which is the minimum of both input signals.
     */
    template<class InputIterator, class OutputIterator>
        void minimizeSignals(InputIterator signal1_first, InputIterator signal1_last, InputIterator signal2_first, InputIterator signal2_last, 
                            OutputIterator result)
        {
            while(signal1_first != signal1_last && signal2_first != signal2_last)
            {
                if(std::abs(*signal1_first) < std::abs(*signal2_first))
                    *result = *signal1_first;
                else
                    *result = *signal2_first;
                result++;
                signal1_first++;
                signal2_first++;
            }
        }
        
    /**
     * Returns a signal which is the maximum of both input signals.
     */
    template<class InputIterator, class OutputIterator>
        void maximizeSignals(InputIterator signal1_first, InputIterator signal1_last, InputIterator signal2_first, InputIterator signal2_last, 
                            OutputIterator result)
        {
            while(signal1_first != signal1_last && signal2_first != signal2_last)
            {
                if(std::abs(*signal1_first) > std::abs(*signal2_first))
                    *result = *signal1_first;
                else
                    *result = *signal2_first;
                result++;
                signal1_first++;
                signal2_first++;
            }
        }
    
    /**
     * This squares every element of the signal by a given exponent.
     */
    template<class InputIterator, class OutputIterator>
        void squareSignal(InputIterator first, InputIterator last, OutputIterator result, double exponent)
        {
            while(first != last)
            {
                *result = pow(*first, exponent);
                result++;
                first++;
            }
        }
        
    /**
     * Applies a given function pointer to each element of the signal.
     */
    template<class InputIterator, class OutputIterator, class AccumulatorType>
        void applyFunction(InputIterator first, InputIterator last, OutputIterator result, AccumulatorType (*function)(AccumulatorType))
        {
            while(first != last)
            {
                *result = function(*first);
                result++;
                first++;
            }
        }
    
    /**
     * Subtracts a signal, given by a function pointer, from the signal.
     * @param upper_limit higher values will reduced to this
     * @param lower_limit smaller values will increased to this.
     */
    template<class InputIterator, class OutputIterator, class AccumulatorType>
        void subtractFunctionFromSignal(InputIterator first, InputIterator last, OutputIterator result, AccumulatorType (*function)(AccumulatorType),
                                        AccumulatorType upper_limit, AccumulatorType lower_limit)
        {
            int i = 0;
            while(first != last)
            {
                *result = *first - function((AccumulatorType)i);
                if(*result > upper_limit)
                    *result = upper_limit;
                else if(*result < lower_limit)
                    *result = lower_limit;
                i++;
                first++;
                result++;
            }
        }
       
    /**
     * Adds a signal, given by a function pointer, from the signal.
     * @param upper_limit higher values will reduced to this
     * @param lower_limit smaller values will increased to this.
     */
    template<class InputIterator, class OutputIterator, class AccumulatorType>
        void addFunctionToSignal(InputIterator first, InputIterator last, OutputIterator result, AccumulatorType (*function)(AccumulatorType),
                                        AccumulatorType upper_limit, AccumulatorType lower_limit)
        {
            int i = 0;
            while(first != last)
            {
                *result = *first + function((AccumulatorType)i);
                if(*result > upper_limit)
                    *result = upper_limit;
                else if(*result < lower_limit)
                    *result = lower_limit;
                i++;
                first++;
                result++;
            }
        }
};

#endif
