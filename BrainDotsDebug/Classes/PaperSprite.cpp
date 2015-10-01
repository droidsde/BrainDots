//
//  PaperSprite.cpp
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/28.
//
//

#include "PaperSprite.h"

PaperSprite::PaperSprite(bool isScale, bool isCallbackFunction)
{
    this->isScale = isScale;
    this->isCallbackFunction = isCallbackFunction;
}

PaperSprite::~PaperSprite()
{
    
}

PaperSprite* PaperSprite::create(std::string filename, bool isFromPlist, bool isScale, bool isCallbackFunction)
{
    PaperSprite* pPaperSprite = new PaperSprite(isScale, isCallbackFunction);
    
    if (isFromPlist) {
        if (pPaperSprite->initWithSpriteFrameName(filename)) {
            
            pPaperSprite->autorelease();
            
            pPaperSprite->initOptions();
            
            pPaperSprite->addEvents();
            
            return pPaperSprite;
        }
    } else {
        if (pPaperSprite->initWithFile(filename)) {
            
            pPaperSprite->autorelease();
            
            pPaperSprite->initOptions();
            
            pPaperSprite->addEvents();
            
            return pPaperSprite;
        }
    }
    
    CC_SAFE_DELETE(pPaperSprite);
    return NULL;
}

void PaperSprite::initOptions()
{
    enableTouch = true;
    _callback = NULL;
    _callbackFunction = NULL;
}

void PaperSprite::setCallback(cocos2d::Sequence *callback)
{
    _callback = callback;
    _callback->retain();
}

void PaperSprite::setCallbackFunction(std::function<void ()> callback)
{
    _callbackFunction = callback;
}

void PaperSprite::addEvents()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = [&] (Touch* touch, Event* event )
    {
        Vec2 p = touch->getLocation();
        cocos2d::Rect rect = this->getBoundingBox();
        
        if(rect.containsPoint(p))
        {
            if (this->isScale) {
                this->setScale(1.1);
            }
            return true; // to indicate that we have consumed it.
        }
        
        return false; // we did not consume this event, pass thru.
    };
    
    listener->onTouchEnded = [=](cocos2d::Touch* touch, cocos2d::Event* event)
    {
        if (this->isScale) {
            this->setScale(1.0f);
        }
        PaperSprite::touchEvent(touch);
//        Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void PaperSprite::touchEvent(cocos2d::Touch *touch)
{
    if(isCallbackFunction){
        if(_callbackFunction!=NULL) {
            stopAllActions();
            _callbackFunction();
        }
        
    }
    if(_callback!=NULL) {
        stopAllActions();
        runAction(_callback);
        _callback->release();
    }
}