#ifndef TENSOR_FIELD_DEPTHPEELINGPROCESSOR_H_
#define TENSOR_FIELD_DEPTHPEELINGPROCESSOR_H_

#include <boost/smart_ptr/scoped_ptr.hpp>

#include "pbge/pbge.h"

class DepthPeelingProcessor : public pbge::SceneProcessor {
public:
    DepthPeelingProcessor() : initialized(false) {}
    void initialize(pbge::GraphicAPI * gfx, pbge::Renderer * renderer);
    void process(pbge::GraphicAPI * gfx, pbge::Renderer * renderer);
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

    bool initialized;
    bool active;
};

#endif