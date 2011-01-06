//TODO Terminar de implementar!!!!!

#include <GL/glew.h>
#include <IL/ilut.h>
#include <cstdio>
#include <cstring>

#include "pbge/gfx/Texture.h"

namespace pbge {

    void Texture2D::loadFromBuffer(pbge::Buffer *buffer, GLint width, GLint height, GLenum internalFormat, GLenum bufferDataType, OpenGL * ogl) {
        buffer->bindOn(GL_PIXEL_UNPACK_BUFFER, ogl);
       
        buffer->unbind(ogl);
    }
    
}