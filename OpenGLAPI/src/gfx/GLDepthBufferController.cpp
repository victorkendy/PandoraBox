#include <GL/glew.h>
#include "pbge/exceptions/exceptions.h"
#include "OpenGLAPI/gfx/GLDepthBufferController.h"

using namespace pbge;

GLDepthBufferController::GLDepthBufferController() :
    writeEnabled(true),
    testEnabled(false),
    depthFunc(pbge::DepthBufferController::DEPTH_LESS){}

void GLDepthBufferController::enableDepthTest() {
    if(!testEnabled) {
        glEnable(GL_DEPTH_TEST);
        testEnabled = true;
    }
}

void GLDepthBufferController::disableDepthTest() {
    if(testEnabled) {
        glDisable(GL_DEPTH_TEST);
        testEnabled = false;
    }
}

void GLDepthBufferController::enableDepthWrite() {
    if(!writeEnabled) {
        glDepthMask(GL_TRUE);
        writeEnabled = true;
    }
}

void GLDepthBufferController::disableDepthWrite() {
    if(writeEnabled) {
        glDepthMask(GL_FALSE);
        writeEnabled = false;
    }
}
void GLDepthBufferController::useDepthFunc(DepthFunc func) {
    if(depthFunc != func) {
        depthFunc = func;
        if(depthFunc == DepthBufferController::DEPTH_ALWAYS) {
            glDepthFunc(GL_ALWAYS);
        } else if(depthFunc == DepthBufferController::DEPTH_GREATER) {
            glDepthFunc(GL_GREATER);
        } else if(depthFunc == DepthBufferController::DEPTH_GREATER_EQUAL) {
            glDepthFunc(GL_GEQUAL);
        } else if(depthFunc == DepthBufferController::DEPTH_LESS) {
            glDepthFunc(GL_LESS);
        } else if(depthFunc == DepthBufferController::DEPTH_LESS_EQUAL) {
            glDepthFunc(GL_LEQUAL);
        } else if(depthFunc == DepthBufferController::DEPTH_NEVER) {
            glDepthFunc(GL_NEVER);
        } else {
            throw IllegalArgumentException("Not a depth function");
        }
    }
}
