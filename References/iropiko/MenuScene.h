//
//  MenuScene.h
//  PoolExample
//
//  Created by Nguyen Minh Hoang on 2015/07/28.
//
//

#ifndef __PoolExample__MenuScene__
#define __PoolExample__MenuScene__

#include <cocos2d.h>
#include <ui/CocosGUI.h>
#include "CCLabelBMFontAnimated.h"
#include "ManageScene.h"
#include "cocos-ext.h"
#include "cocosbuilder/CocosBuilder.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

class MenuScene: public LayerColor
{
    
public:
    
    static Scene* createScene();
    
    CREATE_FUNC(MenuScene);
    
    virtual bool init();
    
    virtual void onExit() override;
    
private:
    
    void addWords();
    
    void addChooseLanguages();
    
    void addSettings();
    
    void moveAudioManagerScene();
    
    void nextScene();
    
    void moveSetting();
    
    void moveMainMenu();
    
    void setupSettings();
    
    cocos2d::Size visibleSize;
    
    Vec2 origin;
    
    CCLabelBMFontAnimated* label;
    
    void drawSelected(Vec2 position);
    
    DrawNode* draw;
    
    cocos2d::ui::Button* flagJp, *flagEn;
    
    cocos2d::ui::Layout* mainLayout, *settingLayout;
    
    void valueChanged(cocos2d::Ref* sender, cocos2d::extension::Control::EventType controlEvent);
    void touchButtonEvent(Ref* sender, Widget::TouchEventType type);
    ControlSwitch *scSound;
    ControlSwitch *scNumberName;
    ControlSwitch *scNumber;
};

#endif /* defined(__PoolExample__MenuScene__) */
