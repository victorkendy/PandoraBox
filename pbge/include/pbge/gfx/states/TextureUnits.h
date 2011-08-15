#ifndef PBGE_GFX_STATES_TEXTUREUNITS_H
#define PBGE_GFX_STATES_TEXTUREUNITS_H

#include <vector>

#include "pbge/core/core.h"
#include "pbge/gfx/State.h"


namespace pbge {
    class GraphicAPI;
    class Texture;

    class PBGE_EXPORT TextureUnit : public State{
    public:
        TextureUnit(unsigned index);
        void makeChange(GraphicAPI * ogl);
        bool shouldChange(GraphicAPI * ogl);
        void setTexture(Texture * tex) {
            this->nextTexture = tex;
        }
        Texture * getNextTexture() {
            return nextTexture;
        }
        Texture * getCurrentTexture() {
            return boundTexture;
        }
        const unsigned getIndex() {
            return index;
        }
    private:
        Texture * nextTexture;
        Texture * boundTexture;
        unsigned index;
    };

    class PBGE_EXPORT TextureUnits : public State {
    public: 
        TextureUnits(GraphicAPI * api);

        void makeChange(GraphicAPI * ogl);

        bool shouldChange(GraphicAPI * ogl);

        TextureUnit * unitFor(Texture * texture);
    private:
        std::vector<TextureUnit> units;
    };
}

#endif