//
//  MiniBlock.cpp
//  GameTetris
//
//  Created by Nguyen Minh Hoang on 2015/03/19.
//
//

#include "MiniBlock.h"

bool MiniBlock::init()
{
    if(!Sprite::init())
    {
        return false;
    }
    return true;
}

void MiniBlock::createByDef(const BlockDef &block_def)
{
    float posX = (block_def._coordinateX) * BLOCK_WIDTH;
    float posY = (block_def._coordinateY) * BLOCK_HEIGHT;
    
    this->setPosition(posX, posY);
    this->initWithSpriteFrameName(getBlockFileName());
    this->setAnchorPoint(Vec2::ZERO);
    
}

bool MiniBlock::turnDownAble()
{
    //TODO
    Point curPosition = this->toBoardCoordinate();
    Point newPosition = Point(curPosition.x, curPosition.y - 1);
    if (newPosition.y >= FIELD_BOTTOM && newPosition.y <= FIELD_TOP)
    {

        bool isMoveable = !(this->getGameBoard()->isPointOccupied(newPosition));
        
        return isMoveable;
    }
    else if (newPosition.y < FIELD_BOTTOM)
    {
        if (getBlockDefinition()._isOccupOrEmpty == OCCUPIED)
        {
            return false;
        }
        else
            return true;
    }
    else if (newPosition.y > FIELD_TOP)
    {
        return true;
    }
}


bool MiniBlock::turnLeftAble()
{
    Point curPosition = this->toBoardCoordinate();
    Point newPosition = Point(curPosition.x - 1 , curPosition.y);
    
    if( newPosition.x >= FIELD_LEFT_BOARD && newPosition.y <= FIELD_TOP)
    {
        bool isMoveAble = !(this->getGameBoard()->isPointOccupied(newPosition));
        return isMoveAble;
    }
    else if(newPosition.x < FIELD_LEFT_BOARD)
    {
        if (getBlockDefinition()._isOccupOrEmpty == OCCUPIED)
        {
            return false;
        }
        else return true;
    }
    else return true;
}

bool MiniBlock::turnRightAble()
{
    Point curPosition = this->toBoardCoordinate();
    Point newPosition = Point(curPosition.x + 1 , curPosition.y);
    
    if (newPosition.x <= FIELD_RIGHT_BOARD && newPosition.y <= FIELD_TOP) {
        bool isMoveAble = !(this->getGameBoard()->isPointOccupied(newPosition));
        return isMoveAble;
    }
    else if (newPosition.x > FIELD_RIGHT_BOARD)
    {
        if (getBlockDefinition()._isOccupOrEmpty == OCCUPIED)
        {
            return false;
        }
        else return true;
    }
    else return true;
}

bool MiniBlock::turnLeftAbleStep(int step)
{
//    Point curPosition = this->toBoardCoordinate();
//    Point newPosition = Point(curPosition.x - step , curPosition.y);
//    
//    if( newPosition.x >= FIELD_LEFT_BOARD && newPosition.y <= FIELD_TOP)
//    {
//        bool isMoveAble = !(this->getGameBoard()->isPointOccupied(newPosition));
//        return isMoveAble;
//    }
//    else if(newPosition.x < FIELD_LEFT_BOARD)
//    {
//        if (getBlockDefinition()._isOccupOrEmpty == OCCUPIED)
//        {
//            return false;
//        }
//        else return true;
//    }
    
    Point curPosition = this->toBoardCoordinate();
    bool leftAble = false;
    for (int i = step ; i >= 1; i--)
    {
        Point newPosition = Point(curPosition.x - 1 , curPosition.y);
        if( newPosition.x >= FIELD_LEFT_BOARD && newPosition.y <= FIELD_TOP)
        {
            leftAble = !(this->getGameBoard()->isPointOccupied(newPosition));
            if (!leftAble)
            {
                return false;
            }
        }
        else if(newPosition.x < FIELD_LEFT_BOARD)
        {
            if (getBlockDefinition()._isOccupOrEmpty == OCCUPIED)
            {
                return false;
            }
            else
            {
                leftAble = true;
            }
        }

        else leftAble = true;
        
        if (leftAble)
        {
            curPosition.x -= 1;
        }
        
    }
 
    if (!leftAble)
    {
        return false;
    }
    else
        return true;
}

