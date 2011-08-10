
#include <cstring>

#include "math3d/math3d.h"

#include "pbge/gfx/MatrixStack.h"

using namespace pbge;

MatrixStack::MatrixStack():size(16),index(-1) {
    matrices.reset(new math3d::matrix44[size]);
}
MatrixStack::~MatrixStack() {
    matrices.reset(NULL);
}
void MatrixStack::push(const math3d::matrix44 & m) {
    if(index == size - 1) {
        resize();
    }
    matrices[++index] = m;
}

void MatrixStack::pop() {
    if(index >= 0) {
        index--;
    }
}

const math3d::matrix44 MatrixStack::peek() const {
    if(index == -1) {
        return math3d::identity44;
    } else {
        return matrices[index];
    }
}

void MatrixStack::clear() {
    index = -1;
}

void MatrixStack::resize() {
    unsigned newSize = 2 * size;
    math3d::matrix44 * newMatrices = new math3d::matrix44[newSize];
    memcpy(newMatrices, matrices.get(), sizeof(math3d::matrix44) * size);
    matrices.reset(newMatrices);
    size = newSize;
}