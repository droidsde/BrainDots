//
//  PencilPopupLayer.cpp
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/09.
//
//

#include "PencilPopupLayer.h"

USING_NS_CC_EXT;
using namespace cocos2d::extension;

PencilPopupLayer::PencilPopupLayer()
{
    
}

PencilPopupLayer::~PencilPopupLayer()
{
    CC_SAFE_RELEASE_NULL(itemArray);
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
    
    // title
    auto title = Text::create("Choose pencil", "arial.ttf", 80);
    title->setPosition(Vec2(sizeLayout.width/2, sizeLayout.height - PADDING));
    title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    title->setColor(Color3B::RED);
    layoutTable->addChild(title);
    
    // define variables
    this->swBox = Rect(0.01f * sizeLayout.width, 0.4f* sizeLayout.height, 0.98*sizeLayout.width, 0.33f*sizeLayout.height);
    this->swPosition = swBox.origin;
    this->swSize = swBox.size;
    this->slSize = Size(2.3f * sizeLayout.width, 0.33f * sizeLayout.height);
    this->disDistance = 0.2f * sizeLayout.width;
    this->disScale = 0.25f;
    
    // init data
    initData();
    
    // add item
    for (int i = 0; i < 10; i++) {
        auto item = Sprite::create("pencil1.png");
        addItem(item);
    }

    // touch screen
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->setEnabled(true);
    listener->onTouchesBegan = CC_CALLBACK_2(PencilPopupLayer::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(PencilPopupLayer::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(PencilPopupLayer::onTouchesEnded, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void PencilPopupLayer::initData()
{
    itemArray = Array::create();
    itemArray->retain();
    
    itemNum = 0;
    offsetPosition = Vec2(swSize.width/2, swSize.height/2);
    
    scrollLayer = Layer::create();
    scrollLayer->setAnchorPoint(Vec2::ZERO);
    scrollLayer->setPosition(Vec2::ZERO);
    scrollLayer->setContentSize(slSize);
    
    scrollView = cocos2d::extension::ScrollView::create(swSize, scrollLayer);
    scrollView->setContentSize(swSize);
    scrollView->setAnchorPoint(Vec2::ZERO);
    scrollView->setTouchEnabled(true);
    scrollView->setDirection(cocos2d::extension::ScrollView::Direction::HORIZONTAL);
    scrollView->setContentOffset(Vec2::ZERO);
    scrollView->setDelegate(this);
    addChild(scrollView,1);
}

void PencilPopupLayer::onExit()
{
    removeAllChildren();
    LayerColor::onExit();
}

bool PencilPopupLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *pEvent)
{
    return true;
}

void PencilPopupLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *pEvent)
{
    Vec2 scroll_prepoint = touch->getPreviousLocation();
    Vec2 scroll_movepoint = touch->getLocation();
    if(swBox.containsPoint(scroll_movepoint))
    {
        Vec2 adjustPoint = scroll_movepoint-scroll_prepoint;
        adjustScrollView(adjustPoint);
        adjustItemScale(adjustPoint);
    }
}

void PencilPopupLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *pEvent)
{
    Vec2 scroll_prepoint = touch->getPreviousLocation();
    Vec2 scroll_endpoint = touch->getLocation();
//    float disX = scroll_endpoint.x - scroll_endpoint.x;
    adjustEndScrollView();
}

void PencilPopupLayer::adjustItemScale(Vec2 adjustPoint)
{
//    float disX = adjustPoint.x;
    Ref* obj = NULL;
    CCARRAY_FOREACH(itemArray,obj)
    {
        Node* card = (Node*) obj;
        float offset = scrollView->getContentOffset().x;
        float posX = card->getPositionX() + offset;
        float disMid = abs(swSize.width/2-posX);
        float scale = 1- disMid/disDistance*disScale;
        card->setScale(scale);
        int zOr = (int) (1000-disMid*0.1);
        card->setZOrder(zOr);
    }
}

void PencilPopupLayer::adjustScrollView(Vec2 adjustPoint)
{
    Vec2 endPoint = scrollView->getContentOffset().add(Vec2(adjustPoint.x,0));
    scrollView->unscheduleAllSelectors();
    scrollView->setContentOffset(endPoint,false);
}

void PencilPopupLayer::adjustEndScrollView()
{
    Ref* obj = NULL;
    float minX = sizeLayout.height;
    float midX = swSize.width/2;
    //èŽ·å�–è·�ç¦»ä¸­é—´æœ€å°�å€¼çš„card
    CCARRAY_FOREACH(itemArray,obj)
    {
        Node* card = (Node*) obj;
        float offset = scrollView->getContentOffset().x;
        //è½¬åŒ–çˆ¶ç±»å��æ ‡
        float posX = card->getPositionX() + offset;
        float disMid = midX-posX;
        if(abs(disMid) < abs(minX)) minX = disMid;
    }
    
    CCARRAY_FOREACH(itemArray,obj)
    {
        Node* item = (Node*) obj;
        //è½¬åŒ–çˆ¶ç±»å��æ ‡
        float offset = scrollView->getContentOffset().x;
        float posX = item->getPositionX() + offset ;
        //è·�ç¦»ä¸­é—´é•¿åº¦
        float disMid = abs(midX - posX - minX);
        //ç›®æ ‡scale
        float scale = 1- disMid/disDistance*disScale;
        ScaleTo* scaleBy = ScaleTo::create(0.2f,scale);
        item->runAction(scaleBy);
        int zOr = (int) (1000-disMid*0.1);
        item->setZOrder(zOr);
    }
    Layer* scrollLayer = (Layer*)scrollView->getContainer();
    MoveBy* moveBy = MoveBy::create(0.2f,Vec2(minX,0));
    //CCCallFuncN* callFuncN = CCCallFuncN::create(this,callfuncN_selector(CoverView::cardViewEnd_callBack));
    //CCSequence* seq = CCSequence::create(moveBy,callFuncN,NULL);
    //scrollLayer->runAction(seq);
    scrollLayer->runAction(moveBy);
}

void PencilPopupLayer::itemViewEnd_callback(Ref* pSender)
{
    getCurItemIndex();
}

void PencilPopupLayer::scrollViewDidScroll(cocos2d::extension::ScrollView* view)
{
    
}

void PencilPopupLayer::scrollViewDidZoom(cocos2d::extension::ScrollView* view)
{
    
}



void PencilPopupLayer::addItem(Node * item)
{
    int zOrder = 1000 - itemNum;
    this->addItem(item, zOrder, 0);
}

void PencilPopupLayer::addItem(Node * item, int zOrder)
{
    this->addItem(item, zOrder,0);
}

void PencilPopupLayer::addItem(Node* item, int zOrder, int tag)
{
    float positionX = offsetPosition.x + disDistance*itemNum;
    float scale = 1 - disScale*itemNum;
    item->setPosition(Vec2(positionX,offsetPosition.y));
    item->setScale(scale);
    itemArray->addObject(item);
    scrollLayer->addChild(item , zOrder,tag);
    itemNum++;
    //CCLog("crad%d:[%f , %f]",cardNum,card->getPositionX(),card->getPositionY());
}

int PencilPopupLayer::getCurItemIndex()
{
    float distance1 = - scrollLayer->getPositionX();
    float distance2 = swSize.width/2 - offsetPosition.x;
    //+5 æµ®ç‚¹æ•°è¯¯å·®
    int index = (distance1 + distance2 + 5) / (disDistance);
    //CCLog("card index:%d  distance1:%f",index,distance1);
    return index;
}

void PencilPopupLayer::setOffsetPosition(Vec2 var)
{
    offsetPosition = var;
    Ref* obj = NULL;
    itemNum = 0;
    CCARRAY_FOREACH(itemArray,obj)
    {
        Node* card = (Node*) obj;
        float positionX = offsetPosition.x + disDistance*itemNum;
        card->setPosition(Vec2(positionX,offsetPosition.y));
        itemNum++;
    }
    adjustItemScale(Vec2::ZERO);
}

Vec2 PencilPopupLayer::getOffsetPosition()
{
    return offsetPosition;
}
