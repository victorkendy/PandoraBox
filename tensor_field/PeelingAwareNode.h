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
    void setAlphaCorrection(float alpha_correction) {
        math3d::matrix44 * end = transforms.get() + number_of_instances;
        math3d::matrix44 * position = std::find_if(transforms.get(), end, LessThanOrEqualTo(alpha_correction - 0.01f));
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
        boundingBox.reset(new FullBoundingBox(box));
        boundingAABB.setDimensions(
            boundingBox->min_x, boundingBox->min_y, boundingBox->min_z,
            boundingBox->max_x, boundingBox->max_y, boundingBox->max_z, pbge::AABB::WORLD_SPACE);
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
    boost::scoped_ptr<FullBoundingBox> boundingBox;
    LODModels * models;
    boost::scoped_ptr<pbge::ModelCollection> collection;
    boost::scoped_array<math3d::matrix44> transforms;
    int number_of_instances;

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
    FieldParent(pbge::GPUProgram * _renderProgram, float _min_alpha, float _max_alpha, float _alpha_step) : alpha_step(_alpha_step), scale(1.0f) {
        min_alpha = std::max(_min_alpha - _alpha_step, 0.0f);
        max_alpha = std::min(_max_alpha + _alpha_step, 1.0f);
        this->alpha_correction = 0;
        resetAlphaCorrection();
        this->renderProgram = _renderProgram;
        uniform_alpha_correction = getUniformSet()->getFloat("alpha_correction");
        uniform_scale = getUniformSet()->getFloat("scale");
        uniform_scale->setValue(scale);
    }
    
    void renderPass(pbge::RenderVisitor * visitor, pbge::GraphicAPI * gfx) {
        gfx->pushUniforms(getUniformSet());
        gfx->getState()->useProgram(this->renderProgram);
        gfx->updateState();
        
        uniform_alpha_correction->setValue(alpha_correction);
        uniform_scale->setValue(scale);
    }

    void postRenderPass(pbge::RenderVisitor * visitor, pbge::GraphicAPI * gfx) {
        gfx->popUniforms();
    }

    void renderPeeling(pbge::GraphicAPI * gfx) {
        gfx->pushUniforms(getUniformSet());
        gfx->getState()->useProgram(this->renderProgram);
        gfx->updateState();

        uniform_alpha_correction->setValue(alpha_correction);
        uniform_scale->setValue(scale);
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

    void scaleUp() {
        scale += 0.05f;
    }

    void scaleDown() {
        scale = std::max(scale - 0.05f, 0.0f);
    }

    void resetAlphaCorrection() {
        setAlphaCorrection(min_alpha);
    }
private:
    float alpha_correction;
    pbge::UniformFloat * uniform_alpha_correction;
    pbge::UniformFloat * uniform_scale;
    pbge::GPUProgram * renderProgram;
    pbge::UniformSet uniforms;
    float min_alpha;
    float max_alpha;
    float alpha_step;
    float scale;

    pbge::UniformSet * getUniformSet() {
        return &uniforms;
    }

    void setAlphaCorrection(float new_alpha_correction) {
        if(alpha_correction != new_alpha_correction) {
            for(pbge::Node::node_list::iterator it = getChildren().begin(); it != getChildren().end(); it++) {
                dynamic_cast<PeelingAwareCollection *>(*it)->setAlphaCorrection(alpha_correction);
            }
            alpha_correction = new_alpha_correction;
        }
    }
};

#endif