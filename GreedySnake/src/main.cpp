#include <iostream>
#include "Screen.h"
#include "Rectangle.h"
#include "Touch.h"
#include "TouchEvent.h"
#include "Painter.h"
#include "Ground.h"
#include "BMPImage.h"
#include "JPGImage.h"
#include <list>
#include "List.hpp"

int main(int argc, char const *argv[])
{

    List<int> list(4);

    list.push_back(5);
    list.push_back(7);
    list.push_back(5);

    list.push_back(0);

    list.push_front(1000);

    std::cout << list << std::endl;
    // screen::Screen *s = screen::Screen::getInstance();

    // painter::Painter p(*s);

    // // 图片
    // JPGImage pic("./Image/1.jpg");

    // while (1)
    // {
    //     p.draw_Image(pic, 0, 0);
    //     sleep(2);
    //     pic.updateImage("./Image/101.jpg");
    //     p.draw_Image(pic, 0, 0);
    //     sleep(2);
    //     pic.updateImage("./Image/1.jpg");
    // }
    // rectangle::Rectangle rect(800, 480, 0x666666);

    // Ground g(680,480,20,0xffffff,0x00);

    // p.draw_Rectangle(&rect,0,0);
    // p.draw_Rectangle(&g,0,0);

    return 0;
}
