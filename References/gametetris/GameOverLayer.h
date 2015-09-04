//
//  GameOverLayer.h
//  GameTetris
//
//  Created by Nguyen Minh Hoang on 2015/03/26.
//
//

#ifndef __GameTetris__GameOverLayer__
#define __GameTetris__GameOverLayer__

#include <cocos2d.h>
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "ManageScene.h"
#include "ManageSound.h"

USING_NS_CC;
using namespace ui;

class GameOverLayer: public cocos2d::LayerColor
{

public:
    
    CREATE_FUNC(GameOverLayer);
    
    virtual bool init() override;
    
    bool onTouchBegan(Touch* touch, Event* event);
    
    void onTouchEnded(Touch* touch, Event* event);
    
    void onTouchCancelled(Touch* touch, Event* event);
    
    virtual void onEnter();
    
    virtual void onExit();
    
    
private:
    
    cocos2d::Point originPoint;
    
    cocos2d::Size  visibleSize;
    
    ui::Text* textScore;
    
    int curScore;
    
    int oldHighScore;
    
    int countScore = 0;
    
    std::string scoreText;
    
    void  restart(cocos2d::Ref* sender);
    
    void  quitGame(cocos2d::Ref* sender);
    
    void  addLayer();
    
    void  addScore();
    
    void  addParticle();
    
    bool  saveScore();
    
    void  runningScore(float dt);
    
    CC_SYNTHESIZE(ManageSoundDelegate*, msDelegator, MS);
};
#endif /* defined(__GameTetris__GameOverLayer__) */
