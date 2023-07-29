#include <iostream>

#include <unistd.h>
#include "Screen.h"
#include "Rectangle.h"
#include "Touch.h"
#include "TouchEvent.h"
#include "Painter.h"
#include "Ground.h"
#include "BMPImage.h"

int main(int argc, char const *argv[])
{
    screen::Screen *s = screen::Screen::getInstance();

    painter::Painter p(*s);

    // 图片
    BMPImage pic("./Image/104.bmp");

    while (1)
    {
        p.draw_Image(pic, 0, 0);
        sleep(2);
        pic.updateImage("./Image/101.bmp");
        p.draw_Image(pic, 0, 0);
        sleep(2);
        pic.updateImage("./Image/104.bmp");
    }

    // rectangle::Rectangle rect(800, 480, 0x666666);

    // Ground g(680,480,20,0xffffff,0x00);

    // p.draw_Rectangle(&rect,0,0);
    // p.draw_Rectangle(&g,0,0);

    return 0;
}
