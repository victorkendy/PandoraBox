
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>

#include "MockOpenGL.h"
#include "pbge/exceptions/exceptions.h"
#include "pbge/core/Manager.h"
#include "pbge/gfx/VBO.h"

using ::testing::_;
using ::testing::Return;


TEST(VertexAttribBuilderTest, attribsThatArentCustomAttribAreEqualIfnCoordIndexAndTypeMatches) {
    pbge::VertexAttribBuilder attrib1(3, pbge::VertexAttrib::TEXCOORD, 0);
    pbge::VertexAttribBuilder attrib2(3, pbge::VertexAttrib::TEXCOORD, 0);
    pbge::VertexAttribBuilder attrib3(3, pbge::VertexAttrib::CUSTOM_ATTRIB, 0);
    pbge::VertexAttribBuilder attrib4(2, pbge::VertexAttrib::TEXCOORD, 0);
    pbge::VertexAttribBuilder attrib5(3, pbge::VertexAttrib::TEXCOORD, 1);

    ASSERT_TRUE(attrib1 == attrib2);
    ASSERT_FALSE(attrib1 == attrib3);
    ASSERT_FALSE(attrib1 == attrib4);
    ASSERT_FALSE(attrib1 == attrib5);
    ASSERT_FALSE(attrib2 == attrib3);
    ASSERT_FALSE(attrib2 == attrib4);
    ASSERT_FALSE(attrib2 == attrib5);
    ASSERT_FALSE(attrib3 == attrib4);
    ASSERT_FALSE(attrib3 == attrib5);
    ASSERT_FALSE(attrib4 == attrib5);
}

TEST(VertexAttribBuilderTest, customAttribsAreEqualIfNCoordAndNameMatches) {
    pbge::VertexAttribBuilder attrib1 = pbge::VertexAttribBuilder(3, pbge::VertexAttrib::CUSTOM_ATTRIB, "name");
    pbge::VertexAttribBuilder eqAttrib1 = pbge::VertexAttribBuilder(3, pbge::VertexAttrib::CUSTOM_ATTRIB, "name");
    pbge::VertexAttribBuilder diffName = pbge::VertexAttribBuilder(3, pbge::VertexAttrib::CUSTOM_ATTRIB, "name2");
    pbge::VertexAttribBuilder diffNumCoord = pbge::VertexAttribBuilder(2, pbge::VertexAttrib::CUSTOM_ATTRIB, "name");

    ASSERT_TRUE(attrib1 == eqAttrib1);
    ASSERT_FALSE(attrib1 == diffName);
    ASSERT_FALSE(attrib1 == diffNumCoord);
}

TEST(VertexAttribBuilderTest, attribBuilderIsValidIfAllIndexesAreValid) {
    pbge::VertexAttribBuilder attrib(4, pbge::VertexAttrib::VERTEX);
    std::vector<unsigned short> indexes;
    indexes.push_back(0);
    indexes.push_back(1);
    indexes.push_back(2);
    indexes.push_back(3);
    indexes.push_back(4);
    attrib.setIndexes(indexes);
    attrib.pushValue(1,2,3,4);
    attrib.pushValue(2,3,4,5);
    attrib.pushValue(1,1,1,1);
    attrib.pushValue(2,2,2,2);
    attrib.pushValue(5,5,5,5);
    EXPECT_TRUE(attrib.isValid());
}

TEST(VertexAttribBuilderTest, attribBuilderIsNotValidIfThereIsAnInvalidIndex) {
    pbge::VertexAttribBuilder attrib(4, pbge::VertexAttrib::VERTEX);
    std::vector<unsigned short> indexes;
    indexes.push_back(4);
    indexes.push_back(4);
    indexes.push_back(4);
    indexes.push_back(4);
    attrib.setIndexes(indexes);
    attrib.pushValue(1,2,3,4);
    attrib.pushValue(1,2,3,4);
    attrib.pushValue(1,2,3,4);
    attrib.pushValue(1,2,3,4);
    EXPECT_FALSE(attrib.isValid());
}

