//
//  MenuPopupLayer.h
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/09.
//
//

#ifndef __BrainDotsDebug__MenuPopupLayer__
#define __BrainDotsDebug__MenuPopupLayer__

#include <cocos2d.h>
#include "BasePopupLayer.h"
#include "SettingPopupLayer.h"
#include "InfoPopupLayer.h"
#include "NotiPopupLayer.h"

USING_NS_CC;
using namespace cocos2d::ui;

class MenuPopupLayer : public BasePopupLayer
{
public:
    
    MenuPopupLayer();
    ~MenuPopupLayer();
    static MenuPopupLayer* create();
    virtual bool init();
    void onEnter();
    void onExit();
    
private:
    Size layoutSize;
    void touchButtonEvent(Ref* sender, Widget::TouchEventType type);
};
#endif /* defined(__BrainDotsDebug__MenuPopupLayer__) */
