//
//  AudioManagerScene.cpp
//  PoolExample
//
//  Created by Nguyen Minh Hoang on 2015/08/05.
//
//

#include "AudioManagerScene.h"

USING_NS_CC;
using namespace cocos2d::ui;

Scene* AudioManagerScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = AudioManagerScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool AudioManagerScene::init()
{
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 255))) {
        return false;
    }

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    audio = CocosDenshion::SimpleAudioEngine::getInstance();
    
    // button close or back
    auto close = Button::create("back.png");
    close->setTouchEnabled(true);
    close->setAnchorPoint(Vec2(0, 1));
    close->setPosition(Vec2(10, origin.y + visibleSize.height - 10));
    this->addChild(close, 50);
    close->setTag(Tag::T_CLOSESETTINGBUTTON);
    close->addTouchEventListener(CC_CALLBACK_2(AudioManagerScene::touchButtonEvent, this));
    
    // init layouts
    initChooseLayout();
    initListLayout();
    initRecordLayout();
    
    return true;
}

void AudioManagerScene::onEnter()
{
    LayerColor::onEnter();
    chooseLayout->setVisible(false);
    
    // create text
    animationText = CCLabelBMFontAnimated::createWithTTF("レコーディング", "fonts/keifont.ttf", 80.0f, Size(visibleSize.width, 40), TextHAlignment::CENTER, TextVAlignment::CENTER);
    animationText->setTextColor(Color4B::WHITE);
    animationText->setAnchorPoint(Vec2::ZERO);
    animationText->setPosition(Vec2(visibleSize.width/2 - animationText->getContentSize().width/2, visibleSize.height / 2));
    addChild(animationText);
    animationText->setAllCharsScale(0);

    // icon
    auto recordIcon = Sprite::create("record_image.png");
    recordIcon->setPosition(Vec2(100, visibleSize.height + 100));
    recordIcon->setScale(0.5);
    recordIcon->setAnchorPoint(Vec2(0.5, 0.5));
    addChild(recordIcon);
    auto dropIn = MoveTo::create(2.0f, Vec2(100, visibleSize.height/2));
    auto spawn = Spawn::create(RotateBy::create(2.0f, 360), MoveBy::create(2.0f, Vec2(visibleSize.width, 0)), CallFunc::create(CC_CALLBACK_0(CCLabelBMFontAnimated::animateInTypewriter, animationText, 1.5f)), NULL);
    auto sequence = Sequence::create(EaseBounceOut::create(dropIn), spawn, DelayTime::create(0.5f), RemoveSelf::create(), CallFunc::create(CC_CALLBACK_0(Node::removeFromParent, animationText)), CallFunc::create(CC_CALLBACK_0(Node::setVisible, chooseLayout, true)), NULL);
    recordIcon->runAction(sequence);
}

void AudioManagerScene::prepareDataLanguage(LANGUAGE_TYPE type)
{
    switch (type) {
        case  LANGUAGE_TYPE::JA :
            ManageScene::getInstance()->loadResouceJson("ja.json");
            break;
            
        case  LANGUAGE_TYPE::EN :
            ManageScene::getInstance()->loadResouceJson("en.json");
            break;
        default:
            break;
    }
    for (int i=0; i<SUMCOLOR; i++) {
        std::string number = "number_" + to_string(i+1);
        numberName[i] = ManageScene::getInstance()->getStringForKey(number).c_str();
    }
}

