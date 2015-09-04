//
//  PopupLayer.cpp
//  PoolExample
//
//  Created by Nguyen Minh Hoang on 2015/08/12.
//
//

#include "PopupLayer.h"

USING_NS_CC;
using namespace cocos2d::ui;

PopupLayer* PopupLayer::createPopup(std::string title, cocos2d::ui::Button::ccWidgetTouchCallback callback)
{
    PopupLayer* layer= new PopupLayer(title, callback);
    layer->init();
    layer->autorelease();
    return layer;
}

PopupLayer* PopupLayer::createPopup(std::string title, cocos2d::ui::Button::ccWidgetTouchCallback callback, int tagOfOkButton)
{
    PopupLayer* layer= new PopupLayer(title, callback, tagOfOkButton);
    layer->init();
    layer->autorelease();
    return layer;
}

PopupLayer::PopupLayer(std::string title, cocos2d::ui::Button::ccWidgetTouchCallback callback, int tag)
{
    titleText = title;
    callbackFunc = callback;
    tagOfOkButton = tag;
}

PopupLayer::PopupLayer(std::string title, cocos2d::ui::Button::ccWidgetTouchCallback callback)
{
    titleText = title;
    callbackFunc = callback;
}

bool PopupLayer::init()
{
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 0))) {
        return false;
    }
    origin = Director::getInstance()->getVisibleOrigin();
    visibleSize = Director::getInstance()->getVisibleSize();

    addBG();
    
    auto eventListener = EventListenerTouchOneByOne::create();
    eventListener->setSwallowTouches(true);
    eventListener->onTouchBegan = CC_CALLBACK_2(PopupLayer::onTouchBegan, this);
    eventListener->onTouchCancelled = CC_CALLBACK_2(PopupLayer::onTouchCancelled, this);
    eventListener->onTouchEnded = CC_CALLBACK_2(PopupLayer::onTouchEnded, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, this);
    
    return true;
}

void PopupLayer::addBG()
{
    background = Sprite::create("popup_bg.png");
    background->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
    background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    bgSize = background->getContentSize();
    addChild(background);
    
    // add title
    addTitle();
    
    // add buttons
    addButtons();
}

void PopupLayer::addButtons()
{
    // add button ok
    auto okButton = Button::create("popup_button.png");
    okButton->setTitleText("はい");
    okButton->setTitleFontName("fonts/keifont.ttf");
    okButton->setTitleFontSize(30);
    okButton->setTitleColor(Color3B::BLACK);
    okButton->setTouchEnabled(true);
    okButton->setPosition(Vec2(bgSize.width/2 -100, bgSize.height/4));
    background->addChild(okButton);
    CCLOG("tagOfOkButton=%d",tagOfOkButton);
    if (tagOfOkButton == 0) {
        okButton->setTag(Tag::T_POPUP_OK);
    } else {
        okButton->setTag(tagOfOkButton);
    }
    
    // add button no
    auto noButton = Button::create("popup_button.png");
    noButton->setTitleText("いいえ");
    noButton->setTitleFontName("fonts/keifont.ttf");
    noButton->setTitleFontSize(30);
    noButton->setTitleColor(Color3B::BLACK);
    noButton->setTouchEnabled(true);
    noButton->setTag(Tag::T_POPUP_NO);
    noButton->setPosition(Vec2(bgSize.width/2 + 100, bgSize.height/4));
    background->addChild(noButton);

    okButton->addTouchEventListener(callbackFunc);
    noButton->addTouchEventListener(CC_CALLBACK_2(PopupLayer::executeTouchButton, this));
}

void PopupLayer::addTitle()
{
    title = Label::createWithTTF(titleText, "fonts/keifont.ttf", 50);
    title->setAnchorPoint(Vec2(0.5, 1));
    title->setColor(Color3B::WHITE);
    title->setVerticalAlignment(TextVAlignment::CENTER);
    title->setHorizontalAlignment(TextHAlignment::CENTER);
    background->addChild(title);
    title->setPosition(Vec2(bgSize.width/2, bgSize.height*2/3));
}

void PopupLayer::executeTouchButton(cocos2d::Ref *sender, Widget::TouchEventType type)
{
    auto button = (Button*) sender;
    if (type == Widget::TouchEventType::ENDED) {
        switch (button->getTag()) {
            case Tag::T_POPUP_NO :
            {
                auto scale = ScaleTo::create(0.2, 0);
                auto set = CallFunc::create(CC_CALLBACK_0(PopupLayer::setVisible, this, false));
                auto remove = CallFunc::create(CC_CALLBACK_0(PopupLayer::removeFromParent, this));
                this->runAction(Sequence::create(scale, set, remove, NULL));
                break;
            }
        }
    }
}

void PopupLayer::onEnter()
{
    LayerColor::onEnter();
    background->setScale(0);
    auto scaleTo = ScaleTo::create(1.0, 1.0);
    auto ease = EaseElasticOut::create(scaleTo);
    background->runAction(ease);
}

void PopupLayer::onExit()
{
    LayerColor::onExit();
}

bool PopupLayer::onTouchBegan(Touch* touch, Event* event)
{
    return true;
}

void PopupLayer::onTouchEnded(Touch* touch, Event* event)
{
    
}

void PopupLayer::onTouchCancelled(Touch* touch, Event* event)
{
    
}