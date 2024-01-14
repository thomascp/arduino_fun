#ifndef INPUT_H
#define INPUT_H

#include "Arduino.h"

class input {
    public:
        enum DIR
        {
            STILL   = 0,
            UP      = (1 << 1),
            DOWN    = (1 << 2),
            LEFT    = (1 << 3),
            RIGHT   = (1 << 4),
            
        };
        bool getPressed();
        int getDir();
        input();

    private:
        int JoyStick_X = A1; //x
        int JoyStick_Y = A0; //y 
        int JoyStick_Z = 5; //key
};

#endif