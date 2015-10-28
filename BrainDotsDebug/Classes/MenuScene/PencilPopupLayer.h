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
#include "SceneManager.h"

USING_NS_CC;

using namespace cocos2d::ui;

class PencilPopupLayer : public BasePopupLayer
{
public:
    
    PencilPopupLayer();
    ~PencilPopupLayer();
    static PencilPopupLayer* create();
    virtual bool init();
    void reloadData();
    
private:
    Size sizeLayout;
    Size sizeListView;
    Size sizePencil;
    float widthLayoutPencil;
    Text* curTextCoin;

    ListView* listViewPencils;
    Vector<Button*> listPencils;
    
    // scale item
    void scaleItems(float curPosX);
    
    // focus item
    void autoFocusItemList(bool isSelectedPencil);
    
    // scroll to index
    void scrollToItem(int index);
    
    // select pencil
    void savePencil(int index);
    
    void selectedItemEvent(Ref *pSender, ListView::EventType type);
    void scrollEvent(Ref* pSender, ui::ScrollView::EventType type);
    void touchButtonEvent(Ref* pSender, Widget::TouchEventType type);
};

#endif /* defined(__BrainDotsDebug__PencilPopupLayer__) */
