//
//  RecordPopupLayer.h
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/09.
//
//

#ifndef __BrainDotsDebug__RecordPopupLayer__
#define __BrainDotsDebug__RecordPopupLayer__

#include <cocos2d.h>
#include "BasePopupLayer.h"

USING_NS_CC;
using namespace cocos2d::ui;

class RecordPopupLayer : public BasePopupLayer
{
public:
    
    RecordPopupLayer();
    ~RecordPopupLayer();
    static RecordPopupLayer* create();
    virtual bool init();
    
};

#endif /* defined(__BrainDotsDebug__RecordPopupLayer__) */
