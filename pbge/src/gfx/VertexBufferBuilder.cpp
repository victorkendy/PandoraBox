#include <vector>
#include <algorithm>

#include "pbge/core/Manager.h"
#include "pbge/exceptions/exceptions.h"
#include "pbge/gfx/VBO.h"
#include "pbge/gfx/GraphicObjectsFactory.h"
#include "pbge/gfx/GraphicAPI.h"
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

const VertexAttrib VertexAttribBuilder::createInstance(int offset, GLsizei stride) {
    return VertexAttrib(nCoord, offset, stride, type);
}


VertexBufferBuilder & VertexBufferBuilder::pushValue(const VertexAttribBuilder & attrib, const float &x, const float & y, const float & z, const float & w) {
    if(curAttrib == NULL || !(*curAttrib == attrib)) {
        std::vector<VertexAttribBuilder>::iterator it = std::find(attribs.begin(), attribs.end(), attrib);
        if(it == attribs.end())
            throw BuilderException("Attribute not defined");
        else {
            curAttrib = &(*it);
            curAttrib->pushValue(x,y,z,w);
        }
    } else {
        curAttrib->pushValue(x,y,z,w);
    }
    return *this;
}

VertexBufferBuilder & VertexBufferBuilder::setAttribIndex(const VertexAttribBuilder & attrib, const std::vector<unsigned short> & indexes) {
    std::vector<VertexAttribBuilder>::iterator it = std::find(attribs.begin(), attribs.end(), attrib);
    if(it == attribs.end()) {
        throw BuilderException("Attribute not defined");
    } else {
        curAttrib = &(*it);
        curAttrib->setIndexes(indexes);
    }
    return *this;
}

VertexBufferBuilder & VertexBufferBuilder::setAttribIndex(const std::vector<unsigned short> & indexes) {
    if(curAttrib != NULL)
        curAttrib->setIndexes(indexes);
    return *this;
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

VertexBuffer * VertexBufferBuilder::done(Buffer::UsageHint usage, GraphicAPI * ogl) {
    validateAttribs();
    GLsizei size = calculateSize();
    GLsizei stride = size / nVertices;
    Buffer * buffer = ogl->getFactory()->createBuffer(size*sizeof(float), usage);
    float * data = static_cast<float*>(buffer->map(Buffer::WRITE_ONLY));
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
    buffer->unmap();
    data = NULL;
    VertexBuffer * vbo = new VertexBuffer(buffer, nVertices);
    createAttribs(vbo, stride * sizeof(float));
    return vbo;
}

