//
//  Constants.h
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/03.
//
//

#ifndef BrainDotsDebug_Constants_h
#define BrainDotsDebug_Constants_h

#pragma once

#define VERSION                         "version 1.0.0"

#define TILE_WITDH                      36
#define TILE_HEIGHT                     20

#define TILE_SIZE                       38

#define TIME_TRANSITION_SCENE           1.0

#define PAUSE_GAME                      "pauseGame"

#define LEVEL_MAX						10

template <typename T>
std::string to_string(T value)
{
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}

typedef enum class game_state
{
    LOGO = 0,
    SPLASH_SCREEN,
    MENU,
    SETTING,
    GAME,
    OVER,
    HIGHSCORE,
}GAME_STATE;

typedef enum class tag
{
	TAG_MENU_LEVEL_CHOOSE = 0,
}TAG_MENU;
struct UserData
{
    int level;
    int score;
    std::string name;
        
    UserData(const std::string& n, int l, int s) :  name(n) , level(l), score(s) {}
};
    
struct compare_score
{
    inline bool operator() (const UserData& struct1, const UserData& struct2)
    {
        return (struct1.score > struct2.score);
    }
};
    
struct compare_level
{
    inline bool operator() (const UserData& struct1, const UserData& struct2)
    {
        return (struct1.level > struct2.level);
    }
};
#endif
