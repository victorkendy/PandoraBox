#ifndef TENSOR_FIELD_IMAGEPROCESSORS_H
#define TENSOR_FIELD_IMAGEPROCESSORS_H

#include "pbge/pbge.h"

pbge::FramebufferImageProcessor * colorInversor() {
    return new pbge::FramebufferImageProcessor(
        "uniform sampler2D color;\n"
        "varying vec2 position;\n"
        "void main() {\n"
        "   vec3 color = (texture2D(color, position.xy)).rgb;\n"
        "   color = 1 - color;\n"
        "   gl_FragColor = vec4(color, 1);\n"
        "}\n"
    );
}

pbge::FramebufferImageProcessor * chooseRed() {
    return new pbge::FramebufferImageProcessor(
        "uniform sampler2D color;\n"
        "varying vec2 position;\n"
        "void main() {\n"
        "   float r = (texture2D(color, position.xy)).r;\n"
        "   gl_FragColor = vec4(r, 0, 0, 1);\n"
        "}\n"
    );
}

pbge::FramebufferImageProcessor * senoidalLens() {
    return new pbge::FramebufferImageProcessor(
        "varying vec2 position;\n"
        "uniform sampler2D color;\n"
        "void main(){\n"
		"	vec2 x = 2 * position - 1.0;\n"
		"   gl_FragColor = texture2D(color, 0.5 + 0.5 * sin(1.5 * x));\n"
        "}"
    );
}

#endif