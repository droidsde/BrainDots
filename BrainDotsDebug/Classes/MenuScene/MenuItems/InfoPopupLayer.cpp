//
//  InfoPopupLayer.cpp
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/10/16.
//
//

#include "InfoPopupLayer.h"

InfoPopupLayer::InfoPopupLayer()
{
    
}

InfoPopupLayer::~InfoPopupLayer()
{
    
}

InfoPopupLayer* InfoPopupLayer::create()
{
    InfoPopupLayer* layer = new InfoPopupLayer();
    layer->init();
    layer->autorelease();
    return layer;
}

bool InfoPopupLayer::init()
{
    BasePopupLayer::init();
    
    layoutSize = layoutTable->getContentSize();
    listviewSize = Size(layoutSize.width * 2/3, layoutSize.height * 3/4);
    // title
    auto title = Text::create("Information", "arial.ttf", 60);
    title->setPosition(Vec2(layoutSize.width/2, layoutSize.height - PADDING));
    title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    title->setColor(Color3B::ORANGE);
    layoutTable->addChild(title);
    
    // create layout profile
    float posY = title->getPositionY() - title->getContentSize().height/2 - PADDING_MENU_HEADER_ITEM;
    
    // textfield name
    Layout* layoutName = Layout::create();
    layoutName->setContentSize(Size(listviewSize.width, listviewSize.height / 4));
    layoutName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    layoutName->setPosition(Vec2(title->getPositionX(), posY));
    
    TextField* textName = TextField::create("", "arial.ttf", 50);
    textName->setString("Hoang Garu");
    textName->setMaxLengthEnabled(true);
    textName->setMaxLength(20);
    textName->setColor(Color3B::ORANGE);
    textName->setPosition(Vec2(PADDING, layoutName->getContentSize().height/2));
    textName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    textName->addEventListener(CC_CALLBACK_2(InfoPopupLayer::textFieldEvent, this));
    layoutName->addChild(textName);
    
    // line
    auto line = Sprite::create();
    line->setAnchorPoint(Vec2::ZERO);
    line->setPosition(Vec2::ZERO);
    line->setTextureRect(Rect(0, 0, listviewSize.width, 1));
    line->setColor(Color3B::ORANGE);
    layoutName->addChild(line);
    layoutTable->addChild(layoutName);
    
    // coin count
    createLayoutItem(Vec2(title->getPositionX(), posY - listviewSize.height/4), "coin_64x64.png", to_string(100));
    
    // level done
    createLayoutItem(Vec2(title->getPositionX(), posY - 2 * listviewSize.height/4), "mini_tick.png", "17/60");
    
    // replay count
    createLayoutItem(Vec2(title->getPositionX(), posY - 3 * listviewSize.height/4), "replay.png", "123/233");
    
    return true;
}

void InfoPopupLayer::createLayoutItem(cocos2d::Vec2 pos, std::string iconFile, std::string text)
{
    // create layout
    Layout* layout = Layout::create();
    layout->setContentSize(Size(listviewSize.width, listviewSize.height / 4));
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    layout->setPosition(pos);
    
    // icon
    auto icon = Sprite::create(iconFile);
    icon->setPosition(Vec2(PADDING, layout->getContentSize().height/2));
    icon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    layout->addChild(icon);
    
    // text
    auto textShow = Text::create(text, "arial.ttf", 50);
    textShow->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    textShow->setPosition(Vec2(listviewSize.width - PADDING, layout->getContentSize().height/2));
    textShow->setColor(Color3B::ORANGE);
    layout->addChild(textShow);
    
    // line
    auto line = Sprite::create();
    line->setAnchorPoint(Vec2::ZERO);
    line->setPosition(Vec2::ZERO);
    line->setTextureRect(Rect(0, 0, listviewSize.width, 1));
    line->setColor(Color3B::ORANGE);
    layout->addChild(line);
    
    layoutTable->addChild(layout);
}

void InfoPopupLayer::textFieldEvent(cocos2d::Ref *pSender, TextField::EventType type)
{
    switch (type) {
        case TextField::EventType::ATTACH_WITH_IME :
        {
            
        }
            break;
            
        case TextField::EventType::DETACH_WITH_IME:
        {
            
        }
            break;
        case TextField::EventType::INSERT_TEXT:
        {
            
        }
            break;
        
        case TextField::EventType::DELETE_BACKWARD:
        {
            
        }
            break;
            
        default:
            break;
    }
}

void InfoPopupLayer::onExit()
{
    BasePopupLayer::onExit();
    NotificationCenter::getInstance()->postNotification(EXIT_LAYER_INFO);
}