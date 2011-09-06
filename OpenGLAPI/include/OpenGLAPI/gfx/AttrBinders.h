
#ifndef PBGE_GFX_OPENGL_ATTRBINDERS_H
#define PBGE_GFX_OPENGL_ATTRBINDERS_H

#include <GL/glew.h>
#include <string>

#include "pbge/gfx/VBO.h"

namespace pbge {
    class GLProgram;

    class AttrBinder {
    public:
        virtual void bind(VertexBuffer * attrs) = 0;

        virtual void unbind() = 0;
    };

    class DeprVertexBinder : public AttrBinder {
    public:
        void bind(VertexBuffer * attrs);

        void unbind();
    };

    class DeprNormalBinder : public AttrBinder {
    public:
        void bind(VertexBuffer * attrs);

        void unbind();
    };

    class DeprColorBinder : public AttrBinder {
    public:
        void bind(VertexBuffer * attrs);

        void unbind();
    };

    class DeprSecondaryColorBinder : public AttrBinder {
    public:
        void bind(VertexBuffer * attrs);

        void unbind();
    };

    class CustomAttrBinder : public AttrBinder {
    public:
        CustomAttrBinder(std::string attrName, GLint attrLocation) : name(attrName), location(attrLocation) {}

        void bind(VertexBuffer * attrs);

        void unbind();
    private:
        std::string name;
        GLint location;
    };

    class SemanticAttribBinder : public AttrBinder {
    public:
        SemanticAttribBinder(const VertexAttrib::Type attrType, GLint attrLocation): type(attrType), location(attrLocation){}

        void bind(VertexBuffer * attrs);

        void unbind();
    private:
        VertexAttrib::Type type;
        GLint location;
    };

    class AttrBinders {
    public:
        static AttrBinder * binderFor(const std::string & name, GLint location);
    private:
        static AttrBinder * constructGLBuiltIn(const std::string & name);
        static AttrBinder * constructCustomAttrib(const std::string & name, GLint location);
    };
}
#endif