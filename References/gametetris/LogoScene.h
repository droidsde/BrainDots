//
//  LogoScene.h
//  GameTetris
//
//  Created by Nguyen Minh Hoang on 2015/03/19.
//
//

#ifndef __GameTetris__LogoScene__
#define __GameTetris__LogoScene__

#include <cocos2d.h>
#include "ManageScene.h"
#include "ManageSound.h"

class LogoScene: public Layer
{
public:
    static Scene* createScene();
    
    CREATE_FUNC(LogoScene);
    
    virtual bool init();
    
private:
    
    void addLogo();
    
    void loadResources();
    
    void loadSounds();
    
    void loadImage(Texture2D* texture);
    
    void startGame();
    
    void nextScene();
    
    virtual void onExit() override;
};

#endif /* defined(__GameTetris__LogoScene__) */
