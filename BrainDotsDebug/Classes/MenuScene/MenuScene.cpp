//
//  MenuScene.cpp
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/04.
//
//

#include "MenuScene.h"

Scene* MenuScene::createScene() {
	auto scene = Scene::create();
	auto layer = MenuScene::create();
	layer->setSoundMusic(SoundManager::getInstance());
	scene->addChild(layer);
	return scene;
}

bool MenuScene::init() {
	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255))) {
		return false;
	}

	origin = Director::getInstance()->getVisibleOrigin();
	visibleSize = Director::getInstance()->getVisibleSize();
    
    // prepare
    auto sprite = Sprite::create("paper2.png");
    sprite->retain();
    auto ministicker = Sprite::create("mini_sticker.png");
    ministicker->retain();
    stickerSize = sprite->getContentSize();
    miniStickerSize = ministicker->getContentSize();
    miniTickSize = Sprite::create("mini_tick.png")->getContentSize();
    
    curLevel = SceneManager::getInstance()->getLevelGame();
    curPage = curLevel / ITEMS_IN_PAGE + 1;
    bodySize = Size(visibleSize.width, visibleSize.height * 5 / 6);
    headerSize = Size(visibleSize.width, visibleSize.height / 6);
    
    // draw grids
    this->drawGrids();
    
    // add header layer
    addHeaderLayer();
    
    // add body layer
    addBodyLayer();
    
	return true;
}

void MenuScene::drawGrids()
{
    DrawNode* draw = DrawNode::create();
    this->addChild(draw);
    
    float lineSize = 0.5;
    Color4F color = Color4F(0/255, 205/255, 1, 0.2);
    
    // draw col
    for (int x = 0; x < visibleSize.width; x++) {
        float xPoint = x * TILE_SIZE;
        draw->drawSegment(Vec2(xPoint, 0), Vec2(xPoint, visibleSize.height), lineSize, color);
    }
    
    // draw row
    for (int y = 0;  y < visibleSize.height; y++) {
        float yPoint = y * TILE_SIZE;
        draw->drawSegment(Vec2(0, yPoint), Vec2(visibleSize.width, yPoint), lineSize, color);
    }
}

void MenuScene::addHeaderLayer()
{
    // header layer
    headerLayer = Layer::create();
    headerLayer->setContentSize(headerSize);
    headerLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    headerLayer->setPosition(Vec2(0, visibleSize.height - headerSize.height));
    addChild(headerLayer, ZORDER_MENU::HEADER_LAYER);
    
    // button title
    auto title = Button::create();
    title->setTouchEnabled(true);
    title->setTitleText("Brain Dots");
    title->setTitleFontSize(80);
    title->setTitleFontName("fonts/keifont.ttf");
    title->setTitleColor(Color3B::ORANGE);
    title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    title->setPosition(Vec2(PADDING_MENU_HEADER_ITEM, headerSize.height/2));
    title->setTag(TAG_MENU::TAG_BUTTON_TITLE);
    title->addTouchEventListener(CC_CALLBACK_2(MenuScene::touchButtonEvent, this));
    headerLayer->addChild(title, ZORDER_MENU::HEADER_TITLE);
    
    // button menu
    Button* menu = Button::create("menu_icon.png");
    menu->setTouchEnabled(true);
    menu->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    menu->setPosition(Vec2(headerSize.width - PADDING_MENU_HEADER_ITEM, headerSize.height/2));
    menu->addTouchEventListener(CC_CALLBACK_2(MenuScene::touchButtonEvent, this));
    menu->setTag(TAG_MENU::TAG_BUTTON_MENU);
    headerLayer->addChild(menu);
    
    // button share
    Button* share = Button::create("share_icon.png");
    share->setTouchEnabled(true);
    share->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    share->setPosition(Vec2(menu->getPositionX() - menu->getContentSize().width - PADDING_MENU_HEADER_ITEM, headerSize.height/2));
    share->addTouchEventListener(CC_CALLBACK_2(MenuScene::touchButtonEvent, this));
    share->setTag(TAG_MENU::TAG_BUTTON_SHARE);
    headerLayer->addChild(share);
    
    // record video
    Button* record = Button::create("recordvideo_icon.png");
    record->setTouchEnabled(true);
    record->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    record->setPosition(Vec2(share->getPositionX() - share->getContentSize().width - PADDING_MENU_HEADER_ITEM, headerSize.height/2));
    record->addTouchEventListener(CC_CALLBACK_2(MenuScene::touchButtonEvent, this));
    record->setTag(TAG_MENU::TAG_BUTTON_RECORD_VIDEO);
    headerLayer->addChild(record);
    
    // button pencil
    Button* pencil = Button::create("pencil_icon.png");
    pencil->setTouchEnabled(true);
    pencil->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    pencil->setPosition(Vec2(record->getPositionX() - record->getContentSize().width - PADDING_MENU_HEADER_ITEM, headerSize.height/2));
    pencil->addTouchEventListener(CC_CALLBACK_2(MenuScene::touchButtonEvent, this));
    pencil->setTag(TAG_MENU::TAG_BUTTON_PENCIL);
    headerLayer->addChild(pencil);
}

