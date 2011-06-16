#ifndef PBGE_CORE_EVENTHANDLER_H_
#define PBGE_CORE_EVENTHANDLER_H_

#include <vector>

#include "pbge/core/core.h"
#include "pbge/core/MouseEventHandler.h"

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

		void handleButtonDown(MouseButton button, int x, int y);

		void handleButtonUp(MouseButton button, int x, int y);

		void handleMove(int x, int y);

        void addKeyboardHandler(KeyboardEventHandler * handler);

		void addMouseHandler(MouseEventHandler * handler);
    private:
        Window * window;

        std::vector<KeyboardEventHandler *> keyboardHandlers;

		std::vector<MouseEventHandler *> mouseHandlers;
    };
}

#endif