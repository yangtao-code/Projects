#ifndef __GREEDYSNAKE_H__
#define __GREEDYSNAKE_H__

#include <iostream>
#include <list>
#include "Ground.h"

namespace snake
{
    // 蛇的方块信息
    struct Block
    {
    public:
        int x;
        int y;

    public:
        
    };


    class GreedySnake
    {
    private:
        std::list<Block> snake;
        Ground map; // 地图

        uint32_t head_color; // 蛇头的颜色
        uint32_t body_color; // 蛇身的颜色
        int speed;           // 蛇移动的速度

    public:

        // GreedySnake();
        // ~GreedySnake();
    };
};

#endif