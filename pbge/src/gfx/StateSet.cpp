#include <vector>
#include <algorithm>
#include "pbge/gfx/OpenGL.h"
#include "pbge/internal/OpenGLStates.h"
#include "pbge/gfx/UniformStack.h"
#include "pbge/gfx/UniformSet.h"
#include "pbge/gfx/StateSet.h"

using namespace pbge;

const unsigned NUMBER_OF_STATES = 17;

class ChangeApplier {
public:
    ChangeApplier(OpenGL * _ogl) {
        this->ogl = _ogl;
    }

    void operator () (State * state) {
        state->applyChanges(ogl);
    }
private:
    OpenGL * ogl;
};


StateSet::StateSet(OpenGL * ogl) {
    states = std::vector<State*>(NUMBER_OF_STATES);
    states[OpenGL::BLEND] = new StateEnabler(GL_BLEND);
    states[OpenGL::COLOR_LOGIC_OP] = new StateEnabler(GL_COLOR_LOGIC_OP);
    states[OpenGL::DEPTH_TEST] = new StateEnabler(GL_DEPTH_TEST);
    states[OpenGL::STENCIL_TEST] = new StateEnabler(GL_STENCIL_TEST);
    states[OpenGL::SCISSOR_TEST] = new StateEnabler(GL_SCISSOR_TEST);
    states[OpenGL::CULL_FACE] = new StateEnabler(GL_CULL_FACE);
    states[OpenGL::DEPTH_CLAMP] = new StateEnabler(GL_DEPTH_CLAMP);
    states[OpenGL::DITHER] = new StateEnabler(GL_DITHER);
    states[OpenGL::LINE_SMOOTH] = new StateEnabler(GL_LINE_SMOOTH);
    states[OpenGL::MULTISAMPLE] = new StateEnabler(GL_MULTISAMPLE);
    states[OpenGL::POLYGON_OFFSET_FILL] = new StateEnabler(GL_POLYGON_OFFSET_FILL);
    states[OpenGL::POLYGON_OFFSET_LINE] = new StateEnabler(GL_POLYGON_OFFSET_LINE);
    states[OpenGL::POLYGON_OFFSET_POINT] = new StateEnabler(GL_POLYGON_OFFSET_POINT);
    states[OpenGL::POLYGON_SMOOTH] = new StateEnabler(GL_POLYGON_SMOOTH);
    states[OpenGL::PRIMITIVE_RESTART] = new StateEnabler(GL_PRIMITIVE_RESTART);
    states[OpenGL::SAMPLE_ALPHA_TO_COVERAGE] = new StateEnabler(GL_SAMPLE_ALPHA_TO_COVERAGE);
    states[OpenGL::PROGRAM_POINT_SIZE] = new StateEnabler(GL_PROGRAM_POINT_SIZE);

    boundProgram = new BoundProgram;
    uniformStack = new UniformStack;

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
}

void StateSet::apply(OpenGL * ogl) {
    ChangeApplier applier(ogl);
    std::for_each(changes.begin(), changes.end(), applier);
    changes.clear();
    if(this->boundProgram != NULL)
        this->boundProgram->updateUniforms(ogl);
}

void StateSet::enable(OpenGL::Mode mode) {
    dynamic_cast<StateEnabler*>(states.at(mode))->enable();
    changes.insert(states.at(mode));
}

void StateSet::disable(OpenGL::Mode mode) {
    dynamic_cast<StateEnabler*>(states.at(mode))->disable();
    changes.insert(states.at(mode));
}

void StateSet::useProgram(GPUProgram * program) {
    this->boundProgram->changeProgram(program);
    changes.insert(boundProgram);
}

UniformValue * StateSet::getUniformValue(const UniformInfo & info) {
    return uniformStack->getGloabalUniforms()->getValue(info);
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