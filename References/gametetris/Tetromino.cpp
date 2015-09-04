//
//  Tetromino.cpp
//  GameTetris
//
//  Created by Nguyen Minh Hoang on 2015/03/20.
//
//

#include "Tetromino.h"

bool Tetromino::init()
{
    if (!Sprite::init()) {
        return false;
    }
    
    this->setMS(ManageSound::getInstance());
    
    initBlocks();

    return true;
}

Tetromino* Tetromino::create(TETROMINO_TYPE type , bool ghost)
{
    Tetromino* tetromino = new Tetromino(type, ghost);
    if(tetromino && tetromino->init())
    {
        tetromino->autorelease();
        return tetromino;
    }
    else{
        CC_SAFE_DELETE(tetromino);
        tetromino = NULL;
        return NULL;
    }
}

Tetromino::Tetromino(TETROMINO_TYPE type, bool isGhost)
{
    this->setContentSize(Size(BLOCK_WIDTH * PIECE_SIZE, BLOCK_HEIGHT * PIECE_SIZE));
    shapesVector.resize(SHAPE_SIZE);
    blocksVector.resize(PIECE_SIZE);
    for (int i = 0; i < PIECE_SIZE; i++)
        blocksVector[i].resize(PIECE_SIZE);
    
    
    //init shape for tetromino
    setShapesByType(type);
    
    //set ghost
    this->setIsGhost(isGhost);
    
}

Tetromino::~Tetromino()
{
    
}

void Tetromino::initBlocks()
{
    char16_t mask = 0x8000;
    TETROMINO_TYPE type =  getType();
    for (int y = PIECE_SIZE - 1; y >= 0; y--)
    {
        for (int x = 0; x < PIECE_SIZE; x++)
        {
            shapeMatrix[y][x] = (bool)(mask & shapesVector[curShape]);
            mask = mask >> 1;
            if (getIsGhost())
            {
                BlockDef blockDef = { TETROMINO, shapeMatrix[y][x], type, x, y, true };
                auto blockCell = BlockGhost::create(blockDef);
                this->addChild(blockCell);
                blocksVector[y][x] = blockCell;
            }
            else
            {
                BlockDef blockDef = { TETROMINO, shapeMatrix[y][x], type, x, y ,false};
                auto blockCell = BlockImage::create(blockDef);
                this->addChild(blockCell);
                blocksVector[y][x] = blockCell;
            }
        }
    }
}

void Tetromino::setShapesByType(TETROMINO_TYPE type)
{
    this->setType(type);
    switch (type) {
        case TETROMINO_TYPE::O :
        {
            char16_t o_shape[SHAPE_SIZE] = { 0x6600, 0x6600, 0x6600, 0x6600 };
            //char16_t o_shape[4] = { 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF };
            for (int i = 0; i < SHAPE_SIZE; i++)
                this->shapesVector[i] = o_shape[i];
            break;
        }
           
        case TETROMINO_TYPE::I :
        {
            char16_t i_shape[SHAPE_SIZE] = { 0x0f00, 0x2222, 0x00f0, 0x4444 };
            for (int i = 0; i < SHAPE_SIZE; i++)
                this->shapesVector[i] = i_shape[i];
            break;
        }
            
        case TETROMINO_TYPE::T :
        {
            char16_t t_shape[SHAPE_SIZE] = { 0x4E00, 0x4640, 0x0E40, 0x4C40 };
            for (int i = 0; i < SHAPE_SIZE; i++)
                this->shapesVector[i] = t_shape[i];
            break;
        }
            
        case TETROMINO_TYPE::S :
        {
            char16_t s_shape[SHAPE_SIZE] = { 0x6C00, 0x4620, 0x06C0, 0x8C40 };
            for (int i = 0; i < SHAPE_SIZE; i++)
                this->shapesVector[i] = s_shape[i];
            break;
        }
            
        case TETROMINO_TYPE::Z :
        {
            char16_t z_shape[SHAPE_SIZE] = { 0xC600, 0x2640, 0x0C60, 0x4C80 };
            for (int i = 0; i < SHAPE_SIZE; i++)
                this->shapesVector[i] = z_shape[i];
            break;
        }
            
        case TETROMINO_TYPE::L :
        {
            char16_t l_shape[SHAPE_SIZE] = { 0x8E00, 0x6440, 0x0E20, 0x44C0 };
            for (int i = 0; i < SHAPE_SIZE; i++)
                this->shapesVector[i] = l_shape[i];
            break;
        }
            
        case TETROMINO_TYPE::J :
        {
            char16_t j_shape[SHAPE_SIZE] = { 0x2E00, 0x4460, 0x0E80, 0xC440 };
            for (int i = 0; i < SHAPE_SIZE; i++)
                this->shapesVector[i] = j_shape[i];
            break;
        }
            
        default:
            break;
    }
    
    this->curShape = 0;
}

