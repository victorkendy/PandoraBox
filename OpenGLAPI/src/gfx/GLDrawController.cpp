#include <GL/glew.h>
#include <iostream>
#include "GL/glew.h"

#include "pbge/gfx/VBO.h"
#include "pbge/gfx/Model.h"
#include "pbge/gfx/ShaderUniform.h"
#include "pbge/gfx/UniformSet.h"
#include "pbge/gfx/StateSet.h"

#include "OpenGLAPI/gfx/GLGraphic.h"
#include "OpenGLAPI/gfx/GLDrawController.h"

using namespace pbge;

GLDrawController::GLDrawController(GLGraphic * _ogl):ogl(_ogl), coreSupported(false), arbSupported(false), extSupported(false) {
}

void GLDrawController::initialize() {
    if((ogl->getMajorVersion() == 3 && !GLEW_VERSION_3_0) || ogl->getMajorVersion() >= 4) {
        coreSupported = true;
    }
    if(GLEW_ARB_draw_instanced) {
        arbSupported = true;
    }
    if(GLEW_EXT_draw_instanced) {
        extSupported = true;
    }
}

void GLDrawController::draw(Model * model) {
    model->beforeRender(ogl);
    callRender(model);
    model->afterRender(ogl);
}

void GLDrawController::draw(Model * model, int times) {
    model->beforeRender(ogl);
    UniformSet uniforms;
    ogl->pushUniforms(&uniforms);
    // FIXME: replace by getInt
    UniformFloat * instanceID = uniforms.getFloat("instanceID");
    for(int i = 0; i < times; i++) {
        instanceID->setValue((float)i);
        callRender(model);
    }
    ogl->popUniforms();
    model->afterRender(ogl);
}

void GLDrawController::drawVBOModel(VBOModel *model) {
    bindVBO(model->getVBO());
    glDrawArrays(model->getPrimitive(), 0, model->getVBO()->getNVertices());
    unbindVBO(model->getVBO());
}

// Instanced Rendering optimization if possible
void GLDrawController::drawVBOModel(VBOModel *model, int times) {
    bindVBO(model->getVBO());
    if(coreSupported) {
        glDrawArraysInstanced(model->getPrimitive(), 0, model->getVBO()->getNVertices(), times);
    } else if(arbSupported) {
        glDrawArraysInstancedARB(model->getPrimitive(), 0, model->getVBO()->getNVertices(), times);
    } else if(extSupported) {
        // ......
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

#define ATTRIB_POINTER_OFFSET(offset) ((GLbyte *)NULL + (offset))

void GLDrawController::bindVBO(VertexBuffer * buffer) {
    ogl->getState()->useVertexBuffer(buffer);
    ogl->updateState();
}

#undef ATTRIB_POINTER_OFFSET

void GLDrawController::unbindVBO(VertexBuffer * buffer) {
    ogl->getState()->useVertexBuffer(NULL);
}
