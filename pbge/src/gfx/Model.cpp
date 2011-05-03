#include <math.h>

#include "pbge/gfx/UniformSet.h"
#include "pbge/gfx/Shader.h"
#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/ResourceStorage.h"
#include "pbge/gfx/Model.h"
#include "pbge/gfx/VBO.h"
#include "pbge/gfx/StateSet.h"
#include "pbge/exceptions/exceptions.h"
#include "pbge/core/definitions.h"
#include "math3d/math3d.h"

using namespace pbge;

VBOModel::VBOModel(pbge::VertexBuffer * _vbo, GLenum _primitive) {
    this->vbo = _vbo;
    this->primitive = _primitive;
}

void VBOModel::render(ModelInstance * instance, GraphicAPI * ogl) {
    vbo->bind(ogl);
    glDrawArrays(primitive, 0, vbo->getNVertices());
    vbo->unbind(ogl);
    ogl->disable(GL_VERTEX_ARRAY);
}

void VBOModel::renderDepth(ModelInstance* instance, GraphicAPI * ogl) {
    vbo->bind(ogl);
    glDrawArrays(primitive, 0, vbo->getNVertices());
    vbo->unbind(ogl);
    ogl->disable(GL_VERTEX_ARRAY);
}

BezierCurve::BezierCurve() {
    this->controlPoints = new float[16];
    this->evaluator = NULL;
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

GPUProgram * BezierCurve::getEvaluator(GraphicAPI * ogl) {
    const std::string evaluatorVS = 
        "uniform vec4 p0;\n"
        "uniform vec4 p1;\n"
        "uniform vec4 p2;\n"
        "uniform vec4 p3;\n"
        "void main() {\n"
        "   float t = gl_Vertex.x;\n"
        "   float oneMinusT = 1.0 - t;\n"
        "   float tl = oneMinusT;\n"
        "   vec4 position = 3 * tl * p2 + t * p3;\n"
        "   tl *= oneMinusT;\n"
        "   position = 3 * tl * p1 + t * position;\n"
        "   oneMinusT *= oneMinusT;\n"
        "   position = tl * p0 + t * position;\n"
        "   gl_Position = gl_ModelViewProjectionMatrix * position;\n"
        "   gl_FrontColor = gl_Color;\n"
        "}";
    if(evaluator == NULL) {
        GPUProgram * storedEvaluator = ogl->getStorage()->getNamedProgram("pbge.defaultBezierEvaluator");
        if(storedEvaluator == NULL) {
            GLProgram * program = GLProgram::fromString(evaluatorVS, "");
            ogl->getStorage()->storeNamedProgram("pbge.defaultBezierEvaluator", program);
            evaluator = program;
        } else {
            evaluator = storedEvaluator;
        }
    }
    return evaluator;
}

void BezierCurve::render(ModelInstance * instance, GraphicAPI * ogl) {

    GPUProgram * program = this->getEvaluator(ogl);
    ogl->getState()->useProgram(program);
    dynamic_cast<UniformFloatVec4*>(ogl->getState()->getUniformValue(UniformInfo("p0", FLOAT_VEC4, -1)))->setValue(-1,0,0,1);
    dynamic_cast<UniformFloatVec4*>(ogl->getState()->getUniformValue(UniformInfo("p1", FLOAT_VEC4, -1)))->setValue(-0.5f,-2.0f,0,1);
    dynamic_cast<UniformFloatVec4*>(ogl->getState()->getUniformValue(UniformInfo("p2", FLOAT_VEC4, -1)))->setValue(0.0f,1.0f,0,1);
    dynamic_cast<UniformFloatVec4*>(ogl->getState()->getUniformValue(UniformInfo("p3", FLOAT_VEC4, -1)))->setValue(1.0f,1.0f,0,1);
    ogl->updateState();
    glBegin(GL_LINE_STRIP);
    glColor3f(1,1,1);
    for(int i = 0; i < 100; i++) {
        glVertex2f(0.01f * i, 0.01f * i);
    }
    glEnd();
    ogl->getState()->useProgram(NULL);
}

void BezierCurve::renderDepth(ModelInstance * instance, GraphicAPI * ogl) {}


ModelInstance::ModelInstance() {
    this->model = NULL;
    this->uniforms = new UniformSet();
    this->renderProgram = NULL;
    this->depthProgram = NULL;
}

ModelInstance::ModelInstance(Model * m) {
    this->model = m;
    this->uniforms = new UniformSet();
    this->renderProgram = NULL;
    this->depthProgram = NULL;
}

ModelInstance::~ModelInstance() {
    delete uniforms;
}

void ModelInstance::renderPass(RenderVisitor * visitor, GraphicAPI * ogl) {
    ogl->pushUniforms(this->uniforms);
    ogl->getState()->useProgram(this->renderProgram);
    ogl->updateState();
    model->render(this, ogl);
}

void ModelInstance::postRenderPass(RenderVisitor * visitor, GraphicAPI * ogl) {
    ogl->popUniforms();
}

void ModelInstance::depthPass(RenderVisitor * visitor, GraphicAPI * ogl) {
    ogl->pushUniforms(this->uniforms);
    ogl->getState()->useProgram(this->depthProgram);
    ogl->updateState();
    model->renderDepth(this, ogl);
}

void ModelInstance::postDepthPass(RenderVisitor * visitor, GraphicAPI * ogl) {
    ogl->popUniforms();
}

Circle::Circle(const float & _radius, const int & _slices) {
    if(_radius <= 0) {
        throw IllegalArgumentException("radius must have a positive value");
    }
    if(_slices < 3) {
        throw IllegalArgumentException("there must be at least 3 slices");
    }
    this->radius = _radius;
    this->slices = _slices;
}

void Circle::renderDepth(ModelInstance * instance, GraphicAPI * ogl) {}

void Circle::render(ModelInstance * instance, GraphicAPI * ogl) {
    float radius = this->radius;
    float param_step = 2 * PBGE_pi / this->slices;
    
    glBegin(GL_LINE_LOOP);
    glColor3f(1,1,1);
    for(float t = 0; t < 2 * PBGE_pi; t+=param_step) {
        glVertex2f(radius * cos(t), radius * sin(t));
    }
    glEnd();
    ogl->getState()->useProgram(NULL);
}

void Ellipse::setTransformation(const math3d::matrix44 & transformation) {
    *(this->transformation) = transformation;
}

// I want a default value for slices
Ellipse::Ellipse(const float & _x_semi_axis, const float & _y_semi_axis, const int & _slices) : Circle(1.0f, _slices) {
    if(_x_semi_axis <= 0 || _y_semi_axis <= 0) {
        throw IllegalArgumentException("both semi-axis must be positive values");
    }
    this->x_semi_axis = _x_semi_axis;
    this->y_semi_axis = _y_semi_axis;
    this->evaluator = NULL;
    this->transformation = new math3d::matrix44(math3d::identity44);
}

void Ellipse::render(ModelInstance * instance, GraphicAPI * ogl) {
    GPUProgram * program = this->getEvaluator(ogl);
    ogl->getState()->useProgram(program);
    dynamic_cast<UniformFloatVec2*>(ogl->getState()->getUniformValue(UniformInfo("scale", FLOAT_VEC2, -1)))->setValue(this->x_semi_axis, this->y_semi_axis);
    dynamic_cast<UniformMat4*>(ogl->getState()->getUniformValue(UniformInfo("transformation", FLOAT_MAT4, -1)))->setValue(*(this->transformation));
    ogl->updateState();
    Circle::render(instance, ogl);
}

GPUProgram * Ellipse::getEvaluator(GraphicAPI * ogl) {
    const std::string evaluatorVS = 
        "uniform vec2 scale;\n"
        "uniform mat4 transformation;\n"
        "void main() {\n"
        "   mat4 scaleMatrix = mat4(1.0);\n"
        "   scaleMatrix[0][0] = scale[0];\n"
        "   scaleMatrix[1][1] = scale[1];\n"
        "   gl_Position = gl_ModelViewProjectionMatrix * transformation * scaleMatrix * gl_Vertex;\n"
        "   gl_FrontColor = gl_Color;\n"
        "}";
    if(evaluator == NULL) {
        GPUProgram * storedEvaluator = ogl->getStorage()->getNamedProgram("pbge.defaultEllipseEvaluator");
        if(storedEvaluator == NULL) {
            GLProgram * program = GLProgram::fromString(evaluatorVS, "");
            ogl->getStorage()->storeNamedProgram("pbge.defaultEllipseEvaluator", program);
            evaluator = program;
        } else {
            evaluator = storedEvaluator;
        }
    }
    return evaluator;
}

Sphere::Sphere(const float & _radius, const int & _slices) {
    if(_radius <= 0) {
        throw IllegalArgumentException("radius must have a positive value");
    }
    if(_slices < 3) {
        throw IllegalArgumentException("there must be at least 3 slices");
    }
    this->radius = _radius;
    this->slices = _slices;
}

void Sphere::renderDepth(ModelInstance * instance, GraphicAPI * ogl) {}

void Sphere::render(ModelInstance * instance, GraphicAPI * ogl) {
    float radius = this->radius;
    float param_step = 2 * PBGE_pi / this->slices;
    
    glBegin(GL_QUAD_STRIP);
    glColor3f(1,1,1);
    for(float phi = 0, next_phi = param_step; phi < 2 * PBGE_pi; phi = next_phi, next_phi += param_step) {
        for(float theta = 0; theta < PBGE_pi; theta += param_step) {
            glVertex3f(radius * sin(theta) * cos(phi), radius * sin(theta) * sin(phi), radius * cos(theta));
            glVertex3f(radius * sin(theta) * cos(next_phi), radius * sin(theta) * sin(next_phi), radius * cos(theta));
        }
    }
    glEnd();
    ogl->getState()->useProgram(NULL);
}

void Ellipsoid::setTransformation(const math3d::matrix44 & transformation) {
    *(this->transformation) = transformation;
}

// I want a default value for slices
Ellipsoid::Ellipsoid(const float & _x_semi_axis, const float & _y_semi_axis, const float & _z_semi_axis, const int & _slices) : Sphere(1.0f, _slices) {
    if(_x_semi_axis <= 0 || _y_semi_axis <= 0 || _z_semi_axis <= 0) {
        throw IllegalArgumentException("all semi-axis must be positive values");
    }
    this->x_semi_axis = _x_semi_axis;
    this->y_semi_axis = _y_semi_axis;
    this->z_semi_axis = _z_semi_axis;
    this->evaluator = NULL;
    this->transformation = new math3d::matrix44(math3d::identity44);
}

void Ellipsoid::render(ModelInstance * instance, GraphicAPI * ogl) {
    GPUProgram * program = this->getEvaluator(ogl);
    ogl->getState()->useProgram(program);
    dynamic_cast<UniformFloatVec3*>(ogl->getState()->getUniformValue(UniformInfo("scale", FLOAT_VEC3, -1)))->setValue(this->x_semi_axis, this->y_semi_axis, this->z_semi_axis);
    dynamic_cast<UniformMat4*>(ogl->getState()->getUniformValue(UniformInfo("transformation", FLOAT_MAT4, -1)))->setValue(*(this->transformation));
    ogl->updateState();
    Sphere::render(instance, ogl);
}

GPUProgram * Ellipsoid::getEvaluator(GraphicAPI * ogl) {
    const std::string evaluatorVS = 
        "uniform vec3 scale;\n"
        "uniform mat4 transformation;\n"
        "void main() {\n"
        "   mat4 scaleMatrix = mat4(1.0);\n"
        "   scaleMatrix[0][0] = scale[0];\n"
        "   scaleMatrix[1][1] = scale[1];\n"
        "   scaleMatrix[2][2] = scale[2];\n"
        "   gl_Position = gl_ModelViewProjectionMatrix * transformation * scaleMatrix * gl_Vertex;\n"
        "   gl_FrontColor = gl_Color;\n"
        "}";
    if(evaluator == NULL) {
        GPUProgram * storedEvaluator = ogl->getStorage()->getNamedProgram("pbge.defaultEllipseEvaluator");
        if(storedEvaluator == NULL) {
            GLProgram * program = GLProgram::fromString(evaluatorVS, "");
            ogl->getStorage()->storeNamedProgram("pbge.defaultEllipseEvaluator", program);
            evaluator = program;
        } else {
            evaluator = storedEvaluator;
        }
    }
    return evaluator;
}