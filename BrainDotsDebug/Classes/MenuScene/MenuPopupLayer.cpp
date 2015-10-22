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

void MenuPopupLayer::onEnter()
{
    BasePopupLayer::onEnter();
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BasePopupLayer::moveOrigin), EXIT_LAYER_SETTING, NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BasePopupLayer::moveIn), EXIT_LAYER_LANGUAGE, NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BasePopupLayer::moveOut), OPEN_LAYER_LANGUAGE, NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BasePopupLayer::moveOrigin), EXIT_LAYER_INFO, NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BasePopupLayer::moveOrigin), EXIT_LAYER_NOTI, NULL);
}

void MenuPopupLayer::onExit()
{
    BasePopupLayer::onExit();
    NotificationCenter::getInstance()->removeObserver(this, EXIT_LAYER_SETTING);
    NotificationCenter::getInstance()->removeObserver(this, EXIT_LAYER_LANGUAGE);
    NotificationCenter::getInstance()->removeObserver(this, OPEN_LAYER_LANGUAGE);
    NotificationCenter::getInstance()->removeObserver(this, EXIT_LAYER_INFO);
    NotificationCenter::getInstance()->removeObserver(this, EXIT_LAYER_NOTI);
}

bool MenuPopupLayer::init()
{
    BasePopupLayer::init();
    
    layoutSize = layoutTable->getContentSize();
    std::string menuItemFile[8] = {"menu_notification_icon.png", "menu_info_icon.png", "menu_setting_icon.png", "menu_blockads_icon.png","menu_support_icon.png", "menu_twitter_icon.png", "menu_facebook_icon.png", "menu_youtube_icon.png"};
    std::string menuItemName[8] = {"Notification", "Information", "Setting", "No more ads",
    "Support", "Twitter", "Facebook", "Youtube"};
    // title
    auto title = Text::create("Menu", "fonts/keifont.ttf", 60);
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
            auto text = Text::create(menuItemName[index], "fonts/keifont.ttf", 30);
            text->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            text->setColor(Color3B::ORANGE);
            text->setPosition(Vec2(button->getPositionX(), button->getPositionY() - button->getContentSize().height/2 - PADDING));
            layoutTable->addChild(text);
        }
    }
    
    return true;
}


void MenuPopupLayer::touchButtonEvent(cocos2d::Ref *sender, Widget::TouchEventType type)
{
    auto receiver = (Node*) sender;
    if (type == ui::Widget::TouchEventType::ENDED) {
        switch (receiver->getTag()) {
            case  TAG_MENU_ITEM::NOTIFICATION :
            {
                auto noti = NotiPopupLayer::create();
                this->addChild(noti);
                moveOut(nullptr);
                break;
            }
                
            case  TAG_MENU_ITEM::INFORMATION :
            {
                auto info = InfoPopupLayer::create();
                this->addChild(info);
                moveOut(nullptr);
                break;
            }
                
            case  TAG_MENU_ITEM::SETTING :
            {
                auto setting = SettingPopupLayer::create();
                this->addChild(setting);
                moveOut(nullptr);
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
                Application::getInstance()->openURL("http://www.youtube.com/");
                break;
            }
            default:
                break;
        }
    }
}