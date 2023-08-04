#include "output.h"
#include <stdio.h>
#include <stdlib.h>

void show_ui(int (*array)[4], int row, int col)
{
    // 清理屏幕
    system("clear");
    // 再显示新的数据
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            printf("%d  ", array[i][j]);
        }
        printf("\n");
    }
}