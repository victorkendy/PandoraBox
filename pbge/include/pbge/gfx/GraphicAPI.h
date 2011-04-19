

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
    class GraphicObjectsFactory;

    class GLContext {
    public:
        virtual void makeCurrent() = 0;

        virtual void swapBuffers() = 0;

        virtual void release() = 0;

        virtual void getSystemGLContext(void * p_context) = 0;

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

        virtual void setContext(GLContext * newContext) = 0;

        virtual GLContext * getContext() = 0;

        virtual void releaseContext() = 0;

        virtual void makeContextCurrent() = 0;

        virtual void swapBuffers() = 0;

        virtual void setMatrixMode(GLenum mode) = 0;

        virtual void loadViewMatrix(const math3d::matrix44 & m) = 0;

        virtual void loadProjectionMatrix(const math3d::matrix44 & m) = 0;

        virtual void loadModelMatrix(const math3d::matrix44 & m) = 0;

        virtual void updateState() = 0;

        virtual void uploadProjection() = 0;

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
        
        
        // raw OpenGL API calls
        // TODO: remove all this....
        virtual void alphaFunc(GLenum func, GLclampf ref);

        virtual void attachShader(GLuint program, GLuint shader);

        virtual void begin(GLenum mode);

        virtual void bindAttribLocation(GLuint program, GLuint index, const GLchar* name);

        virtual void blendFunc(GLenum sfactor, GLenum dfactor);

        virtual void clear(GLbitfield mask);

        virtual void clearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);

        virtual void clearDepth(GLclampd depth);

        virtual void clientActiveTexture(GLenum textureUnit);

        virtual void colorPointer(GLint size, GLenum type, GLsizei stride, GLvoid * pointer);

        virtual void compileShader(GLuint shader);

        virtual GLuint createProgram();

        virtual GLuint createShader(GLenum shaderType);

        virtual void depthFunc(GLenum func);

        virtual void depthMask(GLboolean flag);

        virtual void disable(GLenum mode);

        virtual void drawBuffer(GLenum mode);

        virtual void enable(GLenum mode);

        virtual void enableClientState(GLenum cap);

        virtual void getActiveUniform(GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name);

        virtual void getFloatv(GLenum pname, GLfloat * params);

        virtual void getIntegerv(GLenum pname, GLint * params);

        virtual void getShaderiv(GLuint shader, GLenum pname, GLint * ptr);

        virtual void getShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei * length, GLchar * infoLog);

        virtual const char * getString(GLenum name);

        virtual GLint getUniformLocation(GLuint program, const char * name);

        virtual void linkProgram(GLuint program);

        virtual void loadMatrix(GLfloat * matrix);

        virtual void matrixMode(GLenum mode);

        virtual void normalPointer(GLenum type, GLsizei stride, GLvoid * pointer);

        virtual void readBuffer(GLenum mode);

        virtual void secondaryColorPointer(GLint size, GLenum type, GLsizei stride, GLvoid * pointer);

        virtual void shaderSource(GLuint id, GLsizei count, const GLchar ** strings, const GLint * sizes);

        virtual void texCoordPointer(GLint size, GLenum type, GLsizei stride, GLvoid * pointer);

        virtual void useProgram(GLuint program);

        virtual void vertexPointer(GLint size, GLenum type, GLsizei stride, GLvoid * pointer);

        virtual void viewport(GLint x, GLint y, GLint w, GLint h);
    };
}

#endif
