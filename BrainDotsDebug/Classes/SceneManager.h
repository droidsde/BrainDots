//
//  SceneManager.h
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/03.
//
//

#ifndef __BrainDotsDebug__SceneManager__
#define __BrainDotsDebug__SceneManager__

#include <cocos2d.h>
#include "Constants.h"
#include "LogoScene.h"
#include "MenuScene/MenuScene.h"
#include "GameScene.h"

#include "SoundManager/SoundManager.h"
#include "ExecuteShapePhysic.h"

#include "PaperSprite.h"
// lib gif
#include "Gif/GIFMovie.h"
#include "Gif/CacheGif.h"
#include "Gif/InstantGif.h"

USING_NS_CC;

class SceneManager
{
public:
    
    SceneManager();
    ~SceneManager();
    
    static SceneManager* getInstance();
    
    CC_SYNTHESIZE(cocos2d::Scene*, curScene, CurScene);
    CC_SYNTHESIZE(GAME_STATE, curState, CurState);
    CC_SYNTHESIZE(int, gameLevel, LevelGame);
    
    void changeState(GAME_STATE = GAME_STATE::LOGO);
    void loadingScene(Layer*);
    void saveLevel(int level);
    
    bool checkFileExist(std::string filename);
private:
  
    static SceneManager* _sceneManager;
    
    void nextScene(cocos2d::Scene*);
    
    void moveLogoScene();
    
    void moveSplashScreen();
    
    void moveMenuScene();
    
    void moveSettingLayer();
    
    void moveGameScene();
    
    void moveGameOverScene();
    
    void moveHighScore();
};

#endif /* defined(__BrainDotsDebug__SceneManager__) */
