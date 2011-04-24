#include <iostream>
#include <stdlib.h>
#include "pbge/pbge.h"
#include "vbo_setup.h"

int cam_node_name;

class CustomSceneInitializer : public pbge::SceneInitializer {
    pbge::SceneGraph * operator () (pbge::GraphicAPI * ogl) {
        const float m_pi = 3.1415f;
        // FIXME: remove the state change line
        ogl->enableMode(pbge::GraphicAPI::DEPTH_TEST);
        pbge::ModelInstance * vboModel = createVBOInstance(ogl);
        pbge::SceneGraph * scene = new pbge::SceneGraph(new pbge::TransformationNode);
        pbge::Node * child = scene->appendChildTo(pbge::SceneGraph::ROOT, pbge::TransformationNode::rotation(m_pi/3, 0,0,20)->scale(0.5f, 0.5f, 0.5f));
        pbge::Node * light_parent = scene->appendChildTo(pbge::SceneGraph::ROOT, pbge::TransformationNode::translation(0.0f, 1.0f, 0.0f));
        dynamic_cast<pbge::Light*>(scene->appendChildTo(light_parent, new pbge::PointLight))->setDiffuseColor(1,0,0,1);
        scene->appendChildTo(light_parent, new pbge::ModelInstance(new pbge::BezierCurve()));
        pbge::Node * circle_parent = scene->appendChildTo(light_parent, pbge::TransformationNode::translation(1, 1, 0));
        scene->appendChildTo(circle_parent, new pbge::ModelInstance(new pbge::Ellipse(0.5f,0.2f,100)));
        
        float ** tensor;
        tensor = (float**)malloc(2*sizeof(float*));
        tensor[0] = (float*)malloc(2*sizeof(float));
        tensor[1] = (float*)malloc(2*sizeof(float));
        tensor[0][0] = 0.6f;
        tensor[0][1] = 0.2f;
        tensor[1][0] = 0.2f;
        tensor[1][1] = 0.3f;

        float ** tensor3d;
        tensor3d = (float**)malloc(3*sizeof(float*));
        tensor3d[0] = (float*)malloc(3*sizeof(float));
        tensor3d[1] = (float*)malloc(3*sizeof(float));
        tensor3d[2] = (float*)malloc(3*sizeof(float));
        tensor3d[0][0] = 0.2f;
        tensor3d[0][1] = -0.1f;
        tensor3d[0][2] = 0.1f;
        tensor3d[1][0] = -0.1f;
        tensor3d[1][1] = 0.1f;
        tensor3d[1][2] = -0.0f;
        tensor3d[2][0] = 0.1f;
        tensor3d[2][1] = -0.0f;
        tensor3d[2][2] = 0.1f;

        scene->appendChildTo(circle_parent, new pbge::ModelInstance(new pbge::TensorModel(tensor, 2, 50)));
        cam_node_name = scene->appendChildTo(pbge::SceneGraph::ROOT, pbge::TransformationNode::translation(0.0f, 1.0f, 5.0f))->getSceneGraphIndex();
        dynamic_cast<pbge::Light*>(scene->appendChildTo(cam_node_name, new pbge::PointLight))->setDiffuseColor(0,1,1,1);
        scene->appendChildTo(child, vboModel);
        pbge::Node * sphereParent = scene->appendChildTo(child, pbge::TransformationNode::translation(-1.5f, 0.0f, 0.0f));
        scene->appendChildTo(sphereParent, new pbge::ModelInstance(new pbge::TensorModel(tensor3d, 3, 20)));
        pbge::CameraNode * cam = dynamic_cast<pbge::CameraNode*>(scene->appendChildTo(cam_node_name, new pbge::CameraNode()));
        cam->lookAt(math3d::vector4(0,1,0), math3d::vector4(0,0,-1));
        cam->setPerspective(45, 1, 1.0f, 10);
        //math3d::print_internal_memory_page_info();

        return scene;
    }
};

/*
void keyboard(unsigned char k, int x, int y) {
    pbge::TransformationNode * cam_node = dynamic_cast<pbge::TransformationNode*>(scene->getGraphNode(cam_node_name));
    math3d::matrix44 m = cam_node->getTransformationMatrix();
    switch(k) {
        case 'w': m[1][3] += 0.1f; break;
        case 's': m[1][3] -= 0.1f; break;
    }
    cam_node->setTransformationMatrix(m);
}*/

int main(int argc, char ** argv) {
    pbge::Manager * manager = new pbge::Manager;
    manager->setWindowDimensions(500, 500);
    manager->setFullscreen(false);
    manager->setWindowTitle("vbo_rendering");
    manager->setSceneInitializer(new CustomSceneInitializer);
    manager->printDebugInformation(true);
    manager->displayGraphics();
    delete manager;
    return 0;
}