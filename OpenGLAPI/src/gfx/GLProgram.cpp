#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <set>

#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/Texture.h"
#include "pbge/gfx/states/TextureUnits.h"

#include "OpenGLAPI/gfx/GLShader.h"
#include "OpenGLAPI/gfx/GLProgram.h"
#include "OpenGLAPI/gfx/AttrBinders.h"


namespace {
    pbge::UniformType translateGLType(GLenum type) {
        switch(type) {
            case GL_FLOAT: return pbge::FLOAT; break;
            case GL_FLOAT_VEC2: return pbge::FLOAT_VEC2; break;
            case GL_FLOAT_VEC3: return pbge::FLOAT_VEC3; break;
            case GL_FLOAT_VEC4: return pbge::FLOAT_VEC4; break;
            case GL_INT: return pbge::INT; break;
            case GL_INT_VEC2: return pbge::INT_VEC2; break;
            case GL_INT_VEC3: return pbge::INT_VEC3; break;
            case GL_INT_VEC4: return pbge::INT_VEC4; break;
            case GL_BOOL: return pbge::BOOL; break;
            case GL_BOOL_VEC2: return pbge::BOOL_VEC2; break;
            case GL_BOOL_VEC3: return pbge::BOOL_VEC3; break;
            case GL_BOOL_VEC4: return pbge::BOOL_VEC4; break;
            case GL_FLOAT_MAT2: return pbge::FLOAT_MAT2; break;
            case GL_FLOAT_MAT3: return pbge::FLOAT_MAT3; break;
            case GL_FLOAT_MAT4: return pbge::FLOAT_MAT4; break;
            case GL_FLOAT_MAT2x3: return pbge::FLOAT_MAT23; break;
            case GL_FLOAT_MAT2x4: return pbge::FLOAT_MAT24; break;
            case GL_SAMPLER_BUFFER: return pbge::BUFFER_SAMPLER; break;
            case GL_SAMPLER_1D: return pbge::SAMPLER_1D; break;
            case GL_SAMPLER_2D: return pbge::SAMPLER_2D; break;
            case GL_SAMPLER_3D: return pbge::SAMPLER_3D; break;
            case GL_SAMPLER_CUBE: return pbge::SAMPLER_CUBE; break;
            case GL_SAMPLER_1D_SHADOW: return pbge::SAMPLER_1D_SHADOW; break;
            case GL_SAMPLER_2D_SHADOW: return pbge::SAMPLER_2D_SHADOW; break;
            default: return pbge::INVALID; break;
        }
    }
}

using namespace pbge;

void GLProgram::bind(GraphicAPI * gfx){
    if(!linked) {
        link(gfx);
        if(!linked)
            throw 1;
    }
    glUseProgram(programID);
    updateUniforms(gfx);
}

void GLProgram::unbind(GraphicAPI * ogl){
    std::for_each(binders.begin(), binders.end(), std::mem_fun(&AttrBinder::unbind));
    glUseProgram(0);
}

void GLProgram::updateUniforms(GraphicAPI * gfx) {
    std::vector<UniformInfo>::iterator it;
    for(it = uniforms.begin(); it != uniforms.end(); it++) {
        UniformValue * value = gfx->searchUniform(*it);
        if(value != NULL) {
            value->bindValueOn(this, *it, gfx);
        }
    }
}

void GLProgram::setAttributes(VertexBuffer * attr) {
    std::for_each(binders.begin(), binders.end(), std::bind2nd(std::mem_fun(&AttrBinder::bind), attr));
}

bool GLProgram::link(GraphicAPI * gfx){
    GLint status;
    std::vector<GLShader*>::iterator it;
    if(programID == 0) programID = glCreateProgram();
    for(it = attachedShaders.begin(); it != attachedShaders.end(); it++) {
        if(!(*it)->compile(gfx)) {
            std::cout << this->getInfoLog();
            return false;
        }
        glAttachShader(programID, (*it)->getID());
    }
    glLinkProgram(programID);
    extractInfoLog();
    glGetProgramiv(programID, GL_LINK_STATUS, &status);
    linked = (status == GL_TRUE);
    if(linked) {
        glUseProgram(programID);
        extractUniformInformation(gfx);
        extractAttribs();
    }
    return linked;
}

int GLProgram::getOutputLocation(const std::string & name) {
    if(name == "color") return 0;
    std::map<std::string, int>::iterator it = outputLocations.find(name);
    if(it == outputLocations.end()) {
        // TODO: verify gl version before using the getfragdatalocation
        int location = glGetFragDataLocation(programID, name.c_str());
        outputLocations[name] = location;
        return location;
    } else {
        return it->second;
    }
}

void GLProgram::extractInfoLog() {
    GLint infoLogLength;
    GLsizei lixo;
    GLchar * _infoLog;
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
    _infoLog = new GLchar[infoLogLength];
    glGetProgramInfoLog(programID, infoLogLength, &lixo, _infoLog);
    this->infoLog = std::string(_infoLog, infoLogLength);
    delete [] _infoLog;
}

