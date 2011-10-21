#ifndef TENSORFIELD_BOUNDINGBOX_H
#define TENSORFIELD_BOUNDINGBOX_H

#include "math3d/math3d.h"
#include <boost/smart_ptr/scoped_array.hpp>

struct BoundingBox {
    float max_x, max_y, max_z;
    float min_x, min_y, min_z;
};

#endif