

#ifndef PBGE_TESTS_MOCKGRAPHICAPI_H_
#define PBGE_TESTS_MOCKGRAPHICAPI_H_

#include <string>
#include <gmock/gmock.h>

#include "pbge/core/File.h"
#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/Buffer.h"
#include "pbge/gfx/Texture.h"
#include "pbge/gfx/Shader.h"
#include "pbge/gfx/GraphicObjectsFactory.h"

class MockGraphicAPI : public pbge::GraphicAPI {
public:
    MOCK_METHOD0(numberOfTextureUnits, const int());
    MOCK_METHOD1(setMatrixMode, void(GLenum mode));
    MOCK_METHOD1(setContext, void(pbge::GLContext * newContext));
    MOCK_METHOD0(getContext, pbge::GLContext*());
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


    MOCK_METHOD1(activeTexture, void(GLenum textureUnit));
    MOCK_METHOD2(alphaFunc, void(GLenum func, GLclampf ref));
    MOCK_METHOD2(attachShader, void(GLuint program, GLuint shader));
    MOCK_METHOD1(begin, void(GLenum mode));
    MOCK_METHOD2(beginQuery, void(GLenum target, GLuint id));
    MOCK_METHOD3(bindAttribLocation, void(GLuint program, GLuint index, const GLchar* name));
    MOCK_METHOD2(bindTexture, void(GLenum target, GLuint texture));
    MOCK_METHOD1(clear, void(GLbitfield mask));
    MOCK_METHOD4(clearColor, void(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha));
    MOCK_METHOD1(clearDepth, void(GLclampd depth));
    MOCK_METHOD1(clearStencil, void(GLint s));
    MOCK_METHOD1(clientActiveTexture, void(GLenum textureUnit));
    MOCK_METHOD2(clipPlane, void(GLenum plane, const GLdouble * equation));
    MOCK_METHOD4(colorPointer, void(GLint size, GLenum type, GLsizei stride, GLvoid * pointer));
    MOCK_METHOD1(disable, void(GLenum mode));
    MOCK_METHOD1(enable, void(GLenum mode));
    MOCK_METHOD1(enableClientState, void(GLenum cap));
    MOCK_METHOD2(getFloatv, void(GLenum pname, GLfloat * params));
    MOCK_METHOD2(getIntegerv, void(GLenum pname, GLint * params));
    MOCK_METHOD1(getString, const char *(GLenum name));
    MOCK_METHOD1(loadMatrix, void(GLfloat * matrix));
    MOCK_METHOD1(matrixMode, void(GLenum mode));
    MOCK_METHOD3(normalPointer, void(GLenum type, GLsizei stride, GLvoid * pointer));
    MOCK_METHOD4(secondaryColorPointer, void(GLint size, GLenum type, GLsizei stride, GLvoid * pointer));
    MOCK_METHOD4(texCoordPointer, void(GLint size, GLenum type, GLsizei stride, GLvoid * pointer));
    MOCK_METHOD4(vertexPointer, void(GLint size, GLenum type, GLsizei stride, GLvoid * pointer));
    MOCK_METHOD4(viewport, void(GLint x, GLint y, GLint w, GLint h));
    
    MOCK_METHOD2(genFramebuffersEXT, void(GLsizei n, GLuint * buffers));
    MOCK_METHOD2(deleteFramebuffersEXT, void(GLsizei n, GLuint * buffers));
};

class MockGraphicFactory : public pbge::GraphicObjectsFactory {
public:
    MOCK_METHOD2(createBuffer, pbge::Buffer*(size_t _size, pbge::Buffer::UsageHint _usage));
    MOCK_METHOD0(create2DTexture, pbge::Texture2D*());
    MOCK_METHOD2(createShaderFromFile, pbge::Shader*(pbge::FileReader * file, pbge::Shader::ShaderType type));
    MOCK_METHOD2(createShaderFromString, pbge::Shader*(const std::string & source, pbge::Shader::ShaderType type));
    MOCK_METHOD0(createProgram, pbge::GPUProgram*());
    MOCK_METHOD2(createProgramFromFile, pbge::GPUProgram*(pbge::FileReader * vsSource, pbge::FileReader * fsSource));
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
