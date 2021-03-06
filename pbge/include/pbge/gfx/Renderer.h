

#ifndef PBGE_GFX_RENDERER_H_
#define PBGE_GFX_RENDERER_H_

#include <map>
#include <string>
#include <vector>
#include <boost/smart_ptr/scoped_ptr.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

#include "pbge/core/core.h"
#include "pbge/gfx/FramebufferObject.h"
#include "pbge/gfx/NodeVisitors.h"
#include "pbge/gfx/Model.h"
#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/GPUProgram.h"
#include "pbge/gfx/ScenePostProcessor.h"
#include "pbge/gfx/SceneProcessor.h"


namespace pbge {
    class Camera;
    class SceneGraph;
    class Texture2D;
    class Node;
    class GPUProgram;

    class PBGE_EXPORT Renderer{
    public:
        Renderer(boost::shared_ptr<GraphicAPI> _ogl);

        void setScene(boost::shared_ptr<SceneGraph> & scene_manager);

        SceneGraph * getScene();

        void updateScene();

        void render();

        void initialize();

        void addPostProcessor(ScenePostProcessor * processor) {
            postProcessors.push_back(processor);
        }

        void addSceneProcessor(SceneProcessor * processor) {
            sceneProcessors.push_back(processor);
        }

        std::map<std::string, Texture2D*> & getRenderables() {
            return renderables;
        }

        FramebufferObject * getFramebufferObject() {
            return fbo.get();
        }

        void renderScreenQuad(GPUProgram * program);
    private:
        void renderWithCamera(Node * root, Camera * camera);

        boost::shared_ptr<SceneGraph> scene;

        boost::scoped_ptr<UpdaterVisitor> updater;

        boost::shared_ptr<GraphicAPI> ogl;

		boost::scoped_ptr<FramebufferObject> fbo;

        std::map<std::string, Texture2D*> renderables;

        std::vector<ScenePostProcessor*> postProcessors;

        std::vector<SceneProcessor*> sceneProcessors;

        boost::scoped_ptr<VBOModel> quad;
    };
}
#endif
