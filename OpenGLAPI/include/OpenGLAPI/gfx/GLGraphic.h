
#ifndef PBGE_GFX_OPENGL_GLGRAPHIC_H
#define PBGE_GFX_OPENGL_GLGRAPHIC_H

#include <boost/smart_ptr/scoped_array.hpp>
#include <boost/smart_ptr/scoped_ptr.hpp>

#include "pbge/core/core.h"
#include "pbge/gfx/GraphicAPI.h"

namespace pbge {
    class GLDrawController;

    class GLGraphic : public GraphicAPI {
    public:
        GLGraphic();

        ~GLGraphic();
        
        void setContext(GraphicContext * newContext);

        GraphicContext * getContext();

        void releaseContext();

        void makeContextCurrent();

        void swapBuffers();

        void setMatrixMode(GLenum mode);

        void loadViewMatrix(const math3d::matrix44 & m);

        void loadProjectionMatrix(const math3d::matrix44 & m);

        void loadModelMatrix(const math3d::matrix44 & m);

        void updateState();

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

        DrawController * getDrawController();

        void setViewport(int x, int y, int w, int h);
    public:
        /** Calculates the major version of the OpenGL implementation
            
            @return The major version of the GL.
        */
        const unsigned getMajorVersion() {
            return this->majorVersion;
        }
 
    private:
        void initContextVersion();

        void createDefaultShaders();

        bool projectionUpdated;

        boost::scoped_array<math3d::matrix44> matrices;

        GLenum currentMatrixMode;

        boost::scoped_ptr<StateSet> state;

        boost::scoped_ptr<ResourceStorage> storage;

        boost::scoped_ptr<GraphicContext> context;

        boost::scoped_ptr<GraphicObjectsFactory> factory;

        boost::scoped_ptr<GLDrawController> drawController;

        unsigned majorVersion;
    };

}

#endif