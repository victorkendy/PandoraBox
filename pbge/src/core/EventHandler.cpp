#include <iostream>

#include "pbge/core/KeyboardEventHandler.h"
#include "pbge/core/MouseEventHandler.h"
#include "pbge/core/EventHandler.h"

using namespace pbge;

EventHandler::EventHandler(Window * _window) {
    this->window = _window;
}

EventHandler::~EventHandler() {
    std::vector<KeyboardEventHandler *>::iterator keyboard_it;
    for(keyboard_it = keyboardHandlers.begin(); keyboard_it != keyboardHandlers.end(); keyboard_it++) { 
        delete (*keyboard_it);
    }
	std::vector<MouseEventHandler *>::iterator mouse_it;
    for(mouse_it = mouseHandlers.begin(); mouse_it != mouseHandlers.end(); mouse_it++) { 
        delete (*mouse_it);
    }
}

void EventHandler::handleResize(unsigned int newWidth, unsigned int newHeight) {
    std::cout << "resizing to: (" << newWidth << ", " << newHeight << ")" << std::endl;
}

void EventHandler::addKeyboardHandler(KeyboardEventHandler *handler) {
    keyboardHandlers.push_back(handler);
}

void EventHandler::addMouseHandler(MouseEventHandler *handler) {
    mouseHandlers.push_back(handler);
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

void EventHandler::handleButtonDown(MouseButton button, int x, int y) {
	std::vector<MouseEventHandler *>::iterator it;
    for(it = mouseHandlers.begin(); it != mouseHandlers.end(); it++) { 
		bool handled = (*it)->buttonDown(button, x, y);
        if(handled) {
            break;
        }
    }
}

void EventHandler::handleButtonUp(MouseButton button, int x, int y) {
	std::vector<MouseEventHandler *>::iterator it;
    for(it = mouseHandlers.begin(); it != mouseHandlers.end(); it++) { 
		bool handled = (*it)->buttonUp(button, x, y);
        if(handled) {
            break;
        }
    }
}

void EventHandler::handleMove(int x, int y) {
	std::vector<MouseEventHandler *>::iterator it;
    for(it = mouseHandlers.begin(); it != mouseHandlers.end(); it++) { 
		bool handled = (*it)->move(x, y);
        if(handled) {
            break;
        }
    }
}