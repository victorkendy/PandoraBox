

#ifndef PBGE_GFX_STATE_H
#define PBGE_GFX_STATE_H
#include "pbge/core/core.h"

namespace pbge {
    class GraphicAPI;
    
    /** A lazy state object.
    */
    class PBGE_EXPORT State {
    public: 
        void applyChanges(GraphicAPI * gfx) {
            if(shouldChange(gfx)) {
                makeChange(gfx);
            }
        }

        virtual void makeChange(GraphicAPI * gfx) = 0;

        virtual bool shouldChange(GraphicAPI * gfx) = 0;
    };
}

#endif