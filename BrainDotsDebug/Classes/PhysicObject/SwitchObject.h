//
//  SwitchObject.h
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/10/14.
//
//

#ifndef __BrainDotsDebug__SwitchObject__
#define __BrainDotsDebug__SwitchObject__

#include <cocos2d.h>
#include "Box2D/Box2D.h"
#include "SceneManager.h"

USING_NS_CC;

class SwitchObject: public Sprite
{
public:
    SwitchObject(b2Body* body);
    ~SwitchObject();
    
    static SwitchObject* create(std::string filename, bool isFromPlist, b2Body* body);
    
    bool init();
    
    CC_SYNTHESIZE(bool, turnOff, TurnOff);
    
    void moveUp(float distance);
    
    void moveDown(float distance);

private:
    b2Body* myBody;
};

#endif /* defined(__BrainDotsDebug__SwitchObject__) */
