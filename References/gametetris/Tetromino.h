//
//  Tetromino.h
//  GameTetris
//
//  Created by Nguyen Minh Hoang on 2015/03/20.
//
//

#ifndef __GameTetris__Tetromino__
#define __GameTetris__Tetromino__

#include <cocos2d.h>
#include "Constants.h"
#include "BlockImage.h"
#include "MiniBlock.h"
#include "ManageScene.h"
#include "BlockGhost.h"
#include "RandomTetro.h"
#include "RotationTetromino.h"
#include "ManageSound.h"
#include "HubLayer.h"

USING_NS_CC;

class Tetromino : public cocos2d::Sprite
{
public:
    
    virtual bool init();
    
    static Tetromino* create(TETROMINO_TYPE, bool is_ghost);
    
    Tetromino(TETROMINO_TYPE, bool);
    
    ~Tetromino();
    
    CC_SYNTHESIZE(TETROMINO_TYPE, myType, Type);
    
    void setInitStateToTetro();
    
    void setInitStateToPre();
    
    void setInitStateToGhost();
    
    void setInitStateToHold();
    
    void switchState(TETROMINO_STATE);
    
    bool isMoveAble();
    
    void checkAndStopLockOn();
    
private:
    
    friend class RotationTetromino;
    
    std::vector<char16_t> shapesVector; // all shapes
    
    std::vector<std::vector<class MiniBlock*>> blocksVector; // vector of vector block*
    
    CC_SYNTHESIZE(TETROMINO_STATE, curState, CurState);  // current state
    
    CC_SYNTHESIZE(bool, isGhost, IsGhost);               // check is ghost
    
    CC_SYNTHESIZE(bool, mobility, CurMobility);          //
    
    CC_SYNTHESIZE(int, curShape, CurShape);//the index of current Shape
    
    CC_SYNTHESIZE(ManageSoundDelegate*, msDelegator, MS); // sound
    
    bool shapeMatrix[PIECE_SIZE][PIECE_SIZE]; //mask the ocupied cell
    
    void initBlocks();
    
    void doCleanBeforeReset();
    
    void setShapesByType(TETROMINO_TYPE);
    
    void reShowing();
    
    void onLeft(int);           // turn left tetro
    
    void onRight(int);          // turn right
    
    void onRotate();            // turn rotate
    
    void onDown(int);           // turn down
    
    void onHardDrop();          // turn hard drop
    
    void onHold();              // push hold
    
    bool leftAble(int);         // check turn leftable
    
    bool rightAble(int);        // check turn rightable
    
    bool downAble();            // check turn downable
    
    bool rotateAble();          // check rotateable
    
    void doRotation();          // implement rotate
    
    void fallingDown(float time);   // implement down

    void lockOn();              // lock on
    
    void instantLockon();
    
    void startFalling();        // start falling
    
    void stopFalling();         // stop falling
    
    bool isFalling();           // check falling
    
    void runLockDelay();
    
    void getHaunted();
    
    void refreshType(TETROMINO_TYPE type);
    
    void refreshShape(int shape);
    
    void overGame();
};

#endif /* defined(__GameTetris__Tetromino__) */
