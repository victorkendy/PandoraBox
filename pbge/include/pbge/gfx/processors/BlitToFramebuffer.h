
#ifndef PBGE_GFX_PROCESSORS_BLITTOFRAMEBUFFER_H
#define PBGE_GFX_PROCESSORS_BLITTOFRAMEBUFFER_H

#include <boost/smart_ptr/scoped_ptr.hpp>
#include "pbge/core/core.h"
#include "pbge/gfx/GPUProgram.h"
#include "pbge/gfx/Model.h"
#include "pbge/gfx/ScenePostProcessor.h"

namespace pbge {
    class PBGE_EXPORT BlitToFramebuffer : public ScenePostProcessor {
    public:
        void initialize(GraphicAPI * gfx, Renderer * renderer);

        void process(GraphicAPI * gfx, Renderer * renderer);

        bool isInitialized(GraphicAPI * gfx) {
            // It's initialized if it has a valid blitter and a valid quad
            return quad.get() != NULL && blitter.get() != NULL;
        }

        bool isActive() {
            return active;
        }

        void toggle() {
            active = !active;
        }
    private:
        boost::scoped_ptr<VBOModel> quad;

        boost::scoped_ptr<GPUProgram> blitter;

        bool active;
    };
}

#endif