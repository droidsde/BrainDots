//
//  PencilPopupLayer.h
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/09.
//
//

#ifndef __BrainDotsDebug__PencilPopupLayer__
#define __BrainDotsDebug__PencilPopupLayer__

#include <cocos2d.h>
#include "BasePopupLayer.h"

USING_NS_CC;
using namespace cocos2d::ui;

class PencilPopupLayer : public BasePopupLayer
{
public:
    
    PencilPopupLayer();
    ~PencilPopupLayer();
    static PencilPopupLayer* create();
    virtual bool init();
    
};

#endif /* defined(__BrainDotsDebug__PencilPopupLayer__) */
