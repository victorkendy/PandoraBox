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


    bool GLProgram::bind(OpenGL * ogl){
        if(!linked) {
            link(ogl);
            if(!linked)
                return false;
        }
        ogl->useProgram(programID);
        return true;
    }

    void GLProgram::unbind(OpenGL * ogl){
        ogl->useProgram(0);
    }

    bool GLProgram::link(OpenGL * ogl){
        GLint status;
        std::vector<GLShader*>::iterator it;
        for(it = attachedShaders.begin(); it != attachedShaders.end(); it++) {
            if(!(*it)->isCompiled()) {
                if(!(*it)->compile(ogl))
                    return false;
                ogl->attachShader(programID, (*it)->getID());
            }
        }
        ogl->linkProgram(programID);
        ogl->getProgramiv(programID, GL_LINK_STATUS, &status);
        linked = (status == GL_TRUE);
        return linked;
    }

    void GLProgram::attachShader(GLShader *shader){
        attachedShaders.push_back(shader);
    }

    GLProgram * GLProgram::fromString(const string &vertexShader, const string &fragmentShader){
        GLShader * vs, * fs;
        if(vertexShader != ""){
            vs = GLShader::loadSource(vertexShader, Shader::VERTEX_SHADER);
        }
        if(fragmentShader != ""){
            fs = GLShader::loadSource(fragmentShader, Shader::FRAGMENT_SHADER);
        }
        GLProgram * program = new GLProgram;
        program->attachShader(vs);
        program->attachShader(fs);
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
}
