#include <iostream>

#include "pbge/gfx/Shader.h"
#include "pbge/core/Manager.h"
#include "pbge/gfx/Light.h"
#include "pbge/gfx/Buffer.h"
#include "pbge/gfx/SceneGraph.h"
#include "pbge/gfx/Node.h"
#include "pbge/gfx/Renderer.h"
#include "pbge/gfx/StateSet.h"

#include "vbo_setup.h"

int cam_node_name;
pbge::SceneGraph * scene;

#ifndef M_PI
#define M_PI 3.1415f
#endif

void setUp() {
    // FIXME: remove the state change line
    pbge::Manager::getInstance()->getOpenGL()->enableMode(pbge::OpenGL::DEPTH_TEST);

    pbge::ModelInstance * vboModel = createVBOInstance();
    scene = new pbge::SceneGraph(new pbge::TransformationNode);
    pbge::Node * child = scene->appendChildTo(pbge::SceneGraph::ROOT, pbge::TransformationNode::rotation(M_PI/3, 0,0,20)->scale(0.5f, 0.5f, 0.5f));
    pbge::Node * light_parent = scene->appendChildTo(pbge::SceneGraph::ROOT, pbge::TransformationNode::translation(0.0f, 1.0f, 0.0f));
    dynamic_cast<pbge::Light*>(scene->appendChildTo(light_parent, new pbge::PointLight))->setDiffuseColor(1,0,0,1);
    scene->appendChildTo(light_parent, new pbge::ModelInstance(new pbge::BezierCurve()));
    cam_node_name = scene->appendChildTo(pbge::SceneGraph::ROOT, pbge::TransformationNode::translation(0.0f, 1.0f, 5.0f))->getSceneGraphIndex();
    dynamic_cast<pbge::Light*>(scene->appendChildTo(cam_node_name, new pbge::PointLight))->setDiffuseColor(0,1,1,1);
    scene->appendChildTo(child, vboModel);
    pbge::CameraNode * cam = dynamic_cast<pbge::CameraNode*>(scene->appendChildTo(cam_node_name, new pbge::CameraNode()));
    cam->lookAt(math3d::vector4(0,1,0), math3d::vector4(0,0,-1));
    cam->setPerspective(45, 1, 1.0f, 10);
    math3d::print_internal_memory_page_info();
}

void keyboard(unsigned char k, int x, int y) {
    pbge::TransformationNode * cam_node = dynamic_cast<pbge::TransformationNode*>(scene->getGraphNode(cam_node_name));
    math3d::matrix44 m = cam_node->getTransformationMatrix();
    switch(k) {
        case 'w': m[1][3] += 0.1f; break;
        case 's': m[1][3] -= 0.1f; break;
    }
    cam_node->setTransformationMatrix(m);
}

int main(int argc, char ** argv) {
    pbge::Manager::init();
    pbge::Manager * manager = pbge::Manager::getInstance();
    manager->setWindowDimensions(500, 500);
    manager->setFullscreen(false);
    manager->setWindowTitle("vbo_rendering");
    setUp();
    manager->setMainSceneGraph(scene);
    manager->displayGraphics();
    delete scene;
    return 0;
}