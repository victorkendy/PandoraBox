#include <GL/glew.h>

#include "pbge/gfx/Texture.h"
#include "OpenGLAPI/gfx/GLFramebufferObject.h"

using namespace pbge;

bool GLFramebufferObject::isInitialized() {
    return GLID != 0;
}

void GLFramebufferObject::initialize() {
    glGenFramebuffersEXT(1, &GLID);    
}

void GLFramebufferObject::bindFramebuffer() {
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, GLID);
}

void GLFramebufferObject::unbindFramebuffer() {
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void GLFramebufferObject::attachRenderable(pbge::Texture2D *texture) {
    
}

void GLFramebufferObject::dettachRenderable(pbge::Texture2D *texture) {

}