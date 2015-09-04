//
//  MenuScene.h
//  GameTetris
//
//  Created by Nguyen Minh Hoang on 2015/03/19.
//
//

#ifndef __GameTetris__MenuScene__
#define __GameTetris__MenuScene__

#include <cocos2d.h>
#include "ManageSound.h"
#include "ManageScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Constants.h"
#include "OptionLayer.h"


USING_NS_CC;
using namespace ui;

class MenuScene: public Layer
{
    
public:
    static Scene* createScene();
    
    CREATE_FUNC(MenuScene);
    
    virtual bool init();
    
    CC_SYNTHESIZE(ManageSoundDelegate*, msDelegator, MS);
    
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);

private:
    
    cocos2d::Vec2 originalPoint;
    
    cocos2d::Size  visibleSize;
    
    void addMenu();
    
    void addBackgroundAndTitle();
    
    void addButtons();
    
    void startGame(cocos2d::Ref* sender);
    
    void showOption(cocos2d::Ref* sender);
    
    void showHighScore(cocos2d::Ref* sender);
    
};

#endif /* defined(__GameTetris__MenuScene__) */
