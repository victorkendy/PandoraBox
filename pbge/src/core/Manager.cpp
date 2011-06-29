#include <GL/glew.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstdio>

#include "pbge/core/Window.h"
#include "pbge/core/Manager.h"
#include "pbge/gfx/GraphicAPI.h"
#include "pbge/gfx/SceneGraph.h"


namespace pbge {
    
    Manager::Manager() : window(new Window){
        log = new LogTerminal();
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
            log->write("Environment Variable PBGE_HOME not found");
        }
        ogl.reset(GraphicAPI::createInstance());
        window->setGraphicAPI(ogl);
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
        delete log;
    }

    void Manager::setWindowDimensions(const unsigned & w, const unsigned & h) {
        window->setWindowDimensions(w, h);
    }

    void Manager::setFullscreen(const bool & fullscreen) {
        window->setFullscreen(fullscreen);
    }

    void Manager::setWindowTitle(const std::string title) {
        window->setTitle(title);
    }

    void Manager::displayGraphics() {
        window->displayWindow();
    }

    void Manager::setSceneInitializer(SceneInitializer * initializer) {
        window->setSceneInitializer(initializer);
    }

    void Manager::setKeyboardEventHandler(KeyboardEventHandler * handler) {
        window->setKeyboardEventHandler(handler);
    }

    void Manager::setMainSceneGraph(SceneGraph * scene) {
        window->setScene(scene);
    }

    void Manager::printDebugInformation(const bool & showInfo) {
        window->setShowDebug(showInfo);
    }
}
