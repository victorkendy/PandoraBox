#ifndef PBGE_GFX_KEYBOARDEVENTHANDLER
#define PBGE_GFX_KEYBOARDEVENTHANDLER

namespace pbge {
    class PBGE_EXPORT KeyboardEventHandler {
    public:
        /*
            this method is called when a key is pressed
        */
        virtual void keyDown(char c) = 0;
        
        /*
            this method is called when a key is released
        */
        virtual void keyUp(char c) = 0;
    };
}

#endif
