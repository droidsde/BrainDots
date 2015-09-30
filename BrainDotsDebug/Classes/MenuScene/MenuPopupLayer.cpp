//
//  MenuPopupLayer.cpp
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/09.
//
//

#include "MenuPopupLayer.h"

MenuPopupLayer::MenuPopupLayer()
{
    
}

MenuPopupLayer::~MenuPopupLayer()
{
    
}

MenuPopupLayer* MenuPopupLayer::create()
{
    MenuPopupLayer* layer = new MenuPopupLayer();
    layer->init();
    layer->autorelease();
    return layer;
}

bool MenuPopupLayer::init()
{
    BasePopupLayer::init();
    
    layoutSize = layoutTable->getContentSize();
    std::string menuItemFile[8] = {"menu_notification_icon.png", "menu_info_icon.png", "menu_setting_icon.png", "menu_blockads_icon.png","menu_support_icon.png", "menu_twitter_icon.png", "menu_facebook_icon.png", "menu_youtube_icon.png"};
    std::string menuItemName[8] = {"Notification", "Information", "Setting", "No more ads",
    "Support", "Twitter", "Facebook", "Youtube"};
    // title
    auto title = Text::create("Menu", "arial.ttf", 60);
    title->setPosition(Vec2(layoutSize.width/2, layoutSize.height - PADDING));
    title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    title->setColor(Color3B::RED);
    layoutTable->addChild(title);
    
    // add buttons
    float posX = layoutSize.width / 5;
    float posY = layoutSize.height / 3;
    
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 4; j++) {
            int index = 4 * i + j;
            
            // button
            auto button = Button::create(menuItemFile[index]);
            button->setPosition(Vec2(posX * (j+1), posY * (2-i)));
            button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            button->setTag(index);
            button->addTouchEventListener(CC_CALLBACK_2(MenuPopupLayer::touchButtonEvent, this));
            layoutTable->addChild(button);
            
            // text
            auto text = Text::create(menuItemName[index], "arial.ttf", 30);
            text->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            text->setColor(Color3B::ORANGE);
            text->setPosition(Vec2(button->getPositionX(), button->getPositionY() - button->getContentSize().height/2 - PADDING));
            layoutTable->addChild(text);
        }
    }
    
    return true;
}

void MenuPopupLayer::moveOrigin()
{
    layoutTable->runAction(MoveBy::create(0.3f, Vec2(PADDING_MENU_HEADER_ITEM, PADDING_MENU_HEADER_ITEM)));
}

void MenuPopupLayer::touchButtonEvent(cocos2d::Ref *sender, Widget::TouchEventType type)
{
    auto receiver = (Node*) sender;
    if (type == ui::Widget::TouchEventType::ENDED) {
        switch (receiver->getTag()) {
            case  TAG_MENU_ITEM::NOTIFICATION :
            {
                break;
            }
                
            case  TAG_MENU_ITEM::INFORMATION :
            {
                break;
            }
                
            case  TAG_MENU_ITEM::SETTING :
            {
//                layoutTable->runAction(MoveBy::create(0.3f, Vec2(-PADDING_MENU_HEADER_ITEM, -PADDING_MENU_HEADER_ITEM)));
                auto setting = SettingPopupLayer::create();
                this->addChild(setting);
                break;
            }
                
            case  TAG_MENU_ITEM::NOMOREADS :
            {
                break;
            }
                
            case  TAG_MENU_ITEM::SUPPORT :
            {
                break;
            }
                
            case  TAG_MENU_ITEM::TWITTER :
            {
                break;
            }
                
            case  TAG_MENU_ITEM::FACEBOOK :
            {
                break;
            }
                
            case  TAG_MENU_ITEM::YOUTUBE :
            {
                break;
            }
            default:
                break;
        }
    }
}