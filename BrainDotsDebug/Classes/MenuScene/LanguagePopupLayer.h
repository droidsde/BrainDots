//
//  LanguagePopupLayer.h
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/30.
//
//

#ifndef __BrainDotsDebug__LanguagePopupLayer__
#define __BrainDotsDebug__LanguagePopupLayer__

#include <cocos2d.h>
#include "BasePopupLayer.h"
#include <extensions/cocos-ext.h>
#include "ScrollBarView.h"

USING_NS_CC;
USING_NS_CC_EXT;

class LanguagePopupLayer : public BasePopupLayer, cocos2d::extension::ScrollViewDelegate
{
public:
    
    LanguagePopupLayer();
    ~LanguagePopupLayer();
    static LanguagePopupLayer* create();
    virtual bool init();
    void touchButtonEvent(Ref* sender, Widget::TouchEventType type);
    
    void scrollViewDidScroll(cocos2d::extension::ScrollView* view);
    void scrollViewDidZoom(cocos2d::extension::ScrollView *view);
    
private:
    
    Size layoutSize;
    ScrollBarView* bar;
};

#endif /* defined(__BrainDotsDebug__LanguagePopupLayer__) */
