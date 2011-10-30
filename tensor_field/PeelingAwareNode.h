#ifndef TENSOR_FIELD_PEELINGAWARENODE_H_
#define TENSOR_FIELD_PEELINGAWARENODE_H_

#include <cmath>
#include <cfloat>
#include <boost/smart_ptr/scoped_ptr.hpp>
#include <boost/smart_ptr/scoped_array.hpp>
#include <algorithm>

#include "pbge/pbge.h"
#include "math3d/math3d.h"

#include "BoundingBox.h"
#include "LODModels.h"
#include "Comparators.h"

class PeelingAwareNode {
public:
    virtual void renderPeeling(pbge::GraphicAPI * gfx) = 0;
    virtual void postRenderPeeling(pbge::GraphicAPI * gfx) = 0;
};

class PeelingAwareCollection : public pbge::Node, public PeelingAwareNode {
public:
    PeelingAwareCollection(pbge::Model * _model, pbge::GPUProgram * _peelingProgram) : peelingProgram(_peelingProgram), models(NULL) {
        collection.reset(new pbge::ModelCollection(_model));
    }
    PeelingAwareCollection(LODModels * _models, pbge::GPUProgram * _peelingProgram) : peelingProgram(_peelingProgram), models(_models) {
        collection.reset(new pbge::ModelCollection(_models->forDistance(0)));
    }
    void setMinAlphaCorrection(float min_alpha_correction) {
        math3d::matrix44 * end = transforms.get() + number_of_instances;
        math3d::matrix44 * position = std::find_if(transforms.get(), end, LessThanOrEqualTo(min_alpha_correction - 0.01f));
        collection->setNumberOfInstances(number_of_instances - (end - position));
    }
    void setTransforms(math3d::matrix44 * _transforms) {
        transforms.reset(new math3d::matrix44[number_of_instances]);
        memcpy(transforms.get(), _transforms, sizeof(math3d::matrix44) * number_of_instances);
    }
    void renderPeeling(pbge::GraphicAPI * gfx) {
        gfx->pushUniforms(collection->getUniformSet());
        gfx->getState()->useProgram(this->peelingProgram);
        gfx->updateState();

        pbge::VBOModel * vboModel = dynamic_cast<pbge::VBOModel *>(collection->getModel());
        gfx->getDrawController()->drawVBOModel(vboModel, collection->getNumberOfInstances());
    }
    void postRenderPeeling(pbge::GraphicAPI * gfx) {
        gfx->popUniforms();
    }
    void updatePass(pbge::UpdaterVisitor * visitor, pbge::GraphicAPI * gfx) {
        if(models == NULL) {
            return;
        }
        math3d::matrix44 inveretedViewMatrix = gfx->getViewMatrix()->get().inverse();
        math3d::vector4 cameraPosition(inveretedViewMatrix[0][3], inveretedViewMatrix[1][3], inveretedViewMatrix[2][3], 1.0f);
        float dist = FLT_MAX;
        dist = boundingAABB.distance(cameraPosition);
        collection->setModel(models->forDistance(dist));
    }
    void postUpdatePass(pbge::UpdaterVisitor * visitor, pbge::GraphicAPI * gfx) {
        collection->postUpdatePass(visitor, gfx);
    }
    void renderPass(pbge::RenderVisitor * visitor, pbge::GraphicAPI * gfx) {
        collection->renderPass(visitor, gfx);
    }
    void postRenderPass(pbge::RenderVisitor * visitor, pbge::GraphicAPI * gfx) {
        collection->postRenderPass(visitor, gfx);
    }
    void depthPass(pbge::RenderVisitor * visitor, pbge::GraphicAPI * gfx) {
        collection->depthPass(visitor, gfx);
    }
    void postDepthPass(pbge::RenderVisitor * visitor, pbge::GraphicAPI * gfx) {
        collection->postDepthPass(visitor, gfx);
    }
    void addChild(pbge::Node * node) {
        collection->addChild(node);
    }
    pbge::Node::node_list & getChildren() {
        return collection->getChildren();
    }
    void setBoundingBox(BoundingBox box) {
        boundingAABB.setDimensions(
            box.min_x, box.min_y, box.min_z,
            box.max_x, box.max_y, box.max_z, pbge::AABB::WORLD_SPACE);
    }
    void setNumberOfInstances(int instances) {
        number_of_instances = instances;
        collection->setNumberOfInstances(instances);
    }
    pbge::UniformSet * getUniformSet() {
        return collection->getUniformSet();
    }
    void setRenderPassProgram(pbge::GPUProgram * renderPassProgram) {
        collection->setRenderPassProgram(renderPassProgram);
    }
    void setDepthPassProgram(pbge::GPUProgram * depthPassProgram) {
        collection->setDepthPassProgram(depthPassProgram);
    }

