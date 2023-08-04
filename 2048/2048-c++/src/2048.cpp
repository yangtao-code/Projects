#include "2048.h"
#include "BMPImage.h"
#include "TouchEvent.h"
#include <thread>
#include <string>
#include <cstring>

// 游戏的主界面
void Game_2048::game_main()
{
    while (!shutdown)
    {
        BMPImage menu("./pic/menu.bmp");
        p->draw_Image(menu, 0, 0);
        tev->event(*this->ts);
        if (tev->get_state() == tevent::CLICK &&
            tev->get_y() >= 410 && tev->get_y() <= 440)
        {
            if (tev->get_x() >= 210 && tev->get_x() <= 380)
            {
                this->playgame();
            }
            else if (tev->get_x() >= 420 && tev->get_x() <= 580)
            {
                close_2048();
            }
        }
    }
}

void Game_2048::playgame()
{
    srand((unsigned int)time(NULL));
    // 初始化游戏数据
    InitGame();

    BMPImage pic("./pic/backdrop.bmp"); // 背景图片
    p->draw_Image(pic, 0, 0);
    pic.updateImage("./pic/return.bmp"); // 返回键图片
    p->draw_Image(pic, 750, 430);
    // 打印棋盘
    Playboard();
    while (!shutdown)
    {
        tev->event(*this->ts);

        if (tev->get_state() == tevent::CLICK && tev->get_x() >= 750 && tev->get_x() < 800 &&
            tev->get_y() >= 430 && tev->get_y() < 480)
        {
            break;
        }
        switch (tev->get_state())
        {
        case tevent::UP:
            Move_up();
            break;
        case tevent::DOWN:
            Move_down();
            break;
        case tevent::LEFT:
            Move_left();
            break;
        case tevent::RIGHT:
            Move_right();
            break;
        default:
            continue;
        }
        // 打印棋盘
        Playboard();
        if (If_game_over())
        {
            std::cout<<"游戏结束!"<<std::endl;
            break;
        }
    }
}

bool Game_2048::If_game_over()
{
    if (If_Win())
    {
        printf("恭喜您赢了!\n");
        return true;
    }
    if (If_Lose())
    {
        printf("很遗憾,您输了!\n");
        return true;
    }
    return false;
}

bool Game_2048::If_Lose()
{
    if (If_Full() && !If_Move())
    {
        return true;
    }
    return false;
}

bool Game_2048::If_Move()
{
    for (int i = 1; i < ROW - 1; i++)
    {
        if (board[i][0] == board[i - 1][0] || board[i][0] == board[i + 1][0])
            return true;
        if (board[i][COL - 1] == board[i - 1][COL - 1] || board[i][COL - 1] == board[i + 1][COL - 1])
            return true;
    }
    for (int j = 1; j < COL - 1; j++)
    {
        if (board[0][j] == board[0][j - 1] || board[0][j] == board[0][j + 1])
            return true;
        if (board[ROW - 1][j] == board[ROW - 1][j - 1] || board[ROW - 1][j] == board[ROW - 1][j + 1])
            return true;
    }
    for (int i = 1; i < ROW - 1; i++)
    {
        for (int j = 1; j < COL - 1; j++)
        {
            if (board[i][j] == board[i - 1][j])
                return true;
            if (board[i][j] == board[i + 1][j])
                return true;
            if (board[i][j] == board[i][j - 1])
                return true;
            if (board[i][j] == board[i][j + 1])
                return true;
        }
    }
    return false;
}

bool Game_2048::If_Full()
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            if (board[i][j] == 0)
                return false;
        }
    }
    return true;
}

bool Game_2048::If_Win()
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            if (board[i][j] == WIN)
                return true;
        }
    }
    return false;
}

void Game_2048::Playboard()
{
    printf("****  2048小游戏  ****\n");
    printf("        当前分数:%d\n", score);
    printf("_____________________\n");
    for (int i = 0; i < ROW; i++)
    {
        printf("|    |    |    |    |\n|");
        for (int j = 0; j < COL; j++)
        {

            printf("%-4d|", board[i][j]);
        }
        printf("\n");
        printf("|___________________|\n");
    }

    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            std::string pic_name;
            pic_name = "./pic/" + std::to_string(board[i][j]) + ".bmp";
            BMPImage pic(pic_name);
            p->draw_Image(pic, 160 + j * 120, 0 + i * 120);
        }
    }
}

void Game_2048::InitGame()
{
    bzero(board, ROW * COL * 4);
    score = 0;
    Get_randnum();
    Get_randnum();
}

void Game_2048::Get_randnum()
{
    if (If_Full() == 1)
        return;
    int rand_num[3] = {2, 2, 4};
    int row, col, k;
    while (1)
    {
        row = rand() % ROW;
        col = rand() % COL;
        if (board[row][col] == 0)
        {
            k = rand() % 3;
            board[row][col] = rand_num[k];
            break;
        }
    }
}


void Game_2048::close_2048()
{
    shutdown = true;
}

Game_2048::Game_2048()
    : s(nullptr), p(nullptr),
      ts(nullptr), tev(nullptr),
      score(0), shutdown(false)
{
    this->s = screen::Screen::getInstance();
    this->p = new painter::Painter(*s);
    this->ts = touch::Touch::getInstance();
    this->tev = new tevent::TouchEvent();
}

Game_2048::~Game_2048()
{
    // 释放显示屏、触摸屏、画家、触摸事件
    delete tev;
    delete p;
    delete ts;
    delete s;
}