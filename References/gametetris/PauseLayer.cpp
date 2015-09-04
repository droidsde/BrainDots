//
//  PauseLayer.cpp
//  GameTetris
//
//  Created by Nguyen Minh Hoang on 2015/04/20.
//
//

#include "PauseLayer.h"

bool PauseLayer::init()
{
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 150)))
    {
        return false;
    }
    
    originPoint = Director::getInstance()->getVisibleOrigin();
    visibleSize = Director::getInstance()->getVisibleSize();
    
    addLayer();
    
    return true;
}


void PauseLayer::pauseLayerCallback()
{
    auto action = Spawn::create(MoveTo::create(0.1f, Point(originPoint.x + visibleSize.width / 2, originPoint.y + visibleSize.height / 2)), NULL);
    this->runAction(action);
}

void PauseLayer::addLayer()
{
    this->setMS(ManageSound::getInstance());
    this->ignoreAnchorPointForPosition(false);
    this->setPosition(Point(originPoint.x + visibleSize.width / 2, originPoint.y + visibleSize.height / 2));
    
    // add root node
    Node* rootNode = CSLoader::createNode("PauseLayer.csb");
    rootNode->setPosition(Point(originPoint.x + visibleSize.width / 2, originPoint.y + visibleSize.height / 2));
    rootNode->setAnchorPoint(Vec2(0.5, 0.5));
    rootNode->setName("rootName");
    this->addChild(rootNode);
    
    this->setPositionX(originPoint.x + 2 * visibleSize.width);
    
    // button restart
    auto restartButton = dynamic_cast<Button*>(rootNode->getChildByName("restartButton"));
    if (restartButton)
    {
        restartButton->addClickEventListener(CC_CALLBACK_1(PauseLayer::restartGame, this));
    }
    
    // button quit
    auto quitButton = dynamic_cast<Button*>(rootNode->getChildByName("quitButton"));
    if (quitButton)
    {
        quitButton->addClickEventListener(CC_CALLBACK_1(PauseLayer::quitGame, this));
    }
    
    // button resume
    auto resumeButton = dynamic_cast<Button*>(rootNode->getChildByName("resumeButton"));
    if (resumeButton)
    {
        resumeButton->addClickEventListener(CC_CALLBACK_1(PauseLayer::resumeGame, this));
    }
}

void PauseLayer::resumeGame(cocos2d::Ref *sender)
{
    getMS()->playClickSound();
    auto action = Spawn::create(MoveTo::create(0.1f, Point((originPoint.x + 2 * visibleSize.width) , (originPoint.y + visibleSize.height / 2))),nullptr);
    auto action_ = Sequence::create(action, nullptr);
    this->runAction(action_);
    
    // callback gameboard
    getGameBoardCallback()->resumeGame(sender);
}

void PauseLayer::restartGame(cocos2d::Ref *sender)
{
    getMS()->playClickSound();
    ManageScene::getInstance()->setSpeedLevel(0);
    ManageScene::getInstance()->setCurScore(0);
    auto action = Spawn::create(MoveTo::create(0.3f, Point(-(originPoint.x + visibleSize.width) , (originPoint.y + visibleSize.height / 2))), FadeOut::create(0.3f),nullptr);
    auto set = CallFunc::create(CC_CALLBACK_0(PauseLayer::setVisible, this, false));
    auto remove = CallFunc::create(CC_CALLBACK_0(PauseLayer::removeFromParent, this));
    auto go = CallFunc::create(CC_CALLBACK_0(ManageScene::changeState, ManageScene::getInstance(), GAME_STATE::GAME));
    auto action_ = Sequence::create(action, set, remove, go, nullptr);
    this->runAction(action_);
}

void PauseLayer::quitGame(cocos2d::Ref *sender)
{
    getMS()->playClickSound();
    ManageScene::getInstance()->setSpeedLevel(0);
    ManageScene::getInstance()->setCurScore(0);
    auto go = CallFunc::create(CC_CALLBACK_0(ManageScene::changeState, ManageScene::getInstance(), GAME_STATE::MENU));
    this->runAction(go);
    ManageSound::getInstance()->pauseBackgroundMusic();
}
