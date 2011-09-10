#ifndef TENSOR_FIELD_SCENEINITIALIZATION_H
#define TENSOR_FIELD_SCENEINITIALIZATION_H

#include "pbge/pbge.h"

class CustomSceneInitializer : public pbge::SceneInitializer {

public:
    CustomSceneInitializer(std::string _filename) : filename(_filename) {}
    
    pbge::SceneGraph * operator () (pbge::GraphicAPI * gfx, pbge::Window * window) {
        pbge::FramebufferImageProcessor * inversor = new pbge::FramebufferImageProcessor(
            "uniform sampler2D color;\n"
            "varying vec2 position;\n"
            "void main() {\n"
            "   vec3 color = (texture2D(color, position.xy)).rgb;\n"
            "   color = 1 - color;\n"
            "   gl_FragColor = vec4(color, 1);\n"
            "}\n"
        );
        pbge::FramebufferImageProcessor * redder = new pbge::FramebufferImageProcessor(
        "uniform sampler2D color;\n"
            "varying vec2 position;\n"
            "void main() {\n"
            "   float r = (texture2D(color, position.xy)).r;\n"
            "   gl_FragColor = vec4(r, 0, 0, 1);\n"
            "}\n"
        );
        window->getRenderer()->addSceneProcessor(new pbge::RenderPassProcessor);
        window->getRenderer()->addPostProcessor(inversor);
        window->getRenderer()->addPostProcessor(redder);
        window->getRenderer()->addPostProcessor(new pbge::BlitToFramebuffer);
        pbge::SceneGraph * scene;
        int cam_node_name;
        // FIXME: remove the state change line
        gfx->enableMode(pbge::GraphicAPI::DEPTH_TEST);
        scene = new pbge::SceneGraph(new pbge::TransformationNode);
        createSceneTransformations(scene);
        createSceneLights(scene, cam_node->getSceneGraphIndex());
        createSceneModels(scene, gfx);
        cam_node_name = cam_node->getSceneGraphIndex();
        pbge::CameraNode * cam = dynamic_cast<pbge::CameraNode*>(scene->appendChildTo(cam_node_name, new pbge::CameraNode()));
        cam->lookAt(math3d::vector4(0,1,0), math3d::vector4(0,0,-1));
        cam->setPerspective(20.0f, 1.0f, 1.0f, 1000.0f);
        window->getEventHandler()->addKeyboardHandler(new EffectToggler(inversor, redder));
		window->getEventHandler()->addKeyboardHandler(new CustomKeyboardEventHandler(scene, cam_trans_node->getSceneGraphIndex()));
		window->getEventHandler()->addMouseHandler(new CustomMouseEventHandler(scene, cam_rot_node->getSceneGraphIndex()));
        return scene;
    }
private:
    std::string filename;
    void createSceneModels(pbge::SceneGraph * graph, pbge::GraphicAPI * gfx) {
        CompiledFieldReader reader;
        reader.read(filename.c_str());
        reader.generateFieldOn(light_parent, gfx);
    }

    void createSceneLights(pbge::SceneGraph * graph, int cam_node_name) {
        pbge::Light * light1 = new pbge::PointLight;
        light1->setDiffuseColor(1,0,0,1);
        graph->appendChildTo(light_parent, light1);

        pbge::Light* light2 = new pbge::PointLight;
        light2->setDiffuseColor(0,1,1,1);
        graph->appendChildTo(cam_node_name, light2);
    }

    void createSceneTransformations(pbge::SceneGraph * graph) {
        child = pbge::TransformationNode::rotation(pbge::degToRad(60), 0,0,20)->scale(0.5f, 0.5f, 0.5f);
        graph->appendChildTo(pbge::SceneGraph::ROOT, child);
        
        light_parent = pbge::TransformationNode::translation(0.0f, 0.0f, 0.0f);
        graph->appendChildTo(pbge::SceneGraph::ROOT, light_parent);

        circle_parent = graph->appendChildTo(light_parent, pbge::TransformationNode::translation(1, 1, 0)); 

		cam_rot_node = graph->appendChildTo(pbge::SceneGraph::ROOT, pbge::TransformationNode::translation(0.0f, 0.0f, 0.0f));

		cam_trans_node = graph->appendChildTo(cam_rot_node, pbge::TransformationNode::translation(0.0f, 1.0f, 5.0f));

		cam_node = graph->appendChildTo(cam_trans_node, pbge::TransformationNode::translation(0.0f, 0.0f, 0.0f));

        sphereParent = graph->appendChildTo(child, pbge::TransformationNode::translation(-1.5f, 0.0f, 0.0f));
    }
    pbge::Node * light_parent;
    pbge::Node * child;
    pbge::Node * circle_parent;
    pbge::Node * cam_trans_node;
	pbge::Node * cam_rot_node;
	pbge::Node * cam_node;
    pbge::Node * sphereParent;
};

#endif
