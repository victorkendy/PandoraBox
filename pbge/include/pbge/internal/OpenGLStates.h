

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

    class PBGE_EXPORT BoundProgram : public State {
    public:
        BoundProgram() {
            this->current = NULL;
            this->next = NULL;
        }

        void makeChange(GraphicAPI * ogl);

        bool shouldChange(GraphicAPI * ogl);

        void changeProgram(GPUProgram * program) {
            next = program;
        }

        void updateUniforms(GraphicAPI * ogl);

    private:
        GPUProgram * current;
        GPUProgram * next;
    };

    class PBGE_EXPORT TextureUnit : public State{
    public:
        TextureUnit(GraphicAPI * ogl, unsigned index);

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
        bool active;
        Texture * nextTexture;
        Texture * boundTexture;
        unsigned index;
    };

}

#endif //PBGE_INTERNAL_OPENGLSTATES_H_