bool MiniBlock::turnRightAbleStep(int step)
{
//    Point curPosition = this->toBoardCoordinate();
//    Point newPosition = Point(curPosition.x + step , curPosition.y);
//    
//    if (newPosition.x <= FIELD_RIGHT_BOARD && newPosition.y <= FIELD_TOP) {
//        bool isMoveAble = !(this->getGameBoard()->isPointOccupied(newPosition));
//        return isMoveAble;
//    }
//    else if (newPosition.x > FIELD_RIGHT_BOARD)
//    {
//        if (getBlockDefinition()._isOccupOrEmpty == OCCUPIED)
//        {
//            return false;
//        }
//        else return true;
//    }
    Point curPosition = this->toBoardCoordinate();
    bool rightAble = false;
    for (int i = 1 ; i <= step; i++)
    {
        Point newPosition = Point(curPosition.x + 1 , curPosition.y);
        if( newPosition.x <= FIELD_RIGHT_BOARD && newPosition.y <= FIELD_TOP)
        {
            rightAble = !(this->getGameBoard()->isPointOccupied(newPosition));
            if (!rightAble)
            {
                return false;
            }
        }
        else if(newPosition.x > FIELD_RIGHT_BOARD)
        {
            if (getBlockDefinition()._isOccupOrEmpty == OCCUPIED)
            {
                return false;
            }
            else
            {
                rightAble = true;
            }
        }
//        else if (newPosition.y > FIELD_TOP+1 )
//        {
//            return false;
//        }
        else
            rightAble = true;
        
        if (rightAble)
        {
            curPosition.x += 1;
        }
        
    }
    
    if (!rightAble)
    {
        return false;
    }
    else
        return true;
}

bool MiniBlock::turnRotateAble(int x, int y)
{
    Point curPosition = this->toBoardCoordinate();
    curPosition.x += x;
    curPosition.y += y;
    //if (curPosition.x < 0 || curPosition.x > 9 || curPosition.y < 0 || curPosition.y > 19)
    if (curPosition.x < FIELD_LEFT_BOARD || curPosition.x > FIELD_RIGHT_BOARD || curPosition.y < FIELD_BOTTOM)
    {
        return false;
    }
    else if (curPosition.y > FIELD_TOP)
    {
        return true;
    }
    else
    {
        return !(this->getGameBoard()->isPointOccupied(curPosition));
    }
}

bool MiniBlock::isMeEmpty()
{
    return !(this->getBlockDefinition()._isOccupOrEmpty);
}

Point MiniBlock::getParentCenter()
{
    if (blockDef._isFieldOrTetro == TETROMINO)
    {
        return parentCenter;
    }
    else
        return Point(Point::ZERO);
}

void MiniBlock::setParentCenter()
{
    Point p_position = this->getParent()->getPosition();
    float p_position_x = p_position.x + BLOCK_WIDTH * 2;
    float p_position_y = p_position.y + BLOCK_HEIGHT * 2;
    
    int center_x = p_position_x / BLOCK_WIDTH;
    int center_y = p_position_y / BLOCK_WIDTH;
    
    parentCenter = (Point(center_x, center_y));
}

BoardInterface* MiniBlock::getGameBoard()
{
    if (blockDef._isFieldOrTetro == PLAYFIELD)
    {
        return (dynamic_cast<BoardInterface*>(this->getParent()));
    }
    else
        return (dynamic_cast<BoardInterface*>((this->getParent())->getParent()));
}

Point MiniBlock::toBoardCoordinate()
{
    this->setParentCenter();
    Point center = this->getParentCenter();
//    CCLOG("%f,%f,my getParentCenter", center.x, center.y);
    int coodinate_x = getBlockDefinition()._coordinateX < 2 ?
    center.x - (2 - getBlockDefinition()._coordinateX) :
    center.x + (getBlockDefinition()._coordinateX - 2);
    int coodinate_y = getBlockDefinition()._coordinateY < 2 ?
    center.y + (getBlockDefinition()._coordinateY - 2) :
    center.y - (2 - getBlockDefinition()._coordinateY);
    
//    CCLOG("%d,%d,my coodinate", coodinate_x, coodinate_y);
    return Point(coodinate_x, coodinate_y);
}

void MiniBlock::switchShowing()
{
    this->resetOccupySituation();
    this->resetShowingSituation();
    
    std::string file_name = this->getBlockFileName();
    auto newFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(file_name);
    this->setSpriteFrame(newFrame);
}

void MiniBlock::switchDarkShowing()
{
    this->resetOccupySituation();
    this->resetShowingSituation();
    
    std::string file_name = "block_dark.png";
    auto newFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(file_name);
    this->setSpriteFrame(newFrame);
}

void MiniBlock::resetOccupySituation()
{
    auto newDef = this->getBlockDefinition();
    newDef._isOccupOrEmpty = !newDef._isOccupOrEmpty;
    this->setBlockDefinition(newDef);
}

void MiniBlock::resetShowingSituation()
{
    auto curDef = this->getBlockDefinition();
    
    if (curDef._isOccupOrEmpty == OCCUPIED)
    {
        curDef._tetroType = getGameBoard()->getTetroType();
        this->setBlockDefinition(curDef);
        
    }
    else if (curDef._isOccupOrEmpty == EMPTY)
    {
        curDef._tetroType = TETROMINO_TYPE::NOTHING;
        this->setBlockDefinition(curDef);
    }
}