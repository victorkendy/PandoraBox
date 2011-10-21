#include <string>
#include <cstring>
#include "math3d/math3d.h"

#include "pbge/pbge.h"

#include "Ellipsoids.h"

Ellipsoids::Ellipsoids(pbge::GraphicAPI * _gfx, int total_ellipsoids) {
    pbge::Model ** _models = new pbge::Model*[5];
    _models[0] = pbge::Geometrics::createSphere(1.0f, 10, _gfx);
    _models[1] = pbge::Geometrics::createSphere(1.0f, 7, _gfx);
    _models[2] = pbge::Geometrics::createSphere(1.0f, 5, _gfx);
    _models[3] = pbge::Geometrics::createSphere(1.0f, 4, _gfx);
    _models[4] = pbge::Geometrics::createSphere(1.0f, 3, _gfx);
    _models[5] = pbge::Geometrics::createSphere(1.0f, 2, _gfx);
    float * distances = new float[4];
    distances[0] = 10.0f;
    distances[1] = 20.0f;
    distances[2] = 35.0f;
    distances[3] = 60.0f;
    distances[4] = 100.0f;
    this->models = new LODModels(_models, distances, 6);
    this->gfx = _gfx;
    this->tex = gfx->getFactory()->createTextureBuffer(total_ellipsoids * sizeof(math3d::matrix44));
    this->added_ellipsoids = 0;
    this->render_pass_program = NULL;
    this->depth_pass_program = NULL;
    this->peeling_program = NULL;
    this->transformation_shader = NULL;
}

PeelingAwareCollection * Ellipsoids::createEllipsoids(unsigned number_of_ellipsoids, math3d::matrix44 * transforms, BoundingBox box) {
    void * texData = tex->getBuffer()->map(pbge::Buffer::WRITE_ONLY);
    memcpy((unsigned char *)texData + this->added_ellipsoids * sizeof(math3d::matrix44), transforms, number_of_ellipsoids * sizeof(math3d::matrix44));
    tex->getBuffer()->unmap();
    texData = NULL;
    tex->setInternalFormat(pbge::Texture::FLOAT, pbge::Texture::RGBA);
    PeelingAwareCollection * ellipsoids = new PeelingAwareCollection(models, get_peeling_program());
    ellipsoids->setNumberOfInstances(number_of_ellipsoids);

    pbge::UniformBufferSampler * uniform = ellipsoids->getUniformSet()->getBufferSampler("transforms");
    uniform->setValue(tex);
    ellipsoids->setTransforms(transforms);

    pbge::UniformFloat * base_instance = ellipsoids->getUniformSet()->getFloat("base_instance");
    base_instance->setValue((float)this->added_ellipsoids);
    this->added_ellipsoids += number_of_ellipsoids;
	
	ellipsoids->setRenderPassProgram(get_render_pass_program());
    ellipsoids->setDepthPassProgram(get_depth_pass_program());
    ellipsoids->setBoundingBox(box);
    return ellipsoids;
}

pbge::Shader * Ellipsoids::get_transformation_shader() {
    if(this->transformation_shader == NULL) {
        this->transformation_shader = gfx->getFactory()->createShaderFromString(
            "#version 150\n"
		    "uniform samplerBuffer transforms;\n"
            "uniform float base_instance;\n"
            "uniform float scale;\n"
            "uniform mat4 pbge_ModelViewMatrix;\n"
            "in  vec4 pbge_Vertex;\n"
            "void calc_transformations(out vec4 view_position, out vec3 view_normal, out vec4 color, out mat4 view_transform) {\n"
            "   int index = (int(base_instance) + gl_InstanceID) * 4;\n"
            "   vec4 vertex = vec4(pbge_Vertex.xyz * scale, 1.0);\n"
		    "   vec4 col1 = texelFetch(transforms, index);\n"
		    "   vec4 col2 = texelFetch(transforms, index + 1);\n"
		    "   vec4 col3 = texelFetch(transforms, index + 2);\n"
		    "   vec4 col4 = texelFetch(transforms, index + 3);\n"
		    "   color = vec4(col1.w,col2.w,col3.w,col4.w);\n"
            "   col1 = vec4(col1.xyz, 0);\n"
		    "   col2 = vec4(col2.xyz, 0);\n"
		    "   col3 = vec4(col3.xyz, 0);\n"
		    "   col4 = vec4(col4.xyz, 1);\n"
		    "   mat4 transformation = mat4(col1, col2, col3, col4);\n"
		    "   view_transform = pbge_ModelViewMatrix * transformation;\n"
		    "   vec4 _normal = inverse(transpose(view_transform)) * pbge_Vertex;\n"
		    "   view_normal = normalize(_normal.xyz);\n"
		    "   view_position = view_transform * vertex;\n"
            "}",
            pbge::Shader::VERTEX_SHADER);
    }
    return this->transformation_shader;
}

