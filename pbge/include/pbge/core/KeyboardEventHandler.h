#ifndef PBGE_GFX_KEYBOARDEVENTHANDLER_H_
#define PBGE_GFX_KEYBOARDEVENTHANDLER_H_

namespace pbge {
    class KeyboardEventHandler {
    public:
        /*
            this method is called when a key is pressed
        */
        virtual bool keyDown(char key) = 0;
        
        /*
            this method is called when a key is released
        */
        virtual bool keyUp(char key) = 0;
    };
}

#endif
