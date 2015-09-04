//
//  HubLayer.cpp
//  GameTetris
//
//  Created by Nguyen Minh Hoang on 2015/03/23.
//
//

#include "HubLayer.h"


HubLayer::HubLayer()
{
    lineNumber = 0;
    comboNumber = 0;
}

HubLayer::~HubLayer()
{
    removeAllChildrenWithCleanup(true);
    removeFromParentAndCleanup(true);
}

bool HubLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    // add layer hub
    addLayerHub();
    
    return true;
}

void HubLayer::onEnter()
{
    Layer::onEnter();
    
    // set up touch layer
    setUpTouchEvent();
}

void HubLayer::addLayerHub()
{
    Point originPoint = Director::getInstance()->getVisibleOrigin();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Size winSize = Director::getInstance()->getWinSize();
    
    this->ignoreAnchorPointForPosition(false);
    this->setPosition(Point(originPoint.x + visibleSize.width / 2, originPoint.y + visibleSize.height / 2));
    
    // big node
    std::string hub_name = "";
    if (checkScreenSize()) {
        hub_name = "HubLayer_ipad.csb";
    }
    else
        hub_name = "HubLayer.csb";
    Node* rootNode = CSLoader::createNode(hub_name);
    rootNode->setAnchorPoint(Vec2(0.5, 0.5));
    rootNode->setPosition(winSize.width / 2, winSize.height / 2);
    this->addChild(rootNode);
    rootNode->setName("rootNode");
    
    // add line
    currentLine = dynamic_cast<Text*>(rootNode->getChildByName("textLine"));
//    currentLine->setTouchEnabled(true);
//    currentLine->setTouchScaleChangeEnabled(true);
    
    // add score
    currentScore = dynamic_cast<Text*>(rootNode->getChildByName("textScore"));
    
    // add combo
    currentCombo = dynamic_cast<Text*>(rootNode->getChildByName("textCombo"));
    
    // add speed
    currentSpeed = dynamic_cast<Text*>(rootNode->getChildByName("textSpeed"));
    currentSpeed->setString(String::createWithFormat("%d", ManageScene::getInstance()->getSpeedLevel())->getCString());
    
    // add button pause resume
    pauseButton = dynamic_cast<CheckBox*>(rootNode->getChildByName("pauseButton"));
    if (pauseButton) {
        pauseButton->addEventListener(CC_CALLBACK_2(HubLayer::pauseCallback, this));
    }
    
    // add button restart game
    auto restartButton = dynamic_cast<Button*>(rootNode->getChildByName("restartButton"));
    if (restartButton) {
        restartButton->addClickEventListener(CC_CALLBACK_1(HubLayer::restartCallback, this));
    }
    
    // add button hold tetromino
    holdButton = dynamic_cast<Button*>(rootNode->getChildByName("holdButton"));
    if (holdButton) {
        holdButton->addClickEventListener(CC_CALLBACK_1(HubLayer::onHold, this));
    }
}

void HubLayer::addLineHub(int line)
{
    int combo = line - 1;
    int temp = lineNumber;
    lineNumber += line;
    
    if (temp % COUNT_LINE_TO_ADD_SPEED > lineNumber % COUNT_LINE_TO_ADD_SPEED)
    {
        this->addSpeedHub();
    }
    
    currentLine->setString(String::createWithFormat("%d", lineNumber)->getCString());
    animationAddScore(currentLine);
    
    Vector<FiniteTimeAction*> vector_action;
    for (int i = 0; i < combo; i++)
    {
        auto action = Sequence::create(CallFunc::create(CC_CALLBACK_0(HubLayer::addComboHub, this)), DelayTime::create(0.1 / line),nullptr);
        vector_action.pushBack(action);
    }
    
    for (int i = 0; i < line * line; i++)
    {
        auto action = Sequence::create(CallFunc::create(CC_CALLBACK_0(HubLayer::addScoreHub, this)), DelayTime::create(0.1 / (line * line)) , nullptr);
        vector_action.pushBack(action);
    }
 
    auto action = Sequence::create(vector_action);
    this->runAction(action);
}

void HubLayer::addComboHub()
{
    comboNumber += 1;
    currentCombo->setString(String::createWithFormat("%d", comboNumber)->getCString());
    animationAddScore(currentCombo);
}