void MenuScene::addBodyLayer()
{
    // add list horizontal
    addListHorizontal();
    
    // add pageview
    addPageView();
}

void MenuScene::addListHorizontal()
{
    // add body layer
    bodyLayer = Layer::create();
    bodyLayer->setContentSize(bodySize);
    bodyLayer->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    bodyLayer->setPosition(Vec2::ZERO);
    addChild(bodyLayer, ZORDER_MENU::HEADER_LAYER);
    
    // add listview in body layer
    listview = ListView::create();
    listview->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    listview->setGravity(ListView::Gravity::CENTER_VERTICAL);
    listview->setTouchEnabled(true);
    listview->setBounceEnabled(true);
    listview->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    listview->setPosition(bodySize/2);
    listview->setContentSize(bodySize);
    listview->setInertiaScrollEnabled(true);
    listview->setItemsMargin(LIST_BIG_ITEM_MARGIN);
    listview->addEventListener(CC_CALLBACK_2(MenuScene::selectedItemEvent, this));
    listview->addEventListener(CC_CALLBACK_2(MenuScene::scrollEvent, this));
    
    bodyLayer->addChild(listview, ZORDER_MENU::BODY_LISTVIEW);
    
    // create item in listview
    reloadData();
}

void MenuScene::reloadData() {
    int maxStage = (int)((int) (SceneManager::getInstance()->getCurMaxLevel()/6)) % LEVEL_MAX;
	listview->removeAllItems();
	for (int i = 0; i < LEVEL_MAX; ++i) {
        // layout include sticker and text
		Layout* layout = Layout::create();
        Button* stage = Button::create("paper2.png");
        
        if (i == 0 || i == LEVEL_MAX-1) {
            layout->setContentSize(Size(stickerSize.width/2+visibleSize.width/2, bodySize.height));
            if (i == 0) {
                stage->setPosition(Vec2(layout->getContentSize().width-stickerSize.width/2, bodySize.height - stickerSize.height/2 - miniTickSize.height/2));
            } else if (i == LEVEL_MAX-1)
            {
                stage->setPosition(Vec2(stickerSize.width/2, bodySize.height - stickerSize.height/2 - miniTickSize.height/2));
            }
        }
        else {
            layout->setContentSize(Size(stickerSize.width, bodySize.height));
            stage->setPosition(Vec2(layout->getContentSize().width/2, bodySize.height - stickerSize.height/2 - miniTickSize.height/2));
        }
        
        // stage in sticker
        stage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		stage->setTitleText(StringUtils::format("Stage %d",i+1));
		stage->setTitleColor(Color3B::RED);
		stage->setTitleFontSize(100);
		stage->setTitleFontName("fonts/keifont.ttf");
		stage->setTouchEnabled(true);
        stage->setZoomScale(0);
		stage->setTag(i);
		stage->addTouchEventListener(
				CC_CALLBACK_2(MenuScene::touchButtonEvent, this));
        
        // text
        Text* text = Text::create(" 6 / 6", "fonts/keifont.ttf", 60);
        text->setColor(Color3B::RED);
        text->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        text->setPosition(Vec2(stage->getPositionX(), stage->getPositionY() - stickerSize.height/2));
        layout->addChild(text, ZORDER_MENU::BODY_STICKER_TEXT);
        
        // add tick and text
        ImageView* minitick = ImageView::create("mini_tick.png");
        minitick->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        minitick->setPosition(Vec2(text->getPositionX() - text->getContentSize().width/2 - PADDING, text->getPositionY() - text->getContentSize().height/2));
        layout->addChild(minitick, ZORDER_MENU::BODY_STICKER_MINI_TICK);
        
        if (i > maxStage) {
            stage->setTouchEnabled(false);
            text->setString(" 0 / 6");
            
            auto frameGray = Layout::create();
            frameGray->setContentSize(stickerSize);
            frameGray->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
            frameGray->setBackGroundColor(Color3B::GRAY);
            frameGray->setBackGroundColorOpacity(150);
            frameGray->setPosition(Vec2::ZERO);
//            stage->addChild(frameGray);
            stage->setBright(false);
            auto lockIcon = Sprite::create("lock_icon_256x256.png");
            lockIcon->setPosition(stickerSize/2);
            stage->addChild(lockIcon);
        } else if (i < maxStage) {
            
            // add tick icon in stage image
            ImageView* tick = ImageView::create("mini_tick.png");
            tick->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            tick->setPosition(Vec2(stickerSize.width - PADDING, stickerSize.height - PADDING));
            stage->addChild(tick, ZORDER_MENU::BODY_STICKER_TICK);
            
        } else {
            text->setString(" "+to_string(SceneManager::getInstance()->getCurMaxLevel() % 6) + " / 6");
        }
        layout->addChild(stage, ZORDER_MENU::BODY_STICKER);
		listview->insertCustomItem(layout, i);
	}
	listview->refreshView();
}

