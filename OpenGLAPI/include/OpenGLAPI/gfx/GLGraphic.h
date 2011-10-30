
#ifndef PBGE_GFX_OPENGL_GLGRAPHIC_H
#define PBGE_GFX_OPENGL_GLGRAPHIC_H

#include <boost/smart_ptr/scoped_array.hpp>
#include <boost/smart_ptr/scoped_ptr.hpp>

#include <GL/glew.h>

#include "pbge/core/core.h"
#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/ShaderUniform.h"

#include "OpenGLAPI/gfx/GLExtensions.h"
#include "OpenGLAPI/gfx/GLDepthBufferController.h"
#include "OpenGLAPI/gfx/GLBlendController.h"

namespace pbge {
    class GLDrawController;
    class GLObjectsFactory;

    class GLGraphic : public GraphicAPI {
    public:
        GLGraphic();

        void setContext(GraphicContext * newContext);

        GraphicContext * getContext();

        void releaseContext();

        void makeContextCurrent();

        void swapBuffers();

        void setMatrixMode(GLenum mode);

        void loadViewMatrix(const math3d::matrix44 & m);

        void loadProjectionMatrix(const math3d::matrix44 & m);

        void loadModelMatrix(const math3d::matrix44 & m);

        BuiltInUniformMatrix * getModelMatrix() {
            return &modelMatrix;
        }

        BuiltInUniformMatrix * getProjectionMatrix() {
            return &projectionMatrix;
        }

        BuiltInUniformMatrix * getViewMatrix() {
            return &viewMatrix;
        }

        void updateState();

        GraphicObjectsFactory * getFactory();

        UniformValue * getUniformValue(const UniformInfo & info);

        UniformValue * searchUniform(const UniformInfo & info);

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

        void bindFramebufferObject(FramebufferObject * fbo);

        GPUProgram * getCurrentProgram();

        DepthBufferController * depthBufferController();

        BlendController * getBlendController();
    public:
        /** Calculates the major version of the OpenGL implementation
            
            @return The major version of the GL.
        */
        const unsigned getMajorVersion() {
            return this->majorVersion;
        }

        GLExtensions & getExtensions() {
            return *extensions;
        }
 
    private:
        void initContextVersion();

        boost::scoped_ptr<StateSet> state;

        boost::scoped_ptr<ResourceStorage> storage;

        boost::scoped_ptr<GraphicContext> context;

        boost::scoped_ptr<GLObjectsFactory> factory;

        boost::scoped_ptr<GLDrawController> drawController;

        boost::scoped_ptr<GLDepthBufferController> depthController;

        boost::scoped_ptr<GLBlendController> blendController;

        BuiltInUniformMatrix viewMatrix;
        BuiltInUniformMatrix modelMatrix;
        BuiltInUniformMatrix projectionMatrix;

        FramebufferObject * currentFBO;

        unsigned majorVersion;

        boost::scoped_ptr<GLExtensions> extensions;
    };

}

#endif