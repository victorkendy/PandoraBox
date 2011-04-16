
#include "pbge/gfx/OpenGL/GLTextures.h"
#include "pbge/internal/OpenGLStates.h"
#include "pbge/gfx/Texture.h"
#include "pbge/gfx/OpenGL.h"
#include "pbge/gfx/Image.h"

using namespace pbge;

GLTexture2D::GLTexture2D(OpenGL * gl) {
    this->ogl = gl;
    this->data = NULL;
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
    glActiveTexture(unit->getIndex());
    if(this->GLID != 0) {
        glBindTexture(GL_TEXTURE_2D, this->GLID);
    } else {
        glGenTextures(1, &GLID);
        glBindTexture(GL_TEXTURE_2D, this->GLID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->magFilter);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, dataType, data);
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
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, dataType, data);
        }
}

void GLTexture2D::replaceInternalData(Texture::DataType type, Texture::Format dataFormat, void * image, unsigned size, 
                                      int w, int h, Texture::Format format) {
    this->internalFormat = format;
    this->width = w;
    this->height = h;
    if(this->data != NULL) {
        delete this->data;
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