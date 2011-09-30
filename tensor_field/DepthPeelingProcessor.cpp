#include <iostream>
#include "DepthPeelingProcessor.h"
#include "DepthPeelingVisitor.h"
#include "pbge/pbge.h"

const int PEELING_STEPS = 3;

void DepthPeelingProcessor::process(pbge::GraphicAPI * gfx, pbge::Renderer * renderer) {
    std::map<std::string, pbge::Texture2D*> & renderables = renderer->getRenderables();
    pbge::DepthBufferController * depth = gfx->depthBufferController();
    depth->enableDepthTest();
    pbge::UniformSampler2D* depthSampler = 
            dynamic_cast<pbge::UniformSampler2D*>(gfx->getUniformValue(pbge::UniformInfo("depth", pbge::SAMPLER_2D)));
    pbge::UniformSampler2D* colorSampler = 
            dynamic_cast<pbge::UniformSampler2D*>(gfx->getUniformValue(pbge::UniformInfo("color", pbge::SAMPLER_2D)));
    DepthPeelingVisitor peelingVisitor;
    pbge::BlendController * blend = gfx->getBlendController();
    // set the first depth texture to the texture of the previous processor
    depthSampler->setValue(renderables["depth"]);
    blend->disableBlending();
    depth->enableDepthTest();
    for(int i = 0; i < PEELING_STEPS; i++) {
        std::cout << i << std::endl;
        int previousId = i % 2;
        int currentId = 1 - previousId;

        gfx->bindFramebufferObject(fbos[currentId].get());
        gfx->updateState();
        gfx->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        peelingVisitor.visit(renderer->getScene()->getSceneGraphRoot(), gfx);
        gfx->bindFramebufferObject(renderer->getFramebufferObject());

        depth->disableDepthTest();
        depth->disableDepthWrite();
        blend->enableBlending();
        blend->useBlendEquation(pbge::BlendController::BLEND_EQ_ADD);
        blend->useBlendFuncSeparate(pbge::BlendController::BLEND_FACTOR_DST_ALPHA, 
                                    pbge::BlendController::BLEND_FACTOR_ONE,
                                    pbge::BlendController::BLEND_FACTOR_ZERO,
                                    pbge::BlendController::BLEND_FACTOR_ONE_MINUS_SRC_ALPHA);
        colorSampler->setValue(colors[currentId].get());
        renderer->renderScreenQuad(blitProgram.get());
        depth->enableDepthTest();
        depth->enableDepthWrite();
        blend->disableBlending();
        depthSampler->setValue(depths[currentId].get());
    }
    /*gfx->bindFramebufferObject(renderer->getFramebufferObject());
    depth->disableDepthTest();
    blend->disableBlending();
    renderer->renderScreenQuad(blitProgram.get());*/
}

void DepthPeelingProcessor::initialize(pbge::GraphicAPI *gfx, pbge::Renderer *renderer) {
    for(int i = 0; i < 2; i++)
        createFBO(gfx, renderer, i);
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

void DepthPeelingProcessor::createFBO(pbge::GraphicAPI *gfx, pbge::Renderer *renderer, int i) {
    int width = renderer->getFramebufferObject()->getWidth();
    int height = renderer->getFramebufferObject()->getHeight();
    pbge::GraphicObjectsFactory * factory = gfx->getFactory();
    colors[i].reset(factory->create2DTexture());
    depths[i].reset(factory->create2DTexture());
    fbos[i].reset(factory->createFramebuffer(width, height));
    fbos[i]->setDepthRenderable(depths[i].get());
    fbos[i]->addRenderable(colors[i].get(), "color");
}

