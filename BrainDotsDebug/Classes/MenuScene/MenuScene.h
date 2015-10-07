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
#include "ManageSoundDelegate.h"
#include "PencilPopupLayer.h"
#include "RecordPopupLayer.h"
#include "SharePopupLayer.h"
#include "MenuPopupLayer.h"

USING_NS_CC;
using namespace ui;

class BasePopupLayer;
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
    Size bodySize, headerSize;
    Layer *headerLayer, *bodyLayer;
    ListView* listview;
    PageView* pageview;
    Size stickerSize, miniStickerSize, miniTickSize;
    int curPage = 1; // 1->10
    int curLevel; // 0 -> (6 * PAGE)-1
    
    void drawGrids();
    
    void addHeaderLayer();
    
    void addBodyLayer();
    
    void addListHorizontal();
    
    void addPageView();
    
    void addButtons();
    
    void startGame();

    void reloadData();
    
    void capturingScreen(std::string filename);
    void afterCaptured(bool succeed, const std::string& outputFile);

    void touchButtonEvent(Ref* sender, Widget::TouchEventType type);
    
    void pageViewEvent(Ref* pSender, PageView::EventType type);
    
    void selectedItemEvent(Ref *pSender, ListView::EventType type);
    void scrollEvent(Ref* ref, ui::ScrollView::EventType EventType);
    
    void openStage(int i);
    
    void showLevelsInStage(int stage);
    void showStages(int stage);
};

#endif /* defined(__BrainDotsDebug__MenuScene__) */
