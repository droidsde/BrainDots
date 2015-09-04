//
//  GameBoard.cpp
//  GameTetris
//
//  Created by Nguyen Minh Hoang on 2015/03/19.
//
//

#include "GameBoard.h"

bool GameBoard::init()
{
    if (!Layer::init())
    {
        return false;
    }
    this->winSize = Director::getInstance()->getWinSize();
    
    this->setIsPaused(false);
    this->setPosition(PADDING_BUTTON_MENU, winSize.height / 2 - BOARD_HEIGHT / 2 * BLOCK_WIDTH);
    
    // add background
    auto panel = Sprite::createWithSpriteFrameName("board_background.png");
    this->addChild(panel);
    panel->setAnchorPoint(Point::ZERO);
    panel->setPosition(Point(-5,-5));
    panel->setColor(Color3B::GRAY);
    
    // add grid to board
    auto grid = Sprite::create("grid.png");
    this->addChild(grid);
    grid->setAnchorPoint(Point::ZERO);
    
    // set sound
    setMS(ManageSound::getInstance());
    
    // init field
    initPlayField();
    
    // start game
    gameStart();
    
    // create shadow
    auto shadowBG = Sprite::create("background.png");
    shadowBG->setTextureRect(Rect(0, 0, BOARD_WIDTH * BLOCK_WIDTH + 10, BLOCK_HEIGHT * SHAPE_SIZE));
    shadowBG->setAnchorPoint(Vec2(0.0 , 0.0));
    shadowBG->setPosition(Vec2(0 , BLOCK_HEIGHT * BOARD_HEIGHT));
    this->addChild(shadowBG, 100);
    
    return true;
}

GameBoard::GameBoard()
{
    this->setContentSize(Size(BLOCK_WIDTH * BOARD_WIDTH, BLOCK_WIDTH * BOARD_HEIGHT));
    playFieldVector.resize(BOARD_HEIGHT+1);
    
    for (int i = 0; i <= BOARD_HEIGHT; i++) {
        playFieldVector[i].resize(BOARD_WIDTH);
    }
    
    comboVector.resize(4);
    
    currentTetromino = nullptr;
    curTop = FIELD_BOTTOM;
    
}

GameBoard::~GameBoard()
{
    playFieldVector.clear();
    this->removeAllChildrenWithCleanup(true);
}

void GameBoard::onEnter()
{
    CCLOG("gameboard onenter");
    Layer::onEnter();
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameBoard::pauseGame), PAUSE_GAME, NULL);
}

void GameBoard::onExit()
{
    CCLOG("gameboard onexit");
    Layer::onExit();
    NotificationCenter::getInstance()->removeObserver(this, PAUSE_GAME);
}

// init board field
void GameBoard::initPlayField()
{
    for (int y = FIELD_TOP+1; y >= FIELD_BOTTOM; y--) // 19->0
    {
        for (int x = FIELD_LEFT_BOARD; x <= FIELD_RIGHT_BOARD; x++) // 0 -> 9
        {
            BlockDef blockDef = { PLAYFIELD, EMPTY, TETROMINO_TYPE::NOTHING, x, y};
            auto blockCell = BlockImage::create(blockDef);
            this->addChild(blockCell);
            playFieldVector[y][x] = blockCell;
        }
    }
}

// shaking board
void GameBoard::shakingBoard()
{
    auto smallMoveDown = MoveTo::create(0.1f, Vec2(PADDING_BUTTON_MENU, winSize.height / 2 - BOARD_HEIGHT / 2 * BLOCK_WIDTH - 5));
    auto smallMoveBack = MoveTo::create(0.1f, Vec2(PADDING_BUTTON_MENU, winSize.height / 2 - BOARD_HEIGHT / 2 * BLOCK_WIDTH));
    auto sequence = Sequence::create(smallMoveDown, smallMoveBack, nullptr);
    this->runAction(sequence);
}

// Start game
void GameBoard::gameStart()
{
    createCurrentTetro();
    createNextTetro();
    createGhostPiece();
    this->holdTetromino = nullptr;
    
    // play music
    getMS()->playBackgroundMusic();
}


// create current tetromino
void GameBoard::createCurrentTetro()
{
    auto tetro = Tetromino::create((TETROMINO_TYPE)RandomTetro::getInstance()->getItemInList(), false);
    // set state for tetro
    tetro->setInitStateToTetro();
    this->addChild(tetro);
    this->setCurTetromino(tetro);
    this->setCurrentTetroType(tetro->getType());
}


// create next tetromino
void GameBoard::createNextTetro()
{
    auto preTetro = Tetromino::create((TETROMINO_TYPE)RandomTetro::getInstance()->getItemInList(), false);
    preTetro->setInitStateToPre();
    this->addChild(preTetro);
    this->setNextTetromino(preTetro);
}

