#include <GL/glew.h>

#include "OpenGLAPI/gfx/GLBuffer.h"

using namespace pbge;

GLenum translateUsageHint(Buffer::UsageHint usageHint) {
    GLenum usage;
    switch (usageHint) {
        case Buffer::STREAM_COPY: usage = GL_STREAM_COPY; break;
        case Buffer::STREAM_DRAW: usage = GL_STREAM_DRAW; break;
        case Buffer::STREAM_READ: usage = GL_STREAM_READ; break;

        case Buffer::STATIC_COPY: usage = GL_STATIC_COPY; break;
        case Buffer::STATIC_DRAW: usage = GL_STATIC_DRAW; break;
        case Buffer::STATIC_READ: usage = GL_STATIC_READ; break;

        case Buffer::DYNAMIC_COPY: usage = GL_DYNAMIC_COPY; break;
        case Buffer::DYNAMIC_DRAW: usage = GL_DYNAMIC_DRAW; break;
        case Buffer::DYNAMIC_READ: usage = GL_DYNAMIC_READ; break;

        default: throw 1;
    }
    return usage;
}

GLBuffer::GLBuffer(size_t _size, Buffer::UsageHint _usage, GraphicAPI * ogl):size(_size), target(0), glID(0), gl(ogl) {
    this->usage = translateUsageHint(_usage);
    this->data = malloc(size);
    if (this->data == NULL) {
        // error
    }
} 

void * GLBuffer::map(AccessPattern accessPattern) {
    unsigned access = 0;
    if(data != NULL) {
        return data;
    }
    switch(accessPattern) {
        case READ_ONLY: access = GL_READ_ONLY; break;
        case WRITE_ONLY: access = GL_WRITE_ONLY; break;
        case READ_WRITE: access = GL_READ_WRITE; break;
        // TODO: log warn
        default: throw 1;
    }
    glBindBuffer(target, glID);
    return glMapBuffer(target, access);
}

void GLBuffer::unmap() {
    if(data == NULL)
        glUnmapBuffer(target);
}

void GLBuffer::bindOn(Target _target) {
    GLenum bindPoint;
    switch(_target) {
        case Buffer::VertexBuffer: bindPoint = GL_ARRAY_BUFFER; break;
        case Buffer::IndexBuffer: bindPoint = GL_ELEMENT_ARRAY_BUFFER; break;
        case Buffer::PixelReadBackBuffer: bindPoint = GL_PIXEL_PACK_BUFFER; break;
        case Buffer::PixelSendBuffer: bindPoint = GL_PIXEL_UNPACK_BUFFER; break;
        // TODO: log error
        default: throw 1;
    }
    // the default target will be the target of the first bind
    if (this->target == 0) {
        this->target = bindPoint;
    }
    if (this->glID == 0) {
        this->initialize();
    }
    glBindBuffer(bindPoint, glID);
}

void GLBuffer::unbind() {
    glBindBuffer(target, 0);
}

void GLBuffer::initialize() {
    glGenBuffers(1, &glID);
    glBindBuffer(target, glID);
    if (data != NULL) {
        glBufferData(target, size, data, usage);
        delete data;
        data = NULL;
    } else { // maybe allocation failed?
        glBufferData(target, size, NULL, usage);
    }
}

void GLBuffer::destroy() {
    if (glID != 0) {
        glDeleteBuffers(1, &glID);
    }
    if (this->data != NULL) {
        delete data;
    }
}