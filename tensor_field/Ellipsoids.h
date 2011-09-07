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

    pbge::GPUProgram * render_pass_program;

    pbge::GPUProgram * depth_pass_program;

    pbge::GPUProgram * get_render_pass_program();

    pbge::GPUProgram * get_depth_pass_program();
};

#endif