// set state and position of tetromino
void Tetromino::setInitStateToTetro()
{
    this->setCurState(TETROMINO_STATE::SHOW);
    this->setPosition(CUR_POINT);
    this->setCurMobility(true);
    
    // start falling tetromino
    this->startFalling();
    
}

// set state and position of tetromino next
void Tetromino::setInitStateToPre()
{
    this->setCurState(TETROMINO_STATE::PRE);
    this->setCurMobility(false);
    this->setPosition(PRE_POINT);
}

// set state and position of ghost tetromino
void Tetromino::setInitStateToGhost()
{
    this->setCurState(TETROMINO_STATE::GHOST);
    this->setCurMobility(false);
    this->setPosition(0, 0);
}

// set state and position of hold tetromino
void Tetromino::setInitStateToHold()
{
    this->setCurState(TETROMINO_STATE::HOLD);
    this->setCurMobility(false);
    this->refreshShape(0);
    this->setPosition(HOLD_POINT);
   
    
    if (this->getType() != TETROMINO_TYPE::I && this->getType() != TETROMINO_TYPE::O)
    {
        this->setPositionX((BLOCK_WIDTH * BOARD_WIDTH) + BLOCK_WIDTH * 3 - BLOCK_WIDTH / 2);
    }
}


// change state
void Tetromino::switchState(TETROMINO_STATE next_state)
{
    if (curState == TETROMINO_STATE::SHOW && next_state == TETROMINO_STATE::PRE)
    {
        this->setInitStateToPre();
        this->stopFalling();
        this->reShowing();
        
        dynamic_cast<GameBoard*> (getParent())->getHub()->onHoldCallback();
    }
    else if (curState == TETROMINO_STATE::PRE && next_state == TETROMINO_STATE::SHOW)
    {
        setInitStateToTetro();
    }
    else if (curState == TETROMINO_STATE::SHOW && next_state == TETROMINO_STATE::HOLD)
    {
        setInitStateToHold();
        this->stopFalling();
        //this->reShowing();
    }
    else if (curState == TETROMINO_STATE::HOLD && next_state == TETROMINO_STATE::SHOW)
    {
        setInitStateToTetro();
    }
}

// redraw when next tetro
void Tetromino::reShowing()
{
    TETROMINO_TYPE type = (TETROMINO_TYPE)RandomTetro::getInstance()->getItemInList();
    this->setShapesByType(type);
    if (this->getType() != TETROMINO_TYPE::I && this->getType() != TETROMINO_TYPE::O)
    {
        this->setPositionX((BLOCK_WIDTH * BOARD_WIDTH) + BLOCK_WIDTH * 3 - BLOCK_WIDTH / 2);
    }
    
    char16_t mask = 0x8000;
    
    for (int y = PIECE_SIZE - 1; y >= 0; y--)
    {
        for (int x = 0; x < PIECE_SIZE; x++)
        {
            shapeMatrix[y][x] = (bool)(mask & shapesVector[curShape]);
            mask = mask >> 1;
            BlockDef blockDef = { TETROMINO, shapeMatrix[y][x], getType(), x, y };
            dynamic_cast<BlockImage*>(blocksVector[y][x])->reShowing(blockDef);
            
        }
    }
}

void Tetromino::refreshShape(int shape)
{
    char16_t mask = 0x8000;
    for (int y = PIECE_SIZE - 1; y >= 0; y--) {
        for (int x = 0; x < PIECE_SIZE; x++) {
            if (shapeMatrix[y][x] == static_cast<bool>(mask & shapesVector[shape])) {
                mask = mask >> 1;
                continue;
            }
            else
            {
                shapeMatrix[y][x] = (bool) (mask & shapesVector[shape]);
                blocksVector[y][x]->switchShowing();
            }
            mask = mask >> 1;
        }
    }
    this->setCurShape(shape);
}

void Tetromino::overGame()
{
    this->stopFalling();
}

bool Tetromino::isFalling()
{
    return isScheduled(CC_SCHEDULE_SELECTOR(Tetromino::fallingDown));
}

