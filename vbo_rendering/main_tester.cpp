#include <iostream>

#include "pbge/core/Manager.h"
#include "pbge/gfx/Light.h"
#include "pbge/gfx/Buffer.h"
#include "pbge/gfx/SceneGraph.h"
#include "pbge/gfx/Node.h"
#include "pbge/gfx/Renderer.h"
#include "pbge/gfx/StateSet.h"

#include <GL/glut.h>

#include "vbo_setup.h"

int cam_node_name;
pbge::Renderer * renderer;
pbge::SceneGraph * scene;

void display() {
    renderer->render();
    GLenum error;
    while((error = glGetError()) != GL_NO_ERROR) {
        std::cout << gluErrorString(error) << std::endl;
    }
    glutSwapBuffers();
}

#ifndef M_PI
#define M_PI 3.1415f
#endif

void setUp() {
    pbge::Manager::init();
    // FIXME: remove the state change line
    pbge::Manager::getInstance()->getOpenGL()->getState().enable(pbge::OpenGL::DEPTH_TEST);
    glClearColor(0,0,0,0);
    pbge::ModelInstance * vboModel = createVBOInstance();
    // TODO: find somewhere else to put the instantiation
    renderer = new pbge::Renderer(pbge::Manager::getInstance()->getOpenGL());
    scene = new pbge::SceneGraph(new pbge::TransformationNode);
    pbge::Node * child = scene->appendChildTo(pbge::SceneGraph::ROOT, pbge::TransformationNode::rotation(M_PI/3, 0,0,20)->scale(0.5f, 0.5f, 0.5f));
    pbge::Node * light_parent = scene->appendChildTo(pbge::SceneGraph::ROOT, pbge::TransformationNode::translation(0.0f, 5.0f, 0.0f));
    scene->appendChildTo(light_parent, new pbge::PointLight);
    cam_node_name = scene->appendChildTo(pbge::SceneGraph::ROOT, pbge::TransformationNode::translation(0.0f, 1.0f, 5.0f))->getSceneGraphIndex();
    scene->appendChildTo(cam_node_name, new pbge::PointLight);
    scene->appendChildTo(child, vboModel);
    pbge::CameraNode * cam = dynamic_cast<pbge::CameraNode*>(scene->appendChildTo(cam_node_name, new pbge::CameraNode()));
    cam->lookAt(math3d::vector4(0,1,0), math3d::vector4(0,0,-1));
    cam->setPerspective(45, 1, 1.0f, 10);
    renderer->setScene(scene);
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
    glutPostRedisplay();
}

int main(int argc, char ** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
    glutInitWindowSize(500,500);
    glutCreateWindow("vbo_rendering");
    setUp();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    delete scene;
    delete renderer;
    return 0;
}