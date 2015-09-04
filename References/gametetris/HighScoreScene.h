//
//  HighScoreScene.h
//  GameTetris
//
//  Created by Nguyen Minh Hoang on 2015/04/02.
//
//

#ifndef __GameTetris__HighScoreScene__
#define __GameTetris__HighScoreScene__

#include <cocos2d.h>
#include <string>
#include <extensions/cocos-ext.h>
#include "Constants.h"
#include "ManageScene.h"
#include "ManageSound.h"

USING_NS_CC;
using namespace extension;

class HighScoreScene: public Layer, public TableViewDataSource, public TableViewDelegate
{
public:
    cocos2d::Size winSize;
    
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    // TableViewDataSource method
    virtual cocos2d::Size cellSizeForTable(TableView* table);
    virtual TableViewCell* tableCellAtIndex(TableView* table,ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView* table);
    
    // TableViewDelegate method
    virtual void tableCellTouched(TableView* table,TableViewCell* cell);
    
    // TableViewDelegate for scrollView
    virtual void scrollViewDidScroll(extension::ScrollView* view){};
    virtual void scrollViewDidZoom(extension::ScrollView* view){};
    
    CREATE_FUNC(HighScoreScene);

private:
    
    Sprite* board;
    
    cocos2d::Size sizeBoard;
    
    std::vector<UserData> listUserData;
    
    void closeScene(Ref* sender);
    
    CC_SYNTHESIZE(ManageSoundDelegate*, msDelegator, MS);
    
};
#endif /* defined(__GameTetris__HighScoreScene__) */
