#include <GL/glew.h>

#include "OpenGLAPI/gfx/GLTextures.h"
#include "pbge/internal/OpenGLStates.h"
#include "pbge/gfx/Texture.h"
#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/Image.h"

namespace {
    GLint choosePixelFormatFor(GLenum format, GLenum type) {
        GLint rgba32f = format, rgb32f = format, rg32f = format, r32f = format;
        if(type != GL_FLOAT) return format;
        if(GLEW_VERSION_3_0 || GLEW_VERSION_3_1 || GLEW_VERSION_3_2 || GLEW_VERSION_3_3 || GLEW_VERSION_4_0) {
            rgba32f = GL_RGBA32F;
            rgb32f = GL_RGB32F;
            rg32f = GL_RG32F;
            r32f = GL_R32F;
        } else if(GLEW_ARB_texture_float) {
            rgba32f = GL_RGBA32F_ARB;
            rgb32f = GL_RGB32F_ARB;
        } else return format;
        if(format == GL_RGBA) return rgba32f;
        else if(format == GL_RGB) return rgb32f;
        else if(format == GL_RG) return rg32f;
        else if(format == GL_RED) return r32f;
        return format;
    }
}
using namespace pbge;

GLTexture1D::GLTexture1D(GraphicAPI * gl) {
    this->ogl = gl;
    this->data = NULL;
    this->minFilter = GL_LINEAR;
    this->magFilter = GL_LINEAR;
    this->GLID = 0;
}

void GLTexture1D::setMinFilter(Texture::Filter filter) {
    this->minFilter = filter;
}

void GLTexture1D::setMagFilter(Texture::Filter filter) {
    this->magFilter = filter;
}

void GLTexture1D::bindTextureOn(TextureUnit * unit) {
    glActiveTexture(GL_TEXTURE0 + unit->getIndex());
    if(this->GLID != 0) {
        glBindTexture(GL_TEXTURE_1D, this->GLID);
    } else {
        glGenTextures(1, &GLID);
        glBindTexture(GL_TEXTURE_1D, this->GLID);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, this->minFilter);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, this->magFilter);
        glTexImage1D(GL_TEXTURE_1D, 0, choosePixelFormatFor(internalFormat, dataType), width, 0, dataFormat, dataType, data);
    }
}

void GLTexture1D::setImageData(Texture::DataType type, Texture::Format _dataFormat, void * image, unsigned size, int _width, Texture::Format _internalFormat) {
    this->dataType = type;
    this->dataFormat = _dataFormat;
    this->data = malloc(size);
    memcpy(this->data, image, size);
    this->width = _width;
    this->internalFormat = _internalFormat;
}







GLTexture2D::GLTexture2D(GraphicAPI * gl) {
    this->ogl = gl;
    this->data = NULL;
    this->GLID = 0;
    this->minFilter = GL_LINEAR;
    this->magFilter = GL_LINEAR;
}

void GLTexture2D::setImage(Image * image, Texture::Format format) {
    if (this->GLID == 0) {
        this->replaceInternalData(image->getDataType(), image->getFormat(), image->getData(), image->getDataSize(), 
                                  image->getWidth(), image->getHeight(), format);
    } else {
        this->replaceGLObjectData(image->getDataType(), image->getFormat(), image->getData(), image->getDataSize(), 
                                  image->getWidth(), image->getHeight(), format);
    }
}

void GLTexture2D::setImageData(Texture::DataType type, Texture::Format dataFormat, void * image, unsigned size, 
                               int w, int h, Texture::Format format) {
    if(this->GLID == 0) {
        this->replaceInternalData(type, dataFormat, image, size, w, h, format);
    } else {
        this->replaceGLObjectData(type, dataFormat, image, size, w, h, format);
    }
}

void GLTexture2D::bindTextureOn(TextureUnit * unit) {
    glActiveTexture(GL_TEXTURE0 + unit->getIndex());
    if(this->GLID != 0) {
        glBindTexture(GL_TEXTURE_2D, this->GLID);
    } else {
        glGenTextures(1, &GLID);
        glBindTexture(GL_TEXTURE_2D, this->GLID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->magFilter);
        glTexImage2D(GL_TEXTURE_2D, 0, choosePixelFormatFor(internalFormat, dataType), width, height, 0, dataFormat, dataType, data);
    }
}

void GLTexture2D::setMinFilter(Texture::Filter filter) {
    this->minFilter = filter;
}

void GLTexture2D::setMagFilter(Texture::Filter filter) {
    this->magFilter = filter;
}

void GLTexture2D::replaceGLObjectData(Texture::DataType type, Texture::Format dataFormat, void * image, unsigned size, 
                                      int w, int h, Texture::Format format) {
    TextureUnit * unit = this->ogl->chooseTextureUnit(this);
        this->bindTextureOn(unit);
        if (this->isCompatibleWith(w, h, format)) {
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, dataFormat, dataType, data);
        } else {
            glTexImage2D(GL_TEXTURE_2D, 0, choosePixelFormatFor(internalFormat, dataType), width, height, 0, dataFormat, dataType, data);
        }
}

void GLTexture2D::replaceInternalData(Texture::DataType type, Texture::Format dataFormat, void * image, unsigned size, 
                                      int w, int h, Texture::Format format) {
    this->internalFormat = format;
    this->width = w;
    this->height = h;
    if(this->data != NULL) {
        free(this->data);
        this->data = NULL;
    }
    data = malloc(size);
    memcpy(data, image, size);
    this->dataType = type;
}

bool GLTexture2D::isCompatibleWith(unsigned w, unsigned h, Texture::Format format) {
    return ((this->width == w) && 
            (this->height == h) && 
            (this->internalFormat == format));
}