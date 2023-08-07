#include <iostream>
#include <string>
#include "game_info.h"

Game_Info::Game_Info(std::string game_path, std::string game_picture)
{
    this->m_game_path = game_path;
    this->m_game_picture = game_picture;

    this->m_x = 0;
    this->m_y = 0;

    this->m_height = 0;
    this->m_width = 0;
}