// create ghost tetromino
void GameBoard::createGhostPiece()
{
    auto ghost = Tetromino::create(currentTetromino->getType(), true);
    this->setGhostTetromino(ghost);
    ghost->setInitStateToGhost();
    this->addChild(ghost);
    ghost->getHaunted();
}

// create hold piece
void GameBoard::createHoldPiece()
{
    auto tetro = Tetromino::create(getCurrentTetroType(), false);
    this->setHoldTetromino(tetro);
    holdTetromino->setInitStateToHold();
    this->addChild(tetro);
}

bool GameBoard::isPointOccupied(cocos2d::Point point)
{
    return playFieldVector[point.y][point.x]->getBlockDefinition()._isOccupOrEmpty;
}

TETROMINO_TYPE GameBoard::getTetroType()
{
    return this->getCurrentTetroType();
}

void GameBoard::gameOver()
{
    getHub()->removeTouch();
    currentTetromino->overGame();
    CCLOG("game over");
    animationGameOver();
}

void GameBoard::animationGameOver()
{
    // about 1s animation (0.05 x 20)
    Vector<FiniteTimeAction*> vector_action;
    for (int y = 0; y <= FIELD_TOP; y++)
    {
        Vector<FiniteTimeAction*> oneRow;
        for (int x = 0; x <= FIELD_RIGHT_BOARD; x++)
        {
            if (!playFieldVector[y][x]->isMeEmpty())
            {
                auto oneBlock = Sequence::create(CCCallFunc::create(CC_CALLBACK_0(BlockImage::switchDarkShowing, playFieldVector[y][x])), nullptr);
                oneRow.pushBack(oneBlock);
            }
        }
        auto spawnOneRow = Spawn::create(oneRow);
        auto sequence = Sequence::create(spawnOneRow, DelayTime::create(0.05f) , nullptr);
        vector_action.pushBack(sequence);
    }
    // run animation overgame
    auto action = Sequence::create(vector_action);
//    auto removeTetro = CallFunc::create(CC_CALLBACK_0(Node::removeFromParentAndCleanup, currentTetromino, true));
//    auto removeGhost = CallFunc::create(CC_CALLBACK_0(Node::removeFromParentAndCleanup, ghostTetromino, true));
    auto nextOverBoard = CallFunc::create(CC_CALLBACK_0(ManageScene::changeState,ManageScene::getInstance() ,GAME_STATE::OVER));
    
    auto sequence = Sequence::create(action, CallFunc::create(CC_CALLBACK_0(GameBoard::removeTetroWhenGameover, this)), nextOverBoard, NULL);
    this->runAction(sequence);
}

void GameBoard::removeTetroWhenGameover()
{
    currentTetromino->removeAllChildrenWithCleanup(true);
    ghostTetromino->removeAllChildrenWithCleanup(true);
    currentTetromino = nullptr;
    ghostTetromino = nullptr ;
}

void GameBoard::setCurTop(int top)
{
    this->curTop = top;
}

int GameBoard::getCurTop()
{
    return this->curTop;
}

void GameBoard::checkClear()
{
    // play sound
    getMS()->playLockOnSound();
    
    for (int i = 0; i < 4; i++) {
        comboVector[i] = -1;
    }
    int index = 0;
    
    for (int y = 0; y <= getCurTop(); y++) {
        for (int x = 0; x <= FIELD_RIGHT_BOARD; x++) {
            if (playFieldVector[y][x]->isMeEmpty()) {
                break;
            }
            if (x == FIELD_RIGHT_BOARD) {
                // which line is full
                comboVector[index++] = y;
            }
        }
    }
    
    // no line found
    if (index == 0)
    {
        return;
    }
    else
    {
        // clear line
        clearLine(comboVector);
    }
}

