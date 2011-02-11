#include <GL/glew.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstdio>

#include "pbge/core/Window.h"
#include "pbge/core/Manager.h"
#include "pbge/gfx/OpenGL.h"
#include "pbge/gfx/SceneGraph.h"


namespace {
    pbge::Manager * manager = NULL;
}

namespace pbge {
    
    class OpenGL;
    // This class should be instantiated ONLY by the initManager() function call
    Manager::Manager(bool test) {
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
        this->testConfiguration = test;
        this->ogl = new OpenGL;
        this->window = new Window;

        window->setOpenGL(ogl);
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

    Manager::~Manager() {
        delete ogl;
        this->writeLog("Saindo");
    }

    void Manager::init(bool test) {
        manager = new Manager(test);
    }
    
    Manager * Manager::getInstance() {
        if(manager == NULL) {
            manager = new Manager();
        }
        return manager;
    }

    void Manager::setWindowDimensions(const unsigned & w, const unsigned & h) {
        this->window->setWindowDimensions(w, h);
    }

    void Manager::setFullscreen(const bool & fullscreen) {
        this->window->setFullscreen(fullscreen);
    }

    void Manager::setWindowTitle(const std::string title) {
        this->window->setTitle(title);
    }

    void Manager::displayGraphics() {
        this->window->displayWindow();
    }

    void Manager::setSceneInitializer(SceneInitializer * initializer) {
        this->window->setSceneInitializer(initializer);
    }

    void Manager::setMainSceneGraph(SceneGraph * scene) {
        this->window->setScene(scene);
    }
}