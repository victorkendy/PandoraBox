


#ifndef gfxstateset_h_
#define gfxstateset_h_

#include "pbge/gfx/OpenGL.h"
#include "pbge/core/core.h"

#include <vector>

namespace pbge {
    class Texture;
    class TextureUnit;

    class PBGE_EXPORT State {
    public: 
        virtual void applyChanges(OpenGL * ogl) = 0;
    };



    class PBGE_EXPORT StateSet {
    public:
        StateSet(OpenGL * ogl);

        ~StateSet();

        void apply(OpenGL * ogl);

        void setTextureAt(unsigned texUnit, Texture * texture);

        void enable(OpenGL::Mode mode);

        void disable(OpenGL::Mode mode);

    private:
        std::vector<State *> states;
        std::vector<TextureUnit*> textureUnits;
    };
}


#endif