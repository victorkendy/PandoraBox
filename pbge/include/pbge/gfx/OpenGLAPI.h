
#include <GL/glew.h>
#include <iostream>

#include "pbge/core/Object.h"
#include "math3d/math3d.h"

/* Wrapper for the OpenGL API Calls */
/* The API calls that are part of the unextended OpenGL 2.1 will not be checked */

#if defined(DEBUG) || defined(_DEBUG)
#define DEBUG_DEPRECATION_WARN(entry) std::cout << "INFO: Deprecated entry point: " << #entry << std::endl
#else
#define DEBUG_DEPRECATION_WARN(entry)
#endif

namespace {
    inline bool checkPointer(void * p) {
        if(p == NULL) {
            // Error message?
            return false;
        }
        return true;
    }
            
}

namespace pbge {

    class OpenGLAPI {
    public:
        /* Standard API calls */
        inline void matrixMode(GLenum mode) {
            DEBUG_DEPRECATION_WARN(glMatrixMode);
            glMatrixMode(mode);
        }

        inline void loadMatrix(GLfloat * matrix) {
            DEBUG_DEPRECATION_WARN(glLoadMatrixf);
            glLoadMatrixf(matrix);
        }

        inline void viewport(GLint x, GLint y, GLsizei w, GLsizei h) {
            glViewport(x, y, w, h);
        }
        
        inline void getIntegerv(GLenum pname, GLint * params) {
            glGetIntegerv(pname, params);
        }

        inline void getFloatv(GLenum pname, GLfloat * params) {
            glGetFloatv(pname, params);
        }

        inline const char * getString(GLenum name) {
            return (const char *)glGetString(name);
        }

        /* Extended OpenGL calls */
        inline void genFramebuffersEXT(GLsizei n, GLuint * buffers) {
            if(checkPointer(glGenFramebuffersEXT))
                glGenFramebuffersEXT(n, buffers);
        }

        inline void deleteFramebuffersEXT(GLsizei n, GLuint * buffers) {
            if(checkPointer(glDeleteFramebuffersEXT)) 
                glDeleteFramebuffersEXT(n, buffers);
        }

    };

}