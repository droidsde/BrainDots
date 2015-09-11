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
#define LIST_ITEM_MARGIN                50
#define SCROLL_THRESHOLD                50
#define DELTA_TRANSLATE                 20
#define PADDING_MENU_HEADER_ITEM        40

#define PTM_RATIO 32.0 // 32px = 1m in Box2D

const float BALL_RADIUS = 25.0f;
const float OUTSIDE = 500;

// category definiti
const short CATEGORY_BALL = 0x0001;
const short CATEGORY_PLATFORM = 0x0002;
const short CATEGORY_BARRAGE = 0x0004;
const short CATEGORY_WALL1 = 0x0008;
const short CATEGORY_WALL2 = 0x0016;

// maskbit definiti
const short MASK_BALL = -1;
const short MASK_PLATFORM = CATEGORY_BALL | CATEGORY_BARRAGE | CATEGORY_PLATFORM | CATEGORY_WALL2;
const short MASK_BARRAGE = -1;
const short MASK_WALL1 = CATEGORY_BALL | CATEGORY_BARRAGE;
const short MASK_WALL2 = CATEGORY_BALL | CATEGORY_BARRAGE | CATEGORY_PLATFORM;

template <typename T>
std::string to_string(T value)
{
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}

enum ZORDER_MENU
{
    HEADER_LAYER = 0,
    HEADER_TITLE,
    HEADER_PENCIL,
    HEADER_SHARE,
    HEADER_SETTING,
    HEADER_POPUP_LAYER,
    
    BODY_LAYER = 0,
    BODY_LISTVIEW,
    BODY_STICKER,
    BODY_STICKER_TICK,
    BODY_STICKER_MINI_TICK,
    BODY_STICKER_TEXT,
};

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
    TAG_STAGE_CHOOSE = 0,
	TAG_LEVEL_CHOOSE = 100,
    TAG_BUTTON_TITLE = 3000,
    TAG_BUTTON_PENCIL,
    TAG_BUTTON_RECORD_VIDEO,
    TAG_BUTTON_SHARE,
    TAG_BUTTON_MENU,
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
