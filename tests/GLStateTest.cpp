
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "mocks/MockGraphicAPI.h"

#include "pbge/gfx/StateSet.h"
#include "pbge/gfx/Texture.h"
#include "pbge/gfx/states/TextureUnits.h"

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




