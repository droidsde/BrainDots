//
//  ModelDialogLayer.cpp
//  PoolExample
//
//  Created by Nguyen Minh Hoang on 2015/08/17.
//
//

#include "ModelDialogLayer.h"
USING_NS_CC;
using namespace cocos2d::ui;

// create with 2 button
ModelDialogLayer* ModelDialogLayer::create(const char *message, cocos2d::Ref *target, SEL_CallFuncN okSelector, SEL_CallFuncN ngSelector)
{
    ModelDialogLayer* layer = ModelDialogLayer::create();
    layer->setStyle(message, target, okSelector, ngSelector, 0);
    return layer;
}

// create with 1 button
ModelDialogLayer* ModelDialogLayer::create(const char *message, cocos2d::Ref *target, SEL_CallFuncN ngSelector)
{
    ModelDialogLayer* layer = ModelDialogLayer::create();
    layer->setStyle(message, target, nullptr, ngSelector, 0);
    return layer;
}

// show dialog
ModelDialogLayer* ModelDialogLayer::create(const char *message, int timeLife)
{
    ModelDialogLayer* layer = ModelDialogLayer::create();
    layer->setStyle(message, nullptr, nullptr, nullptr, timeLife);
    return layer;
}

bool ModelDialogLayer::init()
{
    if(!LayerColor::initWithColor(Color4B(0, 0, 0, 0)))
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    frame = Sprite::create("alert_background.png");
    frame->setTag(ModelDialogLayerTagBG);
    frame->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    frame->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/4));
    bgSize = frame->getContentSize();
    this->addChild(frame);
    return true;
}

void ModelDialogLayer::setStyle(const char *message, cocos2d::Ref *target, SEL_CallFuncN okSelector, SEL_CallFuncN ngSelector, int timeLife)
{
    // set variables
    this->target = target;
    this->okSelector = okSelector;
    this->ngSelector = ngSelector;
    this->timeLife = timeLife;
    
    // show message
    auto title = Label::createWithTTF(message, "fonts/keifont.ttf", 40);
    title->setAnchorPoint(Vec2(0.5, 1));
    title->setColor(Color3B::WHITE);
    title->setVerticalAlignment(TextVAlignment::CENTER);
    title->setHorizontalAlignment(TextHAlignment::CENTER);
    title->setPosition(Vec2(bgSize.width/2, bgSize.height*2/3));
    frame->addChild(title);
    
    // auto close when have timelife
    if (!okSelector && !ngSelector && timeLife > 0) {
        this->runAction(Sequence::create(DelayTime::create(timeLife), FadeOut::create(0.2f), RemoveSelf::create(), NULL));
        return;
    }
    
    // ok button
    if (okSelector) {
        auto okButton = Button::create("popup_button.png");
        okButton->setTitleText("はい");
        okButton->setTitleFontName("fonts/keifont.ttf");
        okButton->setTitleFontSize(30);
        okButton->setTitleColor(Color3B::BLACK);
        okButton->setTouchEnabled(true);
        okButton->setTag(ModelDialogLayerTagNG);
        okButton->setPosition(Vec2(bgSize.width/2 -100, bgSize.height/4));
        okButton->addTouchEventListener(CC_CALLBACK_2(ModelDialogLayer::menuAlertOK, this));
        frame->addChild(okButton);
    }
    
    // ng button
    if (ngSelector) {
        auto noButton = Button::create("popup_button.png");
        noButton->setTitleText("いいえ");
        noButton->setTitleFontName("fonts/keifont.ttf");
        noButton->setTitleFontSize(30);
        noButton->setTitleColor(Color3B::BLACK);
        noButton->setTouchEnabled(true);
        noButton->setTag(ModelDialogLayerTagNG);
        noButton->setPosition(Vec2(bgSize.width/2 + 100, bgSize.height/4));
        noButton->addTouchEventListener(CC_CALLBACK_2(ModelDialogLayer::menuAlertClose, this));
        frame->addChild(noButton);
    }
}

void ModelDialogLayer::menuAlertOK(Ref* pSender, Widget::TouchEventType type)
{
    CCLOG("ModelDialogLayer OK");
    
    this->menuAlertOK(pSender, type);
    if (this->target && this->okSelector)
    {
        (this->target->*this->okSelector)(this);
    }
}

void ModelDialogLayer::menuAlertClose(Ref* pSender, Widget::TouchEventType type)
{
    CCLOG("ModelDialogLayer NG");
    if (((Node*)pSender)->getTag() == ModelDialogLayerTagNG && this->target && this->ngSelector)
    {
        (this->target->*this->ngSelector)(this);
    }
}