#include <GL/glew.h>

#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/GPUProgram.h"
#include "pbge/gfx/Renderer.h"
#include "pbge/gfx/Geometrics.h"
#include "pbge/gfx/GraphicObjectsFactory.h"
#include "pbge/gfx/processors/BlitToFramebuffer.h"
#include "pbge/gfx/DrawController.h"
#include "pbge/gfx/Model.h"
#include "pbge/gfx/ShaderUniform.h"
#include "pbge/gfx/StateSet.h"


using namespace pbge;

void BlitToFramebuffer::initialize(GraphicAPI *gfx, Renderer *renderer) {
    quad.reset(Geometrics::createSquare(2.0f, gfx));
    blitter.reset(gfx->getFactory()->createProgramFromString(
        "varying vec2 position;\n"
        "void main() {\n"
        "   gl_Position=gl_Vertex;\n"
        "   position = 0.5 * gl_Vertex.xy + 0.5;"
        "}", 
        "varying vec2 position;\n"
        "uniform sampler2D color;\n"
        "void main(){\n"
		"	vec2 x = 2 * position - 1.0;\n"
		"   gl_FragColor = texture2D(color, 0.5 + 0.5 * sin(1.5 * x));\n"
        "}"));
}

void BlitToFramebuffer::process(GraphicAPI *gfx, Renderer *renderer) {
    // the argh method
    gfx->bindFramebufferObject(NULL);
    glDisable(GL_DEPTH_TEST);
    UniformSampler2D* sampler = dynamic_cast<UniformSampler2D*>(gfx->getUniformValue(UniformInfo("color", pbge::SAMPLER_2D)));
    sampler->setValue(renderer->getColorTexture());
    gfx->getState()->useProgram(blitter.get());
    gfx->getDrawController()->drawVBOModel(quad.get());
    glEnable(GL_DEPTH_TEST);
}