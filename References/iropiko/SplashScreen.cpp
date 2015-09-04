//
//  SplashScreen.cpp
//  PoolExample
//
//  Created by Nguyen Minh Hoang on 2015/07/27.
//
//

#include "SplashScreen.h"
USING_NS_CC;

Scene* SplashScreen::createScene()
{
    auto scene = Scene::create();
    
    auto layer = SplashScreen::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool SplashScreen::init()
{
    if (!LayerColor::initWithColor(Color4B(0,0,0,255))) {
        return false;
    }

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    CCLOG("%s", FileUtils::getInstance()->getWritablePath().c_str());
    // add logo company
    addLogo();
    
    return true;
}

void SplashScreen::addLogo()
{
    auto logo = Sprite::create("biglogo.png");
    logo->setAnchorPoint(Vec2(0.5, 0.5));
    logo->setPosition(origin.x + visibleSize.width/2, origin.y + visibleSize.height*3/4);
    addChild(logo);
    logo->runAction(Sequence::create(FadeIn::create(TIME_TRANSITION_SCENE), CallFunc::create(CC_CALLBACK_0(SplashScreen::addWords, this)), nullptr));
}

void SplashScreen::addWords()
{
    if (label!=nullptr) {
        label->removeFromParent();
    }
    label = CCLabelBMFontAnimated::createWithTTF("かずびこ", "fonts/keifont.ttf", 150.0f, Size(visibleSize.width, 40), TextHAlignment::CENTER, TextVAlignment::TOP);
    label->setTextColor(Color4B::WHITE);
    label->setAnchorPoint(Vec2(0.5, 0.5));
    label->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
    addChild(label);
    label->animateInDropFromTop(2, CallFunc::create(CC_CALLBACK_0(SplashScreen::nextScene, this)));
}

void SplashScreen::nextScene()
{
    ManageScene::getInstance()->changeState(GAME_STATE::MENU);
}

void SplashScreen::onExit()
{
    LayerColor::onExit();
    this->stopAllActions();
}
