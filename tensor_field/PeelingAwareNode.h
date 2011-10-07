#ifndef TENSOR_FIELD_PEELINGAWARENODE_H_
#define TENSOR_FIELD_PEELINGAWARENODE_H_

#include "pbge/pbge.h"

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
    FieldParent(float _alpha_correction, pbge::GPUProgram * _renderProgram) {
        this->alpha_correction = _alpha_correction;
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

    void setAlphaCorrection(float new_alpha_correction) {
        if(new_alpha_correction < 0.0f) {
            new_alpha_correction = 0.0f;
        }
        else if(new_alpha_correction > 1.0f) {
            new_alpha_correction = 1.0f;
        }
        if(alpha_correction != new_alpha_correction) {
            alpha_correction = new_alpha_correction;
            alpha_changed_render = true;
            alpha_changed_peeling = true;
        }
    }

    float getAlphaCorrection() {
        return alpha_correction;
    }
private:
    float alpha_correction;
    bool alpha_changed_render;
    bool alpha_changed_peeling;
    pbge::UniformFloat * uniform_alpha_correction;
    pbge::GPUProgram * renderProgram;
    pbge::UniformSet * uniforms;

    pbge::UniformSet * getUniformSet() {
        return uniforms;
    }
};

#endif