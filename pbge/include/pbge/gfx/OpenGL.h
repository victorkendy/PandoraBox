

#ifndef gfxopengl_h_
#define gfxopengl_h_ 1
#include <iostream>
#include <GL/glew.h>

#include "math3d/math3d.h"

#include "pbge/core/core.h"


namespace pbge {
    
    class Buffer;

    class PBGE_EXPORT OpenGL{
    public:
        OpenGL();

        virtual void setMatrixMode(GLenum mode);

        virtual void loadViewMatrix(const math3d::matrix44 & m) {
            matrices[0] = m;
        }

        virtual void loadProjectionMatrix(const math3d::matrix44 & m) {
            matrices[1] = m;
        }

        virtual void loadModelMatrix(const math3d::matrix44 & m) {
            matrices[2] = m;
        }

        virtual void uploadModelview();

        virtual void uploadProjection();

        virtual Buffer * createBuffer(size_t _size, GLenum _usage, GLenum _target);

        // raw OpenGL calls
        virtual void activeTexture(GLenum textureUnit);

        virtual void alphaFunc(GLenum func, GLclampf ref);

        virtual void attachShader(GLuint program, GLuint shader);

        virtual void begin(GLenum mode);

        virtual void beginQuery(GLenum target, GLuint id);

        virtual void bindAttribLocation(GLuint program, GLuint index, const GLchar* name);

        virtual void bindBuffer(GLenum target, GLuint buffer);

        virtual void bindTexture(GLenum target, GLuint texture);

        virtual void bufferData(GLenum target, GLsizeiptr size, GLvoid * data, GLenum usage);

        virtual void bufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid * data);
        
        virtual void clear(GLbitfield mask);

        virtual void clearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);

        virtual void clearDepth(GLclampd depth);

        virtual void clearStencil(GLint s);

        virtual void clientActiveTexture(GLenum textureUnit);

        virtual void clipPlane(GLenum plane, const GLdouble * equation);

        virtual void colorPointer(GLint size, GLenum type, GLsizei stride, GLvoid * pointer);

        virtual void deleteBuffers(GLsizei n, GLuint * buffers);

        virtual void enableClientState(GLenum cap);

        virtual void genBuffers(GLsizei n, GLuint * buffers);

        virtual void getFloatv(GLenum pname, GLfloat * params);

        virtual void getIntegerv(GLenum pname, GLint * params);

        virtual const char * getString(GLenum name);

        virtual void loadMatrix(GLfloat * matrix);

        virtual void * mapBuffer(GLenum target, GLenum access);

        virtual void matrixMode(GLenum mode);

        virtual void normalPointer(GLenum type, GLsizei stride, GLvoid * pointer);

        virtual void secondaryColorPointer(GLint size, GLenum type, GLsizei stride, GLvoid * pointer);

        virtual void texCoordPointer(GLint size, GLenum type, GLsizei stride, GLvoid * pointer);

        virtual void vertexPointer(GLint size, GLenum type, GLsizei stride, GLvoid * pointer);

        virtual void viewport(GLint x, GLint y, GLint w, GLint h);
        
        //Extensions to 2.1
        virtual void genFramebuffersEXT(GLsizei n, GLuint * buffers);

        virtual void deleteFramebuffersEXT(GLsizei n, GLuint * buffers);

    private:
        GLenum currentMatrixMode;

        math3d::matrix44 matrices[3];

    };

}

#endif
