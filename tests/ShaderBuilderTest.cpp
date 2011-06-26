
#include <vector>
#include <gtest/gtest.h>

#include "pbge/gfx/Shader.h"
#include "pbge/utils/ShaderBuilder.h"

TEST(ShaderStageParameter, isInputIfInIsEqualString_in) {
    pbge::ShaderStageParameter p("in", "vec3", "name");
    EXPECT_TRUE(p.isInput());
}

TEST(ShaderStageParameter, isNotInputIfInIsEqualString_out) {
    pbge::ShaderStageParameter p("out", "vec3", "name");
    EXPECT_FALSE(p.isInput());
}

bool parameterEqual(const pbge::ShaderStageParameter & p1, const pbge::ShaderStageParameter & p2) {
    return ((p1.getName() == p2.getName()) && (p1.getType() == p2.getType()) && (p1.isInput() == p2.isInput()));
}

TEST(ShaderBuilder, shouldParseASimplePrototype) {
    std::string code = 
        "uniform vec4 a_uniform_parameter;\n"
        "\n"
        "void entry(in vec4 vertex, in vec4 color, out vec4 position, out vec4 diffuseColor) {\n"
        "   position = vertex;\n"
        "   diffuseColor = color;\n"
        "}"
        ;
    pbge::ShaderBuilder shader(code, "entry", pbge::Shader::VERTEX_SHADER);
    const std::vector<pbge::ShaderStageParameter> & parameters = shader.getParameters();
    ASSERT_EQ(4, parameters.size());
    EXPECT_TRUE(parameterEqual(parameters.at(0), pbge::ShaderStageParameter("in","vec4","vertex")));
    EXPECT_TRUE(parameterEqual(parameters.at(1), pbge::ShaderStageParameter("in","vec4","color")));
    EXPECT_TRUE(parameterEqual(parameters.at(2), pbge::ShaderStageParameter("out","vec4","position")));
    EXPECT_TRUE(parameterEqual(parameters.at(3), pbge::ShaderStageParameter("out","vec4","diffuseColor")));
}

TEST(ShaderBuilder, shouldParseInterleavedParameters) {
    std::string code = 
        "uniform vec4 a_uniform_parameter;\n"
        "\n"
        "void entry(in vec4 vertex, out vec4 position, in vec4 color, out vec4 diffuseColor) {\n"
        "   position = vertex;\n"
        "   diffuseColor = color;\n"
        "}"
        ;
    pbge::ShaderBuilder shader(code, "entry", pbge::Shader::VERTEX_SHADER);
    const std::vector<pbge::ShaderStageParameter> & parameters = shader.getParameters();
    ASSERT_EQ(4, parameters.size());
    EXPECT_TRUE(parameterEqual(parameters.at(0), pbge::ShaderStageParameter("in","vec4","vertex")));
    EXPECT_TRUE(parameterEqual(parameters.at(1), pbge::ShaderStageParameter("out","vec4","position")));
    EXPECT_TRUE(parameterEqual(parameters.at(2), pbge::ShaderStageParameter("in","vec4","color")));
    EXPECT_TRUE(parameterEqual(parameters.at(3), pbge::ShaderStageParameter("out","vec4","diffuseColor")));
}

TEST(ShaderBuilder, shouldIgnoreExtraWhiteSpaces) {
    std::string code = 
        "   uniform vec4 a_uniform_parameter;   \n   "
        "\n"
        "    void       entry(    in    vec4     vertex,     in    vec4    \t   color,    out    vec4     position, out vec4 diffuseColor) \t{    \n"
        "   position = vertex;\n"
        "   diffuseColor = color;\n"
        "}"
        ;
    pbge::ShaderBuilder shader(code, "entry", pbge::Shader::VERTEX_SHADER);
    const std::vector<pbge::ShaderStageParameter> & parameters = shader.getParameters();
    ASSERT_EQ(4, parameters.size());
    EXPECT_TRUE(parameterEqual(parameters.at(0), pbge::ShaderStageParameter("in","vec4","vertex")));
    EXPECT_TRUE(parameterEqual(parameters.at(1), pbge::ShaderStageParameter("in","vec4","color")));
    EXPECT_TRUE(parameterEqual(parameters.at(2), pbge::ShaderStageParameter("out","vec4","position")));
    EXPECT_TRUE(parameterEqual(parameters.at(3), pbge::ShaderStageParameter("out","vec4","diffuseColor")));
}

TEST(ShaderBuilder, shouldIgnoreLineBreakOnParameterList) {
    std::string code = 
        "   uniform vec4 a_uniform_parameter;   \n   "
        "\n"
        "void entry(in vec4 vertex, \nin vec4 color, \nout vec4 position, \nout vec4 diffuseColor) { \n"
        "   position = vertex;\n"
        "   diffuseColor = color;\n"
        "}"
        ;
    pbge::ShaderBuilder shader(code, "entry", pbge::Shader::VERTEX_SHADER);
    const std::vector<pbge::ShaderStageParameter> & parameters = shader.getParameters();
    ASSERT_EQ(4, parameters.size());
    EXPECT_TRUE(parameterEqual(parameters.at(0), pbge::ShaderStageParameter("in","vec4","vertex")));
    EXPECT_TRUE(parameterEqual(parameters.at(1), pbge::ShaderStageParameter("in","vec4","color")));
    EXPECT_TRUE(parameterEqual(parameters.at(2), pbge::ShaderStageParameter("out","vec4","position")));
    EXPECT_TRUE(parameterEqual(parameters.at(3), pbge::ShaderStageParameter("out","vec4","diffuseColor")));
}