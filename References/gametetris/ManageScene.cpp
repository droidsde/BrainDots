//
//  ManageScene.cpp
//  GameTetris
//
//  Created by Nguyen Minh Hoang on 2015/03/19.
//
//
#include "ManageScene.h"

ManageScene* ManageScene::_manageScene = nullptr;

ManageScene::ManageScene()
{
    curSpeed = 0;
    curScore = 0;
}

ManageScene::~ManageScene()
{
    CC_SAFE_DELETE(_manageScene);
}

ManageScene* ManageScene::getInstance()
{
    
    if (_manageScene == nullptr)
    {
        _manageScene = new ManageScene();
        return _manageScene;
    }else
        return _manageScene;
    
}

void ManageScene::changeState(GAME_STATE state)
{
    
    this->setCurState(state);
    
    switch (state) {
            
        case GAME_STATE::LOGO :
            jumpToLogoScene();
            break;
            
        case GAME_STATE::MENU :
            jumpToMenuScene();
            break;
            
        case GAME_STATE::GAME :
            jumpToGameScene();
            break;
            
        case GAME_STATE::OVER :
            jumpToGameOverScene();
            break;
            
        case GAME_STATE::HIGHSCORE :
            jumpToHighScore();
            break;
        
        default:
            break;
    }
}

void ManageScene::nextScene(Scene* scene)
{
    this->setCurScene(scene);
    TransitionScene* transition = TransitionFade::create(TIME_TRANSITION_SCENE, scene);
    Director::getInstance()->replaceScene(transition);
}


void ManageScene::jumpToLogoScene()
{
    auto scene = LogoScene::createScene();
    this->nextScene(scene);
}


void ManageScene::jumpToMenuScene()
{
    auto scene = MenuScene::createScene();
    this->nextScene(scene);
}


void ManageScene::jumpToGameScene()
{
    auto scene = GameScene::createScene();
    this->nextScene(scene);
}


void ManageScene::jumpToGameOverScene()
{
    auto layer = GameOverLayer::create();
    this->getCurScene()->addChild(layer);
}

void ManageScene::jumpToHighScore()
{
    auto scene = HighScoreScene::createScene();
    this->nextScene(scene);
}
