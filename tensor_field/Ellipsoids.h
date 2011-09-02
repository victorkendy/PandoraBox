#ifndef TENSOR_FIELD_ELLIPSOIDS_H_
#define TENSOR_FIELD_ELLIPSOIDS_H_

#include "pbge/pbge.h"
#include "math3d/math3d.h"

class Ellipsoids {
public:
    Ellipsoids(pbge::GraphicAPI * gfx);

    pbge::ModelCollection * createEllipsoids(unsigned number_of_ellipsoids, math3d::matrix44 * transforms);
private:
    pbge::VBOModel * sphere;
    
    pbge::GraphicAPI * gfx;
};

#endif