

#ifndef PBGE_TESTS_MOCKGRAPHICAPI_H_
#define PBGE_TESTS_MOCKGRAPHICAPI_H_

#include <string>
#include <vector>
#include <gmock/gmock.h>

#include "pbge/core/File.h"
#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/Buffer.h"
#include "pbge/gfx/Texture.h"
#include "pbge/gfx/Shader.h"
#include "pbge/gfx/GraphicObjectsFactory.h"
#include "pbge/gfx/DrawController.h"

class MockGraphicAPI : public pbge::GraphicAPI {
public:
    MOCK_METHOD0(numberOfTextureUnits, const int());
    MOCK_METHOD1(setMatrixMode, void(GLenum mode));
    MOCK_METHOD1(setContext, void(pbge::GraphicContext * newContext));
    MOCK_METHOD0(getContext, pbge::GraphicContext*());
    MOCK_METHOD0(releaseContext, void());
    MOCK_METHOD0(makeContextCurrent, void());
    MOCK_METHOD0(swapBuffers, void());
    MOCK_METHOD1(loadViewMatrix, void(const math3d::matrix44 & m));
    MOCK_METHOD1(loadProjectionMatrix, void(const math3d::matrix44 & m));
    MOCK_METHOD1(loadModelMatrix, void(const math3d::matrix44 & m));
    MOCK_METHOD0(updateState, void());
    MOCK_METHOD0(uploadProjection, void());
    MOCK_METHOD0(getFactory, pbge::GraphicObjectsFactory*());
    MOCK_METHOD1(getUniformValue, pbge::UniformValue*(const pbge::UniformInfo & info));
    MOCK_METHOD1(searchUniform, pbge::UniformValue*(const pbge::UniformInfo & info));
    MOCK_METHOD1(enableMode, void(pbge::GraphicAPI::Mode mode));
    MOCK_METHOD0(disableDrawBuffer, void());
    MOCK_METHOD1(enableDrawBuffer, void(GLenum buffer));
    MOCK_METHOD0(getState, pbge::StateSet*());
    MOCK_METHOD0(getStorage, pbge::ResourceStorage*());
    MOCK_METHOD1(chooseTextureUnit, pbge::TextureUnit*(pbge::Texture * texture));
    MOCK_METHOD1(pushUniforms, void(pbge::UniformSet * uniforms));
    MOCK_METHOD0(popUniforms, void());
    MOCK_METHOD1(bindVertexBuffer, void(pbge::VertexBuffer * vbo));
    MOCK_METHOD0(getDrawController, pbge::DrawController*());
    MOCK_METHOD4(setViewport, void(int x, int y, int w, int h));

    MOCK_METHOD2(alphaFunc, void(GLenum func, GLclampf ref));
    MOCK_METHOD2(beginQuery, void(GLenum target, GLuint id));
    MOCK_METHOD1(clear, void(GLbitfield mask));
    MOCK_METHOD4(clearColor, void(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha));
    MOCK_METHOD1(clearDepth, void(GLclampd depth));
    MOCK_METHOD1(clearStencil, void(GLint s));
    MOCK_METHOD1(clientActiveTexture, void(GLenum textureUnit));
    MOCK_METHOD2(clipPlane, void(GLenum plane, const GLdouble * equation));
    MOCK_METHOD1(disable, void(GLenum mode));
    MOCK_METHOD1(enable, void(GLenum mode));
};

class MockGraphicFactory : public pbge::GraphicObjectsFactory {
public:
    MOCK_METHOD2(createBuffer, pbge::Buffer*(size_t _size, pbge::Buffer::UsageHint _usage));
    MOCK_METHOD0(create1DTexture, pbge::Texture1D*());
    MOCK_METHOD0(create2DTexture, pbge::Texture2D*());
    MOCK_METHOD2(createShaderFromFile, pbge::Shader*(pbge::FileReader * file, pbge::Shader::ShaderType type));
    MOCK_METHOD2(createShaderFromString, pbge::Shader*(const std::string & source, pbge::Shader::ShaderType type));
    MOCK_METHOD0(createProgram, pbge::GPUProgram*());
    MOCK_METHOD2(createProgramFromFile, pbge::GPUProgram*(pbge::FileReader * vsSource, pbge::FileReader * fsSource));
    MOCK_METHOD2(createProgram, pbge::GPUProgram*(const std::vector<pbge::Shader*> & vertexShaders, const std::vector<pbge::Shader *> & fragShaders));
    MOCK_METHOD2(createProgramFromString, pbge::GPUProgram*(const std::string & vsSource, const std::string & fsSource));
};

class MockBuffer : public pbge::Buffer {
public:
    MOCK_METHOD1(map, void*(pbge::Buffer::AccessPattern pattern));
    MOCK_METHOD0(unmap, void());
    MOCK_METHOD0(unbind, void());
    MOCK_METHOD1(bindOn, void(Target _target));
    MOCK_METHOD0(destroy, void());
};

#endif //PBGE_TESTS_MOCKOPENGL_H_
