#include <iostream>
#include <stdlib.h>
#include "pbge/pbge.h"
#include "vbo_setup.h"
#include "TensorModel.h"

int cam_node_name;

class CustomSceneInitializer : public pbge::SceneInitializer {
private:
    pbge::GPUProgram * ellipseProgram;

    pbge::ModelInstance * createEllipse(pbge::VBOModel * circle, float x_semi_axis, float y_semi_axis) {
        pbge::ModelInstance * ellipse = new pbge::ModelInstance(circle);
        pbge::UniformSet * uniforms = ellipse->getUniformSet();
        uniforms->getFloatVec2("scale")->setValue(x_semi_axis, y_semi_axis);
        ellipse->setRenderPassProgram(ellipseProgram);
        return ellipse;
    }

    void createEllipseProgram(pbge::GraphicAPI * gfx) {
        std::string vertexShader =
            "uniform vec2 scale;\n"
            "void main() {\n"
            "   mat4 scaleMatrix = mat4(1.0);\n"
            "   scaleMatrix[0][0] = scale[0];\n"
            "   scaleMatrix[1][1] = scale[1];\n"
            "   gl_Position = gl_ModelViewProjectionMatrix * scaleMatrix * gl_Vertex;\n"
            "   gl_FrontColor = vec4(1,1,1,1);\n"
            "}";
        ellipseProgram = gfx->getFactory()->createProgramFromString(vertexShader, "");
    }
public:
    pbge::SceneGraph * operator () (pbge::GraphicAPI * ogl) {
        createEllipseProgram(ogl);
        const float m_pi = 3.1415f;
        // FIXME: remove the state change line
        ogl->enableMode(pbge::GraphicAPI::DEPTH_TEST);
        pbge::ModelInstance * vboModel = createVBOInstance(ogl);
        pbge::SceneGraph * scene = new pbge::SceneGraph(new pbge::TransformationNode);
        pbge::Node * child = scene->appendChildTo(pbge::SceneGraph::ROOT, pbge::TransformationNode::rotation(m_pi/3, 0,0,20)->scale(0.5f, 0.5f, 0.5f));
        pbge::Node * light_parent = scene->appendChildTo(pbge::SceneGraph::ROOT, pbge::TransformationNode::translation(0.0f, 1.0f, 0.0f));
        dynamic_cast<pbge::Light*>(scene->appendChildTo(light_parent, new pbge::PointLight))->setDiffuseColor(1,0,0,1);
        
        pbge::VBOModel * bezier = pbge::Geometrics::createBezier(math3d::vector4(-1,0,0,1), math3d::vector4(-0.5f,-2.0f,0,1), 
                                                                 math3d::vector4(0.0f,1.0f,0,1), math3d::vector4(1.0f,1.0f,0,1), 100, ogl);
        scene->appendChildTo(light_parent, new pbge::ModelInstance(bezier));

        pbge::Node * circle_parent = scene->appendChildTo(light_parent, pbge::TransformationNode::translation(1, 1, 0));

        pbge::VBOModel * circle = pbge::Geometrics::createCircle(1.0f, 100, ogl);
        scene->appendChildTo(circle_parent, new pbge::ModelInstance(circle));
        scene->appendChildTo(circle_parent, createEllipse(circle, 1.0f, 0.2f));

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

        TensorModel * model = new TensorModel(tensor, 2, 50);
        scene->appendChildTo(circle_parent, new pbge::ModelInstance(model));
        cam_node_name = scene->appendChildTo(pbge::SceneGraph::ROOT, pbge::TransformationNode::translation(0.0f, 1.0f, 5.0f))->getSceneGraphIndex();
        dynamic_cast<pbge::Light*>(scene->appendChildTo(cam_node_name, new pbge::PointLight))->setDiffuseColor(0,1,1,1);
        scene->appendChildTo(child, vboModel);
        pbge::Node * sphereParent = scene->appendChildTo(child, pbge::TransformationNode::translation(-1.5f, 0.0f, 0.0f));
        scene->appendChildTo(sphereParent, new pbge::ModelInstance(new TensorModel(tensor3d, 3, 20)));
        pbge::CameraNode * cam = dynamic_cast<pbge::CameraNode*>(scene->appendChildTo(cam_node_name, new pbge::CameraNode()));
        cam->lookAt(math3d::vector4(0,1,0), math3d::vector4(0,0,-1));
        cam->setPerspective(45, 1, 1.0f, 10);

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