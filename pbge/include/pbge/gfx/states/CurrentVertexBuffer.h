#ifndef PBGE_GFX_STATES_CURRENTVERTEXBUFFER_H
#define PBGE_GFX_STATES_CURRENTVERTEXBUFFER_H

#include "pbge/core/core.h"
#include "pbge/gfx/State.h"

namespace pbge {
    class GraphicAPI;
    class VertexBuffer;

    class PBGE_EXPORT CurrentVertexBuffer : public State {
    public:
        CurrentVertexBuffer() : current(NULL), next(NULL) {}

        void makeChange(GraphicAPI * gfx);

        bool shouldChange(GraphicAPI * gfx);

        VertexBuffer * getCurrent();

    private:
        VertexBuffer * current;

        VertexBuffer * next;
    };
}


#endif