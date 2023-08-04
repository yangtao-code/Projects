#ifndef __2048__H__
#define __2048__H___

#include <iostream>
#include "Screen.h"
#include "Touch.h"
#include "Painter.h"
#include "TouchEvent.h"

// 2048游戏类
class Game_2048
{
private:
    static const uint32_t ROW = 4;     // 2048游戏棋盘行数
    static const uint32_t COL = 4;     // 2048游戏棋盘列数
    static const uint32_t WIN = 65535; // 当达到这个数时,玩家赢

    uint32_t board[ROW][COL]; // 游戏棋盘
    uint32_t score;           // 玩家的分数
    screen::Screen *s;        // 显示屏
    painter::Painter *p;      // 画笔
    touch::Touch *ts;         // 触摸屏
    tevent::TouchEvent *tev;  // 触摸事件
    bool shutdown;            // 游戏退出标志

public:
    // 游戏的主界面
    void game_main();
    // 游戏开始界面
    void playgame();

    Game_2048();
    ~Game_2048();

private:
    // 初始化游戏数据
    void InitGame();
    // 生成一个随机数
    void Get_randnum();
    // 打印界面
    void Playboard();

    /***
     * @brief 判断玩家有没有胜利
     *
     * @return true 胜利返回true
     * @return false 没有返回false
     */
    bool If_Win();

    /***
     * @brief 判断玩家有没有输
     *
     * @return true 输返回true
     * @return false 其他情况返回false
     */
    bool If_Lose();

    /***
     * @brief 判断是否结束游戏
     *
     * @return true 游戏结束返回true
     * @return false
     */
    bool If_game_over();

    /**
     * @brief 判断2048所有位置有没有满,返回1为已满
     *
     * @return int
     */
    bool If_Full();

    /***
     * @brief 判断2048还可不可以移动
     *
     * @return true 可以移动返回true
     * @return false 不可以移动返回false
     */
    bool If_Move();

    /**
     * @brief 关闭2048
     */
    void close_2048();
    // 保存一个棋盘的数据,以供后面判断是否生成一个随机数
    void copyboard(int (*tmp)[ROW]);

    /**
     * @brief 比较移动后的棋盘和移动前的棋盘,返回0为相等,返回1为不相等
     *
     * @return int
     */
    int cmpboard(int (*tmp)[ROW]);

    /****   上移模块   ****/
    void Move_up();
    void compress_up();
    void merge_up();

    /****   下移模块    ****/
    void Move_down();
    void compress_down();
    void merge_down();

    /****   左移模块    ****/
    void Move_left();
    void compress_left();
    void merge_left();

    /****   右移模块    ****/
    void Move_right();
    void compress_right();
    void merge_right();
};

#endif