void AudioManagerScene::initChooseLayout()
{
    // setup layout
    chooseLayout = Layout::create();
    chooseLayout->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    chooseLayout->setBackGroundColor(Color3B::BLACK);
    chooseLayout->setSize(visibleSize);
    chooseLayout->setPosition(Vec2(0,0));
    this->addChild(chooseLayout);

    // title
    auto title = Text::create("言語選択", "fonts/keifont.ttf", 100);
    title->enableOutline(Color4B::RED, 5);
    title->setColor(Color3B::YELLOW);
    title->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height*3/4));
    chooseLayout->addChild(title);
    
    // button in layout
    auto jpButton = Button::create("flag_jp.png");
    jpButton->setTouchEnabled(true);
    jpButton->setTag(Tag::T_JPBUTTON);
    jpButton->setAnchorPoint(Vec2(0.5, 0));
    jpButton->addTouchEventListener(CC_CALLBACK_2(AudioManagerScene::touchButtonEvent, this));
    chooseLayout->addChild(jpButton);
    
    auto enButton = Button::create("flag_en.png");
    enButton->setTouchEnabled(true);
    enButton->setTag(Tag::T_ENBUTTON);
    enButton->setAnchorPoint(Vec2(0.5, 1));
    enButton->addTouchEventListener(CC_CALLBACK_2(AudioManagerScene::touchButtonEvent, this));
    chooseLayout->addChild(enButton);
    
    jpButton->setPosition(Vec2(origin.x + visibleSize.width/2 , origin.y + visibleSize.height/3 + 60));
    enButton->setPosition(Vec2(origin.x + visibleSize.width/2 , origin.y + visibleSize.height/3 - 20));

}

void AudioManagerScene::initListLayout()
{
    // setup layout
    listLayout = Layout::create();
    listLayout->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    listLayout->setBackGroundColor(Color3B::BLACK);
    listLayout->setSize(visibleSize);
    listLayout->setPosition(Vec2(visibleSize.width,0));
    this->addChild(listLayout);
    
    // title
    auto title = Text::create("レコード", "fonts/keifont.ttf", 80);
    title->enableOutline(Color4B::RED, 5);
    title->setColor(Color3B::YELLOW);
    title->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    float pY = getChildByTag(Tag::T_CLOSESETTINGBUTTON)->getPositionY() - getChildByTag(Tag::T_CLOSESETTINGBUTTON)->getContentSize().height/2;
    title->setPosition(Vec2(origin.x + visibleSize.width/2, pY));
    listLayout->addChild(title);
    
    // add table
    // board
    board = Sprite::create("list_board.png");
    sizeBoard = board->getContentSize();
    
    // button reset
    auto resetButton = Button::create("button_reset.png");
    resetButton->setPosition(Vec2(origin.x + visibleSize.width - 10, title->getPositionY()));
    resetButton->setAnchorPoint(Vec2(1 , 0.5));
    resetButton->setTouchEnabled(true);
    resetButton->setTag(Tag::T_RESETBUTTON);
    resetButton->addTouchEventListener(CC_CALLBACK_2(AudioManagerScene::touchButtonEvent, this));
    listLayout->addChild(resetButton);
    
    // listview
    listview = ListView::create();
    listview->setTouchEnabled(true);
    listview->setBackGroundImage("list_board.png");
    listview->setSize(sizeBoard);
    listview->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    listview->setPosition(Vec2(origin.x + visibleSize.width / 2, title->getPositionY() - title->getContentSize().height));
    listview->setBounceEnabled(true);
    listLayout->addChild(listview);
    reloadData();
}

