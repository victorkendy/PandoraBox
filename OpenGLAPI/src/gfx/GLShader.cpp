#include "OpenGLAPI/gfx/GLShader.h"
#include "pbge/core/File.h"

using namespace pbge;

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

bool GLShader::compile(GraphicAPI * gfx) {
    GLint status;
    if(shaderID == 0) { 
        if(type == VERTEX_SHADER)
            shaderID = glCreateShader(GL_VERTEX_SHADER);
        else if(type == FRAGMENT_SHADER)
            shaderID = glCreateShader(GL_FRAGMENT_SHADER);
        else 
            return false;
    }
    const GLchar * strPtr = const_cast<GLchar*>(source);
    glShaderSource(shaderID, 1, &strPtr, NULL);
    glCompileShader(shaderID);
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
    compiled = (status == GL_TRUE);
    extractInfolog(gfx);
    return compiled;
}

void GLShader::extractInfolog(GraphicAPI * gfx) {
    GLint infoLogLength;
    GLchar * _infolog;
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    _infolog = new GLchar[infoLogLength];
    glGetShaderInfoLog(shaderID, infoLogLength, NULL, _infolog);
    this->infoLog = std::string(_infolog);
    delete [] _infolog;
}
