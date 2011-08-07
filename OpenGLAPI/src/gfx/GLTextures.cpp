#include <GL/glew.h>

#include "OpenGLAPI/gfx/GLGraphic.h"
#include "OpenGLAPI/gfx/GLObjectsFactory.h"
#include "OpenGLAPI/gfx/GLTextures.h"
#include "pbge/internal/OpenGLStates.h"
#include "pbge/gfx/Texture.h"
#include "pbge/gfx/Image.h"

namespace {
    GLint choosePixelFormatFor(GLenum format, GLenum type, const pbge::TextureFormats & formats) {
        GLint rgba32f = format, rgb32f = format, rg32f = format, r32f = format;
        if(type != GL_FLOAT) return format;
        if(format == GL_RGBA) return formats.rgba32f;
        else if(format == GL_RGB) return formats.rgb32f;
        else if(format == GL_RG) return formats.rg32f;
        else if(format == GL_RED) return formats.r32f;
        return format;
    }
}
using namespace pbge;


GLTextureBuffer::GLTextureBuffer(GLGraphic * _ogl, size_t _size):size(_size) {
    // need types
    ogl = _ogl;
    buffer = dynamic_cast<GLBuffer*>(ogl->getFactory()->createBuffer(size, Buffer::DYNAMIC_DRAW));
    bindingPoint = ogl->getExtensions().getBufferTextureInfo().binding;
    GLID = 0;
    internalFormat = 0;
}
void GLTextureBuffer::setInternalFormat(Texture::DataType type, Texture::Format format) {
    internalFormat = choosePixelFormatFor(format, type, ogl->getExtensions().getTextureFormats());
}

void GLTextureBuffer::bindTextureOn(TextureUnit * unit) {
    glActiveTexture(GL_TEXTURE0 + unit->getIndex());
    if(GLID != 0) {
        // @todo: verify if buffer changed
        glBindTexture(bindingPoint, GLID);
    } else {
        glGenTextures(1, &GLID);
        glBindTexture(bindingPoint, GLID);
        buffer->bindOn(Buffer::BufferTextureStorage);
        glTexBuffer(bindingPoint, internalFormat, buffer->getID());
        buffer->unbind();
    }
}


GLTexture1D::GLTexture1D(GLGraphic * gl):ogl(gl), data(NULL), GLID(0) {
    this->minFilter = GL_LINEAR;
    this->magFilter = GL_LINEAR;
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
        glTexImage1D(GL_TEXTURE_1D, 0, choosePixelFormatFor(internalFormat, dataType, ogl->getExtensions().getTextureFormats()), width, 0, dataFormat, dataType, data);
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







GLTexture2D::GLTexture2D(GLGraphic * gl): ogl(gl), data(NULL), GLID(0) {
    this->minFilter = GL_LINEAR;
    this->magFilter = GL_LINEAR;
	width = 0;
	height = 0;
}

const bool GLTexture2D::isInitialized() const {
	return GLID != 0;
}

void GLTexture2D::initialize() {
	pbge::TextureUnit * unit = ogl->chooseTextureUnit(this);
	bindTextureOn(unit);
	unit->applyChanges(ogl);
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
	if(unit != NULL) {
		glActiveTexture(GL_TEXTURE0 + unit->getIndex());
		if(this->GLID != 0) {
			glBindTexture(GL_TEXTURE_2D, this->GLID);
		} else {
			glGenTextures(1, &GLID);
			glBindTexture(GL_TEXTURE_2D, this->GLID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->minFilter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->magFilter);
            glTexImage2D(GL_TEXTURE_2D, 0, choosePixelFormatFor(internalFormat, dataType, ogl->getExtensions().getTextureFormats()), width, height, 0, dataFormat, dataType, data);
		}
	} else {
		glBindTexture(GL_TEXTURE_2D, 0);
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
            glTexImage2D(GL_TEXTURE_2D, 0, choosePixelFormatFor(internalFormat, dataType, ogl->getExtensions().getTextureFormats()), width, height, 0, dataFormat, dataType, data);
        }
}

void GLTexture2D::replaceInternalData(Texture::DataType type, Texture::Format _dataFormat, void * image, unsigned size, 
                                      int w, int h, Texture::Format format) {
    this->internalFormat = format;
    this->width = w;
    this->height = h;
	this->dataFormat = _dataFormat;
    if(this->data != NULL) {
        free(this->data);
        this->data = NULL;
    }
	if(image != NULL) {
		data = malloc(size);
		memcpy(data, image, size);
	} else {
		data = NULL;
	}
    this->dataType = type;
}

bool GLTexture2D::isCompatibleWith(unsigned w, unsigned h, Texture::Format format) {
    return ((this->width == w) && 
            (this->height == h) && 
            (this->internalFormat == format));
}