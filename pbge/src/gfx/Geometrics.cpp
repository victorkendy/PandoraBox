#include <cmath>

#include "pbge/core/definitions.h"
#include "pbge/exceptions/exceptions.h"
#include "pbge/gfx/Model.h"
#include "pbge/gfx/VBO.h"
#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/Geometrics.h"


using namespace pbge;

VBOModel * Geometrics::createCircle(float radius, unsigned int slices, GraphicAPI * gfx) {
    if(radius <= 0) {
        throw IllegalArgumentException("radius must have a positive value");
    }
    if(slices < 3) {
        throw IllegalArgumentException("there must be at least 3 slices");
    }
    VertexBufferBuilder builder(slices);
    VertexAttribBuilder vertex = builder.addAttrib(4, VertexAttrib::VERTEX);
    float param_step = 2 * PBGE_pi / slices;
    for(float t = 0; t < 2 * PBGE_pi; t+=param_step) {
        builder.on(vertex).pushValue(radius * cos(t), radius * sin(t), 0.0f, 1.0f);
    }
    VertexBuffer * vbo = builder.done(Buffer::STATIC_DRAW, gfx);
    return new VBOModel(vbo, GL_LINE_LOOP);
}