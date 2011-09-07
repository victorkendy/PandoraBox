#include <string>
#include <cstring>
#include "math3d/math3d.h"

#include "pbge/pbge.h"

#include "Ellipsoids.h"

Ellipsoids::Ellipsoids(pbge::GraphicAPI * _gfx) {
    this->sphere = pbge::Geometrics::createSphere(1.0f, 3, _gfx);
    this->gfx = _gfx;
}

pbge::ModelCollection * Ellipsoids::createEllipsoids(unsigned number_of_ellipsoids, math3d::matrix44 * transforms) {
    pbge::TextureBuffer * tex = gfx->getFactory()->createTextureBuffer(number_of_ellipsoids * sizeof(math3d::matrix44));
    void * texData = tex->getBuffer()->map(pbge::Buffer::WRITE_ONLY);
    memcpy(texData, transforms, number_of_ellipsoids * sizeof(math3d::matrix44));
    tex->getBuffer()->unmap();
    texData = NULL;
    tex->setInternalFormat(pbge::Texture::FLOAT, pbge::Texture::RGBA);
    pbge::ModelCollection * ellipsoids = new pbge::ModelCollection(sphere);
    ellipsoids->setNumberOfInstances(number_of_ellipsoids);

    pbge::UniformBufferSampler * uniform = ellipsoids->getUniformSet()->getBufferSampler("transforms");
    uniform->setValue(tex);
	
	ellipsoids->setRenderPassProgram(gfx->getFactory()->createProgramFromString(
        "#version 150\n"
		"uniform samplerBuffer transforms;\n"
        "uniform mat4 pbge_ModelViewMatrix;\n"
        "uniform mat4 pbge_ProjectionMatrix;\n"
		"out vec4 position;\n"
		"out vec3 normal;\n"
		"out vec4 lightPosition;\n"
        "in  vec4 pbge_Vertex;\n"
		"void main() {\n"
		"   const vec4 light_position = vec4(16,16,16,1);\n"
		"   int index = gl_InstanceID * 4;\n"
		"   vec4 col1 = texelFetch(transforms, index);\n"
		"   vec4 col2 = texelFetch(transforms, index + 1);\n"
		"   vec4 col3 = texelFetch(transforms, index + 2);\n"
		"   vec4 col4 = texelFetch(transforms, index + 3);\n"
		"   vec3 color = vec3(col1.w,col2.w,col3.w);\n"
		"   col1 = vec4(col1.xyz, 0);\n"
		"   col2 = vec4(col2.xyz, 0);\n"
		"   col3 = vec4(col3.xyz, 0);\n"
		"   col4 = vec4(col4.xyz, 1);\n"
		"   mat4 transformation = mat4(col1, col2, col3, col4);\n"
		"   mat4 t = pbge_ModelViewMatrix * transformation;\n"
		"   vec4 _normal = inverse(transpose(t)) * pbge_Vertex;\n"
		"   normal = normalize(_normal.xyz);\n"
		"   position = t * pbge_Vertex;\n"
		"   lightPosition = t * light_position;\n"
		"   gl_Position = pbge_ProjectionMatrix * position;\n"
		"   gl_FrontColor = vec4(color, 1.0);\n"
		"}",
        "in vec4 position;\n"
		"in vec3 normal;\n"
		"in vec4 lightPosition;\n"
		"void main() {\n"
		"   vec4 diffuseColor = gl_Color;\n"
		"   vec4 lightDiffuseColor = vec4(1.0,1.0,1,1);\n"
		"   vec3 lightDir = normalize((lightPosition - position).xyz);\n"
		"   float intensity = max(0.0, dot(lightDir, normal));\n"
		"   gl_FragData[0] = vec4(diffuseColor.rgb * lightDiffuseColor.rgb * intensity, gl_Color.a);\n"
		"}"
        ));
    ellipsoids->setDepthPassProgram(gfx->getFactory()->createProgramFromString(
        "#version 140\n"
		"#extension GL_EXT_gpu_shader4: enable\n"
        "#extension GL_ARB_draw_instanced: enable\n"
        "uniform samplerBuffer transforms;\n"
        "uniform mat4 pbge_ModelViewProjectionMatrix;\n"
        "in  vec4 pbge_Vertex;\n"
        "void main() {\n"
        "   int index = gl_InstanceIDARB * 4;\n"
        "   vec4 col1 = texelFetch(transforms, index);\n"
        "   vec4 col2 = texelFetch(transforms, index + 1);\n"
        "   vec4 col3 = texelFetch(transforms, index + 2);\n"
        "   vec4 col4 = texelFetch(transforms, index + 3);\n"
		"   col1 = vec4(col1.xyz, 0);\n"
		"   col2 = vec4(col2.xyz, 0);\n"
		"   col3 = vec4(col3.xyz, 0);\n"
		"   col4 = vec4(col4.xyz, 1);\n"
        "   mat4 transformation = mat4(col1, col2, col3, col4);\n"
        "   gl_Position = pbge_ModelViewProjectionMatrix * transformation * pbge_Vertex;\n"
        "}", ""));
    return ellipsoids;
}
