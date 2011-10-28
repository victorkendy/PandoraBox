#ifndef TENSOR_FIELD_DEPTHPEELINGPROCESSOR_H_
#define TENSOR_FIELD_DEPTHPEELINGPROCESSOR_H_

#include <boost/smart_ptr/scoped_ptr.hpp>
#include <map>
#include <string>

#include "pbge/pbge.h"

class DepthPeelingProcessor : public pbge::SceneProcessor {
public:
    DepthPeelingProcessor() : initialized(false), active(false) {}
    void initialize(pbge::GraphicAPI * gfx, pbge::Renderer * renderer);
    void process(pbge::GraphicAPI * gfx, pbge::Renderer * renderer, pbge::Camera * camera);
    bool isInitialized(pbge::GraphicAPI * gfx) {
        return initialized;
    }
    bool isActive() {
        return active;
    }
    void toggle() {
        active = !active;
    }
private:
    boost::scoped_ptr<pbge::GPUProgram> blitProgram;
    boost::scoped_ptr<pbge::FramebufferObject> fbos[2];
    boost::scoped_ptr<pbge::Texture2D> colors[2];
    boost::scoped_ptr<pbge::Texture2D> depths[2];
    bool initialized;
    bool active;
    void createFBO(pbge::GraphicAPI* gfx, pbge::Renderer * renderer, int i);
};

#endif