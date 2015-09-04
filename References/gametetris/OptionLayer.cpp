//
//  OptionLayer.cpp
//  GameTetris
//
//  Created by Nguyen Minh Hoang on 2015/04/02.
//
//

#include "OptionLayer.h"

bool OptionLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    originPoint = Director::getInstance()->getVisibleOrigin();
    visibleSize = Director::getInstance()->getVisibleSize();
    
    addLayerOption();
    
    return true;
}

void OptionLayer::addLayerOption()
{
    this->setMS(ManageSound::getInstance());
    this->ignoreAnchorPointForPosition(false);
    Size winSize = Director::getInstance()->getWinSize();
    
    auto eventListener = EventListenerTouchOneByOne::create();
    eventListener->setSwallowTouches(true);
    eventListener->onTouchBegan = CC_CALLBACK_2(OptionLayer::onTouchBegan, this);
    eventListener->onTouchCancelled = CC_CALLBACK_2(OptionLayer::onTouchCancelled, this);
    eventListener->onTouchEnded = CC_CALLBACK_2(OptionLayer::onTouchEnded, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, this);

    // father node
    Node* rootNode = CSLoader::createNode("OptionLayer.csb");
    rootNode->setName("rootNode");
    rootNode->setPosition(winSize.width / 2, winSize.height / 2 );
    rootNode->setAnchorPoint(Vec2(0.5 , 0.5));
    this->addChild(rootNode);
    
    this->setPosition(winSize.width/2, -winSize.height/2);
    
    // button close
    auto closeOption = dynamic_cast<Button*> (rootNode->getChildByName("closeButton"));
    if (closeOption)
    {
        closeOption->addClickEventListener(CC_CALLBACK_1(OptionLayer::closeOption, this));
    }
    
    // music checkbox
    auto musicCheckbox = dynamic_cast<ui::CheckBox*>(rootNode->getChildByName("musicCheckbox"));
    if (musicCheckbox)
    {
        musicCheckbox->addEventListener(CC_CALLBACK_2(OptionLayer::musicCheckBoxCallback, this));
    }
    
    auto soundCheckbox = dynamic_cast<CheckBox*>(rootNode->getChildByName("soundCheckbox"));
    if (soundCheckbox)
    {
        soundCheckbox->addEventListener(CC_CALLBACK_2(OptionLayer::soundCheckBoxCallback, this));
    }
    
    musicCheckbox->setSelected(getMS()->isMusicOn());
    soundCheckbox->setSelected(getMS()->isSoundOn());
    
    // speed choose
    int curSpeed = ManageScene::getInstance()->getSpeedLevel();
    auto speedText = dynamic_cast<Text*>(rootNode->getChildByName("speedText"));
    speedText->setString(String::createWithFormat("%d", curSpeed)->getCString());
    
    auto speedSlider = dynamic_cast<Slider*>(rootNode->getChildByName("speedSlider"));
    if (speedSlider)
    {
        speedSlider->addEventListener(CC_CALLBACK_2(OptionLayer::speedSliderCallback, this));
    }
    speedSlider->setPercent(curSpeed * 10);
}

void OptionLayer::closeOption(cocos2d::Ref *sender)
{
    getMS()->playClickSound();
    auto action = Spawn::create(MoveTo::create(0.3f, Point(originPoint.x + visibleSize.width / 2, -(originPoint.y + visibleSize.height / 2))), FadeOut::create(0.3f), ScaleBy::create(0.3f, 0.3f), nullptr);
    auto set = CallFunc::create(CC_CALLBACK_0(OptionLayer::setVisible, this, false));
    auto remove = CallFunc::create(CC_CALLBACK_0(OptionLayer::removeFromParent, this));
    auto action_ = Sequence::create(action, set, remove, nullptr);
    this->runAction(action_);
}

void OptionLayer::musicCheckBoxCallback(cocos2d::Ref *pSender, CheckBox::EventType type)
{
    switch (type)
    {
        case CheckBox::EventType::SELECTED :
            ManageSound::getInstance()->setMusicOn(true);
            break;
        case CheckBox::EventType::UNSELECTED :
            ManageSound::getInstance()->setMusicOn(false);
            break;
    }
}

void OptionLayer::soundCheckBoxCallback(cocos2d::Ref *pSender, CheckBox::EventType type)
{
    switch (type)
    {
        case CheckBox::EventType::SELECTED :
            ManageSound::getInstance()->setSoundOn(true);
            break;
        case CheckBox::EventType::UNSELECTED :
            ManageSound::getInstance()->setSoundOn(false);
            break;
    }
}

void OptionLayer::speedSliderCallback(cocos2d::Ref *pSender, Slider::EventType type)
{
    auto rootNode = this->getChildByName("rootNode");
    if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
    {
        Slider* slider = dynamic_cast<Slider*>(pSender);
        int percent = slider->getPercent();
        int tem = percent / 10;
        for(auto node : rootNode->getChildren())
        {
            if (node->getName() == "speedText")
            {
                auto speedText = dynamic_cast<Text*>(node);
                speedText->setString(String::createWithFormat("%d", tem)->getCString());
                ManageScene::getInstance()->setSpeedLevel(tem);

            }
        }
    }
}

void OptionLayer::onEnter()
{
    Layer::onEnter();
    auto action = Spawn::create(MoveTo::create(0.3f, Point(originPoint.x + visibleSize.width / 2, (originPoint.y + visibleSize.height / 2))), FadeIn::create(0.3f), nullptr);
    this->runAction(action);
}

bool OptionLayer::onTouchBegan(Touch* touch, Event* event)
{
    return true;
}

void OptionLayer::onTouchEnded(Touch* touch, Event* event)
{
    
}

void OptionLayer::onTouchCancelled(Touch* touch, Event* event)
{
    
}