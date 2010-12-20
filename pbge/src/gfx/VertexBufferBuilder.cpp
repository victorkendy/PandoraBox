
#include "pbge/core/Manager.h"
#include "pbge/gfx/VBO.h"
#include "pbge/gfx/OpenGl.h"
#include "pbge/gfx/Buffer.h"

using namespace pbge;

size_t VertexBufferBuilder::calculateSize() {
    return 10000;
}

VertexBuffer * VertexBufferBuilder::done() {
    OpenGL * ogl = Manager::getInstance()->getOpenGL();
    // Deixar uso como parametro?
    Buffer * buffer = ogl->createBuffer(calculateSize(), GL_STATIC_DRAW, GL_ARRAY_BUFFER);
    float * data = static_cast<float*>(buffer->map());
    // fazer a mágica ...
    return NULL;
}