//
//  BaseCoverFlow.h
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/14.
//
//

#ifndef __BrainDotsDebug__BaseCoverFlow__
#define __BrainDotsDebug__BaseCoverFlow__

#include <cocos2d.h>
#include <extensions/cocos-ext.h>

USING_NS_CC;
USING_NS_CC_EXT;

class BaseCoverFlow : public Layer, public TableViewDataSource, public TableViewDelegate
{
public:
    
    BaseCoverFlow();
    ~BaseCoverFlow();
    
    static BaseCoverFlow* create(Size size, int total);
    virtual bool init(Size size, int total);
    
    // table view data source
    virtual Size cellSizeForTable(TableView* table);
    virtual TableViewCell* tableCellAtIndex(TableView* table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView* table);
    
    // tableview delegate
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    
    // tableviewdelegate and scrollview
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view){};
    virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view){};
    
    // touch screen
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);
    
    void adjustItemScale(Vec2 adjustPoint);
//    void adjustEndScrollView();
    
private:
    Size mSize;
    int mTotal;
    
    TableView* tableView;
    
    float tempX;
    
    std::vector<Sprite*> listPencil;
};
#endif /* defined(__BrainDotsDebug__BaseCoverFlow__) */
