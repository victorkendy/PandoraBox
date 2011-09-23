#include <map>

#include "DepthPeelingProcessor.h"
#include "DepthPeelingVisitor.h"
#include "pbge/pbge.h"

void DepthPeelingProcessor::process(pbge::GraphicAPI * gfx, pbge::Renderer * renderer) {
    std::map<std::string, pbge::Texture2D*> & renderables = renderer->getRenderables();
    pbge::FramebufferObject * fbo = renderer->getFramebufferObject();
    
    pbge::UniformSampler2D* sampler = 
            dynamic_cast<pbge::UniformSampler2D*>(gfx->getUniformValue(pbge::UniformInfo("depth", pbge::SAMPLER_2D)));
    
    DepthPeelingVisitor visitor;
    pbge::Texture2D * colorAux = renderables["color_aux"];
    pbge::Texture2D * color = renderables["color"];
    pbge::Texture2D * auxBuffer = renderables["depth_aux"];
    pbge::Texture2D * depthBuffer = renderables["depth"];
    pbge::DepthBufferController * depth = gfx->depthBufferController();
    pbge::BlendController * blend = gfx->getBlendController();
    for(int i = 0; i < 3; i++) {
        fbo->setDepthRenderable(auxBuffer);
        fbo->removeRenderable("color");
        fbo->addRenderable(colorAux, "color");
        sampler->setValue(depthBuffer);
        fbo->update(gfx);
        gfx->clear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        visitor.visit(renderer->getScene()->getSceneGraphRoot(), gfx);
        fbo->removeRenderable("color");
        fbo->addRenderable(color, "color");
        depth->disableDepthTest();
        depth->disableDepthWrite();
        blend->enableBlending();
        blend->useBlendFunc(pbge::BlendController::BLEND_FACTOR_ONE, pbge::BlendController::BLEND_FACTOR_ONE);
        renderer->renderScreenQuad(blitProgram.get());
        blend->disableBlending();

        pbge::Texture2D * aux = auxBuffer;
        auxBuffer = depthBuffer;
        depthBuffer = aux;
    }
}

void DepthPeelingProcessor::initialize(pbge::GraphicAPI *gfx, pbge::Renderer *renderer) {
    std::map<std::string, pbge::Texture2D*> & renderables = renderer->getRenderables();
    if(renderables.find("depth_aux") == renderables.end()) {
        pbge::Texture2D * depthAux = gfx->getFactory()->create2DTexture();
        renderables["depth_aux"] = depthAux;
    }
    if(renderables.find("color_aux") == renderables.end()) {
        pbge::Texture2D * colorAux = gfx->getFactory()->create2DTexture();
        renderables["color_aux"] = colorAux;
    }
    blitProgram.reset(gfx->getFactory()->createProgramFromString(
        "varying vec2 position;\n"
        "void main() {\n"
        "   gl_Position=gl_Vertex;\n"
        "   position = 0.5 * gl_Vertex.xy + 0.5;"
        "}", 
        "varying vec2 position;\n"
        "uniform sampler2D color;\n"
        "void main(){\n"
		"   gl_FragColor = texture2D(color, position.xy);\n"
        "}"));
    initialized = true;
}