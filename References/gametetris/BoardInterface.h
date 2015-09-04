//
//  BoardInterface.h
//  GameTetris
//
//  Created by Nguyen Minh Hoang on 2015/03/20.
//
//

#pragma once

#include "cocos2d.h"
#include "Constants.h"

USING_NS_CC;


class BoardInterface
{
public:
    
    virtual bool isPointOccupied(cocos2d::Point point) = 0;
    
    virtual TETROMINO_TYPE getTetroType() = 0;
    
    virtual void gameOver() = 0;
    
    virtual void setCurTop(int) = 0;
    
    virtual int  getCurTop() = 0;
    
    virtual void switchShowingForCell(cocos2d::Point) = 0;
    
    virtual void checkClear() = 0;
};