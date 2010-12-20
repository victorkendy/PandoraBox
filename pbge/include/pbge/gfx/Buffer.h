

#ifndef gfxbuffer_h_
#define gfxbuffer_h_

#include <cstdlib>
#include <iostream>

#include "pbge/core/Manager.h"
#include "pbge/gfx/OpenGL.h"

namespace pbge {
    class Buffer {
    public:
        Buffer(size_t _size, GLenum _usage, GLenum _target) {
            this->size = _size;
            this->usage = _usage;
            this->target = _target;
            // malloc is used because raw data is needed
            this->data = malloc(size);
            if(this->data == NULL) {
                Manager::getInstance()->writeErrorLog("ERROR: Out Of Memory for Buffer");
            }
            this->glID = 0;
        }

        ~Buffer() {
            if(glID) 
                Manager::getInstance()->getOpenGL()->getApi()->deleteBuffers(1, &glID);
            if(this->data)
                free(this->data);
        }
        
        // returns the data buffer
        void * map() {
            return data;
        }

        // Maps with an offset from the begining of the data buffer
        void * map(int _begin) {
            return (char*)data + _begin;
        }
        
        // force call to glMapBuffer
        void * forceDirectMap(GLenum access) {
            std::cout << "my id is " << glID << std::endl;
            Manager::getInstance()->getOpenGL()->getApi()->bindBuffer(target, glID);
            return Manager::getInstance()->getOpenGL()->getApi()->mapBuffer(target, access);
        }

        void setData(void * _data, size_t _size);

        void setSubData(void * _data, size_t _size, int _begin, int _end);
        
        // call this to update the OpenGL state
        void flush() {
            if(glID == 0) createBuffer();
            if(begin_update >=0) {
                OpenGL * ogl = Manager::getInstance()->getOpenGL();
                ogl->getApi()->bindBuffer(target, glID);
                ogl->getApi()->bufferSubData(target, begin_update, size - begin_update, (char*)data + begin_update);
                ogl->getApi()->bindBuffer(target, 0);
            }
        }
    private:
        void createBuffer() {
            OpenGL * ogl = Manager::getInstance()->getOpenGL();
            ogl->getApi()->genBuffers(1, &glID);
            ogl->getApi()->bindBuffer(target, glID);
            ogl->getApi()->bufferData(target, size, NULL, usage);
            ogl->getApi()->bindBuffer(target, 0);
        }

        int begin_update;

        GLenum usage;

        GLenum target;

        void * data;

        GLuint glID;

        size_t size;
    };

}

#endif