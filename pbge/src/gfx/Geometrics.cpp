#include <cmath>

#include "pbge/core/definitions.h"
#include "pbge/gfx/Model.h"
#include "pbge/gfx/VBO.h"
#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/Geometrics.h"


using namespace pbge;

VBOModel * Geometrics::createCircle(float radius, unsigned int slices, GraphicAPI * gfx) {
    VertexBufferBuilder builder(slices);
    VertexAttribBuilder vertex = builder.addAttrib(4, VertexAttrib::VERTEX);
    //VertexAttribBuilder color = builder.addAttrib(3, VertexAttrib::COLOR);
    float param_step = 2 * PBGE_pi / slices;
    for(float t = 0; t < 2 * PBGE_pi; t+=param_step) {
        builder.on(vertex).pushValue(radius * cos(t), radius * sin(t), 0.0f, 1.0f);
        //builder.on(color).pushValue(1,1,1);
    }
    VertexBuffer * vbo = builder.done(Buffer::STATIC_DRAW, gfx);
    return new VBOModel(vbo, GL_LINE_LOOP);
}