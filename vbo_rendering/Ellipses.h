#ifndef VBO_RENDERING_ELLIPSES_H_
#define VBO_RENDERING_ELLIPSES_H_

#include "pbge/pbge.h"

class Ellipses {
public:
    Ellipses(pbge::GraphicAPI * gfx);

    pbge::ModelInstance * createEllipse(float x_semi_axis, float y_semi_axis);
private:
    pbge::VBOModel * circle;
    pbge::GPUProgram * shader;
};

#endif