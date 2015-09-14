//
//  BaseCoverFlow.cpp
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/14.
//
//

#include "BaseCoverFlow.h"

BaseCoverFlow::BaseCoverFlow()
{
    
}

BaseCoverFlow::~BaseCoverFlow()
{
    
}

BaseCoverFlow* BaseCoverFlow::create(cocos2d::Size size, int total)
{
    BaseCoverFlow* cover = new BaseCoverFlow();
    if (cover && cover->init(size, total)) {
        cover->autorelease();
        return cover;
    }
    CC_SAFE_DELETE(cover);
    return NULL;
}

bool BaseCoverFlow::init(cocos2d::Size size, int total)
{
    if (!Layer::init()) {
        return false;
    }
    this->mSize = size;
    this->mTotal = total;
    
    // add table view
    tableView = TableView::create(this, Size(mSize.width*0.9, 128));
    tableView->setDirection(TableView::Direction::HORIZONTAL);
    tableView->setDelegate(this);
    tableView->setBounceable(true);
    tableView->reloadData();
    tableView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    tableView->setPosition(Vec2(mSize.width*.05, mSize.height/2 - 64));
    tableView->setContentOffset(Vec2(mSize.width*0.9*0.5 - 64, 0));
    addChild(tableView);
    
    // add touch screen
    auto dispatcher = EventListenerTouchOneByOne::create();
    dispatcher->onTouchBegan = CC_CALLBACK_2(BaseCoverFlow::onTouchBegan, this);
    dispatcher->onTouchMoved = CC_CALLBACK_2(BaseCoverFlow::onTouchMoved, this);
    dispatcher->onTouchEnded = CC_CALLBACK_2(BaseCoverFlow::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(dispatcher, this);
    
    return true;
}

Size BaseCoverFlow::cellSizeForTable(cocos2d::extension::TableView *table)
{
    return Size(mSize.width*0.9 / 5, 128);
}

ssize_t BaseCoverFlow::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
    return mTotal;
}

void BaseCoverFlow::tableCellTouched(cocos2d::extension::TableView *table, cocos2d::extension::TableViewCell *cell)
{
    CCLOG("%zi cell touched %f %f contentoffset", cell->getIdx(), table->getContentOffset().x, table->getContentOffset().y);
}

TableViewCell* BaseCoverFlow::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
    auto string = StringUtils::format("%zd", idx);
    TableViewCell* cell = table->dequeueCell();
    cell = new TableViewCell();
    cell->autorelease();
    
    auto sprite = Sprite::create("pencil1.png");
    sprite->setPosition(Vec2(mSize.width*0.9/10, 64));
    sprite->setAnchorPoint(Vec2(0.5, 0.5));
    cell->addChild(sprite);
    
    auto text = Label::createWithTTF(string, "arial.ttf", 80);
    text->setPosition(sprite->getPosition());
    text->setColor(Color3B::BLACK);
    cell->addChild(text);
    
    CCLOG("add cell %zi", idx);
    return cell;
}

bool BaseCoverFlow::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    return true;
}

void BaseCoverFlow::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    Vec2 scroll_prepoint = touch->getPreviousLocation();
    Vec2 scroll_movepoint = touch->getLocation();
    if(tableView->getBoundingBox().containsPoint(scroll_movepoint))
    {
        Vec2 adjustPoint = scroll_movepoint-scroll_prepoint;
//        adjustItemScale(adjustPoint);
    }
}

void BaseCoverFlow::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
//    adjustEndScrollView();
}

void BaseCoverFlow::adjustItemScale(Vec2 adjustPoint)
{
    float disDistance = mSize.width * 0.9 * 0.2;
    for (int i=0; i<mTotal; i++) {
        TableViewCell* cell = (TableViewCell*)tableView->getChildByTag(i+100);
        float offset = tableView->getContentOffset().x;
        float posX = cell->getPositionX() + offset;
        float disMid = abs(mSize.width*0.9/2-posX);
        float scale = 1- disMid/disDistance*0.25;
        cell->setScale(scale);
        int zOr = (int) (1000-disMid*0.1);
        cell->setZOrder(zOr);
    }
}