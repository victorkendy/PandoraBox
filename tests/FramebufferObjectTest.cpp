#include <cstddef>
#include <vector>
#include <string>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "pbge/gfx/Shader.h"
#include "pbge/exceptions/exceptions.h"
#include "pbge/gfx/FramebufferObject.h"
#include "pbge/gfx/Texture.h"
#include "pbge/gfx/Image.h"

#include "mocks/MockGraphicAPI.h"

using ::testing::Return;

class FakeFramebufferObject : public pbge::FramebufferObject {
public:
	FakeFramebufferObject(size_t width=500,size_t height=500):pbge::FramebufferObject(width, height){}
    MOCK_METHOD0(isInitialized, bool());
    MOCK_METHOD0(initialize, void());
    MOCK_METHOD1(attachRenderable, void(pbge::Texture2D* tex));
    MOCK_METHOD1(dettachRenderable, void(pbge::Texture2D *tex));
    MOCK_METHOD0(bindFramebuffer, void());
    MOCK_METHOD0(unbindFramebuffer, void());
	MOCK_METHOD1(attachDepthRenderable, void(pbge::Texture2D * tex));
    MOCK_METHOD1(useRenderables, void(const std::vector<pbge::Texture2D*> & textures));
};

class MockProgram : public pbge::GPUProgram {
public:
    MOCK_METHOD1(bind, void(pbge::GraphicAPI * ogl));
    MOCK_METHOD1(unbind, void(pbge::GraphicAPI * ogl));
    MOCK_METHOD1(link, bool(pbge::GraphicAPI * ogl));
    MOCK_METHOD0(isLinked, bool());
    MOCK_METHOD0(getInfoLog, const std::string());
    MOCK_METHOD0(getId, GLuint());
    MOCK_METHOD1(updateUniforms, void(pbge::GraphicAPI * ogl));
    MOCK_METHOD1(getShaderOfType, const std::vector<pbge::Shader*>(pbge::Shader::ShaderType type));
    MOCK_METHOD1(getOutputLocation, int(const std::string & name));

    void bindFloat(const pbge::UniformInfo & info, pbge::GraphicAPI * ogl, const float * values, const unsigned size){}

    void bindFloatVec2(const pbge::UniformInfo & info, pbge::GraphicAPI * ogl, const float* values, const unsigned size){}

    void bindFloatVec3(const pbge::UniformInfo & info, pbge::GraphicAPI * ogl, const float * values, const unsigned size){}

    void bindFloatVec4(const pbge::UniformInfo & info, pbge::GraphicAPI * ogl, const float * values, const unsigned size) {}

    void bindSampler1D(const pbge::UniformInfo & info, pbge::GraphicAPI * ogl, pbge::Texture1D * tex){}
    
    void bindSampler2D(const pbge::UniformInfo & info, pbge::GraphicAPI * ogl, pbge::Texture2D * tex) {}

    void bindMat4(const pbge::UniformInfo & info, pbge::GraphicAPI * ogl, const float * v){}
};

TEST(FramebufferObject, shouldCallBindFramebufferOnBind) {
    MockGraphicAPI api;
    FakeFramebufferObject fbo;
    EXPECT_CALL(fbo, isInitialized()).WillOnce(Return(true));
    EXPECT_CALL(fbo, bindFramebuffer());
    fbo.bind(&api);
}

TEST(FramebufferObject, shouldBeInitializedOnFirstBind) {
    FakeFramebufferObject fbo;
    MockGraphicAPI api;
    EXPECT_CALL(fbo, isInitialized()).WillOnce(Return(false));
    EXPECT_CALL(fbo, initialize());
    EXPECT_CALL(fbo, bindFramebuffer());
    fbo.bind(&api);
}

TEST(FramebufferObject, shouldntInitializeIfAlreadyInitialized) {
    FakeFramebufferObject fbo;
    MockGraphicAPI api;
    EXPECT_CALL(fbo, isInitialized()).WillOnce(Return(true));
    EXPECT_CALL(fbo, initialize()).Times(0);
    EXPECT_CALL(fbo, bindFramebuffer());
    fbo.bind(&api);
}

