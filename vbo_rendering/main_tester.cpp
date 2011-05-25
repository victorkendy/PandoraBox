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
        // FIXME: remove the state change line
        gfx->enableMode(pbge::GraphicAPI::DEPTH_TEST);
        scene = new pbge::SceneGraph(new pbge::TransformationNode);
        createSceneTransformations(scene);
        createSceneLights(scene);
        createSceneModels(scene, gfx);
        cam_node_name = cam_node->getSceneGraphIndex();
        pbge::CameraNode * cam = dynamic_cast<pbge::CameraNode*>(scene->appendChildTo(cam_node_name, new pbge::CameraNode()));
        cam->lookAt(math3d::vector4(0,1,0), math3d::vector4(0,0,-1));
        cam->setPerspective(45, 1, 1.0f, 10);
        return scene;
    }
private:
    void createSceneModels(pbge::SceneGraph * graph, pbge::GraphicAPI * gfx) {
        pbge::VBOModel * bezier = pbge::Geometrics::createBezier(
            math3d::vector4(-1,0,0,1), math3d::vector4(2,-2,0,1), 
            math3d::vector4(-2,-2,0,1), math3d::vector4(1,0,0,1), 100, gfx);
        graph->appendChildTo(light_parent, new pbge::ModelInstance(bezier));

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
        graph->appendChildTo(circle_parent, circle_instance);

        pbge::ModelInstance * vboModel = createVBOInstance(gfx);
        graph->appendChildTo(child, vboModel);

        pbge::Model * sphereModel = pbge::Geometrics::createSphere(1.0f, 100, gfx);
        pbge::ModelCollection * spheres = new pbge::ModelCollection(sphereModel);
        spheres->setRenderPassProgram(gfx->getFactory()->createProgramFromString(
            "#extension GL_ARB_draw_instanced: enable\n"
            "void main() {\n"
            "   gl_Position = ftransform() + gl_InstanceIDARB;\n"
            "   gl_FrontColor = vec4(1,1,1,1);\n"
            "}", ""));
        spheres->setDepthPassProgram(gfx->getFactory()->createProgramFromString(
            "#extension GL_ARB_draw_instanced: enable\n"
            "void main() {\n"
            "   gl_Position = ftransform() + gl_InstanceIDARB;\n"
            "}", ""));
        graph->appendChildTo(sphereParent, spheres);
    }

    void createSceneLights(pbge::SceneGraph * graph) {
        pbge::Light * light1 = new pbge::PointLight;
        light1->setDiffuseColor(1,0,0,1);
        graph->appendChildTo(light_parent, light1);

        pbge::Light* light2 = new pbge::PointLight;
        light2->setDiffuseColor(0,1,1,1);
        scene->appendChildTo(cam_node_name, light2);
    }

    void createSceneTransformations(pbge::SceneGraph * graph) {
        child = pbge::TransformationNode::rotation(pbge::degToRad(60), 0,0,20)->scale(0.5f, 0.5f, 0.5f);
        graph->appendChildTo(pbge::SceneGraph::ROOT, child);
        
        light_parent = pbge::TransformationNode::translation(0.0f, 1.0f, 0.0f);
        graph->appendChildTo(pbge::SceneGraph::ROOT, light_parent);

        circle_parent = graph->appendChildTo(light_parent, pbge::TransformationNode::translation(1, 1, 0));

        cam_node = graph->appendChildTo(pbge::SceneGraph::ROOT, pbge::TransformationNode::translation(0.0f, 1.0f, 5.0f));

        sphereParent = graph->appendChildTo(child, pbge::TransformationNode::translation(-1.5f, 0.0f, 0.0f));
    }
    pbge::Node * light_parent;
    pbge::Node * child;
    pbge::Node * circle_parent;
    pbge::Node * cam_node;
    pbge::Node * sphereParent;
};

class CustomKeyboardEventHandler : public pbge::KeyboardEventHandler {
public:
    bool keyDown(char key) {
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
        return true;
    }

    bool keyUp(char key) {
        return true;
    }
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