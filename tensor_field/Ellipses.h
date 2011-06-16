#ifndef TENSOR_FIELD_ELLIPSES_H_
#define TENSOR_FIELD_ELLIPSES_H_

#include "pbge/pbge.h"
#include "math3d/math3d.h"

class Ellipses {
public:
    Ellipses(pbge::GraphicAPI * gfx);

    pbge::ModelInstance * createEllipse(float x_semi_axis, float y_semi_axis);

    Ellipses * createEllipses(unsigned n);

    Ellipses * addTransform(const math3d::matrix44 & m);

    pbge::ModelCollection * done(pbge::GraphicAPI * gfx);
private:
    pbge::VBOModel * circle;
    pbge::GPUProgram * shader;

    math3d::matrix44 * matrices;
    unsigned numberOfEllipses;
    int current;
};

#endif