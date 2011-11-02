#include <cmath>
#include "pbge/pbge.h"
#include "DevilImage.h"

class GrassField {
public:
    GrassField(pbge::GraphicAPI * gfx) {
        createGrassField(gfx);
        createGround(gfx);
    }
    pbge::ModelCollection * getField() {
        return field;
    }
    pbge::ModelCollection * getGround() {
        return NULL;
    }
private:
    void createGround(pbge::GraphicAPI * gfx) {}
    void createGrassField(pbge::GraphicAPI * gfx) {
        pbge::VBOModel * model = grassModel(gfx);
        pbge::TextureBuffer * positions = calculatePositions(gfx);
        DevilImage image("./Resources/grass.png");
        pbge::Texture2D * grassTex = gfx->getFactory()->create2DTexture();
        grassTex->setImage(&image, pbge::Texture::RGBA);
        field = new pbge::ModelCollection(model);
        field->setNumberOfInstances(500);
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
            "void main() {\n"
            "   coord = texCoord;\n"
            "   mat4 m = pbge_ModelMatrix;\n"
            "   vec4 scaling = vec4(2.0,3.0,2.0,1.0);"
            "   m[3] += texelFetch(positions, gl_InstanceID);\n"
            "   gl_Position = pbge_ProjectionMatrix * pbge_ViewMatrix * m * (pbge_Vertex*scaling);\n"
            "   gl_FrontColor = vec4(texCoord, 0, 1);\n"
            "}\n",
            "#version 150\n"
            "uniform sampler2D grassTex;\n"
            "in vec2 coord;\n"
            "void main(){\n"
            "   vec4 color = texture2D(grassTex, coord);\n"
            "   if(color.a < 0.7) discard;\n"
            "   gl_FragColor = color;\n"
            "}"
            );
    }
    pbge::TextureBuffer * calculatePositions(pbge::GraphicAPI * gfx) {
        pbge::TextureBuffer * positions = gfx->getFactory()->createTextureBuffer(500 * 4 * sizeof(float));
        pbge::Buffer * buffer = positions->getBuffer();
        
        float * mappedBuffer = (float*)buffer->map(pbge::Buffer::WRITE_ONLY);
        for(int i = 0; i < 500; i++) {
            int baseIndex = 4 * i;
            mappedBuffer[baseIndex]  = 40.0f * ((float)rand()/(float)RAND_MAX - 0.5f);
            mappedBuffer[baseIndex+1] = 0.0f;
            mappedBuffer[baseIndex+2] = 40.0f * ((float)rand()/(float)RAND_MAX -0.5f);
            mappedBuffer[baseIndex+3] = 1.0f;
        }
        buffer->unmap();
        positions->setInternalFormat(pbge::Texture::FLOAT, pbge::Texture::RGBA);
        return positions;
    }
    pbge::ModelCollection * field;
};