TEST(FramebufferObject, shouldCallUnbindFramebufferOnUnbind) {
    FakeFramebufferObject fbo;
    MockGraphicAPI api;
    EXPECT_CALL(fbo, bindFramebuffer());
    EXPECT_CALL(fbo, unbindFramebuffer());
    fbo.bind(&api);
    fbo.unbind();
}

class MockTexture2D : public pbge::Texture2D {
public:
	MockTexture2D(size_t w=500, size_t h=500):width(w),height(h),initialized(false) {}
    void setMinFilter(pbge::Texture::Filter filter){}
    void setMagFilter(pbge::Texture::Filter filter){}
    MOCK_METHOD1(bindTextureOn, void(pbge::TextureUnit * unit));
    void setImage(pbge::Image * image, pbge::Texture::Format format){}
    MOCK_METHOD7(setImageData, void(pbge::Texture::DataType type, pbge::Texture::Format dataFormat, void * image, unsigned size, int width, int height, pbge::Texture::Format internalFormat));
	const size_t getWidth() const {return width;}
	const size_t getHeight() const {return height;}
	void setInitialized(bool b) {initialized = b;}
	const bool isInitialized() const {return initialized;}
	MOCK_METHOD0(initialize, void());

private:
	size_t width, height;
	bool initialized;
};

class FramebufferObjectTest : public testing::Test {
public:
    void SetUp() {
        EXPECT_CALL(fbo, isInitialized()).WillRepeatedly(Return(true));
        EXPECT_CALL(fbo, initialize()).Times(0);
    }
    FakeFramebufferObject fbo;
    MockGraphicAPI api;
};

TEST_F(FramebufferObjectTest, shouldInitializeUnitializedTexturesOnBind) {
	MockTexture2D tex;
	fbo.addRenderable(&tex, "tex");
	EXPECT_CALL(tex, initialize()).Times(1);
	EXPECT_CALL(fbo, attachRenderable(&tex));
    EXPECT_CALL(fbo, bindFramebuffer());
	fbo.bind(&api);
}
/* need some equivalent to do answer...
TEST_F(FramebufferObjectTest, shouldInitializeTextureWithNoImageOnBind){
	MockTexture2D tex(0,0);
	EXPECT_CALL(fbo, bindFramebuffer());
	EXPECT_CALL(tex, setImageData(pbge::Texture::UNSIGNED_BYTE, pbge::Texture::RGBA, NULL, 0, 500, 500, pbge::Texture::RGBA));
	EXPECT_CALL(tex, initialize());
	EXPECT_CALL(fbo, attachRenderable(&tex));
	fbo.addRenderable(&tex, "tex");
	fbo.bind();
}

TEST_F(FramebufferObjectTest, shouldInitializeTextureOnAddIfBound) {
	MockTexture2D tex(0,0);
	EXPECT_CALL(fbo, bindFramebuffer());
	fbo.bind();
	EXPECT_CALL(tex, setImageData(pbge::Texture::UNSIGNED_BYTE, pbge::Texture::RGBA, NULL, 0, 500, 500, pbge::Texture::RGBA));
	EXPECT_CALL(tex, initialize());
	EXPECT_CALL(fbo, attachRenderable(&tex));
	fbo.addRenderable(&tex, "tex");
}
*/
TEST_F(FramebufferObjectTest, shouldInitializeUnitializedTexturesOnAddIfAlreadyBound) {
	MockTexture2D tex;
	EXPECT_CALL(fbo, bindFramebuffer());
	fbo.bind(&api);
	EXPECT_CALL(tex, initialize());
	EXPECT_CALL(fbo, attachRenderable(&tex));
	fbo.addRenderable(&tex, "tex");
}

TEST_F(FramebufferObjectTest, shouldUnbindBoundTexturesOnBind) {
	MockTexture2D tex;
	fbo.addRenderable(&tex,"tex");
	EXPECT_CALL(tex, bindTextureOn(NULL)).Times(1);
	fbo.bind(&api);
}

