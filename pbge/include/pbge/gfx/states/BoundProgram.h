#ifndef PBGE_GFX_STATES_BOUNDPROGRAM_H
#define PBGE_GFX_STATES_BOUNDPROGRAM_H

#include "pbge/core/core.h"
#include "pbge/gfx/State.h"

namespace pbge {

    class GraphicAPI;
    class GPUProgram;

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

        GPUProgram * getCurrent() {
            return current;
        }
    private:
        GPUProgram * current;
        GPUProgram * next;
    };

}

#endif