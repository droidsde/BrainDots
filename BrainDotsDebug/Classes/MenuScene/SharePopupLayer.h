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

USING_NS_CC;
using namespace cocos2d::ui;

class SharePopupLayer : public BasePopupLayer
{
public:
    
    SharePopupLayer();
    ~SharePopupLayer();
    static SharePopupLayer* create();
    virtual bool init();
    
};
#endif /* defined(__BrainDotsDebug__SharePopupLayer__) */
