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
