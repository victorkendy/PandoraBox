
#include "pbge/gfx/Model.h"
#include "pbge/gfx/VBO.h"

using namespace pbge;

VBOModel::VBOModel(pbge::VertexBuffer * _vbo, GLenum _primitive) {
    this->vbo = _vbo;
    this->primitive = _primitive;
}

void VBOModel::render(pbge::ModelInstance * instance, pbge::OpenGL * ogl) {
    ogl->enable(GL_VERTEX_ARRAY);
    vbo->bind(ogl);
    glDrawArrays(primitive, 0, vbo->getNVertices());
    vbo->unbind(ogl);
    ogl->disable(GL_VERTEX_ARRAY);
}


void ModelInstance::renderPass(RenderVisitor * visitor, OpenGL * ogl) {
    model->render(this, ogl);
}