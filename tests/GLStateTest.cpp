
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "mocks/MockOpenGL.h"

#include "pbge/gfx/StateSet.h"
#include "pbge/gfx/Texture.h"
#include "pbge/internal/OpenGLStates.h"

using ::testing::Return;
using ::testing::_;

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

TEST(StateSetTest, ifThereIsAFreeUnitAndTheTextureIsNotBoundReturnTheFreeUnit) {
    MockOpenGL ogl;
    MockTexture texture;
    MockTexture texture2;
    EXPECT_CALL(ogl, numberOfTextureUnits()).Times(1).WillOnce(Return(2));
    pbge::StateSet stateSet(&ogl);
    pbge::TextureUnit * unit = stateSet.chooseTexUnit(&texture);
    unit->setTexture(&texture);
    ASSERT_TRUE(unit->shouldChange(&ogl));
    EXPECT_CALL(ogl, activeTexture(GL_TEXTURE0 + unit->getIndex())).Times(1);
    EXPECT_CALL(ogl, bindTexture(_, _));
    unit->makeChange(&ogl);
    ASSERT_EQ(NULL, stateSet.chooseTexUnit(&texture2)->getCurrentTexture());
}

/*
TEST(StateSetTest, ifThereAreNoFreeUnitsChooseTextureUnitReturnsTheUnitWithTheOldestBinding) {

}

*/