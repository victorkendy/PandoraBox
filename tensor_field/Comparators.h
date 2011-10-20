#ifndef TENSOR_FIELD_COMPARATORS_H_
#define TENSOR_FIELD_COMPARATORS_H_

#include "math3d/math3d.h"

class LessThanOrEqualTo {
public:
    LessThanOrEqualTo(float comp) : pivot(comp) {} 

    bool operator()(const math3d::matrix44 & value) {
        return value[3][3] <= pivot;
    }
private:
    float pivot;
};

#endif