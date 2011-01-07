

#ifndef PBGE_INTERNAL_OPENGLSTATES_H_
#define PBGE_INTERNAL_OPENGLSTATES_H_

#include "pbge/core/core.h"
#include "pbge/gfx/StateSet.h"

namespace pbge {
    class PBGE_EXPORT StateEnabler : public State {
    public:
        StateEnabler(GLenum _mode);

        void applyChanges(OpenGL * ogl);

        void enable();

        void disable();
    private:
        bool current, next;
        GLenum mode;
    };

    class PBGE_EXPORT TextureUnit : public State{
        TextureUnit(OpenGL * ogl, unsigned index);

        void applyChanges(OpenGL * ogl);
    private:
        bool active;
        Texture * nextTexture;
        Texture * boundTexture;
        unsigned index;
    };

}

#endif //PBGE_INTERNAL_OPENGLSTATES_H_