pbge::GPUProgram * Ellipsoids::get_render_pass_program() {
    if(this->render_pass_program == NULL) {
        pbge::Shader * vertex_shader = gfx->getFactory()->createShaderFromString(
            "#version 150\n"
            "uniform mat4 pbge_ProjectionMatrix;\n"
            "out vec4 position;\n"
		    "out vec3 normal;\n"
		    "out vec4 lightPosition;\n"
		    "void calc_transformations(out vec4 view_position, out vec3 view_normal, out vec4 color, out mat4 view_tranform);\n"
            "void main() {\n"
		    "   mat4 view_transform;\n"
            "   const vec4 light_position = vec4(16,16,16,1);\n"
            "   calc_transformations(position, normal, gl_FrontColor, view_transform);\n"
            "   lightPosition = view_transform * light_position;\n"
		    "   gl_Position = pbge_ProjectionMatrix * position;\n"
		    "}",
            pbge::Shader::VERTEX_SHADER);
        pbge::Shader * frag_shader = gfx->getFactory()->createShaderFromString(
            "uniform float alpha_correction;\n"
            "in vec4 position;\n"
		    "in vec3 normal;\n"
		    "in vec4 lightPosition;\n"
		    "void main() {\n"
		    "   vec4 diffuseColor = gl_Color;\n"
            "   float alpha = gl_Color.a;\n"
            "   if(alpha <= alpha_correction - 0.005) discard;\n"
		    "   vec4 lightDiffuseColor = vec4(1.0,1.0,1,1);\n"
		    "   vec3 lightDir = normalize((lightPosition - position).xyz);\n"
		    "   float intensity = max(0.0, dot(lightDir, normal));\n"
		    "   gl_FragData[0] = vec4(diffuseColor.rgb * lightDiffuseColor.rgb * intensity + 0.2, alpha);\n"
		    "}",
            pbge::Shader::FRAGMENT_SHADER);
        std::vector<pbge::Shader *> vertex_shaders;
        std::vector<pbge::Shader *> fragment_shaders;

        vertex_shaders.push_back(vertex_shader);
        vertex_shaders.push_back(get_transformation_shader());
        fragment_shaders.push_back(frag_shader);
        this->render_pass_program = gfx->getFactory()->createProgram(
            vertex_shaders,
            fragment_shaders
            );
    }
    return this->render_pass_program;
}

pbge::GPUProgram * Ellipsoids::get_depth_pass_program() {
    if(this->depth_pass_program == NULL) {
        pbge::Shader * vertex_shader = gfx->getFactory()->createShaderFromString(
            "#version 150\n"
            "uniform mat4 pbge_ProjectionMatrix;\n"
            "out vec4 position;\n"
		    "out vec3 normal;\n"
		    "out vec4 lightPosition;\n"
		    "void calc_transformations(out vec4 view_position, out vec3 view_normal, out vec4 color, out mat4 view_tranform);\n"
            "void main() {\n"
		    "   mat4 view_transform;\n"
            "   calc_transformations(position, normal, gl_FrontColor, view_transform);\n"
            "   gl_Position = pbge_ProjectionMatrix * position;\n"
		    "}",
            pbge::Shader::VERTEX_SHADER);
        pbge::Shader * frag_shader = gfx->getFactory()->createShaderFromString(
            "",
            pbge::Shader::FRAGMENT_SHADER);
        std::vector<pbge::Shader *> vertex_shaders;
        std::vector<pbge::Shader *> fragment_shaders;

        vertex_shaders.push_back(vertex_shader);
        vertex_shaders.push_back(get_transformation_shader());
        fragment_shaders.push_back(frag_shader);
        this->depth_pass_program = gfx->getFactory()->createProgram(
            vertex_shaders,
            fragment_shaders
            );
    }
    return this->depth_pass_program;
}

pbge::GPUProgram * Ellipsoids::get_peeling_program() {
    if(this->peeling_program == NULL) {
        pbge::Shader * vertex_shader = gfx->getFactory()->createShaderFromString(
            "#version 150\n"
		    "uniform mat4 pbge_ProjectionMatrix;\n"
		    "out vec4 position;\n"
            "out vec4 nposition;\n"
		    "out vec3 normal;\n"
		    "out vec4 lightPosition;\n"
            "void calc_transformations(out vec4 view_position, out vec3 view_normal, out vec4 color, out mat4 view_tranform);\n"
            "void main() {\n"
            "   mat4 view_transform;\n"
            "   calc_transformations(position, normal, gl_FrontColor, view_transform);\n"
		    "   const vec4 light_position = vec4(16,16,16,1);\n"
            "   lightPosition = view_transform * light_position;\n"
            "   nposition = pbge_ProjectionMatrix * position;\n"
		    "   gl_Position = nposition;\n"
		    "}",
            pbge::Shader::VERTEX_SHADER);
        pbge::Shader * frag_shader = gfx->getFactory()->createShaderFromString(
            "in vec4 position;\n"
		    "in vec3 normal;\n"
		    "in vec4 lightPosition;\n"
            "in vec4 nposition;\n"
            "uniform sampler2D depth;\n"
            "uniform float alpha_correction;\n"
		    "void main() {\n"
            // nposition is in ndc so we need to do the perspective division to transform the position 
            // to the range -1 to 1.
            "   vec2 p = 0.5 * (nposition.xy / nposition.w) + 0.5;\n"
            "   float alpha = gl_Color.a;\n"
            // depth + offset to avoid z-fighting
            "   if(gl_FragCoord.z <= (texture2D(depth,p.xy)).r + 0.0001) discard;\n"
            "   if(normal.z >= 0) discard;\n"
            "   if(alpha <= alpha_correction - 0.005) discard;\n"
		    "   vec4 diffuseColor = gl_Color;\n"
		    "   vec4 lightDiffuseColor = vec4(1.0,1.0,1,1);\n"
		    "   vec3 lightDir = normalize((lightPosition - position).xyz);\n"
		    "   float intensity = max(0.0, dot(lightDir, normal));\n"
		    "   gl_FragData[0] = vec4(diffuseColor.rgb * lightDiffuseColor.rgb * intensity + 0.2, alpha);\n"
		    "}",
            pbge::Shader::FRAGMENT_SHADER);
        std::vector<pbge::Shader *> vertex_shaders;
        std::vector<pbge::Shader *> fragment_shaders;

        vertex_shaders.push_back(vertex_shader);
        vertex_shaders.push_back(get_transformation_shader());
        fragment_shaders.push_back(frag_shader);
        this->peeling_program = gfx->getFactory()->createProgram(
            vertex_shaders,
            fragment_shaders
            );
    }
    return this->peeling_program;
}