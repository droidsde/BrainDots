//
//  SettingPopupLayer.h
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/30.
//
//

#ifndef __BrainDotsDebug__SettingPopupLayer__
#define __BrainDotsDebug__SettingPopupLayer__

#include <cocos2d.h>
#include "BasePopupLayer.h"
#include "cocos-ext.h"
#include "LanguagePopupLayer.h"
#include "SceneManager.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

class SettingPopupLayer : public BasePopupLayer
{
public:
    
    SettingPopupLayer();
    ~SettingPopupLayer();
    static SettingPopupLayer* create();
    virtual bool init();
    
    void touchButtonEvent(Ref* sender, Widget::TouchEventType type);
    void onEnter();
    void onExit();
    
private:
    Size layoutSize;
    Size listviewSize;
    Text* languageText;
    void reloadData(Ref* sender);
    void valueChanged(cocos2d::Ref* sender, cocos2d::extension::Control::EventType controlEvent);
};

#endif /* defined(__BrainDotsDebug__SettingPopupLayer__) */
