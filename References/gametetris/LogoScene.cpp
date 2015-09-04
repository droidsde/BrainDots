//
//  LogoScene.cpp
//  GameTetris
//
//  Created by Nguyen Minh Hoang on 2015/03/19.
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
    
    auto logo = Sprite::create("biglogo.png");
    logo->setPosition(origin.x + visibleSize.width/2 , origin.y + visibleSize.height/2 );
    logo->setName("logo");
    this->addChild(logo);
    logo->runAction(Sequence::create(FadeTo::create(TIME_TRANSITION_SCENE, 255), CallFunc::create(CC_CALLBACK_0(LogoScene::loadResources, this)), nullptr));
}

void LogoScene::loadResources()
{
    Director::getInstance()->getTextureCache()->addImageAsync("tetrisSpriteSheet.png", CC_CALLBACK_1(LogoScene::loadImage, this));
}

void LogoScene::loadSounds()
{
    ManageSound::getInstance()->preLoad();
}


void LogoScene::loadImage(cocos2d::Texture2D *texture)
{
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("tetrisSpriteSheet.plist",texture);
    
    this->loadSounds();
    
    this->startGame();
}

void LogoScene::startGame()
{
    CallFunc* nextScene = CallFunc::create(std::bind(&LogoScene::nextScene, this));
    this->getChildByName("logo")->runAction(Sequence::create(FadeOut::create(TIME_TRANSITION_SCENE), nextScene, nullptr));
}

void LogoScene::nextScene()
{
    ManageScene::getInstance()->changeState(GAME_STATE::MENU);
}

void LogoScene::onExit()
{
    Layer::onExit();
    this->stopAllActions();
}