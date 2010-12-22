
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <iostream>

#include "MockOpenGL.h"
#include "pbge/core/Manager.h"
#include "pbge/gfx/VBO.h"

using ::testing::_;
using ::testing::Return;

class VertexBufferBuilderTest : public testing::Test {

};

TEST_F(VertexBufferBuilderTest, builderBuildsVerticesFromIndexesCorrectly) {
    pbge::Manager::init(true);
    MockOpenGL ogl;
    pbge::Manager::getInstance()->_setOpenGL(&ogl);
    MockBuffer buffer;
    float * buf = new float[100];
    EXPECT_CALL(ogl, createBuffer(_,_,_)).Times(1).WillOnce(Return(&buffer));
    EXPECT_CALL(buffer, map()).Times(1).WillOnce(Return((void*)(buf)));
    pbge::Manager::getInstance()->_setOpenGL(&ogl);
    
    
    pbge::VertexBufferBuilder builder(3);
    
    pbge::VertexBufferBuilder::VertexAttribBuilder vertex = builder.addAttrib(3, pbge::VertexBuffer::VertexAttrib::VERTEX);
    std::vector<unsigned short> indexes;
    builder.pushValue(vertex, 1,2,3).pushValue(vertex, 2,3,4).pushValue(vertex,3,4,5);
    indexes.push_back(1);
    indexes.push_back(0);
    indexes.push_back(2);
    builder.setAttribIndex(vertex, indexes);
    builder.done();

    delete [] buf;

    /*
    float * vertex_data = builder->get_data();
    EXPECT_FLOAT_EQ(2.0f, vertex_data[0]);
    EXPECT_FLOAT_EQ(3.0f, vertex_data[1]);
    EXPECT_FLOAT_EQ(4.0f, vertex_data[2]);

    EXPECT_FLOAT_EQ(1.0f, vertex_data[3]);
    EXPECT_FLOAT_EQ(2.0f, vertex_data[4]);
    EXPECT_FLOAT_EQ(3.0f, vertex_data[5]);

    EXPECT_FLOAT_EQ(3.0f, vertex_data[6]);
    EXPECT_FLOAT_EQ(4.0f, vertex_data[7]);
    EXPECT_FLOAT_EQ(5.0f, vertex_data[8]);
    */
}
/*
TEST_F(VertexBufferBuilderTest, ifTheIndexexVectorPointsToinvalidDataThrowsException) {
    builder = new pbge::VertexBufferBuilder(3,0);
    std::vector<unsigned> indexes;
    builder->push_vertex(1,2,3);
    builder->push_vertex(2,3,4);
    indexes.push_back(1);
    indexes.push_back(0);
    indexes.push_back(2);
    builder->set_vertex_index(indexes);
    EXPECT_ANY_THROW({
        builder->done();
    });
}

TEST_F(VertexBufferBuilderTest, builderBuildsCombinationVertexAndNormalIterleaved) {
    builder = new pbge::VertexBufferBuilder(4,3);
    builder->push_vertex(1,2,3);
    builder->push_vertex(4,5,6);
    builder->push_normal(1,0,1);
    builder->push_normal(0,1,0);

    std::vector<unsigned> vertex_indexes;
    std::vector<unsigned> normal_indexes;
    vertex_indexes.push_back(0);
    vertex_indexes.push_back(1);
    normal_indexes.push_back(1);
    normal_indexes.push_back(0);

    builder->set_vertex_index(vertex_indexes);
    builder->set_normal_index(normal_indexes);
    builder->done();

    float * data = builder->get_data();
    EXPECT_FLOAT_EQ(1.0f, data[0]);
    EXPECT_FLOAT_EQ(2.0f, data[1]);
    EXPECT_FLOAT_EQ(3.0f, data[2]);
    EXPECT_FLOAT_EQ(1.0f, data[3]);

    EXPECT_FLOAT_EQ(0.0f, data[4]);
    EXPECT_FLOAT_EQ(1.0f, data[5]);
    EXPECT_FLOAT_EQ(0.0f, data[6]);

    EXPECT_FLOAT_EQ(4.0f, data[7]);
    EXPECT_FLOAT_EQ(5.0f, data[8]);
    EXPECT_FLOAT_EQ(6.0f, data[9]);
    EXPECT_FLOAT_EQ(1.0f, data[10]);

    EXPECT_FLOAT_EQ(1.0f, data[11]);
    EXPECT_FLOAT_EQ(0.0f, data[12]);
    EXPECT_FLOAT_EQ(1.0f, data[13]);
}*/