#include <vector>
#include <algorithm>
#include "pbge/gfx/GraphicAPI.h"
#include "pbge/internal/OpenGLStates.h"
#include "pbge/gfx/UniformStack.h"
#include "pbge/gfx/UniformSet.h"
#include "pbge/gfx/StateSet.h"

using namespace pbge;

const unsigned NUMBER_OF_STATES = 17;

StateSet::StateSet(GraphicAPI * ogl) {
    states = std::vector<State*>(NUMBER_OF_STATES);
    states[GraphicAPI::BLEND] = new StateEnabler(GL_BLEND);
    states[GraphicAPI::COLOR_LOGIC_OP] = new StateEnabler(GL_COLOR_LOGIC_OP);
    states[GraphicAPI::DEPTH_TEST] = new StateEnabler(GL_DEPTH_TEST);
    states[GraphicAPI::STENCIL_TEST] = new StateEnabler(GL_STENCIL_TEST);
    states[GraphicAPI::SCISSOR_TEST] = new StateEnabler(GL_SCISSOR_TEST);
    states[GraphicAPI::CULL_FACE] = new StateEnabler(GL_CULL_FACE);
    states[GraphicAPI::DEPTH_CLAMP] = new StateEnabler(GL_DEPTH_CLAMP);
    states[GraphicAPI::DITHER] = new StateEnabler(GL_DITHER);
    states[GraphicAPI::LINE_SMOOTH] = new StateEnabler(GL_LINE_SMOOTH);
    states[GraphicAPI::MULTISAMPLE] = new StateEnabler(GL_MULTISAMPLE);
    states[GraphicAPI::POLYGON_OFFSET_FILL] = new StateEnabler(GL_POLYGON_OFFSET_FILL);
    states[GraphicAPI::POLYGON_OFFSET_LINE] = new StateEnabler(GL_POLYGON_OFFSET_LINE);
    states[GraphicAPI::POLYGON_OFFSET_POINT] = new StateEnabler(GL_POLYGON_OFFSET_POINT);
    states[GraphicAPI::POLYGON_SMOOTH] = new StateEnabler(GL_POLYGON_SMOOTH);
    states[GraphicAPI::PRIMITIVE_RESTART] = new StateEnabler(GL_PRIMITIVE_RESTART);
    states[GraphicAPI::SAMPLE_ALPHA_TO_COVERAGE] = new StateEnabler(GL_SAMPLE_ALPHA_TO_COVERAGE);
    states[GraphicAPI::PROGRAM_POINT_SIZE] = new StateEnabler(GL_PROGRAM_POINT_SIZE);

    boundProgram = new BoundProgram;
    uniformStack = new UniformStack;
    boundFBO = new BoundFBO;

    int numberOfTextureUnits = ogl->numberOfTextureUnits();
    for (int i = 0; i < numberOfTextureUnits; i++)
        textureUnits.push_back(new TextureUnit(ogl, i));
}

StateSet::~StateSet() {
    std::vector<State*>::iterator it;
    std::vector<TextureUnit*>::iterator texIt;
    for(it = states.begin(); it != states.end(); it++) {
        delete *it;
    }
    for(texIt = textureUnits.begin(); texIt != textureUnits.end(); texIt++) {
        delete *texIt;
    }

    delete boundProgram;
    delete boundFBO;
}

void StateSet::apply(GraphicAPI * ogl) {
    std::for_each(changes.begin(), changes.end(), std::bind2nd(std::mem_fun(&State::applyChanges), ogl));
    changes.clear();
    if(this->boundProgram != NULL)
        this->boundProgram->updateUniforms(ogl);
}

void StateSet::enable(GraphicAPI::Mode mode) {
    dynamic_cast<StateEnabler*>(states.at(mode))->enable();
    changes.insert(states.at(mode));
}

void StateSet::disable(GraphicAPI::Mode mode) {
    dynamic_cast<StateEnabler*>(states.at(mode))->disable();
    changes.insert(states.at(mode));
}

void StateSet::useProgram(GPUProgram * program) {
    this->boundProgram->changeProgram(program);
    changes.insert(boundProgram);
}

void StateSet::useFBO(FramebufferObject * fbo) {
    boundFBO->changeFBO(fbo);
    changes.insert(boundFBO);
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
    std::vector<TextureUnit*>::iterator unit;
    TextureUnit * chosen = NULL;
    for(unit = textureUnits.begin(); unit != textureUnits.end(); unit++) {
        if((*unit)->getCurrentTexture() == texture) {
            chosen = *unit;
            break;
        } else if(((*unit)->getCurrentTexture() == NULL) && (chosen == NULL))
            chosen = *unit;
    }
    return chosen;
}

void StateSet::pushUniformSet(UniformSet * uniforms) {
    this->uniformStack->push(uniforms);
}

void StateSet::popUniformSet() {
    this->uniformStack->pop();
}
