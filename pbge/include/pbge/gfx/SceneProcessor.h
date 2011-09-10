#ifndef PBGE_GFX_SCENEPROCESSOR_H
#define PBGE_GFX_SCENEPROCESSOR_H

namespace pbge {
    class GraphicAPI;
    class Renderer;

    class SceneProcessor {
    public:
        /** The object initialization method.

            This method should be used to initialize any context dependent state of the processor and
            will be called when the renderer begins rendering for the first time if the isInitialized 
            method returns false.
        */
        virtual void initialize(GraphicAPI * gfx, Renderer * renderer) = 0;

        /** The scene processing algorithm.

            This method will be called once for every frame if the isActive method returns true after the 
            renderer executes the update traversal on the scene graph.
            
        */
        virtual void process(GraphicAPI * gfx, Renderer * renderer) = 0;

        /** The initialization check method 

            @return true if the object is ready to execute, false otherwise
        */
        virtual bool isInitialized(GraphicAPI * gfx) = 0;

        /** Checks if the process method should be executed

            @return true if the method should be executed, false otherwise
        */
        virtual bool isActive() { 
            return true; 
        }
    };
}

#endif