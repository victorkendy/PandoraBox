

#ifndef gfxmodel_h_
#define gfxmodel_h_

#include "pbge/core/core.h"
#include "pbge/gfx/OpenGL.h"
#include "pbge/gfx/Node.h"
#include "math3d/math3d.h"

namespace pbge {
    class VertexBuffer;
    class ModelInstance;
    class GPUProgram;
    class UniformSet;

    class PBGE_EXPORT Model{
    public:
        virtual void render(ModelInstance * instance, OpenGL * ogl)=0;

        virtual void renderDepth(ModelInstance * instance, OpenGL * ogl)=0;
    };

    class PBGE_EXPORT VBOModel : public Model {
    public:
        VBOModel(VertexBuffer * vbo, GLenum primitive);

        void render(ModelInstance * instance, OpenGL * ogl);

        void renderDepth(ModelInstance * instance, OpenGL * ogl);
    private:
        VertexBuffer * vbo;
        GLenum primitive;
    };

    class PBGE_EXPORT BezierCurve : public Model {
    public:
        BezierCurve();
        
        void addControlPoint(const float & x, const float & y, const float & z, const float & w);

        void render(ModelInstance * instance, OpenGL * ogl);

        void renderDepth(ModelInstance * instance, OpenGL * ogl);

        void setEvaluator(GPUProgram * _evaluator) {
            evaluator = _evaluator;
        }

        GPUProgram * getEvaluator(OpenGL * ogl);
    private:
        float * controlPoints;

        GPUProgram * evaluator;

        int currentIndex;
    };

    class PBGE_EXPORT Circle : public Model {
    public:
        Circle(const float & radius, const int & slices = 16);

        void renderDepth(ModelInstance * instance, OpenGL * ogl);

        void render(ModelInstance * instance, OpenGL * ogl);
    private:
        float radius;
        int slices;
    };

    class PBGE_EXPORT Ellipse : public Circle {
    public:
        Ellipse(const float & x_semi_axis, const float & y_semi_axis, const int & slices);

        void render(ModelInstance * instance, OpenGL * ogl);

        void setEvaluator(GPUProgram * _evaluator) {
            evaluator = _evaluator;
        }

        GPUProgram * getEvaluator(OpenGL * ogl);

        void setTransformation(const math3d::matrix44 & transformation);
    private:
        float x_semi_axis;
        float y_semi_axis;
        GPUProgram * evaluator;
        math3d::matrix44 * transformation;
    };

    class PBGE_EXPORT Sphere : public Model {
    public:
        Sphere(const float & radius, const int & slices = 16);

        void renderDepth(ModelInstance * instance, OpenGL * ogl);

        void render(ModelInstance * instance, OpenGL * ogl);
    private:
        float radius;
        int slices;
    };

    class PBGE_EXPORT Ellipsoid : public Sphere {
    public:
        Ellipsoid(const float & x_semi_axis, const float & y_semi_axis, const float & z_semi_axis, const int & slices);

        void render(ModelInstance * instance, OpenGL * ogl);

        void setEvaluator(GPUProgram * _evaluator) {
            evaluator = _evaluator;
        }

        GPUProgram * getEvaluator(OpenGL * ogl);

        void setTransformation(const math3d::matrix44 & transformation);
    private:
        float x_semi_axis;
        float y_semi_axis;
        float z_semi_axis;
        GPUProgram * evaluator;
        math3d::matrix44 * transformation;
    };

    class PBGE_EXPORT ModelInstance : public Node {
    public:
        ModelInstance();

        ModelInstance(Model * _model);

        ~ModelInstance();
    // Node interface methods
    public:
        void updatePass(UpdaterVisitor * visitor, OpenGL * ogl) {}

        void postUpdatePass(UpdaterVisitor * visitor, OpenGL * ogl) {}

        void renderPass(RenderVisitor * visitor, OpenGL * ogl);

        void postRenderPass(RenderVisitor * visitor, OpenGL * ogl);

        void depthPass(RenderVisitor * visitor, OpenGL * ogl);

        void postDepthPass(RenderVisitor * visitor, OpenGL * ogl);

        void addChild(Node * node) {
            childs.push_back(node);
        }

        node_list & getChilds() {
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
}
#endif
