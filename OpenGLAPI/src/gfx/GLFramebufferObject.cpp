#include <algorithm>
#include <iterator>
#include <vector>
#include <GL/glew.h>

#include "pbge/gfx/Texture.h"
#include "OpenGLAPI/gfx/GLTextures.h"
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
    // Find a free binding point for the new texture
    std::vector<Texture2D*>::iterator i = std::find(renderables.begin(), renderables.end(), (Texture2D*)NULL);
    *i = texture;
    GLTexture2D * tex = dynamic_cast<GLTexture2D*>(texture);
    // bind the texture to GL_COLOR_ATTACHMENTi_EXT where i = position of the texture in the vector
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + (i - renderables.begin()), GL_TEXTURE_2D, tex->getId(), 0);
}

void GLFramebufferObject::dettachRenderable(pbge::Texture2D *texture) {
    std::vector<Texture2D*>::iterator i = std::find(renderables.begin(), renderables.end(), texture);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + (i - renderables.begin()), GL_TEXTURE_2D, 0, 0);
    // Release the binding point
    *i = NULL;
}

void GLFramebufferObject::attachDepthRenderable(Texture2D * texture) {
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, 0, 0);
	if(texture != NULL) {
		GLTexture2D * depthTexture = dynamic_cast<GLTexture2D*>(texture);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, depthTexture->getId(), 0);
	}
}

void GLFramebufferObject::useRenderables(const std::vector<Texture2D *> & textures) {
    std::vector<Texture2D*>::const_iterator it, begin = textures.begin();
    int i = 0;
    for(it = textures.begin(); it != textures.end(); it++) {
        std::vector<Texture2D*>::const_iterator position = std::find(renderables.begin(), renderables.end(), *it);
        bindingPoints[i++] = GL_COLOR_ATTACHMENT0_EXT + (position - renderables.begin());
    }
    glDrawBuffers(i, bindingPoints.get());
    glViewport(0,0, getWidth(), getHeight());
}
