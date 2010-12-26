#include <iostream>

#include "pbge/core/Manager.h"
#include "pbge/exceptions/exceptions.h"
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

bool VertexAttribBuilder::operator == (const VertexAttribBuilder & other) {
    if(type == VertexAttrib::CUSTOM_ATTRIB) {
        if(other.type == type)
            return (name == other.name && nCoord == other.nCoord);
        else
            return false;
    }
    return (this->type == other.type && this->index == other.index && this->nCoord == other.nCoord);
}

VertexAttrib * VertexAttribBuilder::createInstance(int offset, GLsizei stride) {
    switch(type) {
        case VertexAttrib::VERTEX: return new VertexPositionAttrib(nCoord, offset, stride);
        case VertexAttrib::NORMAL: return new VertexNormalAttrib(offset, stride);
        case VertexAttrib::TEXCOORD: return new VertexTexcoordAttrib(nCoord, offset, stride);
        case VertexAttrib::COLOR: return new VertexColorAttrib(nCoord, offset, stride);
        case VertexAttrib::SECONDARY_COLOR: return new VertexSecondaryColorAttrib(nCoord, offset, stride);
        case VertexAttrib::CUSTOM_ATTRIB: return new VertexColorAttrib(nCoord, offset, stride);
        default: return NULL;
    }
}

GLsizei VertexBufferBuilder::calculateSize() {
    unsigned size = 0;
    std::vector<VertexAttribBuilder>::iterator it;
    for(it = attribs.begin(); it != attribs.end(); it++)
        size += it->numberOfCoordinates();
    return size * nVertices;
}

void VertexBufferBuilder::validateAttribs() {
    std::vector<VertexAttribBuilder>::iterator it;
    for(it = attribs.begin(); it != attribs.end(); it++) {
        if(!it->areIndexesAssigned()) {
            throw BuilderValidationException("attrib has no index vector");
        }
    }
}

void VertexBufferBuilder::createAttribs(VertexBuffer * vbo, GLsizei stride) {
    std::vector<VertexAttribBuilder>::iterator it;
    int offset = 0;
    for(it = attribs.begin(); it != attribs.end(); it++) {
        vbo->addAttrib(it->createInstance(offset, stride));
        offset += it->numberOfCoordinates() * sizeof(float);
    }
}

VertexBuffer * VertexBufferBuilder::done(GLenum usage) {
    validateAttribs();
    OpenGL * ogl = Manager::getInstance()->getOpenGL();
    GLsizei size = calculateSize();
    GLsizei stride = size / nVertices;
    Buffer * buffer = ogl->createBuffer(size*sizeof(float), usage, GL_ARRAY_BUFFER);
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
    VertexBuffer * vbo = new VertexBuffer(buffer, nVertices);
    createAttribs(vbo, stride * sizeof(float));
    return vbo;
}

