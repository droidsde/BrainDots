//
//  TestButton.h
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/10/05.
//
//

#ifndef __BrainDotsDebug__TestButton__
#define __BrainDotsDebug__TestButton__

#include <cocos2d.h>
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

class BlendFuncButton : public Button
{
public:
    BlendFuncButton();
    ~BlendFuncButton();
    
    static BlendFuncButton* create(std::string fileNormal);
    
    void setBlendFunc(const BlendFunc &blendFunc);
};

#endif /* defined(__BrainDotsDebug__TestButton__) */
