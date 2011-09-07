#include <vector>
#include <algorithm>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>

#include "pbge/core/Manager.h"
#include "pbge/exceptions/exceptions.h"
#include "pbge/gfx/VBO.h"
#include "pbge/gfx/GraphicObjectsFactory.h"
#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/Buffer.h"

using namespace boost::lambda;
using namespace pbge;

namespace imp_detail {
    class VertexAttribCreator {
    public:
        VertexAttribCreator(VertexBuffer* _vbo,int _stride):vbo(_vbo),offset(0),stride(_stride){}
        inline void operator()(VertexAttribBuilder & attrib) {
            vbo->addAttrib(attrib.createInstance(offset, stride));
            offset += attrib.numberOfCoordinates() * sizeof(float);
        }
    private:
        int offset;
        int stride;
        VertexBuffer * vbo;
    };

    class VertexBufferDataLogic {
    public:
        VertexBufferDataLogic(float * _data):data(_data),dataIndex(0){
        }

        // this method needs to be called with args by ref
        inline void operator() (VertexAttribBuilder & attrib) {
            float buf[4];
            attrib.getNextElement(buf);
            int n = attrib.numberOfCoordinates();
            std::copy(buf, buf + n, data + dataIndex);
            dataIndex += n;
        }
    private:
        int dataIndex;
        float * data;
    };
}


bool VertexAttribBuilder::isValid() {
    unsigned short maxIndex = values.size() / 4 - 1;
    return (std::find_if(indexes.begin(), indexes.end(), maxIndex < _1) == indexes.end());
}

void VertexAttribBuilder::getNextElement(float * elems) {
    std::vector<float>::iterator begin;
    int baseIndex;
    if(!this->indexesAssigned)
        baseIndex = currentElement++;
    else 
        baseIndex = indexes[currentElement++];
    begin = values.begin() + baseIndex * 4;
    std::copy(begin, begin + 4, elems);
}

bool VertexAttribBuilder::operator == (const VertexAttribBuilder & other) {
    if(type == VertexAttrib::CUSTOM_ATTRIB) {
        return (other.type == type && nCoord == other.nCoord && name == other.name);
    }
    return (this->type == other.type && this->index == other.index && this->nCoord == other.nCoord);
}

const VertexAttrib VertexAttribBuilder::createInstance(int offset, size_t stride) {
    return VertexAttrib(nCoord, offset, stride, type, name);
}


VertexBufferBuilder & VertexBufferBuilder::pushValue(const VertexAttribBuilder & attrib, const float &x, const float & y, const float & z, const float & w) {
    if(curAttrib == NULL || !(*curAttrib == attrib)) {
        std::vector<VertexAttribBuilder>::iterator it = std::find(attribs.begin(), attribs.end(), attrib);
        if(it == attribs.end()) {
            throw BuilderException("Attribute not defined");
        }
        curAttrib = &(*it);
    }
    curAttrib->pushValue(x,y,z,w);
    return *this;
}

VertexBufferBuilder & VertexBufferBuilder::setAttribIndex(const VertexAttribBuilder & attrib, const std::vector<unsigned short> & indexes) {
    std::vector<VertexAttribBuilder>::iterator it = std::find(attribs.begin(), attribs.end(), attrib);
    if(it == attribs.end()) {
        throw BuilderException("Attribute not defined");
    }
    curAttrib = &(*it);
    curAttrib->setIndexes(indexes);
    return *this;
}

VertexBufferBuilder & VertexBufferBuilder::setAttribIndex(const std::vector<unsigned short> & indexes) {
    if(curAttrib != NULL)
        curAttrib->setIndexes(indexes);
    return *this;
}


size_t VertexBufferBuilder::calculateSize() {
    unsigned size = 0;
    std::for_each(attribs.begin(), attribs.end(), 
        var(size) += bind(&VertexAttribBuilder::numberOfCoordinates, _1));
    return size * nVertices;
}

void VertexBufferBuilder::createAttribs(VertexBuffer * vbo, size_t stride) {
    std::for_each(attribs.begin(), attribs.end(), imp_detail::VertexAttribCreator(vbo, stride));
}

VertexBuffer * VertexBufferBuilder::done(Buffer::UsageHint usage, GraphicAPI * ogl) {
    size_t size = calculateSize();
    size_t stride = size / nVertices;
    Buffer * buffer = ogl->getFactory()->createBuffer(size*sizeof(float), usage);
    float * data = (float*) buffer->map(Buffer::WRITE_ONLY);
    imp_detail::VertexBufferDataLogic logic = imp_detail::VertexBufferDataLogic(data);
    for(unsigned int i = 0; i < nVertices; i++) {
        // accumulates the result of the functor object
        logic = std::for_each(attribs.begin(), attribs.end(), logic);
    }
    buffer->unmap();
    VertexBuffer * vbo = new VertexBuffer(buffer, nVertices);
    createAttribs(vbo, stride * sizeof(float));
    return vbo;
}

