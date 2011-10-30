

#ifndef PBGE_INTERNAL_OPENGLSTATES_H_
#define PBGE_INTERNAL_OPENGLSTATES_H_

#include "pbge/core/core.h"
#include "pbge/gfx/StateSet.h"

namespace pbge {
    class GraphicAPI;

    class BoundFBO : public State {
    public:
        BoundFBO() {
            current = next = NULL;
        }
        bool shouldChange(GraphicAPI * api);
        void makeChange(GraphicAPI * api);
        void changeFBO(FramebufferObject * fbo) {
            next = fbo;
        }
    private:
        FramebufferObject * current;
        FramebufferObject * next;
    };
}

#endif //PBGE_INTERNAL_OPENGLSTATES_H_
