

#ifndef PBGE_INTERNAL_OPENGLSTATES_H_
#define PBGE_INTERNAL_OPENGLSTATES_H_

#include "pbge/core/core.h"
#include "pbge/gfx/StateSet.h"

namespace pbge {
    class GraphicAPI;

    class PBGE_EXPORT StateEnabler : public State {
    public:
        StateEnabler(GLenum _mode);

        void makeChange(GraphicAPI * ogl);

        bool shouldChange(GraphicAPI * ogl);

        void enable();

        void disable();
    private:
        bool current, next;
        GLenum mode;
    };

    

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
