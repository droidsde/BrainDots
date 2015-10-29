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
#include "Box2D/Box2D.h"

#define VERSION                         "version 1.0.0"

#define TILE_WITDH                      36
#define TILE_HEIGHT                     20

#define TILE_SIZE                       38

#define TIME_TRANSITION_SCENE           0.5
#define TIME_LOGO_FADE                  1.0
#define TIME_LOADING                    0.0
#define TIME_DELAY_GAMEOVER             2.0

#define PAUSE_GAME                      "pauseGame"
#define RELOAD_LANGUAGE                 "reloadLanguage"
#define EXIT_LAYER_LANGUAGE             "exitLayerLanguage"
#define OPEN_LAYER_LANGUAGE             "openLayerLanguage"
#define EXIT_LAYER_SETTING              "exitLayerSetting"
#define EXIT_LAYER_MENU                 "exitLayerMenu"
#define EXIT_LAYER_INFO                 "exitLayerInfo"
#define EXIT_LAYER_NOTI                 "exitLayerNoti"

#define LEVEL_MAX						10
#define LEVEL_DEFAULT_UNLOCKED          1
#define ITEMS_IN_PAGE                   6
#define PADDING                         10
#define LIST_BIG_ITEM_MARGIN            100
#define LIST_SMALL_ITEM_MARGIN          50
#define SCROLL_THRESHOLD                50
#define DELTA_TRANSLATE                 20
#define PADDING_MENU_HEADER_ITEM        40

#define PENCIL_MAX                      20
#define MAX_SCALE_PENCIL                2.2
#define MIN_SCALE_PENCIL                1.0
#define PENCIL_DEFAULT_UNLOCK           5
#define DEFAULT_COIN                    100
#ifndef DEGTORAD
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#endif

#define PTM_RATIO 32.0 // 32px = 1m in Box2D
#define MAX_BLAST_RAYS 128

const float BALL_RADIUS = 25.0f;
const float BASE_EXPLOSION_SPACE = 200.0f;
const int   NUM_EXPLOSION_CIRCLE = 60;
const int   EXPLOSION_CIRCLE_RADIUS = 5;
const float OUTSIDE = 500;

// category definition
const short CATEGORY_BALL = 0x0001;
const short CATEGORY_PLATFORM = 0x0002;
const short CATEGORY_BARRIER = 0x0004;
const short CATEGORY_WALL1 = 0x0008;
const short CATEGORY_WALL2 = 0x0010;
const short CATEGORY_EXPLOSION = 0x0020;
const short CATEGORY_BARRIER_ELECTRICITY = 0x0040;

// maskbit definition
const short MASK_BALL = -1;
const short MASK_PLATFORM = CATEGORY_BALL | CATEGORY_BARRIER | CATEGORY_PLATFORM | CATEGORY_WALL2;
const short MASK_BARRIER = CATEGORY_BALL | CATEGORY_BARRIER | CATEGORY_PLATFORM | CATEGORY_WALL2 | CATEGORY_WALL1;
const short MASK_WALL1 = CATEGORY_BALL | CATEGORY_BARRIER;
const short MASK_WALL2 = CATEGORY_BALL | CATEGORY_BARRIER | CATEGORY_PLATFORM;
const short MASK_HEXGRID = 0;
const short MASK_BARRIER_ELECTRICITY = CATEGORY_BALL;

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
}GAME_STATE;

typedef enum class barrier_type
{
    DEFAULT_RECTANGLE = 0,
    DEFAULT_CIRCLE,
    ELECTRICITY,
    CONVEYOR,
    HEXGRID,
    SWITCH,
    POLYGON,
    POLYGON_HOLE,
    PHYSICSEDITOR,
    UNKNOWN,
        
}BARRIER_TYPE;
    
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

enum TAG_MENU_ITEM
{
    NOTIFICATION = 0,
    INFORMATION,
    SETTING,
    NOMOREADS,
    SUPPORT,
    TWITTER,
    FACEBOOK,
    YOUTUBE,
};

enum TAG_PENCIL_ITEM
{
    BUY_BUTTON = 0,
    SELECT_BUTTON,
};

enum TAG_SETTING_ITEM
{
    MUSIC = 0,
    SOUND,
    VIDEO,
    THUMBNAIL,
    LANGUGAGE,
    RELOAD_PURCHASES
};

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
    
enum TAG_POPUP_SHARE
{
    TAG_TWITTER = 0,
    TAG_FACEBOOK,
    TAG_LINE,
    TAG_GGPLUS,
    TAG_MORESHARE,
};
    
enum TAG_GAME
{
    TAG_BUTTON_BACK = 10,
    TAG_BUTTON_REPLAY,
    TAG_BUTTON_NEXT,
    TAG_BALLA,
    TAG_BALLB,
    TAG_PAPER_MINI,
    TAG_PLATFORM
};

enum ZORDER_GAME
{
    ZORDER_MAPLEVEL = 0,
    ZORDER_GRID = 0,
    ZORDER_DRAW_RENDER,
    ZORDER_ANIMATION,
	ZORDER_BUTTON_BACK = 10,
	ZORDER_BUTTON_REPLAY = 11,
    ZORDER_BUTTON_NEXT,
    ZORDER_CAPTURE_SCREEN_BIG,
    ZORDER_CAPTURE_SCREEN_SMALL,
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

class FixtureDef
{
public:
    FixtureDef()
    : next(nullptr) {}
        
    ~FixtureDef() {
        delete next;
        delete fixture.shape;
    }
        
    FixtureDef *next;
    b2FixtureDef fixture;
    int callbackData;
};
    
class BodyDef
{
public:
    BodyDef()
    : fixtures(nullptr) {}
        
    ~BodyDef() {
        if (fixtures)
            delete fixtures;
    }
        
    FixtureDef *fixtures;
    cocos2d::Point anchorPoint;
};
    
class ConveyorBelt
{
public:
    b2Fixture* fixture;
    float friction;
    float tangentSpeed;
};
#endif
