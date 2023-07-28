#include <iostream>
using namespace std;

#include "Screen.h"
#include "Rectangle.h"
#include "Touch.h"
#include "TouchEvent.h"
#include "Painter.h"
#include "Ground.h"

int main(int argc, char const *argv[])
{
    screen::Screen *s = screen::Screen::getInstance();

    painter::Painter p(*s);
    rectangle::Rectangle rect(800, 480, 0xffff00);

    Ground g(680,480,20,0xffffff,0xff00);
    
    p.draw_Rectangle(&rect,0,0);
    p.draw_Rectangle(&g,0,0);

    return 0;
}
