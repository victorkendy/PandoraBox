#include <string>

#include "pbge/pbge.h"

#include "Ellipses.h"

Ellipses::Ellipses(pbge::GraphicAPI * gfx) {
    std::string vertexShader =
            "uniform vec2 scale;\n"
            "void main() {\n"
            "   mat4 scaleMatrix = mat4(1.0);\n"
            "   scaleMatrix[0][0] = scale[0];\n"
            "   scaleMatrix[1][1] = scale[1];\n"
            "   gl_Position = gl_ModelViewProjectionMatrix * scaleMatrix * gl_Vertex;\n"
            "   gl_FrontColor = vec4(1,1,1,1);\n"
            "}";

    this->circle = pbge::Geometrics::createCircunference(1.0f, 100, gfx);
    this->shader = gfx->getFactory()->createProgramFromString(vertexShader, "");
}

pbge::ModelInstance * Ellipses::createEllipse(float x_semi_axis, float y_semi_axis) {
    pbge::ModelInstance * ellipse = new pbge::ModelInstance(circle);
    pbge::UniformSet * uniforms = ellipse->getUniformSet();
    uniforms->getFloatVec2("scale")->setValueAt(0, x_semi_axis, y_semi_axis);
    uniforms->getFloatVec2("scale")->setValueAt(1, 1, 1);
    ellipse->setRenderPassProgram(shader);
    return ellipse;
}
