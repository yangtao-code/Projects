#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lcd.h"
#include "ts.h"
#include "CoolAlbum.h"

int main(int argc, char const *argv[])
{
    init_lcd(); // 初始化屏幕
    open_ts();  // 打开触摸屏

    Linklist *list = create_piclist("./picture");

    // 电子相册的主要功能函数
    CoolAlbum(list);

    close_ts();             // 关闭触摸屏
    destroy_lcd();          // 关闭屏幕
    Destroy_Linklist(list); // 释放链表
    return 0;
}