TEST_F(FramebufferObjectTest, shouldUnibindBoundTexturesOnAddIfAlreadyBound) {
	MockTexture2D tex;
	fbo.bind(&api);
	EXPECT_CALL(tex, bindTextureOn(NULL));
	fbo.addRenderable(&tex, "tex");
}

TEST_F(FramebufferObjectTest, shouldUpdateRenderablesOnBind) {
    MockTexture2D tex, tex2;
    EXPECT_CALL(fbo, attachRenderable(&tex));
    EXPECT_CALL(fbo, attachRenderable(&tex2));
    EXPECT_CALL(fbo, bindFramebuffer());
    fbo.addRenderable(&tex, "tex");
    fbo.addRenderable(&tex2, "tex2");
    fbo.bind(&api);
}

TEST_F(FramebufferObjectTest, shouldUseRenderablesOnlyOnceBetweenBinds) {
    MockTexture2D tex, tex2;
    EXPECT_CALL(fbo, attachRenderable(&tex)).Times(1);
    EXPECT_CALL(fbo, attachRenderable(&tex2)).Times(1);
    EXPECT_CALL(fbo, bindFramebuffer()).Times(2);
    EXPECT_CALL(fbo, unbindFramebuffer()).Times(1);
    fbo.addRenderable(&tex, "tex");
    fbo.addRenderable(&tex2, "tex2");
    fbo.bind(&api);
    fbo.unbind();
    fbo.addRenderable(&tex, "tex");
    fbo.addRenderable(&tex2, "tex2");
    fbo.bind(&api);
}

TEST_F(FramebufferObjectTest, shouldRemoveARenderableOnNextBind) {
    MockTexture2D tex, tex2;
    EXPECT_CALL(fbo, attachRenderable(&tex)).Times(1);
    EXPECT_CALL(fbo, attachRenderable(&tex2)).Times(1);
    EXPECT_CALL(fbo, dettachRenderable(&tex)).Times(1);
    EXPECT_CALL(fbo, dettachRenderable(&tex2)).Times(0);
    EXPECT_CALL(fbo, bindFramebuffer()).Times(2);
    EXPECT_CALL(fbo, unbindFramebuffer()).Times(1);
    fbo.addRenderable(&tex, "tex");
    fbo.addRenderable(&tex2, "tex2");
    fbo.bind(&api);
    fbo.unbind();
    fbo.removeRenderable("tex");
    fbo.bind(&api);
}

TEST_F(FramebufferObjectTest, shouldClearAllRenderablesOnNextBind) {
    MockTexture2D tex, tex2;
    EXPECT_CALL(fbo, attachRenderable(&tex)).Times(1);
    EXPECT_CALL(fbo, attachRenderable(&tex2)).Times(1);
    EXPECT_CALL(fbo, dettachRenderable(&tex)).Times(1);
    EXPECT_CALL(fbo, dettachRenderable(&tex2)).Times(1);
    EXPECT_CALL(fbo, bindFramebuffer()).Times(2);
    EXPECT_CALL(fbo, unbindFramebuffer()).Times(1);
    fbo.addRenderable(&tex, "tex");
    fbo.addRenderable(&tex2, "tex2");
    fbo.bind(&api);
    fbo.unbind();
    fbo.clearRenderables();
    fbo.bind(&api);
}

TEST_F(FramebufferObjectTest, shouldAttachARenderableImmediatelyIfTheFBOIsBound) {
    MockTexture2D tex;
    EXPECT_CALL(fbo, bindFramebuffer()).Times(1);
    fbo.bind(&api);
    EXPECT_CALL(fbo, attachRenderable(&tex)).Times(1);
    fbo.addRenderable(&tex, "tex");
}

