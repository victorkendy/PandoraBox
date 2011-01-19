#include <GL/glew.h>
#include <string>
#include <cstdio>
#include <cstring>

#include "pbge/gfx/Camera.h"
#include "pbge/gfx/SceneGraph.h"
#include "pbge/gfx/Renderer.h"
#include "pbge/gfx/Node.h"
#include "pbge/gfx/NodeVisitors.h"
#include "pbge/gfx/OpenGL.h"
#include "pbge/core/Manager.h"

using namespace pbge;

Renderer::Renderer(OpenGL * _ogl){
    this->ogl = _ogl;
    this->updater = new UpdaterVisitor;
    this->renderer = new ColorPassVisitor;
    this->depthRenderer = new DepthPassVisitor;
}

void Renderer::setScene(const SceneGraph * scene_graph) {
    scene = const_cast<SceneGraph *>(scene_graph);
}

SceneGraph * Renderer::getScene() {
    return scene;
}

void Renderer::updateScene(){
    updater->visit(scene->getSceneGraphRoot(), ogl);
}

void Renderer::renderWithCamera(Camera * camera, Node * root) {
    camera->setCamera(ogl);
    ogl->drawBuffer(GL_NONE);
    ogl->readBuffer(GL_NONE);
    ogl->depthFunc(GL_LEQUAL);
    depthRenderer->visit(root, ogl);
    ogl->depthMask(GL_FALSE);
    ogl->drawBuffer(GL_BACK);
    ogl->drawBuffer(GL_BACK);
    renderer->visit(root, ogl);
    camera->unsetCamera(ogl);
}

void Renderer::render(){
    ogl->depthMask(GL_TRUE);
    ogl->clear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    Node * root = this->getScene()->getSceneGraphRoot();
    if(this->getScene() == NULL || root == NULL) return;
    updateScene();
    std::vector<Camera*> & activeCameras = updater->getActiveCameras();
    std::vector<Camera*>::iterator camera;
    for(camera = activeCameras.begin(); camera != activeCameras.end(); camera++) {
        renderWithCamera(*camera, root);
    }
}

