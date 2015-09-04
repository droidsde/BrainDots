//
//  GameOverLayer.cpp
//  GameTetris
//
//  Created by Nguyen Minh Hoang on 2015/03/26.
//
//

#include "GameOverLayer.h"

bool GameOverLayer::init()
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

void GameOverLayer::addLayer()
{
    // load score
    curScore = ManageScene::getInstance()->getCurScore();
    scoreText = StringUtils::format("score%d", ManageScene::getInstance()->getSpeedLevel());
    oldHighScore = UserDefault::getInstance()->getIntegerForKey(scoreText.c_str(), 0);
    
    this->setMS(ManageSound::getInstance());
    this->ignoreAnchorPointForPosition(false);
    this->setPosition(Point(originPoint.x + visibleSize.width / 2, originPoint.y + visibleSize.height / 2));
    
    // add root node
    Node *rootNode = CSLoader::createNode("GameOverLayer.csb");
    rootNode->setPosition(Vec2(originPoint.x + visibleSize.width / 2, originPoint.y + visibleSize.height / 2));
    rootNode->setAnchorPoint(Vec2(0.5, 0.5));
    rootNode->setName("rootNode");
    this->addChild(rootNode);
    
    this->setPositionY(-(originPoint.y + visibleSize.height / 2));
    
    auto eventListener = EventListenerTouchOneByOne::create();
    eventListener->setSwallowTouches(true);
    eventListener->onTouchBegan = CC_CALLBACK_2(GameOverLayer::onTouchBegan, this);
    eventListener->onTouchCancelled = CC_CALLBACK_2(GameOverLayer::onTouchCancelled, this);
    eventListener->onTouchEnded = CC_CALLBACK_2(GameOverLayer::onTouchEnded, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, this);
    
    auto restartButton = dynamic_cast<ui::Button*> (rootNode->getChildByName("restartButton"));
    if (restartButton)
    {
        restartButton->addClickEventListener(CC_CALLBACK_1(GameOverLayer::restart, this));
    }
    
    auto quitButton = dynamic_cast<ui::Button*>(rootNode->getChildByName("quitButton"));
    if (quitButton) {
        quitButton->addClickEventListener(CC_CALLBACK_1(GameOverLayer::quitGame, this));
    }
    
    // text score
    textScore = dynamic_cast<Text*>(rootNode->getChildByName("scoreText"));
    if (textScore)
    {
//        scoreText->setString(String::createWithFormat("%d", curScore)->getCString());
        textScore->setString("0");
    }
    
    auto winText = dynamic_cast<Text*>(rootNode->getChildByName("winText"));
    auto inputText = dynamic_cast<Text*>(rootNode->getChildByName("inputText"));
    auto inputName = dynamic_cast<TextField*>(rootNode->getChildByName("inputName"));
    
    // show input name when got highscore
    if (curScore > oldHighScore)
    {
        if (winText)
        {
            winText->setString(StringUtils::format("HighScore"));
        }
        if (inputName && inputText)
        {
            inputText->setVisible(true);
            inputName->setVisible(true);
        }
    }
    else
    {
        if (inputName && inputText)
        {
            inputText->setVisible(false);
            inputName->setVisible(false);
        }
    }
}

bool GameOverLayer::onTouchBegan(Touch* touch, Event* event)
{
    return true;
}

void GameOverLayer::onTouchEnded(Touch* touch, Event* event)
{
    
}

void GameOverLayer::onTouchCancelled(Touch* touch, Event* event)
{
    
}

void GameOverLayer::addScore()
{
    this->schedule(CC_SCHEDULE_SELECTOR(GameOverLayer::runningScore), 0.1f);
}

void GameOverLayer::addParticle()
{
    auto particle = ParticleSystemQuad::create("ringWon.plist");
    particle->setPosition(textScore->getPosition());
    addChild(particle);
}


void GameOverLayer::runningScore(float dt)
{
    countScore += (curScore/SCORE_ADDING);
    if (countScore > curScore)
    {
        unschedule(CC_SCHEDULE_SELECTOR(GameOverLayer::runningScore));
//        addParticle();
    }
    else
    {
        textScore->setString(String::createWithFormat("%d", countScore)->getCString());
    }
}

void GameOverLayer::onEnter()
{
    Layer::onEnter();
    auto action = Sequence::create(MoveTo::create(0.5f, Point(originPoint.x + visibleSize.width / 2,originPoint.y + visibleSize.height / 2)), CallFunc::create(CC_CALLBACK_0(GameOverLayer::addScore, this)) ,nullptr);
    this->runAction(action);
}

void GameOverLayer::onExit()
{
    
    Layer::onExit();
    
}

bool GameOverLayer::saveScore()
{
    std::string name = "";
    auto rootNode = this->getChildByName("rootNode");
    for (auto node : rootNode->getChildren())
    {
        if (node->getName() == "inputName")
        {
            auto inputName = dynamic_cast<TextField*>(node);
            name = inputName->getString();
        }
    }
    
    if (name.compare("")==0)
    {
        return false;
    }
    else
    {
        UserDefault::getInstance()->setIntegerForKey(scoreText.c_str(), curScore);
        std::string name_ = StringUtils::format("name%d", ManageScene::getInstance()->getSpeedLevel());
        UserDefault::getInstance()->setStringForKey( name_.c_str(), name);
        return true;
    }
}

void GameOverLayer::restart(Ref* sender)
{
    Director::getInstance()->getOpenGLView()->setIMEKeyboardState(false);
    getMS()->playClickSound();
    if (curScore > oldHighScore)
    {
        if (!saveScore())
        {
            return;
        }
    }
    
    ManageScene::getInstance()->setSpeedLevel(0);
    ManageScene::getInstance()->setCurScore(0);
    auto action = Spawn::create(MoveTo::create(0.3f, Point(-(originPoint.x + visibleSize.width) , (originPoint.y + visibleSize.height / 2))), FadeOut::create(0.3f),nullptr);
    auto set = CallFunc::create(CC_CALLBACK_0(GameOverLayer::setVisible, this, false));
    auto remove = CallFunc::create(CC_CALLBACK_0(GameOverLayer::removeFromParent, this));
    auto go = CallFunc::create(CC_CALLBACK_0(ManageScene::changeState, ManageScene::getInstance(), GAME_STATE::GAME));
    auto action_ = Sequence::create(action, set, remove, go, nullptr);
    this->runAction(action_);
}

void GameOverLayer::quitGame(cocos2d::Ref *sender)
{
    Director::getInstance()->getOpenGLView()->setIMEKeyboardState(false);
    getMS()->playClickSound();
    if (curScore > oldHighScore)
    {
        if (!saveScore())
        {
            return;
        }
    }
    ManageScene::getInstance()->setSpeedLevel(0);
    ManageScene::getInstance()->setCurScore(0);
    auto go = CallFunc::create(CC_CALLBACK_0(ManageScene::changeState, ManageScene::getInstance(), GAME_STATE::MENU));
    this->runAction(go);
    ManageSound::getInstance()->pauseBackgroundMusic();
}
