#include <cmath>

#include "pbge/core/definitions.h"
#include "pbge/exceptions/exceptions.h"
#include "pbge/gfx/Model.h"
#include "pbge/gfx/VBO.h"
#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/Geometrics.h"


using namespace pbge;

VBOModel * Geometrics::createCircunference(float radius, unsigned int slices, GraphicAPI * gfx) {
    if(radius <= 0) {
        throw IllegalArgumentException("radius must have a positive value");
    }
    if(slices < 3) {
        throw IllegalArgumentException("there must be at least 3 slices");
    }
    VertexBufferBuilder builder(slices);
    VertexAttribBuilder vertex = builder.addAttrib(4, VertexAttrib::VERTEX);
    float param_step = 2 * pbge::pi / slices;
    for(float t = 0; t < 2 * pbge::pi; t+=param_step) {
        builder.on(vertex).pushValue(radius * cos(t), radius * sin(t), 0.0f, 1.0f);
    }
    VertexBuffer * vbo = builder.done(Buffer::STATIC_DRAW, gfx);
    return new VBOModel(vbo, GL_LINE_LOOP);
}

VBOModel * Geometrics::createSphere(float radius, unsigned slices, GraphicAPI * gfx) {
    if(radius <= 0) {
        throw IllegalArgumentException("radius must have a positive value");
    }
    if(slices < 3) {
        throw IllegalArgumentException("there must be at least 3 slices");
    }
    float param_step = 2 * pbge::pi / slices;
    VertexBufferBuilder builder(slices * slices);
    VertexAttribBuilder vertex = builder.addAttrib(4, VertexAttrib::VERTEX);
    builder.on(vertex);
    for(float phi = 0, next_phi = param_step; phi < 2 * pbge::pi; phi = next_phi, next_phi += param_step) {
        for(float theta = 0; theta < pbge::pi; theta += param_step) {
            builder.pushValue(radius * sin(theta) * cos(phi), radius * sin(theta) * sin(phi), radius * cos(theta), 1.0f);
            builder.pushValue(radius * sin(theta) * cos(next_phi), radius * sin(theta) * sin(next_phi), radius * cos(theta), 1.0f);
        }
    }
    return new VBOModel(builder.done(Buffer::STATIC_DRAW, gfx), GL_QUAD_STRIP);
}

VBOModel * Geometrics::createBezier(const math3d::vector4 &p0, const math3d::vector4 &p1, 
                                    const math3d::vector4 &p2, const math3d::vector4 &p3, 
                                    unsigned slices, GraphicAPI * gfx) {
    VertexBufferBuilder builder(slices);
    VertexAttribBuilder vertex = builder.addAttrib(4, VertexAttrib::VERTEX);
    float step = 1.0f / slices;
    builder.on(vertex);
    for(unsigned i = 0; i < slices; i++) {
        float t = i * step;
        float oneMinusT = 1.0f - t;
        float tl = oneMinusT;
        math3d::vector4 position = 3 * tl * p2 + t * p3;
        tl *= oneMinusT;
        position *= t;
        position += 3 * tl * p1;
        tl *= oneMinusT;
        position *= t;
        position += tl * p0;
        builder.pushValue(position[0], position[1], position[2], position[3]);
    }
    return new VBOModel(builder.done(Buffer::STATIC_DRAW, gfx), GL_LINE_STRIP);
}