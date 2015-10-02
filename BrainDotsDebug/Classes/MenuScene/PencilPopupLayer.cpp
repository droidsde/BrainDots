//
//  PencilPopupLayer.cpp
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/09.
//
//

#include "PencilPopupLayer.h"

PencilPopupLayer::PencilPopupLayer()
{
    
}

PencilPopupLayer::~PencilPopupLayer()
{
//    CC_SAFE_RELEASE_NULL(itemArray);
}

PencilPopupLayer* PencilPopupLayer::create()
{
    PencilPopupLayer* layer = new PencilPopupLayer();
    layer->init();
    layer->autorelease();
    return layer;
}

bool PencilPopupLayer::init()
{
    BasePopupLayer::init();
    sizeLayout = layoutTable->getContentSize();
    sizeListView = Size(sizeLayout.width *4/5, sizeLayout.height*2 / 3);
    
    // title
    auto title = Text::create("Choose pencil", "arial.ttf", 60);
    title->setPosition(Vec2(sizeLayout.width/2, sizeLayout.height - PADDING));
    title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    title->setColor(Color3B::RED);
    layoutTable->addChild(title);
    
    // add listview horizontal
    listViewPencils = ListView::create();
    listViewPencils->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    listViewPencils->setGravity(cocos2d::ui::ListView::Gravity::CENTER_HORIZONTAL);
    listViewPencils->setTouchEnabled(true);
    listViewPencils->setBounceEnabled(true);
    listViewPencils->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    listViewPencils->setPosition(Vec2(title->getPositionX(), sizeLayout.height/2));
    listViewPencils->setContentSize(sizeListView);
    listViewPencils->setInertiaScrollEnabled(true);
    listViewPencils->setItemsMargin(LIST_SMALL_ITEM_MARGIN);
    listViewPencils->addEventListener(CC_CALLBACK_2(PencilPopupLayer::selectedItemEvent, this));
    listViewPencils->addEventListener(CC_CALLBACK_2(PencilPopupLayer::scrollEvent, this));

    layoutTable->addChild(listViewPencils);
    
    // reload data
    reloadData();
    
    // button
    Button* buyButton = Button::create("lang_normal.png");
    buyButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    buyButton->setPosition(Vec2(sizeLayout.width/3, PADDING_MENU_HEADER_ITEM));
    buyButton->setTitleText("Buy pencil");
    buyButton->setTitleColor(Color3B::WHITE);
    buyButton->setTitleFontSize(40);
    buyButton->setTitleFontName("arial.ttf");
    layoutTable->addChild(buyButton);
    
    Button* useButton = Button::create("lang_selected.png");
    useButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    useButton->setPosition(Vec2(sizeLayout.width*2/3, PADDING_MENU_HEADER_ITEM));
    useButton->setTitleText("Use pencil");
    useButton->setTitleColor(Color3B::WHITE);
    useButton->setTitleFontSize(40);
    useButton->setTitleFontName("arial.ttf");
    layoutTable->addChild(useButton);
    
    return true;
}

void PencilPopupLayer::reloadData()
{
    listViewPencils->removeAllItems();
    listPencils.clear();
    
    for (int i = 0; i < PENCIL_MAX; i++) {
        // container
        Layout* layout = Layout::create();
        auto pencil = Button::create("pencil1.png");
        sizePencil = pencil->getContentSize();
        
        if ( i == 0 || i == PENCIL_MAX-1) {
            layout->setContentSize(Size(sizeListView.width/2 + sizePencil.width/2, sizeListView.height));
            if (i == 0) {
                pencil->setPosition(Vec2(layout->getContentSize().width - sizePencil.width/2, sizeListView.height/2));
            } else if ( i == PENCIL_MAX-1) {
                pencil->setPosition(Vec2(sizePencil.width/2, sizeListView.height/2));
            }
        } else {
            layout->setContentSize(Size(pencil->getContentSize().width, sizeListView.height));
            pencil->setPosition(layout->getContentSize()/2);
        }
        
        // pencil sprite
        pencil->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        pencil->setTitleText(StringUtils::format("%d",i+1));
        pencil->setTitleColor(Color3B::BLACK);
        pencil->setTitleFontSize(40);
        pencil->setTitleFontName("arial.ttf");
        layout->addChild(pencil);
        
        listPencils.pushBack(pencil);
        // insert listview
        listViewPencils->insertCustomItem(layout, i);
    }
    listViewPencils->refreshView();
}

