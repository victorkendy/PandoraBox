#ifndef TENSOR_FIELD_PEELINGAWARENODE_H_
#define TENSOR_FIELD_PEELINGAWARENODE_H_

#include <cmath>
#include <cfloat>
#include <boost/smart_ptr/scoped_ptr.hpp>

#include "pbge/pbge.h"
#include "math3d/math3d.h"

#include "BoundingBox.h"
#include "LODModels.h"

class PeelingAwareNode {
public:
    virtual void renderPeeling(pbge::GraphicAPI * gfx) = 0;
    virtual void postRenderPeeling(pbge::GraphicAPI * gfx) = 0;
};

class PeelingAwareCollection : public pbge::ModelCollection, public PeelingAwareNode {
public:
    PeelingAwareCollection(pbge::Model * _model, pbge::GPUProgram * _peelingProgram) : pbge::ModelCollection(_model), peelingProgram(_peelingProgram), models(NULL) {}
    PeelingAwareCollection(LODModels * _models, pbge::GPUProgram * _peelingProgram) : pbge::ModelCollection(_models->forDistance(0)), peelingProgram(_peelingProgram), models(_models) {}
    void renderPeeling(pbge::GraphicAPI * gfx) {
        gfx->pushUniforms(getUniformSet());
        gfx->getState()->useProgram(this->peelingProgram);
        gfx->updateState();

        pbge::VBOModel * vboModel = dynamic_cast<pbge::VBOModel *>(getModel());
        gfx->getDrawController()->drawVBOModel(vboModel, getNumberOfInstances());
    }
    void updatePass(pbge::UpdaterVisitor * visitor, pbge::GraphicAPI * gfx) {
        if(boundingBox == NULL || models == NULL) {
            return;
        }
        math3d::matrix44 inveresedViewMatrix = gfx->getViewMatrix().inverse();
        math3d::vector4 cameraPosition(inveresedViewMatrix[0][3], inveresedViewMatrix[1][3], inveresedViewMatrix[2][3]);
        float dist = FLT_MAX;
        if(cameraIsInBoundingBox(cameraPosition)) {
            dist = 0.0f;
        }
        else {
            for(int i = 0; i < 8; i++) {
                float dist_aux = (boundingBox->positions[i] - cameraPosition).size();
                if(dist_aux < dist) dist = dist_aux;
            }
        }
        setModel(models->forDistance(dist));
    }
    void postRenderPeeling(pbge::GraphicAPI * gfx) {
        gfx->popUniforms();
    }
    void setBoundingBox(BoundingBox box) {
        boundingBox.reset(new FullBoundingBox(box));
    }
private:
    pbge::GPUProgram * peelingProgram;
    boost::scoped_ptr<FullBoundingBox> boundingBox;
    LODModels * models;

    bool cameraIsInBoundingBox(const math3d::vector4 & camera_position) {
        float x = camera_position[0];
        float y = camera_position[1];
        float z = camera_position[2];
        return boundingBox->max_x > x && boundingBox->min_x < x &&
               boundingBox->max_y > y && boundingBox->min_y < y &&
               boundingBox->max_z > z && boundingBox->min_z < z;
    }
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