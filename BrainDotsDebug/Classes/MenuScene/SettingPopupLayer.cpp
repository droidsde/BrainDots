//
//  SettingPopupLayer.cpp
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/30.
//
//

#include "SettingPopupLayer.h"

SettingPopupLayer::SettingPopupLayer()
{
    
}

SettingPopupLayer::~SettingPopupLayer()
{
    
}

SettingPopupLayer* SettingPopupLayer::create()
{
    SettingPopupLayer* layer = new SettingPopupLayer();
    layer->init();
    layer->autorelease();
    return layer;
}

void SettingPopupLayer::onEnter()
{
    BasePopupLayer::onEnter();
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SettingPopupLayer::reloadData), RELOAD_LANGUAGE, NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BasePopupLayer::moveOrigin), EXIT_LAYER_LANGUAGE, NULL);
}

void SettingPopupLayer::onExit()
{
    BasePopupLayer::onExit();
    NotificationCenter::getInstance()->removeObserver(this, RELOAD_LANGUAGE);
    NotificationCenter::getInstance()->removeObserver(this, EXIT_LAYER_LANGUAGE);
    NotificationCenter::getInstance()->postNotification(EXIT_LAYER_SETTING);
}

bool SettingPopupLayer::init()
{
    BasePopupLayer::init();
    std::string listItemName[6] = {"BGM", "Sound Effects", "Video recording", "Solution thumnail", "Language", "Reload Purchases"};
    
    layoutSize = layoutTable->getContentSize();
    listviewSize = Size(layoutSize.width * 2/3, layoutSize.height * 4/5);
    // title
    auto title = Text::create("Setting", "fonts/keifont.ttf", 60);
    title->setPosition(Vec2(layoutSize.width/2, layoutSize.height - PADDING));
    title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    title->setColor(Color3B::RED);
    layoutTable->addChild(title);
    
    // init listivew
    float posY = title->getPositionY() - title->getContentSize().height/2 - PADDING_MENU_HEADER_ITEM;
    
    for (int i = 0; i < 6; i++) {
        Layout* layout = Layout::create();
        layout->setTouchEnabled(true);
        layout->setContentSize(Size(listviewSize.width, listviewSize.height / 6));
        layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        layout->setPosition(Vec2(title->getPositionX(), posY - i * listviewSize.height/6));
        
        // text
        auto text = Text::create(listItemName[i], "fonts/keifont.ttf", 35);
        text->setColor(Color3B::ORANGE);
        text->setPosition(Vec2(PADDING, layout->getContentSize().height / 2));
        text->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        layout->addChild(text);
        
        // line
        auto line = Sprite::create();
        line->setAnchorPoint(Vec2::ZERO);
        line->setPosition(Vec2::ZERO);
        line->setTextureRect(Rect(0, 0, listviewSize.width, 1));
        line->setColor(Color3B::ORANGE);
        layout->addChild(line);
        
        // toggle
        if (i < 4) {
            ControlSwitch* toggle = ControlSwitch::create
            (
             Sprite::create("menu_switch_mask.png"),
             Sprite::create("menu_switch_on.png"),
             Sprite::create("menu_switch_off.png"),
             Sprite::create("menu_switch_thumb.png"),
             Label::createWithTTF("", "fonts/keifont.ttf", 32),
             Label::createWithTTF("", "fonts/keifont.ttf", 32)
             );
            toggle->setPosition(Vec2(listviewSize.width - PADDING, layout->getContentSize().height/2));
            toggle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            toggle->addTargetWithActionForControlEvents(this, cccontrol_selector(SettingPopupLayer::valueChanged), Control::EventType::VALUE_CHANGED);
            toggle->setTag(i);
            
            layout->addChild(toggle);
            
            switch (i) {
                case 0:
                    toggle->setOn(getSoundManage()->isMusicOn());
                    break;
                case 1:
                    toggle->setOn(getSoundManage()->isSoundOn());
                    break;
                case 2:
                    break;
                case 3:
                    break;
                default:
                    break;
            }
        } else if(i==4){
            languageText = Text::create("English", "fonts/keifont.ttf", 35);
            languageText->setString(SceneManager::getInstance()->getCurLanguage());
            languageText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            languageText->setColor(Color3B::ORANGE);
            languageText->setTouchEnabled(true);
            languageText->setPosition(Vec2(listviewSize.width - PADDING, layout->getContentSize().height/2));
            languageText->setTag(i);
            languageText->addTouchEventListener(CC_CALLBACK_2(SettingPopupLayer::touchButtonEvent, this));
            layout->addChild(languageText);
        }
        
        layoutTable->addChild(layout);
    }
    
    return true;
}

void SettingPopupLayer::reloadData(cocos2d::Ref *sender)
{
    if (languageText) {
        languageText->setString(SceneManager::getInstance()->getCurLanguage());
    }
}

void SettingPopupLayer::valueChanged(cocos2d::Ref *sender, cocos2d::extension::Control::EventType controlEvent)
{
    ControlSwitch* pSwitch = (ControlSwitch*)sender;
    switch (pSwitch->getTag()) {
        case TAG_SETTING_ITEM::MUSIC :
        {
            SoundManager::getInstance()->setMusicOn(pSwitch->isOn());
            break;
        }
            
        case TAG_SETTING_ITEM::SOUND :
        {
            SoundManager::getInstance()->setSoundOn(pSwitch->isOn());
            break;
        }
            
        case TAG_SETTING_ITEM::VIDEO :
        {
            break;
        }
            
        case TAG_SETTING_ITEM::THUMBNAIL :
        {
            break;
        }
    }
}

void SettingPopupLayer::touchButtonEvent(cocos2d::Ref *sender, Widget::TouchEventType type)
{
    auto receiver = (Node*) sender;
    if (type == ui::Widget::TouchEventType::ENDED) {
        switch (receiver->getTag()) {
            case TAG_SETTING_ITEM::LANGUGAGE :
            {
                LanguagePopupLayer* layer = LanguagePopupLayer::create();
                this->addChild(layer);
                moveOut(nullptr);
                NotificationCenter::getInstance()->postNotification(OPEN_LAYER_LANGUAGE);
                break;
            }
            case TAG_SETTING_ITEM::RELOAD_PURCHASES :
                break;
        }
    }
}