#include <iostream>

#include "pbge/exceptions/exceptions.h"
#include "pbge/core/Manager.h"
#include "pbge/gfx/Buffer.h"
#include "pbge/gfx/Node.h"
#include "pbge/gfx/Model.h"
#include "pbge/gfx/Renderer.h"
#include "pbge/gfx/VBO.h"

#include <GL/glew.h>
#include <GL/glut.h>

pbge::Node * root, * child;
pbge::TransformationNode * cam_node;
pbge::Renderer * renderer;
pbge::SceneManager manager;
pbge::Camera camera;

class UmModelo : public pbge::Model {
public:
    UmModelo(pbge::VertexBuffer * _vbo, GLenum _primitive) {
        vbo = _vbo;
        primitive = _primitive;
    }

    void render(pbge::ModelInstance * instance, pbge::OpenGL * ogl) {
        glEnable(GL_VERTEX_ARRAY);
        vbo->bind(ogl);
        glDrawArrays(primitive, 0, vbo->getNVertices());
        vbo->unbind(ogl);
        glDisable(GL_VERTEX_ARRAY);
    }

private:
    GLenum primitive;
    pbge::VertexBuffer * vbo;
};

pbge::ModelInstance * vboModel = NULL;

void display() {
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    renderer->render();
    GLenum error;
    while((error = glGetError()) != GL_NO_ERROR) {
        std::cout << gluErrorString(error) << std::endl;
    }
    glutSwapBuffers();
}

void createNormalIndexes(std::vector<unsigned short> & ni) {
    unsigned short indexes[] = {3,3,3,3, 5,5,5,5, 0,0,0,0, 2,2,2,2, 1,1,1,1, 4,4,4,4};
    ni = std::vector<unsigned short>(indexes, indexes + 24);
}

std::vector<unsigned short> make(unsigned short * p, int n) {
    return std::vector<unsigned short>(p, p + n);
}

void createVertexIndexes(std::vector<unsigned short> & vi) {
    unsigned short indexes[] = {0,1,2,3, 7,4,1,0, 6,5,4,7, 3,2,5,6, 5,2,1,4, 0,3,6,7};
    vi = std::vector<unsigned short>(indexes, indexes + 24);
}

void createVBOInstance() {
    float v = 0.5f;
    std::vector<unsigned short> vIndexes;
    std::vector<unsigned short> nIndexes;
    createNormalIndexes(nIndexes);
    createVertexIndexes(vIndexes);
    pbge::VertexBufferBuilder builder(24);
    pbge::VertexAttribBuilder vertex = builder.addAttrib(3, pbge::VertexAttrib::VERTEX);
    pbge::VertexAttribBuilder normal = builder.addAttrib(3, pbge::VertexAttrib::NORMAL);

    builder.pushValue(normal,1,0,0).pushValue(normal,0,1,0).pushValue(normal,0,0,1).pushValue(normal,-1,0,0).pushValue(normal,0,-1,0).pushValue(normal,0,0,-1);
    builder.pushValue(vertex,-v,-v,-v).pushValue(vertex,-v,v,-v).pushValue(vertex,-v,v,v).pushValue(vertex,-v,-v,v);
    builder.pushValue(vertex,v,v,-v).pushValue(vertex,v,v,v).pushValue(vertex,v,-v,v).pushValue(vertex,v,-v,-v);
    builder.setAttribIndex(normal, nIndexes).setAttribIndex(vertex, vIndexes);
    pbge::VertexBuffer * vbo = builder.done();
    vboModel = new pbge::ModelInstance(new UmModelo(vbo, GL_QUADS));
}

void setUp() {
    glewInit();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0,0,0,0);
    glColor3f(1,0,0);

    createVBOInstance();
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
    cam_matrix[2][3] = 8.0f;
    cam_node->setTransformationMatrix(&cam_matrix);

    camera.setParent(cam_node);
    camera.lookAt(math3d::vector4(0,1,0), math3d::vector4(0,0,-1));
    camera.frustum.setPerspective(30, 1, 0.1f, 10);
    
    manager.setSceneGraph(root);
    manager.addCamera(&camera);
    renderer->setScene(&manager);
}

int main(int argc, char ** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
    glutInitWindowSize(500,500);
    glutCreateWindow("ahahah");
    setUp();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}