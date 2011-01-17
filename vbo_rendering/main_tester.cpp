#include <iostream>

#include "pbge/core/Manager.h"
#include "pbge/gfx/Buffer.h"
#include "pbge/gfx/Node.h"
#include "pbge/gfx/Renderer.h"
#include "pbge/gfx/StateSet.h"

#include <GL/glut.h>

#include "vbo_setup.h"

pbge::Node * root, * child;
pbge::TransformationNode * cam_node;
pbge::Renderer * renderer;
pbge::SceneManager manager;
pbge::Camera camera;

pbge::ModelInstance * vboModel = NULL;

void display() {
    renderer->render();
    GLenum error;
    while((error = glGetError()) != GL_NO_ERROR) {
        std::cout << gluErrorString(error) << std::endl;
    }
    glutSwapBuffers();
}
#ifndef M_PI
#define M_PI 3.1415
#endif
void setUp() {
    pbge::Manager::init();
    // FIXME: remove the state change line
    pbge::Manager::getInstance()->getOpenGL()->getState().enable(pbge::OpenGL::DEPTH_TEST);
    glClearColor(0,0,0,0);
    vboModel = createVBOInstance();
    // TODO: find somewhere else to put the instantiation
    renderer = new pbge::Renderer(pbge::Manager::getInstance()->getOpenGL());
    root = new pbge::TransformationNode;
    cam_node = pbge::TransformationNode::translation(0.0f, 1.0f, 5.0f);
    child = pbge::TransformationNode::rotation(M_PI/3, 0,0,20);
    root->addChild(child);
    root->addChild(cam_node);
    child->addChild(vboModel);
    cam_node->addChild(new pbge::CameraNode(&camera));
    camera.lookAt(math3d::vector4(0,1,0), math3d::vector4(0,0,-1));
    camera.frustum.setPerspective(45, 1, 1.0f, 10);
    manager.setSceneGraph(root);
    renderer->setScene(&manager);
}

void keyboard(unsigned char k, int x, int y) {
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
    return 0;
}