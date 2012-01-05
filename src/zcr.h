/**
* Zero crossing rate...
**/

/**
* @param T type of data
* @param data pointer to the date
* @param size Size of the array
* @return return the total zeros crossings 
*/
template<typename T>
unsigned int zeroCrossingRate( T* data , unsigned int size)  {
    unsigned int ret =0;
    if(size <2){
        return ret;
    }
    
    T* end = data+size;
    T* start = data;
    T val = *data;
    for(;start != end;  ){
        ++start;
        if(val * (*start) < 0 ){
            ++ret;
        }
        val = (*start);
    }
    return ret;
}