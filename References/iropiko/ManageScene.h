//
//  ManageScene.h
//  PoolExample
//
//  Created by Nguyen Minh Hoang on 2015/08/07.
//
//

#ifndef __PoolExample__ManageScene__
#define __PoolExample__ManageScene__

#include <cocos2d.h>
#include "Constanst.h"
#include "SplashScreen.h"
#include "MenuScene.h"
#include "HelloWorldScene.h"
#include "AudioManagerScene.h"
#include "cocos-ext.h"

using namespace rapidjson; // library that we use for json parsing

USING_NS_CC;

class ManageScene
{
public:
    ManageScene();
    ~ManageScene();
    static ManageScene* getInstance();
    Document document;

    void changeState(GAME_STATE = GAME_STATE::LOGO);
    void loadResouceJson(std::string jsonFile);
    std::string getStringForKey(std::string key);
    static std::string getString(std::string key);
    
    CC_SYNTHESIZE(Scene*, curScene, CurScene);
    CC_SYNTHESIZE(GAME_STATE, curState, CurState);
    CC_SYNTHESIZE(std::string, curLanguage, CurrentLanguage); // current language using
    CC_SYNTHESIZE(bool, showNameNumber, ShowNameNumber);
    CC_SYNTHESIZE(bool, showNumber, ShowNumber);
    CC_SYNTHESIZE(bool, playSound, PLaySound);
    
    bool checkFileSoundExist(std::string filename);
    
    std::string getFileNameFollowPlatform(std::string filename);
private:
    static ManageScene* _manageScene;
    
    void nextScene(Scene*);
    void moveSplashScene();
    void moveMenuScene();
    void moveManageRecord();
    void moveGameScene();
};

#endif /* defined(__PoolExample__ManageScene__) */
