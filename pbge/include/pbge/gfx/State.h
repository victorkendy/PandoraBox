

#ifndef PBGE_GFX_STATE_H
#define PBGE_GFX_STATE_H
#include "pbge/core/core.h"

namespace pbge {
    class PBGE_EXPORT State {
    public: 
        void applyChanges(GraphicAPI * ogl) {
            if(shouldChange(ogl)) {
                makeChange(ogl);
            }
        }

        virtual void makeChange(GraphicAPI * ogl) = 0;

        virtual bool shouldChange(GraphicAPI * ogl) = 0;
    };
}

#endif