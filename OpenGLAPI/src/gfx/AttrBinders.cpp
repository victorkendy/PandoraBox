#include <GL/glew.h>
#include <string>
#include "pbge/gfx/VBO.h"

#include "OpenGLAPI/gfx/GLProgram.h"
#include "OpenGLAPI/gfx/AttrBinders.h"

using namespace pbge;


#define ATTRIB_POINTER_OFFSET(offset) ((GLbyte *)NULL + (offset))


void DeprVertexBinder::bind(VertexBuffer *attrs) {
    VertexAttrib * attr = attrs->findByType(VertexAttrib::VERTEX);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(attr->getNCoord(), GL_FLOAT, attr->getStride(), ATTRIB_POINTER_OFFSET(attr->getOffset()));
}

void DeprVertexBinder::unbind() {
    glDisableClientState(GL_VERTEX_ARRAY);
}

void DeprNormalBinder::bind(VertexBuffer *attrs) {
    VertexAttrib * attr = attrs->findByType(VertexAttrib::NORMAL);
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, attr->getStride(), ATTRIB_POINTER_OFFSET(attr->getOffset()));
}

void DeprNormalBinder::unbind() {
    glDisableClientState(GL_NORMAL_ARRAY);
}

void DeprColorBinder::bind(VertexBuffer * attrs) {
    VertexAttrib * attr = attrs->findByType(VertexAttrib::COLOR);
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(attr->getNCoord(), GL_FLOAT, attr->getStride(), ATTRIB_POINTER_OFFSET(attr->getOffset()));
}

void DeprColorBinder::unbind() {
    glDisableClientState(GL_COLOR_ARRAY);
}

void DeprSecondaryColorBinder::bind(VertexBuffer * attrs) {
    VertexAttrib * attr = attrs->findByType(VertexAttrib::SECONDARY_COLOR);
    glEnableClientState(GL_SECONDARY_COLOR_ARRAY);
    glSecondaryColorPointer(3, GL_FLOAT, attr->getStride(), ATTRIB_POINTER_OFFSET(attr->getOffset()));
}

void DeprSecondaryColorBinder::unbind() {
    glEnableClientState(GL_SECONDARY_COLOR_ARRAY);
}

void CustomAttrBinder::bind(pbge::VertexBuffer *attrs) {
    VertexAttrib * attr = attrs->findByName(name);
    // missing arguments?
    glVertexAttribPointer(location, attr->getNCoord(), GL_FLOAT, GL_FALSE, attr->getStride(), ATTRIB_POINTER_OFFSET(attr->getOffset()));
    glEnableVertexAttribArray(location);
}

void CustomAttrBinder::unbind() {
    glDisableVertexAttribArray(location);
}

void SemanticAttribBinder::bind(VertexBuffer * attrs) {
    VertexAttrib * attr = attrs->findByType(type);
    glVertexAttribPointer(location, attr->getNCoord(), GL_FLOAT, GL_FALSE, attr->getStride(), ATTRIB_POINTER_OFFSET(attr->getOffset()));
    glEnableVertexAttribArray(location);
}

void SemanticAttribBinder::unbind() {
    glDisableVertexAttribArray(location);
}

AttrBinder * AttrBinders::binderFor(const std::string &name, GLint location) {
    if(name.find("gl_") == 0) { // built-in attr
        return constructGLBuiltIn(name);
    } else {
        return constructCustomAttrib(name, location);
    }
}

AttrBinder * AttrBinders::constructCustomAttrib(const std::string & name, GLint location) {
    if(name == "pbge_Vertex") {
        return new SemanticAttribBinder(VertexAttrib::VERTEX, location);
    } else if (name == "pbge_Normal") {
        return new SemanticAttribBinder(VertexAttrib::NORMAL, location);
    } else if (name == "pbge_Color") {
        return new SemanticAttribBinder(VertexAttrib::COLOR, location);
    } else if (name == "pbge_SecondaryColor") {
        return new SemanticAttribBinder(VertexAttrib::SECONDARY_COLOR, location);
    } else if(location != -1) { // check if attr is valid
        return new CustomAttrBinder(name, location);
    } else {
        return NULL;
    }
}

AttrBinder * AttrBinders::constructGLBuiltIn(const std::string & name) {
    if(name == "gl_Vertex") {
        return new DeprVertexBinder;
    } else if (name == "gl_Normal") {
        return new DeprNormalBinder;
    } else if (name == "gl_Color") {
        return new DeprColorBinder;
    } else if (name == "gl_SecondaryColor") {
        return new DeprSecondaryColorBinder;
    } else {
        return NULL;
    }
}

#undef ATTRIB_POINTER_OFFSET