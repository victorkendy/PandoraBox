

#ifndef PBGE_CORE_WINDOW_H_
#define PBGE_CORE_WINDOW_H_

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/scoped_ptr.hpp>

#include <string>
#include "pbge/core/EventHandler.h"

namespace pbge {
    class GraphicAPI;
    class Renderer;
    class SceneGraph;
    class SceneInitializer;
    class KeyboardEventHandler;
	class MouseEventHandler;

    class Window {
    public:
        Window() {
            showDebug = false;
            fullscreen = false;
            width = 500;
            height = 500;
            positionX = 0;
            positionY = 0;
            windowTitle = "";
            renderer = NULL;
            initializer = NULL;
            handler.reset(new EventHandler(this));
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

        void setGraphicAPI(boost::shared_ptr<GraphicAPI> & gfx) {
            this->ogl = gfx;
        }

        GraphicAPI * getGraphicAPI () {
            return ogl.get();
        }

        Renderer * getRenderer () {
            return renderer;
        }

        void setScene(SceneGraph * sceneGraph) {
            scene.reset(sceneGraph);
        }

        boost::shared_ptr<SceneGraph> & getScene() {
            return scene;
        }

        void setSceneInitializer(SceneInitializer * newInitializer) {
            initializer = newInitializer;
        }

        SceneInitializer * getSceneInitializer() {
            return initializer;
        }

        void setKeyboardEventHandler(KeyboardEventHandler * newHandler) {
            handler->addKeyboardHandler(newHandler);
        }

		void setMouseEventHandler(MouseEventHandler * newHandler) {
            handler->addMouseHandler(newHandler);
        }

        void setShowDebug(const bool & show) {
            this->showDebug = show;
        }

        bool getShowDebug() {
            return showDebug;
        }

        void displayWindow();

        EventHandler * getEventHandler() {
            return handler.get();
        }

    private:
        std::string windowTitle;

        unsigned width;
        
        unsigned height;

        unsigned positionX;

        unsigned positionY;

        bool fullscreen;

        bool showDebug;

        Renderer * renderer;

        boost::shared_ptr<GraphicAPI> ogl;

        boost::shared_ptr<SceneGraph> scene;

        SceneInitializer * initializer;

        boost::scoped_ptr<EventHandler> handler;
    };
}
#endif