// remove lines full
void GameBoard::clearLine(std::vector<int> &lineVector)
{
    Vector<FiniteTimeAction*> vector_action;
    int combo = 0;
    
    for (int i = 0; i < 4; i++) {
        if (lineVector[i] == -1) {
            continue;
        }
        else
        {
            combo++;
            int tem = lineVector[i];
            tem >>= 1;
            tem <<= 1;
            if (tem == lineVector[i])
            {
                for (int x = FIELD_LEFT_BOARD; x <= FIELD_RIGHT_BOARD; x++)
                {
                    auto action = Sequence::create(DelayTime::create(0.02f), CCCallFunc::create(CC_CALLBACK_0(BlockImage::switchShowing, playFieldVector[lineVector[i] - (combo - 1)][x])), nullptr);
                    vector_action.pushBack(action);
                }
            }
            else
            {
                for (int x = FIELD_RIGHT_BOARD; x >= FIELD_LEFT_BOARD; x--)
                {
                    auto action = Sequence::create(DelayTime::create(0.02f), CCCallFunc::create(CC_CALLBACK_0(BlockImage::switchShowing, playFieldVector[lineVector[i] - (combo - 1)][x])), nullptr);
                    vector_action.pushBack(action);
                }
            }
            
            auto fall = CCCallFunc::create(CC_CALLBACK_0(GameBoard::fallLine, this, lineVector[i] - (combo - 1)));
            vector_action.pushBack(fall);
        }
    }
    
    auto action = Sequence::create(vector_action);
    this->runAction(action);
    
    // add to hub
    getHub()->addLineHub(combo);
    
    // show won lines
    switch((NUM_LINE)combo)
    {
        case NUM_LINE::ONE_LINE : { showWonLines("Single line"); }
            break;
        case NUM_LINE::TWO_LINE: { showWonLines("Double lines"); }
            break;
        case NUM_LINE::THREE_LINE: {showWonLines("Triple lines");}
            break;
        case NUM_LINE::FOUR_LINE: {showWonLines("Incredible!");}
            break;
        default:
            showWonLines("Incredible lines");
            break;
    }
    
    // check when maximum speed
}

void GameBoard::showWonLines(std::string text)
{
    auto label = Label::createWithTTF(text, "kenpixel_future_square.ttf", 30);
    label->setPosition(BLOCK_WIDTH * BOARD_WIDTH / 2, BOARD_HEIGHT / 2 * BLOCK_WIDTH);
    label->setColor(Color3B::RED);
    this->addChild(label);
    
    auto scaleBig = ScaleTo::create(0.5, 1.05);
    auto scaleSmall = ScaleTo::create(0.5, 0.0);
    auto fadeOut = FadeOut::create(0.5);
    auto spawn = Spawn::create(scaleSmall,  fadeOut, nullptr);
    auto sequence = Sequence::create(Spawn::create(FadeIn::create(0.5f), scaleBig, NULL), spawn, RemoveSelf::create(), nullptr);
    
    label->runAction(sequence);
}

// fall line after clear line
void GameBoard::fallLine(int line)
{
    // fall all tetro above line was clear
    for (int y = line; y <= getCurTop(); y++) {
        for (int x = 0; x <= FIELD_RIGHT_BOARD; x++) {
            BlockDef def = playFieldVector[y+1][x]->getBlockDefinition();
            // all block down 1
            def._coordinateY -= 1;
            // redraw
            playFieldVector[y][x]->reShowing(def);
        }
    }
    
    this->setCurTop(getCurTop() - 1);
    this->ghostTetromino->getHaunted();
    this->currentTetromino->checkAndStopLockOn();
    
    getMS()->playClearLineSound();
}

void GameBoard::switchShowingForCell(cocos2d::Point point)
{
    playFieldVector[point.y][point.x]->switchShowing();
}

void GameBoard::switchCurrentAndHoldTetromino()
{
    if (holdTetromino == nullptr)
    {
        createHoldPiece();
        this->switchCurrentAndNextTetromino();
    }
    else
    {
        this->currentTetromino->switchState(TETROMINO_STATE::HOLD);
        this->holdTetromino->switchState(TETROMINO_STATE::SHOW);
        auto tem = this->currentTetromino;
        
        this->setCurTetromino(holdTetromino);
        this->setHoldTetromino(tem);
        
        this->setCurrentTetroType(getCurTetromino()->getType());
        
        this->ghostTetromino->getHaunted();
    }
}

void GameBoard::switchCurrentAndNextTetromino()
{
    //TODO check change tetromino
    if (getCurTop() == FIELD_TOP)
    {
        if (!checkCreateNextTetro())
        {
            gameOver();
            return;
        }
    }
    
    this->currentTetromino->switchState(TETROMINO_STATE::PRE);
    this->nextTetromino->switchState(TETROMINO_STATE::SHOW);
    
    auto tem = this->currentTetromino;
    
//    if (curTop >= FIELD_TOP)
//    {
//        this->nextTetromino->setPosition(CUR_POINT_OTHER_TYPE_);
//    }
    this->setCurTetromino(nextTetromino);
    this->setNextTetromino(tem);
    
    this->setCurrentTetroType(getCurTetromino()->getType());
    
    this->ghostTetromino->getHaunted();
}

