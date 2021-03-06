#include "math3d/math3d.h"
#include "pbge/pbge.h"
#include "GrassField.h"
#include "SkyProcessor.h"
#include "Timer.h"

class GrassFieldBuilder : public pbge::SceneInitializer {
public:
    pbge::SceneGraph * operator () (pbge::GraphicAPI * gfx, pbge::Window * window) {
        pbge::Renderer * renderer = window->getRenderer();
        renderer->addSceneProcessor(new pbge::RenderPassProcessor);
        renderer->addSceneProcessor(new SkyProcessor);
        renderer->addPostProcessor(new pbge::BlitToFramebuffer);
        pbge::Node * root = new pbge::TransformationNode;
        pbge::SceneGraph * graph = new pbge::SceneGraph(root);
        
        configureCamera(root, gfx);
        createGrassField(root, gfx);

        return graph;
    }

private:
    void configureCamera(pbge::Node * parent, pbge::GraphicAPI * gfx) {
        pbge::TransformationNode * cameraParent = 
            pbge::TransformationNode::translation(0, 4, 10);
        pbge::CameraNode * camera = new pbge::CameraNode;
        camera->lookAt(math3d::vector4(0,1,0), math3d::vector4(0, -1, -5));
        camera->setPerspective(90, 1.0f, 2.0f, 30.0f);
        cameraParent->addChild(camera);
        parent->addChild(cameraParent);
    }
    void createGrassField(pbge::Node * parent, pbge::GraphicAPI * gfx) {
        GrassField grass(gfx);
        Timer * timer = new Timer();
        parent->addChild(timer);
        timer->addChild(grass.getField());
        parent->addChild(grass.getGround());
    }
};

int main() {
    pbge::Manager manager;
    GrassFieldBuilder grassBuilder;
    manager.setWindowTitle("grass field demo");
    manager.setWindowDimensions(1024, 768);
    manager.setSceneInitializer(&grassBuilder);
    manager.displayGraphics();
    return 0;
}