void AudioManagerScene::reloadData()
{
    listview->removeAllItems();
    for (int i=0; i<SUMCOLOR; i++) {
        std::string id = StringUtils::format("%d", i+1);
        std::string name = StringUtils::format("%s", numberName[i].c_str());
        
        Layout* layout = Layout::create();
        layout->setSize(Size(sizeBoard.width, sizeBoard.height/8));
        auto record = Button::create("button_origin.png","button_press.png", "");
        record->setAnchorPoint(Vec2(0, 0.5));
        record->setTag((int)(Tag_ListItem::T_LISTITEM_OPEN) + (int)i);
        record->setPosition(Vec2(20, layout->getContentSize().height/2));
        record->addTouchEventListener(CC_CALLBACK_2(AudioManagerScene::touchButtonEvent, this));
        layout->addChild(record);
        
        // id text
        auto *idText = LabelTTF::create(id.c_str(), "fonts/keifont.ttf", 40);
        idText->setColor(Color3B::WHITE);
        idText->setAnchorPoint(Vec2(0, 0.5));
        idText->setPosition(Vec2(20, record->getContentSize().height/2));
        record->addChild(idText);
        
        // name text
        auto *nameText = LabelTTF::create(name.c_str(), "fonts/keifont.ttf", 40);
        nameText->setColor(Color3B::WHITE);
        nameText->setAnchorPoint(Vec2(0.5, 0.5));
        nameText->setPosition(Vec2(record->getContentSize().width/2, idText->getPositionY()));
        record->addChild(nameText);
        
        Button* remove = Button::create("button_remove.png");
        remove->setAnchorPoint(Vec2(1, 0.5));
        remove->setPosition(Vec2(sizeBoard.width - 20, sizeBoard.height*.125/2));
        remove->addTouchEventListener(CC_CALLBACK_2(AudioManagerScene::touchButtonEvent, this));
        
        std::string number = to_string(i+1);
        std::string tempname;
        // create filename sound
        tempname = "number_" + number + "_" + ManageScene::getInstance()->getCurrentLanguage();
        tempname = ManageScene::getInstance()->getFileNameFollowPlatform(tempname);
        
        // check sound exist
        if (ManageScene::getInstance()->checkFileSoundExist(tempname.c_str())) {
            layout->addChild(remove);
            remove->setTag((int)(Tag_ListItem::T_LISTITEM_REMOVE) + (int)i);
        }
        
        listview->insertCustomItem(layout, i);
    }
}

void AudioManagerScene::initRecordLayout()
{
    // setup layout
    recordLayout = Layout::create();
    recordLayout->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    recordLayout->setBackGroundColor(Color3B::GRAY);
    recordLayout->setOpacity(200);
    recordLayout->setSize(visibleSize);
    recordLayout->setPosition(Vec2(0 , -visibleSize.height));
    this->addChild(recordLayout);
    
    // number
    auto number = Label::createWithTTF("", "fonts/keifont.ttf", 150);
    number->setColor(Color3B::RED);
    number->setPosition(Vec2(origin.x + visibleSize.width/2, origin.x + visibleSize.height*7/8));
    number->setVerticalAlignment(TextVAlignment::CENTER);
    number->setHorizontalAlignment(TextHAlignment::CENTER);
    number->setTag(Tag::T_RECORD_NUMBER);
    recordLayout->addChild(number);
    
    // namenumber
    auto numberName = Label::createWithTTF("three", "fonts/keifont.ttf", 150);
    numberName->setColor(Color3B::RED);
    numberName->setPosition(Vec2(origin.x + visibleSize.width/2, origin.x + visibleSize.height*3/4));
    numberName->setVerticalAlignment(TextVAlignment::CENTER);
    numberName->setHorizontalAlignment(TextHAlignment::CENTER);
    numberName->setTag(Tag::T_RECORD_NAME);
    recordLayout->addChild(numberName);
    
    // button play
    auto buttonPlay = Button::create("play_button.png");
    buttonPlay->setAnchorPoint(Vec2(0.5, 0.5));
    buttonPlay->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
    buttonPlay->setTouchEnabled(true);
    buttonPlay->setTag(Tag::T_RECORD_PLAY);
    buttonPlay->addTouchEventListener(CC_CALLBACK_2(AudioManagerScene::touchButtonEvent, this));
    recordLayout->addChild(buttonPlay);
    
    // button record
    auto buttonRecord = Button::create("button_record.png");
    buttonRecord->setAnchorPoint(Vec2(0.5, 0.5));
    buttonRecord->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/6));
    buttonRecord->setTouchEnabled(true);
    buttonRecord->setTag(Tag::T_RECORD_RECORDING);
    buttonRecord->addTouchEventListener(CC_CALLBACK_2(AudioManagerScene::touchButtonEvent, this));
    recordLayout->addChild(buttonRecord);
    auto animZoom = Sequence::create(ScaleTo::create(0.2, 0.9), EaseElasticOut::create(ScaleTo::create(1.0, 1.0f)), DelayTime::create(0.7f), NULL);
    buttonRecord->runAction(RepeatForever::create(animZoom));
    
    // number count down
    auto countNumber = Label::createWithTTF(to_string(TIME_RECORDING), "fonts/keifont.ttf", 150);
    countNumber->setColor(Color3B::WHITE);
    countNumber->setPosition(Vec2(buttonRecord->getPosition()));
    countNumber->setVerticalAlignment(TextVAlignment::CENTER);
    countNumber->setHorizontalAlignment(TextHAlignment::CENTER);
    countNumber->setTag(Tag::T_RECORD_COUNTDOWN);
    countNumber->setScale(0);
    recordLayout->addChild(countNumber);
}

