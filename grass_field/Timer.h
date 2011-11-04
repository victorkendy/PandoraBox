#ifndef GRASS_FIELD_TIMER_H_
#define GRASS_FIELD_TIMER_H_

#include <ctime>

#include "pbge/pbge.h"

class Timer : public pbge::Node {
public:
    Timer() {
        this->begin = clock();
        timer = uniforms.getFloat("t");
    }

    virtual void updatePass(pbge::UpdaterVisitor * visitor, pbge::GraphicAPI * gfx) {}

    virtual void postUpdatePass(pbge::UpdaterVisitor * visitor, pbge::GraphicAPI * gfx) {}

    virtual void renderPass(pbge::RenderVisitor * visitor, pbge::GraphicAPI * gfx) {
        float time_elapsed = (float)(clock() - begin)/CLOCKS_PER_SEC;
        timer->setValue(time_elapsed);
        gfx->pushUniforms(&uniforms);
    }

    virtual void postRenderPass(pbge::RenderVisitor * visitor, pbge::GraphicAPI * gfx) {}

    virtual void depthPass(pbge::RenderVisitor * visitor, pbge::GraphicAPI * gfx) {}

    virtual void postDepthPass(pbge::RenderVisitor * visitor, pbge::GraphicAPI * gfx) {}

    virtual void addChild(pbge::Node * node) {
        children.push_back(node);
    }
    
    virtual node_list & getChildren() {
        return children;
    }
private:
    clock_t begin;
    pbge::UniformFloat * timer;
    pbge::UniformSet uniforms;
    pbge::Node::node_list children;
};

#endif