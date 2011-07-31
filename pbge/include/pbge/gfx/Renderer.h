

#ifndef PBGE_GFX_RENDERER_H_
#define PBGE_GFX_RENDERER_H_

#include <map>
#include <string>
#include <boost/smart_ptr/scoped_ptr.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

#include "pbge/core/core.h"
#include "pbge/gfx/Model.h"
#include "pbge/gfx/Shader.h"

namespace pbge {
    class UpdaterVisitor;
    class RenderVisitor;
    class Camera;
    class GraphicAPI;
    class LightPassVisitor;
    class SceneGraph;
    class Texture2D;
    class Node;
    class GPUProgram;
	class FramebufferObject;

    class PBGE_EXPORT Renderer{
    public:
        Renderer(boost::shared_ptr<GraphicAPI> _ogl);

        void setScene(boost::shared_ptr<SceneGraph> & scene_manager);

        SceneGraph * getScene();

        void updateScene();

        void render();
    private:
        void renderWithCamera(Camera * camera, Node * root);

        boost::shared_ptr<SceneGraph> scene;

        boost::scoped_ptr<UpdaterVisitor> updater;

        boost::scoped_ptr<RenderVisitor> renderer;

        boost::scoped_ptr<RenderVisitor> depthRenderer;

        boost::scoped_ptr<LightPassVisitor> lightPassVisitor;

        boost::shared_ptr<GraphicAPI> ogl;

		boost::scoped_ptr<FramebufferObject> fbo;

        boost::scoped_ptr<VBOModel> quad;

        boost::scoped_ptr<GPUProgram> blitter;

        std::map<std::string, Texture2D*> renderables;
    };
}
#endif
