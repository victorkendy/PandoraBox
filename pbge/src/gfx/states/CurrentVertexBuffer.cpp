#include "pbge/gfx/Buffer.h"
#include "pbge/gfx/VBO.h"
#include "pbge/gfx/states/CurrentVertexBuffer.h"

using namespace pbge;

void CurrentVertexBuffer::makeChange(GraphicAPI * gfx) {
    if(current != NULL) {
        current->getBuffer()->unbind();
    }
    if(next != NULL) {
        next->getBuffer()->bindOn(Buffer::VertexBuffer);
    }
    current = next;
}

bool CurrentVertexBuffer::shouldChange(GraphicAPI * gfx) {
    return current != next;
}

VertexBuffer * CurrentVertexBuffer::getCurrent() {
    return current;
}

void CurrentVertexBuffer::changeVBO(VertexBuffer * buffer) {
    this->next = buffer;
}