bool GameBoard::checkCreateNextTetro()
{
    TETROMINO_TYPE type = this->nextTetromino->getType();
    bool* checkArray = new bool[10];
    for (int i = 0; i <= FIELD_RIGHT_BOARD; i++)
    {
        checkArray[i] = false;
    }
    CCLOG("next tetro %d", type);
    for (int x = 0; x <= FIELD_RIGHT_BOARD; x++)
    {
        BlockDef def = playFieldVector[FIELD_TOP][x]->getBlockDefinition();
        if (def._isOccupOrEmpty)
        {
            checkArray[x] = true;
        }
        CCLOG("x=%d y=%d: _isOccupOrEmpty:%s", def._coordinateX, def._coordinateY, def._isOccupOrEmpty == true ? "true" : "false");
    }
        
    // checking
    return checkFollowTetroType(type, checkArray);
}

bool GameBoard::checkFollowTetroType(TETROMINO_TYPE type, bool* checkArray)
{
    for (int i = 0; i <= FIELD_RIGHT_BOARD; i++)
    {
        CCLOG("checkFollowTetrotype %s",checkArray[i] ? "true" : "false");
    }
    if (checkArray[4])
    {
        return false;
    }
    switch (type) {
        case  TETROMINO_TYPE::O :
            if (checkArray[5])
            {
                return false;
            }
            break;
            
        case  TETROMINO_TYPE::I :
            if (checkArray[3] || checkArray[5] || checkArray[6])
            {
                return false;
            }
            break;
            
        case  TETROMINO_TYPE::T :
            if (checkArray[3] || checkArray[5])
            {
                return false;
            }
            break;
            
        case  TETROMINO_TYPE::L :
            if (checkArray[3] || checkArray[5])
            {
                return false;
            }
            break;
            
        case  TETROMINO_TYPE::J :
            if (checkArray[3] || checkArray[5])
            {
                return false;
            }
            break;
            
        case  TETROMINO_TYPE::Z :
            if (checkArray[5])
            {
                return false;
            }
            break;
            
        case  TETROMINO_TYPE::S :
            if (checkArray[3])
            {
                return false;
            }
            break;
            
        default:
            break;
    }
    return true;
}

// TOUCH FUNCTION

void GameBoard::moveLeft(int step)
{
    if (step <= 0)
    {
        return;
    }
    currentTetromino->onLeft(step);
    // move ghost
    ghostTetromino->getHaunted();
}

void GameBoard::moveRight(int step)
{
    if (step <= 0)
    {
        return;
    }
    currentTetromino->onRight(step);
    // move ghost
    ghostTetromino->getHaunted();
}

void GameBoard::moveDown(int step)
{
    if (step <= 0)
    {
        return;
    }
    currentTetromino->onDown(step);
}

void GameBoard::moveRotate()
{
    this->getMS()->playRotateSound();
    currentTetromino->onRotate();
    // move ghost
    ghostTetromino->getHaunted();
}

void GameBoard::moveHardDrop()
{
    this->getMS()->playHardDropSound();
    currentTetromino->onHardDrop();
    shakingBoard();
}

void GameBoard::moveSoftDropStart()
{
    this->getMS()->playMoveSound();
    bool isSoftDroping = currentTetromino->isFalling();
    if (isSoftDroping) {
        this->currentTetromino->stopFalling();
    }
    else return;
}

void GameBoard::moveSoftDropStop()
{
    this->getMS()->playMoveSound();
    bool isSoftDroping = currentTetromino->isFalling();
    if (!isSoftDroping)
    {
        this->currentTetromino->startFalling();
    }
    else return;
}


// add current tetro into Hold
void GameBoard::onHold()
{
    this->getMS()->playMoveSound();
    switchCurrentAndHoldTetromino();
}

// touch button pause
void GameBoard::onPause()
{
    this->getMS()->playClickSound();
    this->setIsPaused(!getIsPaused());
    if (getIsPaused())
    {
        this->currentTetromino->pause();
        this->getMS()->pauseBackgroundMusic();
        // call pause layer
        this->getpauseLayer()->pauseLayerCallback();
    }
    else
    {
        this->currentTetromino->resume();
        this->getMS()->resumeBackgroundMusic();
    }
}

// pause game when home button or something else

void GameBoard::pauseGame(cocos2d::Ref *sender)
{
    if(this->currentTetromino == nullptr)
        return;
    this->setIsPaused(true);
    this->currentTetromino->pause();
    this->getMS()->pauseBackgroundMusic();
    getHub()->onPauseCallback();
    // call pause layer
    this->getpauseLayer()->pauseLayerCallback();

}

void GameBoard::resumeGame(cocos2d::Ref *sender)
{
    this->setIsPaused(false);
    this->currentTetromino->resume();
    this->getMS()->resumeBackgroundMusic();
    getHub()->onResumeCallback();
}

