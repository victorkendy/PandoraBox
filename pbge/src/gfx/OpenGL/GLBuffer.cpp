

#include "pbge/gfx/OpenGL/GLBuffer.h"

using namespace pbge;

GLBuffer::GLBuffer(size_t _size, GLenum _usage, GraphicAPI * ogl) {
    this->size = _size;
    this->usage = _usage;
    this->target = 0;
    this->data = malloc(size);
    if (this->data == NULL) {
        // error
    }
    this->glID = 0;
    this->gl = ogl;
} 

void * GLBuffer::map(GLenum access) {
    if (data == NULL) {
        glBindBuffer(target, glID);
        return glMapBuffer(target, access);
    }
    return data;
}

void GLBuffer::bindOn(GLenum _target) {
    // the default target will be the target of the first bind
    if (this->target == 0) {
        this->target = _target;
    }
    if (this->glID == 0) {
        this->initialize();
    }
    glBindBuffer(target, glID);
}

void GLBuffer::unbind() {
    glBindBuffer(target, 0);
}

void GLBuffer::initialize() {
    glGenBuffers(1, &glID);
    glBindBuffer(target, glID);
    if (data != NULL) {
        glBufferData(target, size, usage, data);
        delete data;
        data = NULL;
    } else { // maybe allocation failed?
        glBufferData(target, size, usage, NULL);
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