#include <map>
#include <GL/glew.h>
#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/Renderer.h"
#include "pbge/gfx/Texture.h"
#include "pbge/gfx/GraphicObjectsFactory.h"
#include "pbge/gfx/ShaderUniform.h"
#include "pbge/gfx/processors/FramebufferImageProcessor.h"

using namespace pbge;

FramebufferImageProcessor::FramebufferImageProcessor(const std::string & fs):fragShader(fs) {
}

void FramebufferImageProcessor::initialize(GraphicAPI * gfx, Renderer * renderer) {
    // treat the uniforms??
    program.reset(gfx->getFactory()->createProgramFromString(
        "varying vec2 position;\n"
        "void main() {\n"
        "   gl_Position=gl_Vertex;\n"
        "   position = 0.5 * gl_Vertex.xy + 0.5;"
        "}", fragShader));
    std::map<std::string, Texture2D*> & renderables = renderer->getRenderables();
    // if it does not have a auxiliary color buffer for ping pong rendering
    // create one
    if(renderables.find("color_aux") == renderables.end()) {
        pbge::Texture2D * auxBuffer = gfx->getFactory()->create2DTexture();
        renderables["color_aux"] = auxBuffer;
    }
}

void FramebufferImageProcessor::process(GraphicAPI *gfx, Renderer *renderer) {
    std::map<std::string, Texture2D*> & renderables = renderer->getRenderables();
    Texture2D * auxBuffer = renderables["color_aux"];
    Texture2D * colorBuffer = renderables["color"];
    renderables["color"] = auxBuffer;
    renderables["color_aux"] = colorBuffer;
    FramebufferObject * fbo = renderer->getFramebufferObject();
    fbo->removeRenderable("color");
    fbo->addRenderable(auxBuffer, "color");
    fbo->update(gfx);
    UniformSampler2D* sampler = 
        dynamic_cast<UniformSampler2D*>(gfx->getUniformValue(UniformInfo("color", pbge::SAMPLER_2D)));
    sampler->setValue(colorBuffer);
    renderer->renderScreenQuad(program.get());
}
