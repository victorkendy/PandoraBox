


#ifndef gfxstateset_h_
#define gfxstateset_h_

#include "pbge/gfx/OpenGL.h"
#include "pbge/core/core.h"

#include <vector>

namespace pbge {
    class Texture;

    class PBGE_EXPORT State {
    public: 
        virtual void applyChanges(OpenGL * ogl) = 0;
    };

    class PBGE_EXPORT StateEnabler : public State {
    public:
        StateEnabler(GLenum _mode);

        void applyChanges(OpenGL * ogl);

        void enable();
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

    class PBGE_EXPORT StateSet {
    public:
        StateSet(OpenGL * ogl);

        ~StateSet();

        void apply(OpenGL * ogl);

        void setTextureAt(unsigned texUnit, Texture * texture);

        void enable(OpenGL::Mode mode);

    private:
        std::vector<State *> states;
        std::vector<TextureUnit*> textureUnits;
    };
}


#endif