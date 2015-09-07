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

	auto title = Button::create();
	title->setTouchEnabled(true);
	title->setTitleText("Brain Dots");
	title->setTitleFontSize(80);
	title->setTitleFontName("arial.ttf");
	title->setTitleColor(Color3B::ORANGE);
	title->setAnchorPoint(Vec2(0, 1));
	title->setPosition(Vec2(20, visibleSize.height - 20));
    title->setTag(TAG_MENU::TAG_MENU_BUTTON_TITLE);
    title->addTouchEventListener(CC_CALLBACK_2(MenuScene::touchButtonEvent, this));
	addChild(title);
    
    // prepare
    auto sprite = Sprite::create("sticker.png");
    sprite->retain();
    auto ministicker = Sprite::create("mini_sticker.png");
    ministicker->retain();
    stickerSize = sprite->getContentSize();
    miniStickerSize = ministicker->getContentSize();
    curLevel = SceneManager::getInstance()->getLevelGame();
    curPage = curLevel / ITEMS_IN_PAGE + 1;
    
    // add list horizontal
    addListHorizontal();
    
    // add pageview
    addPageView();
    
	return true;
}

void MenuScene::reloadData() {
	listview->removeAllChildren();
	for (int i = 0; i < LEVEL_MAX; ++i) {
		Layout* layout = Layout::create();
        Button* stage = Button::create("sticker.png");
        
        if (i == 0 || i == LEVEL_MAX-1) {
            layout->setContentSize(Size(stickerSize.width/2+visibleSize.width/2, listview->getContentSize().height));
            if (i == 0) {
                stage->setPosition(Vec2(layout->getContentSize().width-stickerSize.width/2, layout->getContentSize().height/2));
            } else if (i == LEVEL_MAX-1)
            {
                stage->setPosition(Vec2(stickerSize.width/2, layout->getContentSize().height/2));
            }
        }
        else {
            layout->setContentSize(Size(stickerSize.width, listview->getContentSize().height));
            stage->setPosition(Vec2(layout->getContentSize() / 2));
        }
        
        stage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		stage->setTitleText(StringUtils::format("Stage %d",i+1));
		stage->setTitleColor(Color3B::RED);
		stage->setTitleFontSize(100);
		stage->setTitleFontName("arial.ttf");
		stage->setTouchEnabled(true);
		stage->setTag(i);
		stage->addTouchEventListener(
				CC_CALLBACK_2(MenuScene::touchButtonEvent, this));
		// add tick icon
		ImageView* tick = ImageView::create("tick.png");
		tick->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
		tick->setPosition(Vec2(stage->getContentSize().width - 10, stage->getContentSize().height - 10));
		stage->addChild(tick);

		layout->addChild(stage);
		listview->insertCustomItem(layout, i);
	}
	listview->refreshView();
}

void MenuScene::addListHorizontal()
{
    listview = ListView::create();
    listview->setDirection(ScrollView::Direction::HORIZONTAL);
    listview->setGravity(ListView::Gravity::CENTER_VERTICAL);
    listview->setTouchEnabled(true);
    listview->setBounceEnabled(true);
    listview->setAnchorPoint(Vec2(0, 0.5));
    listview->setPosition(Vec2(0, visibleSize.height / 2));
    listview->setContentSize(Size(visibleSize.width, stickerSize.height));
    listview->setItemsMargin(50);
    listview->addEventListener(CC_CALLBACK_2(MenuScene::selectedItemEvent, this));
    addChild(listview);
    
    reloadData();
}

void MenuScene::addPageView()
{
    pageview = PageView::create();
    pageview->setContentSize(Size(visibleSize.width, stickerSize.height));
    pageview->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    pageview->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    pageview->removeAllPages();
    
    float spaceX = (visibleSize.width - 3 * miniStickerSize.width) / 4 + miniStickerSize.width;
    float spaceY = (stickerSize.height - 2 * miniStickerSize.height) / 3 + miniStickerSize.height;
    
    for (int i = 0; i < LEVEL_MAX; i++) {
        
        Layout* layout = Layout::create();
        layout->setContentSize(pageview->getContentSize());
        
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 3; k++) {
                Button* level = Button::create("mini_sticker.png");
                level->setTitleText(to_string(ITEMS_IN_PAGE * i + 3 * j + k + 1));
                level->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
                level->setTitleColor(Color3B::RED);
                level->setTitleFontSize(60);
                level->setTitleFontName("arial.ttf");
                level->setTouchEnabled(true);
                level->setTag(TAG_MENU::TAG_MENU_LEVEL_CHOOSE + ITEMS_IN_PAGE * i + 3 * j + k);
                level->addTouchEventListener(
                                             CC_CALLBACK_2(MenuScene::touchButtonEvent, this));
                level->setPosition(Vec2( (k+1) * spaceX, (2-j) * spaceY));
                layout->addChild(level);
            }
        }
        pageview->insertPage(layout, i);
    }
    pageview->setCustomScrollThreshold(10);
    pageview->addEventListener(CC_CALLBACK_2(MenuScene::pageViewEvent, this));
    addChild(pageview);
    pageview->setVisible(false);
}

void MenuScene::touchButtonEvent(Ref* sender, Widget::TouchEventType type) {
	auto receiver = (Node*) sender;
	if (type == ui::Widget::TouchEventType::ENDED) {
        
        for (int i = TAG_MENU::TAG_MENU_LEVEL_CHOOSE; i < TAG_MENU::TAG_MENU_LEVEL_CHOOSE + LEVEL_MAX*ITEMS_IN_PAGE; i++) {
            if (receiver->getTag() == i) {
                CCLOG("level %d", i);
                // open game level
                SceneManager::getInstance()->setLevelGame(i-TAG_MENU::TAG_MENU_LEVEL_CHOOSE);
                SceneManager::getInstance()->changeState(GAME_STATE::GAME);
            }
        }
        
		switch (receiver->getTag()) {
            case TAG_MENU::TAG_MENU_BUTTON_TITLE :
                if (!listview->isVisible()) {
                    showStages(curPage-1);
                }
                break;
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
            CCLOG("page turning %d", curPage);
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
            CCLOG("page %d %f", i, listview->getInnerContainer()->getPositionX());
            openStage(i);
            break;
        }
            
        default:
            break;
    }
}

void MenuScene::openStage(int i)
{
    if (listview->getInnerContainer()->getPositionX() != -(stickerSize.width+50)*i) {
        listview->getInnerContainer()->setPositionX(-(stickerSize.width+50)*i);
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
    if (listview->getInnerContainer()->getPositionX() != -(stickerSize.width+50)*i) {
        listview->getInnerContainer()->setPositionX(-(stickerSize.width+50)*i);
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
