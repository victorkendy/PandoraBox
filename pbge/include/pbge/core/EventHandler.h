#ifndef PBGE_CORE_EVENTHANDLER_H_
#define PBGE_CORE_EVENTHANDLER_H_

#include <vector>

#include "pbge/core/core.h"

namespace pbge {
    class Window;
    class KeyboardEventHandler;

    class PBGE_EXPORT EventHandler {
    public:
        EventHandler(Window * window);

        ~EventHandler();

        void handleResize(unsigned newWidth, unsigned newHeight);

        void handleKeyDown(char key);

        void handleKeyUp(char key);

        void addKeyboardHandler(KeyboardEventHandler * handler);
    private:
        Window * window;

        std::vector<KeyboardEventHandler *> keyboardHandlers;

    };
}

#endif