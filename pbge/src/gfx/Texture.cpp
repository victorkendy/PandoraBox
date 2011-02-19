//TODO Terminar de implementar!!!!!

#include <cstdio>
#include <cstring>

#include "pbge/gfx/OpenGL.h"
#include "pbge/gfx/Texture.h"

namespace pbge {

    void Texture::bindTexture(OpenGL * ogl) {
        if(this->glID == 0) {
            ogl->genTextures(1, &glID);
            initializeTexture(ogl);
        }
        ogl->bindTexture(target, glID);
    }

    void Texture2D::initializeTexture(OpenGL * ogl) {
        ogl->bindTexture(getTarget(), getGLID());
        // update the gl object tex parameters
        ogl->texParameter(getTarget(), GL_TEXTURE_MIN_FILTER, (GLint)minFilter);
        ogl->texParameter(getTarget(), GL_TEXTURE_MAG_FILTER, (GLint)magFilter);
        // allocate the gl texture data
        ogl->texImage2D(getTarget(), 0, internalFormat, width, height, 0, dataFormat, dataType, data);
    }

}