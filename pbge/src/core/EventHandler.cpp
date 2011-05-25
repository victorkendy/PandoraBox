#include <iostream>

#include "pbge/gfx/KeyboardEventHandler.h"
#include "pbge/core/EventHandler.h"

using namespace pbge;

EventHandler::EventHandler(Window * _window) {
    this->window = _window;
}

void EventHandler::handleResize(unsigned int newWidth, unsigned int newHeight) {
    std::cout << "resizing to: (" << newWidth << ", " << newHeight << ")" << std::endl;
}

void EventHandler::addKeyboardHandler(KeyboardEventHandler *handler) {
    keyboardHandlers.push_back(handler);
}

void EventHandler::handleKeyDown(char key) {
    std::vector<KeyboardEventHandler *>::iterator it;
    for(it = keyboardHandlers.begin(); it != keyboardHandlers.end(); it++) { 
        bool handled = (*it)->keyDown(key);
        if(handled) {
            break;
        }
    }
}

void EventHandler::handleKeyUp(char key) {
    std::vector<KeyboardEventHandler *>::iterator it;
    for(it = keyboardHandlers.begin(); it != keyboardHandlers.end(); it++) { 
        bool handled = (*it)->keyUp(key);
        if(handled) {
            break;
        }
    }
}