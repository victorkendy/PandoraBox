#ifndef PBGE_GFX_PROCESSORS_RENDERPASSPROCESSOR_H
#define PBGE_GFX_PROCESSORS_RENDERPASSPROCESSOR_H

#include "boost/smart_ptr/scoped_ptr.hpp"

#include "pbge/gfx/NodeVisitors.h"

namespace pbge {
    class RenderPassProcessor : public SceneProcessor {
    public:
        RenderPassProcessor() : visitor(new ColorPassVisitor){
        }

        void initialize(GraphicAPI * gfx, Renderer * renderer) {}
        void process(GraphicAPI * gfx, Renderer * renderer);
        bool isInitialized(GraphicAPI * gfx) {
            return true;
        }
        bool isActive() {
            return true;
        }
    private:
        boost::scoped_ptr<ColorPassVisitor> visitor;
    };
}

#endif