void AudioManagerScene::moveLayout(cocos2d::Ref *sender1, cocos2d::Ref *sender2, int direction)
{
    auto layout1 = (Layout*) sender1;
    auto layout2 = (Layout*) sender2;
    auto moveBy1 = MoveBy::create(TIME_TRANSITION_SCENE, Vec2(direction * visibleSize.width, 0));
    auto moveBy2 = moveBy1->clone();
    layout1->runAction(moveBy1);
    layout2->runAction(moveBy2);
}

void AudioManagerScene::openCloseRecordLayout(cocos2d::Ref *sender, int direction)
{
    if (direction > 0) {
        auto eventListener = EventListenerTouchOneByOne::create();
        eventListener->setSwallowTouches(true);
        eventListener->onTouchBegan = CC_CALLBACK_2(AudioManagerScene::onTouchBegan, this);
        eventListener->onTouchCancelled = CC_CALLBACK_2(AudioManagerScene::onTouchCancelled, this);
        eventListener->onTouchEnded = CC_CALLBACK_2(AudioManagerScene::onTouchEnded, this);
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, recordLayout);
    } else {
        curState = State_SM::LIST;
        Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(recordLayout);
    }
    
    auto layout = (Layout*) sender;
    auto moveBy = MoveBy::create(TIME_CHANGE_ANIMATION, Vec2(0, direction * visibleSize.height));
    layout->runAction(moveBy);
}

bool AudioManagerScene::onTouchBegan(Touch* touch, Event* event)
{
    return true;
}

void AudioManagerScene::onTouchEnded(Touch* touch, Event* event)
{
    if (!this->isScheduled(schedule_selector(AudioManagerScene::timeExecute))) {
        openCloseRecordLayout(recordLayout, -1);
    }
}

void AudioManagerScene::onTouchCancelled(Touch* touch, Event* event)
{
    
}

