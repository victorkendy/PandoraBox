
#include "pbge/gfx/Buffer.h"


using namespace pbge;


Buffer * OpenGL::createBuffer(size_t _size, GLenum _usage, GLenum _target) {
    return new Buffer(_size, _usage, _target);
}
