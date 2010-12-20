#include <SDL/SDL.h>
#include <GL/glew.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstdio>

#include "pbge/core/Manager.h"


namespace {
    pbge::Manager * manager = NULL;
    SDL_Surface * screen = NULL;
}

namespace pbge {
    
    class OpenGL;
    // This class should be instantiated ONLY by the initManager() function call
    Manager::Manager() {
        pbgeLog = new LogTerminal();
        // Get the installation directory of the PBGE_HOME
        char * installDir = getenv("PBGE_HOME");
        // Normalize the install dir
        if (installDir != NULL) {
            std::string pbgeShaderPath = std::string(installDir) + std::string("/shaders/");
            for(int i = 0; i < static_cast<int>(pbgeShaderPath.size()); i++) {
                if(pbgeShaderPath[i] == '\\')
                    pbgeShaderPath[i] = '/';
            }
            shaderDirectories.push_back(pbgeShaderPath);
        } else {
            pbgeLog->write("Environment Variable PBGE_HOME not found");
        }
        this->ogl = new OpenGL;
        shaderDirectories.push_back(std::string("./shaders/"));
    }

    void Manager::addShaderDirectory(const std::string &newDir) {
        if (newDir != "") {
            if (newDir[newDir.size() - 1] == '/')
                shaderDirectories.push_back(newDir);
            else
                shaderDirectories.push_back(newDir + std::string("/"));
        }
    }

    const std::vector<std::string> Manager::getShaderDirs() const{
        return shaderDirectories;
    }

    bool Manager::initializeOpenGL(OpenGLParameters parameters) {
        if(SDL_Init(SDL_INIT_VIDEO) != 0) {
            this->writeErrorLog("Unable to Initialize OpenGL");
            this->writeErrorLog(std::string(SDL_GetError()));
            return false;
        }
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, parameters.redSize);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, parameters.greenSize);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, parameters.blueSize);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, parameters.alphaSize);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, parameters.depthSize);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, parameters.stencilSize);
        screen = SDL_SetVideoMode(parameters.window_width,parameters.window_height, 32, SDL_OPENGL);
        if(screen == NULL) {
            this->writeErrorLog("Unable to Initialize OpenGL");
            this->writeErrorLog(std::string(SDL_GetError()));
            return false;
        }
        glewInit();
        return true;
    }

    void Manager::cleanUp() {
        if(screen) {
            SDL_FreeSurface(screen);
            SDL_Quit();
        }
    }

    Manager::~Manager() {
        this->writeLog("Saindo");
        cleanUp();
        SDL_Quit();
    }

    void Manager::init(bool test) {
        manager = new Manager();
    }
    
    Manager * Manager::getInstance() {
        if(manager == NULL) {
            manager = new Manager();
        }
        return manager;
    }
}