//
//  HubLayer.h
//  GameTetris
//
//  Created by Nguyen Minh Hoang on 2015/03/23.
//
//

#ifndef __GameTetris__HubLayer__
#define __GameTetris__HubLayer__

#include <cocos2d.h>
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "HubDelegate.h"
#include "Constants.h"
#include "ManageSound.h"
#include "ManageScene.h"
#include "TouchDelegate.h"

USING_NS_CC;
using namespace ui;

class HubLayer : public Layer, public HubDelegate
{
  
public:
    
    CREATE_FUNC(HubLayer);
    
    virtual bool init() override;
    
    void addLayerHub();
    
    virtual void addLineHub(int) override;
    
    virtual void removeTouch();
    
    virtual void onPauseCallback() override;
    
    virtual void onResumeCallback() override;
    
    HubLayer();
    
    ~HubLayer();
    
    CC_SYNTHESIZE(TouchDelegate*, myDelegate, MyTouchDelegate);
    
    void setUpTouchEvent();
    
    virtual bool onTouchBegan(cocos2d::Touch * touch, cocos2d::Event *event);
    
    virtual void onTouchMoved(cocos2d::Touch *touch,cocos2d::Event * event);
    
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    
    virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event);
    
    ui::Button* holdButton;
    
    ui::CheckBox* pauseButton;
    
    cocos2d::Vec2 firstTouch;
    
    cocos2d::Vec2 previousTouch;
    
    cocos2d::Vec2 currentTouch;
    
    int oldStepX = 0;
    
    int oldStepY = 0;
    
    bool isTouchRotation = true;
    
    bool isHardDrop = false;
    
    bool isDowning = false;
    
    bool isMoving = false;
    
private:
    
    virtual void onEnter();
    
    int lineNumber;
    
    int comboNumber;
    
    CC_SYNTHESIZE(Text*, currentLine, CurrentLine);
    
    CC_SYNTHESIZE(Text*, currentScore, CurrentScore);
    
    CC_SYNTHESIZE(Text*, currentCombo, CurrentCombo);
    
    CC_SYNTHESIZE(Text*, currentSpeed, CurrentSpeed);
    
    virtual void addComboHub() override;
    
    virtual void addScoreHub() override;
    
    virtual void addSpeedHub() override;
    
    virtual void onHoldCallback() override;
    
    void pauseCallback(cocos2d::Ref* pSender, CheckBox::EventType event_type);
    
    void restartCallback(cocos2d::Ref* pSender);
    
    void onHold(cocos2d::Ref* sender);
    
    void animationAddScore(Text* text);
    
    void onLongLeft(float time);
    
    void onLongRight(float time);
    
    void onLongRotate(float time);
    
    bool checkScreenSize();
};

#endif /* defined(__GameTetris__HubLayer__) */
