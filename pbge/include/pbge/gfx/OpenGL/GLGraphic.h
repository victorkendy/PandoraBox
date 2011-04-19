
#ifndef PBGE_GFX_OPENGL_GLGRAPHIC_H
#define PBGE_GFX_OPENGL_GLGRAPHIC_H

#include "pbge/gfx/GraphicAPI.h"

namespace pbge {

    class GLGraphic : public GraphicAPI {
    public:
        GLGraphic();

        ~GLGraphic();
        
        void setContext(GLContext * newContext);

        GLContext * getContext();

        void releaseContext();

        void makeContextCurrent();

        void swapBuffers();

        void setMatrixMode(GLenum mode);

        void loadViewMatrix(const math3d::matrix44 & m);

        void loadProjectionMatrix(const math3d::matrix44 & m);

        void loadModelMatrix(const math3d::matrix44 & m);

        void updateState();

        void uploadProjection();

        void uploadModelView();

        GraphicObjectsFactory * getFactory();

        UniformValue * getUniformValue(const UniformInfo & info);

        UniformValue * searchUniform(const UniformInfo & info);

        void enableMode(Mode mode);

        void disableDrawBuffer();

        void enableDrawBuffer(GLenum buffer);

        StateSet * getState();

        ResourceStorage * getStorage();

        TextureUnit * chooseTextureUnit(Texture * texture);
        
        const int numberOfTextureUnits();

        void pushUniforms(UniformSet * uniforms);

        void popUniforms();
 
    private:
        void uploadModelview();

        math3d::matrix44 * matrices;

        GLenum currentMatrixMode;

        StateSet * state;

        ResourceStorage * storage;

        GLContext * context;

        GraphicObjectsFactory * factory;
    };

}

#endif