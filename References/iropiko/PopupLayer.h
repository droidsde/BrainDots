//
//  PopupLayer.h
//  PoolExample
//
//  Created by Nguyen Minh Hoang on 2015/08/12.
//
//

#ifndef __PoolExample__PopupLayer__
#define __PoolExample__PopupLayer__

#include <cocos2d.h>
#include "ManageScene.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

class PopupLayer: public cocos2d::LayerColor
{
public:
    static PopupLayer* createPopup(std::string title, cocos2d::ui::Button::ccWidgetTouchCallback callback, int tagOfOkButton);
    
    static PopupLayer* createPopup(std::string title, cocos2d::ui::Button::ccWidgetTouchCallback callback);
    
    PopupLayer(std::string title, cocos2d::ui::Button::ccWidgetTouchCallback callback);
    PopupLayer(std::string title, cocos2d::ui::Button::ccWidgetTouchCallback callback, int tagOfOkButton);
    
//    CREATE_FUNC(PopupLayer);
    
    virtual bool init() override;
    
    virtual void onEnter();
    
    virtual void onExit();
    
    void addTitle();
    
    void executeTouchButton(Ref* sender, Widget::TouchEventType type);
    
    bool onTouchBegan(Touch* touch, Event* event);
    
    void onTouchEnded(Touch* touch, Event* event);
    
    void onTouchCancelled(Touch* touch, Event* event);
    
    std::string titleText;
    
    cocos2d::ui::Button::ccWidgetTouchCallback callbackFunc;
    
private:
    
    Vec2 origin;
    
    cocos2d::Size visibleSize;
    
    cocos2d::Size bgSize;
    
    Sprite* background;
    
    Label* title;
    
    int tagOfOkButton = 0;
    
    void addBG();
    
    void addButtons();
    
};

#endif /* defined(__PoolExample__PopupLayer__) */