void MenuScene::addPageView()
{
    pageview = PageView::create();
    pageview->setContentSize(bodySize);
    pageview->setPosition(bodySize/2);
    pageview->setLoopFocus(true);
    pageview->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    pageview->removeAllPages();
    
    float spaceX = (bodySize.width - 3 * miniStickerSize.width) / 4 + miniStickerSize.width;
    float spaceY = (bodySize.height - 2 * miniStickerSize.height) / 3 + miniStickerSize.height;
    
    for (int i = 0; i < LEVEL_MAX; i++) {
        
        Layout* layout = Layout::create();
        layout->setContentSize(pageview->getContentSize());
        
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 3; k++) {
                Button* level = Button::create("paper7.png");
                int index = ITEMS_IN_PAGE * i + 3 * j + k;
                level->setTitleText(to_string(index + 1));
                level->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
                level->setTitleColor(Color3B::RED);
                level->setTitleFontSize(60);
                level->setTitleFontName("fonts/keifont.ttf");
                level->setTouchEnabled(true);
                level->setTag(TAG_MENU::TAG_LEVEL_CHOOSE + index);
                level->addTouchEventListener(
                                             CC_CALLBACK_2(MenuScene::touchButtonEvent, this));
                level->setPosition(Vec2( (k+1) * spaceX, (2-j) * spaceY));
                
                // lock level
                if (index > SceneManager::getInstance()->getCurMaxLevel()) {
                    auto frameGray = Layout::create();
                    frameGray->setContentSize(level->getContentSize());
                    frameGray->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
                    frameGray->setBackGroundColor(Color3B::GRAY);
                    frameGray->setBackGroundColorOpacity(150);
                    frameGray->setPosition(Vec2::ZERO);
                    level->addChild(frameGray);
                    auto lockIcon = Sprite::create("lock_icon_128x128.png");
                    lockIcon->setPosition(level->getContentSize()/2);
                    level->addChild(lockIcon);
                    level->setTouchEnabled(false);
                } else if (index < SceneManager::getInstance()->getCurMaxLevel()){
                    // add tick icon in stage image
                    ImageView* tick = ImageView::create("super_mini_tick.png");
                    tick->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
                    tick->setPosition(Vec2(level->getContentSize().width-tick->getContentSize().width/2-PADDING, level->getContentSize().height-PADDING-tick->getContentSize().height/2));
                    level->addChild(tick, ZORDER_MENU::BODY_STICKER_TICK);
                }
                
                layout->addChild(level);
            }
        }
        pageview->insertPage(layout, i);
    }
    pageview->setCustomScrollThreshold(SCROLL_THRESHOLD);
    pageview->addEventListener(CC_CALLBACK_2(MenuScene::pageViewEvent, this));
    addChild(pageview);
    pageview->setVisible(false);
}


void MenuScene::capturingScreen(std::string filename)
{
    utils::captureScreen(CC_CALLBACK_2(MenuScene::afterCaptured, this), filename);
}

void MenuScene::afterCaptured(bool succeed, const std::string &outputFile)
{
    if (succeed) {
        // show screenshot
        CCLOG("Capture screen successed");
    }
    else
    {
        CCLOG("Capture screen failed");
    }
    
    auto baseLayer = SharePopupLayer::create("capture_menu.png");
    this->addChild(baseLayer);
}