void Tetromino::startFalling()
{

    if (SPEED_GAME_DEFAULT - (0.09f * ManageScene::getInstance()->getSpeedLevel()) >= 0)
    {
        this->schedule(CC_SCHEDULE_SELECTOR(Tetromino::fallingDown), SPEED_GAME_DEFAULT - (0.09f * ManageScene::getInstance()->getSpeedLevel()));
    }
}

void Tetromino::stopFalling()
{
    unschedule(CC_SCHEDULE_SELECTOR(Tetromino::fallingDown));
}


void Tetromino::fallingDown(float time)
{
    if ( downAble() && getCurMobility())
    {
        this->setPositionY(this->getPositionY() - BLOCK_HEIGHT);
    }
    else
    {
        runLockDelay();
    }
}

void Tetromino::runLockDelay()
{
    auto delay = DelayTime::create(TIME_LOCK_DELAY);
    auto lockon = Sequence::create(CallFunc::create(CC_CALLBACK_0(Tetromino::stopFalling, this)), delay, CallFunc::create(CC_CALLBACK_0(Tetromino::lockOn, this)),nullptr);
    lockon->setTag(LOCK_DELAY_TAG);
    this->runAction(lockon);
}

// lock all tetro in board field
void Tetromino::lockOn()
{
    for (int y = PIECE_SIZE - 1; y >= 0; y--)
    {
        for (int x = 0; x < PIECE_SIZE; x++)
        {
            if (blocksVector[y][x]->isMeEmpty())
            {
                continue;
            }
            else
            {
                dynamic_cast<BlockImage*>(blocksVector[y][x])->lockOn();
            }
        }
    }

    // check game over
    if (dynamic_cast<GameBoard*> (getParent())->getCurTop() >= FIELD_TOP + 1)
    {
//        TETROMINO_TYPE typeNextTetro = dynamic_cast<GameBoard*> (getParent())->getNextTetromino()->getType();
//        if (typeNextTetro != TETROMINO_TYPE::I || dynamic_cast<GameBoard*> (getParent())->getCurTop()>=FIELD_TOP)
//        {
            dynamic_cast<GameBoard*> (getParent())->gameOver();
            return;
//        }
    }
 
    // check clear line
    dynamic_cast<GameBoard*> (getParent())->checkClear();
    
    // run next tetromino
    dynamic_cast<GameBoard*> (getParent())->switchCurrentAndNextTetromino();
    
}

// get Ghost tetromino
void Tetromino::getHaunted()
{
    if (!isGhost)
    {
        return;
    }
    TETROMINO_TYPE type = dynamic_cast<GameBoard*> (getParent())->getCurrentTetroType();
    int shape = dynamic_cast<GameBoard*> (getParent())->getCurTetromino()->curShape;
    if (this->getType() != type)
    {
        this->refreshType(type);
        this->refreshShape(shape);
    }
    
    if (shape!=this->curShape)
    {
        this->refreshShape(shape);
    }
    
    Point initPosition = dynamic_cast<GameBoard*> (getParent())->getCurTetromino()->getPosition();
    this->setPosition(initPosition.x, initPosition.y);
    while (downAble())
    {
        this->setPosition(initPosition.x, initPosition.y -= BLOCK_WIDTH);
    };
  
}

void Tetromino::refreshType(TETROMINO_TYPE type)
{
    this->setShapesByType(type);
}

void Tetromino::checkAndStopLockOn()
{
    if (downAble())
    {
        auto lock_on = this->getActionByTag(LOCK_DELAY_TAG);
        if (lock_on)
        {
            this->stopAction(lock_on);
            this->startFalling();
        }
    }
}

////// ABLE to left, right, down
bool Tetromino::downAble()
{
    for (int i = 0; i < PIECE_SIZE; i++) {
        for (int j = 0; j < PIECE_SIZE; j++) {
            if (blocksVector[j][i]->isMeEmpty()) {
                continue;
            }
            bool is_downable = this->blocksVector[j][i]->turnDownAble();
            if (is_downable) {
                break;
            }
            else if(!is_downable)
            {
                return false;
            }
        }
    }
    return true;
}

bool Tetromino::leftAble(int step)
{
    for (int y = PIECE_SIZE - 1; y >= 0; y--)
    {
        for (int x = 0; x < PIECE_SIZE; x++)
        {
            if (blocksVector[y][x]->isMeEmpty())
            {
                continue;
            }
            bool is_leftable = this->blocksVector[y][x]->turnLeftAbleStep(step);
            if (is_leftable)
            {
                break;
            }
            else if (!is_leftable)
            {
                return false;
            }
        }
    }
    return true;
}