void AudioManagerScene::touchButtonEvent(Ref* sender, Widget::TouchEventType type)
{
    auto receiver = (Node*) sender;
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        for (int i=0; i<SUMCOLOR; i++)
        {
            if(receiver->getTag()==(int)(Tag_ListItem::T_LISTITEM_OPEN) + (int)i)
            {
                CCLOG("open tag %d",receiver->getTag());
                std::string number = to_string(i+1);
                ((Label*)recordLayout->getChildByTag(T_RECORD_NUMBER))->setString(number);
                ((Label*)recordLayout->getChildByTag(T_RECORD_NAME))->setString(numberName[i]);
                
                // create filename sound
                filename = "number_" + number + "_" + ManageScene::getInstance()->getCurrentLanguage();
                filename = ManageScene::getInstance()->getFileNameFollowPlatform(filename);
                
                // check sound exist
                auto buttonPlay = (Button*)recordLayout->getChildByTag(T_RECORD_PLAY);
                if (ManageScene::getInstance()->checkFileSoundExist(filename.c_str())) {
                    buttonPlay->setBright(true);
                } else {
                    buttonPlay->setBright(false);
                }
                
                curState = State_SM::RECORDING;
                openCloseRecordLayout(recordLayout, 1);
            }
            else if (receiver->getTag()==(int)(Tag_ListItem::T_LISTITEM_REMOVE) + (int)i)
            {
                // init filename
                fileRemove = "number_" + to_string(i+1) + "_" + ManageScene::getInstance()->getCurrentLanguage();
                fileRemove = ManageScene::getInstance()->getFileNameFollowPlatform(fileRemove);
                
                cocos2d::ui::Button::ccWidgetTouchCallback callback;
                callback = CC_CALLBACK_2(AudioManagerScene::touchButtonEvent, this);
                PopupLayer* popup = PopupLayer::createPopup("このレコードを削除\nしますか？", callback, (int)Tag_ListItem::T_LISTITEM_OKBUTTON);
                addChild(popup, 60);
                popup->setTag(Tag::T_POPUP_LAYER);
            }
        }
        switch (receiver->getTag()) {
            // button recording
            case Tag::T_RECORD_RECORDING :
            {
                if (this->isScheduled(schedule_selector(AudioManagerScene::timeExecute))) {
                    break;
                }
                auto buttonPlay = (Button*)recordLayout->getChildByTag(T_RECORD_PLAY);
                auto buttonRecord = (Button*)recordLayout->getChildByTag(T_RECORD_RECORDING);
                auto countDown = (Label*)recordLayout->getChildByTag(T_RECORD_COUNTDOWN);
                if (countdown == TIME_RECORDING) {
                    // stop audio playing
                    audio->stopAllEffects();
                    buttonRecord->loadTextureNormal("button_recording.png");
                    buttonPlay->setBright(false);
                    buttonRecord->setEnabled(false);
                    countDown->runAction(ScaleTo::create(0.2f, 1.0));
                    countDown->setString(to_string(countdown));
                    NativeUtils::startRecording(filename.c_str());
                    this->schedule(schedule_selector(AudioManagerScene::timeExecute), 1.0f);
                }
                break;
            }
                
            // button play record
            case Tag::T_RECORD_PLAY :
            {
                if (countdown == TIME_RECORDING) {
                    if (ManageScene::getInstance()->checkFileSoundExist(filename)) {
                        audio->unloadEffect(filename.c_str());
                        audio->playEffect(filename.c_str());
                    }
                }
                break;
            }
                
                // button JP language
            case Tag::T_JPBUTTON :
            {
                typeLang = LANGUAGE_TYPE::JA;
                prepareDataLanguage(typeLang);
                moveLayout(chooseLayout, listLayout, (int)STATE_MOVE::LEFT);
                curState = State_SM::LIST;
                reloadData();
                break;
            }
                
                // button english language
            case Tag::T_ENBUTTON :
            {
                typeLang = LANGUAGE_TYPE::EN;
                prepareDataLanguage(typeLang);
                moveLayout(chooseLayout, listLayout, (int)STATE_MOVE::LEFT);
                curState = State_SM::LIST;
                reloadData();
                break;
            }
                
                // button reset
            case Tag::T_RESETBUTTON :
            {
                cocos2d::ui::Button::ccWidgetTouchCallback callback;
                callback = CC_CALLBACK_2(AudioManagerScene::touchButtonEvent, this);
                PopupLayer* popup = PopupLayer::createPopup("全部レコードを削除\nしますか？", callback);
                addChild(popup, 60);
                popup->setTag(Tag::T_POPUP_LAYER);
                break;
            }
            
                // remove a record
            case Tag_ListItem::T_LISTITEM_OKBUTTON :
            {
                CCLOG("fileRemove=%s",fileRemove.c_str());
                if(FileUtils::getInstance()->removeFile(FileUtils::getInstance()->getWritablePath()+fileRemove))
                {
                    CCLOG("fileRemove %s removed", fileRemove.c_str());
                    reloadData();
                }
                else {
                    CCLOG("fileRemove %s not exist or error when removing", fileRemove.c_str());
                }
                auto popup = getChildByTag(Tag::T_POPUP_LAYER);
                auto scale = ScaleTo::create(0.2, 0);
                auto set = CallFunc::create(CC_CALLBACK_0(PopupLayer::setVisible, popup, false));
                auto remove = CallFunc::create(CC_CALLBACK_0(PopupLayer::removeFromParent, popup));
                if(popup)
                    popup->runAction(Sequence::create(scale, set, remove, NULL));
                
                // show dialog success
                const char* message = "削除が完成しました";
                ModelDialogLayer* alertLayer = ModelDialogLayer::create(message, 2);
                this->addChild(alertLayer);
                break;
            }
                // button popup ok
            case Tag::T_POPUP_OK :
            {
                CCLOG("test");
                std::string fileRemove;
                int count = 0;
                std::string message;
                // remove files
                for (int i=0; i<SUMCOLOR; i++)
                {
                    fileRemove = "number_" + to_string(i+1) + "_" + ManageScene::getInstance()->getCurrentLanguage();
                    fileRemove = ManageScene::getInstance()->getFileNameFollowPlatform(fileRemove);
                    if(FileUtils::getInstance()->removeFile(FileUtils::getInstance()->getWritablePath()+fileRemove))
                    {
                        CCLOG("fileRemove %s removed", fileRemove.c_str());
                        count++;
                    }
                    else {
                        CCLOG("fileRemove %s not exist or error when removing", fileRemove.c_str());
                    }
                }
                auto popup = getChildByTag(Tag::T_POPUP_LAYER);
                auto scale = ScaleTo::create(0.2, 0);
                auto set = CallFunc::create(CC_CALLBACK_0(PopupLayer::setVisible, popup, false));
                auto remove = CallFunc::create(CC_CALLBACK_0(PopupLayer::removeFromParent, popup));
                if(popup)
                    popup->runAction(Sequence::create(scale, set, remove, NULL));
                
                // show dialog success
                if(count >= 0)
                {
                    message = to_string(count) + " レコードが\n削除されます";
                }
                ModelDialogLayer* alertLayer = ModelDialogLayer::create(message.c_str(), 2);
                this->addChild(alertLayer);
                reloadData();
                break;
            }
                
                // button close setting button
            case Tag::T_CLOSESETTINGBUTTON :
            {
                if (chooseLayout->getNumberOfRunningActions()>0 ||
                    listLayout->getNumberOfRunningActions()>0 ||
                    recordLayout->getNumberOfRunningActions()>0) {
                    return;
                }
                switch (curState) {
                    case  State_SM::CHOOSE :
                        curState = State_SM::CHOOSE;
                        ManageScene::getInstance()->changeState(GAME_STATE::MENU);
                        break;
                        
                    case  State_SM::LIST :
                        curState = State_SM::CHOOSE;
                        moveLayout(chooseLayout, listLayout, (int)STATE_MOVE::RIGHT);
                        break;
                                
                    case  State_SM::RECORDING :
                        if (!this->isScheduled(schedule_selector(AudioManagerScene::timeExecute))) {
                            curState = State_SM::LIST;
                            openCloseRecordLayout(recordLayout, -1);
                        }
                        break;
                                
                    default:
                        break;
                }
                break;
            }
            default:
                break;
        }

    }
}


