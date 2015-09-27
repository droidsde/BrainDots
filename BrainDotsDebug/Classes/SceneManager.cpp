//
//  SceneManager.cpp
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/03.
//
//

#include "SceneManager.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define FILE_FORMAT ("/mnt/sdcard/loading4.gif") //On the Android platform, the resources are compressed in the asset directory. Therefore, the resources must be files on the sd card
#else
#define FILE_FORMAT ("loading4.gif")
#endif

SceneManager* SceneManager::_sceneManager = nullptr;

SceneManager::SceneManager()
{
    gameLevel = UserDefault::getInstance()->getIntegerForKey("level", 0);
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

void SceneManager::loadingScene(Layer* scene)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    std::string name = FILE_FORMAT;
    name = FileUtils::getInstance()->fullPathForFilename(name.c_str());

    GifBase *gif = InstantGif::create(name.c_str());//InstantGif ：While playing, while parsing
    if(gif == NULL)
    {
        CCLOG("%s","create gif failed");
        return ;
    }
    gif->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    gif->setPosition(visibleSize.width/2 , visibleSize.height/2);
    //    gif->setScale(visibleSize.width/ gif->getContentSize().width, visibleSize.height/gif->getContentSize().height);
    scene->addChild(gif);
}

void SceneManager::saveLevel(int level)
{
    int oldLevel = UserDefault::getInstance()->getIntegerForKey("level", 0);
    if (level > oldLevel) {
        UserDefault::getInstance()->setIntegerForKey("level", level);
    }
}

void SceneManager::moveLogoScene()
{
    auto scene = LogoScene::createScene();
    this->setCurScene(scene);
    Director::getInstance()->replaceScene(scene);
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
