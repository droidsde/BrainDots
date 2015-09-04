//
//  GameScene.h
//  GameTetris
//
//  Created by Nguyen Minh Hoang on 2015/03/19.
//
//

#ifndef __GameTetris__GameScene__
#define __GameTetris__GameScene__

#include <cocos2d.h>
#include "GameBoard.h"
#include "HubLayer.h"
#include "PauseLayer.h"

USING_NS_CC;

class GameScene :public Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    CREATE_FUNC(GameScene);
    
private:
    
    Vec2 originalPoint;
    
    cocos2d::Size  visibleSize;
    
    void addLayers();
    
    void addGameBoard();
    
    void animationBeforeStart();
    
    std::vector<Node*> starVector;
    
};

#endif /* defined(__GameTetris__GameScene__) */
