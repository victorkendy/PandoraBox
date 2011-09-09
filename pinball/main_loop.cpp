#include "pbge/pbge.h"

#include "ObjLoader.h"

class CustomSceneInitializer : public pbge::SceneInitializer {

public:
    pbge::SceneGraph * operator () (pbge::GraphicAPI * gfx, pbge::Window * window) {
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

		//window->getEventHandler()->addKeyboardHandler(new CustomKeyboardEventHandler(scene, cam_trans_node->getSceneGraphIndex()));
		//window->getEventHandler()->addMouseHandler(new CustomMouseEventHandler(scene, cam_rot_node->getSceneGraphIndex()));
        return scene;
    }
private:
    void createSceneModels(pbge::SceneGraph * graph, pbge::GraphicAPI * gfx) {
        et = LoadObject("./modelos/et.obj");
        graph->appendChildTo(pbge::SceneGraph::ROOT, et);
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
        light_parent = pbge::TransformationNode::translation(0.0f, 0.0f, 0.0f);
        graph->appendChildTo(pbge::SceneGraph::ROOT, light_parent);
        
        cam_rot_node = graph->appendChildTo(pbge::SceneGraph::ROOT, pbge::TransformationNode::translation(0.0f, 0.0f, 0.0f));

		cam_trans_node = graph->appendChildTo(cam_rot_node, pbge::TransformationNode::translation(0.0f, 1.0f, 5.0f));

		cam_node = graph->appendChildTo(cam_trans_node, pbge::TransformationNode::translation(0.0f, 0.0f, 0.0f));
    }
    pbge::Node * et;
    pbge::Node * light_parent;
    pbge::Node * cam_trans_node;
	pbge::Node * cam_rot_node;
	pbge::Node * cam_node;
};

int main() {
    pbge::Manager * manager = new pbge::Manager;
    manager->setWindowDimensions(1024, 768);
    manager->setFullscreen(false);
    manager->setWindowTitle("Pinball");
    manager->setSceneInitializer(new CustomSceneInitializer());
    manager->printDebugInformation(true);
    manager->displayGraphics();
    delete manager;
    return 0;
}