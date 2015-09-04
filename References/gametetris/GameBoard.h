//
//  GameBoard.h
//  GameTetris
//
//  Created by Nguyen Minh Hoang on 2015/03/19.
//
//

#ifndef __GameTetris__GameBoard__
#define __GameTetris__GameBoard__

#include <cocos2d.h>
#include <stdlib.h>
#include "ManageSound.h"
#include "Constants.h"
#include "BlockImage.h"
#include "BoardInterface.h"
#include "Tetromino.h"
#include "HubDelegate.h"
#include "RandomTetro.h"
#include "TouchDelegate.h"
#include "PauseLayer.h"

USING_NS_CC;

class GameBoard :public Layer, public BoardInterface, public TouchDelegate
{
public:
    
    virtual bool init();
    
    CREATE_FUNC(GameBoard);
    
    GameBoard();
    
    ~GameBoard();
    
    void onEnter();
    
    void onExit();
    
    void pauseGame(cocos2d::Ref* sender);
    
    void resumeGame(cocos2d::Ref* sender);
    
    void shakingBoard();
    
    void showWonLines(std::string);
    
    // inheritance from BoardInterface
    virtual bool isPointOccupied(cocos2d::Point point) override;
    
    virtual TETROMINO_TYPE getTetroType() override;
    
    virtual void gameOver() override;
    
    virtual void setCurTop(int) override;
    
    virtual int  getCurTop() override;
    
    void switchShowingForCell(cocos2d::Point);
    
    void switchCurrentAndNextTetromino();
    
    void switchCurrentAndHoldTetromino();
    
    virtual void checkClear() override;

    // inheritance from TouchDelegate
    virtual void moveLeft(int) override;
    
    virtual void moveRight(int) override;
    
    virtual void moveDown(int) override;
    
    virtual void moveRotate() override;
    
    virtual void moveHardDrop() override;
    
    virtual void moveSoftDropStart() override;
    
    virtual void moveSoftDropStop() override;
    
    virtual void onHold() override;
    
    virtual void onPause() override;
    
private:
    
    cocos2d::Size winSize;
    
    int curTop;
    
    std::vector<int> comboVector;
    
    std::vector<std::vector<class BlockImage*>> playFieldVector;
    
    void initPlayField();
    
    void gameStart();
    
    void createCurrentTetro();
    
    bool checkCreateNextTetro();
    
    bool checkFollowTetroType(TETROMINO_TYPE, bool*);
    
    void createNextTetro();
    
    void createGhostPiece();
    
    void createHoldPiece();
    
    void clearLine(std::vector<int>&);
    
    void fallLine(int);
    
    void animationGameOver();
    
    void removeTetroWhenGameover();
    
    CC_SYNTHESIZE(bool, isPaused, IsPaused);
    
    CC_SYNTHESIZE(ManageSoundDelegate*, msDelegator, MS);
    
    CC_SYNTHESIZE(class HubDelegate*, HubDelegate, Hub);
    
    CC_SYNTHESIZE(class PauseLayer*, PauseDelegate, pauseLayer);
    
    CC_SYNTHESIZE(class Tetromino*, currentTetromino, CurTetromino);
    
    CC_SYNTHESIZE(class Tetromino*, nextTetromino, NextTetromino);
    
    CC_SYNTHESIZE(class Tetromino*, ghostTetromino, GhostTetromino);
    
    CC_SYNTHESIZE(class Tetromino*, holdTetromino, HoldTetromino);
    
    CC_SYNTHESIZE(TETROMINO_TYPE, currentTetroType, CurrentTetroType);
    
};
#endif /* defined(__GameTetris__GameBoard__) */
