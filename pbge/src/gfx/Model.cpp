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

ModelCollection::ModelCollection() {
    this->model = NULL;
    this->uniforms = new UniformSet();
    this->renderProgram = NULL;
    this->depthProgram = NULL;
    this->numberOfInstances = 1;
}

ModelCollection::ModelCollection(Model * _model) {
    this->model = _model;
    this->uniforms = new UniformSet();
    this->renderProgram = NULL;
    this->depthProgram = NULL;
    this->numberOfInstances = 1;
}

ModelCollection::~ModelCollection() {
    delete uniforms;
}

void ModelCollection::renderPass(RenderVisitor * visitor, GraphicAPI * gfx) {
    gfx->pushUniforms(this->uniforms);
    gfx->getState()->useProgram(this->renderProgram);
    gfx->updateState();
    VBOModel * vboModel = dynamic_cast<VBOModel *>(model);
    if(vboModel == NULL)
        gfx->getDrawController()->draw(model, this->numberOfInstances);
    else
        gfx->getDrawController()->drawVBOModel(vboModel, this->numberOfInstances);
}

void ModelCollection::postRenderPass(RenderVisitor * visitor, GraphicAPI * gfx) {
    gfx->popUniforms();
}

void ModelCollection::depthPass(RenderVisitor * visitor, GraphicAPI * gfx) {
    gfx->pushUniforms(this->uniforms);
    gfx->getState()->useProgram(this->depthProgram);
    gfx->updateState();
    VBOModel * vboModel = dynamic_cast<VBOModel *>(model);
    if(vboModel == NULL)
        gfx->getDrawController()->draw(model, this->numberOfInstances);
    else
        gfx->getDrawController()->drawVBOModel(vboModel, this->numberOfInstances);
}

void ModelCollection::postDepthPass(RenderVisitor * visitor, GraphicAPI * gfx) {
    gfx->popUniforms();
}

