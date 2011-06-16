#ifndef PBGE_CORE_MOUSEEVENTHANDLER_H_
#define PBGE_CORE_MOUSEEVENTHANDLER_H_

namespace pbge {
    typedef enum { R_MOUSE_BUTTON, L_MOUSE_BUTTON } MouseButton;

    class MouseEventHandler {
    public:
        /*
            this method is called when a button is pressed
        */
        virtual bool buttonDown(MouseButton button, int x, int y) = 0;
        
        /*
            this method is called when a button is released
        */
        virtual bool buttonUp(MouseButton button, int x, int y) = 0;

        /*
            this method is called when mouse moves
        */
        virtual bool move(int x, int y) = 0;
    };
}

#endif