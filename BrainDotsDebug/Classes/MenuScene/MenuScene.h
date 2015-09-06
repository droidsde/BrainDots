//
//  MenuScene.h
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/04.
//
//

#ifndef __BrainDotsDebug__MenuScene__
#define __BrainDotsDebug__MenuScene__

#include <cocos2d.h>
#include "SceneManager.h"
#include "ui/CocosGUI.h"
#include "SoundManager/SoundManager.h"

USING_NS_CC;
using namespace ui;

class MenuScene : public LayerColor
{
public:
    static Scene* createScene();
    
    CREATE_FUNC(MenuScene);
    
    virtual bool init() override;
    virtual void onEnter();
    virtual void onExit();
    
    CC_SYNTHESIZE(ManageSoundDelegate*, msDelegator, SoundMusic);
    
private:
    
    Vec2 origin;
    Size visibleSize;
    ListView* listview;
    Size stickerSize;
    
    void addMenu();
    
    void addListHorizontal();
    
    void addPageView();
    
    void addButtons();
    
    void startGame();

    void reloadData();

    void touchButtonEvent(Ref* sender, Widget::TouchEventType type);
};

#endif /* defined(__BrainDotsDebug__MenuScene__) */
