//
//  MenuScene.cpp
//  PoolExample
//
//  Created by Nguyen Minh Hoang on 2015/07/28.
//
//

#include "MenuScene.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

Scene* MenuScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = MenuScene::create();
    
    scene->addChild(layer);
    
    return scene;
}


bool MenuScene::init()
{
    if (!LayerColor::initWithColor(Color4B(0,0,0,255))) {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    // main layout
    mainLayout = Layout::create();
    mainLayout->setSize(visibleSize);
    mainLayout->setPosition(Vec2(0, 0));
    addChild(mainLayout);
    
    // setting layout
    settingLayout = Layout::create();
    settingLayout->setSize(visibleSize);
    settingLayout->setPosition(Vec2(visibleSize.width, 0));
    addChild(settingLayout);
    
    // setup main layout
    addWords();
    addChooseLanguages();
    
    // setup setting layout
    addSettings();
    return true;
}

void MenuScene::addWords()
{
    // label app name
    if (label != nullptr) {
        label->removeFromParent();
    }
    
    label = CCLabelBMFontAnimated::createWithTTF("かずびこ", "fonts/keifont.ttf", 120.0f, Size(visibleSize.width, 40), TextHAlignment::CENTER, TextVAlignment::TOP);
    label->setTextColor(Color4B::WHITE);
    label->setAnchorPoint(Vec2(0.5, 0.5));
    label->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height*5/6));
    mainLayout->addChild(label);
    auto seq = Sequence::create(CallFunc::create(CC_CALLBACK_0(CCLabelBMFontAnimated::animateRainbow, label, 1.2f)),DelayTime::create(2.f), NULL);
    this->runAction(RepeatForever::create(seq));
    
    // label description
    auto descript = Label::createWithTTF("かずをおぼえよう", "fonts/keifont.ttf", 40.0f);
    descript->setTextColor(Color4B::WHITE);
    descript->setPosition(Vec2(label->getPositionX(), label->getPositionY() - 150));
    mainLayout->addChild(descript);
    
    // add button start
    auto button = Text::create("はじめる", "fonts/keifont.ttf", 50);
    button->enableOutline(Color4B::RED, 5);
    button->setTouchEnabled(true);
    button->setTouchScaleChangeEnabled(true);
    button->setColor(Color3B::YELLOW);
    button->setPosition(Vec2(label->getPositionX(), origin.y + visibleSize.height/4));
    mainLayout->addChild(button);
    button->setTag(Tag::T_PLAYBUTTON);
    auto animZoom = Sequence::create(ScaleTo::create(0.2, 0.95), EaseElasticOut::create(ScaleTo::create(1.0, 1.2f)), DelayTime::create(0.7f), NULL);
    button->runAction(RepeatForever::create(animZoom));
    button->addTouchEventListener(CC_CALLBACK_2(MenuScene::touchButtonEvent, this));
    
    // add button audio
    auto audio = Text::create("サウンド管理", "fonts/keifont.ttf", 50);
    audio->enableOutline(Color4B::RED, 5);
    audio->setTouchEnabled(true);
    audio->setTouchScaleChangeEnabled(true);
    audio->setColor(Color3B::YELLOW);
    audio->setPosition(Vec2(button->getPositionX(), button->getPositionY() - 100));
    audio->setTag(Tag::T_AUDIORECORDBUTTON);
    audio->addTouchEventListener(CC_CALLBACK_2(MenuScene::touchButtonEvent, this));
    mainLayout->addChild(audio);
    
    // add button setting
    auto setting = Text::create("設定", "fonts/keifont.ttf", 50);
    setting->enableOutline(Color4B::RED, 5);
    setting->setTouchEnabled(true);
    setting->setTouchScaleChangeEnabled(true);
    setting->setColor(Color3B::YELLOW);
    setting->setPosition(Vec2(button->getPositionX(), audio->getPositionY() - 100));
    mainLayout->addChild(setting);
    setting->setTag(Tag::T_SETTINGBUTTON);
    setting->addTouchEventListener(CC_CALLBACK_2(MenuScene::touchButtonEvent, this));

}