void MenuScene::touchButtonEvent(Ref* sender, Widget::TouchEventType type) {
	auto receiver = (Node*) sender;
	if (type == ui::Widget::TouchEventType::ENDED) {
        
        for (int i = TAG_MENU::TAG_LEVEL_CHOOSE; i < TAG_MENU::TAG_LEVEL_CHOOSE + LEVEL_MAX*ITEMS_IN_PAGE; i++) {
            if (receiver->getTag() == i) {
                // check file game
                std::string nameLevel = "level" + to_string(i-TAG_MENU::TAG_LEVEL_CHOOSE) + ".tmx";
                if (!SceneManager::getInstance()->checkFileExist(nameLevel)) {
                    break;
                }
                // open game level
                SceneManager::getInstance()->setLevelGame(i-TAG_MENU::TAG_LEVEL_CHOOSE);
                
                auto fadeout = CallFunc::create(CC_CALLBACK_0(Node::setOpacity, this, 0));
                auto remove = CallFunc::create(CC_CALLBACK_0(Node::removeAllChildrenWithCleanup, this, true));
                auto loading = CallFunc::create(CC_CALLBACK_0(SceneManager::loadingScene, SceneManager::getInstance(), this));
                auto change = CallFunc::create(CC_CALLBACK_0(SceneManager::changeState, SceneManager::getInstance(), GAME_STATE::GAME));
                
                this->runAction(Sequence::create(Spawn::create(DelayTime::create(TIME_LOADING), remove, fadeout, loading, NULL), change, NULL));
            }
        }
        
		switch (receiver->getTag()) {
            case TAG_MENU::TAG_BUTTON_TITLE :
                if (!listview->isVisible()) {
                    showStages(curPage-1);
                }
                break;
            case TAG_MENU::TAG_BUTTON_PENCIL :
            {
                auto baseLayer = PencilPopupLayer::create();
                this->addChild(baseLayer);
                break;
            }
            case TAG_MENU::TAG_BUTTON_RECORD_VIDEO :
            {
                auto baseLayer = RecordPopupLayer::create();
                this->addChild(baseLayer);
                break;
            }
            case TAG_MENU::TAG_BUTTON_SHARE :
            {
                std::string fileRemove = "capture_menu.png";
                // remove cache image
//                Director::getInstance()->getTextureCache()->removeTextureForKey("capture_menu.png");
                // remove file screen shoot
                if(FileUtils::getInstance()->removeFile(FileUtils::getInstance()->getWritablePath() + fileRemove))
                {
                    CCLOG("fileRemove %s removed", fileRemove.c_str());
                }
                else {
                    CCLOG("fileRemove %s not exist or error when removing", fileRemove.c_str());
                }
                // only capture when show pageview
                if (pageview->isVisible()) {
                    this->capturingScreen(fileRemove);
                } else {
                    auto baseLayer = SharePopupLayer::create(fileRemove);
                    this->addChild(baseLayer);
                }
                break;
            }
            case TAG_MENU::TAG_BUTTON_MENU :
            {
                auto baseLayer = MenuPopupLayer::create();
                this->addChild(baseLayer);
                break;
            }
		}
	}
}

void MenuScene::pageViewEvent(cocos2d::Ref *pSender, PageView::EventType type)
{
    switch (type) {
        case PageView::EventType::TURNING :
        {
            PageView* pageView = dynamic_cast<PageView*>(pSender);
            curPage = (int)pageView->getCurPageIndex() + 1;
        }
            break;
            
        default:
            break;
    }
}

void MenuScene::selectedItemEvent(cocos2d::Ref *pSender, ListView::EventType type)
{
    ListView* listView = static_cast<ListView*>(pSender);
    switch (type) {
        case ListView::EventType::ON_SELECTED_ITEM_START :
        {
            break;
        }
        case ListView::EventType::ON_SELECTED_ITEM_END :
        {
            int i = (int)listView->getCurSelectedIndex();
            openStage(i);
            break;
        }
            
        default:
            break;
    }
}

void MenuScene::scrollEvent(cocos2d::Ref *pSender, ui::ScrollView::EventType type)
{
    
}

void MenuScene::openStage(int i)
{
    float curPos = listview->getInnerContainer()->getPositionX();
    float exactPos = -(stickerSize.width + LIST_BIG_ITEM_MARGIN) * i;
    pageview->scrollToPage(i);
    if (curPos > (exactPos + DELTA_TRANSLATE) || curPos < (exactPos - DELTA_TRANSLATE)) {
        listview->getInnerContainer()->runAction(MoveTo::create(0.3f, Vec2(exactPos, 0)));
    }
    else
    {
        showLevelsInStage(i);
    }
}

void MenuScene::showLevelsInStage(int stage)
{
    curPage = stage + 1;
    listview->setVisible(false);
    pageview->setVisible(true);
    pageview->scrollToPage(stage);
}

void MenuScene::showStages(int i)
{
    listview->setVisible(true);
    pageview->setVisible(false);
    float curPos = listview->getInnerContainer()->getPositionX();
    float exactPos = -(stickerSize.width + LIST_BIG_ITEM_MARGIN) * i;
    
    if (curPos > (exactPos + DELTA_TRANSLATE) || curPos < (exactPos - DELTA_TRANSLATE)) {
//        listview->getInnerContainer()->runAction(MoveTo::create(0.3f, Vec2(exactPos, 0)));
        listview->getInnerContainer()->setPositionX(exactPos);
    }
}


void MenuScene::onEnter() {
	LayerColor::onEnter();
    // jump to stage playing
    if (curLevel < 1) {
        float percent = ((float) (curPage-1) / (float) LEVEL_MAX) * 100;
        listview->jumpToPercentHorizontal(percent);
    }
    else
    {
        showLevelsInStage(curPage-1);
    }
}

void MenuScene::onExit() {
	LayerColor::onExit();
}
