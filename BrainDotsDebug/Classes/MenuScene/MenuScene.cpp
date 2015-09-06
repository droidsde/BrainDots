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
	addChild(title);

	auto sprite = Sprite::create("sticker.png");
	stickerSize = sprite->getContentSize();
	CCLOG("%f %f", stickerSize.width, stickerSize.height);

	listview = ListView::create();
	listview->setDirection(ScrollView::Direction::HORIZONTAL);
	listview->setGravity(ListView::Gravity::CENTER_VERTICAL);
	listview->setTouchEnabled(true);
	listview->setBounceEnabled(true);
	listview->setAnchorPoint(Vec2(0, 0.5));
	listview->setPosition(Vec2(0, visibleSize.height / 2));
	listview->setContentSize(Size(visibleSize.width, stickerSize.height));
	addChild(listview);

	reloadData();
	return true;
}

void MenuScene::reloadData() {
	listview->removeAllChildren();
	for (int i = 0; i < LEVEL_MAX; ++i) {
		Layout* layout = Layout::create();
		layout->setContentSize(
				Size(stickerSize.width + 50,
						listview->getContentSize().height));
		auto level = Button::create("sticker.png", "sticker.png", "");
		level->setTitleText(to_string(i + 1));
		level->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		level->setTitleColor(Color3B::RED);
		level->setTitleFontSize(100);
		level->setTitleFontName("arial.ttf");
		level->setPosition(Vec2(layout->getContentSize() / 2));
		level->setTouchEnabled(true);
		level->setTag((int)TAG_MENU::TAG_MENU_LEVEL_CHOOSE+i);
		level->addTouchEventListener(
				CC_CALLBACK_2(MenuScene::touchButtonEvent, this));
		layout->addChild(level);
		listview->insertCustomItem(layout, i);
	}
	listview->refreshView();
	listview->scrollToPercentHorizontal(((SceneManager::getInstance()->getLevelGame()+0.5)/LEVEL_MAX)*100, 0.5, true);
}

void MenuScene::touchButtonEvent(Ref* sender, Widget::TouchEventType type) {
	auto receiver = (Node*) sender;
	if (type == ui::Widget::TouchEventType::ENDED) {
		for (int i = 0; i < LEVEL_MAX; ++i) {
			if(receiver->getTag()==(int)(TAG_MENU::TAG_MENU_LEVEL_CHOOSE) + (int)i)
			{
				SceneManager::getInstance()->setLevelGame(i);
				SceneManager::getInstance()->changeState(GAME_STATE::GAME);
			}
		}
		switch (receiver->getTag()) {
		}
	}
}

void MenuScene::onEnter() {
	LayerColor::onEnter();
}

void MenuScene::onExit() {
	LayerColor::onExit();
}