bool Tetromino::rightAble(int step)
{
    for (int y = PIECE_SIZE - 1; y >= 0; y--)
    {
        for (int x = PIECE_SIZE - 1; x >= 0; x--)
        {
            if (blocksVector[y][x]->isMeEmpty())
            {
                continue;
            }
            bool is_rightable = this->blocksVector[y][x]->turnRightAbleStep(step);
            if (is_rightable)
            {
                break;
            }
            else if (!is_rightable)
            {
                return false;
            }
        }
    }
    return true;
}

bool Tetromino::rotateAble()
{
    int temShape = curShape + 1;
    if (temShape == 4)
    {
        temShape = 0;
    }
    char16_t mask = 0x8000;
    for (int y = PIECE_SIZE - 1; y >= 0; y--)
    {
        for (int x = 0; x < PIECE_SIZE; x++)
        {
            if (shapeMatrix[y][x] == (bool)(mask & shapesVector[temShape]))
            {
                mask = mask >> 1;
                continue;
            }
            else
            {
                bool is_rotateable = blocksVector[y][x]->turnRotateAble();
                if (!is_rotateable)
                {
                    return false;
                }
            }
            mask = mask >> 1;
        }
    }
    return true;
}

void Tetromino::doRotation()
{
    curShape++;
    if (curShape == 4)
    {
        curShape = 0;
    }
    
    char16_t mask = 0x8000;
    
    for (int y = PIECE_SIZE - 1; y >= 0; y--)
    {
        for (int x = 0; x < PIECE_SIZE; x++)
        {
            if (shapeMatrix[y][x] == static_cast<bool>(mask & shapesVector[curShape]))
            {
                mask = mask >> 1;
                continue;
            }
            else
            {
                shapeMatrix[y][x] = (bool)(mask & shapesVector[curShape]);
                
                blocksVector[y][x]->switchShowing();
            }	
            mask = mask >> 1;
        }
    }
}

void Tetromino::instantLockon()
{
    auto lock_on = this->getActionByTag(LOCK_DELAY_TAG);
    if (lock_on)
    {
        this->stopAction(lock_on);
        
    }
    this->lockOn();
}

// turn down, left, right, rotate
void Tetromino::onLeft(int step)
{
	if (isFalling())
	{
		 if (!downAble())
		    {
//		        instantLockon();
//		        return;
		    }
	} else return;

    float newPosX = this->getPositionX() - step * BLOCK_WIDTH;
    if (leftAble(step))
    {
        this->getMS()->playMoveLeftRightSound();
        this->setPositionX(newPosX);
        this->checkAndStopLockOn();
    }
    
}// turn left tetro

void Tetromino::onRight(int step)
{
	if (isFalling())
	{
		if (!downAble())
		{
//			instantLockon();
//			return;
		}
	} else return;

    float newPosX = this->getPositionX() + step * BLOCK_WIDTH;
    if (rightAble(step))
    {
        this->getMS()->playMoveLeftRightSound();
        this->setPositionX(newPosX);
        this->checkAndStopLockOn();
    }
    
}// turn right

void Tetromino::onRotate()
{
    if (isFalling())
    {
        if (!downAble())
        {
            instantLockon();
            return;
        }
    } else return;

    bool is_rotated = RotationTetromino::getInstance()->doRotation(this);
    if (is_rotated)
    {
        this->checkAndStopLockOn();
    }
    
}// turn rotate

void Tetromino::onDown(int step)
{
    bool downable = false;
    for (int i = 1; i <= step; i++)
    {
        downable = downAble();
        if (downable)
        {
            this->getMS()->playMoveLeftRightSound();
            this->setPositionY(this->getPositionY() - BLOCK_WIDTH);
        }
        else
            break;
    }
   
    if (!downable)
    {
//        CCLOG("instant lock ondown");
//        instantLockon();
    }
    
}// turn down

void Tetromino::onHardDrop()
{
    if (isFalling())
    {
        if (!downAble())
        {
            instantLockon();
            return;
        }
    } else return;
    Point newPosition = dynamic_cast<GameBoard*> (getParent())->getGhostTetromino()->getPosition();
    this->setPosition(newPosition);
    instantLockon();
}// turn hard drop

void Tetromino::onHold()
{
    
}// push hold

bool Tetromino::isMoveAble()
{
    return this->getCurMobility();
}

