
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>

#include "pbge/gfx/VBO.h"
#include "pbge/core/Vec3.h"

using ::testing::_;

class VertexBufferBuilderTest : public testing::Test {
public:
    pbge::VertexBufferBuilder builder;
};
TEST_F(VertexBufferBuilderTest, builderBuildsVerticesFromIndexesCorrectly) {
    std::vector<unsigned> indexes;
    builder.push_vertex(pbge::Vec3(1,2,3));
    builder.push_vertex(pbge::Vec3(2,3,4));
    builder.push_vertex(pbge::Vec3(3,4,5));
    indexes.push_back(1);
    indexes.push_back(0);
    indexes.push_back(2);
    builder.set_vertex_index(indexes);
    builder.done();
    float * vertex_data = builder.get_data();
    EXPECT_FLOAT_EQ(2.0f, vertex_data[0]);
    EXPECT_FLOAT_EQ(3.0f, vertex_data[1]);
    EXPECT_FLOAT_EQ(4.0f, vertex_data[2]);
    EXPECT_FLOAT_EQ(0.0f, vertex_data[3]);

    EXPECT_FLOAT_EQ(1.0f, vertex_data[4]);
    EXPECT_FLOAT_EQ(2.0f, vertex_data[5]);
    EXPECT_FLOAT_EQ(3.0f, vertex_data[6]);
    EXPECT_FLOAT_EQ(0.0f, vertex_data[7]);

    EXPECT_FLOAT_EQ(3.0f, vertex_data[8]);
    EXPECT_FLOAT_EQ(4.0f, vertex_data[9]);
    EXPECT_FLOAT_EQ(5.0f, vertex_data[10]);
    EXPECT_FLOAT_EQ(0.0f, vertex_data[11]);
}

TEST_F(VertexBufferBuilderTest, ifTheIndexexVectorPointsToinvalidDataThrowsException) {
    std::vector<unsigned> indexes;
    builder.push_vertex(pbge::Vec3(1,2,3));
    builder.push_vertex(pbge::Vec3(2,3,4));
    indexes.push_back(1);
    indexes.push_back(0);
    indexes.push_back(2);
    builder.set_vertex_index(indexes);
    EXPECT_ANY_THROW({
        builder.done();
    });
}