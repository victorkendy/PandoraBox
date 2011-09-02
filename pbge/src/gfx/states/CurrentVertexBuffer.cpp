
#include "pbge/gfx/states/CurrentVertexBuffer.h"

using namespace pbge;

void CurrentVertexBuffer::makeChange(GraphicAPI * gfx) {
}

bool CurrentVertexBuffer::shouldChange(GraphicAPI * gfx) {
    return current != next;
}

VertexBuffer * CurrentVertexBuffer::getCurrent() {
    return current;
}