TEST(VertexAttribBuilderTest, getNextElementGenerateElementsIndexedByIndexes) {
    pbge::VertexAttribBuilder attrib(4, pbge::VertexAttrib::VERTEX);
    float elements[20];
    float expected[] = {2.0f,3.0f,4.0f,5.0f, 1.0f,2.0f,3.0f,4.0f, 5.0f,5.0f,5.0f,5.0f, 2.0f,2.0f,2.0f,2.0f, 1.0f,1.0f,1.0f,1.0f};
    std::vector<unsigned short> indexes;
    indexes.push_back(1);
    indexes.push_back(0);
    indexes.push_back(4);
    indexes.push_back(3);
    indexes.push_back(2);
    attrib.setIndexes(indexes);
    attrib.pushValue(1,2,3,4);
    attrib.pushValue(2,3,4,5);
    attrib.pushValue(1,1,1,1);
    attrib.pushValue(2,2,2,2);
    attrib.pushValue(5,5,5,5);
    for(int i = 0; i < 5; i++) {
        attrib.getNextElement(elements + i * 4);
    }
    for(int i = 0; i < 20; i++)
        ASSERT_FLOAT_EQ(expected[i], elements[i]);
}

TEST(VertexAttribBuilderTest, areIndexesAssignedReturnsTrueOnlyAfterTheIndexVectorIsAssigned) {
    pbge::VertexAttribBuilder attrib(4, pbge::VertexAttrib::CUSTOM_ATTRIB, 0);
    std::vector<unsigned short> indexes;
    EXPECT_FALSE(attrib.areIndexesAssigned());
    attrib.setIndexes(indexes);
    EXPECT_TRUE(attrib.areIndexesAssigned());
}

class VertexBufferBuilderTest : public testing::Test {
public:
    MockOpenGL ogl;
    MockBuffer buffer;
    static void SetUpTestCase() {
        pbge::Manager::init(true);
    }

    virtual void setUp() {
        pbge::Manager::getInstance()->_setOpenGL(&ogl);
    }
};

TEST_F(VertexBufferBuilderTest, builderBuildsVerticesFromIndexesCorrectly) {
    float buf[9];
    float expected[] = {2.0f, 3.0f, 4.0f, 1.0f,2.0f,3.0f, 3.0f,4.0f,5.0f};

    EXPECT_CALL(ogl, createBuffer(9,GL_STATIC_DRAW,GL_ARRAY_BUFFER)).Times(1).WillOnce(Return(&buffer));
    EXPECT_CALL(buffer, map()).Times(1).WillOnce(Return((void*)(buf)));
    pbge::Manager::getInstance()->_setOpenGL(&ogl);
    
    pbge::VertexBufferBuilder builder(3);
    
    pbge::VertexAttribBuilder vertex = builder.addAttrib(3, pbge::VertexAttrib::VERTEX);
    std::vector<unsigned short> indexes;
    builder.pushValue(vertex, 1,2,3).pushValue(vertex, 2,3,4).pushValue(vertex,3,4,5);
    indexes.push_back(1);
    indexes.push_back(0);
    indexes.push_back(2);
    builder.setAttribIndex(vertex, indexes);
    builder.done();
    
    for(int i = 0; i < 9; i++) {
        ASSERT_FLOAT_EQ(expected[i], buf[i]);
    }
}

TEST_F(VertexBufferBuilderTest, builderBuildsCombinationVertexAndNormalIterleaved) {
    float buf[12];
    float expected[] = {1.0f,2.0f,3.0f,0.0f,1.0f,0.0f, 4.0f,5.0f,6.0f,1.0f,0.0f,1.0f};

    EXPECT_CALL(ogl, createBuffer(12,GL_DYNAMIC_DRAW,GL_ARRAY_BUFFER)).Times(1).WillOnce(Return(&buffer));
    EXPECT_CALL(buffer, map()).Times(1).WillOnce(Return((void*)(buf)));
    pbge::Manager::getInstance()->_setOpenGL(&ogl);

    pbge::VertexBufferBuilder builder(2);
    pbge::VertexAttribBuilder vertex = builder.addAttrib(3, pbge::VertexAttrib::VERTEX);
    pbge::VertexAttribBuilder normal = builder.addAttrib(3, pbge::VertexAttrib::NORMAL);
    builder.pushValue(vertex, 1,2,3);
    builder.pushValue(vertex, 4,5,6);
    builder.pushValue(normal, 1,0,1);
    builder.pushValue(normal, 0,1,0);
    std::vector<unsigned short> vertex_indexes;
    std::vector<unsigned short> normal_indexes;
    vertex_indexes.push_back(0);
    vertex_indexes.push_back(1);
    normal_indexes.push_back(1);
    normal_indexes.push_back(0);

    builder.setAttribIndex(vertex, vertex_indexes);
    builder.setAttribIndex(normal, normal_indexes);
    builder.done(GL_DYNAMIC_DRAW);
    for(int i = 0; i < 12; i++) {
        ASSERT_FLOAT_EQ(expected[i], buf[i]);
    }
}

