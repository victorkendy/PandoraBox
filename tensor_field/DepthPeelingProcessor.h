#ifndef TENSOR_FIELD_DEPTHPEELINGPROCESSOR_H_
#define TENSOR_FIELD_DEPTHPEELINGPROCESSOR_H_

#include <boost/smart_ptr/scoped_ptr.hpp>
#include <map>
#include <string>

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

    void create2DTextureIfNotExists(std::string texture_name, pbge::GraphicAPI * gfx, std::map<std::string, pbge::Texture2D*> & renderables);
};

#endif