#include <math.h>

#include "pbge/gfx/UniformSet.h"
#include "pbge/gfx/Shader.h"
#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/DrawController.h"
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

// Because the VBO has driver specific optimizations,
// we don't give any implementation for the render method
// but just delegate it to the GraphicAPI
 
void VBOModel::beforeRender(GraphicAPI * gfx) {
}

void VBOModel::afterRender(GraphicAPI * gfx) {
}

void VBOModel::render(GraphicAPI * gfx) {
}

void VBOModel::renderDepth(GraphicAPI * gfx) {
}

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

void ModelInstance::renderPass(RenderVisitor * visitor, GraphicAPI * gfx) {
    gfx->pushUniforms(this->uniforms);
    gfx->getState()->useProgram(this->renderProgram);
    gfx->updateState();
    VBOModel * vboModel = dynamic_cast<VBOModel *>(model);
    if(vboModel == NULL)
        gfx->getDrawController()->draw(model);
    else
        gfx->getDrawController()->drawVBOModel(vboModel);
}

void ModelInstance::postRenderPass(RenderVisitor * visitor, GraphicAPI * gfx) {
    gfx->popUniforms();
}

void ModelInstance::depthPass(RenderVisitor * visitor, GraphicAPI * gfx) {
    gfx->pushUniforms(this->uniforms);
    gfx->getState()->useProgram(this->depthProgram);
    gfx->updateState();
    VBOModel * vboModel = dynamic_cast<VBOModel *>(model);
    if(vboModel == NULL)
        gfx->getDrawController()->draw(model);
    else
        gfx->getDrawController()->drawVBOModel(vboModel);
}

void ModelInstance::postDepthPass(RenderVisitor * visitor, GraphicAPI * gfx) {
    gfx->popUniforms();
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

void Circle::beforeRender(GraphicAPI * gfx) {
}

void Circle::afterRender(GraphicAPI * gfx) {}

void Circle::renderDepth(GraphicAPI * ogl) {}

void Circle::render(GraphicAPI * ogl) {
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

void Ellipse::beforeRender(GraphicAPI * gfx) {}

void Ellipse::afterRender(GraphicAPI * gfx) {}

void Ellipse::render(GraphicAPI * ogl) {
    GPUProgram * program = this->getEvaluator(ogl);
    ogl->getState()->useProgram(program);
    dynamic_cast<UniformFloatVec2*>(ogl->getState()->getUniformValue(UniformInfo("scale", FLOAT_VEC2, -1)))->setValue(this->x_semi_axis, this->y_semi_axis);
    dynamic_cast<UniformMat4*>(ogl->getState()->getUniformValue(UniformInfo("transformation", FLOAT_MAT4, -1)))->setValue(*(this->transformation));
    ogl->updateState();
    Circle::render(ogl);
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

void Sphere::beforeRender(GraphicAPI * gfx){}

void Sphere::afterRender(GraphicAPI * gfx){}

void Sphere::renderDepth(GraphicAPI * ogl) {}

void Sphere::render(GraphicAPI * ogl) {
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

void Ellipsoid::beforeRender(GraphicAPI * gfx) {}

void Ellipsoid::afterRender(GraphicAPI * gfx) {}

void Ellipsoid::render(GraphicAPI * ogl) {
    GPUProgram * program = this->getEvaluator(ogl);
    ogl->getState()->useProgram(program);
    dynamic_cast<UniformFloatVec3*>(ogl->getState()->getUniformValue(UniformInfo("scale", FLOAT_VEC3, -1)))->setValue(this->x_semi_axis, this->y_semi_axis, this->z_semi_axis);
    dynamic_cast<UniformMat4*>(ogl->getState()->getUniformValue(UniformInfo("transformation", FLOAT_MAT4, -1)))->setValue(*(this->transformation));
    ogl->updateState();
    Sphere::render(ogl);
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
