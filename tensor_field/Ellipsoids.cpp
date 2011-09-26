#include <string>
#include <cstring>
#include "math3d/math3d.h"

#include "pbge/pbge.h"
#include "PeelingAwareNode.h"

#include "Ellipsoids.h"

Ellipsoids::Ellipsoids(pbge::GraphicAPI * _gfx, int total_ellipsoids) {
    this->sphere = pbge::Geometrics::createSphere(1.0f, 3, _gfx);
    this->gfx = _gfx;
    this->tex = gfx->getFactory()->createTextureBuffer(total_ellipsoids * sizeof(math3d::matrix44));
    this->added_ellipsoids = 0;
    this->render_pass_program = NULL;
    this->depth_pass_program = NULL;
    this->peeling_program = NULL;
}

pbge::ModelCollection * Ellipsoids::createEllipsoids(unsigned number_of_ellipsoids, math3d::matrix44 * transforms, BoundingBox box) {
    void * texData = tex->getBuffer()->map(pbge::Buffer::WRITE_ONLY);
    memcpy((unsigned char *)texData + this->added_ellipsoids * sizeof(math3d::matrix44), transforms, number_of_ellipsoids * sizeof(math3d::matrix44));
    tex->getBuffer()->unmap();
    texData = NULL;
    tex->setInternalFormat(pbge::Texture::FLOAT, pbge::Texture::RGBA);
    PeelingAwareNode * ellipsoids = new PeelingAwareNode(sphere, get_peeling_program());
    ellipsoids->setNumberOfInstances(number_of_ellipsoids);

    pbge::UniformBufferSampler * uniform = ellipsoids->getUniformSet()->getBufferSampler("transforms");
    uniform->setValue(tex);

    pbge::UniformFloat * base_instance = ellipsoids->getUniformSet()->getFloat("base_instance");
    base_instance->setValue((float)this->added_ellipsoids);
    this->added_ellipsoids += number_of_ellipsoids;
	
	ellipsoids->setRenderPassProgram(get_render_pass_program());
    ellipsoids->setDepthPassProgram(get_depth_pass_program());
    return ellipsoids;
}

pbge::GPUProgram * Ellipsoids::get_render_pass_program() {
    if(this->render_pass_program == NULL) {
        this->render_pass_program = gfx->getFactory()->createProgramFromString(
            "#version 150\n"
		    "uniform samplerBuffer transforms;\n"
            "uniform float base_instance;\n"
            "uniform mat4 pbge_ModelViewMatrix;\n"
            "uniform mat4 pbge_ProjectionMatrix;\n"
		    "out vec4 position;\n"
		    "out vec3 normal;\n"
		    "out vec4 lightPosition;\n"
            "in  vec4 pbge_Vertex;\n"
		    "void main() {\n"
		    "   const vec4 light_position = vec4(16,16,16,1);\n"
            "   int index = (int(base_instance) + gl_InstanceID) * 4;\n"
		    "   vec4 col1 = texelFetch(transforms, index);\n"
		    "   vec4 col2 = texelFetch(transforms, index + 1);\n"
		    "   vec4 col3 = texelFetch(transforms, index + 2);\n"
		    "   vec4 col4 = texelFetch(transforms, index + 3);\n"
		    "   vec4 color = vec4(col1.w,col2.w,col3.w,col4.w);\n"
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
		    "   gl_FrontColor = color;\n"
		    "}",
            "in vec4 position;\n"
		    "in vec3 normal;\n"
		    "in vec4 lightPosition;\n"
		    "void main() {\n"
		    "   vec4 diffuseColor = gl_Color;\n"
		    "   vec4 lightDiffuseColor = vec4(1.0,1.0,1,1);\n"
		    "   vec3 lightDir = normalize((lightPosition - position).xyz);\n"
		    "   float intensity = max(0.0, dot(lightDir, normal));\n"
		    "   gl_FragData[0] = vec4(diffuseColor.rgb * lightDiffuseColor.rgb * intensity + 0.2, gl_Color.a);\n"
		    "}"
            );
    }
    return this->render_pass_program;
}

pbge::GPUProgram * Ellipsoids::get_depth_pass_program() {
    if(this->depth_pass_program == NULL) {
        this->depth_pass_program = gfx->getFactory()->createProgramFromString(
            "#version 140\n"
		    "#extension GL_EXT_gpu_shader4: enable\n"
            "#extension GL_ARB_draw_instanced: enable\n"
            "uniform samplerBuffer transforms;\n"
            "uniform float base_instance;\n"
            "uniform mat4 pbge_ModelViewProjectionMatrix;\n"
            "in  vec4 pbge_Vertex;\n"
            "void main() {\n"
            "   int index = (int(base_instance) + gl_InstanceIDARB) * 4;\n"
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
            "}", "");
    }
    return this->depth_pass_program;
}

pbge::GPUProgram * Ellipsoids::get_peeling_program() {
    if(this->peeling_program == NULL) {
        this->peeling_program = gfx->getFactory()->createProgramFromString(
            "#version 150\n"
		    "uniform samplerBuffer transforms;\n"
            "uniform float base_instance;\n"
            "uniform mat4 pbge_ModelViewMatrix;\n"
            "uniform mat4 pbge_ProjectionMatrix;\n"
		    "out vec4 position;\n"
		    "out vec3 normal;\n"
		    "out vec4 lightPosition;\n"
            "in  vec4 pbge_Vertex;\n"
		    "void main() {\n"
		    "   const vec4 light_position = vec4(16,16,16,1);\n"
            "   int index = (int(base_instance) + gl_InstanceID) * 4;\n"
		    "   vec4 col1 = texelFetch(transforms, index);\n"
		    "   vec4 col2 = texelFetch(transforms, index + 1);\n"
		    "   vec4 col3 = texelFetch(transforms, index + 2);\n"
		    "   vec4 col4 = texelFetch(transforms, index + 3);\n"
		    "   vec4 color = vec4(col1.w,col2.w,col3.w,col4.w);\n"
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
		    "   gl_FrontColor = color;\n"
		    "}",
            "in vec4 position;\n"
		    "in vec3 normal;\n"
		    "in vec4 lightPosition;\n"
            "uniform sampler2D depth;\n"
		    "void main() {\n"
            "   if(position.z <= (texture2D(depth,position.xy)).x) discard;\n"
            "   if(normal.z >= 0) discard;\n"
		    "   vec4 diffuseColor = gl_Color;\n"
		    "   vec4 lightDiffuseColor = vec4(1.0,1.0,1,1);\n"
		    "   vec3 lightDir = normalize((lightPosition - position).xyz);\n"
		    "   float intensity = max(0.0, dot(lightDir, normal));\n"
		    "   gl_FragData[0] = vec4(diffuseColor.rgb * lightDiffuseColor.rgb * intensity + 0.2, gl_Color.a);\n"
		    "}"
            );
    }
    return this->peeling_program;
}