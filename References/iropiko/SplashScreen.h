//
//  SplashScreen.h
//  PoolExample
//
//  Created by Nguyen Minh Hoang on 2015/07/27.
//
//

#ifndef __PoolExample__SplashScreen__
#define __PoolExample__SplashScreen__

#include <cocos2d.h>
#include "ManageScene.h"
#include "CCLabelBMFontAnimated.h"

USING_NS_CC;

class SplashScreen: public LayerColor
{
public:
    static Scene* createScene();
    
    CREATE_FUNC(SplashScreen);
    
    virtual bool init();
    
    virtual void onExit() override;
private:
    
    cocos2d::Size visibleSize;
    
    Vec2 origin;
    
    CCLabelBMFontAnimated* label;
    
    void addLogo();
    
    void addWords();
    
    void nextScene();
    
};
#endif /* defined(__PoolExample__SplashScreen__) */
