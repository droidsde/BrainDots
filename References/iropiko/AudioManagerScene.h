//
//  AudioManagerScene.h
//  PoolExample
//
//  Created by Nguyen Minh Hoang on 2015/08/05.
//
//

#ifndef __PoolExample__AudioManagerScene__
#define __PoolExample__AudioManagerScene__

#include <cocos2d.h>
#include <ui/CocosGUI.h>
#include <NativeUtils.h>
#include <extensions/cocos-ext.h>
#include "SimpleAudioEngine.h"
#include "ManageScene.h"
#include "CCLabelBMFontAnimated.h"
#include "PopupLayer.h"
#include "ModelDialogLayer.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace extension;

class AudioManagerScene : public LayerColor
{

public:
    static Scene* createScene();
    
    virtual bool init();
    
    virtual void onEnter() override;
    virtual void onExit() override;
    
    CREATE_FUNC(AudioManagerScene);
    
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    void onTouchCancelled(Touch* touch, Event* event);
    
private:
    
    cocos2d::Size visibleSize;
    Vec2 origin;
    Sprite* board;
    cocos2d::Size sizeBoard;
    Layout* chooseLayout, *listLayout, *recordLayout;
    CocosDenshion::SimpleAudioEngine* audio;
    std::string filename;
    CCLabelBMFontAnimated* animationText;
    
    int countdown = TIME_RECORDING;
    LANGUAGE_TYPE typeLang;
    State_SM curState = State_SM::CHOOSE;
    std::string numberName[SUMCOLOR];
    std::string fileRemove;
    
    ListView* listview;
    
    void initChooseLayout();
    void initListLayout();
    void initRecordLayout();
    void timeExecute(float dt);
    void reloadData();
    
    void touchButtonEvent(Ref* sender, Widget::TouchEventType type);
    void moveLayout(Ref* sender1, Ref* sender2, int direction);
    void openCloseRecordLayout(cocos2d::Ref *sender, int direction);
    void prepareDataLanguage(LANGUAGE_TYPE type);
};


#endif /* defined(__PoolExample__AudioManagerScene__) */
