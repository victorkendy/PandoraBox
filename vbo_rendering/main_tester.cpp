#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include "pbge/pbge.h"

#include "vbo_setup.h"
#include "TensorModel.h"
#include "Ellipses.h"

int cam_node_name;
pbge::SceneGraph * scene;

class CustomSceneInitializer : public pbge::SceneInitializer {

public:
    pbge::SceneGraph * operator () (pbge::GraphicAPI * gfx) {
        Ellipses ellipses(gfx);

        const float m_pi = 3.1415f;
        // FIXME: remove the state change line
        gfx->enableMode(pbge::GraphicAPI::DEPTH_TEST);
        pbge::ModelInstance * vboModel = createVBOInstance(gfx);
        scene = new pbge::SceneGraph(new pbge::TransformationNode);
        pbge::Node * child = scene->appendChildTo(pbge::SceneGraph::ROOT, pbge::TransformationNode::rotation(m_pi/3, 0,0,20)->scale(0.5f, 0.5f, 0.5f));
        pbge::Node * light_parent = scene->appendChildTo(pbge::SceneGraph::ROOT, pbge::TransformationNode::translation(0.0f, 1.0f, 0.0f));
        dynamic_cast<pbge::Light*>(scene->appendChildTo(light_parent, new pbge::PointLight))->setDiffuseColor(1,0,0,1);
        
        pbge::VBOModel * bezier = pbge::Geometrics::createBezier(math3d::vector4(-1,0,0,1), math3d::vector4(2,-2,0,1), 
                                                                 math3d::vector4(-2,-2,0,1), math3d::vector4(1,0,0,1), 100, gfx);
        scene->appendChildTo(light_parent, new pbge::ModelInstance(bezier));

        pbge::Node * circle_parent = scene->appendChildTo(light_parent, pbge::TransformationNode::translation(1, 1, 0));

        pbge::VBOModel * circle = pbge::Geometrics::createCircunference(1.0f, 100, gfx);
        pbge::ModelInstance * circle_instance = new pbge::ModelInstance(circle);
        pbge::ShaderHelper * helper = pbge::ShaderHelper::create(gfx)
            ->withVertexShader(
                               "void calculateVertex(out vec4 v, out vec3 n, out vec4 c){\n"
                               "    vec4 vertex = 0.3 * (gl_Vertex);\n"
                               "    v = gl_ModelViewMatrix * vec4(vertex.xyz, 1);\n"
                               "    n = gl_NormalMatrix * gl_Normal;\n"
                               "}\n")
            ->withRenderPassShader(
                "void calculateFragmentColor(out vec4 color){"
                "   color=vec4(1,0,0,1);"
                "}"
            );
        helper->setPrograms(circle_instance);
        scene->appendChildTo(circle_parent, circle_instance);
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
        //scene->appendChildTo(sphereParent, new pbge::ModelInstance(new TensorModel(tensor3d, 3, 20)));
        pbge::Model * sphereModel = pbge::Geometrics::createSphere(1.0f, 100, gfx);
        pbge::ModelCollection * spheres = new pbge::ModelCollection(sphereModel);

        spheres->setRenderPassProgram(gfx->getFactory()->createProgramFromString("#extension GL_ARB_draw_instanced: enable\nvoid main() {gl_Position = ftransform() + gl_InstanceIDARB;gl_FrontColor = vec4(1,1,1,1);}", ""));
        spheres->setDepthPassProgram(gfx->getFactory()->createProgramFromString("#extension GL_ARB_draw_instanced: enable\nvoid main() {gl_Position = ftransform() + gl_InstanceIDARB;}", ""));
        scene->appendChildTo(sphereParent, spheres);
        //pbge::ModelInstance * sphere = new pbge::ModelInstance(pbge::Geometrics::createSphere(1.0f, 100, gfx));
        //sphere->setRenderPassProgram(gfx->getFactory()->createProgramFromString("", "void main() {gl_FragColor = vec4(1,1,1,1);}"));
        //scene->appendChildTo(sphereParent, sphere);
        pbge::CameraNode * cam = dynamic_cast<pbge::CameraNode*>(scene->appendChildTo(cam_node_name, new pbge::CameraNode()));
        cam->lookAt(math3d::vector4(0,1,0), math3d::vector4(0,0,-1));
        cam->setPerspective(45, 1, 1.0f, 10);

        return scene;
    }
};

class CustomKeyboardEventHandler : public pbge::KeyboardEventHandler {
public:
    void keyDown(char key) {
        //pressedKeys.push_back(key);

        pbge::TransformationNode * cam_node = dynamic_cast<pbge::TransformationNode*>(scene->getGraphNode(cam_node_name));
        math3d::matrix44 m = cam_node->getTransformationMatrix();
        switch(key) {
            case 'W': m[1][3] += 0.1f; break;
            case 'S': m[1][3] -= 0.1f; break;
            case 'A': m[0][3] -= 0.1f; break;
            case 'D': m[0][3] += 0.1f; break;
            case 'Q': m[2][3] += 0.1f; break;
            case 'E': m[2][3] -= 0.1f; break;
        }
        cam_node->setTransformationMatrix(m);
    }

    void keyUp(char key) {
        //pressedKeys.erase(std::find(pressedKeys.begin(), pressedKeys.end(), key));
        ;
    }
//private:
//    std::vector<char> pressedKeys;
};

int main(int argc, char ** argv) {
    pbge::Manager * manager = new pbge::Manager;
    manager->setWindowDimensions(500, 500);
    manager->setFullscreen(false);
    manager->setWindowTitle("vbo_rendering");
    manager->setSceneInitializer(new CustomSceneInitializer);
    manager->setKeyboardEventHandler(new CustomKeyboardEventHandler);
    manager->printDebugInformation(true);
    manager->displayGraphics();
    delete manager;
    return 0;
}