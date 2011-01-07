

#ifndef PBGE_GFX_RENDERER_H_
#define PBGE_GFX_RENDERER_H_

#include <vector>
#include <GL/glew.h>

#include "pbge/core/Object.h"
#include "pbge/gfx/OpenGL.h"
#include "pbge/gfx/SceneGraph.h"
#include "pbge/gfx/Node.h"
#include "pbge/gfx/Camera.h"

namespace pbge {
    class PBGE_EXPORT BasicRendererVisitor : public NodeVisitor {
    public:
        BasicRendererVisitor(OpenGL * _ogl) : ogl(_ogl){}
        virtual void visit(Node * node) {
            camera->setCamera(ogl);
            ogl->uploadProjection();
            NodeVisitor::visit(node);
            camera->unsetCamera(ogl);
        }

        virtual void doVisit(Node * node) {
            Node::model_list::iterator cur_model;
            Node::model_list * models = node->getModelList();
            if(models != NULL && models->size() > 0) {
                math3d::matrix44 * modelMatrix = node->getTransformationMatrix();
                ogl->loadModelMatrix(*modelMatrix);
                ogl->updateState();
                for(cur_model = models->begin(); cur_model != models->end(); cur_model++) {
                    (*cur_model)->render(ogl);
                }
            }
        }

        void setCamera(Camera * _camera) {
            camera = _camera;
        }
    private:
        OpenGL * ogl;
        Camera * camera;
    };

    class PBGE_EXPORT Renderer: public Object {
    public:
        Renderer(OpenGL * _ogl);
        void setScene(const SceneManager * scene_manager);
        SceneManager * getScene();
        void updateScene();
        void render();
    private:
        SceneManager * scene;
        UpdaterVisitor * updater;
        BasicRendererVisitor * renderer;
        //TextureRenderer * textureRenderer;
        OpenGL * ogl;
    };
}
#endif