void PencilPopupLayer::selectedItemEvent(cocos2d::Ref *pSender, ListView::EventType type)
{
    ListView* list = static_cast<ListView*>(pSender);
    
    switch (type) {
        case ListView::EventType::ON_SELECTED_ITEM_START :
            break;
            
        case ListView::EventType::ON_SELECTED_ITEM_END :
        {
            float curPos = list->getInnerContainer()->getPositionX();
            float exactPos = -(sizePencil.width + LIST_SMALL_ITEM_MARGIN) * ((int) list->getCurSelectedIndex());
            CCLOG("curPos %f exactPos %f", curPos, exactPos);
            if (curPos > (exactPos + DELTA_TRANSLATE) || curPos < (exactPos - DELTA_TRANSLATE)) {
                auto moveAndScale = Sequence::create(MoveTo::create(0.3f, Vec2(exactPos, 0)), CallFunc::create(CC_CALLBACK_0(PencilPopupLayer::scrollEvent, this, listViewPencils, ui::ScrollView::EventType::SCROLLING)),NULL);
                list->getInnerContainer()->runAction(moveAndScale);
            }
            break;
        }
            
        default:
            break;
    }
}

void PencilPopupLayer::scrollEvent(cocos2d::Ref *pSender, ui::ScrollView::EventType type)
{
    ListView* list = static_cast<ListView*>(pSender);
    
    if ( type == ui::ScrollView::EventType::SCROLLING || type == ui::ScrollView::EventType::BOUNCE_LEFT || type == ui::ScrollView::EventType::BOUNCE_RIGHT)
    {
//        CCLOG("SCROLLING");
        curPosX = list->getInnerContainer()->getPositionX();
        // scale item
        this->scaleItems(curPosX);
        
    }
//    else if ( type == ui::ScrollView::EventType::BOUNCE_LEFT) {
//        CCLOG("BOUNCE_LEFT %f", list->getInnerContainer()->getPositionX());
//    } else if ( type == ui::ScrollView::EventType::BOUNCE_RIGHT) {
//        CCLOG("BOUNCE_RIGHT %f", list->getInnerContainer()->getPositionX());
//    }
}

void PencilPopupLayer::scaleItems(float curPosX)
{

    int index = -1;
    float scale = MIN_SCALE_PENCIL;
    
    for (int i = 0 ; i < PENCIL_MAX; i++) {
        
        float startX = -(sizePencil.width + LIST_SMALL_ITEM_MARGIN) * i + sizePencil.width;
        float endX = -(sizePencil.width + LIST_SMALL_ITEM_MARGIN) * i - sizePencil.width;
        float centerX = (startX + endX) / 2;
        
        if (curPosX <= startX && curPosX >= endX) {
            if (curPosX > (centerX + DELTA_TRANSLATE)) {
                scale = MAX_SCALE_PENCIL - (curPosX-centerX) / sizePencil.width;
            } else if ( curPosX < (centerX - DELTA_TRANSLATE)) {
                scale = MAX_SCALE_PENCIL - (centerX - curPosX) / sizePencil.width;
            } else  if (curPosX <=(centerX+DELTA_TRANSLATE) && curPosX >=(curPosX-DELTA_TRANSLATE)){
                scale = MAX_SCALE_PENCIL;
            }
            
            index = i;
            break;
        }
    }

    if (index == -1) return;
    
    for(int i = 0; i< listPencils.size(); i++) {
        auto layout = (Button*)listPencils.at(i);
        if (i == (index)) {
            layout->setScale(scale);
        } else {
            layout->setScale(MIN_SCALE_PENCIL);
        }
    }
}