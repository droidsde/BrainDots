//
//  PaperSprite.cpp
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/28.
//
//

#include "PaperSprite.h"

PaperSprite::PaperSprite(bool isScale)
{
    this->isScale = isScale;
}

PaperSprite::~PaperSprite()
{
    
}

PaperSprite* PaperSprite::create(std::string filename, bool isScale)
{
    PaperSprite* pPaperSprite = new PaperSprite(isScale);
    
    if (pPaperSprite->initWithFile(filename)) {
        
        pPaperSprite->autorelease();
        
        pPaperSprite->initOptions();
        
        pPaperSprite->addEvents();
        
        return pPaperSprite;
    }
    
    CC_SAFE_DELETE(pPaperSprite);
    return NULL;
}

void PaperSprite::initOptions()
{
    
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
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void PaperSprite::touchEvent(cocos2d::Touch *touch)
{
    CCLOG("touch papersprite");
}