TEST_F(VertexBufferBuilderTest, ifThereIsAAttribWithNoIndexVectorThenThrowsBuilderValidationException) {
    pbge::VertexBufferBuilder builder(2);
    pbge::VertexAttribBuilder vertex = builder.addAttrib(3, pbge::VertexAttrib::VERTEX);
    pbge::VertexAttribBuilder normal = builder.addAttrib(3, pbge::VertexAttrib::NORMAL);
    builder.pushValue(vertex, 1,2,3);
    builder.pushValue(vertex, 4,5,6);
    builder.pushValue(normal, 1,0,1);
    builder.pushValue(normal, 0,1,0);
    std::vector<unsigned short> vertex_indexes;
    vertex_indexes.push_back(0);
    vertex_indexes.push_back(1);

    builder.setAttribIndex(vertex, vertex_indexes);
    ASSERT_THROW(builder.done(GL_DYNAMIC_DRAW), pbge::BuilderValidationException);
}

TEST(VertexPositionAttribTest, bindCallsOpenGLWithCorrectParameters) {
    MockOpenGL ogl;
    EXPECT_CALL(ogl, vertexPointer(4, GL_FLOAT, 10, (GLbyte*)NULL + 10)).Times(1);
    EXPECT_CALL(ogl, enableClientState(GL_VERTEX_ARRAY)).Times(1);

    pbge::VertexPositionAttrib attrib = pbge::VertexPositionAttrib(4, 10, 10);
    attrib.bindAttrib(&ogl, NULL);
}

TEST(VertexNormalAttribTest, bindCallsOpenGLWithCorrectParameters) {
    MockOpenGL ogl;
    EXPECT_CALL(ogl, normalPointer(GL_FLOAT, 10, (GLbyte*)NULL + 10)).Times(1);
    EXPECT_CALL(ogl, enableClientState(GL_NORMAL_ARRAY)).Times(1);

    pbge::VertexNormalAttrib attrib = pbge::VertexNormalAttrib(10, 10);
    attrib.bindAttrib(&ogl, NULL);
}

TEST(VertexTexcoordAttribTest, bindCallsOpenGLWithCorrectParameters) {
    MockOpenGL ogl;
    EXPECT_CALL(ogl, texCoordPointer(4, GL_FLOAT, 10, (GLbyte*)NULL + 10)).Times(1);
    EXPECT_CALL(ogl, enableClientState(GL_TEXTURE_COORD_ARRAY)).Times(1);

    pbge::VertexTexcoordAttrib attrib = pbge::VertexTexcoordAttrib(4, 10, 10);
    attrib.bindAttrib(&ogl, NULL);
}

TEST(VertexColorAttribTest, bindCallsOpenGLWithCorrectParameters) {
    MockOpenGL ogl;
    EXPECT_CALL(ogl, colorPointer(4, GL_FLOAT, 10, (GLbyte*)NULL + 10)).Times(1);
    EXPECT_CALL(ogl, enableClientState(GL_COLOR_ARRAY)).Times(1);

    pbge::VertexColorAttrib attrib = pbge::VertexColorAttrib(4, 10, 10);
    attrib.bindAttrib(&ogl, NULL);
}

TEST(VertexSecondaryColorAttribTest, bindCallsOpenGLWithCorrectParameters) {
    MockOpenGL ogl;
    EXPECT_CALL(ogl, secondaryColorPointer(3, GL_FLOAT, 10, (GLbyte*)NULL + 10)).Times(1);
    EXPECT_CALL(ogl, enableClientState(GL_SECONDARY_COLOR_ARRAY)).Times(1);

    pbge::VertexSecondaryColorAttrib attrib = pbge::VertexSecondaryColorAttrib(4, 10, 10);
    attrib.bindAttrib(&ogl, NULL);
}