#ifndef TENSOR_FIELD_COMPARATORS_H_
#define TENSOR_FIELD_COMPARATORS_H_

#include "math3d/math3d.h"

class LessThanOrEqualTo {
public:
    LessThanOrEqualTo(float comp, int alpha_index) : pivot(comp), index(alpha_index) {} 

    bool operator()(const math3d::matrix44 & value) {
        return value[index][3] <= pivot;
    }
private:
    float pivot;
    int index;
};

#endif