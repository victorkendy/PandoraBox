#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

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
    }

    void GLProgram::unbind(OpenGL * ogl){
        ogl->useProgram(0);
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
            if(static_cast<int>(uniformName.find("gl_")) != 0)
                uniforms[uniformName] = UniformInfo(uniformName, uniformType, ogl->getUniformLocation(programID, name));
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

    void GLProgram::bindFloat(const std::string & name, OpenGL * ogl, const float & valor) {
        GLint location = this->uniforms[name].getLocation();
        ogl->uniform1f(location, valor);
    }

    void GLProgram::bindFloatVec2(const std::string & name, OpenGL * ogl, const float & v1, const float & v2) {
        GLint location = this->uniforms[name].getLocation();
        ogl->uniform2f(location, v1, v2);
    }

    void GLProgram::bindFloatVec3(const std::string & name, OpenGL * ogl, const float & v1, const float & v2, const float & v3) {
        GLint location = this->uniforms[name].getLocation();
        ogl->uniform3f(location, v1, v2, v3);
    }

    void GLProgram::bindFloatVec4(const std::string & name, OpenGL * ogl, const float & v1, const float & v2, const float & v3, const float & v4) {
        GLint location = this->uniforms[name].getLocation();
        ogl->uniform4f(location, v1, v2, v3, v4);
    }
}
