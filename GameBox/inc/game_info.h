#ifndef __GAME_INFO_H__
#define __GAME_INFO_H__

class Game_Info
{
public:
    Game_Info(std::string game_path, std::string game_picture);

private:
    std::string m_game_path;    // 游戏的路径
    std::string m_game_picture; // 游戏的图片路径
    int m_x;                    // 图片的x位置
    int m_y;                    // 图片的y坐标
    int m_width;                // 图片的宽度
    int m_height;                // 图片的高度
};

#endif