
#ifndef PBGE_GFX_PROCESSORS_BLITTOFRAMEBUFFER_H
#define PBGE_GFX_PROCESSORS_BLITTOFRAMEBUFFER_H

#include <boost/smart_ptr/scoped_ptr.hpp>
#include "pbge/core/core.h"
#include "pbge/gfx/GPUProgram.h"
#include "pbge/gfx/ScenePostProcessor.h"

namespace pbge {
    class PBGE_EXPORT BlitToFramebuffer : public ScenePostProcessor {
    public:
        /** see SceneProcessor::initialize */
        void initialize(GraphicAPI * gfx, Renderer * renderer);

        /** Render a quad with the texture on the sampler named color. */
        void process(GraphicAPI * gfx, Renderer * renderer);

        /** see SceneProcessor::initialize */
        bool isInitialized(GraphicAPI * gfx) {
            // It's initialized if it has a valid blitter
            return blitter.get() != NULL;
        }

        /** see SceneProcessor::initialize */
        bool isActive() {
            return active;
        }

        /** activate or deactivate the processor. */
        void toggle() {
            active = !active;
        }
    private:
        

        boost::scoped_ptr<GPUProgram> blitter;

        bool active;
    };
}

#endif