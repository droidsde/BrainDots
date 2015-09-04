//
//  MenuScene.cpp
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/04.
//
//

#include "MenuScene.h"

Scene* MenuScene::createScene()
{
    auto scene = Scene::create();
    auto layer = MenuScene::create();
    layer->setSoundMusic(SoundManager::getInstance());
    scene->addChild(layer);
    return scene;
}

bool MenuScene::init()
{
    if (!LayerColor::initWithColor(Color4B(255,255,255,255))) {
        return false;
    }
    
    origin = Director::getInstance()->getVisibleOrigin();
    visibleSize = Director::getInstance()->getVisibleSize() ;
    
    auto title = Button::create();
    title->setTouchEnabled(true);
    title->setTitleText("Brain Dots");
    title->setTitleFontSize(80);
    title->setTitleFontName("arial.ttf");
    title->setTitleColor(Color3B::ORANGE);
    title->setAnchorPoint(Vec2(0, 1));
    title->setPosition(Vec2(20, visibleSize.height-20));
    addChild(title);
    
    auto listview = ListView::create();
    listview->setTouchEnabled(true);
    
    return true;
}

void MenuScene::onEnter()
{
    LayerColor::onEnter();
}

void MenuScene::onExit()
{
    LayerColor::onExit();
}