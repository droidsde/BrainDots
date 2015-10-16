//
//  NotiPopupLayer.cpp
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/10/16.
//
//

#include "NotiPopupLayer.h"

NotiPopupLayer::NotiPopupLayer()
{
    
}

NotiPopupLayer::~NotiPopupLayer()
{
    
}

NotiPopupLayer* NotiPopupLayer::create()
{
    NotiPopupLayer* layer = new NotiPopupLayer();
    layer->init();
    layer->autorelease();
    return layer;
}

bool NotiPopupLayer::init()
{
    BasePopupLayer::init();
    
    layoutSize = layoutTable->getContentSize();
    
    // title
    auto title = Text::create("Notification", "arial.ttf", 60);
    title->setPosition(Vec2(layoutSize.width/2, layoutSize.height - PADDING));
    title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    title->setColor(Color3B::ORANGE);
    layoutTable->addChild(title);
    
    float maxHeight = layoutSize.height - title->getContentSize().height - 3*PADDING;
    std::string fileName = "notification.png";
    // load image
    if (!SceneManager::getInstance()->checkFileExist(fileName)) {
        ClientRequest* cl = new ClientRequest();
        cl->sendRequest("http://placeimg.com/640/480/any", fileName);
    }
    
    auto infoImg = Sprite::create(fileName);
    if (infoImg != NULL) {
        layoutTable->addChild(infoImg);
        infoImg->setPosition(Vec2(layoutSize.width/2, PADDING));
        infoImg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        infoImg->setScaleY(maxHeight / infoImg->getContentSize().height);
    }
    
    return true;
}

void NotiPopupLayer::onEnter()
{
    BasePopupLayer::onEnter();
    
}

void NotiPopupLayer::onExit()
{
    BasePopupLayer::onExit();
    NotificationCenter::getInstance()->postNotification(EXIT_LAYER_NOTI);
}