

#ifndef gfxopengl_h_
#define gfxopengl_h_ 1
#include <iostream>

#include "pbge/core/core.h"
#include "pbge/gfx/OpenGLAPI.h"

namespace pbge {
    
    class Buffer;

    class PBGE_EXPORT OpenGL{
    public:
        OpenGL() {
            GLint initialMatrixMode;
            api->getIntegerv(GL_MATRIX_MODE, &initialMatrixMode);
            currentMatrixMode = initialMatrixMode;
            api = new OpenGLAPI();
            matrices[2] = math3d::identity44;
        }

        virtual void setMatrixMode(GLenum mode) {
            if(currentMatrixMode != mode)
                api->matrixMode(mode);
            currentMatrixMode = mode;
        }

        virtual void loadViewMatrix(const math3d::matrix44 & m) {
            matrices[0] = m;
        }

        virtual void loadProjectionMatrix(const math3d::matrix44 & m) {
            matrices[1] = m;
        }

        virtual void loadModelMatrix(const math3d::matrix44 & m) {
            matrices[2] = m;
        }

        virtual void uploadModelview() {
            api->matrixMode(GL_MODELVIEW);
            api->loadMatrix((matrices[0]*matrices[2]).transpose());
        }

        virtual void uploadProjection() {
            api->matrixMode(GL_PROJECTION);
            api->loadMatrix(matrices[1].transpose());
        }
        
        virtual void viewport(GLint x, GLint y, GLint w, GLint h) {
            api->viewport(x,y,w,h);
        }

        virtual Buffer * createBuffer(size_t _size, GLenum _usage, GLenum _target);

        OpenGLAPI * getApi() { return api; }
    private:
        GLenum currentMatrixMode;

        math3d::matrix44 matrices[3];

        OpenGLAPI * api;
    };

}

#endif