    pbge::AABB * getBoundingVolume() {
        return &boundingAABB;
    }
private:
    pbge::AABB boundingAABB;
    pbge::GPUProgram * peelingProgram;
    LODModels * models;
    boost::scoped_ptr<pbge::ModelCollection> collection;
    boost::scoped_array<math3d::matrix44> transforms;
    int number_of_instances;
};

class FieldParent : public pbge::TransformationNode, public PeelingAwareNode {
public:
    FieldParent(pbge::GPUProgram * _renderProgram, float _min_alpha, float _max_alpha, float _alpha_step, float dim[3]) : alpha_step(_alpha_step), scale(1.0f) {
        min_alpha = std::max(_min_alpha - _alpha_step, 0.0f);
        max_alpha = std::min(_max_alpha + _alpha_step, 1.0f);
        this->min_alpha_correction = 0;
        this->max_alpha_correction = 1;
        resetAlphaCorrection();
        this->renderProgram = _renderProgram;
        uniform_min_alpha_correction = getUniformSet()->getFloat("min_alpha");
        uniform_max_alpha_correction = getUniformSet()->getFloat("max_alpha");
        uniform_scale = getUniformSet()->getFloat("scale");
        uniform_scale->setValue(scale);
        for(int i = 0; i < 3; i++) {
            this->dimensions[i] = dim[i];
        }
    }

    float * getDim() {
        return dimensions;
    }
    
    void renderPass(pbge::RenderVisitor * visitor, pbge::GraphicAPI * gfx) {
        gfx->pushUniforms(getUniformSet());
        gfx->updateState();
        
        uniform_min_alpha_correction->setValue(min_alpha_correction);
        uniform_max_alpha_correction->setValue(max_alpha_correction);
        uniform_scale->setValue(scale);
    }

    void postRenderPass(pbge::RenderVisitor * visitor, pbge::GraphicAPI * gfx) {
        gfx->popUniforms();
    }

    void renderPeeling(pbge::GraphicAPI * gfx) {
        gfx->pushUniforms(getUniformSet());
        gfx->updateState();

        uniform_min_alpha_correction->setValue(min_alpha_correction);
        uniform_max_alpha_correction->setValue(max_alpha_correction);
        uniform_scale->setValue(scale);
    }

    void postRenderPeeling(pbge::GraphicAPI * gfx) {
        gfx->popUniforms();
    }

    void stepMinForward() {
        float new_alpha_correction = min_alpha_correction + alpha_step;
        if(new_alpha_correction > max_alpha) {
            new_alpha_correction = max_alpha;
        }
        setMinAlphaCorrection(new_alpha_correction);
    }

    void stepMinBackward() {
        float new_alpha_correction = min_alpha_correction - alpha_step;
        if(new_alpha_correction < min_alpha) {
            new_alpha_correction = min_alpha;
        }
        setMinAlphaCorrection(new_alpha_correction);
    }

    void stepMaxForward() {
        float new_alpha_correction = max_alpha_correction + alpha_step;
        if(new_alpha_correction > max_alpha) {
            new_alpha_correction = max_alpha;
        }
        setMaxAlphaCorrection(new_alpha_correction);
    }

    void stepMaxBackward() {
        float new_alpha_correction = max_alpha_correction - alpha_step;
        if(new_alpha_correction < min_alpha) {
            new_alpha_correction = min_alpha;
        }
        setMaxAlphaCorrection(new_alpha_correction);
    }

    void scaleUp() {
        scale += 0.05f;
    }

    void scaleDown() {
        scale = std::max(scale - 0.05f, 0.0f);
    }

    void resetAlphaCorrection() {
        setMinAlphaCorrection(min_alpha);
        setMaxAlphaCorrection(max_alpha);
    }
private:
    float min_alpha_correction;
    float max_alpha_correction;
    pbge::UniformFloat * uniform_min_alpha_correction;
    pbge::UniformFloat * uniform_max_alpha_correction;
    pbge::UniformFloat * uniform_scale;
    pbge::GPUProgram * renderProgram;
    pbge::UniformSet uniforms;
    float min_alpha;
    float max_alpha;
    float alpha_step;
    float scale;
    float dimensions[3];

    pbge::UniformSet * getUniformSet() {
        return &uniforms;
    }

    void setMinAlphaCorrection(float new_alpha_correction) {
        if(min_alpha_correction != new_alpha_correction) {
            printf("%f\n", new_alpha_correction);
            for(pbge::Node::node_list::iterator it = getChildren().begin(); it != getChildren().end(); it++) {
                dynamic_cast<PeelingAwareCollection *>(*it)->setMinAlphaCorrection(new_alpha_correction);
            }
            min_alpha_correction = new_alpha_correction;
        }
    }

    void setMaxAlphaCorrection(float new_alpha_correction) {
        max_alpha_correction = new_alpha_correction;
    }
};

#endif