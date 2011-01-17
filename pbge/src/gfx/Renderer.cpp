#include <GL/glew.h>
#include <string>
#include <cstdio>
#include <cstring>

#include "pbge/gfx/Renderer.h"
#include "pbge/gfx/Node.h"
#include "pbge/gfx/NodeVisitors.h"
#include "pbge/gfx/OpenGL.h"
#include "pbge/core/Manager.h"

using namespace pbge;

Renderer::Renderer(OpenGL * _ogl){
    this->ogl = _ogl;
    this->updater = new UpdaterVisitor;
    this->renderer = new RenderVisitor;
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

void Renderer::render(){
    ogl->clear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    Node * root = this->getScene()->getSceneGraphRoot();
    if(this->getScene() == NULL || root == NULL) return;
    updateScene();
    std::vector<Camera*> & activeCameras = updater->getActiveCameras();
    std::vector<Camera*>::iterator camera;
    for(camera = activeCameras.begin(); camera != activeCameras.end(); camera++) {
        renderer->setCurrentCamera(*camera);
        renderer->visit(root, ogl);
    }
}

