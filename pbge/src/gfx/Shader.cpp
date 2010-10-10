#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <GL/glew.h>

#include "math3d/math3d.h"
#include "pbge/core/Manager.h"
#include "pbge/core/definitions.h"
#include "pbge/gfx/Shader.h"


using std::string;

namespace pbge {

    // TODO: Melhorar os tratamentos de erro

    static char _source[9*PBGE_KB];


    int GLShader::loadSourceFromFile(string fileName, const int type) {
        FILE *fp;
        Manager * manager = Manager::getInstance();
        std::vector<std::string> shaderDirectories = manager->getShaderDirs();
        std::vector<std::string>::iterator currentDir;
        for (currentDir = shaderDirectories.begin(); currentDir != shaderDirectories.end(); currentDir++) {
            fp = fopen((*currentDir + fileName).c_str(), "rb");
            if (fp != NULL)
                break;
        }
        if(fp == NULL) {
            fp = fopen(fileName.c_str(), "rb");
            if (fp == NULL) {
                manager->writeErrorLog("FILE NOT FOUND: " + fileName);
                return PBGE_false;
            }
        }
        memset(_source, 0, 9 * PBGE_KB);
        if(fread(_source, 1, 9*PBGE_KB, fp) == 9*PBGE_KB){
            fclose(fp);
            return PBGE_false;
        }else{
            fclose(fp);
        }
        return this->shaderFromPointer(_source, type);
    }

    int GLShader::loadSource(string source, const int type){
        return this->shaderFromPointer(source.c_str(), type);
    }

    int GLShader::shaderFromPointer(const char *ptr, const int type){
        GLint status;
        if(type == PBGE_vertex_shader)
            shaderID = glCreateShader(GL_VERTEX_SHADER);
        else if(type == PBGE_fragment_shader)
            shaderID = glCreateShader(GL_FRAGMENT_SHADER);
        else
            return PBGE_false;
        glShaderSource(shaderID, 1, &ptr, NULL);
        glCompileShader(shaderID);
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
        if(status == GL_FALSE){
            glGetShaderInfoLog(shaderID, 9000, &status, _source);
            puts(_source);
        }
        return PBGE_true;
    }

    int GLProgram::bind(){
        if(_isCompiled == PBGE_false) {
            return PBGE_false;
        }
        glUseProgram(programID);
        return PBGE_true;
    }
    void GLProgram::unbind(){
        glUseProgram(0);
    }
    int GLProgram::compile(){
        GLint status;
        glLinkProgram(programID);
        glGetProgramiv(programID, GL_LINK_STATUS, &status);
        if(status == GL_FALSE){
            glGetProgramInfoLog(programID, 9000, &status, _source);
            puts(_source);
            return PBGE_false;
        }
        _isCompiled = PBGE_true;
        return PBGE_true;
    }

    int GLProgram::attachShader(GLShader &shader){
        if(!programID)
            programID = glCreateProgram();
        if(shader.getID()){
            glAttachShader(programID, shader.getID());
            return PBGE_true;
        }
        else
            return PBGE_false;
    }

    int GLProgram::attachShader(GLShader *shader){
        return attachShader(*shader);
    }

    int GLProgram::compileFromString(const string &vertexShader, const string &fragmentShader){
        GLShader vs, fs;
        if(vertexShader != ""){
            vs.loadSource(vertexShader, PBGE_vertex_shader);
            this->attachShader(vs);
        }
        if(fragmentShader != ""){
            fs.loadSource(fragmentShader, PBGE_fragment_shader);
            this->attachShader(fs);
        }
        return this->compile();
    }

    int GLProgram::compileFromFile(const string &vertexShaderFile, const string &fragmentShaderFile){
        GLShader vs, fs;
        if(vertexShaderFile != ""){
            vs.loadSourceFromFile(vertexShaderFile, PBGE_vertex_shader);
            this->attachShader(vs);
        }
        if(fragmentShaderFile != ""){
            fs.loadSourceFromFile(fragmentShaderFile, PBGE_fragment_shader);
            this->attachShader(fs);
        }
        return this->compile();
    }
}