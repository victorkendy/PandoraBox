

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "mocks/MockGraphicAPI.h"
#include "pbge/gfx/states/TextureUnits.h"

using ::testing::Return;
using ::testing::_;

TEST(TextureUnitsTest, ifAllUnitsAreFreeReturnsAFreeUnit) {
    MockGraphicAPI ogl;
    MockTexture texture;
    EXPECT_CALL(ogl, numberOfTextureUnits()).Times(1).WillOnce(Return(2));
    
    pbge::TextureUnits units(&ogl);
    pbge::TextureUnit * unit = units.unitFor(&texture);
    ASSERT_EQ(NULL, unit->getCurrentTexture());
}

TEST(TextureUnitsTest, ifTheTextureIsBoundToAUnitReturnsTheUnitThatIsBoundToTheTexture) {
    MockGraphicAPI ogl;
    MockTexture texture;
    EXPECT_CALL(ogl, numberOfTextureUnits()).Times(1).WillOnce(Return(2));
    pbge::TextureUnits units(&ogl);
    pbge::TextureUnit * unit = units.unitFor(&texture);
    unit->setTexture(&texture);
    ASSERT_TRUE(unit->shouldChange(&ogl));
    unit->makeChange(&ogl);
    ASSERT_EQ(&texture, units.unitFor(&texture)->getCurrentTexture());
}

void bindTexOnUnit(pbge::TextureUnit * unit, MockTexture & texture, MockGraphicAPI & ogl) {
    unit->setTexture(&texture);
    ASSERT_TRUE(unit->shouldChange(&ogl));
    unit->makeChange(&ogl);
}

TEST(TextureUnitsTest, ifThereIsAFreeUnitAndTheTextureIsNotBoundReturnTheFreeUnit) {
    MockGraphicAPI ogl;
    MockTexture texture;
    MockTexture texture2;
    EXPECT_CALL(ogl, numberOfTextureUnits()).Times(1).WillOnce(Return(2));
    pbge::TextureUnits units(&ogl);
    pbge::TextureUnit * unit = units.unitFor(&texture);
    bindTexOnUnit(unit, texture, ogl);
    ASSERT_EQ(NULL, units.unitFor(&texture2)->getCurrentTexture());
}


TEST(TextureUnitsTest, ifThereAreNoFreeUnitsChooseTextureUnitReturnsTheUnitWithTheOldestBinding) {
    MockGraphicAPI ogl;
    MockTexture texture;
    MockTexture texture2;
    MockTexture texture3;

    EXPECT_CALL(ogl, numberOfTextureUnits()).Times(1).WillOnce(Return(2));
    pbge::TextureUnits units(&ogl);
    pbge::TextureUnit * unit = units.unitFor(&texture);
    bindTexOnUnit(unit, texture, ogl);
    
    unit = units.unitFor(&texture2);
    bindTexOnUnit(unit, texture2, ogl);
}

