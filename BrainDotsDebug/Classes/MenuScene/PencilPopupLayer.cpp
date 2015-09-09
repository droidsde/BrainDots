//
//  PencilPopupLayer.cpp
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/09.
//
//

#include "PencilPopupLayer.h"

PencilPopupLayer::PencilPopupLayer()
{
    
}

PencilPopupLayer::~PencilPopupLayer()
{
    
}

PencilPopupLayer* PencilPopupLayer::create()
{
    PencilPopupLayer* layer = new PencilPopupLayer();
    layer->init();
    layer->autorelease();
    return layer;
}

bool PencilPopupLayer::init()
{
    BasePopupLayer::init();
    
    // title
    auto title = Text::create("Choose pencil", "arial.ttf", 80);
    title->setPosition(Vec2(layoutTable->getContentSize().width/2, layoutTable->getContentSize().height - PADDING));
    title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    title->setColor(Color3B::RED);
    layoutTable->addChild(title);
    
    return true;
}