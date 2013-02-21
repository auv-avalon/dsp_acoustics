#ifndef NORMALIZER_H
#define NORMALIZER_H

#include <vector>
#include <math.h>
#include <iostream>

namespace dsp {
    class Normalizer {
        public:
            Normalizer(float normalizeValue);
            ~Normalizer();
            
            void normalize(std::vector<float> *data, float maximum, float minimum);
            void normalize(std::vector<float> *data);
        protected:
            float _normalizeValue;
   };
}

#endif