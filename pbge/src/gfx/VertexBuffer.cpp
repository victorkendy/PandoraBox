#include <vector>
#include <algorithm>

#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/Buffer.h"
#include "pbge/gfx/VBO.h"

namespace pbge {
    #define ATTRIB_POINTER_OFFSET(offset) ((GLbyte *)NULL + (offset))

    void VertexPositionAttrib::bindAttrib(GraphicAPI * ogl) {
        ogl->enableClientState(GL_VERTEX_ARRAY);
        ogl->vertexPointer(this->getNCoord(), GL_FLOAT, this->getStride(), ATTRIB_POINTER_OFFSET(this->getOffset()));
    }

    void VertexNormalAttrib::bindAttrib(pbge::GraphicAPI *ogl) {
        ogl->enableClientState(GL_NORMAL_ARRAY);
        ogl->normalPointer(GL_FLOAT, getStride(), ATTRIB_POINTER_OFFSET(getOffset()));
    }

    void VertexTexcoordAttrib::bindAttrib(GraphicAPI * ogl) {
        ogl->enableClientState(GL_TEXTURE_COORD_ARRAY);
        ogl->texCoordPointer(getNCoord(), GL_FLOAT, getStride(), ATTRIB_POINTER_OFFSET(getOffset()));
    }

    void VertexColorAttrib::bindAttrib(pbge::GraphicAPI *ogl) {
        ogl->enableClientState(GL_COLOR_ARRAY);
        ogl->colorPointer(getNCoord(),GL_FLOAT, getStride(), ATTRIB_POINTER_OFFSET(getOffset()));
    }

    void VertexSecondaryColorAttrib::bindAttrib(pbge::GraphicAPI *ogl) {
        ogl->enableClientState(GL_SECONDARY_COLOR_ARRAY);
        ogl->secondaryColorPointer(3,GL_FLOAT, getStride(), ATTRIB_POINTER_OFFSET(getOffset()));
    }

    #undef ATTRIB_POINTER_OFFSET

    void VertexBuffer::bindAllAttribs(GraphicAPI * ogl) {
        std::vector<VertexAttrib*>::iterator it;
        for(it = attribs.begin(); it != attribs.end(); it++) {
            (*it)->bindAttrib(ogl);
        }
    }

    void VertexBuffer::bindOnly(VertexAttrib::Type type, GraphicAPI * ogl) {
        buffer->bindOn(Buffer::VertexBuffer);
        std::vector<VertexAttrib*>::iterator it;
        for(it = attribs.begin(); it != attribs.end(); it++) {
            if((*it)->getType() == type)
                (*it)->bindAttrib(ogl);
        }
    }

    void VertexBuffer::bind(GraphicAPI * ogl) {
        buffer->bindOn(Buffer::VertexBuffer);
        bindAllAttribs(ogl);
    }

    void VertexBuffer::unbind(GraphicAPI * ogl) {
        this->buffer->unbind();
    }

    VertexBuffer::~VertexBuffer() {
        delete buffer;
        std::vector<VertexAttrib*>::iterator it;
        for(it = attribs.begin(); it != attribs.end(); it++) {
            VertexAttrib * attrib = *it;
            delete attrib;
        }
    }
}