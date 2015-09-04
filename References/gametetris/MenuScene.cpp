//
//  MenuScene.cpp
//  GameTetris
//
//  Created by Nguyen Minh Hoang on 2015/03/19.
//
//

#include "MenuScene.h"

Scene* MenuScene::createScene()
{
    auto scene = Scene::create();
    auto layer = MenuScene::create();
    
    layer->setMS(ManageSound::getInstance());
    
    scene->addChild(layer);
    
    return scene;
}


bool MenuScene::init()
{
    if(!Layer::init())
    {
        return false;
    }
    originalPoint = Director::getInstance()->getVisibleOrigin();
    visibleSize = Director::getInstance()->getVisibleSize();
    
    auto keyboardListener = cocos2d::EventListenerKeyboard::create();
    keyboardListener->onKeyReleased = CC_CALLBACK_2(MenuScene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    addMenu();
    return true;
}

void MenuScene::addMenu()
{
    // add background and title
    addBackgroundAndTitle();
    
    //TODO button
    addButtons();
}

void MenuScene::addBackgroundAndTitle()
{
    // background
    auto background = Sprite::createWithSpriteFrameName("background.png");
    float scale_x = visibleSize.width / background->getContentSize().width;
    float scale_y = visibleSize.height / background->getContentSize().height;
    
    background->setScaleX(scale_x);
    background->setScaleY(scale_y);
    background->setPosition(originalPoint.x + visibleSize.width/2, originalPoint.y + visibleSize.height/2);
    this->addChild(background);
    
    // title
    auto title = Label::createWithTTF("TETRIS", "kenpixel_future_square.ttf", 80);
    title->setPosition(originalPoint.x + visibleSize.width/2, originalPoint.y + visibleSize.height/ 10 * 9);
    title->enableOutline(Color4B(25,26,25,155),5);
    this->addChild(title);
    
    // version
    auto version = Label::createWithTTF(VERSION, "kenpixel_future_square.ttf", 20);
    version->setAnchorPoint(Vec2(1.0f, 0.0f));
    version->setPosition(originalPoint.x + visibleSize.width - PADDING_BUTTON_MENU, originalPoint.y + PADDING_BUTTON_MENU);
    version->setColor(Color3B::WHITE);
    this->addChild(version);
}

void MenuScene::addButtons()
{
//    // background for buttons menu
//    auto background = Sprite::createWithSpriteFrameName("menu_board.png");
//    background->setPosition(originalPoint.x + visibleSize.width/2, originalPoint.y + visibleSize.height / 2);
//    background->setAnchorPoint(Vec2(0.5, 0.5));
//    
//    float halfWidth = background->getContentSize().width / 2;
//    float halfHeight = background->getContentSize().height / 2;
//    
//    auto playButton = MenuItemSprite::create(Sprite::createWithSpriteFrameName("play.png"), Sprite::createWithSpriteFrameName("play_press.png"), CC_CALLBACK_1(MenuScene::startGame, this));
//    playButton->setPosition(halfWidth, halfHeight + playButton->getContentSize().height/2 + PADDING_BUTTON_MENU/2);
//    
//    auto optionButton = MenuItemSprite::create(Sprite::createWithSpriteFrameName("option.png"), Sprite::createWithSpriteFrameName("option_press.png"), CC_CALLBACK_1(MenuScene::showOption, this));
//    optionButton->setPosition(halfWidth, halfHeight - playButton->getContentSize().height/2 - PADDING_BUTTON_MENU/2);
//    
//    auto menu = Menu::create(playButton, optionButton, NULL);
//    menu->setPosition(Point::ZERO);
//    
//    background->addChild(menu);
//    this->addChild(background);
    
    Node *rootNode = CSLoader::getInstance()->createNodeWithFlatBuffersFile("MenuLayer.csb");
//    CSLoader::createNode("MenuLayer.csb");
    rootNode->setAnchorPoint(Vec2(0.5, 0.5));
    rootNode->setPosition(originalPoint.x + visibleSize.width/2, originalPoint.y + visibleSize.height / 2);
    this->addChild(rootNode);
    
    auto playButton = dynamic_cast<ui::Button*> (rootNode->getChildByName("playButton"));
    if (playButton)
    {
        playButton->addClickEventListener(CC_CALLBACK_1(MenuScene::startGame, this));
    }
    
    auto optionButton = dynamic_cast<ui::Button*>(rootNode->getChildByName("optionButton"));
    if (optionButton)
    {
        optionButton->addClickEventListener(CC_CALLBACK_1(MenuScene::showOption, this));
    }
    
    auto highscoreButton = dynamic_cast<ui::Button*>(rootNode->getChildByName("highscoreButton"));
    if (highscoreButton)
    {
        highscoreButton->addClickEventListener(CC_CALLBACK_1(MenuScene::showHighScore, this));
    }
}

void MenuScene::startGame(cocos2d::Ref *sender)
{
    getMS()->playClickSound();
    ManageScene::getInstance()->changeState(GAME_STATE::GAME);
}

void MenuScene::showOption(cocos2d::Ref *sender)
{
    getMS()->playClickSound();
    auto optionLayer = OptionLayer::create();
    this->addChild(optionLayer);
}

void MenuScene::showHighScore(cocos2d::Ref *sender)
{
    getMS()->playClickSound();
    ManageScene::getInstance()->changeState(GAME_STATE::HIGHSCORE);
}

void MenuScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		Director::getInstance()->end();
	}
}
