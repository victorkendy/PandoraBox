#ifndef TENSORFIELD_BOUNDINGBOX_H
#define TENSORFIELD_BOUNDINGBOX_H

#include "math3d/math3d.h"

struct BoundingBox {
    float max_x, max_y, max_z;
    float min_x, min_y, min_z;
};

struct FullBoundingBox : public BoundingBox {
    FullBoundingBox(BoundingBox box) : BoundingBox(box) {
        positions[0] = math3d::vector4(box.max_x, box.max_y, box.max_z);
        positions[1] = math3d::vector4(box.max_x, box.max_y, box.min_z);
        positions[2] = math3d::vector4(box.max_x, box.min_y, box.max_z);
        positions[3] = math3d::vector4(box.min_x, box.max_y, box.max_z);
        positions[4] = math3d::vector4(box.max_x, box.min_y, box.min_z);
        positions[5] = math3d::vector4(box.min_x, box.min_y, box.max_z);
        positions[6] = math3d::vector4(box.min_x, box.max_y, box.min_z);
        positions[7] = math3d::vector4(box.min_x, box.min_y, box.min_z);
    }
    math3d::vector4 positions[8];
};

#endif