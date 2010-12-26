#include <iostream>

#include "pbge/exceptions/exceptions.h"
#include "pbge/core/Manager.h"

#include "pbge/gfx/Buffer.h"
#include "pbge/gfx/Node.h"
#include "pbge/gfx/Model.h"
#include "pbge/gfx/Renderer.h"
#include "pbge/gfx/TextureRendererFactory.h"
#include "pbge/gfx/VBO.h"

#include <GL/glew.h>
#include <GL/glut.h>

pbge::Node * root, * child;
pbge::TransformationNode * cam_node;
pbge::OpenGL * ogl = new pbge::OpenGL;
pbge::Renderer renderer(ogl);
pbge::SceneManager manager;

pbge::Camera camera;

pbge::OpenGL * gl;

class UmModelo : public pbge::Model {
public:
    UmModelo(pbge::VertexBuffer * _vbo) {
        vbo = _vbo;
    }

    void render(pbge::ModelInstance * instance, pbge::OpenGL * ogl) {
        glEnable(GL_VERTEX_ARRAY);
        vbo->bind(ogl);
        glDrawArrays(GL_QUADS, 0, 24);
        vbo->unbind(ogl);
        glDisable(GL_VERTEX_ARRAY);
    }

private:
    pbge::VertexBuffer * vbo;
};

class Teapot : public pbge::Model {
    void render(pbge::ModelInstance * instance, pbge::OpenGL * ogl) {
        glutSolidTeapot(1);
    }
};

pbge::ModelInstance * instance = new pbge::ModelInstance(new Teapot);
pbge::ModelInstance * vboModel = NULL;


void display() {
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    renderer.render();
    GLenum error;
    while((error = glGetError()) != GL_NO_ERROR) {
        std::cout << gluErrorString(error) << std::endl;
    }
    glutSwapBuffers();
}

void createNormalIndexes(std::vector<unsigned short> & ni) {
    ni.push_back(3); ni.push_back(3); ni.push_back(3); ni.push_back(3);
    ni.push_back(5); ni.push_back(5); ni.push_back(5); ni.push_back(5);
    ni.push_back(0); ni.push_back(0); ni.push_back(0); ni.push_back(0);
    ni.push_back(2); ni.push_back(2); ni.push_back(2); ni.push_back(2);
    ni.push_back(1); ni.push_back(1); ni.push_back(1); ni.push_back(1);
    ni.push_back(4); ni.push_back(4); ni.push_back(4); ni.push_back(4);
}

void createVertexIndexes(std::vector<unsigned short> & vi) {
    vi.push_back(0); vi.push_back(1); vi.push_back(2); vi.push_back(3);
    vi.push_back(7); vi.push_back(4); vi.push_back(1); vi.push_back(0);
    vi.push_back(6); vi.push_back(5); vi.push_back(4); vi.push_back(7);
    vi.push_back(3); vi.push_back(2); vi.push_back(5); vi.push_back(6);
    vi.push_back(5); vi.push_back(2); vi.push_back(1); vi.push_back(4);
    vi.push_back(0); vi.push_back(3); vi.push_back(6); vi.push_back(7);
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
    vboModel = new pbge::ModelInstance(new UmModelo(vbo));
}

void setUp() {
    glewInit();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0,0,0,0);
    glColor3f(1,0,0);

    createVBOInstance();
    pbge::TransformationNode * node = new pbge::TransformationNode;

    math3d::matrix44 m = math3d::identity44;
    root = new pbge::TransformationNode;
    cam_node = new pbge::TransformationNode;
  
    child = node;
    node->setTransformationMatrix(&m);
    root->addChild(child);
    root->addChild(cam_node);
    //child->addModelInstance(instance);
    child->addModelInstance(vboModel);
    
    math3d::matrix44 cam_matrix = math3d::identity44;
    cam_matrix[2][3] = 8.0f;
    cam_node->setTransformationMatrix(&cam_matrix);

    camera.setParent(cam_node);
    camera.lookAt(math3d::vector4(0,1,0), math3d::vector4(0,0,-1));
    camera.frustum.setPerspective(30, 1, 0.1f, 10);
    
    manager.setSceneGraph(root);
    manager.addCamera(&camera);

    renderer.setScene(&manager);
    gl = pbge::Manager::getInstance()->getOpenGL();
    
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