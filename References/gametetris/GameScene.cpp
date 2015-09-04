//
//  GameScene.cpp
//  GameTetris
//
//  Created by Nguyen Minh Hoang on 2015/03/19.
//
//

#include "GameScene.h"

Scene* GameScene::createScene()
{
    auto scene = Scene::create();
    auto layer = GameScene::create();
    scene->addChild(layer);
    return scene;
}

bool GameScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    originalPoint = Director::getInstance()->getVisibleOrigin();
    visibleSize = Director::getInstance()->getVisibleSize();
    
    // add layer
    addLayers();
    
    // add game board and hub layer
    animationBeforeStart();
    
    return true;
}


void GameScene::addLayers()
{
    // background
    auto background = Sprite::createWithSpriteFrameName("background.png");
    float scale_x = visibleSize.width / background->getContentSize().width;
    float scale_y = visibleSize.height / background->getContentSize().height;
    
    background->setScaleX(scale_x);
    background->setScaleY(scale_y);
    background->setPosition(originalPoint.x + visibleSize.width/2, originalPoint.y + visibleSize.height/2);
    this->addChild(background);
    
}

void GameScene::addGameBoard()
{
    // set up game board
    auto gameboard = GameBoard::create();
    this->addChild(gameboard, 50);
    
    // set up hud layer
    auto hub = HubLayer::create();
    this->addChild(hub);
    gameboard->setHub(hub);
    hub->setMyTouchDelegate(gameboard);
    
    // add pause layer
    auto pause = PauseLayer::create();
    this->addChild(pause, 300);
    gameboard->setpauseLayer(pause);
    pause->setGameBoardCallback(gameboard);
}

// animation before start game
void GameScene::animationBeforeStart()
{
    // title
    auto title = Label::createWithTTF("TETRIS", "kenpixel_future_square.ttf", 42);
    title->setPosition( -(originalPoint.x + visibleSize.width/4), originalPoint.y + visibleSize.height / 2  + BLOCK_WIDTH * BOARD_HEIGHT / 2 + 2 * PADDING_BUTTON_MENU);
    title->enableOutline(Color4B(25,26,25,155),5);
    this->addChild(title, 300);
    auto moveLeftToRight = MoveTo::create(0.5f, Vec2(PADDING_BUTTON_MENU + BLOCK_WIDTH * 5, originalPoint.y + visibleSize.height / 2  + BLOCK_WIDTH * BOARD_HEIGHT / 2 + 2 * PADDING_BUTTON_MENU));
    auto easeAnim = EaseBounceInOut::create(moveLeftToRight);

    // animation start
    // animation
    auto move = MoveTo::create(TIME_TRANSITION_SCENE, Vec2(originalPoint.x + visibleSize.width / 2 , originalPoint.y + visibleSize.height / 2));
    auto easeInElastic = EaseElasticInOut::create(move);
    auto fadeOut = FadeOut::create(0.2);
    
    // two label
    auto readyLabel = Label::createWithTTF("R E A D Y", "kenpixel_future_square.ttf", 50);
    readyLabel->setPosition(originalPoint.x + visibleSize.width / 2 , originalPoint.y + visibleSize.height + 2 * PADDING_BUTTON_MENU);
    readyLabel->setColor(Color3B::RED);
    readyLabel->enableOutline(Color4B(25,26,25,155),2);
    this->addChild(readyLabel);
    
    auto goLabel = Label::createWithTTF("G O !", "kenpixel_future_square.ttf", 50);
    goLabel->setPosition(originalPoint.x + visibleSize.width / 2 , originalPoint.y + visibleSize.height + 2 * PADDING_BUTTON_MENU);
    goLabel->setColor(Color3B::RED);
    goLabel->enableOutline(Color4B(25,26,25,155),2);
    this->addChild(goLabel);
    
    auto sequence = Sequence::create(easeInElastic, fadeOut, RemoveSelf::create(), nullptr);
    
    auto ac1 = TargetedAction::create(readyLabel, sequence);
    auto ac2 = TargetedAction::create(goLabel, sequence);
    auto ac3 = TargetedAction::create(title, easeAnim);
    auto seq = Sequence::create(ac1, ac2, ac3, CallFunc::create(CC_CALLBACK_0(GameScene::addGameBoard, this)), nullptr);
    runAction(seq);
}
