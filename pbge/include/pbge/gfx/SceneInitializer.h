
#ifndef PBGE_GFX_SCENEINITIALIZER
#define PBGE_GFX_SCENEINITIALIZER

#include "pbge/core/core.h"

namespace pbge {
    class SceneGraph;
    class GraphicAPI;

    /** A class that allows users to initialize the scene graph within a valid graphic context.

        This class allows users to create api objects that requires a valid graphic context
        like texture objects, buffers, framebuffers and renderbuffers.

        @author Victor Kendy Harada
    */
    class PBGE_EXPORT SceneInitializer {
    public:
        /** Method that is called only when the graphic context is guaranteed to be initialized.
    
            @return A valid SceneGraph instance
        */
        virtual SceneGraph * operator () (GraphicAPI * ogl, Window * window) = 0;

    };
}


#endif
