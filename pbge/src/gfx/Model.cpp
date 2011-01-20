
#include "pbge/gfx/Model.h"
#include "pbge/gfx/VBO.h"

using namespace pbge;

VBOModel::VBOModel(pbge::VertexBuffer * _vbo, GLenum _primitive) {
    this->vbo = _vbo;
    this->primitive = _primitive;
}

void VBOModel::render(ModelInstance * instance, OpenGL * ogl) {
    ogl->enable(GL_VERTEX_ARRAY);
    vbo->bind(ogl);
    glDrawArrays(primitive, 0, vbo->getNVertices());
    vbo->unbind(ogl);
    ogl->disable(GL_VERTEX_ARRAY);
}

void VBOModel::renderDepth(ModelInstance* instance, OpenGL * ogl) {
    ogl->enable(GL_VERTEX_ARRAY);
    vbo->bindOnly(VertexAttrib::VERTEX, ogl);
    glDrawArrays(primitive, 0, vbo->getNVertices());
    vbo->unbind(ogl);
    ogl->disable(GL_VERTEX_ARRAY);
}


void ModelInstance::renderPass(RenderVisitor * visitor, OpenGL * ogl) {
    ogl->updateState();
    ogl->uploadProjection();
    model->render(this, ogl);
}

void ModelInstance::depthPass(RenderVisitor * visitor, OpenGL * ogl) {
    ogl->updateState();
    ogl->uploadProjection();
    model->renderDepth(this, ogl);
}