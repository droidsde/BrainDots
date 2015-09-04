//
//  PauseLayer.h
//  GameTetris
//
//  Created by Nguyen Minh Hoang on 2015/04/20.
//
//

#ifndef __GameTetris__PauseLayer__
#define __GameTetris__PauseLayer__

#include <cocos2d.h>
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "ManageScene.h"
#include "ManageSound.h"
#include "GameBoard.h"

USING_NS_CC;
using namespace ui;

class PauseLayer : public cocos2d::LayerColor
{
  
public:
    
    CREATE_FUNC(PauseLayer);
    
    virtual bool init() override;
    
    CC_SYNTHESIZE( class GameBoard*, miniGameBoard, GameBoardCallback);
    
private:
    
    cocos2d::Point originPoint;
    
    cocos2d::Size  visibleSize;
    
    CC_SYNTHESIZE(ManageSoundDelegate*, msDelegator, MS);
    
    void restartGame(cocos2d::Ref* sender);
    
    void quitGame(cocos2d::Ref* sender);
    
    void resumeGame(cocos2d::Ref* sender);
    
    void addLayer();
    
    void pauseLayerCallback();
};


#endif /* defined(__GameTetris__PauseLayer__) */
