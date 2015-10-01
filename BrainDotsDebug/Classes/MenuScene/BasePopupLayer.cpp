//
//  BasePopupLayer.cpp
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/09.
//
//

#include "BasePopupLayer.h"

BasePopupLayer* BasePopupLayer::create()
{
    BasePopupLayer* layer = new BasePopupLayer();
    layer->init();
    layer->autorelease();
    return layer;
}

BasePopupLayer::BasePopupLayer()
{
    layoutTable = nullptr;
}

BasePopupLayer::~BasePopupLayer()
{

}

bool BasePopupLayer::init()
{
    if (!LayerColor::initWithColor(Color4B(128, 128, 128, 100))) {
        return false;
    }
    this->setSoundManage(SoundManager::getInstance());
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    layoutTable = Layout::create();
    layoutTable->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    layoutTable->setBackGroundColor(Color3B::WHITE);
    layoutTable->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    layoutTable->setContentSize(Size(visibleSize.width*4/5, visibleSize.height*4/5));
    addChild(layoutTable);
    
    // set position
    posOrigin = Vec2(visibleSize.width/2 - layoutTable->getContentSize().width/2 + PADDING_MENU_HEADER_ITEM, visibleSize.height + PADDING);
    posNew = Vec2(visibleSize.width/2 - layoutTable->getContentSize().width/2, visibleSize.height/2 - layoutTable->getContentSize().height/2);
    layoutTable->setRotation(-10);
    layoutTable->setPosition(posOrigin);
    
    // button exit
    Button* exit = Button::create("close_button.png");
    exit->setPosition(Vec2(layoutTable->getContentSize().width -  PADDING_MENU_HEADER_ITEM, layoutTable->getContentSize().height - PADDING_MENU_HEADER_ITEM));
    exit->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    exit->setTouchEnabled(true);
    exit->addTouchEventListener([this](Ref* pSender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED) {
            this->closeLayer();
        }
    });
    layoutTable->addChild(exit);
    
    auto eventListener = EventListenerTouchOneByOne::create();
    eventListener->setSwallowTouches(true);
    eventListener->onTouchBegan = CC_CALLBACK_2(BasePopupLayer::onTouchBegan, this);
    eventListener->onTouchCancelled = CC_CALLBACK_2(BasePopupLayer::onTouchCancelled, this);
    eventListener->onTouchEnded = CC_CALLBACK_2(BasePopupLayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);
    return true;
}

void BasePopupLayer::onEnter()
{
    LayerColor::onEnter();
    auto rotate = RotateTo::create(0.3, 0);
    auto move = MoveTo::create(0.3, posNew);
    auto spawn = Spawn::create(move, rotate, NULL);
    layoutTable->runAction(spawn);
}

void BasePopupLayer::closeLayer()
{
    auto rotate = RotateTo::create(0.3, -10);
    auto move = MoveTo::create(0.3, posOrigin);
    auto spawn = Spawn::create(rotate, move, NULL);
    auto exit = CallFunc::create([=] {
        layoutTable->runAction(spawn);
    });
    auto set = CallFunc::create(CC_CALLBACK_0(BasePopupLayer::setVisible, this, false));
    auto remove = CallFunc::create(CC_CALLBACK_0(BasePopupLayer::removeFromParent, this));
    
    this->runAction(Sequence::create(exit, DelayTime::create(0.3f), set, remove, RemoveSelf::create(), NULL));
}

cocos2d::ui::Button* BasePopupLayer::createButton(std::string fileName, int tag, Vec2 anchorPoint)
{
    auto button = Button::create(fileName);
    button->setTag(tag);
    button->setTouchEnabled(true);
    button->setAnchorPoint(anchorPoint);
    return button;
}

void BasePopupLayer::moveOut(Ref* pSender)
{
    layoutTable->runAction(MoveBy::create(0.3f, Vec2(-PADDING_MENU_HEADER_ITEM, -PADDING_MENU_HEADER_ITEM)));
}

void BasePopupLayer::moveIn(cocos2d::Ref *pSender)
{
    layoutTable->runAction(MoveBy::create(0.3f, Vec2(PADDING_MENU_HEADER_ITEM, PADDING_MENU_HEADER_ITEM)));
}

void BasePopupLayer::moveOrigin(Ref* pSender)
{
    layoutTable->runAction(MoveTo::create(0.3f, posNew));
}

bool BasePopupLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    return true;
}

void BasePopupLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    
}

void BasePopupLayer::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event)
{
    
}