#include "game.h"
#include "input.h"
#include "output.h"

#define ROW 4
#define COL 4

static int array[ROW][COL] = {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}};

// 游戏初始化
void game_init()
{
    // 在随机的位置生成一个随机的数字
    // 在随机的位置生成一个随机的数字
    // 显示界面
    show_ui(array, ROW, COL);
}
// 游戏主循环
void game_run()
{
    while (1)
    {
        // 获取一个方向
        Direction d = get_direction();
        // 根据用户输入的方向，操作二维数组
        switch (d)
        {
        case LEFT:
            move_left();
            break;
        case RIGHT:
            // move_right();
            break;
        case UP:
            // move_up();
            break;
        case DOWN:
            // move_down();
            break;
        default:
            break;
        }
        show_ui(array, ROW, COL);
    }
}
// 释放游戏资源
void game_close()
{
}

/*
2 2 2 2 =>  4 4 0 0
2 0 2 0 =>  4 0 0 0
2 0 0 2 =>  4 0 0 0 
0 0 0 2 =>  2 0 0 0

int b[4] = {2, 2, 2, 2}; => {4, 0, 4, 0}

*/

void move_left()
{
    // 压缩
    compress_left();
    // 合并
    merge_left();
    // 压缩
    compress_left();
}


void merge_left()
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL-1; j++)
        {
            if (array[i][j] == array[i][j+1])
            {
                array[i][j] *= 2;
                array[i][j+1] = 0;
            }
        }
    }
}

void compress_left()
{
    for (int i = 0; i < ROW; i++)
    {
        int tmp[COL] = {0};
        int k = 0;
        // 如果该行的元素的值非0，写入到tmp数组
        for (int j = 0; j < COL; j++)
        {
            if (array[i][j] != 0)
            {
                tmp[k++] = array[i][j];
            }
        }
        // 把tmp数组的值覆盖掉二维数组对应的行
        for (int j = 0; j < COL; j++)
        {
            array[i][j] = tmp[j];
        }
    }
}


