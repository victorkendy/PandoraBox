#include <string>
#include "math3d/math3d.h"

#include "pbge/pbge.h"

#include "Ellipsoids.h"

Ellipsoids::Ellipsoids(pbge::GraphicAPI * gfx) {
    std::string vertexShader =
            "uniform vec3 scale;\n"
            "uniform mat4 transform;\n"
            "void main() {\n"
            "   mat4 scaleMatrix = mat4(1.0);\n"
            "   scaleMatrix[0][0] = scale[0];\n"
            "   scaleMatrix[1][1] = scale[1];\n"
			"   scaleMatrix[2][2] = scale[2];\n"
            "   gl_Position = gl_ModelViewProjectionMatrix * transform * scaleMatrix * gl_Vertex;\n"
            "   gl_FrontColor = vec4(1,1,1,1);\n"
            "}";

    this->sphere = pbge::Geometrics::createSphere(1.0f, 10, gfx);
    this->shader = gfx->getFactory()->createProgramFromString(vertexShader, "");
}

Ellipsoids * Ellipsoids::createEllipsoids(unsigned n) {
    numberOfEllipsoids = n;
    this->matrices = new math3d::matrix44[n];
    current = 0;
    return this;
}

Ellipsoids * Ellipsoids::addTransform(const math3d::matrix44 & m) {
    // Store m in colmajor
    this->matrices[current] = m.transpose();
    current++;
    return this;
}

pbge::ModelCollection * Ellipsoids::done(pbge::GraphicAPI * gfx) {
    pbge::Texture1D * tex = gfx->getFactory()->create1DTexture();
    tex->setImageData(pbge::Texture::FLOAT, pbge::Texture::RGBA, matrices, numberOfEllipsoids * sizeof(math3d::matrix44), 4 * numberOfEllipsoids, pbge::Texture::RGBA);
    pbge::ModelCollection * ellipsoids = new pbge::ModelCollection(sphere);
    ellipsoids->setNumberOfInstances(numberOfEllipsoids);
    pbge::UniformSampler1D * uniform = ellipsoids->getUniformSet()->getSampler1D("transforms");
    uniform->setValue(tex);
    ellipsoids->setRenderPassProgram(gfx->getFactory()->createProgramFromString(
        "#version 130\n"
        "#extension GL_ARB_draw_instanced: enable\n"
        "uniform sampler1D transforms;\n"
        "void main() {\n"
        "   int index = gl_InstanceIDARB * 4;\n"
        "   vec4 col1 = texelFetch(transforms, index, 0);\n"
        "   vec4 col2 = texelFetch(transforms, index + 1, 0);\n"
        "   vec4 col3 = texelFetch(transforms, index + 2, 0);\n"
        "   vec4 col4 = texelFetch(transforms, index + 3, 0);\n"
        "   mat4 transformation = mat4(col1, col2, col3, col4);\n"
        "   gl_Position = gl_ModelViewProjectionMatrix * transformation * gl_Vertex;\n"
        "   gl_FrontColor = vec4(1,1,1,1);\n"
        "}", ""));
    ellipsoids->setDepthPassProgram(gfx->getFactory()->createProgramFromString(
        "#version 130\n"
        "#extension GL_ARB_draw_instanced: enable\n"
        "uniform sampler1D transforms;\n"
        "void main() {\n"
        "   int index = gl_InstanceIDARB * 4;\n"
        "   vec4 col1 = texelFetch(transforms, index, 0);\n"
        "   vec4 col2 = texelFetch(transforms, index + 1, 0);\n"
        "   vec4 col3 = texelFetch(transforms, index + 2, 0);\n"
        "   vec4 col4 = texelFetch(transforms, index + 3, 0);\n"
        "   mat4 transformation = mat4(col1, col2, col3, col4);\n"
        "   gl_Position = gl_ModelViewProjectionMatrix * transformation * gl_Vertex;\n"
        "}", ""));
    return ellipsoids;
}

pbge::ModelInstance * Ellipsoids::createEllipsoid(float x_semi_axis, float y_semi_axis, float z_semi_axis) {
    pbge::ModelInstance * ellipsoid = new pbge::ModelInstance(sphere);
    pbge::UniformSet * uniforms = ellipsoid->getUniformSet();
    uniforms->getFloatVec3("scale")->setValueAt(0, x_semi_axis, y_semi_axis, z_semi_axis);
    uniforms->getFloatMat4("transform")->setValue(math3d::identity44);
    ellipsoid->setRenderPassProgram(shader);
    return ellipsoid;
}
