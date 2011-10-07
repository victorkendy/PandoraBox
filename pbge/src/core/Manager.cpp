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
        ogl.reset(GraphicAPI::createInstance());
        window->setGraphicAPI(ogl);
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
}
