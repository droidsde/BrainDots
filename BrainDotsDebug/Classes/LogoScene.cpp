//
//  LogoScene.cpp
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/03.
//
//

#include "LogoScene.h"

Scene* LogoScene::createScene()
{
    auto scene = Scene::create();
    auto layer = LogoScene::create();
    scene->addChild(layer);
    return scene;
}

bool LogoScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    this->addLogo();
    return true;
}

void LogoScene::addLogo()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto logo = Sprite::create("logo.png");
    logo->setPosition(origin.x + visibleSize.width/2 , origin.y + visibleSize.height/2 );
    float scaleX = visibleSize.width / logo->getContentSize().width;
    float scaleY = visibleSize.height / logo->getContentSize().height;
    logo->setScale(scaleX, scaleY);
    logo->setName("logo");
    logo->setOpacity(0);
    this->addChild(logo);
    logo->runAction(Sequence::create(FadeIn::create(TIME_TRANSITION_SCENE), CallFunc::create(CC_CALLBACK_0(LogoScene::startSplashScreen, this)), nullptr));
}

void LogoScene::loadResources()
{
//    Director::getInstance()->getTextureCache()->addImageAsync("tetrisSpriteSheet.png", CC_CALLBACK_1(LogoScene::loadImage, this));
}

void LogoScene::loadImage(cocos2d::Texture2D *texture)
{
//    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("tetrisSpriteSheet.plist",texture);
    
//    this->loadSounds();
    
//    this->startGame();
}

void LogoScene::loadSounds()
{
    SoundManager::getInstance()->preLoad();
}

void LogoScene::startSplashScreen()
{
    CallFunc* nextScene = CallFunc::create(std::bind(&LogoScene::nextScene, this));
    this->getChildByName("logo")->runAction(Sequence::create(DelayTime::create(TIME_TRANSITION_SCENE),FadeOut::create(TIME_TRANSITION_SCENE), nextScene, nullptr));
}

void LogoScene::nextScene()
{
    SceneManager::getInstance()->changeState(GAME_STATE::MENU);
}

void LogoScene::onExit()
{
    Layer::onExit();
    this->stopAllActions();
}