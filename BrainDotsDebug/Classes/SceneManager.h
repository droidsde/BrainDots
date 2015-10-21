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

#include "json/document.h"

USING_NS_CC;

class SceneManager
{
public:
    
    SceneManager();
    ~SceneManager();
    rapidjson::Document document;
    
    static SceneManager* getInstance();
    
    CC_SYNTHESIZE(cocos2d::Scene*, curScene, CurScene);
    CC_SYNTHESIZE(GAME_STATE, curState, CurState);
    CC_SYNTHESIZE(int, gameLevel, LevelGame);
    CC_SYNTHESIZE(int, curMaxLevel, CurMaxLevel);
    CC_SYNTHESIZE(std::string, curLanguage, CurLanguage);
    CC_SYNTHESIZE(int, curUnlockPencil, CurUnlockPencil);
    CC_SYNTHESIZE(int, curUsePencil, CurUsePencil);
    CC_SYNTHESIZE(int, curCoin, CurCoin);
    
    void changeState(GAME_STATE = GAME_STATE::LOGO);
    void loadingScene(Layer*);
    void saveLevel(int level);
    void saveLanguage();
    void saveUnlockPencil(int index);
    void saveUsePencil(int pencil);
    void saveCoin(int coin);
    
    // load list language
    void loadListLanguage(std::string jsonFile);
    std::string getStringForKey(std::string key);
    int getIntForKey(std::string key);
    std::vector<std::string> getListData(std::string root, std::string key);
    std::map<std::string, std::string> getMapData(std::string root, std::string key, std::string value);
    // load current language
    void loadCurrentLanguage(std::string jsonFile);
    
    bool checkFileExist(std::string filename);
private:
  
    static SceneManager* _sceneManager;
    
    GifBase* gif;
    
    void nextScene(cocos2d::Scene*);
    
    void moveLogoScene();
    
    void moveSplashScreen();
    
    void moveMenuScene();
    
    void moveSettingLayer();
    
    void moveGameScene();
    
    void moveGameOverScene();
    
};

#endif /* defined(__BrainDotsDebug__SceneManager__) */
