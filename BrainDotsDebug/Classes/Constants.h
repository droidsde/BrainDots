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
#define ITEMS_IN_PAGE                   6
#define PADDING                         10

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

enum TAG_MENU
{
    TAG_MENU_BUTTON_TITLE = 3000,
    TAG_MENU_STAGE_CHOOSE = 0,
	TAG_MENU_LEVEL_CHOOSE = 100,
};

enum TAG_GAME
{
    TAG_BUTTON_BACK = 10,
    TAG_BUTTON_REPLAY,
};

enum ZORDER_GAME
{
	ZORDER_MAPLEVEL = 0,
	ZORDER_BUTTON_BACK = 10,
	ZORDER_BUTTON_REPLAY = 11,
};
    
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