void HubLayer::addScoreHub()
{
    ManageSound::getInstance()->playAddScoreSound();
    int score = ManageScene::getInstance()->getCurScore();
    score += SCORE_ADDING;
    ManageScene::getInstance()->setCurScore(score);
    
    currentScore->setString(String::createWithFormat("%d", score)->getCString());
    animationAddScore(currentScore);
}

void HubLayer::addSpeedHub()
{
    int speed = ManageScene::getInstance()->getSpeedLevel() + 1;
    ManageScene::getInstance()->setSpeedLevel(speed);
    currentSpeed->setString(String::createWithFormat("%d", speed)->getCString());
    animationAddScore(currentSpeed);
}

void HubLayer::animationAddScore(cocos2d::ui::Text *text)
{
    auto scaleBig = ScaleTo::create(0.3, 1.1);
    auto scaleSmalle = ScaleTo::create(0.2, 1);
    
    auto sequence = Sequence::create(scaleBig, scaleSmalle, nullptr);
    
    text->runAction(sequence);
}

void HubLayer::pauseCallback(cocos2d::Ref *pSender, CheckBox::EventType type)
{
    auto rootNode = this->getChildByName("rootNode");
    switch (type) {
        case CheckBox::EventType::SELECTED :
            this->getMyTouchDelegate()->onPause();
            Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(this);
            for(auto node : rootNode->getChildren())
            {
                if (node->getName() == "pauseButton")
                {
                    dynamic_cast<CheckBox*>(node)->setEnabled(true);
                    continue;
                }
                else if (dynamic_cast<Button*>(node))
                {
                    dynamic_cast<Button*>(node)->setEnabled(true);
                }
            }
            break;
        case CheckBox::EventType::UNSELECTED :
            this->getMyTouchDelegate()->onPause();
            Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(this);
            for(auto node : rootNode->getChildren())
            {
                if (node->getName() == "pauseButton")
                {
                    dynamic_cast<CheckBox*>(node)->setEnabled(false);
                    continue;
                }
                else if (dynamic_cast<Button*>(node))
                {
                    dynamic_cast<Button*>(node)->setEnabled(false);
                }
            }
            break;
        default:
            break;
    }
}

void HubLayer::restartCallback(cocos2d::Ref *pSender)
{
    ManageScene::getInstance()->setSpeedLevel(0);
    ManageScene::getInstance()->setCurScore(0);
    auto go = CallFunc::create(CC_CALLBACK_0(ManageScene::changeState, ManageScene::getInstance(), GAME_STATE::GAME));
    this->runAction(go);
}

