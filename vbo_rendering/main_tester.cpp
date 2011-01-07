#include <iostream>

#include "pbge/exceptions/exceptions.h"
#include "pbge/core/Manager.h"
#include "pbge/gfx/Buffer.h"
#include "pbge/gfx/Node.h"
#include "pbge/gfx/Model.h"
#include "pbge/gfx/Renderer.h"
#include "pbge/gfx/VBO.h"
#include "pbge/gfx/StateSet.h"

#include <GL/glut.h>

#include "vbo_setup.h"

pbge::Node * root, * child, * trueRoot;
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

void setUp() {
    pbge::Manager::init();
    // FIXME: remove the state change line
    pbge::Manager::getInstance()->getOpenGL()->getState().enable(pbge::OpenGL::DEPTH_TEST);
    glClearColor(0,0,0,0);
    vboModel = createVBOInstance();
    renderer = new pbge::Renderer(pbge::Manager::getInstance()->getOpenGL());
    pbge::TransformationNode * node = new pbge::TransformationNode;
    math3d::matrix44 m = math3d::identity44;
    root = new pbge::TransformationNode;
    cam_node = new pbge::TransformationNode;
    child = node;
    node->setTransformationMatrix(&m);
    root->addChild(child);
    root->addChild(cam_node);
    child->addModelInstance(vboModel);
    math3d::matrix44 cam_matrix = math3d::identity44;
    cam_matrix[2][3] = 3.0f; cam_matrix[1][3] = 1.0f;
    cam_node->setTransformationMatrix(&cam_matrix);
    camera.setParent(cam_node);
    camera.lookAt(math3d::vector4(0,1,0), math3d::vector4(0,0,-1));
    camera.frustum.setPerspective(45, 1, 1.0f, 10);
    manager.setSceneGraph(root);
    manager.addCamera(&camera);
    renderer->setScene(&manager);
}

void keyboard(unsigned char k, int x, int y) {
    math3d::matrix44 * m = cam_node->getTransformationMatrix();
    switch(k) {
        case 'w': (*m)[1][3] += 0.1f; break;
        case 's': (*m)[1][3] -= 0.1f; break;
    }
    cam_node->setTransformationMatrix(m);
    glutPostRedisplay();
}

int main(int argc, char ** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
    glutInitWindowSize(500,500);
    glutCreateWindow("ahahah");
    setUp();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}