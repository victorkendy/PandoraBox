#include <iostream>
#include "DepthPeelingProcessor.h"
#include "DepthPeelingVisitor.h"
#include "pbge/pbge.h"

#define DEPTH_AUX1 "depth_aux1"
#define DEPTH_AUX2 "depth_aux2"
#define COLOR_AUX1 "color_aux1"
#define COLOR_AUX2 "color_aux2"
#define COLOR_BLEND "color_blend"
#define PEELING_STEPS 3

void DepthPeelingProcessor::process(pbge::GraphicAPI * gfx, pbge::Renderer * renderer) {
    glGetError();
    std::map<std::string, pbge::Texture2D*> & renderables = renderer->getRenderables();
    //pbge::FramebufferObject * fbo = renderer->getFramebufferObject();

    pbge::Texture2D * colorAux[2];
    pbge::Texture2D * depthAux[2];
    colorAux[0] = renderables[COLOR_AUX1];
    colorAux[1] = renderables[COLOR_AUX2];
    //pbge::Texture2D * color = renderables["color"];
    depthAux[0] = renderables[DEPTH_AUX1];
    depthAux[1] = renderables[DEPTH_AUX2];
    pbge::Texture2D * colorBlend = renderables[COLOR_BLEND];
    //pbge::Texture2D * depthBuffer = renderables["depth"];

    pbge::FramebufferObject * fbos[2];
    for(int i = 0; i < 2; i++) {
        fbos[i] = gfx->getFactory()->createFramebuffer(renderer->getFramebufferObject()->getWidth(), renderer->getFramebufferObject()->getHeight());
        fbos[i]->addRenderable(colorAux[i], "color");
        fbos[i]->setDepthRenderable(depthAux[i]);
    }
    pbge::FramebufferObject * colorBlenderFbo = gfx->getFactory()->createFramebuffer(renderer->getFramebufferObject()->getWidth(), renderer->getFramebufferObject()->getHeight());
    colorBlenderFbo->addRenderable(colorBlend, "color");
    colorBlenderFbo->setDepthRenderable(depthAux[0]);

    pbge::DepthBufferController * depth = gfx->depthBufferController();

    gfx->bindFramebufferObject(colorBlenderFbo);
    gfx->clearColor(0,0,0,1);
    gfx->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    depth->enableDepthTest();

    pbge::DepthPassVisitor depthVisitor;
    pbge::ColorPassVisitor colorVisitor;

    depthVisitor.visit(renderer->getScene()->getSceneGraphRoot(), gfx);
    colorVisitor.visit(renderer->getScene()->getSceneGraphRoot(), gfx);
    
    pbge::UniformSampler2D* depthSampler = 
            dynamic_cast<pbge::UniformSampler2D*>(gfx->getUniformValue(pbge::UniformInfo("depth", pbge::SAMPLER_2D)));
    pbge::UniformSampler2D* colorSampler = 
            dynamic_cast<pbge::UniformSampler2D*>(gfx->getUniformValue(pbge::UniformInfo("color", pbge::SAMPLER_2D)));

    DepthPeelingVisitor peelingVisitor;
    pbge::BlendController * blend = gfx->getBlendController();
    gfx->clearColor(0,0,0,0);

    for(int i = 0; i < PEELING_STEPS; i++) {
        int previousId = i % 2;
        int currentId = 1 - previousId;

        gfx->bindFramebufferObject(fbos[currentId]);
        blend->disableBlending();
        depth->enableDepthTest();

        depthSampler->setValue(depthAux[previousId]);

        // Do I have to do this?
        fbos[currentId]->update(gfx);

        gfx->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        peelingVisitor.visit(renderer->getScene()->getSceneGraphRoot(), gfx);

        gfx->bindFramebufferObject(colorBlenderFbo);
        depth->disableDepthTest();
        blend->enableBlending();
        blend->useBlendEquation(pbge::BlendController::BLEND_EQ_ADD);
        blend->useBlendFuncSeparate(pbge::BlendController::BLEND_FACTOR_DST_ALPHA, 
                                    pbge::BlendController::BLEND_FACTOR_ONE,
                                    pbge::BlendController::BLEND_FACTOR_ZERO,
                                    pbge::BlendController::BLEND_FACTOR_ONE_MINUS_SRC_ALPHA);

        colorSampler->setValue(colorAux[currentId]);
        renderer->renderScreenQuad(blitProgram.get());
        blend->disableBlending();
    }
    gfx->bindFramebufferObject(renderer->getFramebufferObject());
    depth->disableDepthTest();
    blend->disableBlending();

    colorSampler->setValue(colorBlend);
    renderer->renderScreenQuad(blitProgram.get());

    glLoadIdentity();
}

void DepthPeelingProcessor::initialize(pbge::GraphicAPI *gfx, pbge::Renderer *renderer) {
    std::map<std::string, pbge::Texture2D*> & renderables = renderer->getRenderables();
    create2DTextureIfNotExists(DEPTH_AUX1, gfx, renderables);
    create2DTextureIfNotExists(COLOR_AUX1, gfx, renderables);
    create2DTextureIfNotExists(DEPTH_AUX2, gfx, renderables);
    create2DTextureIfNotExists(COLOR_AUX2, gfx, renderables);
    create2DTextureIfNotExists(COLOR_BLEND, gfx, renderables);
    
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

void DepthPeelingProcessor::create2DTextureIfNotExists(std::string texture_name, pbge::GraphicAPI * gfx, std::map<std::string, pbge::Texture2D*> & renderables) {
    if(renderables.find(texture_name) == renderables.end()) {
        pbge::Texture2D * depthAux = gfx->getFactory()->create2DTexture();
        renderables[texture_name] = depthAux;
    }
}