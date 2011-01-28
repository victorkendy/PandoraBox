#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <set>

#include "pbge/core/Manager.h"
#include "pbge/core/File.h"
#include "pbge/gfx/OpenGL.h"
#include "pbge/gfx/Shader.h"


using std::string;

namespace pbge {

    GLShader * GLShader::loadSourceFromFile(FileReader * file, const ShaderType _type) {
        if(!file->is_open())
            return false;
        GLShader * shader = new GLShader;
        shader->type = _type;
        shader->source = new char[file->getSize() + 1];
        strcpy(shader->source, file->getData());
        file->close();
        return shader;
    }

    GLShader * GLShader::loadSource(const std::string & source, const ShaderType _type) {
        GLShader * shader = new GLShader;
        shader->type = _type;
        shader->source = new char[source.size() + 1];
        strcpy(shader->source, source.c_str());
        return shader;
    }

    bool GLShader::compile(OpenGL * ogl) {
        GLint status;
        if(type == VERTEX_SHADER)
            shaderID = ogl->createShader(GL_VERTEX_SHADER);
        else if(type == FRAGMENT_SHADER)
            shaderID = ogl->createShader(GL_FRAGMENT_SHADER);
        else 
            return false;
        const GLchar * strPtr = const_cast<GLchar*>(source);
        ogl->shaderSource(shaderID, 1, &strPtr, NULL);
        ogl->compileShader(shaderID);
        ogl->getShaderiv(shaderID, GL_COMPILE_STATUS, &status);
        compiled = (status == GL_TRUE);
        return compiled;
    }


    void GLProgram::bind(OpenGL * ogl){
        if(!linked) {
            link(ogl);
            if(!linked)
                return;
        }
        ogl->useProgram(programID);
        updateUniforms(ogl);
    }

    void GLProgram::unbind(OpenGL * ogl){
        ogl->useProgram(0);
    }

    void GLProgram::updateUniforms(OpenGL * ogl) {
        std::set<UniformInfo>::iterator it;
        for(it = uniforms.begin(); it != uniforms.end(); it++) {
            ogl->getUniformValue(*it)->bindValueOn(this, *it, ogl);
        }
    }

    bool GLProgram::link(OpenGL * ogl){
        GLint status;
        std::vector<GLShader*>::iterator it;
        if(programID == 0) programID = ogl->createProgram();
        for(it = attachedShaders.begin(); it != attachedShaders.end(); it++) {
            if(!(*it)->isCompiled()) {
                if(!(*it)->compile(ogl))
                    return false;
                ogl->attachShader(programID, (*it)->getID());
            }
        }
        ogl->linkProgram(programID);
        extractInfoLog(ogl);
        ogl->getProgramiv(programID, GL_LINK_STATUS, &status);
        linked = (status == GL_TRUE);
        if(linked)
            extractUniformInformation(ogl);
        std::cout << this->getInfoLog();
        return linked;
    }

    void GLProgram::extractInfoLog(OpenGL * ogl) {
        GLint infoLogLength;
        GLsizei lixo;
        GLchar * _infoLog;
        ogl->getProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
        _infoLog = new GLchar[infoLogLength];
        ogl->getProgramInfoLog(programID, infoLogLength, &lixo, _infoLog);
        this->infoLog = std::string(_infoLog, infoLogLength);
        delete [] _infoLog;
    }

    void GLProgram::extractUniformInformation(OpenGL * ogl) {
        GLint numberOfActiveUniforms;
        GLint maxUniformNameSize;
        GLint uniformSize;
        GLenum uniformType;
        GLchar * name;
        ogl->useProgram(programID);
        ogl->getProgramiv(programID, GL_ACTIVE_UNIFORMS, &numberOfActiveUniforms);
        ogl->getProgramiv(programID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameSize);
        name = new GLchar [maxUniformNameSize];
        for(int uniformIndex = 0; uniformIndex < numberOfActiveUniforms; ++uniformIndex) {
            ogl->getActiveUniform(programID, uniformIndex, maxUniformNameSize, NULL, &uniformSize, &uniformType, name);
            std::string uniformName = name;
            // don't include reserved names
            if(static_cast<int>(uniformName.find("gl_")) != 0) {
                uniforms.insert(UniformInfo(uniformName, uniformType, ogl->getUniformLocation(programID, name)));
                std::cout << "found uniform: " << uniformName << std::endl;
            }
        }
        delete [] name;
    }

    void GLProgram::attachShader(GLShader *shader){
        attachedShaders.push_back(shader);
    }

    GLProgram * GLProgram::fromString(const string &vertexShader, const string &fragmentShader){
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
        GLShader *vs, *fs;
        if(filevs != NULL){
            vs = GLShader::loadSourceFromFile(filevs, Shader::VERTEX_SHADER);
        }
        if(filefs != NULL){
            fs = GLShader::loadSourceFromFile(filefs, Shader::FRAGMENT_SHADER);
        }
        GLProgram * program = new GLProgram;
        program->attachShader(vs);
        program->attachShader(fs);
        return program;
    }

    void GLProgram::bindFloat(const UniformInfo & info, OpenGL * ogl, const float & valor) {
        ogl->uniform1f(info.getLocation(), valor);
    }

    void GLProgram::bindFloatVec2(const UniformInfo & info, OpenGL * ogl, const float & v1, const float & v2) {
        ogl->uniform2f(info.getLocation(), v1, v2);
    }

    void GLProgram::bindFloatVec3(const UniformInfo & info, OpenGL * ogl, const float & v1, const float & v2, const float & v3) {
        ogl->uniform3f(info.getLocation(), v1, v2, v3);
    }

    void GLProgram::bindFloatVec4(const UniformInfo & info, OpenGL * ogl, const float & v1, const float & v2, const float & v3, const float & v4) {
        ogl->uniform4f(info.getLocation(), v1, v2, v3, v4);
    }
}