void GLProgram::extractUniformInformation(GraphicAPI * ogl) {
    GLint numberOfActiveUniforms;
    GLint maxUniformNameSize;
    GLint uniformSize;
    GLenum uniformType;
    GLchar * name;
    glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &numberOfActiveUniforms);
    glGetProgramiv(programID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameSize);
    name = new GLchar [maxUniformNameSize];
    for(int uniformIndex = 0; uniformIndex < numberOfActiveUniforms; ++uniformIndex) {
        glGetActiveUniform(programID, uniformIndex, maxUniformNameSize, NULL, &uniformSize, &uniformType, name);
        std::string uniformName = name;
        // don't include reserved names
        if(static_cast<int>(uniformName.find("gl_")) != 0) {
            UniformInfo info = UniformInfo(uniformName, translateGLType(uniformType), glGetUniformLocation(programID, name), uniformSize);
            uniforms.push_back(info);
            std::cout << "found uniform: " << info.toString() << std::endl;
        }
    }
    delete [] name;
}

void GLProgram::extractAttribs() {
    GLint numberOfAttrs;
    GLint maxAttrNameSize;
    GLint attrSize;
    GLenum attrType;
    GLchar * name;
    glGetProgramiv(programID, GL_ACTIVE_ATTRIBUTES, &numberOfAttrs);
    glGetProgramiv(programID, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttrNameSize);
    name = new GLchar[maxAttrNameSize];
    for(int index = 0; index < numberOfAttrs; index++) {
        glGetActiveAttrib(programID, index, maxAttrNameSize, NULL, &attrSize, &attrType, name);
        AttrBinder * binder = AttrBinders::binderFor(name, glGetAttribLocation(programID, name));
        if(binder != NULL) { // some attributes are implementation dependent and aren't meant to be used by the api
            binders.push_back(binder);
        }
    }
    delete [] name;
}

void GLProgram::attachShader(GLShader *shader){
    attachedShaders.push_back(shader);
}

GLProgram * GLProgram::fromString(const std::string &vertexShader, const std::string &fragmentShader){
    GLShader * vs, * fs;
    GLProgram * program = new GLProgram;
    if(vertexShader != ""){
        vs = GLShader::loadSource(vertexShader, Shader::VERTEX_SHADER);
        program->attachShader(vs);
    }
    if(fragmentShader != ""){
        fs = GLShader::loadSource(fragmentShader, Shader::FRAGMENT_SHADER);
        program->attachShader(fs);
    }
    
    return program;
}

GLProgram * GLProgram::fromFile(FileReader * filevs, FileReader * filefs){
    GLProgram * program = new GLProgram;
    GLShader *vs, *fs;
    if(filevs != NULL){
        vs = GLShader::loadSourceFromFile(filevs, Shader::VERTEX_SHADER);
        program->attachShader(vs);
    }
    if(filefs != NULL){
        fs = GLShader::loadSourceFromFile(filefs, Shader::FRAGMENT_SHADER);
        program->attachShader(fs);
    }
    return program;
}

void GLProgram::bindFloat(const UniformInfo & info, GraphicAPI * ogl, const float * values, const unsigned size) {
    glUniform1fv(info.getLocation(), size, values);
}

void GLProgram::bindFloatVec2(const UniformInfo & info, GraphicAPI * ogl, const float * values, const unsigned size) {
    glUniform2fv(info.getLocation(), size, values);
}

void GLProgram::bindFloatVec3(const UniformInfo & info, GraphicAPI * ogl, const float * values, const unsigned size) {
    glUniform3fv(info.getLocation(), size, values);
}

void GLProgram::bindFloatVec4(const UniformInfo & info, GraphicAPI * ogl, const float * values, const unsigned size) {
    glUniform4fv(info.getLocation(), size, values);
}

void GLProgram::bindSampler(const UniformInfo & info, GraphicAPI * ogl, Texture * tex) {
    TextureUnit * unit = ogl->chooseTextureUnit(tex);
    unit->setTexture(tex);
    unit->makeChange(ogl);
    glUniform1i(info.getLocation(), unit->getIndex());
}

void GLProgram::bindBufferSampler(const UniformInfo & info, GraphicAPI * ogl, TextureBuffer * tex) {
    bindSampler(info, ogl, tex);
}

void GLProgram::bindSampler1D(const UniformInfo & info, GraphicAPI * ogl, Texture1D * tex) {
    bindSampler(info, ogl, tex);
}

void GLProgram::bindSampler2D(const UniformInfo & info, GraphicAPI * ogl, Texture2D * tex) {
    bindSampler(info, ogl, tex);
}

void GLProgram::bindMat4(const UniformInfo & info, GraphicAPI * ogl, const float * v) {
    glUniformMatrix4fv(info.getLocation(), 1, GL_FALSE, v);
}

