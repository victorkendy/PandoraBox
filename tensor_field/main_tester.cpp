#include <iostream>
#include <vector>
#include "pbge/pbge.h"

#include "vbo_setup.h"
#include "TensorModel.h"
#include "Ellipses.h"
#include "Ellipsoids.h"
#include "FieldReader.h"

class CustomKeyboardEventHandler : public pbge::KeyboardEventHandler {
public:
    CustomKeyboardEventHandler(pbge::SceneGraph * graph, int cam_name) {
        this->cam_node = dynamic_cast<pbge::TransformationNode*>(graph->getGraphNode(cam_name));
    }

    bool keyDown(char key) {
        math3d::matrix44 m = cam_node->getTransformationMatrix();
        switch(key) {
            case 'W': cam_node->translate(0, 0.1f, 0); break;
            case 'S': cam_node->translate(0, -0.1f, 0); break;
            case 'A': cam_node->translate(-0.1f, 0, 0); break;
            case 'D': cam_node->translate(0.1f, 0, 0); break;
            case 'Q': cam_node->translate(0, 0, 0.1f); break;
            case 'E': cam_node->translate(0, 0, -0.1f); break;
        }
        return true;
    }

    bool keyUp(char key) {
        return true;
    }
private:
    pbge::TransformationNode * cam_node;
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
    pbge::SceneGraph * operator () (pbge::GraphicAPI * gfx, pbge::Window * window) {
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
        cam->setPerspective(45, 1, 1.0f, 10);

		window->getEventHandler()->addKeyboardHandler(new CustomKeyboardEventHandler(scene, cam_trans_node->getSceneGraphIndex()));
		window->getEventHandler()->addMouseHandler(new CustomMouseEventHandler(scene, cam_rot_node->getSceneGraphIndex()));
        
        pbge::Texture1D * tex = gfx->getFactory()->create1DTexture();

        unsigned char data[] = {0,0,255, 255,0,0, 0,255,0, 255,255,0, 0,255,255,
                                255,0,255, 255,255,255, 0,0,0, 128,128,128, 255,255,128};
        tex->setImageData(pbge::Texture::UNSIGNED_BYTE, pbge::Texture::RGB, data, sizeof(data), 10, pbge::Texture::RGB);
        tex->bindTextureOn(gfx->getState()->chooseTexUnit(tex));
        return scene;
    }
private:
    void createSceneModels(pbge::SceneGraph * graph, pbge::GraphicAPI * gfx) {
        AnalyzeReader aReader(gfx);
        aReader.loadField("./Resources/dhelix_dti_tensor");
        aReader.generateFieldOn(graph, light_parent);
		
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
        //graph->appendChildTo(child, vboModel);

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
        spheres->setNumberOfInstances(5);
        //graph->appendChildTo(sphereParent, spheres);

        Ellipses ellipses(gfx);
        pbge::ModelCollection * ellipsesCollection = ellipses.createEllipses(1)->addTransform(math3d::translationMatrix(-1, 1, 1))->done(gfx);
        //graph->appendChildTo(sphereParent, ellipsesCollection);

		Ellipsoids ellipsoids(gfx);
        pbge::ModelCollection * ellipsoidsCollection = ellipsoids.createEllipsoids(1)->addTransform(math3d::scaleMatrix(1, 0.5f, 0.2f)*math3d::translationMatrix(1,1,1))->done(gfx);
		//graph->appendChildTo(sphereParent, ellipsoidsCollection);
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
        
        light_parent = pbge::TransformationNode::translation(0.0f, 1.0f, 0.0f);
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



int main(int argc, char ** argv) {
    pbge::Manager * manager = new pbge::Manager;
    manager->setWindowDimensions(500, 500);
    manager->setFullscreen(false);
    manager->setWindowTitle("tensor_field");
    manager->setSceneInitializer(new CustomSceneInitializer);
    manager->printDebugInformation(true);
    manager->displayGraphics();
    delete manager;
    return 0;
}