#pragma __GAMEBOX_H__

#include "game_info.h"

class GameBox
{
public:
    //构造函数
    GameBox();
    //析构函数
    ~GameBox();
    //游戏盒子的主函数
    void GameBox_main();
    
private:
    std::list<Game_Info> gameinfo;
    
};