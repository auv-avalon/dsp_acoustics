%module rdsp_acoustics
%{
/* Includes the header in the wrapper code */
#include "DspAcoustics.hpp"
%}

%include "std_vector.i"
namespace std {
   %template(IntVector) vector<int>;
   %template(DoubleVector) vector<double>;
   %template(CharVector) vector<char>;
   %template(FloatVector) vector<float>;
}

%include "typemaps.i"
void extractTarget(std::vector<char>, int, int &OUTPUT, int &OUTPUT, std::vector<int> &OUTPUT);

