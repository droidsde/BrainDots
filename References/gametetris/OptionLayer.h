//
//  OptionLayer.h
//  GameTetris
//
//  Created by Nguyen Minh Hoang on 2015/04/02.
//
//

#ifndef __GameTetris__OptionLayer__
#define __GameTetris__OptionLayer__

#include <cocos2d.h>
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "ManageSound.h"
#include "ManageScene.h"

USING_NS_CC;
using namespace ui;

class OptionLayer : public Layer
{
public:
    
    CREATE_FUNC(OptionLayer);
    
    virtual bool init() override;
    
    void addLayerOption();
    
    virtual void onEnter();
    
    bool onTouchBegan(Touch* touch, Event* event);
    
    void onTouchEnded(Touch* touch, Event* event);
    
    void onTouchCancelled(Touch* touch, Event* event);
    
private:
    
    cocos2d::Point originPoint;
    
    cocos2d::Size  visibleSize;
    
    void addOption();
    
    void closeOption(Ref* sender);
    
    CC_SYNTHESIZE(ManageSoundDelegate*, msDelegator, MS);
    
    void musicCheckBoxCallback(Ref *pSender, CheckBox::EventType type);
    
    void soundCheckBoxCallback(Ref *pSender, CheckBox::EventType type);
    
    void speedSliderCallback(Ref*pSender, Slider::EventType type);
};

#endif /* defined(__GameTetris__OptionLayer__) */
