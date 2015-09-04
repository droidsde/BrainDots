//
//  Constanst.h
//  PoolExample
//
//  Created by Nguyen Minh Hoang on 2015/07/24.
//
//

#ifndef PoolExample_Constanst_h
#define PoolExample_Constanst_h

#define VERSION                 "version 1.0.0"
const int PADDING           = 10;
const int SEGMENT           = 1000;
const int INCREASE          = 200;
const int SPACING_P1P2      = 10;
const int MIN_MOVING        = 100;
const int SUMCOLOR          = 10;
const float TIME_TRANSITION_SCENE = 0.5f;
const float CIRCLE_RADIUS   = 60;
const float TIME_CHANGE_ANIMATION = 0.3f;
const int TIME_RECORDING    = 3;

// type curve to draw
typedef enum class type_curve
{
    QUAD    = 1,
    CUBIC   = 2,
}TYPE_CURVE;
    
// state moving layout
typedef enum class state_move
{
    STANDING    =   0,
    LEFT        =   -1,
    RIGHT       =   1,
}STATE_MOVE;

enum Tag
{
    T_NUMBERNAME = 10,
    T_NUMBER,
    T_SOUND,
    T_PLAYBUTTON,
    T_AUDIORECORDBUTTON,
    T_SETTINGBUTTON,
    T_JPBUTTON,
    T_ENBUTTON,
    T_CLOSESETTINGBUTTON,
    
    T_RESETBUTTON,
    T_POPUP_LAYER,
    T_POPUP_OK,
    T_POPUP_NO,
    
    T_RECORD_NUMBER,
    T_RECORD_NAME,
    T_RECORD_PLAY,
    T_RECORD_RECORDING,
    T_RECORD_COUNTDOWN,
};

typedef enum class game_state
{
    LOGO = 0,
    MENU,
    RECORD,
    GAME,
}GAME_STATE;

typedef enum class language_type
{
    JA = 0,
    EN,
}LANGUAGE_TYPE;

enum State_SM
{
    CHOOSE = 1,
    LIST,
    RECORDING,
};

enum Tag_ListItem
{
    T_LISTITEM_OPEN = 0,
    T_LISTITEM_REMOVE = 100,
    T_LISTITEM_OKBUTTON = 200,
};

template <typename T>
std::string to_string(T value)
{
    std::ostringstream os ;
    os << value ;
    return os.str() ;
};
#endif
