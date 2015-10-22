//
//  LanguagePopupLayer.cpp
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/30.
//
//

#include "LanguagePopupLayer.h"

LanguagePopupLayer::LanguagePopupLayer()
{
    
}

LanguagePopupLayer::~LanguagePopupLayer()
{

}

LanguagePopupLayer* LanguagePopupLayer::create()
{
    LanguagePopupLayer* layer = new LanguagePopupLayer();
    layer->init();
    layer->autorelease();
    return layer;
}

bool LanguagePopupLayer::init()
{
    BasePopupLayer::init();
    layoutSize = layoutTable->getContentSize();
    listviewSize = Size(layoutSize.width, layoutSize.height * 4/5);
    
    // title
    auto title = Text::create("Language", "fonts/keifont.ttf", 60);
    title->setPosition(Vec2(layoutSize.width/2, layoutSize.height - PADDING));
    title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    title->setColor(Color3B::RED);
    layoutTable->addChild(title);

    listView = ListView::create();
    listView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
    listView->setTouchEnabled(true);
    listView->setBounceEnabled(true);
    listView->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    listView->setPosition(Vec2(title->getPositionX(), title->getPositionY() - title->getContentSize().height/2 - PADDING_MENU_HEADER_ITEM));
    listView->setContentSize(listviewSize);
    listView->setInertiaScrollEnabled(true);
    layoutTable->addChild(listView);
    
    // load list language
    SceneManager::getInstance()->loadListLanguage("listlanguage.json");
    // get size
    countLanguage = SceneManager::getInstance()->getIntForKey("CountLanguage");
    // get map
    mapLanguage = SceneManager::getInstance()->getMapData("language", "key", "value");
    // get List
    listLanguageName = SceneManager::getInstance()->getListData("language", "key");
    
    // draw
    reloadData();
    return true;
}

void LanguagePopupLayer::reloadData()
{
    listView->removeAllChildren();
    listButton.clear();
    
    for (int i = 0; i < countLanguage/2; i++) {
        Layout* layout = Layout::create();
        layout->setContentSize(Size(listviewSize.width, listviewSize.height / 4));
        
        Button* lang1 = Button::create("lang_normal.png");
        lang1->setTitleText(listLanguageName[i * 2]);
        lang1->setTitleFontSize(30);
        lang1->setTitleFontName("fonts/keifont.ttf");
        lang1->setTitleColor(Color3B::WHITE);
        lang1->setTouchEnabled(true);
        lang1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lang1->setPosition(Vec2(listviewSize.width/3, layout->getContentSize().height/2));
        lang1->setTag(i*2);
        lang1->addTouchEventListener(CC_CALLBACK_2(LanguagePopupLayer::touchButtonEvent, this));
        layout->addChild(lang1);
        listButton.pushBack(lang1);
        
        Button* lang2 = Button::create("lang_normal.png");
        lang2->setTitleText(listLanguageName[i * 2 + 1]);
        lang2->setTitleFontSize(30);
        lang2->setTitleFontName("fonts/keifont.ttf");
        lang2->setTitleColor(Color3B::WHITE);
        lang2->setTouchEnabled(true);
        lang2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lang2->setPosition(Vec2(listviewSize.width*2/3, layout->getContentSize().height/2));
        lang2->setTag(i*2+1);
        lang2->addTouchEventListener(CC_CALLBACK_2(LanguagePopupLayer::touchButtonEvent, this));
        layout->addChild(lang2);
        listButton.pushBack(lang2);
        
        listView->insertCustomItem(layout, i);
    }
    
    for (int i = 0; i < listButton.size(); i++) {
        if (listButton.at(i)->getTitleText() == SceneManager::getInstance()->getCurLanguage()) {
            listButton.at(i)->loadTextureNormal("lang_selected.png");
            break;
        }
    }
}

std::string LanguagePopupLayer::getValueFromMap(std::string key)
{
    std::map<std::string, std::string>::iterator it = mapLanguage.find(key);
    if (it != mapLanguage.end()) {
        return it->second;
    } else return "";
}

void LanguagePopupLayer::onExit()
{
    BasePopupLayer::onExit();
    NotificationCenter::getInstance()->postNotification(EXIT_LAYER_LANGUAGE);
}

void LanguagePopupLayer::touchButtonEvent(cocos2d::Ref *sender, Widget::TouchEventType type)
{
    Button* receiver = (Button*)sender;
    if (type == ui::Widget::TouchEventType::ENDED) {
        for (int i = 0; i < listButton.size(); i++) {
            listButton.at(i)->loadTextureNormal("lang_normal.png");
        }
        receiver->loadTextureNormal("lang_selected.png");
        SceneManager::getInstance()->setCurLanguage(receiver->getTitleText());
        SceneManager::getInstance()->saveLanguage();
        NotificationCenter::getInstance()->postNotification(RELOAD_LANGUAGE);
    }
}