void HubLayer::setUpTouchEvent()
{
    // set up touch event listener
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();

    // touch
    listener->onTouchBegan = CC_CALLBACK_2(HubLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(HubLayer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(HubLayer::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(HubLayer::onTouchCancelled, this);
    
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HubLayer::removeTouch()
{
    // remove touch
    Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(this);
    
    // remove touch button
    auto rootNode = this->getChildByName("rootNode");
    for(auto node : rootNode->getChildren())
    {
        if (node->getName() == "pauseButton")
        {
            dynamic_cast<CheckBox*>(node)->setEnabled(false);
        }
        else if (dynamic_cast<Button*>(node))
        {
            dynamic_cast<Button*>(node)->setEnabled(false);
        }
    }

}

bool HubLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    isTouchRotation = true;
    firstTouch = touch->getLocation();
    currentTouch = touch->getLocation();
    previousTouch = touch->getLocation();
    oldStepX = 0;
    oldStepY = 0;
    return true;
}

void HubLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    currentTouch = touch->getLocation();
    previousTouch = touch->getPreviousLocation();
    int  spaceX = abs((int)(currentTouch.x - firstTouch.x));
    int  spaceY = abs((int)(currentTouch.y - firstTouch.y));
    
    // CASE ROTATE
    if (spaceX >= 10 || spaceY >= 10)
    {
        isTouchRotation = false;
    }
    
    // CASE HARD DROP
    if (spaceY >= BLOCK_WIDTH && currentTouch.y > previousTouch.y && spaceX < BLOCK_WIDTH)
    {
        if (!isHardDrop && !isMoving && !isDowning)
        {
            this->getMyTouchDelegate()->moveHardDrop();
            isHardDrop = true;
        }
        
    }
    
    // CASE MOVE DOWN
    else if (currentTouch.y < previousTouch.y && spaceY >= BLOCK_WIDTH && spaceX < 2 * BLOCK_WIDTH)
    {
        if (spaceY >= BOARD_HEIGHT * BLOCK_WIDTH)
        {
            spaceY = BLOCK_HEIGHT * BLOCK_WIDTH;
        }
        
        int step = spaceY / (2*BLOCK_WIDTH);
        int tempStep = 0;
        if (oldStepY > step) {
            tempStep = oldStepY - step;
        }
        else if (step > oldStepY)
        {
            tempStep = step - oldStepY;
        }
        
        if (tempStep > 0 && !isHardDrop && !isMoving)
        {
            isDowning = true;
            this->getMyTouchDelegate()->moveDown(tempStep);
        }
        oldStepY = step;
    }
    
    // CASE MOVE LEFT, RIGHT
    else if (spaceX >= BLOCK_WIDTH
//             && spaceY <= 2*BLOCK_HEIGHT
             )
    {
        if (spaceX > BOARD_WIDTH * BLOCK_WIDTH)
        {
            spaceX = BOARD_WIDTH * BLOCK_WIDTH;
        }
        int step = spaceX / BLOCK_WIDTH;
        int tempStep = 0;
        if (oldStepX > step) {
            tempStep = oldStepX - step;
        }
        else if (step > oldStepX)
        {
            tempStep = step - oldStepX;
        }
        if (tempStep >= 0 && !isHardDrop)
        {
            isMoving = true;
            if (currentTouch.x > previousTouch.x)
            {
                this->getMyTouchDelegate()->moveRight(tempStep);
            }
            else
            {
                this->getMyTouchDelegate()->moveLeft(tempStep);
            }
        }
       
        oldStepX = step;
    }
}

void HubLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if (isTouchRotation)
    {
        this->getMyTouchDelegate()->moveRotate();
    }
    firstTouch = Vec2::ZERO;
    currentTouch = Vec2::ZERO;
    previousTouch = Vec2::ZERO;
    isHardDrop = false;
    isMoving = false;
    isDowning = false;
    oldStepX = 0;
    oldStepY = 0;
}

void HubLayer::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event)
{
    cocos2d::log("touch cancelled");
    onTouchEnded(touch, event);
}

void HubLayer::onHold(cocos2d::Ref *sender)
{
    if (holdButton->isEnabled())
    {
        this->getMyTouchDelegate()->onHold();
        if (holdButton)
        {
            holdButton->setEnabled(false);
        }
    }
}


void HubLayer::onLongRotate(float time)
{
    this->getMyTouchDelegate()->moveRotate();
}

bool HubLayer::checkScreenSize()
{
    Size framSize = Director::getInstance()->getOpenGLView()->getFrameSize();
    // size of ipad
    if (framSize.height / framSize.width < 1.5)
    {
        return true;
    }
    else
        return false;
}

void HubLayer::onHoldCallback()
{
    if (holdButton)
    {
        if (!holdButton->isEnabled())
        {
            holdButton->setEnabled(true);
        }
    }
}

void HubLayer::onPauseCallback()
{
    if (pauseButton)
    {
        pauseButton->setSelected(false);
        Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(this);
        auto rootNode = this->getChildByName("rootNode");
        for(auto node : rootNode->getChildren())
        {
            if (node->getName() == "pauseButton")
            {
                dynamic_cast<CheckBox*>(node)->setEnabled(false);
                continue;
            }
            else if (dynamic_cast<Button*>(node))
            {
                dynamic_cast<Button*>(node)->setEnabled(false);
            }
        }

    }
}

void HubLayer::onResumeCallback()
{
    if (pauseButton)
    {
        pauseButton->setSelected(true);
        Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(this);
        auto rootNode = this->getChildByName("rootNode");
        for(auto node : rootNode->getChildren())
        {
            if (node->getName() == "pauseButton")
            {
                dynamic_cast<CheckBox*>(node)->setEnabled(true);
                continue;
            }
            else if (dynamic_cast<Button*>(node))
            {
                dynamic_cast<Button*>(node)->setEnabled(true);
            }
        }
        
    }
}