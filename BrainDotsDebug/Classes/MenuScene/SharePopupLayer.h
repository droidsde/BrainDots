//
//  SharePopupLayer.h
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/09.
//
//

#ifndef __BrainDotsDebug__SharePopupLayer__
#define __BrainDotsDebug__SharePopupLayer__

#include <cocos2d.h>
#include "BasePopupLayer.h"
#include "SceneManager.h"
#include "FBPlugin.h"

USING_NS_CC;
using namespace cocos2d::ui;

class SharePopupLayer : public BasePopupLayer
{
public:
    
    SharePopupLayer(std::string filename);
    ~SharePopupLayer();
    static SharePopupLayer* create(std::string filename);
    virtual bool init();
    
    void touchButtonEvent(Ref* sender, Widget::TouchEventType type);
    
private:
    Button* twitterShare;
    Button* facebookShare;
    Button* lineShare;
    Button* ggplusShare;
    Button* moreShare;
    
    Sprite* imageShare;
    std::string filename;
};
#endif /* defined(__BrainDotsDebug__SharePopupLayer__) */
