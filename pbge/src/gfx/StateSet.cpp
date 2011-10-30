#include <vector>
#include <algorithm>

#include "pbge/gfx/GraphicAPI.h"

#include "pbge/gfx/UniformStack.h"
#include "pbge/gfx/UniformSet.h"
#include "pbge/gfx/StateSet.h"
#include "pbge/gfx/states/TextureUnits.h"
#include "pbge/gfx/states/BoundProgram.h"
#include "pbge/internal/OpenGLStates.h"

using namespace pbge;

StateSet::StateSet(GraphicAPI * ogl) {
    boundProgram = new BoundProgram;
    uniformStack = new UniformStack;
    textureUnits = new TextureUnits(ogl);
    boundFBO = new BoundFBO;
    vertexBuffer = new CurrentVertexBuffer;
}

StateSet::~StateSet() {
    std::vector<State*>::iterator it;
    std::vector<TextureUnit*>::iterator texIt;
    delete boundProgram;
    delete textureUnits;
    delete boundFBO;
}

void StateSet::apply(GraphicAPI * ogl) {
    std::for_each(changes.begin(), changes.end(), std::bind2nd(std::mem_fun(&State::applyChanges), ogl));
    changes.clear();
    boundProgram->updateUniforms(ogl);
    boundProgram->updateAttributes(vertexBuffer->getCurrent());
}

void StateSet::useProgram(GPUProgram * program) {
    this->boundProgram->changeProgram(program);
    changes.insert(boundProgram);
}

void StateSet::useFBO(FramebufferObject * fbo) {
    boundFBO->changeFBO(fbo);
    changes.insert(boundFBO);
}

void StateSet::useVertexBuffer(VertexBuffer * buffer) {
    vertexBuffer->changeVBO(buffer);
    changes.insert(vertexBuffer);
}

GPUProgram * StateSet::currentProgram() {
    return boundProgram->getCurrent();
}

UniformValue * StateSet::getUniformValue(const UniformInfo & info) {
    UniformSet * set = uniformStack->getGloabalUniforms();
    return set->getValue(info);
}

UniformValue * StateSet::searchUniform(const UniformInfo & info) {
    return uniformStack->findUniform(info);
}

TextureUnit * StateSet::chooseTexUnit(Texture * texture) {
    return textureUnits->unitFor(texture);
}

void StateSet::pushUniformSet(UniformSet * uniforms) {
    this->uniformStack->push(uniforms);
}

void StateSet::popUniformSet() {
    this->uniformStack->pop();
}
