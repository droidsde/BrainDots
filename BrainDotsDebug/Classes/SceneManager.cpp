//
//  SceneManager.cpp
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/03.
//
//

#include "SceneManager.h"

SceneManager* SceneManager::_sceneManager = nullptr;

SceneManager::SceneManager()
{
    
}

SceneManager::~SceneManager()
{
    CC_SAFE_DELETE(_sceneManager);
}

SceneManager* SceneManager::getInstance()
{
    if (_sceneManager == nullptr) {
        _sceneManager = new SceneManager();
    }
    
    return _sceneManager;
}

void SceneManager::changeState(GAME_STATE state)
{
    this->setCurState(state);
    
    switch (state) {
        case GAME_STATE::LOGO :
            moveLogoScene();
            break;
            
        case GAME_STATE::SPLASH_SCREEN :
            moveSplashScreen();
            break;
            
        case GAME_STATE::MENU :
            moveMenuScene();
            break;
        
        case GAME_STATE::SETTING :
            moveSettingLayer();
            break;
            
        case GAME_STATE::GAME :
            moveGameScene();
            break;
            
        case GAME_STATE::OVER :
            moveGameOverScene();
            break;
            
        case GAME_STATE::HIGHSCORE :
            moveHighScore();
            break;
        default:
            break;
    }
}

void SceneManager::nextScene(cocos2d::Scene * scene)
{
    this->setCurScene(scene);
    TransitionScene* transition = TransitionFade::create(TIME_TRANSITION_SCENE, scene);
    Director::getInstance()->replaceScene(transition);
}

void SceneManager::moveLogoScene()
{
    auto scene = LogoScene::createScene();
    this->nextScene(scene);
}

void SceneManager::moveSplashScreen()
{
    
}

void SceneManager::moveMenuScene()
{
    auto scene = MenuScene::createScene();
    this->nextScene(scene);
}

void SceneManager::moveSettingLayer()
{
    
}

void SceneManager::moveGameScene()
{
    auto scene = GameScene::createScene();
    this->nextScene(scene);
}

void SceneManager::moveGameOverScene()
{
    
}

void SceneManager::moveHighScore()
{
    
}