
#ifndef PBGE_GFX_LIGHT_H_
#define PBGE_GFX_LIGHT_H_

#include "math3d/vector4.h"

#include "pbge/gfx/Node.h"

namespace pbge {
    
    class Shader;
    class GPUProgram;
    class UpdaterVisitor;
    class RenderVisitor;
    class Texture2D;

    class PBGE_EXPORT Light : public Node {
    public:
        virtual void turnOn() = 0;

        virtual void turnOff() = 0;

        virtual bool isLightOn() = 0;

        virtual GPUProgram * getLightPassProgram(GraphicAPI * ogl) = 0;

        virtual void setNecessaryUniforms(GraphicAPI * ogl, const math3d::matrix44 & viewTransform) = 0;

        virtual void setSpecularColor(const float & red, const float & green, const float & blue, const float & alpha) = 0;

        virtual void setDiffuseColor(const float & red, const float & green, const float & blue, const float & alpha) = 0;
    };

    class PBGE_EXPORT PointLight : public Light {
    public: // the node interface methods
        void updatePass(UpdaterVisitor * visitor, GraphicAPI * ogl);

        void postUpdatePass(UpdaterVisitor * visitor, GraphicAPI * ogl) {}

        void renderPass(RenderVisitor * visitor, GraphicAPI * ogl){}

        void postRenderPass(RenderVisitor * visitor, GraphicAPI * ogl){}

        void depthPass(RenderVisitor * visitor, GraphicAPI * ogl) {}

        void postDepthPass(RenderVisitor * visitor, GraphicAPI * ogl) {}

        void addChild(Node * node) {
            childs.push_back(node);
        }
        
        node_list & getChilds() {
            return childs;
        }

    public:
        // Light specific methods
        PointLight();

        void turnOn() {
            on = true;
        }

        void turnOff() {
            on = false;
        }

        bool isLightOn() {
            return on;
        }

        math3d::vector4 & getPosition() {
            return *position;
        }

        GPUProgram * getLightPassProgram(GraphicAPI * ogl) {
            if(program == NULL)
                program = PointLight::getDefaultLightPassProgram(ogl);
            return program;
        }

        void setNecessaryUniforms(GraphicAPI * ogl, const math3d::matrix44 & viewTransform);

        void setSpecularColor(const float & red, const float & green, const float & blue, const float & alpha) {}

        void setDiffuseColor(const float & red, const float & green, const float & blue, const float & alpha) {
            diffuseColor[0] = red;
            diffuseColor[1] = green;
            diffuseColor[2] = blue;
            diffuseColor[3] = alpha;
        }

    public:
        static Shader * getDefaultLightPassVS(GraphicAPI * ogl);

        static Shader * getDefaultLightPassFS(GraphicAPI * ogl);

        static GPUProgram * getDefaultLightPassProgram(GraphicAPI * ogl);

    private:
        math3d::vector4 * position;

        float diffuseColor[4];

        GPUProgram * program;

        node_list childs;

        Texture2D * texture;

        bool on;
    };
}

#endif
