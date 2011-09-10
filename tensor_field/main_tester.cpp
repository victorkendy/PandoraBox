#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include "pbge/pbge.h"
#include "CompiledFieldReader.h"

#define BRAIN_CTF "./Resources/dti_axial_6dir_dti_tensor.ctf"
#define DHELIX_CTF "./Resources/dhelix_dti_tensor.ctf"

class CustomKeyboardEventHandler : public pbge::KeyboardEventHandler {
public:
    CustomKeyboardEventHandler(pbge::SceneGraph * graph, int cam_name) {
        this->cam_node = dynamic_cast<pbge::TransformationNode*>(graph->getGraphNode(cam_name));
    }

    bool keyDown(char key) {
        math3d::matrix44 m = cam_node->getTransformationMatrix();
        switch(key) {
            case 'W': cam_node->translate(0, 1.0f, 0); break;
            case 'S': cam_node->translate(0, -1.0f, 0); break;
            case 'A': cam_node->translate(-1.0f, 0, 0); break;
            case 'D': cam_node->translate(1.0f, 0, 0); break;
            case 'Q': cam_node->translate(0, 0, 1.0f); break;
            case 'E': cam_node->translate(0, 0, -1.0f); break;
        }
        return true;
    }

    bool keyUp(char key) {
        return true;
    }
private:
    pbge::TransformationNode * cam_node;
};

class EffectToggler : public pbge::KeyboardEventHandler {
public:
    EffectToggler(pbge::FramebufferImageProcessor * _inversor,
        pbge::FramebufferImageProcessor * _redder) {
        inversor = _inversor;
        redder = _redder;
    }
    bool keyDown(char key) {
        return false;
    }
    bool keyUp(char key) {
        if(key == '1') {
            inversor->toggle();
            return true;
        } else if (key == '2') {
            redder->toggle();
            return true;
        }
        return false;
    }
private:
    pbge::FramebufferImageProcessor * inversor;
    pbge::FramebufferImageProcessor * redder;
};

class CustomMouseEventHandler : public pbge::MouseEventHandler {
public:
    CustomMouseEventHandler(pbge::SceneGraph * graph, int cam_name) {
        this->cam_node = dynamic_cast<pbge::TransformationNode*>(graph->getGraphNode(cam_name));
		leftButtonDown = false;
        rightButtonDown = false;
        prev_x = 0;
        prev_y = 0;
		vertical_rotation = 0;
		horizontal_rotation = 0;
		degreeScale = 0.03f;
    }

    bool buttonDown(pbge::MouseButton button, int x, int y) {
        if(button == pbge::L_MOUSE_BUTTON) {
            leftButtonDown = true;
            prev_x = x;
            prev_y = y;
        }
        else if(button == pbge::R_MOUSE_BUTTON) rightButtonDown = true;
		return true;
    }
    bool buttonUp(pbge::MouseButton button, int x, int y) {
        if(button == pbge::L_MOUSE_BUTTON) {
            leftButtonDown = false;
            horizontal_rotation += (prev_x - x) * degreeScale;
			vertical_rotation += (prev_y - y) * degreeScale;
			prev_x = 0;
            prev_y = 0;
        }
        else if(button == pbge::R_MOUSE_BUTTON) rightButtonDown = false;
		return true;
    }
    bool move(int x, int y) {
        if(leftButtonDown) {
			math3d::matrix44 rotation = math3d::rotationMatrix(horizontal_rotation + (prev_x - x) * degreeScale, 0, 1.0f, 0);
            rotation = math3d::rotationMatrix(vertical_rotation + (prev_y - y) * degreeScale, 1.0f, 0, 0) * rotation;
            cam_node->setTransformationMatrix(rotation);
        }
		return true;
    }
private:
    bool leftButtonDown;
    bool rightButtonDown;
    int prev_x;
    int prev_y;
	float vertical_rotation;
	float horizontal_rotation;
	float degreeScale;
    pbge::TransformationNode * cam_node;
};

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

std::string choose_field(int argc, char ** argv) {
    int option;
    bool option_chosen = false;
    
    switch(argc) {
        case 1:
            while(!option_chosen) {
                std::cout << "Choose the field to be read:" << std::endl
                      << " (1) - Double helix" << std::endl 
                      << " (2) - Brain" << std::endl << std::endl;
                scanf("%d", &option);
                option_chosen = true;
                switch(option) {
                    case 1:
                        return std::string(DHELIX_CTF);
                    case 2:
                        return std::string(BRAIN_CTF);
                    default:
                        option_chosen = false;
                        std::cout << "Invalid option. Choose either 1 or 2." << std::endl;
                        break;
                }
            }
            break;
        case 2:
            return argv[1];
        default:
            std::cout << "Illegal command line arguments" << std::endl;
            exit(1);
    }
    std::cout << "Illegal command line arguments" << std::endl;
    exit(1);
    return std::string("");
}

int main(int argc, char ** argv) {
    pbge::Manager * manager = new pbge::Manager;
    manager->setWindowDimensions(1024, 768);
    manager->setFullscreen(false);
    manager->setWindowTitle("tensor_field");
    manager->setSceneInitializer(new CustomSceneInitializer(choose_field(argc, argv)));
    manager->printDebugInformation(true);
    manager->displayGraphics();
    delete manager;
    return 0;
}