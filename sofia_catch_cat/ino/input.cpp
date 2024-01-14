#include "input.h"

bool input::getPressed()
{
    bool pressed = false;

    if (0 == digitalRead(JoyStick_Z))
    {
        pressed = true;
    }

    return pressed;
}
int input::getDir()
{
    int x, y;
    DIR dir = STILL;

    x=analogRead(JoyStick_X);
    y=analogRead(JoyStick_Y);

    if (x < 100)
    {
      dir |= UP;
    }
    if (x > 600)
    {
      dir |= DOWN;
    }
    if (y < 100)
    {
      dir |= RIGHT;
    }
    if (y > 600)
    {
      dir |= LEFT;
    }
    
    return dir;
}
input::input()
{
  pinMode(JoyStick_Z, INPUT);
}
