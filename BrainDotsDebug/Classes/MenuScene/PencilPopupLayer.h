//
//  PencilPopupLayer.h
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/09.
//
//

#ifndef __BrainDotsDebug__PencilPopupLayer__
#define __BrainDotsDebug__PencilPopupLayer__

#include <cocos2d.h>
#include "BasePopupLayer.h"
#include <extensions/cocos-ext.h>

USING_NS_CC;

using namespace cocos2d::ui;
using namespace cocos2d::extension;

class PencilPopupLayer : public BasePopupLayer, public ScrollViewDelegate
{
public:
    
    PencilPopupLayer();
    ~PencilPopupLayer();
    static PencilPopupLayer* create();
    virtual bool init();
    void initData();

    virtual void onExit();
    
    virtual bool onTouchBegan(Touch* touch, Event* pEvent);
    virtual void onTouchMoved(Touch* touch, Event* pEvent);
    virtual void onTouchEnded(Touch* touch, Event* pEvent);
    
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view);
    virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view);
    void adjustItemScale(Vec2 adjustPoint);
    void adjustScrollView(Vec2 adjustPoint);
    void adjustEndScrollView();
    void itemViewEnd_callback(Ref* pSender);
    
    int getCurItemIndex();  // index of current item
    void addItem(Node* item);
    void addItem(Node* item, int zOrder);
    void addItem(Node* item, int zOrder, int tag);
    
    CC_SYNTHESIZE(Vec2, swPosition, SwPosition); // scrollview position
    CC_SYNTHESIZE(Size, swSize, SwSize);    // scrollview size
    CC_SYNTHESIZE(Size, slSize, SlSize);    // scrollLayer size
    CC_SYNTHESIZE(float, disDistance, DisDistance); // distance item
    CC_SYNTHESIZE(float, disScale, DisScale);       // scale item
    CC_SYNTHESIZE(Rect, swBox, SwBox);              //
    CC_SYNTHESIZE(Layer*, scrollLayer, scrollLayer); // layer container
    CC_SYNTHESIZE(int, itemNum, ItemNum);           // item index
    CC_PROPERTY(Vec2, offsetPosition, OffsetPosition); // position offset
    
private:
    Size sizeLayout;
    Array* itemArray;
    cocos2d::extension::ScrollView* scrollView;
};

#endif /* defined(__BrainDotsDebug__PencilPopupLayer__) */
