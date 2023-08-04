#ifndef INPUT_H_
#define INPUT_H_

typedef enum Direction
{
    UNKNOW = 0,
    UP = 1,
    DOWN = 2,
    LEFT = 3,
    RIGHT = 4
} Direction;

// 根据用户的输入，返回一个方向值
Direction get_direction();

#endif