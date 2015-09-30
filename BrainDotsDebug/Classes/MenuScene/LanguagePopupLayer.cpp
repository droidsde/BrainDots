//
//  LanguagePopupLayer.cpp
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/30.
//
//

#include "LanguagePopupLayer.h"

LanguagePopupLayer::LanguagePopupLayer()
{
    
}

LanguagePopupLayer::~LanguagePopupLayer()
{
    bar = nullptr;
}

LanguagePopupLayer* LanguagePopupLayer::create()
{
    LanguagePopupLayer* layer = new LanguagePopupLayer();
    layer->init();
    layer->autorelease();
    return layer;
}

bool LanguagePopupLayer::init()
{
    BasePopupLayer::init();
    layoutSize = layoutTable->getContentSize();
    
    // title
    auto title = Text::create("Language", "arial.ttf", 60);
    title->setPosition(Vec2(layoutSize.width/2, layoutSize.height - PADDING));
    title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    title->setColor(Color3B::RED);
    layoutTable->addChild(title);
    
    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");
    Size size = Size(200, 200);
    extension::ScrollView* sc = extension::ScrollView::create(size);
    sc->setDirection(extension::ScrollView::Direction::HORIZONTAL);
    sc->setContainer(sprite);
    sc->setContentSize(sprite->getContentSize());
    sc->setDelegate(this);
    
    sc->setPosition(layoutSize/2);
    
    this->addChild(sc);
    
    bar = ScrollBarView::create(sc, ScrollBarView::BarType::HORIZONTAL_OUT);
    
    return true;
}

void LanguagePopupLayer::scrollViewDidScroll(extension::ScrollView *view)
{
    bar->refresh();
}

void LanguagePopupLayer::scrollViewDidZoom(extension::ScrollView *view)
{
    
}