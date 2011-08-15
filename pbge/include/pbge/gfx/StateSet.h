


#ifndef gfxstateset_h_
#define gfxstateset_h_

#include <set>
#include <map>

#include "pbge/gfx/ShaderUniform.h"
#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/State.h"
#include "pbge/gfx/states/BoundProgram.h"
#include "pbge/gfx/states/TextureUnits.h"
#include "pbge/core/core.h"

#include <vector>

namespace pbge {
    class Texture;
    class FramebufferObject;
    class BoundFBO;
    class UniformStack;

    class PBGE_EXPORT StateSet {
    public:
        StateSet(GraphicAPI * ogl);

        ~StateSet();

        void apply(GraphicAPI * ogl);

        void useProgram(GPUProgram * program);

        void useFBO(FramebufferObject * fbo);

        GPUProgram * currentProgram();

        void enable(GraphicAPI::Mode mode);

        void disable(GraphicAPI::Mode mode);
        
        UniformValue * getUniformValue(const UniformInfo & info);

        UniformValue * searchUniform(const UniformInfo & info);

        TextureUnit * chooseTexUnit(Texture * texture);

        void pushUniformSet(UniformSet * uniforms);

        void popUniformSet();

    private:

        std::vector<State *> states;

        BoundProgram * boundProgram;

        BoundFBO * boundFBO;

        TextureUnits * textureUnits;

        UniformStack * uniformStack;

        std::set<State*> changes;
    };
}


#endif