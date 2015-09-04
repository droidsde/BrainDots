//
//  ManageScene.h
//  GameTetris
//
//  Created by Nguyen Minh Hoang on 2015/03/19.
//
//

#ifndef __GameTetris__ManageScene__
#define __GameTetris__ManageScene__

#include <cocos2d.h>
#include "Constants.h"
#include "ManageScene.h"
#include "LogoScene.h"
#include "MenuScene.h"
#include "GameScene.h"
#include "GameOverLayer.h"
#include "HighScoreScene.h"

USING_NS_CC;

class ManageScene
{
public:
    ManageScene();
    
    ~ManageScene();
    
    static ManageScene* getInstance();
    
    CC_SYNTHESIZE(cocos2d::Scene*, curScene, CurScene);
    
    CC_SYNTHESIZE(int, curSpeed, SpeedLevel);
    
    CC_SYNTHESIZE(int, curScore, CurScore);
    
    CC_SYNTHESIZE(GAME_STATE, curState, CurState);
    
    void changeState(GAME_STATE = GAME_STATE::LOGO);
    
private:
    
    static ManageScene* _manageScene;
    
    void nextScene(cocos2d::Scene*);
    
    void jumpToLogoScene();
    
    void jumpToMenuScene();
    
    void jumpToGameScene();
    
    void jumpToGameOverScene();
    
    void jumpToHighScore();

};

#endif /* defined(__GameTetris__ManageScene__) */
