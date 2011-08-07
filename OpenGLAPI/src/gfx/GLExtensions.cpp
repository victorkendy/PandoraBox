
#include <GL/glew.h>
#include "OpenGLAPI/gfx/GLGraphic.h"
#include "OpenGLAPI/gfx/GLExtensions.h"


using namespace pbge;

TextureFormats::TextureFormats(GLGraphic *ogl) {
    if(ogl->getMajorVersion() >= 3) {
        rgba32f = GL_RGBA32F;
        rgb32f = GL_RGB32F;
        rg32f = GL_RG32F;
        r32f = GL_R32F;
    } else if (GLEW_ARB_texture_float) {
        rgba32f = GL_RGBA32F_ARB;
        rgb32f = GL_RGB32F_ARB;
        rg32f = r32f = 0; // not supported
    } else {
        rgba32f = rgb32f = rg32f = r32f = 0; // texture float not supported
    }
}

TextureBufferInfo::TextureBufferInfo(GLGraphic * ogl) {
    if(ogl->getMajorVersion() >= 3) {
        binding = GL_TEXTURE_BUFFER;
        bufferBinding = GL_TEXTURE_BUFFER;
    } else if(GLEW_ARB_texture_buffer_object) {
        binding = GL_TEXTURE_BUFFER_ARB;
        bufferBinding = GL_TEXTURE_BUFFER_ARB;
    } else if(GLEW_EXT_texture_buffer_object) {
        binding = GL_TEXTURE_BUFFER_EXT;
        bufferBinding = GL_TEXTURE_BUFFER_EXT;
    } else {
        binding = 0;
        bufferBinding = 0;
    }
}

GLExtensions::GLExtensions(pbge::GLGraphic *ogl):bufferTextureInfo(ogl),textureFormats(ogl){}