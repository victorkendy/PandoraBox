#include "math3d/math3d.h"
#include "pbge/pbge.h"

class MySceneInitializer : public pbge::SceneInitializer {
public:
    pbge::SceneGraph * operator () (pbge::GraphicAPI * gfx, pbge::Window * window) {
        pbge::Renderer * renderer = window->getRenderer();
        renderer->addSceneProcessor(new pbge::RenderPassProcessor);
        renderer->addPostProcessor(new pbge::BlitToFramebuffer);
        pbge::Node * root = new pbge::TransformationNode;
        pbge::SceneGraph * graph = new pbge::SceneGraph(root);
        
        configureCamera(root, gfx);
        createModel(root, gfx);

        return graph;
    }

private:
    void configureCamera(pbge::Node * parent, pbge::GraphicAPI * gfx) {
        pbge::TransformationNode * cameraParent = 
            pbge::TransformationNode::translation(0, 0, 10);
        pbge::CameraNode * camera = new pbge::CameraNode;
        camera->lookAt(math3d::vector4(0,1,0), math3d::vector4(0, 0, -1));
        camera->setPerspective(90, 1.0f, 2.0f, 30.0f);
        cameraParent->addChild(camera);
        parent->addChild(cameraParent);
    }
    void createModel(pbge::Node * parent, pbge::GraphicAPI * gfx) {
        pbge::VBOModel * sphere = pbge::Geometrics::createSphere(2,100,gfx);
        pbge::ModelInstance * model = new pbge::ModelInstance(sphere);
        pbge::GPUProgram * shader = gfx->getFactory()->createProgramFromString(
            "#version 150\n"
            "in vec4 pbge_Vertex;\n"
            "out vec4 color\n;"
            "uniform mat4 pbge_ModelViewProjectionMatrix;\n"
            "void main() {\n"
            "   mat4 scale = mat4(1,0,0,0,\n"
            "                     0,2,0,0,\n"
            "                     0,0,1,0,\n"
            "                     0,0,0,1);\n"
            "   gl_Position = pbge_ModelViewProjectionMatrix*scale*pbge_Vertex;\n"
            "   color = vec4(pbge_Vertex.xyz, 1);\n"
            "}",
            "in vec4 color;\n"
            "void main() {\n"
            "   gl_FragColor = color;\n"
            "}"
            );
        model->setRenderPassProgram(shader);
        parent->addChild(model);
    }
};

int main() {
    pbge::Manager manager;
    MySceneInitializer sceneInitializer;
    manager.setWindowTitle("Ellipsoid demo");
    manager.setWindowDimensions(1024, 768);
    manager.setSceneInitializer(&sceneInitializer);
    manager.displayGraphics();
    return 0;
}