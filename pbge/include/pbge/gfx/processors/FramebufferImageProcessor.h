#ifndef PBGE_GFX_FRAMEBUFFERIMAGEPROCESSOR_H
#define PBGE_GFX_FRAMEBUFFERIMAGEPROCESSOR_H

#include <string>
#include <boost/smart_ptr/scoped_ptr.hpp>
#include "pbge/core/core.h"
#include "pbge/gfx/ScenePostProcessor.h"
#include "pbge/gfx/GPUProgram.h"

namespace pbge {
    class PBGE_EXPORT FramebufferImageProcessor : public ScenePostProcessor {
    public:
        FramebufferImageProcessor(const std::string & fs);

        void initialize(GraphicAPI * gfx, Renderer * renderer);

        /** Do a whole framebuffer post processing.
        */
        void process(GraphicAPI * gfx, Renderer * renderer);

        bool isInitialized(GraphicAPI * gfx) {
            return program.get() != NULL;
        }
        bool isActive() {
            return active;
        }
        void toggle() {
            active = !active;
        }
    private:
        std::string fragShader;
        bool active;
        boost::scoped_ptr<GPUProgram> program;
    };
}

#endif