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
    model->beforeRender(ogl);
    model->render(ogl);
    model->afterRender(ogl);
}

// Instanced Rendering optimization if possible
void GLDrawController::drawVBOModel(VBOModel *model, int times) {
    if(GLEW_ARB_draw_instanced) {
        draw((Model*)model, times);
    } else {
        draw((Model*)model, times);
    }
}

void GLDrawController::callRender(Model * model) {
    model->render(ogl);
}