#include "2048.h"

void Game_2048::Move_right()
{
    int tmp[ROW][COL];
    copyboard(tmp);
    // 先移动位置
    compress_right();
    // 合并
    merge_right();
    // 再移动位置
    compress_right();

    if (cmpboard(tmp) != 0)
        Get_randnum();
}

void Game_2048::compress_right()
{
    for (int i = 0; i < ROW; i++)
    {
        int tmp[COL] = {0};
        int k = 0;
        // 如果该行非零的元素,存入数组tmp中
        for (int j = COL - 1; j >= 0; j--)
        {
            if (board[i][j] != 0)
            {
                tmp[k++] = board[i][j];
            }
        }
        // 将tmp中的元素放回该行
        for (int j = 0; j < COL; j++)
        {
            board[i][COL - j - 1] = tmp[j];
        }
    }
}

void Game_2048::merge_right()
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = COL - 1; j > 0; j--)
        {
            if (board[i][j] == board[i][j - 1])
            {
                board[i][j] = 2 * board[i][j];
                board[i][j - 1] = 0;
                score = score + board[i][j];
            }
        }
    }
}

void Game_2048::Move_left()
{
    int tmp[ROW][COL];
    copyboard(tmp);
    // 先移动位置
    compress_left();
    // 合并
    merge_left();
    // 再移动位置
    compress_left();

    if (cmpboard(tmp) != 0)
        Get_randnum();
}

void Game_2048::compress_left()
{
    for (int i = 0; i < ROW; i++)
    {
        int tmp[COL] = {0};
        int k = 0;
        // 如果该行非零的元素,存入数组tmp中
        for (int j = 0; j < COL; j++)
        {
            if (board[i][j] != 0)
            {
                tmp[k++] = board[i][j];
            }
        }
        // 将tmp中的元素放回该行
        for (int j = 0; j < COL; j++)
        {
            board[i][j] = tmp[j];
        }
    }
}

void Game_2048::merge_left()
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL - 1; j++)
        {
            if (board[i][j] == board[i][j + 1])
            {
                board[i][j] = 2 * board[i][j];
                board[i][j + 1] = 0;
                score = score + board[i][j];
            }
        }
    }
}

void Game_2048::Move_down()
{
    int tmp[ROW][COL];
    copyboard(tmp);
    // 先移动位置
    compress_down();
    // 合并
    merge_down();
    // 再移动位置
    compress_down();

    if (cmpboard(tmp) != 0)
        Get_randnum();
}

void Game_2048::compress_down()
{
    for (int j = 0; j < COL; j++)
    {
        int tmp[COL] = {0};
        int k = 0;
        // 如果该列的元素非零,存入数组tmp中
        for (int i = ROW - 1; i >= 0; i--)
        {
            if (board[i][j] != 0)
            {
                tmp[k++] = board[i][j];
            }
        }
        // 将tmp中的元素放回该列
        for (int i = 0; i < ROW; i++)
        {
            board[ROW - i - 1][j] = tmp[i];
        }
    }
}

void Game_2048::merge_down()
{
    for (int j = 0; j < COL; j++)
    {
        for (int i = ROW - 1; i > 0; i--)
        {
            if (board[i][j] == board[i - 1][j])
            {
                board[i][j] = 2 * board[i][j];
                board[i - 1][j] = 0;
                score = score + board[i][j];
            }
        }
    }
}

void Game_2048::Move_up()
{
    int tmp[ROW][COL];
    copyboard(tmp);
    // 先移动位置
    compress_up();
    // 合并
    merge_up();
    // 再移动位置
    compress_up();

    if (cmpboard(tmp) != 0)
        Get_randnum();
}

void Game_2048::compress_up()
{
    for (int j = 0; j < COL; j++)
    {
        int tmp[COL] = {0};
        int k = 0;
        // 如果该列的元素非零,存入数组tmp中
        for (int i = 0; i < ROW; i++)
        {
            if (board[i][j] != 0)
            {
                tmp[k++] = board[i][j];
            }
        }
        // 将tmp中的元素放回该列
        for (int i = 0; i < ROW; i++)
        {
            board[i][j] = tmp[i];
        }
    }
}

void Game_2048::merge_up()
{
    for (int j = 0; j < COL; j++)
    {
        for (int i = 0; i < ROW - 1; i++)
        {
            if (board[i][j] == board[i + 1][j])
            {
                board[i][j] = 2 * board[i][j];
                board[i + 1][j] = 0;
                score = score + board[i][j];
            }
        }
    }
}

void Game_2048::copyboard(int (*tmp)[ROW])
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            tmp[i][j] = board[i][j];
        }
    }
}

int Game_2048::cmpboard(int (*tmp)[ROW])
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            if (tmp[i][j] != board[i][j])
                return 1;
        }
    }
    return 0;
}
