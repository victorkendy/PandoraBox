#ifndef TENSOR_FIELD_HIDEABLENODE_H_
#define TENSOR_FIELD_HIDEABLENODE_H_

#include "pbge/pbge.h"

class HideableModelInstanceDecorator : public pbge::Node {
public:
    // Node interface methods
    HideableModelInstanceDecorator(pbge::ModelInstance * decorated) : visible(true) {
        this->node = decorated;
    }

    virtual void updatePass(pbge::UpdaterVisitor * visitor, pbge::GraphicAPI * gfx) {
        node->updatePass(visitor, gfx);
    }

    virtual void postUpdatePass(pbge::UpdaterVisitor * visitor, pbge::GraphicAPI * gfx) {
        node->postUpdatePass(visitor, gfx);
    }

    virtual void renderPass(pbge::RenderVisitor * visitor, pbge::GraphicAPI * gfx) {
        if(visible)
            node->renderPass(visitor, gfx);
    }

    virtual void postRenderPass(pbge::RenderVisitor * visitor, pbge::GraphicAPI * gfx) {
        if(visible)
            node->postRenderPass(visitor, gfx);
    }

    virtual void depthPass(pbge::RenderVisitor * visitor, pbge::GraphicAPI * gfx) {
        if(visible)
            node->depthPass(visitor, gfx);
    }

    virtual void postDepthPass(pbge::RenderVisitor * visitor, pbge::GraphicAPI * gfx) {
        if(visible)
            node->postDepthPass(visitor, gfx);
    }

    virtual void addChild(pbge::Node * node) {
        node->addChild(node);
    }
        
    virtual node_list & getChildren() {
        return node->getChildren();
    }

    int getSceneGraphIndex() {
        return node->getSceneGraphIndex();
    }

    void setSceneGraphIndex(int index) {
        node->setSceneGraphIndex(index);
    }

    virtual pbge::AABB * getBoundingVolume() {
        return node->getBoundingVolume();
    }

    // ModelInstance methods
    pbge::Model * getModel() {
        return node->getModel();
    }

    void setDepthPassProgram(pbge::GPUProgram * depthPassProgram) {
        node->setDepthPassProgram(depthPassProgram);
    }

    void setRenderPassProgram(pbge::GPUProgram * renderPassProgram) {
        node->setRenderPassProgram(renderPassProgram);
    }

    pbge::UniformSet * getUniformSet() {
        return node->getUniformSet();
    }

    // HideableModelInstanceDecorator methods
    void toggle() {
        visible = !visible;
    }
private:
    pbge::ModelInstance * node;
    bool visible;
};

#endif