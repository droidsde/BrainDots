//
//  HighScoreScene.cpp
//  GameTetris
//
//  Created by Nguyen Minh Hoang on 2015/04/02.
//
//

#include "HighScoreScene.h"

Scene* HighScoreScene::createScene()
{
    auto scene = Scene::create();
    auto layer = HighScoreScene::create();
    scene->addChild(layer);
    
    return scene;
}

// on "init" you need to initialize your instance
bool HighScoreScene::init()
{
    //init layer
    if ( !Layer::init() ) return false;
    
    //get size of screen
    winSize = Director::getInstance()->getWinSize();
    setMS(ManageSound::getInstance());
    
    auto background = Sprite::createWithSpriteFrameName("background.png");
    float scale_x = winSize.width / background->getContentSize().width;
    float scale_y = winSize.height / background->getContentSize().height;
    
    background->setScaleX(scale_x);
    background->setScaleY(scale_y);
    background->setPosition(winSize.width / 2, winSize.height / 2);
    this->addChild(background);
    
    // title
    auto title = Label::createWithTTF("highscore", "kenpixel_future_square.ttf", 56);
    title->setPosition(winSize.width / 2, winSize.height/ 10 * 9);
    title->enableOutline(Color4B(25,26,25,155),5);
    this->addChild(title);
    
    // board
    board = Sprite::createWithSpriteFrameName("menu_board.png");
    sizeBoard = board->getContentSize();
    board->setPosition(winSize.width / 2, winSize.height / 2);
    
    // close button
    auto menuClose = MenuItemSprite::create(Sprite::createWithSpriteFrameName("close_button.png"), Sprite::createWithSpriteFrameName("close_button.png"), CC_CALLBACK_1(HighScoreScene::closeScene, this));
    menuClose->setScale(1.5f);
    menuClose->setPosition(board->getContentSize().width, board->getContentSize().height);
    auto menu = Menu::create(menuClose, nullptr);
    menu->setPosition(Point::ZERO);
    board->addChild(menu, 300);
    
    // data for table
    for (int i = 0; i <= 10; i++)
    {
        std::string name = StringUtils::format("name%d", i);
        std::string name_ = UserDefault::getInstance()->getStringForKey(name.c_str(), "");
        
        std::string score = StringUtils::format("score%d", i);
        int score_ = UserDefault::getInstance()->getIntegerForKey(score.c_str());
        
        listUserData.push_back(UserData(name_ , i, score_));
    }
    //sort data
    std::sort(listUserData.begin(), listUserData.end(), compare_score());
    
    //table
    TableView* tableView = TableView::create(this, sizeBoard);
    // set direction of table
    tableView->setDirection(TableView::Direction::VERTICAL);
    // fill table from too to bottom
    tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    
    //add tableview
    tableView->setDelegate(this);
    board->addChild(tableView);
    tableView->reloadData();
    
    this->addChild(board);
    return true;
}

// set size of a cell of table
Size HighScoreScene::cellSizeForTable(TableView *table)
{
    return Size(sizeBoard.width, sizeBoard.height*.2);
}

// set up a cell
TableViewCell* HighScoreScene::tableCellAtIndex(TableView *table, ssize_t idx)
{
    std::string id = StringUtils::format("%ld", idx+1);
    std::string score = StringUtils::format("%d", listUserData[idx].score);
    std::string name = StringUtils::format("%s", listUserData[idx].name.c_str());
    
    TableViewCell *cell = table->dequeueCell();
    
    cell = new TableViewCell();
    cell->autorelease();
    cell->setAnchorPoint(Vec2(0.5, 0.5));
    
    auto background_color = Color3B(255,255,255);
    
    // border line
    Sprite* line = Sprite::create();
    line->setAnchorPoint(Point(0, 0));
    line->setTextureRect(Rect(0, 0, sizeBoard.width, 1));
    line->setColor(background_color);
    cell->addChild(line);
    
    // Score text
    auto *idText = LabelTTF::create(id.c_str(), "Arial", 30);
    idText->setPosition(Point(10, sizeBoard.height*.2 / 2));
    idText->setColor(Color3B::WHITE);
    idText->setAnchorPoint(Vec2(0, 0.5));
    idText->setColor(Color3B::RED);
    cell->addChild(idText);
    
    // Score text
    auto *scoreText = LabelTTF::create(score.c_str(), "Arial", 30);
    scoreText->setPosition(Point(420, sizeBoard.height*.2 / 2));
    scoreText->setColor(Color3B::WHITE);
    scoreText->setAnchorPoint(Vec2(1.0, 0.5));
    cell->addChild(scoreText);
    
    // name text
    auto *nameText = LabelTTF::create(name.c_str(), "Arial", 30);
    nameText->setPosition(Point(50, sizeBoard.height*.2 / 2));
    nameText->setColor(Color3B::WHITE);
    nameText->setAnchorPoint(Vec2(0, 0.5));
    cell->addChild(nameText);
    
    return cell;
}

// size of cells
ssize_t HighScoreScene::numberOfCellsInTableView(TableView *table)
{
    return listUserData.size();
}

// cell touch callback
void HighScoreScene::tableCellTouched(TableView* table, TableViewCell* cell)
{
//    CCLOG("%zd cell touch", cell->getIdx());
}

void HighScoreScene::closeScene(cocos2d::Ref *sender)
{
    getMS()->playClickSound();
    ManageScene::getInstance()->changeState(GAME_STATE::MENU);
}
