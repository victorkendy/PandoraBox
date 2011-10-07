#ifndef PBGE_CORE_MANAGER_H_
#define PBGE_CORE_MANAGER_H_

#include <boost/smart_ptr/scoped_ptr.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

#include "pbge/core/core.h"
#include "pbge/core/Window.h"

#include "pbge/gfx/GraphicAPI.h"

#include <string>
#include <vector>

namespace pbge {
    class SceneGraph;
    class SceneInitializer;
    class KeyboardEventHandler;

    class PBGE_EXPORT Manager{
    public:
        GraphicAPI * getGraphicAPI() {
            return ogl.get();
        }

    // public window initialization functions
    public:
        Manager();

        void setWindowDimensions(const unsigned & w, const unsigned & h);

        void setFullscreen(const bool & fullscreen);

        void setWindowTitle(const std::string title);

        void setMainSceneGraph(SceneGraph * sceneGraph);

        void setSceneInitializer(SceneInitializer * initializer);

        void setKeyboardEventHandler(KeyboardEventHandler * handler);

        void displayGraphics();

    private:
        boost::scoped_ptr<Window> window;
        boost::shared_ptr<GraphicAPI> ogl;
    };
}

#endif //pbge_manager
