
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "mocks/MockOpenGL.h"

#include "pbge/gfx/StateSet.h"
#include "pbge/gfx/Texture.h"
#include "pbge/internal/OpenGLStates.h"

using ::testing::Return;
using ::testing::_;


TEST(UniformInfoTest, test1) {
    EXPECT_TRUE(pbge::UniformInfo("p0", pbge::FLOAT_VEC2, -1) < pbge::UniformInfo("p0", pbge::FLOAT_VEC4, -1));
    EXPECT_FALSE(pbge::UniformInfo("p0", pbge::FLOAT_VEC4, -1) < pbge::UniformInfo("p0", pbge::FLOAT_VEC2, -1));

    EXPECT_TRUE(pbge::UniformInfo("p1", pbge::FLOAT_VEC2, -1) < pbge::UniformInfo("p0", pbge::FLOAT_VEC4, -1));
    EXPECT_FALSE(pbge::UniformInfo("p1", pbge::FLOAT_VEC4, -1) < pbge::UniformInfo("p0", pbge::FLOAT_VEC2, -1));

    EXPECT_TRUE(pbge::UniformInfo("p0", pbge::FLOAT_VEC4, -1) < pbge::UniformInfo("scale", pbge::FLOAT_VEC4, -1));
    EXPECT_FALSE(pbge::UniformInfo("scale", pbge::FLOAT_VEC4, -1) < pbge::UniformInfo("p0", pbge::FLOAT_VEC4, -1));
    
    EXPECT_FALSE(pbge::UniformInfo("zzz", pbge::FLOAT_VEC4, -1) < pbge::UniformInfo("aaa", pbge::FLOAT_VEC2, -1));
    EXPECT_TRUE(pbge::UniformInfo("aaa", pbge::FLOAT_VEC2, -1) < pbge::UniformInfo("zzz", pbge::FLOAT_VEC4, -1));

    EXPECT_FALSE(pbge::UniformInfo("p0", pbge::FLOAT_VEC4, -1) < pbge::UniformInfo("p0", pbge::FLOAT_VEC4, -1));
}

TEST(UniformInfoTest, smokeTest) {
    pbge::UniformInfo scale = pbge::UniformInfo("scale", pbge::FLOAT_VEC2, -1);
    pbge::UniformInfo light_position = pbge::UniformInfo("light_position", pbge::FLOAT_VEC4, -1);
    pbge::UniformInfo light_diffuseColor = pbge::UniformInfo("light_diffuseColor", pbge::FLOAT_VEC4, -1);
    pbge::UniformInfo p0 = pbge::UniformInfo("p0", pbge::FLOAT_VEC4, -1);
    pbge::UniformInfo p1 = pbge::UniformInfo("p1", pbge::FLOAT_VEC4, -1);
    pbge::UniformInfo p2 = pbge::UniformInfo("p2", pbge::FLOAT_VEC4, -1);
    pbge::UniformInfo p3 = pbge::UniformInfo("p3", pbge::FLOAT_VEC4, -1);
    
    EXPECT_TRUE(scale < light_position);
    EXPECT_TRUE(scale < light_diffuseColor);
    EXPECT_TRUE(scale < p0);
    EXPECT_TRUE(scale < p1);
    EXPECT_TRUE(scale < p2);
    EXPECT_TRUE(scale < p3);

    EXPECT_FALSE(light_position < scale);
    EXPECT_FALSE(light_diffuseColor < scale);
    EXPECT_FALSE(p0 < scale);
    EXPECT_FALSE(p1 < scale);
    EXPECT_FALSE(p2 < scale);
    EXPECT_FALSE(p3 < scale);
}


class MockTexture : public pbge::Texture {
public:
    MockTexture() : pbge::Texture(pbge::Texture::RGBA) {}
    
    MOCK_METHOD1(initializeTexture, void(pbge::OpenGL * gl));
};

TEST(StateSetTest, ifAllUnitsAreFreeReturnsAFreeUnit) {
    MockOpenGL ogl;
    MockTexture texture;
    EXPECT_CALL(ogl, numberOfTextureUnits()).Times(1).WillOnce(Return(2));
    
    pbge::StateSet stateSet(&ogl);
    pbge::TextureUnit * unit = stateSet.chooseTexUnit(&texture);
    ASSERT_EQ(NULL, unit->getCurrentTexture());
}

TEST(StateSetTest, ifTheTextureIsBoundToAUnitReturnsTheUnitThatIsBoundToTheTexture) {
    MockOpenGL ogl;
    MockTexture texture;
    EXPECT_CALL(ogl, numberOfTextureUnits()).Times(1).WillOnce(Return(2));
    pbge::StateSet stateSet(&ogl);
    pbge::TextureUnit * unit = stateSet.chooseTexUnit(&texture);
    unit->setTexture(&texture);
    ASSERT_TRUE(unit->shouldChange(&ogl));
    EXPECT_CALL(ogl, activeTexture(GL_TEXTURE0 + unit->getIndex())).Times(1);
    EXPECT_CALL(ogl, bindTexture(_, _));
    unit->makeChange(&ogl);
    ASSERT_EQ(&texture, stateSet.chooseTexUnit(&texture)->getCurrentTexture());
}

void bindTexOnUnit(pbge::TextureUnit * unit, MockTexture & texture, MockOpenGL & ogl) {
    unit->setTexture(&texture);
    ASSERT_TRUE(unit->shouldChange(&ogl));
    unit->makeChange(&ogl);
}

TEST(StateSetTest, ifThereIsAFreeUnitAndTheTextureIsNotBoundReturnTheFreeUnit) {
    MockOpenGL ogl;
    MockTexture texture;
    MockTexture texture2;
    EXPECT_CALL(ogl, numberOfTextureUnits()).Times(1).WillOnce(Return(2));
    pbge::StateSet stateSet(&ogl);
    pbge::TextureUnit * unit = stateSet.chooseTexUnit(&texture);
    EXPECT_CALL(ogl, activeTexture(GL_TEXTURE0 + unit->getIndex()));
    EXPECT_CALL(ogl, bindTexture(_, _));
    bindTexOnUnit(unit, texture, ogl);
    ASSERT_EQ(NULL, stateSet.chooseTexUnit(&texture2)->getCurrentTexture());
}


TEST(StateSetTest, ifThereAreNoFreeUnitsChooseTextureUnitReturnsTheUnitWithTheOldestBinding) {
    MockOpenGL ogl;
    MockTexture texture;
    MockTexture texture2;
    MockTexture texture3;

    EXPECT_CALL(ogl, numberOfTextureUnits()).Times(1).WillOnce(Return(2));
    pbge::StateSet stateSet(&ogl);
    pbge::TextureUnit * unit = stateSet.chooseTexUnit(&texture);
    EXPECT_CALL(ogl, activeTexture(GL_TEXTURE0 + unit->getIndex())).Times(1);
    EXPECT_CALL(ogl, bindTexture(_, _)).Times(2);
    bindTexOnUnit(unit, texture, ogl);
    
    unit = stateSet.chooseTexUnit(&texture2);
    EXPECT_CALL(ogl, activeTexture(GL_TEXTURE0 + unit->getIndex())).Times(1);
    bindTexOnUnit(unit, texture2, ogl);
}

