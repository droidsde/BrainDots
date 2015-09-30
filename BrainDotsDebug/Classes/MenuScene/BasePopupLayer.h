//
//  BasePopupLayer.h
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/09.
//
//

#ifndef __BrainDotsDebug__BasePopupLayer__
#define __BrainDotsDebug__BasePopupLayer__
#pragma once

#include <cocos2d.h>
#include "ui/CocosGUI.h"
#include "Constants.h"
#include "SoundManager.h"

USING_NS_CC;
using namespace cocos2d::ui;

class SceneManager;
class BasePopupLayer: public LayerColor
{
public:
    static BasePopupLayer* create();
    BasePopupLayer();
    ~BasePopupLayer();
    virtual bool init() override;
    void onEnter();
    void closeLayer();
    
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    void onTouchCancelled(Touch* touch, Event* event);

    // add button method
    Button* createButton(std::string fileName, int tag, Vec2 anchorPoint);
    
    Vec2 origin;
    Size visibleSize;
    Layout* layoutTable;
    Vec2 posOrigin, posNew;
    CC_SYNTHESIZE(ManageSoundDelegate*, msDelegator, SoundManage);
};

#endif /* defined(__BrainDotsDebug__BasePopupLayer__) */
