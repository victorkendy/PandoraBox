#include <cmath>
#include "pbge/pbge.h"
#include "DevilImage.h"

class GrassField {
public:
    static const int grassInstances = 600;
    GrassField(pbge::GraphicAPI * gfx) {
        createGrassField(gfx);
        createGround(gfx);
    }
    pbge::ModelCollection * getField() {
        return field;
    }
    pbge::ModelCollection * getGround() {
        return ground;
    }
private:
    void createGround(pbge::GraphicAPI * gfx) {
        pbge::VertexBufferBuilder builder(4);
        pbge::VertexAttribBuilder vertex = builder.addAttrib(4, pbge::VertexAttrib::VERTEX);
        builder.on(vertex);
        builder.pushValue(-0.5f, 0.0f, -0.5f, 1.0f).pushValue(0.5f, 0.0f, -0.5f, 1.0f)
               .pushValue(0.5f, 0.0f, 0.5f, 1.0f).pushValue(-0.5f, 0.0f, 0.5f, 1.0f);

        DevilImage image("./Resources/ground2.png");
        pbge::Texture2D * groundTex = gfx->getFactory()->create2DTexture();
        groundTex->setImage(&image, pbge::Texture::RGBA);

        pbge::VBOModel * model = new pbge::VBOModel(builder.done(pbge::Buffer::STATIC_DRAW, gfx), GL_QUADS);
        ground = new pbge::ModelCollection(model);
        ground->setNumberOfInstances(484);
        ground->setRenderPassProgram(groundRenderPassProgram(gfx));
        ground->getUniformSet()->getSampler2D("groundTex")->setValue(groundTex);
    }
    void createGrassField(pbge::GraphicAPI * gfx) {
        pbge::VBOModel * model = grassModel(gfx);
        pbge::TextureBuffer * positions = calculatePositions(gfx);
        DevilImage image("./Resources/grass.png");
        pbge::Texture2D * grassTex = gfx->getFactory()->create2DTexture();
        grassTex->setImage(&image, pbge::Texture::RGBA);
        field = new pbge::ModelCollection(model);
        field->setNumberOfInstances(grassInstances);
        field->setRenderPassProgram(grassRenderPassProgram(gfx));
        field->getUniformSet()->getBufferSampler("positions")->setValue(positions);
        field->getUniformSet()->getSampler2D("grassTex")->setValue(grassTex);
    }
    pbge::VBOModel * grassModel(pbge::GraphicAPI * gfx) {
        float sqrt3 = sqrt(3.0f);
        pbge::VertexBufferBuilder builder(12);
        pbge::VertexAttribBuilder vertex = builder.addAttrib(4, pbge::VertexAttrib::VERTEX);
        pbge::VertexAttribBuilder texCoord = builder.addAttrib(2, pbge::VertexAttrib::CUSTOM_ATTRIB, "texCoord");
        builder.on(vertex)
            .pushValue(-1, 0.0f, -sqrt3/6.0f, 1.0f).pushValue(1, 0.0f, -sqrt3/6.0f, 1.0f)
            .pushValue(1, 1.0f, -sqrt3/6.0f, 1.0f).pushValue(-1, 1.0f, -sqrt3/6.0f, 1.0f)

            .pushValue(-0.25f, 0.0f, 0.583f*sqrt3, 1.0f).pushValue(0.25f, 0.0f, 0.416f*sqrt3, 1.0f)
            .pushValue(0.25f, 1.0f, 0.416f*sqrt3, 1.0f).pushValue(-0.25f, 1.0f, 0.583f*sqrt3, 1.0f)

            .pushValue(0.25f, 0.0f, 0.583f*sqrt3, 1.0f).pushValue(-0.25f, 0.0f, 0.416f*sqrt3, 1.0f)
            .pushValue(-0.25f, 1.0f, 0.416f*sqrt3, 1.0f).pushValue(0.25f, 1.0f, 0.583f*sqrt3, 1.0f);
        builder.on(texCoord)
            .pushValue(0.0f, 0.0f).pushValue(1.0f, 0.0f)
            .pushValue(1.0f, 1.0f).pushValue(0.0f, 1.0f)

            .pushValue(0.0f, 0.0f).pushValue(1.0f, 0.0f)
            .pushValue(1.0f, 1.0f).pushValue(0.0f, 1.0f)

            .pushValue(0.0f, 0.0f).pushValue(1.0f, 0.0f)
            .pushValue(1.0f, 1.0f).pushValue(0.0f, 1.0f);
        pbge::VertexBuffer * vertexBuffer = builder.done(pbge::Buffer::STATIC_DRAW, gfx);
        return new pbge::VBOModel(vertexBuffer, GL_QUADS);
    }
    pbge::GPUProgram * grassRenderPassProgram(pbge::GraphicAPI * gfx) {
        return gfx->getFactory()->createProgramFromString(
            "#version 150\n"
            "in vec4 pbge_Vertex;\n"
            "in vec2 texCoord;\n"
            "out vec2 coord;\n"
            "uniform mat4 pbge_ModelMatrix;\n"
            "uniform mat4 pbge_ViewMatrix;\n"
            "uniform mat4 pbge_ProjectionMatrix;\n"
            "uniform samplerBuffer positions;\n"
            "uniform float t;\n"
            "void main() {\n"
            "   coord = texCoord;\n"
            "   mat4 m = pbge_ModelMatrix;\n"
            "   vec4 translation = texelFetch(positions, gl_InstanceID);\n"
            "   m[3] += translation;\n"
            "   m[3][3] = 1.0;\n"
            "   if(pbge_Vertex.y > 0.1) {\n"
            "       vec4 wind = vec4(0.3, 0, 0, 0) * sin(pbge_Vertex.x*t);\n"
            "       m[3] += wind;\n"
            "   }\n"
            "   vec4 scaling = vec4(vec3(2.0,3.0,2.0)*translation[3],1.0);\n"
            "   gl_Position = pbge_ProjectionMatrix * pbge_ViewMatrix * m * (pbge_Vertex*scaling);\n"
            "   gl_FrontColor = vec4(texCoord, 0, 1);\n"
            "}\n",
            "uniform sampler2D grassTex;\n"
            "in vec2 coord;\n"
            "vec4 calculateFog(in vec4 color) {\n"
            "   const float LOG2 = 1.442695;\n"
            "   const float density = 0.04;\n"
            "   const vec4 fogColor = vec4(1.0,1.0,1.0,1.0);"
            "   float z = gl_FragCoord.z / gl_FragCoord.w;\n"
            "   float fogFactor = exp2(-density * density * z * z * LOG2);\n"
            "   fogFactor = clamp(fogFactor, 0.0, 1.0);\n"
            "   return mix(fogColor, color, fogFactor);\n"
            "}\n"
            "void main(){\n"
            "   vec4 color = texture2D(grassTex, coord);\n"
            "   if(color.a < 0.7) discard;\n"
            "   gl_FragColor = calculateFog(color);\n"
            "}"
            );
    }
    pbge::GPUProgram * groundRenderPassProgram(pbge::GraphicAPI * gfx) {
        return gfx->getFactory()->createProgramFromString(
            "#version 150\n"
            "in vec4 pbge_Vertex;\n"
            "uniform mat4 pbge_ModelMatrix;\n"
            "uniform mat4 pbge_ViewMatrix;\n"
            "uniform mat4 pbge_ProjectionMatrix;\n"
            "void main(){\n"
            "   vec4 position = vec4(-11.0 + float(int(gl_InstanceID/22)), 0.0, -11.0 + float(int(gl_InstanceID % 22)), 0.0f);\n"
            "   mat4 m = pbge_ModelMatrix;\n"
            "   m[3] += position;\n"
            "   gl_Position = pbge_ProjectionMatrix * pbge_ViewMatrix * m * pbge_Vertex;\n"
            "   gl_FrontColor = pbge_Vertex + 0.5;\n"
            "}\n"
            ,
            "uniform sampler2D groundTex;"
            "void main() {\n"
            "   vec4 color = texture2D(groundTex, gl_Color.xz);\n"
            "   gl_FragColor = color;\n"
            "}\n"
            );
    }
    pbge::TextureBuffer * calculatePositions(pbge::GraphicAPI * gfx) {
        pbge::TextureBuffer * positions = gfx->getFactory()->createTextureBuffer(grassInstances * 4 * sizeof(float));
        pbge::Buffer * buffer = positions->getBuffer();
        
        float * mappedBuffer = (float*)buffer->map(pbge::Buffer::WRITE_ONLY);
        for(int i = 0; i < grassInstances; i++) {
            int baseIndex = 4 * i;
            mappedBuffer[baseIndex]  = 40.0f * ((float)rand()/(float)RAND_MAX - 0.5f);
            mappedBuffer[baseIndex+1] = 0.0f;
            mappedBuffer[baseIndex+2] = 40.0f * ((float)rand()/(float)RAND_MAX - 0.5f);
            mappedBuffer[baseIndex+3] = ((float)rand()/(float)RAND_MAX) * 1.5f + 0.2f;
        }
        buffer->unmap();
        positions->setInternalFormat(pbge::Texture::FLOAT, pbge::Texture::RGBA);
        return positions;
    }
    pbge::ModelCollection * field, * ground;
};