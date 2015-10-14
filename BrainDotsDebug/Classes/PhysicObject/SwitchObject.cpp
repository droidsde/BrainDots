//
//  SwitchObject.cpp
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/10/14.
//
//

#include "SwitchObject.h"

SwitchObject::SwitchObject(b2Body* body)
{
    this->myBody = body;
    setTurnOff(false);
}

SwitchObject::~SwitchObject()
{
    myBody = nullptr;
}

SwitchObject* SwitchObject::create(std::string filename, bool isFromPlist, b2Body *body)
{
    SwitchObject* swObj = new SwitchObject(body);
    if (isFromPlist) {
        if (swObj->initWithSpriteFrameName(filename)) {
            swObj->autorelease();
            swObj->init();
            
            return swObj;
        }
    } else {
        if (swObj->initWithFile(filename)) {
            swObj->autorelease();
            swObj->init();
            
            return swObj;
        }
    }
    
    CC_SAFE_DELETE(swObj);
    return nullptr;
}

bool SwitchObject::init()
{
    myBody->SetUserData(this);
    return true;
}

void SwitchObject::moveDown(float distance)
{
    b2Vec2 posNow = b2Vec2(getPositionX() / PTM_RATIO, getPositionY() / PTM_RATIO);
    b2Vec2 posTrans = b2Vec2(posNow.x, posNow.y - distance/PTM_RATIO);
    myBody->SetTransform(posTrans, myBody->GetAngle());
}

void SwitchObject::moveUp(float distance)
{
    b2Vec2 posNow = b2Vec2(getPositionX() / PTM_RATIO, getPositionY() / PTM_RATIO);
    b2Vec2 posTrans = b2Vec2(posNow.x, posNow.y + distance / PTM_RATIO);
    myBody->SetTransform(posTrans, myBody->GetAngle());
}
