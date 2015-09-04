//
//  Constants.h
//  GameTetris
//
//  Created by Nguyen Minh Hoang on 2015/03/19.
//
//

#pragma once

#define VERSION                 "version 2.0.7"

#define BLOCK_WIDTH             32
#define BLOCK_HEIGHT            32

#define PIECE_SIZE              4
#define SHAPE_SIZE              4
#define BOARD_WIDTH             10
#define BOARD_HEIGHT            20

#define FIELD_BOTTOM            0
#define FIELD_TOP               19
#define FIELD_LEFT_BOARD        0
#define FIELD_RIGHT_BOARD       9

#define TIME_TRANSITION_SCENE   1.0

#define PADDING_BUTTON_MENU     32

#define SCORE_ADDING            10
#define COUNT_LINE_TO_ADD_SPEED 10

#define LOCK_DELAY_TAG          1001
#define TIME_LOCK_DELAY         0.5

#define SPEED_GAME_DEFAULT      1.0f

#define OCCUPIED true
#define EMPTY    false

#define PLAYFIELD true
#define TETROMINO false

#define CUR_POINT                   Point((BLOCK_WIDTH * BOARD_WIDTH) / 2 - BLOCK_WIDTH * 2, BLOCK_HEIGHT * BOARD_HEIGHT - BLOCK_HEIGHT * 3)
#define PRE_POINT                   Point((BLOCK_WIDTH * BOARD_WIDTH) + BLOCK_WIDTH * 2, BLOCK_HEIGHT * BOARD_HEIGHT - BLOCK_HEIGHT * 5)
#define HOLD_POINT                  Point((BLOCK_WIDTH * BOARD_WIDTH) + BLOCK_WIDTH * 2, BLOCK_HEIGHT * BOARD_HEIGHT - BLOCK_HEIGHT * 11)

#define PAUSE_GAME "pauseGame"

typedef enum class num_line
{
    ONE_LINE = 1,
    TWO_LINE,
    THREE_LINE,
    FOUR_LINE
}NUM_LINE;
    
typedef enum class game_state
{
    LOGO = 0,
    MENU,
    GAME,
    OVER,
    HIGHSCORE,
}GAME_STATE;

typedef enum class shape_state
{

    SHAPE_0 = 0,
    SHAPE_R,
    SHAPE_2,
    SHAPE_L
}SHAPE_STATE;
    
typedef enum class piece_type
{
    O = 0,
    I, //1
    J, //2
    L, //3
    Z, //4
    S, //5
    T, //6
    NOTHING //means this is for board
}TETROMINO_TYPE;
    
typedef enum class piece_state
{
    SHOW = 0,
    PRE,
    GHOST,
    HOLD
}TETROMINO_STATE;
    

typedef bool FieldOrTetro;
typedef bool OccupOrEmpty;
    
typedef struct
{
    FieldOrTetro      _isFieldOrTetro;
    OccupOrEmpty      _isOccupOrEmpty;
    TETROMINO_TYPE    _tetroType;
    int               _coordinateX;
    int               _coordinateY;
    bool              _isGhost;
}BlockDef;
    
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