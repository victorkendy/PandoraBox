#include <string>

#include "pbge/exceptions/exceptions.h"

#include "pbge/gfx/Model.h"
#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/GraphicObjectsFactory.h"
#include "pbge/gfx/ResourceStorage.h"
#include "pbge/gfx/Shader.h"
#include "pbge/gfx/ShaderHelper.h"

using namespace pbge;

ShaderHelper::ShaderHelper(GraphicAPI * _gfx) {
    this->gfx = _gfx;
    this->depthPassProgram = NULL;
    this->renderPassProgram = NULL;
}

ShaderHelper * ShaderHelper::create(GraphicAPI *gfx) {
    return new ShaderHelper(gfx);
}

ShaderHelper * ShaderHelper::withVertexShader(Shader *shader) {
    if(shader->getType() != Shader::VERTEX_SHADER) {
        throw IllegalArgumentException("Only Vertex Shaders allowed here");
    }
    vertexShaders.push_back(shader);
    return this;
}

ShaderHelper * ShaderHelper::withVertexShader(const std::string &src) {
    vertexShaders.push_back(gfx->getFactory()->createShaderFromString(src, Shader::VERTEX_SHADER));
    return this;
}

ShaderHelper * ShaderHelper::withNamedVertexShader(const std::string & name) {
    Shader * shader = searchShader(name);
    if(shader->getType() != Shader::VERTEX_SHADER) {
        throw IllegalArgumentException("Only Vertex Shaders allowed here");
    }
    vertexShaders.push_back(shader);
    return this;
}

ShaderHelper * ShaderHelper::withDepthPassShader(Shader *shader) {
    if(shader->getType() != Shader::FRAGMENT_SHADER) {
        throw IllegalArgumentException("Only Fragment Shaders allowed here");
    }
    depthPassShaders.push_back(shader);
    return this;
}

ShaderHelper * ShaderHelper::withDepthPassShader(const std::string &src) {
    depthPassShaders.push_back(gfx->getFactory()->createShaderFromString(src, Shader::FRAGMENT_SHADER));
    return this;
}

ShaderHelper * ShaderHelper::withNamedDepthPassShader(const std::string &name) {
    Shader * shader = searchShader(name);
    if(shader->getType() != Shader::FRAGMENT_SHADER) {
        throw IllegalArgumentException("Only Fragment Shaders allowed here");
    }
    depthPassShaders.push_back(shader);
    return this;
}

ShaderHelper * ShaderHelper::withRenderPassShader(pbge::Shader *shader) {
    if(shader->getType() != Shader::FRAGMENT_SHADER) {
        throw IllegalArgumentException("Only Fragment Shaders allowed here");
    }
    renderPassShaders.push_back(shader);
    return this;
}

ShaderHelper * ShaderHelper::withRenderPassShader(const std::string &src) {
    renderPassShaders.push_back(gfx->getFactory()->createShaderFromString(src, Shader::FRAGMENT_SHADER));
    return this;
}

ShaderHelper * ShaderHelper::withNamedRenderPassShader(const std::string &name) {
    Shader * shader = searchShader(name);
    if(shader->getType() != Shader::FRAGMENT_SHADER) {
        throw IllegalArgumentException("Only Fragment Shaders allowed here");
    }
    renderPassShaders.push_back(shader);
    return this;
}

Shader * ShaderHelper::searchShader(const std::string name) {
    return gfx->getStorage()->getNamedShader(name);
}

GPUProgram * ShaderHelper::createProgram(const std::string & name, std::vector<Shader *> & shaders) {
    if(shaders.size() > 0) {
        shaders.push_back(searchShader(name));
    }
    if(shaders.size() > 0 || vertexShaders.size() > 0) {
        return gfx->getFactory()->createProgram(vertexShaders, shaders);
    } else {
        return NULL;
    }
}

void ShaderHelper::setPrograms(ModelInstance * instance) {
    if(vertexShaders.size() > 0) {
        Shader * mainVertexShader = searchShader("pbge.defaultMainVertexShader");
        vertexShaders.push_back(mainVertexShader);
    }
    renderPassProgram = createProgram("pbge.defaultMainRenderPassShader", renderPassShaders);
    depthPassProgram = createProgram("pbge.defaultMainDepthPassShader", depthPassShaders);
    //renderPassProgram->link(gfx);
    //depthPassProgram->link(gfx);
    instance->setDepthPassProgram(depthPassProgram);
    instance->setRenderPassProgram(renderPassProgram);
}
