#include <vector>
#include <algorithm>

#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/Buffer.h"
#include "pbge/gfx/VBO.h"

namespace pbge {
    void VertexBuffer::bind(GraphicAPI * gfx) {
        gfx->bindVertexBuffer(this);
    }

    void VertexBuffer::unbind(GraphicAPI * gfx) {
        this->buffer->unbind();
    }

    VertexBuffer::~VertexBuffer() {
        delete buffer;
    }
}