#ifndef PBGE_GFX_PROCESSORS_RENDERPASSPROCESSOR_H
#define PBGE_GFX_PROCESSORS_RENDERPASSPROCESSOR_H

#include <boost/smart_ptr/scoped_ptr.hpp>

#include "pbge/core/core.h"
#include "pbge/gfx/NodeVisitors.h"

namespace pbge {
    class PBGE_EXPORT RenderPassProcessor : public SceneProcessor {
    public:
        void initialize(GraphicAPI * gfx, Renderer * renderer) {}
        void process(GraphicAPI * gfx, Renderer * renderer);
        bool isInitialized(GraphicAPI * gfx) {
            return true;
        }
        bool isActive() {
            return true;
        }
    private:
        ColorPassVisitor visitor;
    };
}

#endif