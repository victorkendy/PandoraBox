

#ifndef PBGE_CORE_WINDOW_H_
#define PBGE_CORE_WINDOW_H_

#include <string>

namespace pbge {
    class GraphicAPI;
    class Renderer;
    class SceneGraph;
    class SceneInitializer;
    class KeyboardEventHandler;

    class Window {
    public:
        Window () {
            showDebug = false;
            fullscreen = false;
            width = 500;
            height = 500;
            positionX = 0;
            positionY = 0;
            windowTitle = "";
            renderer = NULL;
            ogl = NULL;
            scene = NULL;
            initializer = NULL;
            keyboardEventHandler = NULL;
        }

        ~Window();
        
        void setWindowDimensions(const unsigned & w, const unsigned & h) {
            width = w;
            height = h;
        }

        void setFullscreen(const bool & _fullscreen) {
            fullscreen = _fullscreen;
        }

        void setTitle(const std::string & title) {
            windowTitle = title;
        }

        void setGraphicAPI(GraphicAPI * openGL) {
            this->ogl = openGL;
        }

        GraphicAPI * getGraphicAPI () {
            return ogl;
        }

        Renderer * getRenderer () {
            return renderer;
        }

        void setScene(SceneGraph * sceneGraph) {
            scene = sceneGraph;
        }

        SceneGraph * getScene() {
            return scene;
        }

        void setSceneInitializer(SceneInitializer * newInitializer) {
            initializer = newInitializer;
        }

        SceneInitializer * getSceneInitializer() {
            return initializer;
        }

        void setKeyboardEventHandler(KeyboardEventHandler * newHandler) {
            keyboardEventHandler = newHandler;
        }

        KeyboardEventHandler * getKeyboardEventHandler() {
            return keyboardEventHandler;
        }

        void setShowDebug(const bool & show) {
            this->showDebug = show;
        }

        bool getShowDebug() {
            return showDebug;
        }

        void displayWindow();

    private:
        std::string windowTitle;

        unsigned width;
        
        unsigned height;

        unsigned positionX;

        unsigned positionY;

        bool fullscreen;

        bool showDebug;

        Renderer * renderer;

        GraphicAPI * ogl;

        SceneGraph * scene;

        SceneInitializer * initializer;

        KeyboardEventHandler * keyboardEventHandler;
    };
}
#endif