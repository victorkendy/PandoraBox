


#ifndef gfxstateset_h_
#define gfxstateset_h_

#include <set>
#include <map>

#include "pbge/gfx/ShaderUniform.h"
#include "pbge/gfx/GraphicAPI.h"
#include "pbge/core/core.h"

#include <vector>

namespace pbge {
    class Texture;
    class TextureUnit;
    class BoundProgram;
    class GPUProgram;
    class UniformStack;

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


    /*
        
    */
    class PBGE_EXPORT StateSet {
    public:
        StateSet(GraphicAPI * ogl);

        ~StateSet();

        void apply(GraphicAPI * ogl);

        void useProgram(GPUProgram * program);

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

        UniformStack * uniformStack;

        std::vector<TextureUnit*> textureUnits;

        std::set<State*> changes;
    };
}


#endif