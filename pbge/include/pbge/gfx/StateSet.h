


#ifndef gfxstateset_h_
#define gfxstateset_h_

#include <set>
#include <map>

#include "pbge/gfx/ShaderUniform.h"
#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/State.h"
#include "pbge/gfx/states/BoundProgram.h"
#include "pbge/gfx/states/TextureUnits.h"
#include "pbge/gfx/states/CurrentVertexBuffer.h"
#include "pbge/core/core.h"

#include <vector>

namespace pbge {
    class Texture;
    class FramebufferObject;
    class BoundFBO;
    class UniformStack;
    class VertexBuffer;

    /** The state set is responsible for the lazy state change used by the engine.

        All the state changing methods on this class will just be accumulated to be run
        in batch just before rendering.
    */
    class PBGE_EXPORT StateSet {
    public:
        StateSet(GraphicAPI * ogl);

        ~StateSet();

        /** Apply all the accumulate changes.
        */
        void apply(GraphicAPI * ogl);

        /** Lazy program change.
        */
        void useProgram(GPUProgram * program);

        /** Lazy framebuffer object change. 
        */
        void useFBO(FramebufferObject * fbo);

        /** Lazy change of the current VertexBuffer.
        */
        void useVertexBuffer(VertexBuffer * buffer);

        /** returns the current bound program object.
        */
        GPUProgram * currentProgram();

        /** Returns a valid instance os the uniform value that matches info.
        */
        UniformValue * getUniformValue(const UniformInfo & info);

        /** Returns a valid instance os the uniform value that matches info or null if no value matches.
        */
        UniformValue * searchUniform(const UniformInfo & info);

        /** Chooses a available texture unit for texture.
        */
        TextureUnit * chooseTexUnit(Texture * texture);

        /** Pushes a uniform set on the uniform set stack.
        */
        void pushUniformSet(UniformSet * uniforms);

        /** Removes the top of the uniform set stack.
        */
        void popUniformSet();

    private:
        BoundProgram * boundProgram;

        BoundFBO * boundFBO;

        TextureUnits * textureUnits;

        UniformStack * uniformStack;

        std::set<State*> changes;

        CurrentVertexBuffer * vertexBuffer;
    };
}


#endif