#ifndef TENSOR_FIELD_SCENEINITIALIZATION_H
#define TENSOR_FIELD_SCENEINITIALIZATION_H

#include "pbge/pbge.h"
#include "ImageProcessors.h"
#include "DepthPeelingProcessor.h"
#include "PeelingAwareNode.h"

class CustomSceneInitializer : public pbge::SceneInitializer {

public:
    CustomSceneInitializer(std::string _filename) : filename(_filename) {}
    
    pbge::SceneGraph * operator () (pbge::GraphicAPI * gfx, pbge::Window * window) {
        pbge::FramebufferImageProcessor * inversor = colorInversor();
        pbge::FramebufferImageProcessor * redder = chooseRed();
        pbge::FramebufferImageProcessor * lens = senoidalLens();
        DepthPeelingProcessor * depthPeeling = new DepthPeelingProcessor;
        window->getRenderer()->addSceneProcessor(new pbge::RenderPassProcessor);
        window->getRenderer()->addSceneProcessor(depthPeeling);
        window->getRenderer()->addPostProcessor(inversor);
        window->getRenderer()->addPostProcessor(redder);
        window->getRenderer()->addPostProcessor(lens);
        window->getRenderer()->addPostProcessor(new pbge::BlitToFramebuffer);
        pbge::SceneGraph * scene;
        int cam_node_name;
        scene = new pbge::SceneGraph(new pbge::TransformationNode);
        loadField(gfx);
        createSceneTransformations(scene);
        createSceneLights(scene, cam_node->getSceneGraphIndex());
        createSceneModels(scene, gfx);
        cam_node_name = cam_node->getSceneGraphIndex();
        pbge::CameraNode * cam = dynamic_cast<pbge::CameraNode*>(scene->appendChildTo(cam_node_name, new pbge::CameraNode()));
        cam->lookAt(math3d::vector4(0,1,0), math3d::vector4(0,0,-1));
        cam->setPerspective(90.0f, 1.0f, 1.0f, 200.0f);
        window->getEventHandler()->addKeyboardHandler(new EffectToggler(inversor, redder, lens, depthPeeling, fieldParent));
		window->getEventHandler()->addKeyboardHandler(new CustomKeyboardEventHandler(scene, cam_trans_node->getSceneGraphIndex(), fieldParent));
		window->getEventHandler()->addMouseHandler(new CustomMouseEventHandler(scene, cam_rot_node->getSceneGraphIndex()));
        return scene;
    }
private:
    void loadField(pbge::GraphicAPI * gfx) {
        CompiledFieldReader reader;
        reader.read(filename.c_str());
        this->fieldParent = reader.generateField(gfx);
    }

    void createSceneModels(pbge::SceneGraph * graph, pbge::GraphicAPI * gfx) {
        light_parent->addChild(this->fieldParent);
        
        float * dim = fieldParent->getDim();
        pbge::VertexBufferBuilder builder(6);
        pbge::VertexAttribBuilder vertex = builder.addAttrib(4, pbge::VertexAttrib::VERTEX);
        std::vector<unsigned short> v_indices;
        for(int i = 0; i < 6; i++) {
            v_indices.push_back(i);
        }
        builder.on(vertex).pushValue(-dim[0]/2, 0, 0, 1)
                          .pushValue(dim[0]/2, 0, 0, 1)
                          .pushValue(0, -dim[1]/2, 0, 1)
                          .pushValue(0, dim[1]/2, 0, 1)
                          .pushValue(0, 0, -dim[2]/2, 1)
                          .pushValue(0, 0, dim[2]/2, 1);
        pbge::ModelInstance * grid = new pbge::ModelInstance(
            new pbge::VBOModel(builder.done(pbge::Buffer::STATIC_DRAW, gfx), GL_LINES));
        pbge::GPUProgram * grid_program = gfx->getFactory()->createProgramFromString(
            "in vec4 pbge_Vertex;\n"
            "uniform mat4 pbge_ModelViewProjectionMatrix;\n"
            "void main() {\n"
            "   gl_Position = pbge_ModelViewProjectionMatrix * pbge_Vertex;\n"
            "}",
            "void main() {\n"
            "   gl_FragColor = vec4(1,0,0,1);\n"
            "}"
            );
        grid->setRenderPassProgram(grid_program);
        light_parent->addChild(grid);
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

        cam_node = graph->appendChildTo(cam_trans_node, pbge::TransformationNode::translation(0.0f, 0.0f, fieldParent->getDim()[2]));

        sphereParent = graph->appendChildTo(child, pbge::TransformationNode::translation(-1.5f, 0.0f, 0.0f));
    }

    std::string filename;
    FieldParent * fieldParent;
    pbge::Node * light_parent;
    pbge::Node * child;
    pbge::Node * circle_parent;
    pbge::Node * cam_trans_node;
	pbge::Node * cam_rot_node;
	pbge::Node * cam_node;
    pbge::Node * sphereParent;
};

#endif
