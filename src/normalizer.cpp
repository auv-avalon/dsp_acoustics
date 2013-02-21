#include "normalizer.h"
#include <stdio.h>

namespace dsp {

    Normalizer::Normalizer(float normalizeValue):_normalizeValue(normalizeValue) {}
    
    Normalizer::~Normalizer() {}
    
    void Normalizer::normalize(std::vector<float> *data, float maximum, float minimum) {
        for(int i = 0; i < data->size(); i++) {
            bool isNeg = false;
            if(data->at(i) < 0) isNeg = true;
            
            data->at(i) = ((fabs(data->at(i)) - minimum) * _normalizeValue) / (maximum - minimum);
            if(isNeg) data->at(i) = data->at(i) * -1;
        }
    }
    
    void Normalizer::normalize(std::vector<float> *data) {
        float maximum = 0.0;
        float minimum = 100000.0;
        
        for(int i = 0; i < data->size(); i++) {
            if(data->at(i) > maximum) maximum = data->at(i);
            if(data->at(i) < minimum) minimum = data->at(i);
        }
        
        normalize(data, maximum, 0);
        
        maximum = 0;
        minimum = 100000;
        
        for(int i = 0; i < data->size(); i++) {
            if(data->at(i) > maximum) maximum = data->at(i);
            if(data->at(i) < minimum) minimum = data->at(i);
        }
        
    }
}
