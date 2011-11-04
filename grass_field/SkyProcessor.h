#include "pbge/pbge.h"
#include "DevilImage.h"
#include "Timer.h"

class SkyProcessor : public pbge::SceneProcessor {
public:
    SkyProcessor():uniforms(new pbge::UniformSet), skyBlitter(NULL){
        sampler = uniforms->getSampler2D("skyTex");
        time = uniforms->getFloat("time");
    }
    ~SkyProcessor() {
        delete uniforms;
        delete skyBlitter;
        delete skyTex;
    }
    void initialize(pbge::GraphicAPI * gfx, pbge::Renderer * renderer){
        skyBlitter = gfx->getFactory()->createProgramFromString(
            "#version 150\n"
            "in vec4 pbge_Vertex;\n"
            "out vec2 position;\n"
            "uniform float time;\n"
            "void main(){\n"
            "   gl_Position = pbge_Vertex;\n"
            "   position = 0.5 * pbge_Vertex.xy + 0.5 + 0.001 * time;\n"
            "}\n",
            "#version 150\n"
            "in vec2 position;\n"
            "uniform sampler2D skyTex;\n"
            "void main() {\n"
            "   gl_FragDepth = 1.0;\n"
            "   gl_FragColor = vec4((texture2D(skyTex, position)).rgb + 0.1, 1.0);\n"
            "}\n");
        skyTex = gfx->getFactory()->create2DTexture();
        DevilImage image("./Resources/sky.jpg");
        skyTex->setImage(&image, pbge::Texture::RGBA);
        sampler->setValue(skyTex);
        time->setValue(timer.getElapsedTime());
    }
    void process(pbge::GraphicAPI * gfx, pbge::Renderer * renderer, pbge::Camera * camera) {
        time->setValue(timer.getElapsedTime());
        gfx->pushUniforms(uniforms);
        renderer->renderScreenQuad(skyBlitter);
        gfx->popUniforms();
    }
    bool isInitialized(pbge::GraphicAPI * gfx) {
        return skyBlitter != NULL;
    }
    bool isActive() {
        return true;
    }
private:
    pbge::UniformSet * uniforms;
    pbge::UniformSampler2D * sampler;
    pbge::UniformFloat * time;
    pbge::GPUProgram * skyBlitter;
    pbge::Texture2D * skyTex;
    Timer timer;
};