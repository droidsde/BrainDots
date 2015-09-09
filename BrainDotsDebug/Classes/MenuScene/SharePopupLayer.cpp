//
//  SharePopupLayer.cpp
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/09.
//
//

#include "SharePopupLayer.h"

SharePopupLayer::SharePopupLayer()
{
    
}

SharePopupLayer::~SharePopupLayer()
{
    
}

SharePopupLayer* SharePopupLayer::create()
{
    SharePopupLayer* layer = new SharePopupLayer();
    layer->init();
    layer->autorelease();
    return layer;
}

bool SharePopupLayer::init()
{
    BasePopupLayer::init();
    
    // title
    auto title = Text::create("Share to everybody", "arial.ttf", 80);
    title->setPosition(Vec2(layoutTable->getContentSize().width/2, layoutTable->getContentSize().height - PADDING));
    title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    title->setColor(Color3B::RED);
    layoutTable->addChild(title);
    
    return true;
}