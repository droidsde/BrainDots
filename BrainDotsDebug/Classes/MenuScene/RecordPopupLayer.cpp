//
//  RecordPopupLayer.cpp
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/09.
//
//

#include "RecordPopupLayer.h"

RecordPopupLayer::RecordPopupLayer()
{
    
}

RecordPopupLayer::~RecordPopupLayer()
{
    
}

RecordPopupLayer* RecordPopupLayer::create()
{
    RecordPopupLayer* layer = new RecordPopupLayer();
    layer->init();
    layer->autorelease();
    return layer;
}

bool RecordPopupLayer::init()
{
    BasePopupLayer::init();
    
    // title
    auto title = Text::create("Recording clip", "fonts/keifont.ttf", 60);
    title->setPosition(Vec2(layoutTable->getContentSize().width/2, layoutTable->getContentSize().height - PADDING));
    title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    title->setColor(Color3B::ORANGE);
    layoutTable->addChild(title);
    
    return true;
}