

#ifndef PBGE_GFX_GRAPHICAPI_H_
#define PBGE_GFX_GRAPHICAPI_H_

#include <iostream>
#include <GL/glew.h>

#include "math3d/math3d.h"

#include "pbge/core/core.h"
#include "pbge/gfx/Buffer.h"

namespace pbge {
    class ResourceStorage;
    class StateSet;
    class Buffer;
    class UniformInfo;
    class TextureUnit;
    class Texture;
    class UniformValue;
    class UniformSet;
    class GPUProgram;
    class GraphicObjectsFactory;
    class VertexBuffer;
    class DrawController;
    class FramebufferObject;
    class DepthBufferController;

    class GraphicContext {
    public:
        virtual void makeCurrent() = 0;

        virtual void swapBuffers() = 0;

        virtual void release() = 0;

        virtual void getSystemGraphicContext(void * p_context) = 0;

        virtual void getSystemDeviceContext(void * p_device) = 0;
    };

    class PBGE_EXPORT GraphicAPI {
    public:

        typedef enum {
            DEPTH_TEST = 0,
            STENCIL_TEST,
            SCISSOR_TEST,
            BLEND,
            COLOR_LOGIC_OP,
            CULL_FACE,
            DEPTH_CLAMP,
            DITHER,
            LINE_SMOOTH,
            MULTISAMPLE,
            POLYGON_OFFSET_FILL,
            POLYGON_OFFSET_LINE,
            POLYGON_OFFSET_POINT,
            POLYGON_SMOOTH,
            PRIMITIVE_RESTART,
            SAMPLE_ALPHA_TO_COVERAGE,
            PROGRAM_POINT_SIZE
        } Mode;

    public:
        static GraphicAPI * createInstance();

        virtual ~GraphicAPI(){};

        virtual void setContext(GraphicContext * newContext) = 0;

        virtual GraphicContext * getContext() = 0;

        virtual void releaseContext() = 0;

        virtual void makeContextCurrent() = 0;

        virtual void swapBuffers() = 0;

        virtual void loadViewMatrix(const math3d::matrix44 & m) = 0;

        virtual void loadProjectionMatrix(const math3d::matrix44 & m) = 0;

        virtual void loadModelMatrix(const math3d::matrix44 & m) = 0;

        virtual const math3d::matrix44 getModelMatrix() = 0;

        virtual const math3d::matrix44 getViewMatrix() = 0;

        virtual const math3d::matrix44 getProjectionMatrix() = 0;

        virtual void updateState() = 0;

        virtual GraphicObjectsFactory * getFactory() = 0;

        virtual UniformValue * getUniformValue(const UniformInfo & info) = 0;

        virtual UniformValue * searchUniform(const UniformInfo & info) = 0;

        virtual void enableMode(Mode mode) = 0;

        virtual void disableDrawBuffer() = 0;

        virtual void enableDrawBuffer(GLenum buffer) = 0;

        virtual StateSet * getState() = 0;

        virtual ResourceStorage * getStorage() = 0;

        virtual TextureUnit * chooseTextureUnit(Texture * texture) = 0;
        
        virtual const int numberOfTextureUnits() = 0;

        virtual void pushUniforms(UniformSet * uniforms) = 0;

        virtual void popUniforms() = 0;

        virtual DrawController * getDrawController() = 0;

        virtual void setViewport(int x, int y, int w, int h) = 0;

        virtual void bindFramebufferObject(FramebufferObject * fbo) = 0;

        virtual GPUProgram * getCurrentProgram() = 0;

        virtual DepthBufferController * depthBufferController() = 0;
        
        // raw OpenGL API calls
        // TODO: remove all this....
        virtual void alphaFunc(GLenum func, GLclampf ref);

        virtual void blendFunc(GLenum sfactor, GLenum dfactor);

        virtual void clear(GLbitfield mask);

        virtual void clearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);

        virtual void disable(GLenum mode);

        virtual void enable(GLenum mode);
        
    };
}

#endif
