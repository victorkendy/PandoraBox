

#ifndef gfxmodel_h_
#define gfxmodel_h_

#include "pbge/core/core.h"
#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/Node.h"
#include "math3d/math3d.h"

namespace pbge {
    class VertexBuffer;
    class ModelInstance;
    class GPUProgram;
    class UniformSet;

    class PBGE_EXPORT Model{
    public:
        virtual void beforeRender(GraphicAPI * gfx) = 0;

        virtual void afterRender(GraphicAPI * gfx) = 0;

        virtual void render(GraphicAPI * gfx)=0;

        virtual void renderDepth(GraphicAPI * gfx)=0;
    };

    class PBGE_EXPORT VBOModel : public Model {
    public:
        VBOModel(VertexBuffer * vbo, GLenum primitive);

        void beforeRender(GraphicAPI * gfx);

        void afterRender(GraphicAPI * gfx);

        void render(GraphicAPI * gfx);

        void renderDepth(GraphicAPI * gfx);

        GLenum getPrimitive() {
            return primitive;
        }

        VertexBuffer * getVBO() {
            return vbo;
        }
    private:
        VertexBuffer * vbo;
        GLenum primitive;
    };

    class PBGE_EXPORT ModelInstance : public Node {
    public:
        ModelInstance();

        ModelInstance(Model * _model);

        ~ModelInstance();
    // Node interface methods
    public:
        void updatePass(UpdaterVisitor * visitor, GraphicAPI * ogl) {}

        void postUpdatePass(UpdaterVisitor * visitor, GraphicAPI * ogl) {}

        void renderPass(RenderVisitor * visitor, GraphicAPI * ogl);

        void postRenderPass(RenderVisitor * visitor, GraphicAPI * ogl);

        void depthPass(RenderVisitor * visitor, GraphicAPI * ogl);

        void postDepthPass(RenderVisitor * visitor, GraphicAPI * ogl);

        void addChild(Node * node) {
            childs.push_back(node);
        }

        node_list & getChildren() {
            return childs;
        }
    // Model Instance methods
    public:
        Model * getModel() {
            return model;
        }

        void setDepthPassProgram(GPUProgram * depthPassProgram) {
            depthProgram = depthPassProgram;
        }

        void setRenderPassProgram(GPUProgram * renderPassProgram) {
            renderProgram = renderPassProgram;
        }

        UniformSet * getUniformSet() {
            return uniforms;
        }

    private:
        Model * model;

        node_list childs;

        UniformSet * uniforms;

        GPUProgram * depthProgram, * renderProgram;
    };

    class PBGE_EXPORT ModelCollection : public Node {
    public:
        ModelCollection();

        ModelCollection(Model * _model);

        ~ModelCollection();

        void setNumberOfInstances(int instances) {
            this->numberOfInstances = instances;
        }
    // Node interface methods
    public:
        void updatePass(UpdaterVisitor * visitor, GraphicAPI * ogl) {}

        void postUpdatePass(UpdaterVisitor * visitor, GraphicAPI * ogl) {}

        void renderPass(RenderVisitor * visitor, GraphicAPI * ogl);

        void postRenderPass(RenderVisitor * visitor, GraphicAPI * ogl);

        void depthPass(RenderVisitor * visitor, GraphicAPI * ogl);

        void postDepthPass(RenderVisitor * visitor, GraphicAPI * ogl);

        void addChild(Node * node) {
            childs.push_back(node);
        }

        node_list & getChildren() {
            return childs;
        }
    // Model Instance methods
    public:
        Model * getModel() {
            return model;
        }

        void setDepthPassProgram(GPUProgram * depthPassProgram) {
            depthProgram = depthPassProgram;
        }

        void setRenderPassProgram(GPUProgram * renderPassProgram) {
            renderProgram = renderPassProgram;
        }

        UniformSet * getUniformSet() {
            return uniforms;
        }

    private:
        Model * model;

        node_list childs;

        UniformSet * uniforms;

        GPUProgram * depthProgram, * renderProgram;

        int numberOfInstances;
    };
}
#endif
