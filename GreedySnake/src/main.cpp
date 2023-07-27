#include <iostream>
using namespace std;

#include "Screen.h"
#include "Rectangle.hpp"
#include "Touch.h"
#include "TouchEvent.h"

int main(int argc, char const *argv[])
{
    screen::Screen *s = screen::Screen::getInstance();

    rectangle::Rectangle rect(800, 480, 0x00);
    rect.draw(*s, 0, 0);

    touch::Touch *ts = touch::Touch::getInstance();

    tevent::TouchEvent tev;
    int i = 10;
    while (i--)
    {
        tev.event(*ts);
    }

    s->draw_Rectangle(800, 480, 0, 0, 0xff0000);

    return 0;
}
