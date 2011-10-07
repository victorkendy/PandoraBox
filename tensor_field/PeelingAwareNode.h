#ifndef TENSOR_FIELD_PEELINGAWARENODE_H_
#define TENSOR_FIELD_PEELINGAWARENODE_H_

#include "pbge/pbge.h"
#include <cmath>

class PeelingAwareNode {
public:
    virtual void renderPeeling(pbge::GraphicAPI * gfx) = 0;
    virtual void postRenderPeeling(pbge::GraphicAPI * gfx) = 0;
};

class PeelingAwareCollection : public pbge::ModelCollection, public PeelingAwareNode {
public:
    PeelingAwareCollection(pbge::Model * _model, pbge::GPUProgram * _peelingProgram) : pbge::ModelCollection(_model), peelingProgram(_peelingProgram) {}
    void renderPeeling(pbge::GraphicAPI * gfx) {
        gfx->pushUniforms(getUniformSet());
        gfx->getState()->useProgram(this->peelingProgram);
        gfx->updateState();

        pbge::VBOModel * vboModel = dynamic_cast<pbge::VBOModel *>(getModel());
        gfx->getDrawController()->drawVBOModel(vboModel, getNumberOfInstances());
    }
    void postRenderPeeling(pbge::GraphicAPI * gfx) {
        gfx->popUniforms();
    }
private:
    pbge::GPUProgram * peelingProgram;
};

class FieldParent : public pbge::TransformationNode, public PeelingAwareNode {
public:
    FieldParent(pbge::GPUProgram * _renderProgram, float _min_alpha, float _max_alpha, float _alpha_step) : alpha_step(_alpha_step) {
        min_alpha = std::max(_min_alpha - _alpha_step, 0.0f);
        max_alpha = std::min(_max_alpha + _alpha_step, 1.0f);
        this->alpha_correction = 0;
        resetAlphaCorrection();
        this->alpha_changed_render = true;
        this->alpha_changed_peeling = true;
        uniform_alpha_correction = NULL;
        this->renderProgram = _renderProgram;
        this->uniforms = new pbge::UniformSet();
    }
    
    void renderPass(pbge::RenderVisitor * visitor, pbge::GraphicAPI * gfx) {
        gfx->pushUniforms(getUniformSet());
        gfx->getState()->useProgram(this->renderProgram);
        gfx->updateState();

        if(uniform_alpha_correction == NULL) {
            uniform_alpha_correction = getUniformSet()->getFloat("alpha_correction");
        }
        
        if(alpha_changed_render) {
            alpha_changed_render = false;
            uniform_alpha_correction->setValue(alpha_correction);
        }
    }

    void postRenderPass(pbge::RenderVisitor * visitor, pbge::GraphicAPI * gfx) {
        gfx->popUniforms();
    }

    void renderPeeling(pbge::GraphicAPI * gfx) {
        gfx->pushUniforms(getUniformSet());
        gfx->getState()->useProgram(this->renderProgram);
        gfx->updateState();

        if(uniform_alpha_correction == NULL) {
            uniform_alpha_correction = getUniformSet()->getFloat("alpha_correction");
        }
        
        if(alpha_changed_peeling) {
            alpha_changed_peeling = false;
            uniform_alpha_correction->setValue(alpha_correction);
        }
    }

    void postRenderPeeling(pbge::GraphicAPI * gfx) {
        gfx->popUniforms();
    }

    void stepForward() {
        float new_alpha_correction = alpha_correction + alpha_step;
        if(new_alpha_correction > max_alpha) {
            new_alpha_correction = max_alpha;
        }
        setAlphaCorrection(new_alpha_correction);
    }

    void stepBackward() {
        float new_alpha_correction = alpha_correction - alpha_step;
        if(new_alpha_correction < min_alpha) {
            new_alpha_correction = min_alpha;
        }
        setAlphaCorrection(new_alpha_correction);
    }

    void resetAlphaCorrection() {
        setAlphaCorrection(min_alpha);
    }
private:
    float alpha_correction;
    bool alpha_changed_render;
    bool alpha_changed_peeling;
    pbge::UniformFloat * uniform_alpha_correction;
    pbge::GPUProgram * renderProgram;
    pbge::UniformSet * uniforms;
    float min_alpha;
    float max_alpha;
    float alpha_step;

    pbge::UniformSet * getUniformSet() {
        return uniforms;
    }

    void setAlphaCorrection(float new_alpha_correction) {
        if(alpha_correction != new_alpha_correction) {
            alpha_correction = new_alpha_correction;
            alpha_changed_render = true;
            alpha_changed_peeling = true;
        }
    }
};

#endif