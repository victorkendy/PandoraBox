
#include "pbge/gfx/Shader.h"
#include "pbge/gfx/OpenGL.h"
#include "pbge/gfx/ResourceStorage.h"
#include "pbge/gfx/Model.h"
#include "pbge/gfx/VBO.h"
#include "pbge/gfx/StateSet.h"

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

BezierCurve::BezierCurve() {
    this->controlPoints = new float[16];
    this->tesselator = NULL;
    currentIndex = 0;
}

void BezierCurve::addControlPoint(const float & x, const float & y, const float & z, const float & w) {
    int index = currentIndex * 4;
    currentIndex++;
    controlPoints[index] = x;
    controlPoints[index + 1] = y;
    controlPoints[index + 2] = z;
    controlPoints[index + 3] = w;
}

GPUProgram * BezierCurve::getTesselator(OpenGL * ogl) {
    const std::string tesselatorVS = 
        "uniform vec4 p0;\n"
        "uniform vec4 p1;\n"
        "uniform vec4 p2;\n"
        "uniform vec4 p3;\n"
        "void main() {\n"
        "   float t = gl_Vertex.x;\n"
        "   float oneMinusT = 1.0 - t;\n"
        "   vec4 position = 3 * oneMinusT * p2 + t * p3;\n"
        "   oneMinusT *= 1.0 - t;\n"
        "   position = 3 * oneMinusT * p1 + t * position;\n"
        "   oneMinusT *= 1.0 - t;\n"
        "   position = oneMinusT * p0 + t * position;\n"
        "   gl_Position = gl_ModelViewProjectionMatrix * position;\n"
        "   gl_FrontColor = gl_Color;\n"
        "}";
    if(tesselator == NULL) {
        GPUProgram * storedTesselator = ogl->getStorage().getNamedProgram("pbge.defaultBezierTesselator");
        if(storedTesselator == NULL) {
            GLProgram * program = GLProgram::fromString(tesselatorVS, "");
            ogl->getStorage().storeNamedProgram("pbge.defaultBezierTesselator", program);
            tesselator = program;
        } else {
            tesselator = storedTesselator;
        }
    }
    return tesselator;
}

void BezierCurve::render(ModelInstance * instance, OpenGL * ogl) {
    GPUProgram * program = this->getTesselator(ogl);
    ogl->getState().useProgram(program);
    dynamic_cast<UniformFloatVec4*>(ogl->getState().getUniformValue(UniformInfo("p0", FLOAT_VEC4, -1)))->setValue(-1,0,0,1);
    dynamic_cast<UniformFloatVec4*>(ogl->getState().getUniformValue(UniformInfo("p1", FLOAT_VEC4, -1)))->setValue(-0.5f,-2.0f,0,1);
    dynamic_cast<UniformFloatVec4*>(ogl->getState().getUniformValue(UniformInfo("p2", FLOAT_VEC4, -1)))->setValue(0.0f,1.0f,0,1);
    dynamic_cast<UniformFloatVec4*>(ogl->getState().getUniformValue(UniformInfo("p3", FLOAT_VEC4, -1)))->setValue(1.0f,1.0f,0,1);
    ogl->updateState();
    glBegin(GL_LINE_STRIP);
    glColor3f(1,1,1);
    for(int i = 0; i < 100; i++) {
        glVertex2f(0.01f * i, 0.01f * i);
    }
    glEnd();
    ogl->getState().useProgram(NULL);
}

void BezierCurve::renderDepth(ModelInstance * instance, OpenGL * ogl) {}


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