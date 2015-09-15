//
//  ExecuteShapePhysic.h
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/15.
//
//

#ifndef __BrainDotsDebug__ExecuteShapePhysic__
#define __BrainDotsDebug__ExecuteShapePhysic__

#include <cocos2d.h>
#include <Box2D/Box2D.h>
#include "SceneManager.h"

USING_NS_CC;

class ExecuteShapePhysic
{
public:
    static Rect getBodyRectangle(Size visibleSize, b2Body* body);
};
#endif /* defined(__BrainDotsDebug__ExecuteShapePhysic__) */