void MenuScene::addChooseLanguages()
{
    flagJp = Button::create("flag_jp.png");
    flagJp->setTouchEnabled(true);
    flagJp->setTag(Tag::T_JPBUTTON);
    flagJp->addTouchEventListener(CC_CALLBACK_2(MenuScene::touchButtonEvent, this));
    mainLayout->addChild(flagJp);
    
    flagEn = Button::create("flag_en.png");
    flagEn->setTouchEnabled(true);
    flagEn->setTag(Tag::T_ENBUTTON);
    flagEn->addTouchEventListener(CC_CALLBACK_2(MenuScene::touchButtonEvent, this));
    mainLayout->addChild(flagEn);
    
    draw = DrawNode::create();
    mainLayout->addChild(draw);
    
    flagJp->setPosition(Vec2(origin.x + visibleSize.width/2 - 150, origin.y + visibleSize.height/2));
    flagEn->setPosition(Vec2(origin.x + visibleSize.width/2 + 150, flagJp->getPositionY()));
    drawSelected(flagJp->getPosition());
    ManageScene::getInstance()->loadResouceJson("ja.json");
}

void MenuScene::addSettings()
{
    auto board = Sprite::create("menu_board.png");
    board->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
    settingLayout->addChild(board);
    
    float board_width = board->getContentSize().width;
    float board_height = board->getContentSize().height;
    
    // button close
    auto close = Button::create("close_button.png");
    close->setTouchEnabled(true);
    close->setAnchorPoint(Vec2(0.5, 0.5));
    close->setPosition(Vec2(board_width, board_height));
    board->addChild(close);
    close->setTag(Tag::T_CLOSESETTINGBUTTON);
    close->addTouchEventListener(CC_CALLBACK_2(MenuScene::touchButtonEvent, this));
    
    // 3 label
    auto title = Text::create("設定", "fonts/keifont.ttf", 50);
    auto namenumber = Text::create("文字", "fonts/keifont.ttf", 40);
    auto number = Text::create("数字", "fonts/keifont.ttf", 40);
    auto sound = Text::create("サウンド", "fonts/keifont.ttf", 40);
    board->addChild(namenumber);
    board->addChild(number);
    board->addChild(sound);
    board->addChild(title);
    
    title->setPosition(Vec2(board_width/2, board_height - 50));
    namenumber->setAnchorPoint(Vec2(0,0.5));
    number->setAnchorPoint(Vec2(0,0.5));
    sound->setAnchorPoint(Vec2(0,0.5));
    namenumber->setPosition(Vec2(50, 300));
    number->setPosition(Vec2(50, 200));
    sound->setPosition(Vec2(50, 100));
    
    // Create the switch
    /////////////////////////////////////
    //-------- NUMBER NAME
    scNumberName = ControlSwitch::create
    (
     Sprite::create("switch-mask.png"),
     Sprite::create("switch-on.png"),
     Sprite::create("switch-off.png"),
     Sprite::create("switch-thumb.png"),
     Label::createWithTTF("オン", "fonts/keifont.ttf", 32),
     Label::createWithTTF("オフ", "fonts/keifont.ttf", 32)
     );
    scNumberName->setPosition(Vec2(board_width-100, namenumber->getPositionY()));
    scNumberName->setTag(Tag::T_NUMBERNAME);
    scNumberName->setOn(ManageScene::getInstance()->getShowNameNumber());
    board->addChild(scNumberName);
    
    /////////////////////////////////////
    //-------- NUMBERs
    scNumber = ControlSwitch::create
    (
     Sprite::create("switch-mask.png"),
     Sprite::create("switch-on.png"),
     Sprite::create("switch-off.png"),
     Sprite::create("switch-thumb.png"),
     Label::createWithTTF("オン", "fonts/keifont.ttf", 32),
     Label::createWithTTF("オフ", "fonts/keifont.ttf", 32)
     );
    scNumber->setPosition(Vec2(board_width-100, number->getPositionY()));
    scNumber->setTag(Tag::T_NUMBER);
    scNumber->setOn(ManageScene::getInstance()->getShowNumber());
    board->addChild(scNumber);
    
    /////////////////////////////////////
    //-------- SOUND
    scSound = ControlSwitch::create
    (
     Sprite::create("switch-mask.png"),
     Sprite::create("switch-on.png"),
     Sprite::create("switch-off.png"),
     Sprite::create("switch-thumb.png"),
     Label::createWithTTF("オン", "fonts/keifont.ttf", 32),
     Label::createWithTTF("オフ", "fonts/keifont.ttf", 32)
     );
    scSound->setPosition(Vec2(board_width-100, sound->getPositionY()));
    scSound->setTag(Tag::T_SOUND);
    scSound->setOn(ManageScene::getInstance()->getPLaySound());
    board->addChild(scSound);
    
    scNumberName->addTargetWithActionForControlEvents(this, cccontrol_selector(MenuScene::valueChanged), Control::EventType::VALUE_CHANGED);
    scNumber->addTargetWithActionForControlEvents(this, cccontrol_selector(MenuScene::valueChanged), Control::EventType::VALUE_CHANGED);
    scSound->addTargetWithActionForControlEvents(this, cccontrol_selector(MenuScene::valueChanged), Control::EventType::VALUE_CHANGED);
}

