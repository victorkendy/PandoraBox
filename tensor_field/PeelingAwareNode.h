#ifndef TENSOR_FIELD_PEELINGAWARENODE_H_
#define TENSOR_FIELD_PEELINGAWARENODE_H_

#include "pbge/pbge.h"
#include "math3d/math3d.h"

#include "BoundingBox.h"

class PeelingAwareNode {
public:
    virtual void renderPeeling(pbge::GraphicAPI * gfx) = 0;
    virtual void postRenderPeeling(pbge::GraphicAPI * gfx) = 0;
};

class PeelingAwareCollection : public pbge::ModelCollection, public PeelingAwareNode {
public:
    PeelingAwareCollection(pbge::Model * _model, pbge::GPUProgram * _peelingProgram) : pbge::ModelCollection(_model), peelingProgram(_peelingProgram), boundingBox(NULL) {}
    PeelingAwareCollection(pbge::Model * _models[], pbge::GPUProgram * _peelingProgram) : PeelingAwareCollection(_models[0], _peelingProgram), models(_models) {}
    void renderPeeling(pbge::GraphicAPI * gfx) {
        gfx->pushUniforms(getUniformSet());
        gfx->getState()->useProgram(this->peelingProgram);
        gfx->updateState();

        pbge::VBOModel * vboModel = dynamic_cast<pbge::VBOModel *>(getModel());
        gfx->getDrawController()->drawVBOModel(vboModel, getNumberOfInstances());
    }
    void updatePass(pbge::UpdaterVisitor * visitor, pbge::GraphicAPI * gfx) {
        if(box == NULL || models == NULL) {
            return;
        }
        math3d::matrix44 transposedInveresedViewMatrix = gfx->getViewMatrix().inverse().transpose();
        math3d::vector4 cameraPosition(transposedInveresedViewMatrix[3]);
        
    }
    void postRenderPeeling(pbge::GraphicAPI * gfx) {
        gfx->popUniforms();
    }
    void setBoundingBox(BoundingBox * box) {
        boundingBox = box;
    }
private:
    pbge::GPUProgram * peelingProgram;
    BoundingBox * boundingBox;
    pbge::Model * models[];
};

class FieldParent : public pbge::TransformationNode, public PeelingAwareNode {
public:
    FieldParent(float _alpha_correction, pbge::GPUProgram * _renderProgram) {
        this->alpha_correction = _alpha_correction;
        this->alpha_changed = true;
        uniform_alpha_correction = NULL;
        this->renderProgram = _renderProgram;
        this->uniforms = new pbge::UniformSet();
    }
    
    void renderPeeling(pbge::GraphicAPI * gfx) {
        gfx->pushUniforms(getUniformSet());
        gfx->getState()->useProgram(this->renderProgram);
        gfx->updateState();

        if(uniform_alpha_correction == NULL) {
            uniform_alpha_correction = getUniformSet()->getFloat("alpha_correction");
        }
        
        if(alpha_changed) {
            alpha_changed = false;
            uniform_alpha_correction->setValue(alpha_correction);
        }
    }

    void postRenderPeeling(pbge::GraphicAPI * gfx) {
        gfx->popUniforms();
    }

    void setAlphaCorrection(float new_alpha_correction) {
        if(alpha_correction != new_alpha_correction && new_alpha_correction >= 0 && new_alpha_correction <= 1) {
            alpha_correction = new_alpha_correction;
            alpha_changed = true;
        }
    }

    float getAlphaCorrection() {
        return alpha_correction;
    }
private:
    float alpha_correction;
    bool alpha_changed;
    pbge::UniformFloat * uniform_alpha_correction;
    pbge::GPUProgram * renderProgram;
    pbge::UniformSet * uniforms;

    pbge::UniformSet * getUniformSet() {
        return uniforms;
    }
};

#endif