TEST_F(FramebufferObjectTest, shouldDettachARenderableImmediatelyIfTheFBOIsBound) {
    MockTexture2D tex;
    EXPECT_CALL(fbo, bindFramebuffer()).Times(1);
    EXPECT_CALL(fbo, attachRenderable(&tex)).Times(1);
    fbo.addRenderable(&tex, "tex");
    fbo.bind(&api);
    EXPECT_CALL(fbo, dettachRenderable(&tex)).Times(1);
    fbo.removeRenderable("tex");
}

TEST_F(FramebufferObjectTest, shouldClearAllRenderablesImmediatellyIfBound) {
	MockTexture2D tex;
	MockTexture2D tex2;
    EXPECT_CALL(fbo, bindFramebuffer()).Times(1);
    EXPECT_CALL(fbo, attachRenderable(&tex)).Times(1);
	EXPECT_CALL(fbo, attachRenderable(&tex2)).Times(1);
    fbo.addRenderable(&tex, "tex");
	fbo.addRenderable(&tex2, "tex2");
    fbo.bind(&api);
	EXPECT_CALL(fbo, dettachRenderable(&tex)).Times(1);
    EXPECT_CALL(fbo, dettachRenderable(&tex2)).Times(1);
	fbo.clearRenderables();
}

TEST_F(FramebufferObjectTest, shouldCheckIfTheRenderablesSizeIsTheSameAsTheFBOOnBind) {
	MockTexture2D tex(100, 200);
	fbo.addRenderable(&tex, "tex");
	EXPECT_THROW(fbo.bind(&api), pbge::IllegalArgumentException);
}

TEST_F(FramebufferObjectTest, shouldCheckIfTheRenderablesSizeIsTheSameAsTheFBOIfAlreadyBound) {
	MockTexture2D tex(100, 200);
	EXPECT_CALL(fbo, bindFramebuffer()).Times(1);	
	fbo.bind(&api);
	EXPECT_TRUE(fbo.isBound());
	EXPECT_THROW(fbo.addRenderable(&tex, "tex"), pbge::IllegalArgumentException);
}

TEST_F(FramebufferObjectTest, shouldntAllowUseOfDepthRenderableWithTheDimensionOfTheFBO) {
	MockTexture2D tex(100, 200);
	EXPECT_THROW(fbo.setDepthRenderable(&tex), pbge::IllegalArgumentException);
}

TEST_F(FramebufferObjectTest, shouldAllowUseOfDepthRenderableWithTheDimensionOfTheFBO) {
	MockTexture2D tex(500, 500);
	fbo.setDepthRenderable(&tex);
	EXPECT_CALL(fbo, attachDepthRenderable(&tex)).Times(1);
	fbo.bind(&api);
}

TEST_F(FramebufferObjectTest, shouldChangeTheDepthRenderableImmediatellyIfBound) {
	MockTexture2D tex(500, 500);
	fbo.bind(&api);
	EXPECT_CALL(fbo, attachDepthRenderable(&tex)).Times(1);
	fbo.setDepthRenderable(&tex);
}

TEST_F(FramebufferObjectTest, shouldDrawOnCurrentProgramOutputs) {
    MockProgram program;
    MockTexture2D tex(500, 500);
    MockTexture2D tex2(500, 500);
    MockTexture2D tex3(500, 500);
    std::vector<pbge::Texture2D *> expected;
    expected.push_back(&tex3);
    expected.push_back(&tex2);

    EXPECT_CALL(api, getCurrentProgram()).WillOnce(::testing::Return(&program));

    EXPECT_CALL(program, getOutputLocation("tex")).WillOnce(::testing::Return(-1));
    EXPECT_CALL(program, getOutputLocation("tex2")).WillOnce(::testing::Return(1));
    EXPECT_CALL(program, getOutputLocation("tex3")).WillOnce(::testing::Return(0));

    EXPECT_CALL(fbo, useRenderables(::testing::ContainerEq(expected)));
    fbo.addRenderable(&tex, "tex");
    fbo.addRenderable(&tex2, "tex2");
    fbo.addRenderable(&tex3, "tex3");
    fbo.bind(&api);
}

