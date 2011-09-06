#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/GPUProgram.h"
#include "pbge/gfx/states/BoundProgram.h"
#include "pbge/gfx/VBO.h"

using namespace pbge;

bool BoundProgram::shouldChange(GraphicAPI * ogl) {
    return this->current != this->next;
}

void BoundProgram::makeChange(GraphicAPI * ogl) {
    if(this->next == NULL)
        glUseProgram(0);
    else
        next->bind(ogl);
    current = next;
}

void BoundProgram::updateUniforms(GraphicAPI * ogl) {
    if(this->current != NULL) {
        this->current->updateUniforms(ogl);
    }
}

void BoundProgram::updateAttributes(VertexBuffer * vertexBuffer) {
    std::vector<VertexAttrib>::iterator it;
    if(this->current != NULL && vertexBuffer != NULL) {
        GPUProgram * program = this->current;
        program->setAttributes(vertexBuffer);
    }
}