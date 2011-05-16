#include <iostream>
#include "GL/glew.h"

#include "pbge/gfx/VBO.h"
#include "pbge/gfx/Model.h"

#include "pbge/gfx/OpenGL/GLGraphic.h"
#include "pbge/gfx/OpenGL/GLDrawController.h"

using namespace pbge;

GLDrawController::GLDrawController(GLGraphic * _ogl) {
    this->ogl = _ogl;
}

void GLDrawController::draw(Model * model) {
    model->beforeRender(ogl);
    callRender(model);
    model->afterRender(ogl);
}

void GLDrawController::draw(Model * model, int times) {
    model->beforeRender(ogl);
    for(int i = 0; i < times; i++) {
        callRender(model);
    }
    model->afterRender(ogl);
}

void GLDrawController::drawVBOModel(VBOModel *model) {
    bindVBO(model->getVBO());
    glDrawArrays(model->getPrimitive(), 0, model->getVBO()->getNVertices());
    unbindVBO(model->getVBO());
}

#define ATTRIB_POINTER_OFFSET(offset) ((GLbyte *)NULL + (offset))

void GLDrawController::bindVBO(VertexBuffer * buffer) {
    glEnable(GL_VERTEX_ARRAY);
    buffer->getBuffer()->bindOn(Buffer::VertexBuffer);
    std::vector<VertexAttrib>::iterator attr;
    std::vector<VertexAttrib> & attribs = buffer->getAttribs();
    for(attr = attribs.begin(); attr != attribs.end(); attr++) {
        if(attr->getType() == VertexAttrib::VERTEX) {
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(attr->getNCoord(), GL_FLOAT, attr->getStride(), ATTRIB_POINTER_OFFSET(attr->getOffset()));
        }
        else if (attr->getType() == VertexAttrib::NORMAL) {
            glEnableClientState(GL_NORMAL_ARRAY);
            glNormalPointer(GL_FLOAT, attr->getStride(), ATTRIB_POINTER_OFFSET(attr->getOffset()));
        } else if(attr->getType() == VertexAttrib::COLOR) {
            glEnableClientState(GL_COLOR_ARRAY);
            glColorPointer(attr->getNCoord(), GL_FLOAT, attr->getStride(), ATTRIB_POINTER_OFFSET(attr->getOffset()));
        } else if(attr->getType() == VertexAttrib::SECONDARY_COLOR) {
            glEnableClientState(GL_SECONDARY_COLOR_ARRAY);
            glSecondaryColorPointer(attr->getNCoord(), GL_FLOAT, attr->getStride(), ATTRIB_POINTER_OFFSET(attr->getOffset()));
        } else if(attr->getType() == VertexAttrib::TEXCOORD) {
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(attr->getNCoord(), GL_FLOAT, attr->getStride(), ATTRIB_POINTER_OFFSET(attr->getOffset()));
        }
    }
}

#undef ATTRIB_POINTER_OFFSET

void GLDrawController::unbindVBO(VertexBuffer * buffer) {
    std::vector<VertexAttrib>::iterator attr;
    std::vector<VertexAttrib> & attribs = buffer->getAttribs();
    for(attr = attribs.begin(); attr != attribs.end(); attr++) {
        if(attr->getType() == VertexAttrib::VERTEX) {
            glDisableClientState(GL_VERTEX_ARRAY);
        }
        else if (attr->getType() == VertexAttrib::NORMAL) {
            glDisableClientState(GL_NORMAL_ARRAY);
        } else if(attr->getType() == VertexAttrib::COLOR) {
            glDisableClientState(GL_COLOR_ARRAY);
        } else if(attr->getType() == VertexAttrib::SECONDARY_COLOR) {
            glDisableClientState(GL_SECONDARY_COLOR_ARRAY);
        } else if(attr->getType() == VertexAttrib::TEXCOORD) {
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        }
    }
    buffer->getBuffer()->unbind();
    glDisable(GL_VERTEX_ARRAY);
}

// Instanced Rendering optimization if possible
void GLDrawController::drawVBOModel(VBOModel *model, int times) {
    bindVBO(model->getVBO());
    if(GLEW_ARB_draw_instanced) {
        glDrawArraysInstancedARB(model->getPrimitive(), 0, model->getVBO()->getNVertices(), times);
    } else {
        for(int i = 0; i < times; i++) {
            glDrawArrays(model->getPrimitive(), 0, model->getVBO()->getNVertices());
        }
    }
    unbindVBO(model->getVBO());
}

void GLDrawController::callRender(Model * model) {
    model->render(ogl);
}