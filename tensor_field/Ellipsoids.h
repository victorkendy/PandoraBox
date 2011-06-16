#ifndef TENSOR_FIELD_ELLIPSOIDS_H_
#define TENSOR_FIELD_ELLIPSOIDS_H_

#include "pbge/pbge.h"
#include "math3d/math3d.h"

class Ellipsoids {
public:
    Ellipsoids(pbge::GraphicAPI * gfx);

    pbge::ModelInstance * createEllipsoid(float x_semi_axis, float y_semi_axis, float z_semi_axis);

    Ellipsoids * createEllipsoids(unsigned n);

    Ellipsoids * addTransform(const math3d::matrix44 & m);

    pbge::ModelCollection * done(pbge::GraphicAPI * gfx);
private:
    pbge::VBOModel * sphere;
    pbge::GPUProgram * shader;

    math3d::matrix44 * matrices;
    unsigned numberOfEllipsoids;
    int current;
};

#endif