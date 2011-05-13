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

void GLDrawController::bindVBO(VertexBuffer * buffer) {
    buffer->bind(ogl);
}

void GLDrawController::unbindVBO(VertexBuffer * buffer) {
    buffer->unbind(ogl);
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