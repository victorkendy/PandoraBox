#ifndef OPENGLAPI_GFX_BUILTINUNIFORMBINDERS_H
#define OPENGLAPI_GFX_BUILTINUNIFORMBINDERS_H
#include <string>
#include <GL/glew.h>
#include "math3d/math3d.h"

#include "pbge/gfx/GraphicAPI.h"


namespace pbge {
    class BuiltInUniformBinder {
    public:
        virtual void bind(GraphicAPI * gfx) = 0;
    };

    class DeprModelViewBinder : public BuiltInUniformBinder {
    public:
        void bind(GraphicAPI * gfx);
    };
    class DeprProjectionBinder : public BuiltInUniformBinder {
    public:
        void bind(GraphicAPI * gfx);
    };
    class DeprModelViewProjectionBinder : public BuiltInUniformBinder {
    public:
        void bind(GraphicAPI * gfx);
    };

    
    template<typename MatrixGetter>
    class MatrixBinder : public BuiltInUniformBinder {
    public:
        MatrixBinder(GLint _location) : location(_location) {}

        void bind(GraphicAPI * gfx) {
            if(getter.shouldChange(gfx)) {
                math3d::matrix44 m = getter(gfx);
                glUniformMatrix4fv(location, 1, GL_TRUE, m);
            }
        }
    private:
        MatrixGetter getter;
        
        GLint location;
    };


    class BuiltInUniformBinders {
    public:
        static BuiltInUniformBinder * binderFor(const std::string & name, GLint location);
    };
}

#endif
