#ifndef FIELD_COMPILER_COMPARATORS_H_
#define FIELD_COMPILER_COMPARATORS_H_

#include "math3d/math3d.h"

class DistanceComparator {
public:
    DistanceComparator(const math3d::matrix44 & _pivot) {
        this->pivot = math3d::vector4(_pivot[3][0], _pivot[3][1], _pivot[3][2]);
    }

    DistanceComparator() {}

    bool operator()(const math3d::matrix44 & _first, const math3d::matrix44 & _second) {
        math3d::vector4 first(_first[3][0], _first[3][1], _first[3][2]);
        math3d::vector4 second(_second[3][0], _second[3][1], _second[3][2]);
        
        return (first - pivot).size() < (second - pivot).size();
    }
private:
    math3d::vector4 pivot;
};

class AlphaComparator {
public:
    bool operator()(const math3d::matrix44 & first, const math3d::matrix44 & second) {
        float first_alpha = first[3][3];
        float second_alpha = second[3][3];
        return first_alpha > second_alpha;
    }
};

#endif