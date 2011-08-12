#include <cmath>

#include "pbge/core/definitions.h"
#include "pbge/exceptions/exceptions.h"
#include "pbge/gfx/Model.h"
#include "pbge/gfx/VBO.h"
#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/Geometrics.h"


using namespace pbge;

template<class F>
VertexBuffer * meshgrid(int m, int n, F & f, GraphicAPI* gfx) {
    VertexBufferBuilder builder(4 * m * n);
    VertexAttribBuilder vertex = builder.addAttrib(4, VertexAttrib::VERTEX);
    builder.on(vertex);
    float value[4];
    float dx = 1.0f/m, dy = 1.0f/n;
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            f(i*dx, j*dy, value);
            builder.pushValue(value[0], value[1], value[2], value[3]);
            f((i+1)*dx, j*dy, value);
            builder.pushValue(value[0], value[1], value[2], value[3]);
            f((i+1)*dx, (j+1)*dy, value);
            builder.pushValue(value[0], value[1], value[2], value[3]);
            f(i*dx, (j+1)*dy, value);
            builder.pushValue(value[0], value[1], value[2], value[3]);
        }
    }
    return builder.done(Buffer::STATIC_DRAW, gfx);
}

class SphereEvaluator {
public:
    SphereEvaluator(float r):radius(r){}
    void operator () (float s, float t, float values[]) {
        float theta = s * pbge::pi * 2;
        float phi = t * pbge::pi;
        values[0] = radius * cos(theta) * sin(phi);
        values[1] = radius * sin(theta) * sin(phi);
        values[2] = radius * cos(phi);
        values[3] = 1.0f;
    }
private:
    float radius;
};

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
    VertexBuffer * vbo = meshgrid(slices, slices, SphereEvaluator(radius), gfx);
    return new VBOModel(vbo, GL_QUADS);
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

VBOModel * Geometrics::createSquare(const float & side, GraphicAPI * gfx) {
    VertexBufferBuilder builder(4);
    VertexAttribBuilder vertex = builder.addAttrib(4, VertexAttrib::VERTEX);
    builder.on(vertex);
    float side_2 = side / 2.0f;
    builder.pushValue(-side_2, side_2, 0.0f, 1.0f);
    builder.pushValue(side_2, side_2, 0.0f, 1.0f);
    builder.pushValue(side_2, -side_2, 0.0f, 1.0f);
    builder.pushValue(-side_2, -side_2, 0.0f, 1.0f);
    return new VBOModel(builder.done(Buffer::STATIC_DRAW, gfx), GL_QUADS);
}
