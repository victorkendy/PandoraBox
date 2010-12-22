#include <iostream>

#include "pbge/core/Manager.h"
#include "pbge/gfx/VBO.h"
#include "pbge/gfx/OpenGl.h"
#include "pbge/gfx/Buffer.h"

using namespace pbge;

bool VertexAttribBuilder::isValid() {
    unsigned short maxIndex = values.size() / 4 - 1;
    std::vector<unsigned short>::iterator it;
    for(it = indexes.begin(); it != indexes.end(); it++)
        if(maxIndex < *it) return false;
    return true;
}

void VertexAttribBuilder::getNextElement(float * elems) {
    int baseIndex = indexes[currentElement++] * 4;
    for(int i = 0; i < 4; i++) {
        elems[i] = values[baseIndex + i];
    }
}

size_t VertexBufferBuilder::calculateSize() {
    unsigned size = 0;
    std::vector<VertexAttribBuilder>::iterator it;
    for(it = attribs.begin(); it != attribs.end(); it++)
        size += it->numberOfCoordinates();
    return size * nVertices;
}

VertexBuffer * VertexBufferBuilder::done(GLenum usage) {
    OpenGL * ogl = Manager::getInstance()->getOpenGL();
    // Deixar uso como parametro?
    Buffer * buffer = ogl->createBuffer(calculateSize(), usage, GL_ARRAY_BUFFER);
    float * data = static_cast<float*>(buffer->map());
    std::vector<VertexAttribBuilder>::iterator it;
    int dataIndex = 0;
    float buf[4];
    for(unsigned int i = 0; i < nVertices; i++) {
        for(it = attribs.begin(); it != attribs.end(); it++) {
            it->getNextElement(buf);
            for(int j = 0; j < it->numberOfCoordinates(); j++)
                data[dataIndex++] = buf[j];
        }
    }
    return NULL;
}