void AudioManagerScene::timeExecute(float dt)
{
    CCLOG("time %d",countdown);
    if (countdown == 0) {
        NativeUtils::stopRecording();
        countdown = TIME_RECORDING;
        ((Label*)recordLayout->getChildByTag(T_RECORD_COUNTDOWN))->setString(to_string(countdown));
        auto buttonPlay = (Button*)recordLayout->getChildByTag(T_RECORD_PLAY);
        auto buttonRecord = (Button*)recordLayout->getChildByTag(T_RECORD_RECORDING);
        auto countDown = (Label*)recordLayout->getChildByTag(T_RECORD_COUNTDOWN);
        buttonRecord->setEnabled(true);
        buttonRecord->loadTextureNormal("button_record.png");
        countDown->setScale(0.0f);
        if (ManageScene::getInstance()->checkFileSoundExist(filename.c_str())) {
            buttonPlay->setBright(true);
            reloadData();
        } else {
            buttonPlay->setBright(false);
        }
        this->unschedule(schedule_selector(AudioManagerScene::timeExecute));
        return;
    }
    countdown--;
    ((Label*)recordLayout->getChildByTag(T_RECORD_COUNTDOWN))->setString(to_string(countdown));
}

void AudioManagerScene::onExit()
{
    LayerColor::onExit();
    this->stopAllActions();
}