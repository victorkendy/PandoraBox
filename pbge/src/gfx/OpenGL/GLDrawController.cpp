#include <iostream>
#include "GL/glew.h"

#include "pbge/gfx/Model.h"

#include "pbge/gfx/OpenGL/GLGraphic.h"
#include "pbge/gfx/OpenGL/GLDrawController.h"

using namespace pbge;

GLDrawController::GLDrawController(GLGraphic * _ogl) {
    this->ogl = _ogl;
}

void GLDrawController::draw(Model * model) {
    VBOModel * vboModel = dynamic_cast<VBOModel *>(model);
    model->beforeRender(ogl);
    if(vboModel == NULL) {
        callRender(model);
    } else {
        this->drawVBO(vboModel);
    }
    model->afterRender(ogl);
}

void GLDrawController::draw(Model * model, int times) {
    model->beforeRender(ogl);
    VBOModel * vboModel = dynamic_cast<VBOModel *>(model);
    if(vboModel == NULL) {
        for(int i = 0; i < times; i++) {
            callRender(model);
        }
    } else {
        drawVBO(vboModel, times);
    }
    model->afterRender(ogl);
}

void GLDrawController::drawVBO(VBOModel *model) {
    model->render(ogl);
}

// Instanced Rendering optimization if possible
void GLDrawController::drawVBO(VBOModel *model, int times) {
    if(GLEW_ARB_draw_instanced) {
        draw((Model*)model, times);
    } else {
        draw((Model*)model, times);
    }
}

void GLDrawController::callRender(Model * model) {
    model->render(ogl);
}