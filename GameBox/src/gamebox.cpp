#include <iostream>
#include <string>
#include <list>
#include <sys/types.h>
#include <dirent.h>

#include "gamebox.h"

#define GAMEPATH "./app/"
#define GAMEICON "./app/pic"

inline GameBox::GameBox()
{
    DIR* dirp = opendir(path.c_str);
    struct dirent d;
    while(d = readdir(dirp))
    {
        
    }
    
    //Game_Info g("") 
    this->gameinfo.push_back();
}