void MenuScene::valueChanged(cocos2d::Ref *sender, cocos2d::extension::Control::EventType controlEvent)
{
    ControlSwitch* pSwitch = (ControlSwitch*)sender;
    switch (pSwitch->getTag()) {
        case Tag::T_NUMBERNAME :
            CCLOG("%s",pSwitch->isOn()==true?"numbername on":"numbername off");
            if (pSwitch->isOn()) {
                ManageScene::getInstance()->setShowNameNumber(true);
            } else {
                ManageScene::getInstance()->setShowNameNumber(false);
                if (!scNumber->isOn()) {
                    scNumber->setOn(true, true);
                }
            }
            break;
        case Tag::T_NUMBER :
            CCLOG("%s",pSwitch->isOn()==true?"number on":"number off");
            if (pSwitch->isOn()) {
                ManageScene::getInstance()->setShowNumber(true);
            } else {
                ManageScene::getInstance()->setShowNumber(false);
                if (!scNumberName->isOn()) {
                    scNumberName->setOn(true, true);
                }
            }
            break;
        case Tag::T_SOUND :
            CCLOG("%s",pSwitch->isOn()==true?"sound on":"sound off");
            if (pSwitch->isOn()) {
                ManageScene::getInstance()->setPLaySound(true);
            } else {
                ManageScene::getInstance()->setPLaySound(false);
            }
            break;
        default:
            break;
    }
}

void MenuScene::touchButtonEvent(Ref* sender, Widget::TouchEventType type)
{
    auto receiver = (Node*) sender;
    switch (receiver->getTag()) {
        case Tag::T_PLAYBUTTON :
        {
            if (type == ui::Widget::TouchEventType::ENDED) {
                nextScene();
            }
            break;
        }
            
        case Tag::T_AUDIORECORDBUTTON :
        {
            if (type == ui::Widget::TouchEventType::ENDED) {
                this->moveAudioManagerScene();
            }
            break;
        }
            
        case Tag::T_SETTINGBUTTON :
        {
            if (type == ui::Widget::TouchEventType::ENDED) {
                this->moveSetting();
            }
            break;
        }
            
        case Tag::T_JPBUTTON :
        {
            if (type == ui::Widget::TouchEventType::ENDED) {
                ManageScene::getInstance()->loadResouceJson("ja.json");
                drawSelected(flagJp->getPosition());
            }
            break;
        }
            
        case Tag::T_ENBUTTON :
        {
            if (type == ui::Widget::TouchEventType::ENDED) {
                ManageScene::getInstance()->loadResouceJson("en.json");
                drawSelected(flagEn->getPosition());
            }
            break;
        }
        
        case Tag::T_CLOSESETTINGBUTTON :
        {
            if (type == ui::Widget::TouchEventType::ENDED) {
                this->moveMainMenu();
            }
            break;
        }
        default:
            break;
    }
}

void MenuScene::drawSelected(cocos2d::Vec2 position)
{
    draw->clear();
    draw->drawRect(Vec2(position.x - 150, position.y + 150), Vec2(position.x + 150, position.y - 150), Color4F::RED);
}

void MenuScene::nextScene()
{
    CCLOG("test language %s", ManageScene::getInstance()->getCurrentLanguage().c_str());
    ManageScene::getInstance()->changeState(GAME_STATE::GAME);
}

void MenuScene::moveSetting()
{
    auto moveLeft1 = MoveTo::create(0.5f, Vec2(-visibleSize.width, 0));
    auto moveLeft2 = MoveTo::create(0.5f, Vec2(0, 0));
    mainLayout->runAction(moveLeft1);
    settingLayout->runAction(moveLeft2);
}

void MenuScene::moveAudioManagerScene()
{
    ManageScene::getInstance()->changeState(GAME_STATE::RECORD);
}

void MenuScene::moveMainMenu()
{
    auto moveLeft1 = MoveTo::create(0.5f, Vec2(0, 0));
    auto moveLeft2 = MoveTo::create(0.5f, Vec2(visibleSize.width, 0));
    mainLayout->runAction(moveLeft1);
    settingLayout->runAction(moveLeft2);
}

void MenuScene::onExit()
{
    LayerColor::onExit();
    this->stopAllActions();
}
